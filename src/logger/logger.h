#pragma once
#include <string>

namespace cl
{
enum LogLevel
{
  DEBUG,
  INFO,
  WARN,
  ERROR,
};

class ILogger
{
public:
  virtual void Start(LogLevel level) = 0;
  virtual void Log(LogLevel level, const char *tag, const char *format, va_list args) const = 0;
  virtual void Debug(const char *tag, const char *format, ...) const = 0;
  virtual void Info(const char *tag, const char *format, ...) const = 0;
  virtual void Warn(const char *tag, const char *format, ...) const = 0;
  virtual void Error(const char *tag, const char *format, ...) const = 0;
};
} // namespace cl
