void launchCo2Notification(byte& a_continueCo2Control, byte& a_previousDate) {
  byte co2DialogTimer = 60;
  byte previousCo2Timer = 0;
  bool refreshText = false;
  unsigned long continuePreviousMillis = millis();

  tft.backlight(true);
  tft.displayOn(true);
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
        a_previousDate = rtcTime.date;
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
  }
}

void abortCo2Notification(bool& a_startCo2Relay, unsigned long& a_lastTouch, const unsigned long& a_co2RunTime) {
  unsigned long gasTimeInMinutes = sensor::co2GasTime;
  unsigned long previousGasCounterMillis = millis();
  while (a_startCo2Relay) {
    float timeRemaining = gasTimeInMinutes / 60000.0;
    if (timeRemaining > 0 && timeRemaining < 1)
      timeRemaining = 1;
    continueMessage(message::cancelCo2, timeRemaining, 0, false, true, true);
    gasTimeInMinutes -= millis() - previousGasCounterMillis;

    if (tft.touched()) {
      const uint16_t startX = 166, startY = 166;
      tft.touchReadPixel(&display::touch_x, &display::touch_y);
      if (millis() >= a_lastTouch) {
        if (display::touch_x >= startX + 200 && display::touch_x <= startX + 400 && display::touch_y >= startY + 200 && display::touch_y <= startY + 250) { // Cancel
          digitalWrite(pin::co2Solenoid, device::relayOffState);
          Serial.println(F("Aborted pumping Co2!"));
          sensor::co2GasTime = 0;
          a_startCo2Relay = false;
          device::co2DisabledFans = false;
          clearPage();
          display::refreshPage = true;
        }
      }
    }

    if (millis() > a_co2RunTime) {
      digitalWrite(pin::co2Solenoid, device::relayOffState);
      Serial.println(F("Finished pumping Co2"));
      sensor::co2GasTime = 0;
      a_startCo2Relay = false;
      clearPage();
      display::refreshPage = true;
    }

    previousGasCounterMillis = millis();
  }
}

void launchDrainNotification(byte& a_continueDraining, bool& a_startDraining) {
  bool refreshText = false;
  byte drainingDialogTimer = 60;
  byte previousDrainingTimer = 0;
  unsigned long continuePreviousMillis = millis();
  tft.backlight(true);
  tft.displayOn(true);
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
  }
}

void launchRefillNotification(bool& a_startRefilling, byte& a_continueRefilling) {
  // Call refill function
  if (a_startRefilling) {
    bool refreshText = false;
    byte refillingDialogTimer = 60;
    byte previousRefillingTimer = 0;
    unsigned long continuePreviousMillis = millis();
    tft.backlight(true);
    tft.displayOn(true);
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
    }
  }
}

void launchDosingNotification(const float& a_sensorPercent, const byte& a_dosingMode, unsigned long& a_lastTouch) {
  byte dosingDialogTimer = 60;
  bool refreshDosingNotification = true;
  unsigned long continuePreviousMillis = millis();
  device::continueDosing = device::NOT_SET;
  tft.backlight(true);
  tft.displayOn(true);
  while (dosingDialogTimer != 0 && device::continueDosing == device::NOT_SET) {
    displayDosingNotification(dosingDialogTimer, a_sensorPercent, a_dosingMode, refreshDosingNotification);
    if (millis() - continuePreviousMillis >= 1000UL) {
      if (dosingDialogTimer > 0)
        dosingDialogTimer--;
      continuePreviousMillis = millis();
    }
  }
  a_lastTouch = millis() + 5000UL;
}

// Draw the left and top menu icons
void OuterMenuIcons() {
  // left menu
  drawPageIcon(0, 10, 90, homeIcon);
  drawPageIcon(1, 10, 160, graphIcon);
  drawPageIcon(2, 10, 230, minMaxIcon);
  drawPageIcon(3, 15, 300, presetIcon);
  drawPageIcon(4, 15, 370, settingsIcon);
  // top menu
  drawPageIcon(5, 15, 10, user::convertToTds ? ppmIcon : ecIcon);
  drawPageIcon(6, 116, 10, phIcon);
  drawPageIcon(7, 223, 10, co2Icon);
  drawPageIcon(8, 324, 10, waterTemperatureIcon);
  drawPageIcon(9, 420, 10, doserIcon);
  drawPageIcon(10, 507, 8, lightIcon);
  drawPageIcon(11, 596, 8, fanIcon);
  drawPageIcon(12, 696, 10, warningsIcon);
  display::previousPage = display::page;
}

// Draw the black frame you see around the menu icons
void frame() {
  tft.drawRect(9, 9, 780, 80, RA8875_BLACK);
  tft.drawRect(10, 10, 780, 80, RA8875_BLACK);
  tft.drawRect(9, 89, 80, 364, RA8875_BLACK);
  tft.drawRect(10, 89, 80, 365, RA8875_BLACK);
}

// Draw a menu icon
void drawPageIcon(const byte& a_page, const short& a_x, const short a_y, const byte* a_icon) {
  if (display::page == a_page) {
    tft.drawXbmpArray(a_icon, 800, a_x, a_y, 80, 80, RA8875_BLUE, 1);
  }
  else if (display::previousPage == a_page || display::refreshPage) {
    tft.drawXbmpArray(a_icon, 800, a_x, a_y, 80, 80, display::RA8875_GREY, 1);
  }
}

