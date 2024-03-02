void launchCo2Notification(uint8_t& a_continueCo2Control, uint8_t& a_previousDate) {
  uint8_t co2DialogTimer = 60;
  uint8_t previousCo2Timer = 0;
  bool refreshText = false;
  uint32_t continuePreviousMillis = millis();
  tft.backlight(true);
  tft.displayOn(true);
  if (device::globalDebug)
    Serial.println(F("About to show co2 cancel timer dialog"));
  saveLogMessage(3);
  while (co2DialogTimer != 0 && a_continueCo2Control == device::NOT_SET) {
    if (co2DialogTimer != previousCo2Timer) {
      continueMessage(message::co2Alert, co2DialogTimer, 0, true, true, refreshText);
      previousCo2Timer = co2DialogTimer;
    }
    if (millis() - continuePreviousMillis >= 1000UL) {
      if (co2DialogTimer > 0)
        co2DialogTimer--;
      refreshText = true;
      continuePreviousMillis = millis();
    }
    // touch event
    if (tft.touched()) {
      tft.touchReadPixel(&display::touch_x, &display::touch_y);
      if (display::touch_x >= 276 && display::touch_x <= 416 && display::touch_y >= 366 && display::touch_y <= 412) { // Cancel
        a_previousDate = rtc.day();
        a_continueCo2Control = device::CANCEL;
        beep();
        clearPage();
        display::refreshPage = true;
      }
      else if (display::touch_x >= 450 && display::touch_x <= 628 && display::touch_y >=  366 && display::touch_y <= 412) { // Continue
        a_continueCo2Control = device::CONTINUE;
        beep();
      }
    }
    updateRelayTimers(); 
  }
}

bool abortCo2Notification(uint32_t& a_lastTouch, const int16_t& a_co2RunTime) {
  static int16_t prevCo2RunTime = -1;
  if (prevCo2RunTime != a_co2RunTime) {
    continueMessage(message::cancelCo2, a_co2RunTime / 60000, 0, false, true, true);
    prevCo2RunTime = a_co2RunTime;
  }
  if (tft.touched()) {
    tft.touchReadPixel(&display::touch_x, &display::touch_y);
    const uint16_t startX = 166, startY = 166;
    if (millis() - a_lastTouch >= 5000UL) {
      if (display::touch_x >= startX + 200 && display::touch_x <= startX + 400 && display::touch_y >= startY + 200 && display::touch_y <= startY + 250) { // Cancel     
        if (device::globalDebug)
          Serial.println(F("Aborted pumping Co2!"));
        sensor::co2GasTime = 0;
        device::co2DisabledFans = false;
        clearPage();
        display::refreshPage = true;
        return true;
      }
    }
  }
  return false;
}

void launchDrainNotification(uint8_t& a_continueDraining, bool& a_startDraining) {
  bool refreshText = false;
  uint8_t drainingDialogTimer = 60;
  uint8_t previousDrainingTimer = 0;
  uint32_t continuePreviousMillis = millis();
  tft.backlight(true);
  tft.displayOn(true);
  if (device::globalDebug)
    Serial.println(F("About to show the drain cancel timer dialog"));
  while (drainingDialogTimer != 0 && a_continueDraining == device::NOT_SET) {
    if (drainingDialogTimer != previousDrainingTimer) {
      continueMessage(message::drainingAlert, drainingDialogTimer, 0, true, true, refreshText);
      previousDrainingTimer = drainingDialogTimer;
    }
    if (millis() - continuePreviousMillis >= 1000UL) {
      if (drainingDialogTimer > 0) {
        drainingDialogTimer--;
        refreshText = true;
      }
      continuePreviousMillis = millis();
    }
    // touch event
    if (tft.touched()) {
      tft.touchReadPixel(&display::touch_x, &display::touch_y);
      if (display::touch_x >= 276 && display::touch_x <= 416 && display::touch_y >= 366 && display::touch_y <= 412) { // Cancel
        a_continueDraining = device::CANCEL;
        a_startDraining = false;
        beep();
        clearPage();
        display::refreshPage = true;
      }
      else if (display::touch_x >= 450 && display::touch_x <= 628 && display::touch_y >=  366 && display::touch_y <= 412) { // Continue
        a_continueDraining = device::CONTINUE;
        beep();
        display::refreshPage = true;
      }
    }
    updateRelayTimers();
  }
}

void launchRefillNotification(bool& a_startRefilling, uint8_t& a_continueRefilling) {
  // Call refill function
  if (a_startRefilling) {
    bool refreshText = false;
    uint8_t refillingDialogTimer = 60;
    uint8_t previousRefillingTimer = 0;
    uint32_t continuePreviousMillis = millis();
    tft.backlight(true);
    tft.displayOn(true);
    if (device::globalDebug)
      Serial.println(F("About to show refill cancel timer dialog"));
    while (refillingDialogTimer != 0 && a_continueRefilling == device::NOT_SET) {
      if (refillingDialogTimer != previousRefillingTimer) {
        continueMessage(message::refillingAlert, refillingDialogTimer, 0, true, true, refreshText);
        previousRefillingTimer = refillingDialogTimer;
      }
      if (millis() - continuePreviousMillis >= 1000UL) {
        if (refillingDialogTimer > 0)
          refillingDialogTimer--;
        refreshText = true;
        continuePreviousMillis = millis();
      }
      // touch event
      if (tft.touched()) {
        tft.touchReadPixel(&display::touch_x, &display::touch_y);
        if (display::touch_x >= 276 && display::touch_x <= 416 && display::touch_y >= 366 && display::touch_y <= 412) { // Cancel
          a_continueRefilling = device::CANCEL;
          a_startRefilling = false;
          beep();
          clearPage();
          display::refreshPage = true;
        }
        else if (display::touch_x >= 450 && display::touch_x <= 628 && display::touch_y >=  366 && display::touch_y <= 412) { // Continue
          a_continueRefilling = device::CONTINUE;
          beep();
        }
      }
      updateRelayTimers(); 
    }
  }
}

void launchDosingNotification(const float& a_sensorPercent, const uint8_t& a_dosingMode, uint32_t& a_lastTouch) {
  uint8_t dosingDialogTimer = 60;
  bool refreshDosingNotification = true;
  uint32_t continuePreviousMillis = millis();
  device::continueDosing = device::NOT_SET;
  tft.backlight(true);
  tft.displayOn(true);
  while (dosingDialogTimer > 0 && device::continueDosing == device::NOT_SET) {
    displayDosingNotification(dosingDialogTimer, a_sensorPercent, a_dosingMode, refreshDosingNotification);
    if (millis() - continuePreviousMillis >= 1000UL) {
      dosingDialogTimer--;
      continuePreviousMillis = millis();
    }
    updateRelayTimers();
  }
  a_lastTouch = millis() + 5000UL;
}

// Draw the left and top menu icons
void OuterMenuIcons() {
  // left menu
  const uint32_t iconTime = millis();
  drawPageIcon(0, 17, 102, homeIcon, 462, 66, 56); // 377
  drawPageIcon(1, 17, 187, graphIcon, 371, 66, 45);
  drawPageIcon(2, 16, 257, minMaxIcon, 310, 69, 36);
  drawPageIcon(3, 22, 324, presetsIcon, 245, 53, 37);
  drawPageIcon(4, 21, 389, settingsIcon, 450, 60, 60);
  // top menu
  if (user::convertToTds)
    drawPageIcon(5, 23, 32, ppmIcon, 280, 64, 35);
  else
    drawPageIcon(5, 26, 32, ecIcon, 256, 57, 36);
  drawPageIcon(6, 125, 32, phIcon, 274, 61, 36);
  drawPageIcon(7, 225, 32, co2Icon, 369, 72, 41);
  drawPageIcon(8, 339, 19, waterTemperatureIcon, 418, 54, 62);
  drawPageIcon(9, 433, 17, doserIcon, 468, 55, 68); 
  drawPageIcon(10, 517, 15, timerIcon, 480, 60, 64);
  drawPageIcon(11, 604, 16, fanIcon, 512, 64, 64);
  drawPageIcon(12, 703, 20, warningsIcon, 511, 67, 61);
  display::previousPage = display::page;
  if (device::globalDebug) {
    Serial.print(F("icon time: ")); Serial.println(millis() - iconTime);
  }
}

// Draw the black frame you see around the menu icons
void frame() {
  tft.drawRect(9, 9, 780, 80, RA8875_BLACK);
  tft.drawRect(10, 10, 780, 80, RA8875_BLACK);
  tft.drawRect(9, 89, 80, 364, RA8875_BLACK);
  tft.drawRect(10, 89, 80, 365, RA8875_BLACK);
}

// Draw a menu icon
void drawPageIcon(const uint8_t& a_page, const uint16_t& a_x, const uint16_t a_y, const uint8_t* a_icon, const int16_t& a_arraySize, const uint8_t& a_width, const uint8_t& a_height) {
  if (display::page == a_page) {
    tft.drawXBMP(a_x, a_y, a_icon, a_arraySize, a_width, a_height, RA8875_BLUE, user::backgroundColor, 1);
  }
  else if (display::previousPage == a_page || display::refreshPage) {
    tft.drawXBMP(a_x, a_y, a_icon, a_arraySize, a_width, a_height, display::RA8875_DARKGREY, user::backgroundColor, 1);
  }
  //delay(1);
  //tft.drawPixel(1, 1, user::backgroundColor);
}

