/*****
*
* Copyright (C) 2004, 2005 Yoann Vandoorselaere <yoann@prelude-ids.org>
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

/*
 * This code include an heavily modified version of the prelude-strbuf
 * API made by Krzysztof Zaraska <kzaraska@student.uci.agh.edu.pl>, that
 * is now part of prelude-string.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>

#include "common.h"
#include "libmissing.h"
#include "prelude-log.h"
#include "prelude-inttypes.h"
#include "prelude-string.h"

#define PRELUDE_ERROR_SOURCE_DEFAULT PRELUDE_ERROR_SOURCE_STRING
#include "prelude-error.h"


#define CHUNK_SIZE 1024


/*
 * String structure may be free'd
 */
#define PRELUDE_STRING_OWN_STRUCTURE  0x1

/*
 * String data may be free'd
 */
#define PRELUDE_STRING_OWN_DATA       0x2


/*
 * Whether we can reallocate this string
 */
#define PRELUDE_STRING_CAN_REALLOC    0x4


#define check_string(str, len) check_string_f(__FUNCTION__, __LINE__, (str), (len))


inline static int check_string_f(const char *f, int l, const char *str, size_t len)
{
        if ( str[len] != 0 ) {
                printf("str=%s\n", str);
                fprintf(stderr, "*** %s:%d: warning, string is not NULL terminated.\n", f, l);
                return prelude_error(PRELUDE_ERROR_STRING_NOT_NULL_TERMINATED);
        }

        return 0;
}


static int string_buf_alloc(prelude_string_t *string, size_t len)
{
        /*
         * include room for terminating \0.
         */
        string->data.rwbuf = malloc(len + 1);
        if ( ! string->data.rwbuf ) 
                return prelude_error_from_errno(errno);
        
        string->index = len;
        string->size = len + 1;
        
        return 0;
}



static void string_buf_copy(prelude_string_t *string, const char *buf, size_t len)
{
        assert(len < string->size);
        
        memcpy(string->data.rwbuf, buf, len);
        string->data.rwbuf[len] = '\0';
}



static int allocate_more_chunk_if_needed(prelude_string_t *s, size_t needed_len)
{
        char *ptr;
        size_t len;
        
        if ( needed_len )
                len = MAX(needed_len - (s->size - s->index), CHUNK_SIZE);
        else
                len = CHUNK_SIZE;
        
        if ( s->flags & PRELUDE_STRING_CAN_REALLOC ) {
                
                ptr = prelude_realloc(s->data.rwbuf, s->size + len);
                if ( ! ptr )
                        return prelude_error_from_errno(errno);
        }

        else {             
                ptr = malloc(s->size + len);
                if ( ! ptr )
                        return prelude_error_from_errno(errno);
                
                if ( s->data.robuf )
                        memcpy(ptr, s->data.robuf, s->index + 1);
                
                s->flags |= PRELUDE_STRING_CAN_REALLOC|PRELUDE_STRING_OWN_DATA;
        }

        s->size += len;
        s->data.rwbuf = ptr;

	return 0;
}



/**
 * prelude_string_new:
 * @string: Pointer where to store the created #prelude_string_t.
 *
 * Create a new #prelude_string_t object, and store in in @string.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */        
int prelude_string_new(prelude_string_t **string)
{
        *string = calloc(1, sizeof(**string));
        if ( ! *string )
                return prelude_error_from_errno(errno);
        
        (*string)->refcount = 1;
        prelude_list_init(&(*string)->list);
        (*string)->flags = PRELUDE_STRING_OWN_STRUCTURE;
        
        return 0;
}




/**
 * prelude_string_ref:
 * @string: Pointer to a #prelude_string_t object to reference.
 *
 * Increase @string reference count.
 *
 * Returns: @string.
 */
prelude_string_t *prelude_string_ref(prelude_string_t *data)
{
        data->refcount++;
        return data;
}



