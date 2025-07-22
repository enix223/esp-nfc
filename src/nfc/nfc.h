#pragma once
#include <Wire.h>
#include <Adafruit_PN532.h>
#include <memory>
#include "logger/logger.h"

namespace cl
{
  class NfcModule
  {

  public:
    enum Mode
    {
      READER,
      SIMULATOR,
    };

    NfcModule(std::shared_ptr<cl::ILogger> logger);
    ~NfcModule() = default;

    bool Begin(Mode mode);
    void Loop();
    void Stop();

  private:
    void Read();
    void Emulate();
    std::string FormatHexString(const uint8_t *uid, uint8_t uidLength);

    std::shared_ptr<cl::ILogger> logger_;
    std::unique_ptr<Adafruit_PN532> nfcHw_;
    Mode mode_;
    bool started_;
  };
} // namespace cl