// draws the mini icons to swap between sensors on the graph page
void drawGraphIcons() {
  tft.drawXBMP(747, 98, miniPmIcon, 115, 40, 23, display::showTdsGraph ? display::RA8875_PURPLE : display::RA8875_DARKGREY, user::backgroundColor);
  tft.drawXBMP(747, 131, miniPhIcon, 112, 39, 23, display::showPhGraph ? display::RA8875_LIGHT_GREEN : display::RA8875_DARKGREY, user::backgroundColor);
  tft.drawXBMP(748, 166, miniEcIcon, 110, 38, 23, display::showEcGraph ? RA8875_YELLOW : display::RA8875_DARKGREY, user::backgroundColor);
  tft.drawXBMP(746, 199, miniCo2Icon, 130, 40, 26, display::showCo2Graph ? RA8875_MAGENTA : display::RA8875_DARKGREY, user::backgroundColor);
  tft.drawXBMP(753, 234, miniWaterTempIcon, 109, 27, 32, display::showWaterTempGraph ? display::RA8875_ORANGE : display::RA8875_DARKGREY, user::backgroundColor);
  tft.drawXBMP(754, 273, miniWaterLevelIcon, 101, 27, 30, display::showWaterLvlGraph ? RA8875_BLUE : display::RA8875_DARKGREY, user::backgroundColor);
  tft.drawXBMP(749, 312, miniFanOneIcon, 158, 35, 36, display::showFanOneGraph ? RA8875_WHITE : display::RA8875_DARKGREY, user::backgroundColor);
  tft.drawXBMP(749, 356, miniFanTwoIcon, 158, 35, 36, display::showFanTwoGraph ? RA8875_BLACK : display::RA8875_DARKGREY, user::backgroundColor);
  tft.drawXBMP(754, 396, miniAirTempIcon, 122, 27, 36, display::showAirTempGraph ? RA8875_RED : display::RA8875_DARKGREY, user::backgroundColor);
  tft.drawXBMP(750, 438, miniHumidityIcon, 136, 34, 32, display::showHumidityGraph ? display::RA8875_SEABLUE : display::RA8875_DARKGREY, user::backgroundColor);
  tft.drawRect(742, 88, 48, 391, RA8875_BLACK);
  tft.drawRect(743, 89, 46, 389, RA8875_BLACK);
}

// draw an overal EC and PH+ or PH- button, seen in the doser pages.
void drawEcPhButton(const int16_t& a_x, const int16_t& a_y, const uint8_t& a_doserMode, uint8_t& a_PreviousDoserMode, const bool& a_buttonBackground) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  if (display::refreshPage || a_doserMode != a_PreviousDoserMode) {
    if (a_buttonBackground && a_doserMode != device::DOSER_OFF) {
      tft.fillCircle(a_x, a_y, 16, RA8875_GREEN);
      tft.fillRect(a_x - 2, a_y - 17, 57, 35, RA8875_GREEN);
      tft.fillCircle(a_x + 53, a_y, 16, RA8875_GREEN);
      tft.setTextColor(RA8875_BLACK, RA8875_GREEN);
    }
    else if (a_buttonBackground && a_doserMode == device::DOSER_OFF) {
      tft.fillCircle(a_x, a_y, 16, RA8875_RED);
      tft.fillRect(a_x - 2, a_y - 17, 57, 35, RA8875_RED);
      tft.fillCircle(a_x + 53, a_y, 16, RA8875_RED);
      tft.setTextColor(RA8875_BLACK, RA8875_RED);
    }
    if (a_doserMode == device::DOSER_OFF) {
      tft.print(a_x - 2, a_y - 20, F("OFF"));
    }
    else if (a_doserMode == device::DOSER_PH_UP) {
      tft.print(a_x - 2, a_y - 20, F("PH+"));
    }
    else if (a_doserMode == device::DOSER_PH_DOWN) {
      tft.print(a_x - 2, a_y - 20, F("PH-"));
    }
    else if (a_doserMode == device::DOSER_EC) {
      tft.print(a_x + 6, a_y - 19, F("EC"));
    }
    else if (a_doserMode == device::DOSER_EC_OP) {
      tft.print(a_x - 5, a_y - 19, F("EC#"));
    }
    a_PreviousDoserMode = a_doserMode;
  }
}

// As the function says, this is the screen saver function to save power
// and increase display life time - screen burning.
// Works based on the currentTouch which is the last time the display was touched in ms
// and the afkTime set in the GUI settings.
void screenSaver() {
  if (user::afkTime != 0 && (millis() - display::lastTouchMillis > user::afkTime * 60000UL)) { 
    if (!display::displayIsOff) {
      if (device::globalDebug)
        Serial.println(F("Turning display off"));   
      tft.backlight(false);
      tft.displayOn(false);
      //tft.sleep(true);
      display::displayIsOff = true;
    }
  }
  else if (display::displayIsOff) {
    if (device::globalDebug)
      Serial.println(F("Turning display on"));
    tft.backlight(true);
    tft.displayOn(true);
    //tft.sleep(false);
    display::displayIsOff = false;
  }
}

// Display a new page
void setPage(const uint8_t& a_page) {
  if (display::page != a_page) {
    beep();
    display::page = a_page;
    OuterMenuIcons();
    display::showInfoDialog = false;
    display::refreshPage = true;
    clearPage();
    device::sensorPreviousMillis = millis();
  }
}

// Clear the main draw area, which does not include the outer menus
void clearPage() {
  tft.fillRect(91, 91, 709, 389, user::backgroundColor);
}

void setTextWarningColor(const uint8_t& a_errorState) {
  if (a_errorState == device::NO_WARNING)
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
  else if (a_errorState == device::MINOR_WARNING)
    tft.setTextColor(RA8875_YELLOW, user::backgroundColor);
  else if (a_errorState == device::MAJOR_WARNING)
    tft.setTextColor(RA8875_RED, user::backgroundColor);
}
// Change the display background color
// TO DO: 3 part colour theme so that text and icons have a different color too.
void setBackground(const uint16_t& a_color) {
  beep();// touch tone type 0
  if (user::backgroundColor != a_color) {
    user::backgroundColor = a_color;
    tft.fillWindow(user::backgroundColor);
    frame();
    display::refreshPage = true;
    OuterMenuIcons();
  }
}

// Draw 4 pairs of increment buttons, evently spaced.
void drawFourUpDownButtons(const int16_t& a_yOffset) {
  for (uint8_t i = 0; i < 4; i++) {
    tft.fillCircle(227 + (i * 178), 365 + a_yOffset, 30, RA8875_BLUE);
    tft.fillTriangle(227 + (i * 178), 350 + a_yOffset, 247 + (i * 178), 375 + a_yOffset, 207 + (i * 178), 375 + a_yOffset, RA8875_BLACK);
    tft.fillCircle(139 + (i * 178), 365 + a_yOffset, 30, RA8875_BLUE);
    tft.fillTriangle(139 + (i * 178), 377 + a_yOffset, 159 + (i * 178), 352 + a_yOffset, 119 + (i * 178), 352 + a_yOffset, RA8875_BLACK);
  }
}

// Draw 3 pairs of increment buttons, evently spaced.
void drawThreeUpDownButtons() {
  for (uint8_t i = 0; i < 3; i++) {
    tft.fillCircle(184 + (i * 205), 365, 30, RA8875_BLUE);
    tft.fillTriangle(184 + (i * 205), 377, 204 + (i * 205), 352, 164 + (i * 205), 352, RA8875_BLACK);
    tft.fillCircle(262 + (i * 205), 365, 30, RA8875_BLUE);
    tft.fillTriangle(262 + (i * 205), 350, 282 + (i * 205), 375, 242 + (i * 205), 375, RA8875_BLACK);
  }
}

// Draw 2 pairs of increment buttons, at a given x y
void drawUpDownButtons(const int16_t& a_x1, const int16_t& a_y1, const int16_t& a_x2, const int16_t& a_y2, const uint16_t& a_color) {
  tft.fillCircle(a_x1, a_y1, 30, a_color);
  tft.fillTriangle(a_x1, a_y1 + 12, a_x1 + 20, a_y1 - 13, a_x1 - 20, a_y1 - 13, RA8875_BLACK);
  tft.fillCircle(a_x2, a_y2, 30, a_color);
  tft.fillTriangle(a_x2, a_y2 - 15, a_x2 + 20, a_y2 + 10, a_x2 - 20, a_y2 + 10, RA8875_BLACK);
}

void drawLeftRightButtons(const int16_t& a_x1, const int16_t& a_y1, const int16_t& a_x2, const int16_t& a_y2, const uint16_t& a_color) {
  tft.fillCircle(a_x1, a_y1, 30, a_color);
  tft.fillTriangle(a_x1 - 13, a_y1 - 18, a_x1 - 13, a_y1 + 18, a_x1 + 15, a_y1, RA8875_BLACK);
  tft.fillCircle(a_x2, a_y2, 30, a_color);
  tft.fillTriangle(a_x2 + 13, a_y2 - 18, a_x2 + 13, a_y2 + 18, a_x2 - 15, a_y2, RA8875_BLACK);
}

// Draw a pair of mini increment buttons, at a given x y
void drawMiniButtonIncrements(const int16_t& a_x, const int16_t& a_y) {
  tft.fillCircle(a_x, a_y, 15, RA8875_BLUE);
  tft.fillTriangle(a_x, a_y + 8, a_x + 10, a_y - 5, a_x - 10, a_y - 5, RA8875_BLACK);
  tft.fillCircle(a_x + 80, a_y, 15, RA8875_BLUE);
  tft.fillTriangle(a_x + 80, a_y - 7, a_x + 90, a_y + 5, a_x + 70, a_y + 5, RA8875_BLACK);
}

// Draw a confirm button
void drawMiniConfirmButton(const int16_t& a_x, const int16_t& a_y) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK, display::RA8875_DARKGREY);
  tft.fillRoundRect(a_x, a_y, 70, 38, 5, display::RA8875_DARKGREY);
  tft.drawRoundRect(a_x, a_y, 70, 38, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x + 1, a_y + 1, 68, 36, 5, RA8875_BLACK);
  tft.print(a_x + 12, a_y - 1, F("ok"));
}