/**
 * prelude_string_new_dup_fast:
 * @string: Pointer where to store the created #prelude_string_t object.
 * @str: Initial string value.
 * @len: Lenght of @str.
 *
 * Create a new #prelude_string_t object with a copy of @str as it's
 * initial value.  The copy is owned by the @string and will be freed
 * upon prelude_string_destroy().
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int prelude_string_new_dup_fast(prelude_string_t **string, const char *str, size_t len)
{
        int ret;
        
        ret = check_string(str, len);
        if ( ret < 0 )
                return ret;
        
        ret = prelude_string_new(string);
        if ( ret < 0 )
                return ret;

        ret = string_buf_alloc(*string, len);
        if ( ret < 0 )
                return ret;
        
        string_buf_copy(*string, str, len);
        (*string)->flags |= PRELUDE_STRING_OWN_DATA|PRELUDE_STRING_CAN_REALLOC;
                
        return 0;
}



/**
 * prelude_string_new_dup:
 * @string: Pointer where to store the created #prelude_string_t object.
 * @str: Initial string value.
 *
 * Create a new #prelude_string_t object with a copy of @str as it's
 * initial value. The copy is owned by the @string and will be freed
 * upon prelude_string_destroy().
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int prelude_string_new_dup(prelude_string_t **string, const char *str)
{
        return prelude_string_new_dup_fast(string, str, strlen(str));
}



/**
 * prelude_string_new_nodup_fast:
 * @string: Pointer where to store the created #prelude_string_t object.
 * @str: Initial string value.
 * @len: Lenght of @str.
 *
 * Create a new #prelude_string_t object with a reference to @str as
 * initial value.  @str is owned by @string and will be freed upon
 * prelude_string_destroy().
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int prelude_string_new_nodup_fast(prelude_string_t **string, char *str, size_t len)
{
        int ret;

        ret = check_string(str, len);
        if ( ret < 0 )
                return ret;
                        
        ret = prelude_string_new(string);
        if ( ret < 0 )
                return ret;

        (*string)->index = len;
        (*string)->size = len + 1;
        (*string)->data.rwbuf = str;
        (*string)->flags |= PRELUDE_STRING_OWN_DATA|PRELUDE_STRING_CAN_REALLOC;
        
        return 0;
}



/**
 * prelude_string_new_nodup:
 * @string: Pointer where to store the created #prelude_string_t object.
 * @str: Initial string value.
 *
 * Create a new #prelude_string_t object with a reference to @str as
 * initial value.  @str is owned by @string and will be freed upon
 * prelude_string_destroy().
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int prelude_string_new_nodup(prelude_string_t **string, char *str)
{
        return prelude_string_new_nodup_fast(string, str, strlen(str));
}



/**
 * prelude_string_new_ref_fast:
 * @string: Pointer where to store the created #prelude_string_t object.
 * @str: Initial string value.
 * @len: Length of @str.
 *
 * Create a new #prelude_string_t object with a reference to @str as
 * initial value. @str won't be freed upon prelude_string_destroy().
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int prelude_string_new_ref_fast(prelude_string_t **string, const char *buf, size_t len)
{
        int ret;

        ret = check_string(buf, len);
        if ( ret < 0 )
                return ret;
        
        ret = prelude_string_new(string);
        if ( ret < 0 )
                return ret;

        (*string)->index = len;
        (*string)->size = len + 1;
        (*string)->data.robuf = buf;
        
        return 0;
}



/**
 * prelude_string_new_ref:
 * @string: Pointer where to store the created #prelude_string_t object.
 * @str: Initial string value.
 *
 * Create a new #prelude_string_t object with a reference to @str as
 * initial value. @str won't be freed upon prelude_string_destroy().
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int prelude_string_new_ref(prelude_string_t **string, const char *str)
{
        return prelude_string_new_ref_fast(string, str, strlen(str));
}



/**
 * prelude_string_set_dup_fast:
 * @string: Pointer to a #prelude_string_t object.
 * @buf: String to store in @string.
 * @len: Lenght of @buf.
 *
 * Store a copy of the string pointed by @buf in @string.
 * The @buf copy will be freed upon prelude_string_destroy().
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int prelude_string_set_dup_fast(prelude_string_t *string, const char *buf, size_t len)
{
        int ret;
        
        ret = check_string(buf, len);
        if ( ret < 0 )
                return ret;
        
        prelude_string_destroy_internal(string);

        ret = string_buf_alloc(string, len);
        if ( ret < 0 )
                return ret;
        
        string_buf_copy(string, buf, len);
        string->flags |= PRELUDE_STRING_OWN_DATA|PRELUDE_STRING_CAN_REALLOC;
        
        return 0;
}



/**
 * prelude_string_set_dup:
 * @string: Pointer to a #prelude_string_t object.
 * @buf: String to store in @string.
 *
 * Store a copy of the string pointed by @buf in @string.
 * The @buf copy will be freed upon prelude_string_destroy().
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int prelude_string_set_dup(prelude_string_t *string, const char *buf)
{
        return prelude_string_set_dup_fast(string, buf, strlen(buf));
}



/**
 * prelude_string_set_nodup_fast:
 * @string: Pointer to a #prelude_string_t object.
 * @buf: String to store in @string.
 * @len: Lenght of @buf.
 *
 * Store a reference to the string pointed by @buf in @string.
 * The referenced @buf will be freed upon prelude_string_destroy().
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int prelude_string_set_nodup_fast(prelude_string_t *string, char *buf, size_t len)
{
        int ret;

        ret = check_string(buf, len);
        if ( ret < 0 )
                return ret;
                
        prelude_string_destroy_internal(string);

        string->index = len;
        string->size = len + 1;
        string->data.rwbuf = buf;
        
        string->flags |= PRELUDE_STRING_OWN_DATA|PRELUDE_STRING_CAN_REALLOC;

        return 0;       
}




/**
 * prelude_string_set_nodup:
 * @string: Pointer to a #prelude_string_t object.
 * @buf: String to store in @string.
 *
 * Store a reference to the string pointed by @buf in @string.
 * The referenced @buf will be freed upon prelude_string_destroy().
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int prelude_string_set_nodup(prelude_string_t *string, char *buf)
{
        return prelude_string_set_nodup_fast(string, buf, strlen(buf));
}



/**
 * prelude_string_set_ref_fast:
 * @string: Pointer to a #prelude_string_t object.
 * @buf: String to store in @string.
 * @len: Lenght of @buf.
 *
 * Store a reference to the string pointed by @buf in @string.
 * The referenced @buf value won't be modified or freed.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int prelude_string_set_ref_fast(prelude_string_t *string, const char *buf, size_t len)
{
        int ret;
                
        ret = check_string(buf, len);
        if ( ret < 0 )
                return ret;
                
        prelude_string_destroy_internal(string);

        string->index = len;
        string->size = len + 1;
        string->data.robuf = buf;
 
        string->flags &= ~(PRELUDE_STRING_OWN_DATA|PRELUDE_STRING_CAN_REALLOC);

        return 0;
}



/**
 * prelude_string_set_ref:
 * @string: Pointer to a #prelude_string_t object.
 * @buf: String to store in @string.
 *
 * Store a reference to the string pointed by @buf in @string.
 * The referenced @buf value won't be modified or freed.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int prelude_string_set_ref(prelude_string_t *string, const char *buf)
{
        return prelude_string_set_ref_fast(string, buf, strlen(buf));
}




/**
 * prelude_string_copy_ref:
 * @src: Pointer to a #prelude_string_t object to copy data from.
 * @dst: Pointer to a #prelude_string_t object to copy data to.
 *
 * Reference @src content within @dst.
 * The referenced content won't be modified or freed.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int prelude_string_copy_ref(const prelude_string_t *src, prelude_string_t *dst)
{        
        prelude_string_destroy_internal(dst);

        dst->size = src->size;
        dst->index = src->index;
        dst->data.robuf = src->data.robuf;
                
        dst->flags = src->flags & ~(PRELUDE_STRING_OWN_DATA|PRELUDE_STRING_CAN_REALLOC);
                
        return 0;
}



/**
 * prelude_string_copy_dup:
 * @src: Pointer to a #prelude_string_t object to copy data from.
 * @dst: Pointer to a #prelude_string_t object to copy data to.
 *
 * Copy @src content within @dst.
 * The content is owned by @src and independent of @dst.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int prelude_string_copy_dup(const prelude_string_t *src, prelude_string_t *dst)
{
        prelude_string_destroy_internal(dst);

        dst->data.rwbuf = malloc(src->size);
        if ( ! dst->data.rwbuf )
                return prelude_error_from_errno(errno);
        
        dst->size = src->size;
        dst->index = src->index;
        dst->flags |= PRELUDE_STRING_OWN_DATA|PRELUDE_STRING_CAN_REALLOC;
        
        string_buf_copy(dst, src->data.robuf, src->index);
        
        return 0;
}



/**
 * prelude_string_clone:
 * @src: Pointer to an existing #prelude_string_t object.
 * @dst: Pointer to an address where to store the created #prelude_string_t object.
 *
 * Clone @src within a new #prelude_string_t object stored into @dst.
 * Data carried by @dst and @src are independant.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int prelude_string_clone(const prelude_string_t *src, prelude_string_t **dst)
{
        int ret;
        
        ret = prelude_string_new(dst);
        if ( ret < 0 )
                return ret;
        
        (*dst)->data.rwbuf = malloc(src->size);
        if ( ! (*dst)->data.rwbuf ) {
                free(*dst);
                return prelude_error_from_errno(errno);
        }

        (*dst)->size = src->size;
        (*dst)->index = src->index;
        (*dst)->flags |= PRELUDE_STRING_OWN_DATA|PRELUDE_STRING_CAN_REALLOC;

        string_buf_copy(*dst, src->data.robuf, src->index);
        
        return 0;
}



/**
 * prelude_string_get_len:
 * @string: Pointer to a #prelude_string_t object.
 *
 * Return the length of the string carried by @string object.
 *
 * Returns: The length of the string owned by @string.
 */
