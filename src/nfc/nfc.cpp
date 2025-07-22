#include <sstream>
#include <iomanip>
#include "nfc/nfc.h"

uint8_t ndefMessage[] = {
    0xD1, 0x01, 0x0C, 0x55, 0x01, 0x65, 0x78, 0x61,
    0x6D, 0x70, 0x6C, 0x65, 0x2E, 0x63, 0x6F, 0x6D};

namespace cl
{
  static const char *TAG = "NFC";

  NfcModule::NfcModule(std::shared_ptr<cl::ILogger> logger) : logger_(logger), mode_(Mode::READER), started_(false)
  {
    nfcHw_ = std::make_unique<Adafruit_PN532>(CONFIG_PN532_IRQ, CONFIG_PN532_RESET);
  }

  bool NfcModule::Begin(Mode mode)
  {
    if (started_)
    {
      return false;
    }

    Wire.begin(CONFIG_PN532_I2C_SDA, CONFIG_PN532_I2C_SCL);
    nfcHw_->begin();

    uint32_t versiondata = nfcHw_->getFirmwareVersion();
    if (!versiondata)
    {
      return false;
    }
    nfcHw_->SAMConfig();

    started_ = true;

    std::ostringstream oss;
    oss << "Found chip PN5" << std::hex << ((versiondata >> 24) & 0xFF);
    logger_->Debug(TAG, oss.str().c_str());

    oss.str("");
    oss << "Firmware ver. " << std::dec << ((versiondata >> 16) & 0xFF);
    oss << '.' << ((versiondata >> 8) & 0xFF);
    logger_->Debug(TAG, oss.str().c_str());

    return true;
  }

  void NfcModule::Loop()
  {
    if (mode_ == Mode::READER)
    {
      Read();
      return;
    }

    Emulate();
  }

  void NfcModule::Read()
  {
    uint8_t success;
    uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0}; // Buffer to store the returned UID
    uint8_t uidLength;                     // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

    // Wait for an NTAG203 card
    success = nfcHw_->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

    if (success)
    {
      std::ostringstream oss;

      // Display some basic information about the card
      logger_->Info(TAG, "Found an ISO14443A card");

      oss << "  UID Length: " << static_cast<int>(uidLength) << " bytes";
      logger_->Info(TAG, oss.str().c_str());

      auto hexUid = FormatHexString(uid, uidLength);
      oss.str("");
      oss << "  UID Value: " << hexUid;
      logger_->Info(TAG, oss.str().c_str());

      if (uidLength == 4)
      {
        // We probably have a Mifare Classic card ...
        uint32_t cardid = uid[0];
        cardid <<= 8;
        cardid |= uid[1];
        cardid <<= 8;
        cardid |= uid[2];
        cardid <<= 8;
        cardid |= uid[3];

        oss.str("");
        oss << ("Seems to be a Mifare Classic card #") << cardid;
        logger_->Info(TAG, oss.str().c_str());
      }
      logger_->Info(TAG, "");
    }
  }

  void NfcModule::Emulate()
  {
    // Wait for a reader to contact us
    if (nfcHw_->inListPassiveTarget())
    {
      // Timeout after 1500ms
      logger_->Info(TAG, "Reader detected!");

      uint8_t response[255];
      uint8_t responseLength = sizeof(response);

      bool success = nfcHw_->inDataExchange(ndefMessage, sizeof(ndefMessage), response, &responseLength);

      if (success)
      {
        auto msg = "Sent " + FormatHexString(response, responseLength);
        logger_->Info(TAG, msg.c_str());
        nfcHw_->PrintHexChar(response, responseLength);
      }
      else
      {
        logger_->Warn(TAG, "Not sent");
      }

      // Wait a bit before allowing another read
      delay(1000);
    }
  }

  void NfcModule::Stop()
  {
    Wire.end();
  }

  std::string NfcModule::FormatHexString(const uint8_t *buf, uint8_t bufLen)
  {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0'); // Set to hex format with leading zeros

    for (uint8_t i = 0; i < bufLen; i++)
    {
      oss << std::setw(2) << static_cast<int>(buf[i]); // Each byte as 2-digit hex
      if (i < bufLen - 1)
      {
        oss << " "; // Add space between bytes (optional)
      }
    }

    return oss.str();
  }
} // namespace cl