void drawEcConversionButton(const int16_t& a_x, const int16_t& a_y) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK, display::RA8875_DARKGREY);
  tft.fillRoundRect(a_x, a_y, 70, 38, 5, display::RA8875_DARKGREY);
  tft.drawRoundRect(a_x, a_y, 70, 38, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x + 1, a_y + 1, 58, 36, 5, RA8875_BLACK);
  tft.setCursor(a_x + 12, a_y - 1);
  if (device::conversionType == device::EU)
    tft.print(F("EU"));
  else if (device::conversionType == device::US)
    tft.print(F("US"));
  else if (device::conversionType == device::AU)
    tft.print(F("AU"));
}

// Draw a radio button, based on bool state
void drawRadioButton(const int16_t& a_x, const int16_t& a_y, const bool& option, const bool a_bothGreen) {
  tft.fillRoundRect(a_x - 2, a_y - 2, 154, 44, 5, RA8875_BLACK);
  if (option) {
    tft.fillRoundRect(a_x, a_y, 75, 40, 5, display::RA8875_DARKGREY);
    tft.fillRoundRect(a_x + 75, a_y, 75, 40, 5, RA8875_GREEN);
  }
  else {
    tft.fillRoundRect(a_x, a_y, 75, 40, 5, a_bothGreen ? RA8875_GREEN : RA8875_RED);
    tft.fillRoundRect(a_x + 75, a_y, 75, 40, 5, display::RA8875_DARKGREY);
  }
}

// Draw a toggle button, based on bool state
void drawMiniRadioButton(const int16_t& a_x, const int16_t& a_y, const bool & option) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK, display::RA8875_DARKGREY);
  tft.fillRoundRect(a_x, a_y, 90, 38, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x + 1, a_y + 1, 88, 36, 5, RA8875_BLACK);
  if (option) {
    tft.fillRoundRect(a_x, a_y, 45, 38, 5, display::RA8875_DARKGREY);
    tft.drawRoundRect(a_x, a_y, 45, 38, 5, RA8875_BLACK);
    tft.fillRoundRect(a_x + 45, a_y, 45, 38, 5, RA8875_GREEN);
    tft.drawRoundRect(a_x + 45, a_y, 45, 38, 5, RA8875_BLACK);
  }
  else {
    tft.fillRoundRect(a_x, a_y, 45, 38, 5, RA8875_RED);
    tft.drawRoundRect(a_x, a_y, 45, 38, 5, RA8875_BLACK);
    tft.fillRoundRect(a_x + 45, a_y, 45, 38, 5, display::RA8875_DARKGREY);
    tft.drawRoundRect(a_x + 45, a_y, 45, 38, 5, RA8875_BLACK);
  }
}

// Draw a radio button, based on bool state
void drawMiniEtapeButton(const int16_t& a_x, const int16_t& a_y, const user::heightSensors& option) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK, display::RA8875_DARKGREY);
  tft.fillRoundRect(a_x, a_y, 130, 40, 5, display::RA8875_DARKGREY);
  tft.drawRoundRect(a_x, a_y, 130, 40, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x + 1, a_y + 1, 128, 38, 5, RA8875_BLACK);
  if (option == user::SR04)
    tft.print(a_x + 6, a_y - 1, F("SR-04"));
  else if (option == user::ETAPE)
    tft.print(a_x + 14, a_y - 1, F("Etape"));
  else if (option == user::VL53L0X)
    tft.print(a_x + 6, a_y - 1, F("VL53L"));
}

// show the reset message, it is an odd size so i made it seperate.
void showResetMessage() {
  int16_t startX = 136; // makes it eaiser to position
  int16_t startY = 186;
  //Frame
  tft.fillRoundRect(startX - 20, startY, 580, 130, 5, RA8875_WHITE);
  tft.drawRoundRect(startX - 22, startY - 2, 582, 134, 5, RA8875_BLACK);
  tft.drawRoundRect(startX - 22, startY - 1, 581, 132, 5, RA8875_BLACK);
  // Draw text
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK, RA8875_WHITE);
  tft.print(startX + 8, startY, F("Please wait while the"));
  tft.print(startX + 8, startY + 38, F("system loads the settings"));
  tft.print(startX + 8, startY + 76, F("for the first time."));
}

void pleaseWaitMessage() {
  int16_t startX = 166; // makes it eaiser to position
  int16_t startY = 186;
  //Frame
  tft.fillRoundRect(startX - 20, startY, 580, 110, 5, RA8875_WHITE);
  tft.drawRoundRect(startX - 22, startY - 2, 582, 114, 5, RA8875_BLACK);
  tft.drawRoundRect(startX - 22, startY - 1, 581, 112, 5, RA8875_BLACK);
  // Draw text
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK, RA8875_WHITE);
  tft.print(startX + 8, startY, F("Please wait while the"));
  tft.print(startX + 8, startY + 38, F("calibration is preformed..."));
}

// Show the dosing dialog, it has many different states so it is its own function..
void displayDosingNotification(const uint8_t& a_seconds, const float& a_outOfRange, const uint8_t& a_dosingMode, bool& a_refreshDosingNotification) {
  uint16_t startX = 166, startY = 166;
  static uint8_t previousSeconds;
  static int16_t startSecPos, endSecPos;
  if (a_refreshDosingNotification) {
    if (device::globalDebug)
      Serial.println(F("showing dosing alert"));
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    //Frame
    tft.fillRoundRect(startX - 20, startY, 600, 250, 5, RA8875_WHITE); // RA8875_YELLOW
    tft.drawRoundRect(startX - 22, startY - 2, 602, 254, 5, RA8875_BLACK);
    tft.drawRoundRect(startX - 22, startY - 1, 601, 252, 5, RA8875_BLACK);
    // Buttons
    cancelButton(startX + 108, startY + 200);
    continueButton(startX + 283, startY + 200);
    tft.setTextColor(RA8875_BLACK, RA8875_WHITE);
    // =====================================
    tft.setCursor(startX + 118, startY + 8);
    tft.print(F("Alert adjusting "));
    if (a_dosingMode == 0)
      tft.print(F("PH"));
    else if (a_dosingMode == 1)
      tft.print(F("EC"));
    else if (a_dosingMode == 2)
      tft.print(F("PPM"));
    // =====================================
    if (a_outOfRange >= 100)
      tft.setCursor(startX + 4, startY + 48);
    else
      tft.setCursor(startX + 6, startY + 48);
    if (a_dosingMode == 0)
      tft.print(F("PH"));
    else if (a_dosingMode == 1)
      tft.print(F("EC"));
    else if (a_dosingMode == 2)
      tft.print(F("PPM"));
    tft.print(F(" out of range by "));
    tft.print(a_outOfRange);
    tft.print(F(" %"));
    // ====================================
    tft.setCursor(startX + 42, startY + 88);
    tft.print(F("Starting in "));
    startSecPos = tft.getFontX();
    // ===
    tft.setCursor(startX + 24, startY + 138);
    tft.print(F("Do you want to continue?"));
  }
  if (a_refreshDosingNotification || a_seconds != previousSeconds) {
    tft.fillRect(startSecPos, startY + 90, endSecPos - startSecPos, 34, RA8875_WHITE); // RA8875_YELLOW
    tft.setCursor(startSecPos + 2, startY + 88);
    tft.print(a_seconds);
    tft.print(F(" seconds."));
    endSecPos = tft.getFontX() + 2;
    previousSeconds = a_seconds;
  }
  a_refreshDosingNotification = false;
  // touch event
  if (tft.touched()) {
    tft.touchReadPixel(&display::touch_x, &display::touch_y);
    if (display::touch_x >= startX + 110 && display::touch_x <= startX + 250 && display::touch_y >= startY + 200 && display::touch_y <= startY + 246) { // No
      if (device::globalDebug)
        Serial.println(F("Aborting dosing")); 
      device::continueDosing = device::CANCEL;
      beep();
      clearPage();
      display::refreshPage = true;
    }
    else if (display::touch_x >= startX + 284 && display::touch_x <= startX + 462 && display::touch_y >= startY + 200 && display::touch_y <= startY + 246) { // Yes
      if (device::globalDebug)
        Serial.println(F("Proceeding with dosing")); 
      device::continueDosing = device::CONTINUE;
      beep();
      //clearPage();
      display::refreshPage = true;
    }
  }
}

// Show a continue message with the string stored in the Program memory / Flash, and a float which can be excluded if -1
void continueMessage(const char* a_text, const float& a_num, const uint8_t a_precision, const bool& a_showContiue, const bool& a_showCancel, bool a_refresh) {
  uint16_t startX = 166, startY = 166;
  //Frame
  if (!a_refresh) {
    tft.fillRoundRect(startX - 30, startY, 620, 250, 5, RA8875_WHITE);
    tft.drawRoundRect(startX - 31, startY - 1, 622, 252, 5, RA8875_BLACK);
    // Buttons
    if (a_showContiue && a_showCancel) {
      cancelButton(startX + 108, startY + 200);
      continueButton(startX + 283, startY + 200);
    }
    else if (a_showContiue && !a_showCancel) {
      continueButton(startX + 190, startY + 200);
    }
    else if (!a_showContiue && a_showCancel) {
      cancelButton(startX + 200, startY + 200);
    }
  }
  // Draw text
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK, RA8875_WHITE);
  tft.setCursor(startX - 8, startY);
  static int16_t numX = 0, numY = 0;
  for (uint16_t i = 0; i < strlen_P(a_text); i++) {
    char c = pgm_read_byte_near(a_text+i);
    if (c == '$' && a_num != -1) {
      numX = tft.getFontX();
      numY = tft.getFontY();
      if (a_refresh)
        tft.fillRect(numX, numY, 744 - numX, 36, RA8875_WHITE);
      tft.print(a_num, a_precision);
    }
    else if (c == '\n') {
      startY += 38;
      tft.setCursor(startX - 8, startY);
    }
    else 
      tft.print(c);
  }
}