size_t prelude_string_get_len(const prelude_string_t *string)
{
        return string->index;
}



/**
 * prelude_string_get_string_or_default:
 * @string: Pointer to a #prelude_string_t object.
 * @def: Default value to a return in case @string is empty.
 *
 * Return the string carried on by @string object, or @def if it is empty.
 * There should be no operation done on the returned string since it is still
 * owned by @string.
 *
 * Returns: The string owned by @string or @def.
 */
const char *prelude_string_get_string_or_default(const prelude_string_t *string, const char *def)
{
        return string->data.robuf ? string->data.robuf : def;
}



/**
 * prelude_string_get_string:
 * @string: Pointer to a #prelude_string_t object.
 *
 * Return the string carried on by @string object.
 * There should be no operation done on the returned string since
 * it is still owned by @string.
 *
 * Returns: The string owned by @string if any.
 */
const char *prelude_string_get_string(const prelude_string_t *string)
{
        return string ? string->data.robuf : NULL;
}




/**
 * prelude_string_get_string_released:
 * @string: Pointer to a #prelude_string_t object.
 * @outptr: Pointer to an address where to store the released string.
 *
 * Get @string content, and release it so that further operation on
 * @string won't modify the returned content.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int prelude_string_get_string_released(prelude_string_t *string, char **outptr)
{        
        *outptr = NULL;
        
        if ( ! string->index )
                return 0;

        if ( ! (string->flags & PRELUDE_STRING_CAN_REALLOC) ) {
                *outptr = strdup(string->data.robuf);
                return (*outptr) ? 0 : prelude_error_from_errno(errno);
        }
        
        *outptr = prelude_realloc(string->data.rwbuf, string->index + 1);
        if ( ! *outptr )
                return prelude_error_from_errno(errno);

        string->size = 0;
        string->index = 0;
        string->data.rwbuf = NULL;
        
        return 0;
}




/**
 * prelude_string_is_empty:
 * @string: Pointer to a #prelude_string_t object.
 *
 * Check whether @string is empty.
 *
 * Returns: TRUE if @string is empty, FALSE otherwise.
 */