void drawPageIconQuick(const byte& a_page, const short& a_x, const short a_y, const byte* a_icon) {
  if (display::page == a_page) {
    tft.drawXbmpArray(a_icon, 800, a_x, a_y, 80, 80, RA8875_BLUE, 1);
  }
  else {
    tft.drawXbmpArray(a_icon, 800, a_x, a_y, 80, 80, display::RA8875_GREY, 1);
  }
}

// draws the mini icons to swap between sensors on the graph page
void drawGraphIcons() {
  drawMenuIcon(display::graphOption, 1, 747, 90, miniPmIcon);
  drawMenuIcon(display::graphOption, 2, 747, 124, miniPhIcon);
  drawMenuIcon(display::graphOption, 3, 747, 158, miniEcIcon);
  drawMenuIcon(display::graphOption, 4, 746, 192, miniCo2Icon);
  drawMenuIcon(display::graphOption, 5, 746, 230, miniWaterTempIcon);
  drawMenuIcon(display::graphOption, 6, 747, 268, miniWaterLevelIcon);
  drawMenuIcon(display::graphOption, 7, 747, 310, miniFanOneIcon);
  drawMenuIcon(display::graphOption, 8, 747, 354, miniFanTwoIcon);
  drawMenuIcon(display::graphOption, 9, 747, 394, miniAirTempIcon);
  drawMenuIcon(display::graphOption, 10, 747, 434, miniHumidityIcon);
  tft.drawRect(742, 88, 48, 391, RA8875_BLACK);
  tft.drawRect(743, 89, 46, 389, RA8875_BLACK);
}

// draws a mini graph icon
void drawMenuIcon(const byte& a_currentPage, const byte& a_newPage, const short& a_x, const short a_y, const byte* a_icon) {
  if (a_currentPage == a_newPage) {
    tft.drawXbmpArray(a_icon, 205, a_x, a_y, 40, 40, RA8875_BLUE, 1);
  }
  else {
    tft.drawXbmpArray(a_icon, 205, a_x, a_y, 40, 40, display::RA8875_GREY, 1);
  }
}

// draw an overal EC and PH+ or PH- button, seen in the doser pages.
void drawEcPhButton(const int& a_x, const int& a_y, const byte& a_doserMode, byte& a_PreviousDoserMode, const bool& a_phUpSymbol, const bool& a_buttonBackground) {
  tft.setFont(&akashi_36px_Regular);
  tft.setTextColor(RA8875_BLACK);
  tft.setFontScale(1);
  if (display::refreshPage || a_doserMode != a_PreviousDoserMode) {
    if (a_buttonBackground && a_doserMode != device::DOSER_OFF) {
      tft.fillCircle(a_x, a_y, 16, RA8875_GREEN);
      tft.fillRect(a_x - 2, a_y - 17, 57, 35, RA8875_GREEN);
      tft.fillCircle(a_x + 53, a_y, 16, RA8875_GREEN);
    }
    else if (a_buttonBackground && a_doserMode == device::DOSER_OFF) {
      tft.fillCircle(a_x, a_y, 16, RA8875_RED);
      tft.fillRect(a_x - 2, a_y - 17, 57, 35, RA8875_RED);
      tft.fillCircle(a_x + 53, a_y, 16, RA8875_RED);
    }
    if (a_doserMode == device::DOSER_OFF) {
      tft.print(a_x - 2, a_y - 20, F("OFF"));
    }
    else if (a_doserMode == device::DOSER_PH) {
      tft.setCursor(a_x - 2, a_y - 20);
      if (a_phUpSymbol)
        tft.print(F("PH+"));
      else
        tft.print(F("PH-"));
    }
    else if (a_doserMode == device::DOSER_EC) {
      tft.print(a_x + 6, a_y - 19, F("EC"));
    }
    a_PreviousDoserMode = a_doserMode;
  }
}

// As the function says, this is the screen saver function to save power
// and increase display life time - screen burning.
// Works based on the currentTouch which is the last time the display was touched in ms
// and the afkTime set in the GUI settings.
void screenSaver() {
  if (user::afkTime != 0 && (millis() - display::currentTouch > user::afkTime * 60000UL)) {
    tft.backlight(false);
    tft.displayOn(false);
    display::displayIsOff = true;
  }
  else if (display::displayIsOff) {
    tft.backlight(true);
    tft.displayOn(true);
    display::displayIsOff = false;
  }
}

// Display a new page
void setPage(const byte& a_page) {
  if (display::page != a_page) {
    beep();
    display::page = a_page;
    OuterMenuIcons();
    display::refreshPage = true;
    clearPage();
    display::previousTouchMillis = millis() + 1000UL;
  }
}

// Clear the main draw area, which does not include the outer menus
void clearPage() {
  tft.fillRect(92, 92, 708, 388, user::backgroundColor);
}

