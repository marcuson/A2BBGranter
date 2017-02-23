#define LCD_SCL_PIN    52 // SPI SCK (SPI pin: 52)
#define LCD_SDA_PIN    51 // SPI MOSI (SPI pin: 51)
#define LCD_RS_DC_PIN  49
#define LCD_CS_TFT_PIN 53 // SPI SS (SPI pin: 53)
#define LCD_RES_PIN    48

#define BT_EN_PIN    2
#define BT_STATE_PIN 5

#include <SPI.h>
#include <QRDuino.hpp>
#include <LCD.hpp>
#include <BT.hpp>

LCD lcd(LCD_CS_TFT_PIN, LCD_RS_DC_PIN, LCD_RES_PIN);
BT bt(Serial3, BT_EN_PIN, BT_STATE_PIN);
char otp[5];

void setup()
{
  lcd.init();
  bt.init();

  strcpy(otp, "abcde");
  char qrData[strlen(bt.infoForQR()) + strlen(otp) + 1];
  strcpy(qrData, bt.infoForQR());
  strcat(qrData, ",");
  strcat(qrData, otp);

  lcd.drawBTPasskey(bt.pin());
  QRDuino::setData(qrData);
  QRDuino::encode();
  lcd.drawQR();
}

void loop()
{

}
