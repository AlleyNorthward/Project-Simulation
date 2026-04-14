#include "Log.h"
#include <stdarg.h>
#include <stdio.h>

namespace sdust {

Log &Log::instance() {
  static Log instance;
  return instance;
}

void Log::info(const char *fmt, ...) {
  if (_level < LogLevel::INFO)
    return;
  va_list args;
  va_start(args, fmt);
  printf("[INFO] ");
  vprintf(fmt, args);
  printf("\r\n");
  va_end(args);
  printf("\r\n");
}

void Log::error(const char *fmt, ...) {
  if (_level < LogLevel::ERROR)
    return;

  va_list args;
  va_start(args, fmt);
  printf("[ERROR] ");
  vprintf(fmt, args);
  printf("\r\n");
  va_end(args);
  printf("\r\n");
}

void Log::debug(const char *fmt, ...) {
  if (_level < LogLevel::DEBUG)
    return;

  va_list args;
  va_start(args, fmt);
  printf("[DEBUG] ");
  vprintf(fmt, args);
  printf("\r\n");
  va_end(args);
  printf("\r\n");
}

void Log::warn(const char *fmt, ...) {
  if (_level < LogLevel::WARN)
    return;

  va_list args;
  va_start(args, fmt);
  printf("[WARNING] ");
  vprintf(fmt, args);
  printf("\r\n");
  va_end(args);
  printf("\r\n");
}

} // namespace sdust