void setTextWarningColor(const byte& a_errorState) {
  if (a_errorState == device::NO_WARNING)
    tft.setTextColor(RA8875_BLACK);
  else if (a_errorState == device::MINOR_WARNING)
    tft.setTextColor(RA8875_YELLOW);
  else if (a_errorState == device::MAJOR_WARNING)
    tft.setTextColor(RA8875_RED);
}
// Change the display background color
// TO DO: 3 part colour theme so that text and icons have a different color too.
void setBackground(const unsigned int& a_color) {
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
void drawFourUpDownButtons(const int& a_yOffset) {
  for (byte i = 0; i < 4; i++) {
    tft.fillCircle(227 + (i * 178), 365 + a_yOffset, 30, RA8875_BLUE);
    tft.fillTriangle(227 + (i * 178), 350 + a_yOffset, 247 + (i * 178), 375 + a_yOffset, 207 + (i * 178), 375 + a_yOffset, RA8875_BLACK);

    tft.fillCircle(139 + (i * 178), 365 + a_yOffset, 30, RA8875_BLUE);
    tft.fillTriangle(139 + (i * 178), 377 + a_yOffset, 159 + (i * 178), 352 + a_yOffset, 119 + (i * 178), 352 + a_yOffset, RA8875_BLACK);
  }
}

// Draw 3 pairs of increment buttons, evently spaced.
void drawThreeUpDownButtons() {
  for (byte i = 0; i < 3; i++) {
    tft.fillCircle(184 + (i * 205), 365, 30, RA8875_BLUE);
    tft.fillTriangle(184 + (i * 205), 377, 204 + (i * 205), 352, 164 + (i * 205), 352, RA8875_BLACK);

    tft.fillCircle(262 + (i * 205), 365, 30, RA8875_BLUE);
    tft.fillTriangle(262 + (i * 205), 350, 282 + (i * 205), 375, 242 + (i * 205), 375, RA8875_BLACK);
  }
}

// Draw 2 pairs of increment buttons, at a given x y
void drawUpDownButtons(const int& a_x1, const int& a_y1, const int& a_x2, const int& a_y2, const unsigned int& a_color) {
  tft.fillCircle(a_x1, a_y2, 30, a_color);
  tft.fillTriangle(a_x1, a_y2 + 12, a_x1 + 20, a_y2 - 13, a_x1 - 20, a_y2 - 13, RA8875_BLACK);

  tft.fillCircle(a_x2, a_y1, 30, a_color);
  tft.fillTriangle(a_x2, a_y1 - 15, a_x2 + 20, a_y1 + 10, a_x2 - 20, a_y1 + 10, RA8875_BLACK);
}

// Draw a pair of mini increment buttons, at a given x y
void drawMiniButtonIncrements(const int& a_x, const int& a_y) {
  tft.fillCircle(a_x, a_y, 15, RA8875_BLUE);
  tft.fillTriangle(a_x, a_y + 8, a_x + 10, a_y - 5, a_x - 10, a_y - 5, RA8875_BLACK);

  tft.fillCircle(a_x+ 80, a_y, 15, RA8875_BLUE);
  tft.fillTriangle(a_x+ 80, a_y - 7, a_x + 90, a_y + 5, a_x + 70, a_y + 5, RA8875_BLACK);
}

// Draw a confirm button
void drawMiniConfirmButton(const int& a_x, const int& a_y) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK);
  tft.fillRoundRect(a_x, a_y, 60, 38, 5, display::RA8875_GREY);
  tft.drawRoundRect(a_x, a_y, 60, 38, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x + 1, a_y + 1, 58, 36, 5, RA8875_BLACK);
  tft.print(a_x + 8, a_y - 1, F("ok"));
}

void drawEcConversionButton(const int& a_x, const int& a_y) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK);
  tft.fillRoundRect(a_x, a_y, 60, 38, 5, display::RA8875_GREY);
  tft.drawRoundRect(a_x, a_y, 60, 38, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x + 1, a_y + 1, 58, 36, 5, RA8875_BLACK);
  tft.setCursor(a_x + 8, a_y - 1);
  if (device::conversionType == device::EU)
    tft.print(F("EU"));
  else if (device::conversionType == device::US)
    tft.print(F("US"));
  else if (device::conversionType == device::AU)
    tft.print(F("AU"));
}

// Draw a radio button, based on bool state
void drawRadioButton(const int& a_x, const int& a_y, const bool& option) {
  tft.fillRoundRect(a_x - 2, a_y - 2, 154, 44, 5, RA8875_BLACK);
  if (option) {
    tft.fillRoundRect(a_x, a_y, 75, 40, 5, display::RA8875_GREY);
    tft.fillRoundRect(a_x + 75, a_y, 75, 40, 5, RA8875_GREEN);
  }
  else {
    tft.fillRoundRect(a_x, a_y, 75, 40, 5, RA8875_RED);
    tft.fillRoundRect(a_x + 75, a_y, 75, 40, 5, display::RA8875_GREY);
  }
}