// Show an abort message with the string stored in the Program memory / Flash, and a float and int16_t which can be excluded if -1
void abortMessage(const char *a_text, const char* a_str, const float& a_value, const int16_t& a_doserNum, const float& a_mls, const uint8_t& a_precison, bool a_update) {
  uint16_t startX = 156, startY = 166;
  static uint16_t mlsY = 0;
  if (!a_update) {
    //Frame
    tft.fillRoundRect(startX - 20, startY, 620, 250, 5, RA8875_WHITE);
    tft.drawRoundRect(startX - 21, startY - 1, 622, 252, 5, RA8875_BLACK);
    // Buttons
    cancelButton(startX + 200, startY + 200); // x was 178
  }
  // Draw text
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK, RA8875_WHITE);
  tft.setCursor(startX - 8, startY);
  for (uint16_t i = 0; i < strlen_P(a_text); i++) {
    char c = pgm_read_byte_near(a_text+i);
    if (c == '\n') {
      startY += 38;
      tft.setCursor(startX - 8, startY);
    }
    else if (c == '*' && !a_update)
      tft.print(a_str);
    else if (c == '$' && a_value != -1 && !a_update)
      tft.print(a_value, a_precison);
    else if (c == '#' && a_doserNum != -1 && !a_update)
      tft.print(a_doserNum);
    else if (c == '@' && a_mls != -1){
      if (a_update)
        tft.fillRect(startX - 9, mlsY + 5, 588, 36, RA8875_WHITE);
      tft.print(a_mls, 0);   
      mlsY = tft.getFontY();
      a_update = false;
    }
    else if (!a_update)
      tft.print(c);
  }
}

void infoMessage(const char* const* a_messageArray, const int16_t a_messageIndex) { // message::infoMessageArray, message::infoPos
  char charBuffer[33]{0};
  uint16_t startX = 146, startY = 166;
  uint16_t charPos = 0, fontX = 146, dialogHeight = 40;
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK, RA8875_WHITE);  
  if (display::refreshPage && display::showInfoDialog) {
    const char* text = pgm_read_word(&a_messageArray[a_messageIndex]);
    const uint16_t logLength = strlen_P(text);
    
    if (device::globalDebug) {
      Serial.println(F("Starting infoMessage"));
      Serial.print(F("Log length: ")); Serial.println(logLength);
      //Serial.print(F("Log length: ")); Serial.println((const __FlashStringHelper *)text);
    }
    // Work out the dialog height
    for (uint16_t i = 0; i < logLength; i++) {
      char c = pgm_read_byte_near(text+i);
      if (charPos < 32)
        charBuffer[charPos++] = c;
      if (charPos == 32 || c == ' ' || i == logLength - 1) {
        if (fontX + tft.getStringWidth(charBuffer) > startX + 610) {
          dialogHeight += 40;
          fontX = 146;
        }
        else
          fontX += tft.getStringWidth(charBuffer);
        memset(charBuffer, 0 , 33);
        charPos = 0;
      }
    }
   
    if (charPos > 0 || fontX > 146)
      dialogHeight += 40;
    //Frame
    tft.fillRoundRect(startX - 20, startY, 630, dialogHeight, 5, RA8875_WHITE);
    tft.drawRoundRect(startX - 21, startY - 1, 632, dialogHeight + 2, 5, RA8875_BLACK);
    //closeButton(startX + 200, startY + 250);
    tft.setCursor(startX - 8, startY);
    // Draw text
    charPos = 0;
    memset(charBuffer, 0 , 33);
    for (uint16_t i = 0; i < logLength; i++) {
      char c = pgm_read_byte_near(text+i);
      if (charPos < 32)
        charBuffer[charPos++] = c;
      if (charPos == 32 || c == ' ' || i == logLength - 1) {
        if (tft.getFontX() + tft.getStringWidth(charBuffer) > startX + 610) {
          startY += 38;
          tft.setCursor(startX - 8, startY);
        }
        tft.print(charBuffer);
        memset(charBuffer, 0 , 33);
        charPos = 0;
      }
    }
  }
}

void setupMessage(int16_t& a_x, int16_t& a_y, const char* a_message) {
  char charBuffer[33]{0};
  int16_t charPos = 0, fontX = 0, dialogHeight = 40, dialogWidth = 0;
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK, RA8875_WHITE);
  //clearPage();
  if (device::globalDebug)
    Serial.println(F("Starting setupMessage"));
  // Work out the dialog height
  for (uint16_t i = 0; i < strlen_P(a_message); i++) {
    char c = pgm_read_byte_near(a_message+i);
    if (charPos < 32)
      charBuffer[charPos++] = c;
    if (charPos == 32 || c == '\n' || c == ' ' || i == strlen_P(a_message) - 1) {
      if (fontX > dialogWidth) 
        dialogWidth = fontX;
      if (c == '\n' || (a_x + fontX + tft.getStringWidth(charBuffer) > tft.width() - 10)) { //tft.getStringWidth(charBuffer)
        dialogHeight += 40;
        fontX = 0;
      }
      else
        fontX += tft.getStringWidth(charBuffer);
      if (fontX > dialogWidth) 
        dialogWidth = fontX;
      memset(charBuffer, 0 , 33);
      charPos = 0;
    }
  }
  //if (charPos > 0 || fontX > 0)
  //  dialogHeight += 40;
  if (dialogWidth < 200)
    dialogWidth = 200;
  else if (dialogWidth < tft.width() - 30)
    dialogWidth += 20;

  //Frame
  tft.fillRoundRect(a_x - 20, a_y, dialogWidth, dialogHeight + 55, 5, RA8875_WHITE);
  tft.drawRoundRect(a_x - 21, a_y - 1, dialogWidth+2, dialogHeight + 57, 5, RA8875_BLACK);
  //closeButton(startX + 200, startY + 250);
  tft.setCursor(a_x - 8, a_y);
  // Draw text
  charPos = 0;
  memset(charBuffer, 0 , 33);
  fontX = 0;
  for (uint16_t i = 0; i < strlen_P(a_message); i++) {
    char c = pgm_read_byte_near(a_message+i);
    if (charPos < 32)
      charBuffer[charPos++] = c;
    if (charPos == 32 || c == ' ' || i == strlen_P(a_message) - 1) {
      if (c == '\n' || (tft.getFontX() + tft.getStringWidth(charBuffer) > a_x + dialogWidth)) {
        a_y += 38;
        fontX = 0;
        tft.setCursor(a_x - 8, a_y);
      }
      else
        fontX += tft.getStringWidth(charBuffer);
      tft.print(charBuffer);
      memset(charBuffer, 0 , 33);
      charPos = 0;
    }
  }
  a_x += ((dialogWidth - 194) / 2);
  a_y += dialogHeight + 5;
  continueButton(a_x, a_y);
}

// draw a set button at a given x and y
void infoButton(const int16_t& a_x, const int16_t& a_y) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_WHITE, RA8875_BLUE);
  tft.fillCircle(a_x, a_y, 18, RA8875_BLUE);
  tft.drawCircle(a_x, a_y, 18, RA8875_BLACK);
  tft.drawCircle(a_x, a_y, 19, RA8875_BLACK);
  tft.print(a_x - 2, a_y - 18, F("i"));
}

// draw a set button at a given x and y
void setButton(const int16_t& a_x, const int16_t& a_y) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK, display::RA8875_DARKGREY);
  tft.fillRoundRect(a_x, a_y, 94, 42, 5, display::RA8875_DARKGREY);
  tft.drawRoundRect(a_x - 2, a_y - 2, 98, 46, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x - 1, a_y - 1, 96, 44, 5, RA8875_BLACK);
  tft.print(a_x + 19, a_y + 1, F("Set"));
}

// draw a save button at a given x and y
void saveButton(const int16_t& a_x, const int16_t& a_y) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK, display::RA8875_DARKGREY);
  tft.fillRoundRect(a_x, a_y, 129, 42, 5, display::RA8875_DARKGREY);
  tft.drawRoundRect(a_x - 2, a_y - 2, 133, 46, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x - 1, a_y - 1, 131, 44, 5, RA8875_BLACK);
  tft.print(a_x + 12, a_y, F("Save"));
}

// draw a load button at a given x and y
void loadButton(const int16_t& a_x, const int16_t& a_y) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK, display::RA8875_DARKGREY);
  tft.fillRoundRect(a_x, a_y, 129, 42, 5, display::RA8875_DARKGREY);
  tft.drawRoundRect(a_x - 2, a_y - 2, 133, 46, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x - 1, a_y - 1, 131, 44, 5, RA8875_BLACK);
  tft.print(a_x + 16, a_y, F("Load"));
}

// draw a text input box at a given x and y and text stored in normal memory
void inputBox(const int16_t& a_x, const int16_t& a_y, const char* a_text, const uint16_t a_backColor) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK, a_backColor);
  tft.fillRect(a_x, a_y, 340, 42, a_backColor); // +25px
  tft.drawRect(a_x - 2, a_y - 2, 344, 46, RA8875_BLACK);
  tft.drawRect(a_x - 1, a_y - 1, 342, 44, RA8875_BLACK);
  tft.print(a_x + 5, a_y, a_text);
}

