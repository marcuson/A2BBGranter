#include "BT.hpp"
#include "Arduino.h"

int BT::BAUD = 9600;
char BT::PIN[] = "1234";
char BT::NAME[] = "HC-05";
char BT::MAC[] = "20:16:11:28:31:26";

BT::BT(HardwareSerial &serial, int enPin)
: _serial(serial)
{
  _qrInfo = new char[strlen(MAC) + strlen(NAME) + 1];
  strcpy(_qrInfo, MAC);
  strcat(_qrInfo, ",");
  strcat(_qrInfo, NAME);

  _enPin = enPin;
}

BT::~BT()
{
  delete [] _qrInfo;
}

void BT::init()
{
  pinMode(_enPin, OUTPUT);
  setEnabled(true);
  _serial.begin(BAUD);
}

bool BT::isEnabled()
{
  return digitalRead(_enPin) == HIGH;
}

void BT::setEnabled(bool value)
{
  digitalWrite(_enPin, value ? HIGH : LOW);
}

char *BT::pin()
{
  return PIN;
}

char *BT::infoForQR()
{
  return _qrInfo;
}

int BT::available()
{
  return _serial.available();
}

int BT::readAsPossible()
{
  int toRead = available();
  int actuallyRead = 0;

  if (toRead > 0)
  {
    for (int i = 0; i < toRead; i++)
    {
      if (_readBufferIndex >= _readBufferSize - 1)
      {
        break;
      }

      char c = (char) _serial.read();

      _readBuffer[_readBufferIndex] = c;
      _readBuffer[_readBufferIndex + 1] = '\0';
      _readBufferIndex++;

      if (c == '+')
      {
        _plusFound++;
      }
    }

    if (_readBuffer[0] != '+')
    {
      reset();
    }
  }

  return actuallyRead;
}

bool BT::hasCommandInBuffer()
{
  if (_readBufferIndex < 6 || _plusFound < 3)
  {
    return false;
  }

  return _readBuffer[0] == '+' && _readBuffer[4] == '+' && _readBuffer[_readBufferIndex - 1] == '+';
}

bool BT::checkOtp(char otp[], char* devId)
{
  if (!hasCommandInBuffer())
  {
    return false;
  }

  Serial.print("From BT received: ");
  Serial.println(_readBuffer);

  char comm[4];
  strncpy(comm, _readBuffer + 1, 3);
  comm[3] = '\0';

  if (strcmp(comm, "dev") == 0)
  {
    char *commaPtr = strchr(_readBuffer + 4, ',');
    int commaPos = commaPtr - _readBuffer;

    int devIdSize = commaPos - 5;
    strncpy(devId, _readBuffer + 5, devIdSize);
    devId[devIdSize] = '\0';

    int otpSize = _readBufferIndex - 1 - commaPos - 1;
    if (!_isFirstCheck)
    {
      otpSize -= 2; // Hack because 2nd call to qrduino library messes up BT serial communication
    }
    char checkOtp[otpSize + 1];
    strncpy(checkOtp, commaPtr + 1, otpSize);
    checkOtp[otpSize] = '\0';

    _isFirstCheck = false;

    Serial.print("Try to grant device with id: ");
    Serial.println(devId);

    Serial.print("OTP provided: '");
    Serial.print(checkOtp);

    if (strcmp(checkOtp, otp) != 0)
    {
      Serial.print("' is NOT valid (needed '");
      Serial.print(otp);
      Serial.println("')");

      return false;
    }

    Serial.println("' is valid");
  }

  return true;
}

void BT::reset()
{
  _plusFound = 0;
  _readBufferIndex = 0;
}

void BT::printDebug()
{
  Serial.print("BT buffer: ");

  for (int i = 0; i < _readBufferSize; i++)
  {
    if (_readBuffer[i] == '\0')
    {
      break;
    }

    Serial.print(_readBuffer[i]);
  }

  Serial.println();
}

void BT::send(char *data)
{
  _serial.print(data);
}