// Draw a radio button, based on bool state
void drawMiniRadioButton(const int& a_x, const int& a_y, const bool & option) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK);
  tft.fillRoundRect(a_x, a_y, 90, 38, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x + 1, a_y + 1, 88, 36, 5, RA8875_BLACK);
  if (option) {
    tft.fillRoundRect(a_x, a_y, 45, 38, 5, display::RA8875_GREY);
    tft.drawRoundRect(a_x, a_y, 45, 38, 5, RA8875_BLACK);
    tft.fillRoundRect(a_x + 45, a_y, 45, 38, 5, RA8875_GREEN);
    tft.drawRoundRect(a_x + 45, a_y, 45, 38, 5, RA8875_BLACK);
  }
  else {
    tft.fillRoundRect(a_x, a_y, 45, 38, 5, RA8875_RED);
    tft.drawRoundRect(a_x, a_y, 45, 38, 5, RA8875_BLACK);
    tft.fillRoundRect(a_x + 45, a_y, 45, 38, 5, display::RA8875_GREY);
    tft.drawRoundRect(a_x + 45, a_y, 45, 38, 5, RA8875_BLACK);
  }
}

// Draw a radio button, based on bool state
void drawMiniEtapeButton(const int& a_x, const int& a_y, const bool& option) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK);
  tft.fillRoundRect(a_x, a_y, 130, 40, 5, display::RA8875_GREY);
  tft.drawRoundRect(a_x, a_y, 130, 40, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x + 1, a_y + 1, 128, 38, 5, RA8875_BLACK);
  if (option)
    tft.print(a_x + 14, a_y - 1, F("Etape"));
  else
    tft.print(a_x + 6, a_y - 1, F("SR-04"));
}

// show the reset message, it is an odd size so i made it seperate.
void showResetMessage() {
  int startX = 136; // makes it eaiser to position
  int startY = 186;
  //Frame
  tft.fillRoundRect(startX - 20, startY, 580, 130, 5, display::RA8875_SMOKE_WHITE);
  tft.drawRoundRect(startX - 22, startY - 2, 582, 134, 5, RA8875_BLACK);
  tft.drawRoundRect(startX - 22, startY - 1, 581, 132, 5, RA8875_BLACK);
  // Draw text
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK);
  tft.print(startX + 8, startY, F("Please wait while the"));
  tft.print(startX + 8, startY + 38, F("system loads the settings"));
  tft.print(startX + 8, startY + 76, F("for the first time."));
}

// Show the dosing dialog, it has many different states so it is its own function..
void displayDosingNotification(const byte& a_seconds, const float& a_outOfRange, const byte& a_dosingMode, bool& a_refreshDosingNotification) {
  uint16_t startX = 166, startY = 166;
  static byte previousSeconds;
  static int startSecPos, endSecPos;
  if (a_refreshDosingNotification) {
    Serial.println(F("showing dosing alert"));
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK);
    //Frame
    tft.fillRoundRect(startX - 20, startY, 600, 250, 5, display::RA8875_SMOKE_WHITE); // RA8875_YELLOW
    tft.drawRoundRect(startX - 22, startY - 2, 602, 254, 5, RA8875_BLACK);
    tft.drawRoundRect(startX - 22, startY - 1, 601, 252, 5, RA8875_BLACK);
    // Buttons
    cancelButton(startX + 108, startY + 200);
    continueButton(startX + 283, startY + 200);
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
    tft.print(" %");
    // ====================================
    tft.setCursor(startX + 42, startY + 88);
    tft.print(F("Starting in "));
    startSecPos = tft.getFontX();
    // ===
    tft.setCursor(startX + 24, startY + 138);
    tft.print(F("Do you want to continue?"));
  }
  if (a_refreshDosingNotification || a_seconds != previousSeconds) {
    tft.fillRect(startSecPos, startY + 90, endSecPos - startSecPos, 34, display::RA8875_SMOKE_WHITE); // RA8875_YELLOW
    tft.setCursor(startSecPos + 2, startY + 88);
    tft.print(a_seconds);
    tft.print(" seconds.");
    endSecPos = tft.getFontX() + 2;
    previousSeconds = a_seconds;
  }
  a_refreshDosingNotification = false;
  // touch event
  if (tft.touched()) {
    tft.touchReadPixel(&display::touch_x, &display::touch_y);
    if (display::touch_x >= startX + 110 && display::touch_x <= startX + 250 && display::touch_y >= startY + 200 && display::touch_y <= startY + 246) { // No
      Serial.println(F("NO"));
      device::continueDosing = device::CANCEL;
      beep();
      clearPage();
      display::refreshPage = true;
    }
    else if (display::touch_x >= startX + 284 && display::touch_x <= startX + 462 && display::touch_y >= startY + 200 && display::touch_y <= startY + 246) { // Yes
      Serial.println(F("YES"));
      device::continueDosing = device::CONTINUE;
      beep();
      //clearPage();
      display::refreshPage = true;
    }
  }
}

