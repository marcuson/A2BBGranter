#ifndef __BT_H__INCLUDE__
#define __BT_H__INCLUDE__

#include "Arduino.h"

class BT
{
private:
  bool _isEnabled;
  HardwareSerial &_serial;
  int _enPin;
  int _statePin;
  char *_qrInfo;

public:
  static int BAUD;
  static char PIN[]; // PIN could be get programmatically with HC-05 put in AT mode.
                     // But, the version I had couldn't be put in AT mode during
                     // normal operation (you have to switch off the BT module,
                     // press a button on it, then turn it on).
                     // So the PIN is hardcoded here.
  static char NAME[]; // See comment on PIN
  static char MAC[]; // See comment on PIN

  BT(HardwareSerial &serial, int enPin, int statePin);
  ~BT();
  void init();
  void setEnabled(bool value);
  char *infoForQR();
  char *pin();
};

#endif
