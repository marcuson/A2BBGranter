#define LCD_SCL_PIN    52 // SPI SCK (SPI pin: 52)
#define LCD_SDA_PIN    51 // SPI MOSI (SPI pin: 51)
#define LCD_RS_DC_PIN  49
#define LCD_CS_TFT_PIN 53 // SPI SS (SPI pin: 53)
#define LCD_RES_PIN    48

#define BT_EN_PIN    2
#define BT_STATE_PIN -1

#define WIFI_CH_PD_PIN -1
#define WIFI_RST_PIN -1

#include "Arduino.h"
#include <SPI.h>
#include <QRDuino.hpp>
#include <LCD.hpp>
#include <BT.hpp>

LCD lcd(LCD_CS_TFT_PIN, LCD_RS_DC_PIN, LCD_RES_PIN);
BT bt(Serial3, BT_EN_PIN);

char otp[6];

void newOtp()
{
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
  lcd.drawQR();
}

void setup()
{
  // Init devices
  lcd.init();
  bt.init();
  newOtp();

  Serial.begin(9600);
}

void loop()
{
  // Read from BT module
  bt.readAsPossible();

  if (bt.hasCommandInBuffer()) {
    bt.executeCommand(otp, lcd);
    delay(5000);
    bt.reset();
    newOtp();
  }

  delay(100);
}
