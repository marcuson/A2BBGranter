#ifndef __LCD_H___INCLUDE__
#define __LCD_H___INCLUDE__ 1

#include <Adafruit_ST7735.h>

class LCD
{
private:
  static int TEXT_H_PADDING;
  static int TEXT_V_PADDING;
  static int H_LINE_FROM_BOTTOM;
  static char BT_STR[];
  static char WIFI_STR[];

  Adafruit_ST7735 _tft;

public:
  LCD(int cs_tft, int rs_dc, int res);
  void init();
  void drawBackground();
  void drawQR();
  void drawBTPasskey(const char *status);
  void drawWiFiStatus(const char *status);
};

#endif