prelude_bool_t prelude_string_is_empty(const prelude_string_t *string)
{
        return (string->index == 0) ? TRUE : FALSE;
}



/*
 *  This function cannot be declared static because its invoked
 *  from idmef-tree-wrap.c
 */
void prelude_string_destroy_internal(prelude_string_t *string)
{        
        if ( (string->flags & PRELUDE_STRING_OWN_DATA) && string->data.rwbuf ) {
                free(string->data.rwbuf);
                string->data.rwbuf = NULL;
        }

        /*
         * free() should be done by the caller
         */
}



/**
 * prelude_string_destroy:
 * @string: Pointer to a #prelude_string_t object.
 *
 * Decrease refcount and destroy @string.
 * @string content content is destroyed if applicable (dup and nodup,
 * or a referenced string that have been modified.
 */
void prelude_string_destroy(prelude_string_t *string)
{        
        if ( --string->refcount )
                return;
        
        if ( ! prelude_list_is_empty(&string->list) )
                prelude_list_del_init(&string->list);
        
        prelude_string_destroy_internal(string);
        
        if ( string->flags & PRELUDE_STRING_OWN_STRUCTURE )
                free(string);
}




/**
 * prelude_string_vprintf:
 * @string: Pointer to a #prelude_string_t object.
 * @fmt: Format string to use.
 * @ap: Variable argument list.
 *
 * Produce output according to @fmt, storing argument provided in @ap
 * variable argument list, and write the output to the given @string.
 * See sprintf(3) for more information on @fmt format.
 *
 * Returns: The number of characters written, or a negative value if an error occured.
 */
