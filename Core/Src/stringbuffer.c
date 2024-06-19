/*****************************************************************************
* | File        : stringbuffer.c
* | Author      : Luke Mulder
* | Function    : Circular buffer utilities for data management
* | Info        :
*   Provides a circular buffer implementation to efficiently handle data 
*   streams where elements are continuously read and written. This is 
*   particularly useful for embedded systems or applications requiring 
*   high-performance buffering of data streams. The buffer size is set to a 
*   power of two to optimize index wrapping using bitwise operations.
******************************************************************************/

#include "stringbuffer.h"
#include "string.h"

int is_power_of_two(size_t value) {
    return value != 0 && (value & (value - 1)) == 0;
}

int str_buf_init_custom_size(StringBuffer *sb, size_t sb_size, size_t str_max_len)
{
  // String buffer size MUST be a power of 2
  // Otherwise overflow optimizations will not work in push/pop
  if(!is_power_of_two(sb_size) || str_max_len > STRING_BUFFER_MAX_LENGTH )
  {
    return -1;
  }

  sb->head = 0;
  sb->tail = 0;
  sb->count = 0;
  sb->size = sb_size;
  sb->str_len = str_max_len;

  sb->buf = (char**)malloc(sizeof(char*) * sb_size);

  if(sb->buf == NULL)
  {
    return -1;
  }

  for(int i = 0; i < sb_size; i++)
  {
    sb->buf[i] = (char*)malloc(sizeof(char) * str_max_len);

    if(sb->buf[i] == NULL)
    {
      while(i-- > 0)
      {
          free(sb->buf[i]);
      }
      free(sb->buf);

      return -1;
    }
  }
  
  return 0;
}

int str_buf_init(StringBuffer *sb)
{
  return str_buf_init_custom_size(sb, STRING_BUFFER_SIZE, STRING_BUFFER_MAX_LENGTH);
}

int str_buf_free(StringBuffer *sb)
{
  for(int i = 0; i < sb->size; i++)
  {
    free(sb->buf[i]);
  }

  free(sb->buf);

  return 0;
}

int str_buf_push(StringBuffer *sb, const char* str) {
  if(sb == NULL || str == NULL)
  {
    return -1;
  }

  strncpy(sb->buf[sb->head], str, sb->str_len - 1);
  sb->buf[sb->head][sb->str_len - 1] = '\0';

  // Again the size MUST be a power of 2 to avoid modulus
  sb->head = (sb->head + 1) & (sb->size - 1);

  // if head overwrites tail
  if(sb->head == sb->tail)
    sb->tail = (sb->tail + 1) & (sb->size - 1);

  // Size is the max count
  if(sb->count < sb->size)
    sb->count++;

  return 0;
}

int str_buf_pop(StringBuffer *sb, char** str_container) {
  if(sb == NULL || str_container == NULL)
  {
    return -1;
  }

  if(sb->count == 0)
  {
    *str_container = NULL;
    return 0;
  }

  *str_container = sb->buf[sb->tail];

  sb->tail = (sb->tail + 1) & (sb->size - 1);
  sb->count--;

  return 0;
}

size_t str_buff_count(StringBuffer *sb)
{
  return sb->count;
}