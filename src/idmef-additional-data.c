/*****
*
* Copyright (C) 2004 Yoann Vandoorselaere <yoann@prelude-ids.org>
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
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "libmissing.h"
#include "prelude-log.h"
#include "prelude-string.h"
#include "prelude-inttypes.h"
#include "idmef.h"
#include "idmef-tree-wrap.h"
#include "idmef-additional-data.h"
#include "idmef-message-id.h"


#define IDMEF_ADDITIONAL_DATA_DECL(d_type, d_name, ad_type, c_type, name)		\
int idmef_additional_data_new_ ## name(idmef_additional_data_t **ret, c_type val)	\
{                                                                                       \
        int retval;                                                                     \
											\
	retval = idmef_additional_data_new(ret);		       			\
	if ( retval < 0 )					      			\
		return retval;								\
											\
        idmef_additional_data_set_type(*ret, ad_type);					\
	idmef_data_set_ ## d_name(idmef_additional_data_get_data(*ret), val);		\
											\
	return retval;									\
}											\
											\
void idmef_additional_data_set_ ## name(idmef_additional_data_t *ptr, c_type val)	\
{											\
        idmef_additional_data_set_type(ptr, ad_type);					\
	idmef_data_set_ ## d_name(idmef_additional_data_get_data(ptr), val);		\
}											\
											\
c_type idmef_additional_data_get_ ## name(idmef_additional_data_t *ptr)			\
{											\
	return idmef_data_get_ ## d_name(idmef_additional_data_get_data(ptr));	        \
}


IDMEF_ADDITIONAL_DATA_DECL(IDMEF_DATA_TYPE_FLOAT, float, IDMEF_ADDITIONAL_DATA_TYPE_REAL, float, real)
IDMEF_ADDITIONAL_DATA_DECL(IDMEF_DATA_TYPE_UINT32, uint32, IDMEF_ADDITIONAL_DATA_TYPE_INTEGER, uint32_t, integer)
IDMEF_ADDITIONAL_DATA_DECL(IDMEF_DATA_TYPE_BYTE, byte, IDMEF_ADDITIONAL_DATA_TYPE_BOOLEAN, prelude_bool_t, boolean)
IDMEF_ADDITIONAL_DATA_DECL(IDMEF_DATA_TYPE_CHAR, char, IDMEF_ADDITIONAL_DATA_TYPE_CHARACTER, char, character)


static struct {
	idmef_additional_data_type_t ad_type;
	idmef_data_type_t d_type;
	size_t len;
} idmef_additional_data_type_table[] = {
	{ IDMEF_ADDITIONAL_DATA_TYPE_BOOLEAN, IDMEF_DATA_TYPE_BYTE, sizeof(prelude_bool_t) },
	{ IDMEF_ADDITIONAL_DATA_TYPE_BYTE, IDMEF_DATA_TYPE_BYTE, sizeof(uint8_t) },
	{ IDMEF_ADDITIONAL_DATA_TYPE_CHARACTER, IDMEF_DATA_TYPE_CHAR, sizeof(char) },
	{ IDMEF_ADDITIONAL_DATA_TYPE_DATE_TIME, IDMEF_DATA_TYPE_CHAR_STRING, 0 },
	{ IDMEF_ADDITIONAL_DATA_TYPE_INTEGER, IDMEF_DATA_TYPE_UINT32, sizeof(uint32_t) },
	{ IDMEF_ADDITIONAL_DATA_TYPE_NTPSTAMP, IDMEF_DATA_TYPE_UINT64, sizeof(uint64_t) },
	{ IDMEF_ADDITIONAL_DATA_TYPE_PORTLIST, IDMEF_DATA_TYPE_CHAR_STRING, 0 },
	{ IDMEF_ADDITIONAL_DATA_TYPE_REAL, IDMEF_DATA_TYPE_FLOAT, sizeof(float) },
	{ IDMEF_ADDITIONAL_DATA_TYPE_STRING, IDMEF_DATA_TYPE_CHAR_STRING, 0 },
	{ IDMEF_ADDITIONAL_DATA_TYPE_BYTE_STRING, IDMEF_DATA_TYPE_BYTE_STRING, 0 },
	{ IDMEF_ADDITIONAL_DATA_TYPE_XML, IDMEF_DATA_TYPE_CHAR_STRING, 0 }
};



static int check_type(idmef_additional_data_type_t type, const unsigned char *buf, size_t len)
{
        if ( type < 0 || type > sizeof(idmef_additional_data_type_table) / sizeof(idmef_additional_data_type_table[0]) )
                return -1;

        if ( idmef_additional_data_type_table[type].len != 0 &&
	     len != idmef_additional_data_type_table[type].len )
                return -1;

        if ( idmef_additional_data_type_table[type].len == 0 && len < 1 )
                return -1;

        if ( type == IDMEF_ADDITIONAL_DATA_TYPE_BYTE_STRING )
                return 0;

        return buf[len - 1] == '\0' ? 0 : -1;
}



static idmef_data_type_t idmef_additional_data_type_to_data_type(idmef_additional_data_type_t type)
{
        if ( type < 0 || type > sizeof(idmef_additional_data_type_table) / sizeof(idmef_additional_data_type_table[0]) )
                return IDMEF_DATA_TYPE_UNKNOWN;

	return idmef_additional_data_type_table[type].d_type;
}



int idmef_additional_data_new_ptr_ref_fast(idmef_additional_data_t **nd,
                                           idmef_additional_data_type_t type,
                                           const unsigned char *ptr, size_t len) 
{
        int ret;
        idmef_data_type_t dtype;
        
        ret = check_type(type, ptr, len);
        if ( ret < 0 )
                return ret;
                
        ret = idmef_additional_data_new(nd);
        if ( ret < 0 )
                return ret;

	idmef_additional_data_set_type(*nd, type);
        dtype = idmef_additional_data_type_to_data_type(type);
        
        ret = idmef_data_set_ptr_ref_fast(idmef_additional_data_get_data(*nd), dtype, ptr, len);
	if ( ret < 0 ) {
		idmef_additional_data_destroy(*nd);
		return ret;
	}

        return 0;
}



int idmef_additional_data_new_ptr_dup_fast(idmef_additional_data_t **nd,
                                           idmef_additional_data_type_t type,
                                           const unsigned char *ptr, size_t len) 
{
        int ret;
        idmef_data_type_t dtype;
        
        ret = check_type(type, ptr, len);
        if ( ret < 0 )
                return ret;
                
        ret = idmef_additional_data_new(nd);
        if ( ret < 0 )
                return ret;

	idmef_additional_data_set_type(*nd, type);
        dtype = idmef_additional_data_type_to_data_type(type);

        ret = idmef_data_set_ptr_dup_fast(idmef_additional_data_get_data(*nd), dtype, ptr, len);
	if ( ret < 0 ) {
		idmef_additional_data_destroy(*nd);
		return ret;
	}

        return 0;
}



int idmef_additional_data_new_ptr_nodup_fast(idmef_additional_data_t **nd,
                                             idmef_additional_data_type_t type,
                                             unsigned char *ptr, size_t len) 
{
        int ret;
        idmef_data_type_t dtype;
        
        ret = check_type(type, ptr, len);
        if ( ret < 0 )
                return ret;
                
        ret = idmef_additional_data_new(nd);
        if ( ret < 0 )
                return ret;

	idmef_additional_data_set_type(*nd, type);
        dtype = idmef_additional_data_type_to_data_type(type);

        ret = idmef_data_set_ptr_nodup_fast(idmef_additional_data_get_data(*nd), dtype, ptr, len);
	if ( ret < 0 ) {
		idmef_additional_data_destroy(*nd);
		return ret;
	}

        return ret;
}



int idmef_additional_data_set_ptr_ref_fast(idmef_additional_data_t *data,
                                           idmef_additional_data_type_t type, const unsigned char *ptr, size_t len)
{
        int ret;

        ret = check_type(type, ptr, len);
        if ( ret < 0 )
                return ret;

	idmef_additional_data_set_type(data, type);

	return idmef_data_set_ptr_ref_fast(idmef_additional_data_get_data(data),
					   idmef_additional_data_type_to_data_type(type), ptr, len);
}



int idmef_additional_data_set_ptr_dup_fast(idmef_additional_data_t *data,
                                           idmef_additional_data_type_t type, const unsigned char *ptr, size_t len)
{
        int ret;

        ret = check_type(type, ptr, len);
        if ( ret < 0 )
                return ret;

	idmef_additional_data_set_type(data, type);

	return idmef_data_set_ptr_dup_fast(idmef_additional_data_get_data(data),
					   idmef_additional_data_type_to_data_type(type), ptr, len);
}



int idmef_additional_data_set_ptr_nodup_fast(idmef_additional_data_t *data,
                                             idmef_additional_data_type_t type, unsigned char *ptr, size_t len)
{
        int ret;

        ret = check_type(type, ptr, len);
        if ( ret < 0 )
                return ret;

	idmef_additional_data_set_type(data, type);

	return idmef_data_set_ptr_nodup_fast(idmef_additional_data_get_data(data),
					     idmef_additional_data_type_to_data_type(type), ptr, len);
}



int idmef_additional_data_set_str_dup_fast(idmef_additional_data_t *data, idmef_additional_data_type_t type,
					   const char *str, size_t len)
{        
	idmef_additional_data_set_type(data, type);
	return idmef_data_set_char_string_dup_fast(idmef_additional_data_get_data(data), str, len);
						   
}



int idmef_additional_data_new_str_dup_fast(idmef_additional_data_t **nd,
                                           idmef_additional_data_type_t type, const char *str, size_t len)
{
        int ret;

	ret = idmef_additional_data_new(nd);
	if ( ret < 0 )
		return ret;

        ret = idmef_additional_data_set_str_dup_fast(*nd, type, str, len);
	if ( ret < 0 ) {
		idmef_additional_data_destroy(*nd);
		return ret;
	}

	return 0;
}



/*
 * just make a pointer copy of the embedded data
 */
