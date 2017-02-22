#ifndef __QRDUINO_H__INCLUDE__
#define __QRDUINO_H__INCLUDE__ 1

#include <Adafruit_ST7735.h>

class QRDuino
{
public:
  static void setData(char text[]);
  static void encode();
  static int size();
  static int getAt(int x, int y);
};

#endif
