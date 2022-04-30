#include <SPI.h>
#include <RA8875.h>

// All pins below can be adjust as required except those with notes saying not too...
namespace pin {
const byte ra8875_int = 2;
const byte ra8875_rst = 33;
const byte ra8875_cs = 34;
const byte ra8875_sdo = 50;
const byte ra8875_sdi = 51;
const byte ra8875_sck = 52;
}

RA8875 tft = RA8875(pin::ra8875_cs, pin::ra8875_rst);
uint16_t touchX, touchY;

void interface() {
  tft.fillRect(10, 10, 40, 40, RA8875_WHITE);
  tft.fillRect(10 + (40 * 1) + (10 * 1), 10, 40, 40, RA8875_BLUE);
  tft.fillRect(10 + (40 * 2) + (10 * 2), 10, 40, 40, RA8875_RED);
  tft.fillRect(10 + (40 * 3) + (10 * 3), 10, 40, 40, RA8875_GREEN);
  tft.fillRect(10 + (40 * 4) + (10 * 4), 10, 40, 40, RA8875_CYAN);
  tft.fillRect(10 + (40 * 5) + (10 * 5), 10, 40, 40, RA8875_MAGENTA);
  tft.fillRect(10 + (40 * 6) + (10 * 6), 10, 40, 40, RA8875_YELLOW);
}

void setup() {
  Serial.begin(115200);
  Serial.println("RA8875 easy paint example");
  tft.begin(RA8875_800x480); // BuyDisplay
  //tft.begin(Adafruit_800x480);
  tft.useINT(pin::ra8875_int);
  tft.touchBegin();
  tft.enableISR(true);
  tft.fillWindow(0xA67E);
  interface();
}

uint16_t choosenColor = RA8875_WHITE;

void loop() {
  if (tft.touched()) {
    unsigned int touchX, touchY;
    tft.touchReadPixel(&touchX, &touchY);
    if (touchY >= 10 && touchY <= 55 && touchX > 10 && touchX < 50) {
      choosenColor = RA8875_WHITE;
      interface();
      tft.fillRect(10, 10, 40, 40, RA8875_BLACK);
    }
    else if (touchY >= 10 && touchY <= 55 && touchX > 60 && touchX < 100) {
      choosenColor = RA8875_BLUE;
      interface();
      tft.fillRect(60, 10, 40, 40, RA8875_BLACK);
    }
    else if (touchY >= 10 && touchY <= 55 && touchX > 110 && touchX < 150) {
      choosenColor = RA8875_RED;
      interface();
      tft.fillRect(110, 10, 40, 40, RA8875_BLACK);
    }
    else if (touchY >= 10 && touchY <= 55 && touchX > 160 && touchX < 200) {
      choosenColor = RA8875_GREEN;
      interface();
      tft.fillRect(160, 10, 40, 40, RA8875_BLACK);
    }
    else if (touchY >= 10 && touchY <= 55 && touchX > 210 && touchX < 250) {
      choosenColor = RA8875_CYAN;
      interface();
      tft.fillRect(210, 10, 40, 40, RA8875_BLACK);
    }
    else if (touchY >= 10 && touchY <= 55 && touchX > 260 && touchX < 300) {
      choosenColor = RA8875_MAGENTA;
      interface();
      tft.fillRect(260, 10, 40, 40, RA8875_BLACK);
    }
    else if (touchY >= 10 && touchY <= 55 && touchX > 310 && touchX < 350) {
      choosenColor = RA8875_YELLOW;
      interface();
      tft.fillRect(310, 10, 40, 40, RA8875_BLACK);
    }
    else { //paint
      tft.drawPixel(touchX, touchY, choosenColor);
    }
  }
}
