/*****************************************************************************
* | File        : logging.h
* | Author      : Luke Mulder
* | Function    : Debug utilities for logging system events
* | Info        :
*   Provides a flexible logging mechanism to assist in debugging applications.
*   Supports multiple logging levels (ERROR, WARNING, INFO, and NONE) and is
*   capable of logging messages from various modules of the application. The
*   logging function uses printf-style input for ease of use and flexibility.
*   Designed to facilitate easy integration and minimal dependencies.
*   
*   Use cases include:
*     - Tracking and reporting runtime errors
*     - Monitoring application flow and behavior
*     - Diagnostic outputs for development and testing phases
*
*   Ensure to configure the logger according to the target environment and
*   application requirements. This module is adaptable for both development
*   and production builds, with adjustable log levels and output formats.
*----------------
* | This version:   V1.0
* | Date        :   2024-05-24
* | Info        :   Basic version
*   - Initial release includes basic logging functions.
*   - Supports variable argument lists similar to printf for formatted output.
*   - Configurable modules and logging levels.
*   - Basic thread safety and performance considerations addressed.
*   
*   Future enhancements:
*   - Enhanced thread safety features.
*   - Support for logging to external files or systems.
*   - Extended configuration options for runtime adjustments.
*
******************************************************************************/
#ifndef _LOGGING_H_
#define _LOGGING_H_

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "stm32f7xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "stringbuffer.h"

#define LOGGING_ENABLED 1

#define LOG_MSG_BUFFER_SIZE 128
#define LOG_BUFFER_SIZE 64

#define LOGGING_TASK_PERIOD_MS 10

#define LOG_TASK_PRIORITY (tskIDLE_PRIORITY + 1)

// UART handle used by the logging task to output logs to serial
extern UART_HandleTypeDef huart1;
// Mutex used to protect access to log buffer
extern xSemaphoreHandle logMutex;

typedef enum {
  LOG_LEVEL_NONE = 0,
  LOG_LEVEL_ERROR,
  LOG_LEVEL_WARNING,
  LOG_LEVEL_INFO,
  LOG_LEVEL_MAX
} LogLevel_e;

// LOG LEVELS
// Place these levels in your c file to specify the log level you wish to set
// (From least to most verbose)
// #define LOG_LEVEL_NONE
// #define LOG_LEVEL_ERROR
// #define LOG_LEVEL_WARNING
// #define LOG_LEVEL_INFO

#define LOG_LEVEL_SETTING_NONE    0
#define LOG_LEVEL_SETTING_ERROR   1
#define LOG_LEVEL_SETTING_WARNING 2
#define LOG_LEVEL_SETTING_INFO    3

#ifdef SET_LOG_LEVEL_NONE
  #define LOG_LEVEL LOG_LEVEL_SETTING_NONE
#endif
#ifdef SET_LOG_LEVEL_ERROR
  #define LOG_LEVEL LOG_LEVEL_SETTING_ERROR
#endif
#ifdef SET_LOG_LEVEL_WARNING
  #define LOG_LEVEL LOG_LEVEL_SETTING_WARNING
#endif
#ifdef SET_LOG_LEVEL_INFO
  #define LOG_LEVEL LOG_LEVEL_SETTING_INFO
#endif

void logging(const char *file, int line, const char *func, LogLevel_e level, const char *log_str, ...);

#ifdef LOGGING_ENABLED
  #if LOG_LEVEL >= LOG_LEVEL_SETTING_ERROR
    #define LOG_ERROR(log_str, ...) logging(__FILE__, __LINE__, __func__, LOG_LEVEL_ERROR, log_str, ##__VA_ARGS__)
  #else
    #define LOG_ERROR(log_str, ...)
  #endif

  #if LOG_LEVEL >= LOG_LEVEL_SETTING_WARNING
    #define LOG_WARNING(log_str, ...) logging(__FILE__, __LINE__, __func__, LOG_LEVEL_WARNING, log_str, ##__VA_ARGS__)
  #else
    #define LOG_WARNING(log_str, ...)
  #endif

  #if LOG_LEVEL >= LOG_LEVEL_SETTING_INFO
    #define LOG_INFO(log_str, ...) logging(__FILE__, __LINE__, __func__, LOG_LEVEL_INFO, log_str, ##__VA_ARGS__)
  #else
    #define LOG_INFO(log_str, ...)
  #endif
#else // LOGGING_ENABLED
  #define LOG_ERROR(log_str, ...)
  #define LOG_WARNING(log_str, ...)
  #define LOG_INFO(log_str, ...)
#endif // LOGGING_ENABLED

void loggingInit(void);
void logTask(void *pvParameters);

#endif // _LOGGING_H_