#include "LCD.hpp"
#include <QRDuino.hpp>

const int LCD::TEXT_H_PADDING = 5;
const int LCD::TEXT_V_PADDING = 13;
const int LCD::H_LINE_FROM_BOTTOM = 20;

const char LCD::BT_STR[] = "BT:";
const char LCD::WIFI_STR[] = "WiFi:";

LCD::LCD(int csTftPin, int rsDcPin, int resPin) : _tft(csTftPin, rsDcPin, resPin)
{ }

void LCD::init()
{
  _tft.initR(INITR_BLACKTAB);
  drawBackground();
}

void LCD::clear()
{
  _tft.fillScreen(ST7735_WHITE);
}

void LCD::drawText(char *text, int x, int y, bool wrap)
{
  _tft.setTextColor(ST7735_BLACK);

  if (x >= 0 && y >= 0)
  {
    _tft.setCursor(x, y);
  }

  _tft.setTextWrap(wrap);
  _tft.print(text);
}

void LCD::drawBackground()
{
  clear();
  _tft.drawFastHLine(0, _tft.height() - H_LINE_FROM_BOTTOM, _tft.width(), ST7735_BLACK);

  _tft.setTextColor(ST7735_BLACK);

  _tft.setCursor(TEXT_H_PADDING, _tft.height() - TEXT_V_PADDING);
  _tft.print(BT_STR);

  _tft.setCursor(_tft.width() / 2 + TEXT_H_PADDING, _tft.height() - TEXT_V_PADDING);
  _tft.print(WIFI_STR);
}

void LCD::drawQR()
{
  int blockSize = floor((_tft.width() - 2) / QRDuino::size());
  int startX = (_tft.width() - QRDuino::size() * blockSize) / 2;
  int startY = (_tft.height() - TEXT_V_PADDING - QRDuino::size() * blockSize) / 2;

  _tft.fillRect(startX, startY, QRDuino::size() * blockSize, QRDuino::size() * blockSize, ST7735_WHITE);

  for (int y = 0; y < QRDuino::size(); y++)
  {
    for (int x = 0; x < QRDuino::size(); x++)
    {
      _tft.fillRect(x * blockSize + startX, y * blockSize + startY, blockSize, blockSize,
          QRDuino::getAt(x,y) ? ST7735_BLACK : ST7735_WHITE);
    }
  }
}

void LCD::drawBTPasskey(const char *status)
{
  _tft.setTextColor(ST7735_BLACK);
  _tft.setCursor(TEXT_H_PADDING, _tft.height() - TEXT_V_PADDING);
  _tft.print(BT_STR);
  _tft.print(status);
}

void LCD::drawWiFiStatus(const char *status)
{
  _tft.setTextColor(ST7735_BLACK);
  _tft.setCursor(_tft.width() / 2 + TEXT_H_PADDING, _tft.height() - TEXT_V_PADDING);
  _tft.print(WIFI_STR);
  _tft.print(status);
}

void LCD::drawSuccess()
{
  _tft.fillScreen(ST7735_GREEN);
}

void LCD::drawFail()
{
  _tft.fillScreen(ST7735_RED);
}
