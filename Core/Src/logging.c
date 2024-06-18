/*****************************************************************************
* | File        : logging.c
* | Author      : Luke Mulder
* | Function    : Debug utilities for logging system events
* | Info        :
*   Provides a flexible logging mechanism to assist in debugging applications.
******************************************************************************/

#include "logging.h"

typedef struct {
    char message[LOG_MSG_BUFFER_SIZE];
} LogEntry;

static QueueHandle_t logQueue;
SemaphoreHandle_t logMutex;

void logTask(void *pvParameters)
{
  LogEntry entry;

  logMutex = xSemaphoreCreateMutex();
  logQueue = xQueueCreate(10, sizeof(LogEntry));

  for(;;)
  {
    if (xQueueReceive(logQueue, &entry, portMAX_DELAY) == pdPASS) 
    {
      xSemaphoreTake(logMutex, portMAX_DELAY);
      HAL_UART_Transmit(&huart1, (uint8_t*)entry.message, strlen(entry.message), 0xFFFF);
      xSemaphoreGive(logMutex);
    }
  }

  vTaskDelete(NULL);
}

void logging(const char *file, int line, const char *func, LogLevel_e level, const char *log_str, ...)
{
  if (level == LOG_LEVEL_NONE) return;

  LogEntry entry;
  entry.message[LOG_MSG_BUFFER_SIZE - 1] = '\0';

  va_list args;
  va_start(args, log_str);

  const char* level_str = "";
  switch(level)
  {
      case LOG_LEVEL_ERROR:   level_str = ERROR_STR;   break;
      case LOG_LEVEL_WARNING: level_str = WARNING_STR; break;
      case LOG_LEVEL_INFO:    level_str = INFO_STR;    break;
      default:                level_str = NONE_STR;    break;
  }

  int offset = snprintf(entry.message, sizeof(entry.message), "[%s] %s:%d %s() - ", level_str, file, line, func);
  if (offset < 0 || offset >= sizeof(entry.message))
  {
      va_end(args);
      return;
  }

  int needed = vsnprintf(entry.message + offset, sizeof(entry.message) - offset, log_str, args);
  if (needed < 0 || needed >= (int)(sizeof(entry.message) - offset))
  {
      va_end(args);
      return;
  }

  strncat(entry.message, "\r\n", sizeof(entry.message) - strlen(entry.message) - 1);

  xQueueSend(logQueue, &entry, portMAX_DELAY);
}