// Show a continue message with the string stored in the Program memory / Flash, and a float which can be excluded if -1
void continueMessage(const char* a_text, const float& a_num, const byte a_precision, const bool& a_showContiue, const bool& a_showCancel, bool a_refresh) {
  int startX = 166; // makes it eaiser to position
  int startY = 166;
  //Frame
  if (!a_refresh) {
    tft.fillRoundRect(startX - 30, startY, 620, 250, 5, display::RA8875_SMOKE_WHITE);
    tft.drawRoundRect(startX - 32, startY - 2, 622, 254, 5, RA8875_BLACK);
    tft.drawRoundRect(startX - 32, startY - 1, 621, 252, 5, RA8875_BLACK);
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
  tft.setTextColor(RA8875_BLACK);
  tft.setCursor(startX - 8, startY);
  static int numX = 0, numY = 0;
  for (unsigned int i = 0; i < strlen_P(a_text); i++) {
    char c = pgm_read_byte_near(a_text + i);
    if (c == '$' && a_num != -1) {
      numX = tft.getFontX();
      numY = tft.getFontY();
      if (a_refresh)
        tft.fillRect(numX, numY, 744 - numX, 36, display::RA8875_SMOKE_WHITE);
      tft.print(a_num, a_precision);
    }
    else
      tft.print(c);
    if (c == '\n') {
      startY += 38;
      tft.setCursor(startX - 8, startY);
    }
  }
}

// Show an abort message with the string stored in the Program memory / Flash, and a float and int which can be excluded if -1
void abortMessage(const char *a_text, const char* a_str, const float& a_value, const int& a_doserNum, const byte& a_precison) {
  int startX = 166; // makes it eaiser to position
  int startY = 166;
  //Frame
  tft.fillRoundRect(startX - 20, startY, 600, 250, 5, display::RA8875_SMOKE_WHITE);
  tft.drawRoundRect(startX - 22, startY - 2, 602, 254, 5, RA8875_BLACK);
  tft.drawRoundRect(startX - 22, startY - 1, 601, 252, 5, RA8875_BLACK);
  // Buttons
  cancelButton(startX + 200, startY + 200); // x was 178
  // Draw text
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK);
  tft.setCursor(startX - 8, startY);
  for (unsigned int i = 0; i < strlen_P(a_text); i++) {
    char c = pgm_read_byte_near(a_text + i);
    if (c == '*')
      tft.print(a_str);
    else if (c == '$' && a_value != -1)
      tft.print(a_value, a_precison);
    else if (c == '#' && a_doserNum != -1)
      tft.print(a_doserNum);
    else
      tft.print(c);
    if (c == '\n') {
      startY += 38;
      tft.setCursor(startX - 8, startY);
    }
  }
}

// draw a set button at a given x and y
void setButton(const int& a_x, const int& a_y) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK);
  tft.fillRoundRect(a_x, a_y, 94, 42, 5, display::RA8875_GREY);
  tft.drawRoundRect(a_x - 2, a_y - 2, 98, 46, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x - 1, a_y - 1, 96, 44, 5, RA8875_BLACK);
  tft.print(a_x + 19, a_y + 1, F("Set"));
}

// draw a save button at a given x and y
void saveButton(const int& a_x, const int& a_y) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK);
  tft.fillRoundRect(a_x, a_y, 129, 42, 5, display::RA8875_GREY);
  tft.drawRoundRect(a_x - 2, a_y - 2, 133, 46, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x - 1, a_y - 1, 131, 44, 5, RA8875_BLACK);
  tft.print(a_x + 12, a_y, F("Save"));
}

// draw a load button at a given x and y
void loadButton(const int& a_x, const int& a_y) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK);
  tft.fillRoundRect(a_x, a_y, 129, 42, 5, display::RA8875_GREY);
  tft.drawRoundRect(a_x - 2, a_y - 2, 133, 46, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x - 1, a_y - 1, 131, 44, 5, RA8875_BLACK);
  tft.print(a_x + 16, a_y, F("Load"));
}

// draw a text input box at a given x and y and text stored in normal memory
void inputBox(const int& a_x, const int& a_y, const char* a_text) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK);
  tft.fillRect(a_x, a_y, 340, 42, RA8875_WHITE); // +25px
  tft.drawRect(a_x - 2, a_y - 2, 344, 46, RA8875_BLACK);
  tft.drawRect(a_x - 1, a_y - 1, 342, 44, RA8875_BLACK);
  tft.print(a_x + 5, a_y, a_text);
}

// draw a background color button at a given x and y and 565 colour
void backGroundColorButton(const int& a_x, const int& a_y, const unsigned int& a_color) {
  tft.drawRect(a_x, a_y, 34, 34, RA8875_BLACK);
  tft.drawRect(a_x + 1, a_y + 1, 32, 32, RA8875_BLACK);
  tft.fillRect(a_x + 2, a_y + 2, 30, 30, a_color);
}

// draw a prime button at a given x and y
void primeButton(const int& a_x, const int& a_y) {
  tft.fillCircle(a_x, a_y, 20, RA8875_YELLOW);
  tft.fillRect(a_x - 3, a_y - 20, 100, 41, RA8875_YELLOW);
  tft.fillCircle(a_x + 92, a_y, 20, RA8875_YELLOW);
  tft.setFont(&akashi_36px_Regular);
  tft.setTextColor(RA8875_BLACK);
  tft.setFontScale(1);
  tft.print(a_x - 6, a_y - 23, F("prime"));
}

// draw a exit button at a given x and y
void exitButton(const int& a_x, const int& a_y) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK);
  tft.fillRoundRect(a_x, a_y, 94, 42, 5, display::RA8875_GREY);
  tft.drawRoundRect(a_x - 2, a_y - 2, 98, 46, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x - 1, a_y - 1, 96, 44, 5, RA8875_BLACK);
  tft.print(a_x + 14, a_y + 1, F("Exit"));
}

