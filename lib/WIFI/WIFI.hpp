#ifndef __WIFI_H__INCLUDE__
#define __WIFI_H__INCLUDE__

#include "Arduino.h"
#include <WiFiEsp.h>

class WIFI
{
private:
  WiFiEspClient _client;
  HardwareSerial &_serial;
  int _status = WL_IDLE_STATUS;
  int _readBufferIndex = 0;
  char _readBuffer[1001];
  int _readBufferSize = sizeof(_readBuffer) / sizeof(_readBuffer[0]);
  int _firstBracketIndex = -1;
  int _lastBracketIndex = -1;

  void readAsPossible();

public:
  static long int BAUD;
  static char AP_SSID[];
  static char AP_PASS[];
  static char API_ENDPOINT[];
  static int API_PORT;

  WIFI(HardwareSerial &serial);
  void init();
  void doGetRequest(bool isIn, char devId[], char subId[]);
  int available();
  bool isConnected();
  bool hasResponseInBuffer();
  void printDebug();
  void reset();
  bool isHTTPStatusOk();
  bool isRESTCodeOk();
};

#endif
