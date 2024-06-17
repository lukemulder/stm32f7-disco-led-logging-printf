/*****************************************************************************
* | File        : logging.c
* | Author      : Luke Mulder
* | Function    : Debug utilities for logging system events
* | Info        :
*   Provides a flexible logging mechanism to assist in debugging applications.
******************************************************************************/

#include "logging.h"

#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}

void logging(const char *file, int line, const char *func, LogLevel_e level, const char *log_str, ...) {
  va_list args;
  va_start(args, log_str);

  const char* level_str = "";
  switch(level) {
      case LOGLEVEL_ERROR: level_str = ERROR_STR; break;
      case LOGLEVEL_WARNING: level_str = WARNING_STR; break;
      case LOGLEVEL_INFO: level_str = INFO_STR; break;
      default: level_str = NONE_STR; break;
  }

  if(level != LOGLEVEL_NONE) {
      printf("[%s] %s:%d %s() - ", level_str, file, line, func);
      vprintf(log_str, args);
      printf("\r\n");
  }

  va_end(args);
}