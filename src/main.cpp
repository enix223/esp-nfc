#include "logger/logger_serial.h"
#include "nfc/nfc.h"

static std::shared_ptr<cl::ILogger> logger = std::make_shared<cl::LoggerSerial>();
static std::shared_ptr<cl::NfcModule> nfc = std::make_shared<cl::NfcModule>(logger);
static const char *TAG = "main";

void setup()
{
  delay(3000);

  logger->Start(cl::DEBUG);
  auto rc = nfc->begin(cl::NfcModule::Mode::READER);
  if (!rc)
  {
    logger->Error(TAG, "failed to start nfc module");
    while (1)
      ; // halt
  }
}

void loop()
{
  nfc->loop();

  // Wait 1 second before continuing
  delay(1000);
}