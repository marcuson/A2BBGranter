#include "WIFI.hpp"
#include "Arduino.h"

long int WIFI::BAUD = 115200;

char WIFI::AP_SSID[] = "A2BB_APTest";
char WIFI::AP_PASS[] = "A2BB_APTest";

char WIFI::API_ENDPOINT[] = "192.168.1.138";
int WIFI::API_PORT = 5001;

WIFI::WIFI(HardwareSerial &serial)
: _serial(serial)
{ }

void WIFI::init()
{
  _serial.begin(BAUD);
  WiFi.init(&_serial);

  while (_status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(WIFI::AP_SSID);
    _status = WiFi.begin(WIFI::AP_SSID, WIFI::AP_PASS);
  }

  Serial.println("You're connected to the network!");
  printDebug();
}

bool WIFI::isConnected()
{
  return _status == WL_CONNECTED;
}

void WIFI::doGetRequest(bool isIn, char devId[], char subId[])
{
  reset();

  Serial.println("Starting connection to server...");

  if (!_client.connect(API_ENDPOINT, API_PORT))
  {
    Serial.println("Unable to connect to API endpoint!");
    return;
  }

  Serial.println("Connected to server");

  if (isIn)
  {
    _client.print("GET /api/in/");
  }
  else
  {
    _client.print("GET /api/out/");
  }

  _client.print(devId);
  _client.print("/");
  _client.print(subId);
  _client.println(" HTTP/1.1");

  _client.print("Host: ");
  _client.print(API_ENDPOINT);
  _client.print(":");
  _client.print(API_PORT);
  _client.println();
  _client.println("Connection: close");
  _client.println();

  readAsPossible();
}

int WIFI::available()
{
  return _client.available();
}

void WIFI::readAsPossible()
{
  unsigned long t = millis() + 500; // give the esp some time to send the entire string
  while (t > millis())
  {
    while (_client.available() > 0) {
      if (_readBufferIndex >= _readBufferSize - 1)
      {
        break;
      }

      char c = (char) _client.read();

      if (_firstBracketIndex < 0 && c == '{')
      {
        _firstBracketIndex = _readBufferIndex;
      }

      if (c == '}')
      {
        _lastBracketIndex = _readBufferIndex;
      }

      _readBuffer[_readBufferIndex] = c;
      _readBuffer[_readBufferIndex + 1] = '\0';
      _readBufferIndex++;
    }
  }
}

bool WIFI::hasResponseInBuffer()
{
  return _lastBracketIndex > 0 && _firstBracketIndex >= 0 && _readBuffer[_firstBracketIndex] == '{' &&
      _readBuffer[_lastBracketIndex] == '}';
}

void WIFI::reset()
{
  _firstBracketIndex = -1;
  _lastBracketIndex = -1;
  _readBufferIndex = 0;
}

void WIFI::printDebug()
{
  Serial.println("WiFi info:");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");

  Serial.println("WiFi buffer:");

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

bool WIFI::isHTTPStatusOk()
{
  int startIndex = strlen("HTTP/1.1 ");
  char code[4];
  strncpy(code, _readBuffer + startIndex, 3);
  code[3] = '\0';

  return strcmp("200", code) == 0;
}

bool WIFI::isRESTCodeOk()
{
  char codeIdentifier[] = "\"code\":";
  int matchValue = strlen(codeIdentifier);
  int codeIdentifierIndex = 0;
  int codeIndex = -1;

  for (int i = _firstBracketIndex; i < _lastBracketIndex + 1; i++)
  {
    char c = _readBuffer[i];

    if (c == codeIdentifier[codeIdentifierIndex])
    {
      codeIdentifierIndex++;

      if (codeIdentifierIndex >= matchValue)
      {
        codeIndex = i + 1;
        break;
      }
    }
    else
    {
      codeIdentifierIndex = 0;
    }
  }

  if (codeIndex < 0)
  {
    return false;
  }

  char code = _readBuffer[codeIndex];
  return code == '0';
}
