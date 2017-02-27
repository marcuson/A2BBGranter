#define LCD_SCL_PIN    52 // SPI SCK (SPI pin: 52)
#define LCD_SDA_PIN    51 // SPI MOSI (SPI pin: 51)
#define LCD_RS_DC_PIN  49
#define LCD_CS_TFT_PIN 53 // SPI SS (SPI pin: 53)
#define LCD_RES_PIN    48

#define BT_EN_PIN    2
#define BT_STATE_PIN -1

#define WIFI_CH_PD_PIN -1
#define WIFI_RST_PIN -1

#define IS_IN true
#define GRANTER_ID "granter1"

#include "Arduino.h"
#include <SPI.h>
#include <WiFiEsp.h>

#include <WIFI.hpp>
#include <QRDuino.hpp>
#include <LCD.hpp>
#include <BT.hpp>

LCD lcd(LCD_CS_TFT_PIN, LCD_RS_DC_PIN, LCD_RES_PIN);
BT bt(Serial3, BT_EN_PIN);
WIFI wifi(Serial1);

int phase = 0;
char currDevId[100];
char otp[6];

void success()
{
  lcd.drawSuccess();
  phase = 0;
  delay(5000);
}

void fail()
{
  lcd.drawFail();
  phase = 0;
  delay(5000);
}

void newOtp()
{
  bt.reset();
  wifi.reset();

  for (int i = 0; i < 5; i++) {
    char c = (char) random(97, 123);
    otp[i] = c;
  }
  otp[5] = '\0';

  char qrData[strlen(bt.infoForQR()) + strlen(otp) + 2];
  strcpy(qrData, bt.infoForQR());
  strcat(qrData, ",");
  strcat(qrData, otp);
  strcat(qrData, "\0");

  Serial.print("New QRData is: ");
  Serial.println(qrData);

  QRDuino::setData(qrData);
  QRDuino::encode();

  lcd.drawBackground();
  lcd.drawBTPasskey(bt.pin());
  lcd.drawWiFiStatus(wifi.isConnected() ? "OK" : "KO");
  lcd.drawQR();

  phase++;
}

void readBT()
{
  // Read from BT module
  bt.readAsPossible();

  // If data read is a grant request, go to next phase
  if (bt.hasCommandInBuffer()) {
    lcd.clear();
    phase++;
  }
}

void processBT()
{
  lcd.drawText("Checking OTP...", 0, 0, true);

  if (!bt.checkOtp(otp, currDevId))
  {
    fail();
    return;
  }

  lcd.drawText("OK");

  lcd.drawText("Try granting access to device...\n", 0, lcd.getYNL(), true);
  wifi.doGetRequest(IS_IN, currDevId, GRANTER_ID);

  phase++;
}

void readREST()
{
  wifi.printDebug();

  // if data received is a JSON response object, go to next phase
  if (!wifi.hasResponseInBuffer())
  {
    Serial.println("Unable to get useful response from REST buffer!");
    fail();
    return;
  }

  phase++;
}

void processREST()
{
  Serial.print("HTTP status: ");
  Serial.println(wifi.isHTTPStatusOk() ? "OK" : "NOT OK");
  Serial.print("REST response code: ");
  Serial.println(wifi.isRESTCodeOk() ? "OK" : "NOT OK");

  if (wifi.isHTTPStatusOk() && wifi.isRESTCodeOk())
  {
    Serial.println("OK! Sending ack back");
    bt.send("ack;");
    success();
  }
  else
  {
    Serial.println("NOT ok, Sending nack back");
    bt.send("nack;");
    fail();
  }
}

void setup()
{
  // Init devices
  Serial.begin(9600);

  lcd.init();
  lcd.drawText("Please wait...", 0, 0, true);
  bt.init();
  wifi.init();
}

void loop()
{
  switch (phase)
  {
    case 0:
      newOtp();
      break;

    case 1:
      readBT();
      break;

    case 2:
      processBT();
      break;

    case 3:
      readREST();
      break;

    case 4:
      processREST();
      break;
  }
}
