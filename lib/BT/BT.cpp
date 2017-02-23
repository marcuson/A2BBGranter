#include "BT.hpp"
#include "Arduino.h"

int BT::BAUD = 9600;
char BT::PIN[] = "1234";
char BT::NAME[] = "HC-05";
char BT::MAC[] = "20:16:11:28:31:26";

BT::BT(HardwareSerial &serial, int enPin, int statePin)
: _serial(serial)
{
  _qrInfo = new char[strlen(MAC) + strlen(NAME) + 1];
  Serial.println("1");
  strcpy(_qrInfo, MAC);
  strcat(_qrInfo, ",");
  strcat(_qrInfo, NAME);
  Serial.println("2");

  _serial.begin(BAUD);
  _enPin = enPin;
  _statePin = statePin;
}

BT::~BT() {
  delete [] _qrInfo;
}

void BT::init() {
  pinMode(_enPin, OUTPUT);
  pinMode(_statePin, INPUT);
  setEnabled(true);
}

void BT::setEnabled(bool value) {
  digitalWrite(_enPin, value ? HIGH : LOW);
  _isEnabled = value;
}

char *BT::pin() {
  return PIN;
}

char *BT::infoForQR() {
  return _qrInfo;
}