// draw a Cancel button at a given x and y
void cancelButton(const int& a_x, const int& a_y) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK);
  tft.fillRoundRect(a_x, a_y, 148, 42, 5, display::RA8875_GREY);
  tft.drawRoundRect(a_x - 2, a_y - 2, 152, 46, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x - 1, a_y - 1, 150, 44, 5, RA8875_BLACK);
  tft.print(a_x + 14, a_y + 1, F("Cancel"));
}

// draw a Continue button at a given x and y
void continueButton(const int& a_x, const int& a_y) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK);
  tft.fillRoundRect(a_x, a_y, 190, 42, 5, display::RA8875_GREY);
  tft.drawRoundRect(a_x - 2, a_y - 2, 194, 46, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x - 1, a_y - 1, 192, 44, 5, RA8875_BLACK);
  tft.print(a_x + 14, a_y + 1, F("Continue"));
}

// draw a Calibrate button at a given x and y
void calibrateButton(const int& a_x, const int& a_y) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(RA8875_BLACK);
  tft.fillRoundRect(a_x, a_y, 190, 42, 5, display::RA8875_GREY);
  tft.drawRoundRect(a_x - 2, a_y - 2, 194, 46, 5, RA8875_BLACK);
  tft.drawRoundRect(a_x - 1, a_y - 1, 192, 44, 5, RA8875_BLACK);
  tft.print(a_x + 14, a_y + 1, F("Calibrate"));
}

// draw the slide show seen on the homepage, attributes seen below...
void drawSensorSlide(
  const float & a_sensorData, const float & a_minTarget,
  const float & a_maxTarget, const float & a_offSet,
  const float * a_arrayData, const byte & a_percision) {
  if (display::refreshPage) {
    if (a_offSet != 0)
      tft.setCursor(460, 140);
    else
      tft.setCursor(540, 140);
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.print(F("Target"));
    if (a_offSet != 0) {
      tft.setCursor(tft.getFontX() + 10, 140);
      tft.print(F("+/-"));
      tft.setCursor(tft.getFontX() + 10, 140);
      tft.print(a_offSet, a_percision);
    }
  }
  // draw sensor reading
  static float previousSensorData;
  static int sensorDataPosition;
  if (display::refreshPage || a_sensorData != previousSensorData) {
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setFontScale(2);
    tft.setTextColor(RA8875_BLACK);
    int sensorStartX = 290 - (tft.getStringWidth(a_sensorData, a_percision) / 2);
    tft.fillRect(98, 188, sensorDataPosition - 96, 90, user::backgroundColor);
    tft.print(sensorStartX, 190, a_sensorData, a_percision);
    sensorDataPosition = tft.getFontX();
    previousSensorData = a_sensorData;
  }
  // draw targets
  static int minTargetPosition;
  static float previousMinTarget;
  if (display::refreshPage || a_minTarget != previousMinTarget) {
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setFontScale(1);
    int minTargetStartX = 540 - (tft.getStringWidth(a_minTarget, a_percision) / 2);
    tft.fillRect(438, 228, minTargetPosition - 436, 50, user::backgroundColor);
    tft.print(minTargetStartX, 230, a_minTarget, a_percision); // a_minTarget
    previousMinTarget = a_minTarget;
    minTargetPosition = tft.getFontX();
    tft.setFont(&myriadPro_32px_Regular);
    tft.print(minTargetStartX, 174, F("Min"));
  }
  //
  static int maxTargetPosition;
  static float previousMaxTarget;
  if (display::refreshPage || a_maxTarget != previousMaxTarget) {
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setFontScale(1);
    int maxTargetStartX = 710 - (tft.getStringWidth(a_maxTarget, a_percision) / 2);
    tft.fillRect(608, 228, maxTargetPosition - 606, 50, user::backgroundColor);
    tft.print(maxTargetStartX, 230, a_maxTarget, a_percision); // a_maxTarget
    previousMaxTarget = a_maxTarget;
    maxTargetPosition = tft.getFontX();
    tft.setFont(&myriadPro_32px_Regular);
    tft.print(maxTargetStartX, 174, F("Max"));
  }
  // draw mini graph
  if (display::refreshPage)
    drawGraph(100, 300, 150, tft.width(), 5, true, a_arrayData, device::graphArrayPos, a_percision);
}

// this function is used when setting - updating the RTC chips time / hours
void adjustTime (const int& a_increment) {
  beep();
  if (a_increment < 0) {
    if (user::rtcSecond > 0 && display::currentTouch - display::lastTouch < 10000UL)
      user::rtcSecond--;
    else if (display::currentTouch - display::lastTouch < 20000UL) {
      user::rtcSecond = 59;
      if (user::rtcMinute > 0)
        user::rtcMinute--;
      else {
        user::rtcMinute = 59;
        if (user::rtcHour > 0)
          user::rtcHour--;
        else
          user::rtcHour = 23;
      }
    }
    else {
      if (user::rtcHour > 0)
        user::rtcHour--;
      else
        user::rtcHour = 23;
    }
  }
  else {
    if (user::rtcSecond < 59 && display::currentTouch - display::lastTouch < 10000UL)
      user::rtcSecond++;
    else if (display::currentTouch - display::lastTouch < 20000UL) {
      user::rtcSecond = 0;
      if (user::rtcMinute < 59)
        user::rtcMinute++;
      else {
        user::rtcMinute = 0;
        if (user::rtcHour < 23)
          user::rtcHour++;
        else
          user::rtcHour = 0;
      }
    }
    else {
      if (user::rtcHour < 23)
        user::rtcHour++;
      else
        user::rtcHour = 0;
    }
  }
}

