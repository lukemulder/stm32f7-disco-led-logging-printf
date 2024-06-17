/*****************************************************************************
* | File        : logging.c
* | Author      : Luke Mulder
* | Function    : Debug utilities for logging system events
* | Info        :
*   Provides a flexible logging mechanism to assist in debugging applications.
******************************************************************************/

#include "logging.h"

static char log_msg[LOG_MSG_BUFFER_SIZE];

void logging(const char *file, int line, const char *func, LogLevel_e level, const char *log_str, ...) {
  if (level == LOG_LEVEL_NONE) return;

  va_list args;
  va_start(args, log_str);

  const char* level_str = "";
  switch(level) {
      case LOG_LEVEL_ERROR: level_str = ERROR_STR; break;
      case LOG_LEVEL_WARNING: level_str = WARNING_STR; break;
      case LOG_LEVEL_INFO: level_str = INFO_STR; break;
      default: level_str = NONE_STR; break;
  }

  int offset = snprintf(log_msg, sizeof(log_msg), "[%s] %s:%d %s() - ", level_str, file, line, func);
  if (offset < 0 || offset >= sizeof(log_msg)) {
      va_end(args);
      return;
  }

  int needed = vsnprintf(log_msg + offset, sizeof(log_msg) - offset, log_str, args);
  if (needed < 0 || needed >= (int)(sizeof(log_msg) - offset)) {
      va_end(args);
      return;
  }

  strncat(log_msg, "\r\n", sizeof(log_msg) - strlen(log_msg) - 1);

  HAL_UART_Transmit(&huart1, (uint8_t*)log_msg, strlen(log_msg), 0xFFFF);
}