// draw a background color button at a given x and y and 565 colour
void backGroundColorButton(const int16_t& a_x, const int16_t& a_y, const uint16_t& a_color) {
  tft.drawRect(a_x, a_y, 34, 34, RA8875_BLACK);
  tft.drawRect(a_x + 1, a_y + 1, 32, 32, RA8875_BLACK);
  tft.fillRect(a_x + 2, a_y + 2, 30, 30, a_color);
}

// draw a prime button at a given x and y
void primeButton(const int16_t& a_x, const int16_t& a_y) {
  tft.fillCircle(a_x, a_y, 20, RA8875_YELLOW);
  tft.fillRect(a_x - 3, a_y - 20, 100, 41, RA8875_YELLOW);
  tft.fillCircle(a_x + 92, a_y, 20, RA8875_YELLOW);
  tft.setFont(&akashi_36px_Regular);
  tft.setTextColor(RA8875_BLACK, RA8875_YELLOW);
  tft.setFontScale(1);
  tft.print(a_x - 6, a_y - 23, F("prime"));
}

// draw a prime button at a given x and y
void stopButton(const int16_t& a_x, const int16_t& a_y) {
  tft.fillCircle(a_x, a_y, 20, RA8875_RED);
  tft.fillRect(a_x - 3, a_y - 20, 100, 41, RA8875_RED);
  tft.fillCircle(a_x + 92, a_y, 20, RA8875_RED);
  tft.setFont(&akashi_36px_Regular);
  tft.setTextColor(RA8875_BLACK, RA8875_RED);
  tft.setFontScale(1);
  tft.print(a_x, a_y - 23, F("stop"));
}

// draw a exit button at a given x and y
void exitButton(const int16_t& a_x, const int16_t& a_y) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK, display::RA8875_DARKGREY);
  tft.fillRoundRect(a_x, a_y, 94, 42, 5, display::RA8875_DARKGREY);
  tft.drawRoundRect(a_x - 2, a_y - 2, 98, 46, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x - 1, a_y - 1, 96, 44, 5, RA8875_BLACK);
  tft.print(a_x + 14, a_y + 1, F("Exit"));
}

// draw a Cancel button at a given x and y
void cancelButton(const int16_t& a_x, const int16_t& a_y) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK, display::RA8875_DARKGREY);
  tft.fillRoundRect(a_x, a_y, 148, 42, 5, display::RA8875_DARKGREY);
  tft.drawRoundRect(a_x - 2, a_y - 2, 152, 46, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x - 1, a_y - 1, 150, 44, 5, RA8875_BLACK);
  tft.print(a_x + 14, a_y + 1, F("Cancel"));
}

// draw a Close button at a given x and y
void closeButton(const int16_t& a_x, const int16_t& a_y) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK, display::RA8875_DARKGREY);
  tft.fillRoundRect(a_x, a_y, 148, 42, 5, display::RA8875_DARKGREY);
  tft.drawRoundRect(a_x - 2, a_y - 2, 152, 46, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x - 1, a_y - 1, 150, 44, 5, RA8875_BLACK);
  tft.print(a_x + 18, a_y + 1, F("Close"));
}

// draw a Continue button at a given x and y
void continueButton(const int16_t& a_x, const int16_t& a_y) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK, display::RA8875_DARKGREY);
  tft.fillRoundRect(a_x, a_y, 190, 42, 5, display::RA8875_DARKGREY);
  tft.drawRoundRect(a_x - 2, a_y - 2, 194, 46, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x - 1, a_y - 1, 192, 44, 5, RA8875_BLACK);
  tft.print(a_x + 14, a_y + 1, F("Continue"));
}

// draw a Skip button at a given x and y
void skipButton(const int16_t& a_x, const int16_t& a_y) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK, display::RA8875_DARKGREY);
  tft.fillRoundRect(a_x, a_y, 148, 42, 5, display::RA8875_DARKGREY);
  tft.drawRoundRect(a_x - 2, a_y - 2, 152, 46, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x - 1, a_y - 1, 150, 44, 5, RA8875_BLACK);
  tft.print(a_x + 22, a_y + 1, F("Skip"));
}

// draw a Calibrate button at a given x and y
void calibrateButton(const int16_t& a_x, const int16_t& a_y) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK, display::RA8875_DARKGREY);
  tft.fillRoundRect(a_x, a_y, 190, 42, 5, display::RA8875_DARKGREY);
  tft.drawRoundRect(a_x - 2, a_y - 2, 194, 46, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x - 1, a_y - 1, 192, 44, 5, RA8875_BLACK);
  tft.print(a_x + 14, a_y + 1, F("Calibrate"));
}

// draw the slide show seen on the homepage, attributes seen below...
void drawSensorSlide(const float& a_sensorData, const float& a_minTarget, const float& a_maxTarget, uint16_t* a_arrayData, const bool a_compressed, uint8_t a_percision) {
  if (display::refreshPage) {
    tft.setCursor(540, 140);
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    tft.setFontScale(1);
    tft.print(F("Target"));
  }
  uint8_t percision = a_percision;
  if (a_sensorData >= 1000 && a_percision > 0)
    percision = 0;
  // draw sensor reading
  static float previousSensorData;
  static int16_t sensorDataPosition;
  if (display::refreshPage || hasChanged(a_sensorData, previousSensorData, percision)) {
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setFontScale(2);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    int16_t sensorStartX = 290 - (tft.getStringWidth(a_sensorData, percision) / 2);
    tft.fillRect(98, 188, sensorDataPosition - 96, 90, user::backgroundColor);
    tft.print(sensorStartX, 190, a_sensorData, percision);
    sensorDataPosition = tft.getFontX();
    previousSensorData = a_sensorData;
  }
  // draw targets
  percision = a_percision;
  if (a_minTarget >= 100 && a_percision > 0)
    percision = 0;
  else if (a_minTarget >= 10 && a_percision > 1)
    percision = 1;
  static int16_t minTargetStartPosition, minTargetEndPosition;
  static float previousMinTarget;
  if (display::refreshPage || hasChanged(a_minTarget, previousMinTarget, percision)) {
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    int16_t minTargetStartX = 540 - (tft.getStringWidth(a_minTarget, percision) / 2);
    tft.fillRect(minTargetStartPosition, 189, minTargetEndPosition - minTargetStartPosition, 85, user::backgroundColor);
    tft.print(minTargetStartX, 230, a_minTarget, percision);
    minTargetStartPosition = minTargetStartX;
    minTargetEndPosition = tft.getFontX();
    tft.setFont(&myriadPro_32px_Regular);
    tft.print(minTargetStartX, 186, F("Min"));
    if (tft.getFontX() > minTargetEndPosition)
      minTargetEndPosition = tft.getFontX();
    previousMinTarget = a_minTarget;
  }
  //
  percision = a_percision;
  if (a_maxTarget >= 100 && a_percision > 0)
    percision = 0;
  else if (a_maxTarget >= 10 && a_percision > 1)
    percision = 1;
  static int16_t maxTargetStartPosition, maxTargetEndPosition;
  static float previousMaxTarget;
  if (display::refreshPage || hasChanged(a_maxTarget, previousMaxTarget, percision)) {
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    int16_t maxTargetStartX = 710 - (tft.getStringWidth(a_maxTarget, percision) / 2);
    tft.fillRect(maxTargetStartPosition, 189, maxTargetEndPosition - maxTargetStartPosition, 85, user::backgroundColor);
    tft.print(maxTargetStartX, 230, a_maxTarget, percision);
    maxTargetStartPosition = maxTargetStartX;
    maxTargetEndPosition = tft.getFontX();
    tft.setFont(&myriadPro_32px_Regular);
    tft.print(maxTargetStartX, 186, F("Max"));
    if (tft.getFontX() > maxTargetEndPosition)
      maxTargetEndPosition = tft.getFontX();
    previousMaxTarget = a_maxTarget;
  }
  // draw mini graph
  uint16_t* arrayData[1] {a_arrayData};
  const uint16_t graphColors[1] {RA8875_BLACK};
  const bool compressed[1] {a_compressed};
  if (display::refreshPage || device::newGraphData) 
    drawGraph(95, 300, 164, tft.width() - 95, 5, arrayData, compressed, 1, sensor::sensorArrayPos, a_percision, graphColors);
}