int idmef_additional_data_copy_ref(idmef_additional_data_t *src, idmef_additional_data_t *dst)
{
        int ret;

        ret = prelude_string_copy_ref(idmef_additional_data_get_meaning(src), idmef_additional_data_get_meaning(dst));
        if ( ret < 0 )
                return ret;
        
	idmef_additional_data_set_type(dst, idmef_additional_data_get_type(src));

	return idmef_data_copy_ref(idmef_additional_data_get_data(src), idmef_additional_data_get_data(dst));
}



/*
 * also copy the content of the embedded data
 */
int idmef_additional_data_copy_dup(idmef_additional_data_t *src, idmef_additional_data_t *dst)
{
        int ret;

        ret = prelude_string_copy_dup(idmef_additional_data_get_meaning(src), idmef_additional_data_get_meaning(dst));
        if ( ret < 0 )
		return ret;

	idmef_additional_data_set_type(dst, idmef_additional_data_get_type(src));

	return idmef_data_copy_dup(idmef_additional_data_get_data(src), idmef_additional_data_get_data(dst));
}




int idmef_additional_data_clone(idmef_additional_data_t *src, idmef_additional_data_t **dst)
{
        int ret;
        
        ret = idmef_additional_data_new(dst);
        if ( ret < 0 )
                return ret;

        ret = idmef_additional_data_copy_dup(src, *dst);
	if ( ret < 0 ) {
		idmef_additional_data_destroy(*dst);
		return ret;
	}

        return ret;
}



size_t idmef_additional_data_get_len(idmef_additional_data_t *data)
{
        return idmef_data_get_len(idmef_additional_data_get_data(data));
}



prelude_bool_t idmef_additional_data_is_empty(idmef_additional_data_t *data)
{
        return idmef_data_is_empty(idmef_additional_data_get_data(data));
}



int idmef_additional_data_data_to_string(idmef_additional_data_t *ad, prelude_string_t *out)
{
        int ret;
        uint64_t i;
        idmef_data_t *data;

        data = idmef_additional_data_get_data(ad);
        if ( idmef_data_is_empty(data) )
                return 0;

        switch ( idmef_additional_data_get_type(ad) ) {

	case IDMEF_ADDITIONAL_DATA_TYPE_NTPSTAMP:
		i = idmef_data_get_uint64(data);
		ret = prelude_string_sprintf(out, "0x%08ux.0x%08ux", (unsigned long) (i >> 32), (unsigned long) i);
		break;

	default:
		ret = idmef_data_to_string(data, out);
                break;
        }

        return ret;
}