int prelude_string_vprintf(prelude_string_t *string, const char *fmt, va_list ap)
{
        int ret;

        if ( ! (string->flags & PRELUDE_STRING_CAN_REALLOC) ) {

                ret = allocate_more_chunk_if_needed(string, 0);
                if ( ret < 0 )
                        return ret;
        }
        
        ret = vsnprintf(string->data.rwbuf + string->index, string->size - string->index, fmt, ap);
        
        /*
         * From sprintf(3) on GNU/Linux:
         *
         * snprintf  and vsnprintf do not write more than
         * size bytes (including the trailing '\0'), and return -1 if
         * the  output  was truncated due to this limit.  (Thus until
         * glibc 2.0.6. Since glibc 2.1 these  functions  follow  the
         * C99  standard and return the number of characters (exclud-
         * ing the trailing '\0') which would have  been  written  to
         * the final string if enough space had been available.)
         */
        if ( ret >= 0 && ret < string->size - string->index ) {
                string->index += ret;
                return ret;
        }

        ret = allocate_more_chunk_if_needed(string, (ret < 0) ? 0 : ret + 1);
        if ( ret < 0 )
		return ret;
        
        return prelude_string_vprintf(string, fmt, ap);
}




/**
 * prelude_string_sprintf:
 * @string: Pointer to a #prelude_string_t object.
 * @fmt: Format string to use.
 * @...: Variable argument list.
 *
 * Produce output according to @fmt, and write output to the given
 * @string. See snprintf(3) to learn more about @fmt format.
 *
 * Returns: The number of characters written, or a negative value if an error occured.
 */
int prelude_string_sprintf(prelude_string_t *string, const char *fmt, ...)
{
	int ret;
	va_list ap;
	
        va_start(ap, fmt);
        ret = prelude_string_vprintf(string, fmt, ap);
        va_end(ap);
                		
	return ret;
}



/**
 * prelude_string_cat:
 * @dst: Pointer to a #prelude_string_t object.
 * @str: Pointer to a string.
 * @len: Length of @str to copy.
 *
 * The prelude_string_ncat() function appends @len characters from @str to
 * the @dst #prelude_string_t object over-writing the `\0' character at the
 * end of @dst, and then adds a termi-nating `\0' character.
 *
 * Returns: @len, or a negative value if an error occured.
 */
int prelude_string_ncat(prelude_string_t *dst, const char *str, size_t len)
{
        int ret;
        
	if ( dst->flags & PRELUDE_STRING_CAN_REALLOC && len < dst->size - dst->index ) {
                
                memcpy(dst->data.rwbuf + dst->index, str, len);

                dst->index += len;
                dst->data.rwbuf[dst->index] = '\0';

		return len;
	}

        ret = allocate_more_chunk_if_needed(dst, len + 1);
	if ( ret < 0 )
                return ret;

	return prelude_string_ncat(dst, str, len);
}



/**
 * prelude_string_cat:
 * @dst: Pointer to a #prelude_string_t object.
 * @str: Pointer to a string.
 *
 * The prelude_string_cat() function appends the @str string to the @dst
 * prelude_string_t object over-writing the `\0' character at the end of
 * @dst, and then adds a termi-nating `\0' character. 
 *
 * Returns: @len, or a negative value if an error occured.
 */
int prelude_string_cat(prelude_string_t *dst, const char *str)
{
	return prelude_string_ncat(dst, str, strlen(str));
}




/**
 * prelude_string_clear:
 * @string: Pointer to a #prelude_string_t object.
 *
 * Reset @string content to zero.
 */
void prelude_string_clear(prelude_string_t *string)
{
        string->index = 0;
        
        if ( string->data.rwbuf && string->flags & PRELUDE_STRING_OWN_DATA )
                *(string->data.rwbuf) = '\0';

        else string->data.rwbuf = NULL;
}
