/*****************************************************************************
* | File        : stringbuffer.h
* | Author      : Luke Mulder
* | Function    : String buffer management for dynamic data handling
* | Info        :
*   This header defines the interface for a circular string buffer system
*   designed to manage dynamic string data efficiently. It supports operations
*   such as initialization, pushing data, popping data, and cleanup. The buffer
*   automatically manages memory and can overwrite old data when full, making
*   it suitable for high-throughput data scenarios where older data can be
*   discarded if not processed in time.
*
*   Key features include:
*     - Circular buffer logic to continuously manage data without needing
*       to reset the buffer manually.
*     - Dynamic handling of string data with configurable maximum string length
*       and buffer size.
*     - Automatic memory management including initialization and deallocation.
*     - Overwrite capability when the buffer is full to ensure ongoing operations.
*
*   Usage scenarios:
*     - Logging systems where recent messages are more critical than older ones.
*     - Communication systems that need to buffer incoming messages.
*     - Real-time data processing applications.
*
* | This version:   V1.0
* | Date        :   2024-06-15
* | Info        :   Basic version
*   - Provides fundamental buffer operations like push, pop, and free.
*   - Initial handling of dynamic string operations within a fixed-size circular buffer.
*   - Basic memory management for allocation and deallocation of string buffers.
*
*   Future enhancements:
*   - Enhanced memory optimization for different runtime environments.
*   - Improved thread safety for concurrent access scenarios.
*   - Advanced configuration options to tailor buffer behavior per application needs.
*
*****************************************************************************/
#ifndef STRINGBUFFER_H
#define STRINGBUFFER_H

#include <stdint.h>
#include <stdlib.h>

#define STRING_BUFFER_SIZE 64
#define STRING_BUFFER_MAX_LENGTH 256

typedef struct {
    char** buf;
    size_t head;
    size_t tail;
    size_t count;
    size_t size;
    size_t str_len;
} StringBuffer;

int str_buf_init_custom_size(StringBuffer *sb, size_t sb_size, size_t str_max_len);
int str_buf_init(StringBuffer *sb);
int str_buf_free(StringBuffer *sb);

int str_buf_push(StringBuffer *sb, const char* data);
int str_buf_pop(StringBuffer *sb, char** data);

size_t str_buff_count(StringBuffer *sb);

#endif // STRINGBUFFER_H