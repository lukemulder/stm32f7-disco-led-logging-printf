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

int str_buf_init_custom_size(StringBuffer *sb, size_t buf_size, size_t str_size)
{
  // String buffer size MUST be a power of 2
  // Otherwise overflow optimizations will not work in push/pop
  if(!is_power_of_two(buf_size) || str_size > STRING_BUFFER_MAX_LENGTH )
  {
    return -1;
  }

  sb->head = 0;
  sb->tail = 0;
  sb->count = 0;
  sb->buf_size = buf_size;
  sb->str_size = str_size;

  sb->buf = (char**)malloc(sizeof(char*) * sb->buf_size);

  if(sb->buf == NULL)
  {
    return -1;
  }

  // Memory allocation for buffer string
  for(int i = 0; i < sb->buf_size; i++)
  {
    sb->buf[i] = (char*)malloc(sizeof(char) * sb->str_size);

    if(sb->buf[i] == NULL)
    {
      // If failed to allocate buffer entry return previously allocated
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
  // Deallocate all buffer memory on the heap
  for(int i = 0; i < sb->buf_size; i++)
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

  strncpy(sb->buf[sb->head], str, sb->str_size - 1);
  sb->buf[sb->head][sb->str_size - 1] = '\0';

  // Again the size MUST be a power of 2 to avoid modulus
  sb->head = (sb->head + 1) & (sb->buf_size - 1);

  // if head overwrites tail
  if(sb->head == sb->tail)
    sb->tail = (sb->tail + 1) & (sb->buf_size - 1);

  // Size is the max count
  if(sb->count < sb->buf_size)
    sb->count++;

  return 0;
}

int str_buf_pop(StringBuffer *sb, char** str_container) {
  if(sb == NULL || str_container == NULL)
  {
    return -1;
  }

  // If empty place NULL data but don't fail
  if(sb->count == 0)
  {
    *str_container = NULL;
    return 0;
  }

  *str_container = sb->buf[sb->tail];

  sb->tail = (sb->tail + 1) & (sb->buf_size - 1);
  sb->count--;

  return 0;
}

size_t str_buff_count(StringBuffer *sb)
{
  return sb->count;
}

size_t str_buff_max_str_len(StringBuffer *sb)
{
  return sb->str_size;
}