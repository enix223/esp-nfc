#include "logger/logger_serial.h"
#include <Arduino.h>

// Helper function to convert log level to string
const char *levelToString(cl::LogLevel level)
{
  switch (level)
  {
  case cl::LogLevel::DEBUG:
    return "DEBG";
  case cl::LogLevel::INFO:
    return "INFO";
  case cl::LogLevel::WARN:
    return "WARN";
  case cl::LogLevel::ERROR:
    return "ERRO";
  default:
    return "UNKN";
  }
}

void cl::LoggerSerial::Start(LogLevel level)
{
  Serial.begin(CONFIG_SERIAL_BAUD_RATE);
  mLogLeve = level;
}

void cl::LoggerSerial::Log(LogLevel level, const char *tag, const char *format, va_list args) const
{
  if (level < mLogLeve)
  {
    return;
  }

  char buffer[256];
  vsnprintf(buffer, sizeof(buffer), format, args);
  Serial.printf("[%s][%s]: ", tag, levelToString(level));
  Serial.println(buffer);
}

void cl::LoggerSerial::Debug(const char *tag, const char *format, ...) const
{
  va_list args;
  va_start(args, format);
  Log(LogLevel::DEBUG, tag, format, args);
  va_end(args);
}

void cl::LoggerSerial::Info(const char *tag, const char *format, ...) const
{
  va_list args;
  va_start(args, format);
  Log(LogLevel::INFO, tag, format, args);
  va_end(args);
}

void cl::LoggerSerial::Warn(const char *tag, const char *format, ...) const
{
  va_list args;
  va_start(args, format);
  Log(LogLevel::WARN, tag, format, args);
  va_end(args);
}

void cl::LoggerSerial::Error(const char *tag, const char *format, ...) const
{
  va_list args;
  va_start(args, format);
  Log(LogLevel::ERROR, tag, format, args);
  va_end(args);
}
