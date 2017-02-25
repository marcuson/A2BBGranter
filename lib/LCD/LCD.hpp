#ifndef __LCD_H___INCLUDE__
#define __LCD_H___INCLUDE__

#include <Adafruit_ST7735.h>

class LCD
{
private:
  Adafruit_ST7735 _tft;

public:
  static const int TEXT_H_PADDING;
  static const int TEXT_V_PADDING;
  static const int H_LINE_FROM_BOTTOM;
  static const char BT_STR[];
  static const char WIFI_STR[];

  LCD(int csTftPin, int rsDcPin, int resPin);
  void init();
  void clear();
  void drawText(char *text, int x = -1, int y = -1, bool wrap = false);
  void drawBackground();
  void drawQR();
  void drawBTPasskey(const char *status);
  void drawWiFiStatus(const char *status);
  void drawSuccess();
  void drawFail();
};

#endif
