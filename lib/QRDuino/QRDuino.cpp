#include "QRDuino.hpp"
#include "qrencode.h"
#include "qrencode.c"
#include <string.h>
#include <Adafruit_ST7735.h>
#include <math.h>

void QRDuino::setData(char text[])
{
  memcpy(strinbuf, text, strlen(text));
}

void QRDuino::encode()
{
  qrencode();
}

int QRDuino::size()
{
  return WD;
}

int QRDuino::getAt(int x, int y)
{
  return QRBIT(x, y);
}
