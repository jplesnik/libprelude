/*****
*
* Copyright (C) 2002 Yoann Vandoorselaere <yoann@prelude-ids.org>
* All Rights Reserved
*
* This file is part of the Prelude program.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; see the file COPYING.  If not, write to
* the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*
*****/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "prelude-log.h"
#include "prelude-list.h"
#include "prelude-inttypes.h"
#include "prelude-linked-object.h"
#include "prelude-async.h"
#include "prelude-io.h"
#include "prelude-msg.h"
#include "prelude-msgbuf.h"


struct prelude_message_buffered {
        int flags;
        void *data;
        prelude_client_t *client;
        prelude_msg_t *msg;
        prelude_msg_t *(*send_msg)(prelude_msgbuf_t *msgbuf);
};



static prelude_msg_t *do_send_msg(prelude_msgbuf_t *msgbuf) 
{        
        prelude_client_send_msg(msgbuf->client, msgbuf->msg);
        prelude_msg_recycle(msgbuf->msg);

        return msgbuf->msg;
}



static prelude_msg_t *do_send_msg_async(prelude_msgbuf_t *msgbuf) 
{        
        prelude_client_send_msg(msgbuf->client, msgbuf->msg);
        
        msgbuf->msg = prelude_msg_dynamic_new((void *) msgbuf->send_msg, msgbuf);
        if ( ! msgbuf->msg ) {
                log(LOG_ERR, "memory exhausted.\n");
                return NULL;
        }

        return msgbuf->msg;
}



static prelude_msg_t *default_send_msg_cb(prelude_msgbuf_t *msgbuf)
{
        prelude_msg_t *msg;

        if ( prelude_client_get_flags(msgbuf->client) & PRELUDE_CLIENT_FLAGS_ASYNC_SEND )
                msg = do_send_msg_async(msgbuf);
        else
                msg = do_send_msg(msgbuf);
        
        return msg;
}



/**
 * prelude_msgbuf_set:
 * @msgbuf: Pointer on a #prelude_msgbuf_t object to store the data to.
 * @tag: 8 bits unsigned integer describing the kind of data.
 * @len: len of the data chunk.
 * @data: Pointer to the data.
 *
 * prelude_msgbuf_set() append @len bytes of data from the @data buffer
 * to the @msgbuf object representing a message. The data is tagged with @tag.
 */
void prelude_msgbuf_set(prelude_msgbuf_t *msgbuf, uint8_t tag, uint32_t len, const void *data)
{
        prelude_msg_set(msgbuf->msg, tag, len, data);
}




/**
 * prelude_msgbuf_new:
 * @client: Pointer to the #prelude_client_t to flush message to.
 *
 * Create a new #prelude_msgbuf_t object. You can then write data to
 * @msgbuf using the prelude_msgbuf_set function.
 *
 * When the message buffer is full, the message will be flushed to the
 * list of #prelude_connection_manager_t pointed to the @client object.
 *
 * Returns: a #prelude_msgbuf_t object, or NULL if an error occured.
 */
prelude_msgbuf_t *prelude_msgbuf_new(prelude_client_t *client)
{
        prelude_msgbuf_t *msgbuf;

        msgbuf = malloc(sizeof(*msgbuf));
        if ( ! msgbuf ) {
                log(LOG_ERR, "memory exhausted.\n");
                return NULL;
        }

        msgbuf->client = client;
        msgbuf->send_msg = default_send_msg_cb;
        
        msgbuf->msg = prelude_msg_dynamic_new((void *) msgbuf->send_msg, msgbuf);     
        if ( ! msgbuf->msg )
                return NULL;

        return msgbuf;
}




/**
 * prelude_msgbuf_get_msg:
 * @msgbuf: Pointer on a #prelude_msgbuf_t object.
 *
 * Returns: This function return the current message associated with
 * the message buffer.
 */
prelude_msg_t *prelude_msgbuf_get_msg(prelude_msgbuf_t *msgbuf)
{
        return msgbuf->msg;
}




/**
 * prelude_msgbuf_mark_end:
 * @msgbuf: Pointer on #prelude_msgbuf_t object.
 *
 * This function should be called to tell the msgbuf subsystem
 * that you finished writing your message.
 */
void prelude_msgbuf_mark_end(prelude_msgbuf_t *msgbuf) 
{
        prelude_msg_mark_end(msgbuf->msg);
        
        /*
         * FIXME:
         * only flush the message if we're not under an alert burst.
         */
        msgbuf->send_msg(msgbuf);
}




/**
 * prelude_msgbuf_close:
 * @msgbuf: Pointer on a #prelude_msgbuf_t object.
 *
 * Close the message associated with @msgbuf,
 * all data remaining will be flushed before closing it.
 */
void prelude_msgbuf_close(prelude_msgbuf_t *msgbuf) 
{        
        if ( msgbuf->msg && ! prelude_msg_is_empty(msgbuf->msg) )
                msgbuf->send_msg(msgbuf);

        if ( msgbuf->msg )
                prelude_msg_destroy(msgbuf->msg);

        free(msgbuf);
}




/**
 * prelude_msgbuf_set_callback:
 * @msgbuf: Pointer on a #prelude_msgbuf_t object.
 * @send_msg: Pointer to a function for sending a message.
 *
 * Associate an application specific callback to this @msgbuf.
 */
void prelude_msgbuf_set_callback(prelude_msgbuf_t *msgbuf, prelude_msg_t *(*send_msg)(prelude_msgbuf_t *msgbuf))
{
        msgbuf->send_msg = send_msg;
        prelude_msg_set_callback(msgbuf->msg, (void *) send_msg);
}




void prelude_msgbuf_set_data(prelude_msgbuf_t *msgbuf, void *data) 
{
        msgbuf->data = data;
}




void *prelude_msgbuf_get_data(prelude_msgbuf_t *msgbuf)
{
        return msgbuf->data;
}



prelude_client_t *prelude_msgbuf_get_client(prelude_msgbuf_t *msgbuf)
{
        return msgbuf->client;
}