void setTimer(byte& a_min, byte& a_hour, const unsigned int& a_interalInMinutes) {
  unsigned int hours = a_interalInMinutes / 60;
  unsigned int minutes = a_interalInMinutes % 60;
  if (rtcTime.min + minutes < 60)
    a_min = rtcTime.min + minutes;
  else {
    a_min = (rtcTime.min + minutes) - 60;
    hours++;
  }
  if (rtcTime.hour + hours < 24)
    a_hour = rtcTime.hour + hours;
  else
    a_hour = (rtcTime.hour + hours) - 24;
}

// this function is used when setting - updating the rtc chips time / date
void adjustDMY(const int& a_increment) {
  beep();
  if (a_increment < 0) {
    if (user::rtcDay > 0 && display::currentTouch - display::lastTouch < 10000UL)
      user::rtcDay--;
    else {
      user::rtcDay = 31;
      if (user::rtcMonth > 1)
        user::rtcMonth--;
      else if (user::rtcYear > 2022) {
        user::rtcYear--;
        user::rtcMonth = 12;
      }
    }
  }
  else {
    if (user::rtcDay < 31 && display::currentTouch - display::lastTouch < 10000UL)
      user::rtcDay++;
    else {
      user::rtcDay = 1;
      if (user::rtcMonth < 12)
        user::rtcMonth++;
      else if (user::rtcYear < 2099) {
        user::rtcYear++;
        user::rtcMonth = 1;
      }
    }
  }
}

// display the set RTC time dialog
void displaySetRTC() {
  if (display::refreshPage) {
    drawThreeUpDownButtons();
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK);
    tft.fillRoundRect(260, 420, 164, 42, 5, display::RA8875_SMOKE_WHITE);
    tft.drawRoundRect(258, 418, 167, 46, 5, RA8875_BLACK);
    tft.drawRoundRect(259, 419, 166, 44, 5, RA8875_BLACK);
    tft.print(280, 420, F("Cancel"));
    tft.fillRoundRect(455, 420, 169, 42, 5, display::RA8875_SMOKE_WHITE);
    tft.drawRoundRect(453, 418, 172, 46, 5, RA8875_BLACK);
    tft.drawRoundRect(454, 419, 171, 44, 5, RA8875_BLACK);
    tft.print(480, 420, F("Confirm"));
  }
  // day of week string
  static int previousDaysPosition;
  static byte previousUserRTCdayOfWeek;
  const static String daysOfWeek[7] {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
  if (display::refreshPage || previousUserRTCdayOfWeek != user::rtcDayOfWeek) {
    tft.fillRect(104, 228, previousDaysPosition - 104, 50, user::backgroundColor);
    if (user::rtcDayOfWeek == 3)
      tft.setCursor(104, 230);
    else if (user::rtcDayOfWeek == 4)
      tft.setCursor(130, 230);
    else if (user::rtcDayOfWeek == 5)
      tft.setCursor(160, 230);
    else if (user::rtcDayOfWeek == 6)
      tft.setCursor(135, 230);
    else if (user::rtcDayOfWeek == 7)
      tft.setCursor(150, 230);
    else
      tft.setCursor(145, 230);
    tft.print(daysOfWeek[user::rtcDayOfWeek - 1]);
    previousDaysPosition = tft.getFontX();
    previousUserRTCdayOfWeek = user::rtcDayOfWeek;
  }
  // date
  static int previousDatePosition;
  static byte previousUserRTCDay;
  static byte previousUserRTCMonth;
  if (display::refreshPage || previousUserRTCDay != user::rtcDay || previousUserRTCMonth != user::rtcMonth) {
    tft.fillRect(349, 228, previousDatePosition - 349, 50, user::backgroundColor);
    tft.setCursor(350, 230);
    if (user::rtcDay < 10)
      tft.print(F("0"));
    tft.print(user::rtcDay);
    tft.print(F("/"));
    if (user::rtcMonth < 10)
      tft.print(F("0"));
    tft.print(user::rtcMonth);
    tft.print(F("/"));
    tft.print(user::rtcYear - 2000);
    previousDatePosition = tft.getFontX();
    previousUserRTCDay = user::rtcDay;
    previousUserRTCMonth = user::rtcMonth;
  }
  // time
  static int previousTimePosition;
  static byte previousUserRTCMinute;
  static byte previousUserRTCSecond;
  static byte previousUserRtcHour;
  if (display::refreshPage || previousUserRTCMinute != user::rtcMinute || previousUserRTCSecond != user::rtcSecond || previousUserRtcHour != user::rtcHour) {
    tft.fillRect(564, 228, previousTimePosition - 564, 50, user::backgroundColor);
    tft.setCursor(565, 230);
    if (user::rtcHour < 10)
      tft.print(F("0"));
    tft.print(user::rtcHour);
    tft.print(F(":"));
    if (user::rtcMinute < 10)
      tft.print(F("0"));
    tft.print(user::rtcMinute);
    tft.print(F(":"));
    if (user::rtcSecond < 10)
      tft.print(F("0"));
    tft.print(user::rtcSecond);
    previousUserRTCMinute = user::rtcMinute;
    previousUserRTCSecond = user::rtcSecond;
    previousUserRtcHour = user::rtcHour;
    previousTimePosition = tft.getFontX();
  }
}