// display the set RTC time dialog
void displaySetRTCTime() {
  if (display::refreshPage) {
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    tft.print(270, 116, F("Set RTC time"));
    drawLeftRightButtons(295, 375, 385, 375, RA8875_BLUE);
    drawUpDownButtons(495, 375, 585, 375, RA8875_BLUE);
    cancelButton(260, 420);
    continueButton(455, 420);
    infoButton(770, 120);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
  }
  // day of week string
  static int16_t previousDaysPosition;
  static uint8_t previousUserRTCdayOfWeek;
  char buffer[8] {0};
  const char* daysOfWeek[7] {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
  if (display::refreshPage || previousUserRTCdayOfWeek != user::rtcDayOfWeek) {
    tft.fillRect(299, 180, previousDaysPosition - 104, 45, user::backgroundColor);
    if (display::setRtcTimePos == 0) {
      tft.setTextColor(RA8875_WHITE, user::backgroundColor);
    }
    tft.print(300, 180, daysOfWeek[user::rtcDayOfWeek]);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    previousDaysPosition = tft.getFontX();
    previousUserRTCdayOfWeek = user::rtcDayOfWeek;
  }
  // date
  static int16_t previousDatePosition;
  static uint8_t previousUserRTCDay;
  static uint8_t previousUserRTCMonth;
  static int8_t previousUserRTCYear;
  if (display::refreshPage || previousUserRTCDay != user::rtcDay || previousUserRTCMonth != user::rtcMonth || previousUserRTCYear != user::rtcYear) {
    char dateStr[16];
    memset(dateStr, 0, 16);
    memset(buffer, 0, 8);
    tft.fillRect(299, 235, previousDatePosition - 299, 45, user::backgroundColor);
    // day
    itoa(user::rtcDay, buffer, 10);
    if (user::rtcDay < 10) {
      strcpy(dateStr, "0");
      strcat(dateStr, buffer);
    }
    else {
      strcpy(dateStr, buffer);
    }
    if (display::setRtcTimePos == 1) {
      tft.setTextColor(RA8875_WHITE, user::backgroundColor);
    }
    tft.print(300, 235, dateStr);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    //
    tft.print(F("/"));
    //month
    memset(dateStr, 0, 16);
    if (user::rtcMonth < 10)
      strcat(dateStr, "0");
    memset(buffer, 0, 8);
    itoa(user::rtcMonth, buffer, 10);  
    strcat(dateStr, buffer);
    if (display::setRtcTimePos == 2) {
      tft.setTextColor(RA8875_WHITE, user::backgroundColor);
    }
    tft.print(dateStr);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    //
    tft.print(F("/"));
    // year
    memset(buffer, 0, 8);
    memset(dateStr, 0, 16);
    itoa(user::rtcYear + 2000, buffer, 10);  
    strcat(dateStr, buffer);
    if (display::setRtcTimePos == 3) {
      tft.setTextColor(RA8875_WHITE, user::backgroundColor);
    }
    tft.print(dateStr);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    previousDatePosition = tft.getFontX();
    previousUserRTCDay = user::rtcDay;
    previousUserRTCMonth = user::rtcMonth;
    previousUserRTCYear = user::rtcYear;
  }
  // time
  static int16_t previousTimePosition;
  static uint8_t previousUserRTCMinute;
  static uint8_t previousUserRTCSecond;
  static uint8_t previousUserRtcHour;
  if (display::refreshPage || previousUserRTCMinute != user::rtcMinute || previousUserRTCSecond != user::rtcSecond || previousUserRtcHour != user::rtcHour) {
    char timeStr[16];
    memset(timeStr, 0, 16);
    memset(buffer, 0, 8);
    tft.fillRect(299, 290, previousTimePosition - 299, 45, user::backgroundColor);
    //
    itoa(user::rtcHour, buffer, 10);
    if (user::rtcHour < 10) {
      strcpy(timeStr, "0");
      strcat(timeStr, buffer);
    }
    else {
      strcpy(timeStr, buffer);
    }
    if (display::setRtcTimePos == 4) {
      tft.setTextColor(RA8875_WHITE, user::backgroundColor);
    }
    tft.print(300, 290, timeStr);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    //
    tft.print(F(":"));
    //
    memset(timeStr, 0, 16);
    if (user::rtcMinute < 10)
      strcpy(timeStr, "0");
    memset(buffer, 0, 8);
    itoa(user::rtcMinute, buffer, 10);
    strcpy(timeStr, buffer);
    if (display::setRtcTimePos == 5) {
      tft.setTextColor(RA8875_WHITE, user::backgroundColor);
    }
    tft.print(timeStr);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    //
    tft.print(F(":"));
    //
    memset(timeStr, 0, 16);
    if (user::rtcSecond < 10)
      strcpy(timeStr, "0");
    memset(buffer, 0, 8);
    itoa(user::rtcSecond, buffer, 10);
    strcpy(timeStr, buffer); 
    if (display::setRtcTimePos == 6) {
      tft.setTextColor(RA8875_WHITE, user::backgroundColor);
    }
    tft.print(timeStr);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    previousUserRTCMinute = user::rtcMinute;
    previousUserRTCSecond = user::rtcSecond;
    previousUserRtcHour = user::rtcHour;
    previousTimePosition = tft.getFontX();
  }
}

// draw a large doser icon its doser number, doser value either mls or speed %
void drawDoser(
  const int16_t& a_x, const int16_t& a_y, const uint8_t a_doserNum, const int16_t& a_value,
  int16_t& a_previousValue, int16_t& a_doserPosition, const uint8_t& a_symbol, const int16_t& a_yOffSet)
{
  int16_t startPosition = 0;
  if (display::refreshPage) {
    tft.drawXBMP(a_x + 17, a_y + 6, doserIcon, 468, 55, 68, RA8875_BLACK, user::backgroundColor, 2);
  }
  if (display::refreshPage || a_value != a_previousValue) {
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    if (a_value < 10)
      startPosition = a_x + 45;
    else if (a_value < 100)
      startPosition = a_x + 25;
    else
      startPosition = a_x + 12;
    tft.setCursor(startPosition, a_y + a_yOffSet + 190);
    tft.fillRect(a_x + 3, a_y + a_yOffSet + 188, a_doserPosition - a_x + 3, 50, user::backgroundColor);
    tft.print(a_value);
    a_doserPosition = tft.getFontX();
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setCursor(a_doserPosition + 4, a_y + a_yOffSet + 200);
    if (a_doserPosition < 800 - tft.getStringWidth("ml"))
      a_symbol == 0 ? tft.print(F("ml")) : tft.print(F("%"));
    a_doserPosition = tft.getFontX();
    tft.setCursor(a_x, a_y);
    tft.print(a_doserNum);
    a_previousValue = a_value;
  }
}

// draw the circle slide position icons
void drawSlideIcons(int16_t a_x, const int16_t& a_y, const uint8_t& a_page, const uint8_t& numOfIcons) {
  for (uint8_t i = 0; i < numOfIcons; i++) {
    if (a_page == i)
      tft.fillCircle(a_x, a_y, 12, RA8875_BLUE);
    else
      tft.fillCircle(a_x, a_y, 10, display::RA8875_DARKGREY);
    a_x += 30;
  }
}

void drawCyclicTimer(int16_t& a_textStartX, int16_t& a_textEndX, const int16_t a_textCenterX, const uint16_t a_startY, const int16_t a_timer, int16_t& a_previousTimer) {
  if (display::refreshPage || a_previousTimer != a_timer) {
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setFontScale(1);
    tft.fillRect(a_textStartX, a_startY-1, a_textEndX - a_textStartX, 50, user::backgroundColor);
    int16_t stringWidth = tft.getStringWidth((int16_t)(a_timer / 60));
    tft.setFont(&akashi_36px_Regular);
    stringWidth += (int16_t)(a_timer / 60) > 1 ? tft.getStringWidth(" hrs ") : tft.getStringWidth(" hr ");
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    stringWidth += tft.getStringWidth(a_timer % 60);
    tft.setFont(&akashi_36px_Regular);
    stringWidth += tft.getStringWidth(" mins");  
    a_textStartX = a_textCenterX - (stringWidth / 2);
    tft.setCursor(a_textStartX, a_startY);
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.print((int16_t)(a_timer / 60));
    tft.setFont(&akashi_36px_Regular);
    tft.setCursor(tft.getFontX()+4, a_startY+10);
    tft.print(F("hrs "));
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setCursor(tft.getFontX(), a_startY);
    tft.print(a_timer % 60);
    tft.setFont(&akashi_36px_Regular);
    tft.setCursor(tft.getFontX()+4, a_startY+10);
    tft.print(F("mins"));    
    a_textEndX = tft.getFontX();
    a_previousTimer = a_timer;
  }   
}

void drawTimer(const uint16_t a_x, const uint16_t a_y, uint8_t& a_previousOnTime, uint8_t a_hour, uint8_t a_min) {
  if (display::refreshPage || a_previousOnTime != a_hour + a_min) {
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    tft.setFontScale(1);
    tft.fillRect(a_x-5, a_y-2, 200, 50, user::backgroundColor);
    tft.setCursor(a_x, a_y);
    if (a_hour < 10)
      tft.print(0);
    tft.print(a_hour);
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.print(F(":"));
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setFontScale(1);
    if (a_min < 10)
      tft.print(0);
    tft.print(a_min);
    a_previousOnTime = a_hour + a_min;
  }
}

void drawTriStateButton(const uint16_t a_x, const uint16_t a_y, const uint8_t& a_value, uint8_t& a_previousValue) {
  if (display::refreshPage || a_value != a_previousValue) {
    tft.fillRoundRect(a_x-2, a_y-2, 184, 44, 5, RA8875_BLACK);
    if (a_value == 0) { // auto
      tft.fillRoundRect(a_x, a_y, 60, 40, 5, display::RA8875_DARKGREY);
      tft.fillRoundRect(a_x+60, a_y, 60, 40, 5, RA8875_GREEN);
      tft.fillRoundRect(a_x+120, a_y, 60, 40, 5, display::RA8875_DARKGREY);
    }
    else if (a_value == 1) { // on
      tft.fillRoundRect(a_x, a_y, 60, 40, 5, RA8875_GREEN);
      tft.fillRoundRect(a_x+60, a_y, 60, 40, 5, display::RA8875_DARKGREY);
      tft.fillRoundRect(a_x+120, a_y, 60, 40, 5, display::RA8875_DARKGREY);
      tft.drawLine(a_x+120, a_y, a_x+120, a_y+40, RA8875_BLACK);
    }
    else { // off
      tft.fillRoundRect(a_x, a_y, 60, 40, 5, display::RA8875_DARKGREY);
      tft.fillRoundRect(a_x+60, a_y, 60, 40, 5, display::RA8875_DARKGREY);
      tft.fillRoundRect(a_x+120, a_y, 60, 40, 5, RA8875_RED);
      tft.drawLine(a_x+60, a_y, a_x+60, a_y+40, RA8875_BLACK);
    }
    a_previousValue = a_value;
  }
}

// draw 2 values in large text at give x and y's
void drawTwoValues(
  const int16_t& a_startX, float a_value, const uint16_t& a_color, uint8_t a_precison,
  const int16_t& a_startX2, float a_value2, const uint16_t& a_color2, uint8_t a_precison2,
  const char* a_symbol, const int16_t& a_symbolOffset) {

  static float previousValue = 0, previousValue2 = 0;
  static int16_t startX = 0, endX = 0, startX2 = 0, endX2 = 0;
  tft.setFont(&akashi_36px_Regular);
  int16_t symbolWidth = 0;
  if (a_symbol != NULL) 
    symbolWidth = tft.getStringWidth(a_symbol) / 2;
  tft.setFont(&HallfeticaLargenum_42px_Regular);
  tft.setFontScale(2);
  tft.setTextColor(a_color, user::backgroundColor);
  if (display::refreshPage) {
    startX = 0; endX = 0; startX2 = 0; endX2 = 0;
  }
  // val 1
  if (a_value >= 1000 && a_precison >= 1)
    a_precison = 0;
  else if (a_value >= 100 && a_precison >= 2)
    a_precison = 1;
  if (display::refreshPage || hasChanged(a_value, previousValue, a_precison)) {
    tft.fillRect(startX, 248, endX - startX, 90, user::backgroundColor);
    startX = a_startX - (tft.getStringWidth(a_value, a_precison) + symbolWidth) / 2;
    tft.print(startX, 250, a_value, a_precison);
    if (a_symbol != NULL) {
      tft.setFont(&akashi_36px_Regular);
      tft.setFontScale(1);
      tft.print(tft.getFontX() + 6, 250 + a_symbolOffset, a_symbol);
    }
    endX = tft.getFontX();
    previousValue = a_value;
  }
  // val 2
  if (a_value2 >= 1000 && a_precison2 >= 1)
    a_precison2 = 0;
  else if (a_value2 >= 100 && a_precison2 >= 2)
    a_precison2 = 1;
  tft.setFont(&HallfeticaLargenum_42px_Regular);
  tft.setFontScale(2);
  tft.setTextColor(a_color2, user::backgroundColor);
  if (display::refreshPage || hasChanged(a_value2, previousValue2, a_precison2)) {
    tft.fillRect(startX2, 248, endX2 - startX2, 90, user::backgroundColor);
    startX2 = a_startX2 - (tft.getStringWidth(a_value2, a_precison2) + symbolWidth) / 2;
    tft.print(startX2, 250, a_value2, a_precison2);
    if (a_symbol != NULL) {
      tft.setFont(&akashi_36px_Regular);
      tft.setFontScale(1);
      tft.print(tft.getFontX() + 6, 250 + a_symbolOffset, a_symbol);
    }
    endX2 = tft.getFontX();
    previousValue2 = a_value2;
  }
}

void drawVerticalSlider(const int16_t& a_x, const int16_t& a_y, const int16_t& a_height, const uint8_t a_percent) {
  const int16_t indicatorYpos = a_y + 40 + (((a_height - 120) / 100.0) * a_percent);
  // Foreground
  tft.fillRect(a_x, a_y + 40, 30, a_height - 80, display::RA8875_LIGHTGREY);
  // draw over previous user position indicator
  tft.fillRect(a_x - 2, a_y + 40, 2, a_height - 80, display::RA8875_DARKGREY);
  tft.fillRect(a_x - 5, a_y + 40, 3, a_height - 80, user::backgroundColor);
  tft.fillRect(a_x + 32, a_y + 40, 3, a_height - 80, user::backgroundColor);
  tft.fillRect(a_x + 30, a_y + 40, 2, a_height - 80, display::RA8875_DARKGREY);
  // Current user position indicator
  tft.fillRoundRect(a_x - 5, indicatorYpos, 40, 40, 5, display::RA8875_MIDGREY);
  // Up adjustment arrow
  tft.fillRoundRect(a_x - 5, a_y, 40, 40, 5, display::RA8875_DARKGREY);
  tft.fillTriangle(a_x - 2, a_y + 32, a_x + 15, a_y + 4, a_x + 31, a_y + 32, RA8875_BLACK);
  // Down adjustment arrow
  tft.fillRoundRect(a_x - 5, a_y + a_height - 40, 40, 40, 5, display::RA8875_DARKGREY);
  tft.fillTriangle(a_x - 2, a_y + a_height - 34, a_x + 15, a_y + a_height - 4, a_x + 31, a_y + a_height - 34, RA8875_BLACK);
}

void drawHorizontalSlider(const int16_t& a_x, const int16_t& a_y, const int16_t& a_width, const uint8_t a_percent) {
  const int16_t indicatorXpos = a_x + 30 + (((a_width - 90) / 100.0) * a_percent);
  // Foreground
  tft.fillRect(a_x  + 30, a_y, a_width - 60, 20, display::RA8875_LIGHTGREY);
  // draw over previous user position indicator
  tft.fillRect(a_x + 30, a_y - 2, a_width - 60, 2, display::RA8875_DARKGREY);
  tft.fillRect(a_x + 30, a_y - 5, a_width - 60, 3, user::backgroundColor);
  tft.fillRect(a_x + 22, a_y + 30, a_width - 60, 3, user::backgroundColor);
  tft.fillRect(a_x + 20, a_y + 30, a_width - 60, 2, display::RA8875_DARKGREY);
  // Current user position indicator
  tft.fillRoundRect(indicatorXpos, a_y - 5, 30, 25, 3, display::RA8875_MIDGREY);
  // Up adjustment arrow
  tft.fillRoundRect(a_x, a_y - 5, 30, 25, 3, display::RA8875_DARKGREY);
  tft.fillTriangle(a_x + 22, a_y - 20, a_x + 4, a_y + 10, a_x + 22, a_y + 21, RA8875_BLACK);
  // Down adjustment arrow
  tft.fillRoundRect(a_x + a_width - 30, a_y - 5, a_width, 25, 3, display::RA8875_DARKGREY);
  tft.fillTriangle(a_x + a_width - 24, a_y - 2, a_x + a_width - 4, a_y + 10, a_x + a_width - 24, a_y + 21, RA8875_BLACK);
}

// set a warning text color based on the passed over attributes
uint16_t setWarningColor(const float& a_sensor, const float& a_minTarget, const float& a_maxTarget, const float& a_warningMargin) {
  uint16_t color = RA8875_BLACK;
  if (a_sensor < (a_minTarget - a_warningMargin))
    color = RA8875_RED;
  else if (a_sensor > (a_maxTarget + a_warningMargin))
    color = RA8875_RED;
  else if (a_sensor < a_minTarget)
    color = RA8875_YELLOW;
  else if (a_sensor > a_maxTarget)
    color = RA8875_YELLOW;
  return color;
}

void displaySetAfkTime() {
  static int16_t startX, endX;
  static uint16_t previousAfkTime;
  if (display::refreshPage) {
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    tft.print(300, 116, F("Display timeout"));
    drawUpDownButtons(370, 350, 500, 350, RA8875_BLUE);
    exitButton(392, 430);
    infoButton(770, 120);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
  }
  if (display::refreshPage || previousAfkTime != user::afkTime) {
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setFontScale(1);

    tft.fillRect(startX, 234, endX - startX, 50, user::backgroundColor);
    startX = 410 - (tft.getStringWidth((uint16_t)user::afkTime) / 2);

    if (user::afkTime != 0) {
      tft.setCursor(startX, 234);
      tft.print((uint16_t)user::afkTime);
      tft.setFont(&akashi_36px_Regular); 
      tft.setCursor(tft.getFontX() + 4, 244);
      if (user::afkTime == 1)
        tft.print(F("min"));
      else
        tft.print(F("mins"));
    }
    else if (user::afkTime == 0) {
      tft.setFont(&akashi_36px_Regular);  
      tft.setCursor(400, 234);
      tft.print(F("OFF"));
    }
    endX = tft.getFontX();
    previousAfkTime = user::afkTime;
  }
}

void displaySetBrightness() {
  static int16_t startX, endX;
  static uint8_t previousBrightness;
  if (display::refreshPage) {
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    tft.print(276, 116, F("Display brightness"));
    drawUpDownButtons(370, 350, 500, 350, RA8875_BLUE);
    exitButton(392, 430);
    infoButton(770, 120);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
  }
  if (display::refreshPage || previousBrightness != user::brightness) {
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setFontScale(1);
    tft.fillRect(startX, 234, endX - startX, 50, user::backgroundColor);
    startX = 410 - (tft.getStringWidth((uint16_t)user::brightness) / 2);
    tft.setCursor(startX, 234);
    tft.print(user::brightness);
    tft.setFont(&akashi_36px_Regular); 
    tft.setCursor(tft.getFontX() + 4, 244);
    tft.print("%");
    endX = tft.getFontX();
    previousBrightness = user::brightness;
  }
}

void displaySetGraphInterval() {
  static int16_t startX, endX;
  static uint16_t previousInterval;
  if (display::refreshPage) {
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    tft.print(300, 116, F("Graph interval"));
    drawUpDownButtons(370, 350, 500, 350, RA8875_BLUE);
    exitButton(392, 430);
    infoButton(770, 120);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
  }
  if (display::refreshPage || previousInterval != user::graphInterval) {
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setFontScale(1);
    tft.fillRect(startX, 234, endX - startX, 50, user::backgroundColor);
    startX = 410 - (tft.getStringWidth((uint16_t)user::graphInterval) / 2);
    tft.setCursor(startX, 234);
    tft.print((uint16_t)(user::graphInterval >= 60 ? user::graphInterval / 60 : user::graphInterval));
    tft.setFont(&akashi_36px_Regular); 
    tft.setCursor(tft.getFontX() + 4, 244);
    if (user::graphInterval == 1)
      tft.print(F("sec"));
    else if (user::graphInterval < 60)
      tft.print(F("secs"));
    else if (user::graphInterval == 60)
      tft.print(F("min"));
    else 
      tft.print(F("mins"));
    endX = tft.getFontX();
    previousInterval = user::graphInterval;
  }
}

void displaySetDosingInterval() {
  static int16_t startX, endX;
  static uint8_t previousInterval;
  if (display::refreshPage) {
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    tft.print(300, 116, F("Dosing interval"));
    drawUpDownButtons(370, 350, 500, 350, RA8875_BLUE);
    exitButton(392, 430);
    infoButton(770, 120);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
  }
  if (display::refreshPage || previousInterval != user::dosingInterval) {
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setFontScale(1);
    tft.fillRect(startX, 234, endX - startX, 50, user::backgroundColor);
    startX = 410 - (tft.getStringWidth((uint16_t)user::dosingInterval) / 2);
    tft.setCursor(startX, 234);
    tft.print((uint16_t)user::dosingInterval);
    tft.setFont(&akashi_36px_Regular); 
    tft.setCursor(tft.getFontX() + 4, 244);
    if (user::graphInterval == 1)
      tft.print(F("hr"));
    else
      tft.print(F("hrs"));
    endX = tft.getFontX();
    previousInterval = user::dosingInterval;
  }
}

void displaySetNumberOfDosers() {
  static int16_t startX, endX;
  static uint8_t previousNumOfDosers;
  if (display::refreshPage) {
    tft.setFont(&akashi_36px_Regular);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    tft.print(300, 116, F("Number of dosers"));
    drawUpDownButtons(370, 350, 500, 350, RA8875_BLUE);
    exitButton(392, 430);
    infoButton(770, 120);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
  }
  if (display::refreshPage || previousNumOfDosers != user::numberOfDosers) {
    tft.setFont(&HallfeticaLargenum_42px_Regular);   
    tft.setFontScale(1);
    tft.fillRect(startX, 234, endX - startX, 50, user::backgroundColor);
    startX = 440 - (tft.getStringWidth((uint16_t)user::numberOfDosers) / 2);
    tft.print(startX, 234, (uint16_t)user::numberOfDosers);
    endX = tft.getFontX();
    previousNumOfDosers = user::numberOfDosers;
  }
}

void displaySystemLogs() {
  if (display::refreshPage) {
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLUE, user::backgroundColor);
    tft.print(320, 110, F("System logs"));
    exitButton(392, 430);
    infoButton(770, 120);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    if (message::systemLogPos > 6) {
      int16_t scrollPercentage = map(display::systemLogScrollPos, 0, message::systemLogPos - 6, 0, 100);
      drawVerticalSlider(760, 100, 370, scrollPercentage);
    }
    int16_t startY = 160;
    int16_t maxLogsToDsiplay = 0;
    if (device::globalDebug) {   
      Serial.print(F("message::systemLogPos: ")); Serial.println(message::systemLogPos);
      Serial.print(F("systemLogScrollPos: ")); Serial.println(display::systemLogScrollPos);
    }
    if (display::systemLogScrollPos + 6 <= message::systemLogPos)
      maxLogsToDsiplay = display::systemLogScrollPos + 6;
    else 
      maxLogsToDsiplay = message::systemLogPos;
    if (device::globalDebug) {
      Serial.print(F("maxLogsToDsiplay: ")); Serial.println(maxLogsToDsiplay);
    }
    tft.setFont(&myriadPro_32px_Regular);
    tft.print(700, 105, F("Logs ")); tft.print(display::systemLogScrollPos + 1); tft.print(F(" of ")); tft.print(maxLogsToDsiplay);
    tft.setFont(&akashi_36px_Regular);
    for (uint8_t i = display::systemLogScrollPos; i < maxLogsToDsiplay; i++) {
      int16_t logType = message::logTypeArray[i];   
      const char* logText = pgm_read_word(&message::notificationsArray[logType]);   
      if (device::globalDebug) {
        Serial.print(F("Printing log: ")); Serial.print(i); Serial.print(F(", logType: ")); Serial.println(logType);
      }
      tft.print(105, startY, (const __FlashStringHelper *)logText);
      tft.print(F(" "));
      tft.print(message::timeStrArray[i]);
      startY += 50;
    }
  }
}

