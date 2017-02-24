#ifndef __BT_H__INCLUDE__
#define __BT_H__INCLUDE__

#include "Arduino.h"
#include <LCD.hpp>

class BT
{
private:
  HardwareSerial &_serial;
  int _enPin;
  char *_qrInfo;
  int _readBufferIndex = 0;
  char _readBuffer[101];
  int _readBufferSize = sizeof(_readBuffer) / sizeof(_readBuffer[0]);
  int _plusFound = 0;
  bool _isFirstCheck = true;

public:
  static int BAUD;
  static char PIN[]; // PIN could be get programmatically with HC-05 put in AT mode.
                     // But, the version I had couldn't be put in AT mode during
                     // normal operation (you have to switch off the BT module,
                     // press a button on it, then turn it on).
                     // So the PIN is hardcoded here.
  static char NAME[]; // See comment on PIN
  static char MAC[]; // See comment on PIN

  BT(HardwareSerial &serial, int enPin);
  ~BT();
  void init();
  bool isEnabled();
  void setEnabled(bool value);
  char *infoForQR();
  char *pin();
  int available();
  int readAsPossible();
  bool hasCommandInBuffer();
  void executeCommand(char *otp, LCD &lcd);
  void send(char *data);
  void printDebug();
  void reset();
};

#endif
