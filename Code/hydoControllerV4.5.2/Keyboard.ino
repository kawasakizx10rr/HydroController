// load the keybaord for the given profile pressed
void loadKeyBoard(const byte& a_profileNum) {
  Serial.println(F("loaded keyboard"));
  beep();
  device::profileInputNumber = a_profileNum;
  device::intputPosition = 0;
  display::showKeyboard = true;
  device::updateKeyboardInput = true;
  display::refreshPage = true;
  clearPage();
}

// This is the function requiered to display the keybaord,
// note the touch events is handeled in the "touch" tab
void keyBoard() {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK);
  tft.fillRect(120, 195, 650, 230, RA8875_WHITE);
  int keyboardColumn = 135;
  int keyboardRow = 260;
  tft.setCursor(keyboardColumn + 10, keyboardRow - 60);
  tft.print(F("."));
  keyboardColumn += 60;
  for (byte i = 0; i < 10; i++) {
    tft.setCursor(keyboardColumn, keyboardRow - 60);
    tft.print(i);
    keyboardColumn += 60;
  }
  keyboardColumn = 135;
  keyboardRow = 260;
  char character = 'a';
  if (device::upper)
    character = 'A';
  for (byte i = 0; i < 26; i++) {
    tft.setCursor(keyboardColumn, keyboardRow);
    tft.print(character);
    character++;
    if (i == 10 || i == 21) {
      keyboardColumn = 135;
      keyboardRow += 60;
    }
    else {
      keyboardColumn += 60;
    }
  }
  tft.setCursor(keyboardColumn, keyboardRow);
  tft.print(F("_")); // " " for space
  keyboardColumn += 60;
  tft.setCursor(keyboardColumn, keyboardRow);
  tft.print(F("del"));
  keyboardColumn += 90;
  tft.setCursor(keyboardColumn, keyboardRow);
  tft.print(F("enter"));
  keyboardColumn += 130;
  tft.setCursor(keyboardColumn, keyboardRow);
  if (device::upper)
    tft.print(F("lower"));
  else
    tft.print(F("upper"));
  tft.setFont(&myriadPro_32px_Regular);
  tft.setTextColor(RA8875_BLACK);
  tft.setCursor(330, 155);
  tft.print(F("10"));
  tft.setCursor(365, 155);
  tft.print(F("characters"));
  tft.setCursor(487, 155);
  tft.print(F("max"));
}

// the function to diaply the input box
void keyBoardInput(const char* a_text) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK);
  tft.fillRect(262, 110, 346, 42, RA8875_WHITE);
  tft.drawRect(260, 108, 350, 46, RA8875_BLACK);
  tft.drawRect(261, 109, 348, 44, RA8875_BLACK);
  tft.setCursor(272, 110);
  tft.print(a_text);
  // work out the cursor position
  for (byte c = 0; c < 10; c++) {
    if (a_text[c] == 0 || c == 9) {
      display::cursourPosition = tft.getFontX();
      break;
    }
  }
}

// the blinking cursor within the input box
void showCursor() {
  static bool blinkCursor = true;
  static unsigned long inputPreviousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - inputPreviousMillis >= 500UL) {
    blinkCursor = !blinkCursor;
    inputPreviousMillis = currentMillis;
  }
  if (blinkCursor) {
    tft.drawLine(display::cursourPosition - 3, 111, display::cursourPosition - 3, 148, RA8875_BLACK);
    tft.drawLine(display::cursourPosition - 2, 111, display::cursourPosition - 2, 148, RA8875_BLACK);
  }
  else {
    tft.drawLine(display::cursourPosition - 3, 111, display::cursourPosition - 3, 148, RA8875_WHITE);
    tft.drawLine(display::cursourPosition - 2, 111, display::cursourPosition - 2, 148, RA8875_WHITE);
  }
}