void displaySetEcDosingMode() {
  static user::dosingModes previousMode = user::PRECISE;
  if (display::refreshPage) {
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    tft.print(220, 126, F("EC/TDS dosing mode"));
    tft.print(250, 220, F("Precise"));
    tft.print(250, 320, F("Incremental"));
    exitButton(392, 430);
    infoButton(770, 120);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
  }
  if (display::refreshPage || user::ecDosingMode != previousMode) {   
    tft.fillCircle(200, 240, 20, display::RA8875_LIGHTGREY);
    tft.drawCircle(200, 240, 20, RA8875_BLACK);
    tft.fillCircle(200, 340, 20, display::RA8875_LIGHTGREY); 
    tft.drawCircle(200, 340, 20, RA8875_BLACK);   
    if (user::ecDosingMode == user::PRECISE) {   
      tft.fillCircle(200, 240, 11, RA8875_BLACK);
    }   
    else {      
      tft.fillCircle(200, 340, 11, RA8875_BLACK);
    }
    previousMode = user::ecDosingMode;
  }
}

void displaySetPhDosingMode() {
  static user::dosingModes previousMode = user::PRECISE;
  if (display::refreshPage) {
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    tft.print(250, 126, F("PH dosing mode"));
    tft.print(250, 220, F("Precise"));
    tft.print(250, 320, F("Incremental"));
    exitButton(392, 430);
    infoButton(770, 120);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
  }
  if (display::refreshPage || user::phDosingMode != previousMode) {   
    tft.fillCircle(200, 240, 20, display::RA8875_LIGHTGREY);
    tft.drawCircle(200, 240, 20, RA8875_BLACK);
    tft.fillCircle(200, 340, 20, display::RA8875_LIGHTGREY); 
    tft.drawCircle(200, 340, 20, RA8875_BLACK);   
    if (user::phDosingMode == user::PRECISE) {   
      tft.fillCircle(200, 240, 11, RA8875_BLACK);
    }   
    else {      
      tft.fillCircle(200, 340, 11, RA8875_BLACK);
    }
    previousMode = user::phDosingMode;
  }
}