// draw a large doser icon its doser number, doser value either mls or speed %
void drawDoser(
  const int& a_x, const int& a_y, const byte a_doserNum, const int& a_value,
  int& a_previousValue, int& a_doserPosition, const byte & a_symbol, const int& a_yOffSet)
{
  int startPosition = 0;
  if (display::refreshPage) {
    tft.drawXbmpArray(doserIcon, 800, a_x, a_y, 80, 80, RA8875_BLACK, 2);
  }
  if (display::refreshPage || a_value != a_previousValue) {
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setFontScale(1);
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
    a_symbol == 0 ? tft.print(F("ml")) : tft.print(F("%"));
    a_doserPosition = tft.getFontX();
    tft.setCursor(a_x, a_y);
    tft.print(a_doserNum);
    a_previousValue = a_value;
  }
}

// draw the circle slide position icons
void drawSlideIcons(int a_x, const int& a_y, const byte& a_page, const byte& numOfIcons) {
  for (byte i = 0; i < numOfIcons; i++) {
    if (a_page == i)
      tft.fillCircle(a_x, a_y, 12, RA8875_BLUE);
    else
      tft.fillCircle(a_x, a_y, 10, display::RA8875_GREY);
    a_x += 30;
  }
}

// draw 2 values in large text at give x and y's
void drawTwoValues(
  const int& a_startX, float a_vlaue, const unsigned int& a_color, const byte & a_precison,
  const int& a_startX2, float a_vlaue2, const unsigned int& a_color2, const byte & a_precison2,
  const byte & a_symbol) {
  // val 1
  static float previousValue = 0, previousValue2 = 0;
  static int startX = 0, endX = 0, startX2 = 0, endX2 = 0;
  tft.setFont(&HallfeticaLargenum_42px_Regular);
  tft.setFontScale(2);
  tft.setTextColor(a_color);
  if (display::refreshPage) {
    startX = 0; endX = 0; startX2 = 0; endX2 = 0;
  }
  if (display::refreshPage || previousValue != a_vlaue) {
    tft.fillRect(startX, 248, endX - startX, 90, user::backgroundColor);
    startX = a_startX - (tft.getStringWidth(a_vlaue, a_precison) / 2);
    tft.print(startX, 250, a_vlaue, a_precison);
    printSymbol(tft.getFontX() + 6, 250, a_symbol, a_color);
    endX = tft.getFontX();
    previousValue = a_vlaue;
  }
  // val 2
  tft.setFont(&HallfeticaLargenum_42px_Regular);
  tft.setFontScale(2);
  tft.setTextColor(a_color2);
  if (display::refreshPage || previousValue2 != a_vlaue2) {
    tft.fillRect(startX2, 248, endX2 - startX2, 90, user::backgroundColor);
    startX2 = a_startX2 - (tft.getStringWidth(a_vlaue2, a_precison2) / 2);
    tft.print(startX2, 250, a_vlaue2, a_precison2);
    printSymbol(tft.getFontX() + 6, 250, a_symbol, a_color2);
    endX2 = tft.getFontX();
    previousValue2 = a_vlaue2;
  }
}

// print a F, C, CM or "
void printSymbol(const int& a_x, const int& a_y, const byte & a_symbol, const unsigned int& a_color) {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  tft.setTextColor(a_color);
  if (a_symbol == 1)
    tft.print(a_x, a_y, F("C"));
  else if (a_symbol == 2)
    tft.print(a_x, a_y, F("F"));
  else if (a_symbol == 3)
    tft.print(a_x, a_y + 50, F("CM"));
  else if (a_symbol == 4)
    tft.print(a_x, a_y, F("\""));
  else if (a_symbol == 5)
    tft.print(a_x, a_y + 50, F("%"));
  else if (a_symbol == 6)
    tft.print(a_x, a_y + 50, F("mV"));
}

// set a warning text color based on the passed over attributes
unsigned int setWarningColor(const float& a_sensor, const float& a_minTarget, const float& a_maxTarget, const float& a_targetOffset, const float& a_warningMargin) {
  unsigned int color = RA8875_BLACK;
  if (a_sensor < ((a_minTarget - a_targetOffset) - a_warningMargin))
    color = RA8875_RED;
  else if (a_sensor > ((a_maxTarget + a_targetOffset) + a_warningMargin))
    color = RA8875_RED;
  else if (a_sensor < (a_minTarget - a_targetOffset))
    color = RA8875_YELLOW;
  else if (a_sensor > (a_maxTarget + a_targetOffset))
    color = RA8875_YELLOW;
  return color;
}
