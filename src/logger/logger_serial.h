#pragma once

#include "logger/logger.h"

namespace cl
{
  class LoggerSerial : public ILogger
  {
  public:
    LoggerSerial() {};
    ~LoggerSerial() {};
    void Start(LogLevel level) override;
    void Log(LogLevel level, const char *tag, const char *format, va_list args) const override;
    void Debug(const char *tag, const char *format, ...) const override;
    void Info(const char *tag, const char *format, ...) const override;
    void Warn(const char *tag, const char *format, ...) const override;
    void Error(const char *tag, const char *format, ...) const override;

  private:
    LogLevel mLogLeve = LogLevel::DEBUG;
  };
} // namespace cl
