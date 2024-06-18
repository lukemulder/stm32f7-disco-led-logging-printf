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

int is_power_of_two(uint16_t value) {
    return value != 0 && (value & (value - 1)) == 0;
}

int str_buf_init(StringBuffer *sb, uint16_t sb_size, uint16_t str_max_len)
{
  // String buffer size MUST be a power of 2
  if(!is_power_of_two(sb_size) || str_max_len > STRING_MAX_LENGTH )
  {
    return -1;
  }

  sb->head = 0;
  sb->tail = 0;
  sb->full = 0;
  sb->size = sb_size;

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
  return str_buf_init(sb, STRING_BUFFER_SIZE, STRING_MAX_LENGTH);
}

int str_buf_push(StringBuffer *sb, const char* data) {

  return 0;
}

int str_buf_pop(StringBuffer *sb, char** data) {

  return 0;
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