void displaySetEcTdsValue() {
  if (display::refreshPage) {
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    tft.print(200, 116, F("EC/TDS solution value"));
    tft.print(176, 170, F("EC value"));
    tft.print(520, 170, F("TDS value"));  
    drawUpDownButtons(230, 400, 310, 400, RA8875_BLUE);
    drawUpDownButtons(585, 400, 665, 400, RA8875_BLUE);
    exitButton(392, 430);
    infoButton(770, 120);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
  }
  drawTwoValues(285, sensor::ecSolution, RA8875_BLACK, 2, 650, sensor::tdsSolution, RA8875_BLACK, 2, NULL, 0);
}

void displaySetPhDownUpValue() {
  if (display::refreshPage) {
    drawSlideIcons(110, 112, display::maxMinsPage, 10);
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    tft.print(200, 126, F("PH solution value"));
    tft.print(176, 170, F("PH down value"));
    tft.print(520, 170, F("PH up value"));
    drawUpDownButtons(230, 400, 310, 400, RA8875_BLUE);
    drawUpDownButtons(585, 400, 665, 400, RA8875_BLUE);
    exitButton(392, 430);
    infoButton(770, 120);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
  }
  drawTwoValues(285, sensor::phDownSolution, RA8875_BLACK, 2, 650, sensor::phUpSolution, RA8875_BLACK, 2, NULL, 0);
}

void displayWifiSsid() {
  if (display::refreshPage && !display::showKeyboard) {
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    tft.print(270, 116, F("Set WiFi SSID"));
    cancelButton(260, 420);
    continueButton(455, 420);
    infoButton(770, 120);
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK, RA8875_WHITE);
    tft.fillRect(237, 250, 346, 42, RA8875_WHITE);
    tft.drawRect(235, 248, 350, 46, RA8875_BLACK);
    tft.drawRect(236, 249, 348, 44, RA8875_BLACK);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    tft.print(242, 250, wifi::ssid);
    tft.setFont(&myriadPro_32px_Regular);
    tft.print(330, 295, F("Press to edit"));
  }
  else if (display::refreshPage && display::showKeyboard) {
    keyBoard();
  }
  if ((display::refreshPage && display::showKeyboard) || (display::showKeyboard && device::updateKeyboardInput)) {
    keyBoardInput(wifi::password, 16);
    device::updateKeyboardInput = false;
  }
  if (display::showKeyboard)
    showCursor();
}

void displayWifiPassword() {
  if (display::refreshPage && !display::showKeyboard) {
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    tft.print(230, 116, F("Set WiFi password"));
    cancelButton(260, 420);
    continueButton(455, 420);
    infoButton(770, 120);
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK, RA8875_WHITE);
    tft.fillRect(237, 250, 346, 42, RA8875_WHITE);
    tft.drawRect(235, 248, 350, 46, RA8875_BLACK);
    tft.drawRect(236, 249, 348, 44, RA8875_BLACK);
    tft.print(242, 250, wifi::password);
    tft.setFont(&myriadPro_32px_Regular);
    tft.setTextColor(RA8875_BLACK, user::backgroundColor);
    tft.print(330, 295, F("Press to edit"));
  }
  else if (display::refreshPage && display::showKeyboard) {
    keyBoard();
  }
  if ((display::refreshPage && display::showKeyboard) || (display::showKeyboard && device::updateKeyboardInput)) {
    keyBoardInput(wifi::password, 16);
    device::updateKeyboardInput = false;
  }
  if (display::showKeyboard)
    showCursor();
}