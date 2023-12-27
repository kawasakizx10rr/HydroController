void touchEvent() {
  if (tft.touched()) {
    display::currentTouch = millis();
    //display::touch_x = 0; display::touch_y = 0;
    static uint16_t previousTouchX = 0, previousTouchY = 0;
    //if (!display::enableAverageTouches)
    //  tft.touchReadPixel(&display::touch_x, &display::touch_y);
    //else {
      uint16_t t_touchX = 0, t_touchY = 0;
      tft.touchReadPixel(&t_touchX, &t_touchY);
      display::touch_x = t_touchX;
      display::touch_y = t_touchY;
      //getAverageTouch(t_touchX, t_touchY);
    //}
    if (display::touch_x > 0 && display::touch_y > 0) {
      //tft.fillCircle(display::touch_x, display::touch_y, 2, RA8875_WHITE); // for touch debugging / calibration check
      if (abs(display::touch_x - previousTouchX) <= 20 && abs(display::touch_y - previousTouchY) <= 20)
        mainMenuNavigation();
      sliders();
      if (millis() - display::previousTouchMillis >= display::debounceTime) {
        adjustPageValues();
        display::previousTouchMillis = millis();
      }
      previousTouchX = display::touch_x;
      previousTouchY = display::touch_y;
    }
  }
  else if (millis() > display::currentTouch + display::debounceTime) {
    display::lastTouch = millis();
   // display::lastLeftTouch = tft.width();
   // display::lastRightTouch = 0;
   // display::lastUpTouch = tft.height();
   // display::lastDownTouch = 0;
    display::averageTouchCnt = 0;
    if (device::isPriming != 0) {
      device::isPriming = 0;
      prime(0, 0, 0);
    }
  }
  display::debounceTime = 250;
}

void mainMenuNavigation() {
  if (!display::displayIsOff) {
    if (display::touch_x >= 10 && display::touch_x <= 90 && display::touch_y >= 90 && display::touch_y <= 160) { // HOME
      setPage(0);
      device::slideShowpreviousMillis = millis();
    }
    else if (display::touch_x >= 10 && display::touch_x <= 90 && display::touch_y >= 160 && display::touch_y <= 230) { // GRAPH
      setPage(1);
    }
    else if (display::touch_x >= 10 && display::touch_x <= 90 && display::touch_y >= 230 && display::touch_y <= 300) { // MAX MINS
      display::maxMinsPage = 0;
      setPage(2);
    }
    else if (display::touch_x >= 10 && display::touch_x <= 90 && display::touch_y >= 300 && display::touch_y <= 370) { // PROFILES
      display::showKeyboard = false;
      device::lockSaveButtons = false;
      device::updateKeyboardInput = false;
      setPage(3);
    }
    else if (display::touch_x >= 10 && display::touch_x <= 90 && display::touch_y >= 370 && display::touch_y <= 450) { // SETTINGS
      display::settingsPage = 0;
      display::calDoserPageScrollPos = 0;
      display::settingsPageOneScrollPos = 0;
      display::settingsPageTwoScrollPos = 0;
      display::calPhPageScrollPos = 0;
      display::showSystemLogs = false;
      user::showSr04HeightCalibration = false;
      user::showEtapeCalibration = device::UNCHANGED;
      user::showDoserCalibration = false;
      user::setRTCtime = false;
      user::showTdsCalibration = false;
      user::showCo2Calibration = false;
      user::showPhCalibration = false;
      setPage(4);
    }
    else if (display::touch_x >= 15 && display::touch_x <= 95 && display::touch_y >= 1 && display::touch_y <= 100) { // EC / TDS
      setPage(5);
    }
    else if (display::touch_x >= 116 && display::touch_x <= 196 && display::touch_y >= 1 && display::touch_y <= 100) { // PH
      setPage(6);
    }
    else if (display::touch_x >= 223 && display::touch_x <= 303 && display::touch_y >= 1 && display::touch_y <= 100) { // CO2
      display::co2PageScrollPos = 0;
      setPage(7);
    }
    else if (display::touch_x >= 324 && display::touch_x <= 404 && display::touch_y >= 1 && display::touch_y <= 100) { // WATER height / LVL
      display::waterPageScrollPos = 0;
      setPage(8);
    }
    else if (display::touch_x >= 420 && display::touch_x <= 500 && display::touch_y >= 1 && display::touch_y <= 100) { // DOSERS
      setPage(9);
      display::doserPageScrollPos = 0;
    }
    else if (display::touch_x >= 507 && display::touch_x <= 587 && display::touch_y >= 1 && display::touch_y <= 100) { // LIGHT
      setPage(10);
    }
    else if (display::touch_x >= 596 && display::touch_x <= 676 && display::touch_y >= 1 && display::touch_y <= 100) { // FANS
      display::fansPage = 0;
      setPage(11);
    }
    if (display::touch_x >= 696 && display::touch_x <= 776 && display::touch_y >= 1 && display::touch_y <= 100) { // WARNINGS
      display::warningsPage = 0;
      setPage(12);
    }
  }
}

void sliders() {
  const int slideXIncrement = sliderXTouch();
  const int slideYIncrement = sliderYTouch();
  // Home page X axsis slider
  if (display::page == 0) {
    if (slideXIncrement != 0) {
      if (slideXIncrement == 1) {
        if (display::homePage < 9)
          display::homePage += slideXIncrement;
        else
          display::homePage = 0;
      }
      else if (slideXIncrement == -1) {
        if (display::homePage > 0)
          display::homePage += slideXIncrement;
        else
          display::homePage = 9;
      }
      beep();
      clearPage();
      display::refreshPage = true;
      device::slideShowpreviousMillis = millis();
    }
  }
  // MaxMins page X axsis slider
  else if (display::page == 2) {
    if (slideXIncrement != 0) {
      if (slideXIncrement == 1) {
        if (display::maxMinsPage < 9)
          display::maxMinsPage++;
        else
          display::maxMinsPage = 0;
      }
      else if (slideXIncrement == -1) {
        if (display::maxMinsPage > 0)
          display::maxMinsPage--;
        else
          display::maxMinsPage = 9;
      }
      beep();
      display::refreshPage = true;
      clearPage();
    }
  }
  // Settings page zero Y axsis slider
  else if (display::page == 4) {
    // Doser calibration settings page X axsis slider
    if (user::showDoserCalibration) {
      if (slideXIncrement == 1 && display::calDoserPageScrollPos < user::numberOfDosers - 4) {
        display::calDoserPageScrollPos++;
        display::refreshPage = true;
        beep();
        clearPage();
      }
      else if (slideXIncrement == -1 && display::calDoserPageScrollPos > 0) {
        display::calDoserPageScrollPos--;
        display::refreshPage = true;
        beep();
        clearPage();
      }
    }
    // PH calibration settings page
    else if (user::showPhCalibration) {
      if (slideXIncrement == 1 && display::calPhPageScrollPos < 2) {
        display::calPhPageScrollPos++;
        display::refreshPage = true;
        beep();
        clearPage();
      }
      else if (slideXIncrement == -1 && display::calPhPageScrollPos > 0) {
        display::calPhPageScrollPos--;
        display::refreshPage = true;
        beep();
        clearPage();
      }
    }
    //
    else if (display::showSystemLogs) {
      if (slideYIncrement == 1 && message::systemLogPos > 5 && display::systemLogScrollPos < message::systemLogPos - 6) {
        display::systemLogScrollPos++;
        display::refreshPage = true;
        beep();
        clearPage();
      }
      else if (slideYIncrement == -1 && display::systemLogScrollPos > 0) {
        display::systemLogScrollPos--;
        display::refreshPage = true;
        beep();
        clearPage();
      }
      //Serial.print("systemLogScrollPos: "); Serial.print(display::systemLogScrollPos); Serial.print(", max systemLogPos: "); Serial.println(message::systemLogPos);
    }

    if (display::settingsPage == 0) {
      if (!user::setRTCtime) {
        if (slideYIncrement == 1 && display::settingsPageZeroScrollPos < 2) {
          display::settingsPageZeroScrollPos++;
          display::refreshPage = true;
          beep();
          clearPage();
        }
        else if (slideYIncrement == -1 && display::settingsPageZeroScrollPos > 0) {
          display::settingsPageZeroScrollPos--;
          display::refreshPage = true;
          beep();
          clearPage();
        }
      }
    }
    else if (display::settingsPage == 1) {
      if (!user::resetSettings) {
        if (slideYIncrement == 1 && display::settingsPageOneScrollPos < 7) {
          display::settingsPageOneScrollPos++;
          display::refreshPage = true;
          beep();
          clearPage();
        }
        else if (slideYIncrement == -1 && display::settingsPageOneScrollPos > 0) {
          display::settingsPageOneScrollPos--;
          display::refreshPage = true;
          beep();
          clearPage();
        }
      }
    }
    else if (display::settingsPage == 2) {
      if (slideYIncrement == 1 && display::settingsPageTwoScrollPos < 2) {
        display::settingsPageTwoScrollPos++;
        display::refreshPage = true;
        beep();
        clearPage();
      }
      else if (slideYIncrement == -1 && display::settingsPageTwoScrollPos > 0) {
        display::settingsPageTwoScrollPos--;
        display::refreshPage = true;
        beep();
        clearPage();
      }
    }
    // Settings page X axsis slider
    if (!user::showDoserCalibration && !user::setRTCtime && !display::showSystemLogs && !user::showPhCalibration && !user::resetSettings) {
      if (slideXIncrement == 1 && display::settingsPage < 3) {
        display::settingsPage++;
        display::refreshPage = true;
        beep();
        clearPage();
      }
      else if (slideXIncrement == -1 && display::settingsPage > 0) {
        display::settingsPage--;
        display::refreshPage = true;
        beep();
        clearPage();
      }
    }
  }
  // Co2 page X axsis slider
  else if (display::page == 7) {
    if (slideXIncrement == 1 && display::co2PageScrollPos < 3) {
      display::co2PageScrollPos++;
      display::refreshPage = true;
      beep();
      clearPage();
    }
    else if (slideXIncrement == -1 && display::co2PageScrollPos > 0) {
      display::co2PageScrollPos--;
      display::refreshPage = true;
      beep();
      clearPage();
    }
  }
  // Water page X axsis slider
  else if (display::page == 8) {
    if (slideXIncrement == 1) {
      if (display::waterPageScrollPos < 3) {
        display::waterPageScrollPos++;
        display::refreshPage = true;
        beep();
        clearPage();
      }
      else if (display::waterPageScrollPos == 3 && display::refillDoserPageScrollPos < user::numberOfDosers - 4) {
        display::refillDoserPageScrollPos++;
        display::refreshPage = true;
        beep();
        clearPage();
      }
    }
    else if (slideXIncrement == -1) {
      if (display::waterPageScrollPos == 3 && display::refillDoserPageScrollPos > 0) {
        display::refillDoserPageScrollPos--;
        display::refreshPage = true;
        beep();
        clearPage();
      }
      else if (display::waterPageScrollPos > 0) {
        display::waterPageScrollPos--;
        display::refreshPage = true;
        beep();
        clearPage();
      }
      else {

      }
    }
  }
  // Dosers page X axsis slider
  else if (display::page == 9) {
    if (slideXIncrement == 1 && display::doserPageScrollPos < user::numberOfDosers - 4) {
      display::doserPageScrollPos++;
      display::refreshPage = true;
      beep();
      clearPage();
    }
    else if (slideXIncrement == -1 && display::doserPageScrollPos > 0) {
      display::doserPageScrollPos--;
      display::refreshPage = true;
      beep();
      clearPage();
    }
  }
  // Fans page X axsis slider
  else if (display::page == 11) {
    if (slideXIncrement == 1 && display::fansPage < 4) {
      display::fansPage++;
      display::refreshPage = true;
      beep();
      clearPage();
    }
    else if (slideXIncrement == -1 && display::fansPage > 0) {
      display::fansPage--;
      display::refreshPage = true;
      beep();
      clearPage();
    }
  }
  // Warnings page X axsis slider
  else if (display::page == 12) {
    if (slideXIncrement != 0) {
      if (slideXIncrement == 1) {
        if (display::warningsPage < 7)
          display::warningsPage++;
        else
          display::warningsPage = 0;
      }
      else if (slideXIncrement == -1) {
        if (display::warningsPage > 0)
          display::warningsPage--;
        else
          display::warningsPage = 7;
      }
      beep();
      display::refreshPage = true;
      clearPage();
    }
  }
}


int sliderXTouch() {
  static int scrollLeftCount = 0, scrollRightCount = 0;
  int returnVal = 0;
  //if (display::currentTouch > display::lastTouch) {
    if ((int16_t)display::touch_x - (int16_t)display::lastTouchX >= 35) {
      scrollRightCount++;
      //Serial.print(F("slide R ")); Serial.println(scrollRightCount);
      scrollLeftCount = 0;
      display::lastTouchX = display::touch_x;
    }
    else if ((int16_t)display::touch_x - (int16_t)display::lastTouchX <= -35) {
      scrollLeftCount++;
      //Serial.print(F("slide L ")); Serial.println(scrollLeftCount);
      scrollRightCount = 0;
      display::lastTouchX = display::touch_x;
    }
  //}
  if (scrollRightCount >= 8) {
    returnVal = 1;
    scrollRightCount = 0;
    //display::lastTouchX = 0;
  }
  else if (scrollLeftCount >= 8) {
    returnVal = -1;
    scrollLeftCount = 0;
    //display::lastTouchX = tft.width();
  }
  //if (returnVal != 0) {
  //  Serial.print(F("slide x touch ")); Serial.println(returnVal);
  //}
  return returnVal;
}


int sliderYTouch() {
  static int scrollDownCount = 0, scrollUpCount = 0;
  int returnVal = 0;
  //if (display::currentTouch > display::lastTouch) {
    if ((int16_t)display::touch_y - (int16_t)display::lastTouchY >= 20) {
      scrollDownCount++;
      scrollUpCount = 0;
      display::lastTouchY = display::touch_y;
    }
    else if ((int16_t)display::touch_y - (int16_t)display::lastTouchY <= -20) {
      scrollUpCount++;
      scrollDownCount = 0;
      display::lastTouchY = display::touch_y;
    }
  //}
  if (scrollDownCount >= 10) {
    returnVal = 1;
    scrollDownCount = 0;
    //display::lastDownTouch = 0;
  }
  else if (scrollUpCount >= 10) {
    returnVal = -1;
    scrollUpCount = 0;
    //display::lastUpTouch = tft.height();
  }
  //if (returnVal != 0) {
  //Serial.print(F("slide y touch ")); Serial.println(returnVal);
  //}
  return returnVal;
}

void adjustPageValues() {
  if (display::page == 1)
    graphsPageTouched();
  else if (display::page == 3)
    profilesPageTouched();
  else if (display::page == 4)
    settingsPageTouched();
  else if (display::page == 5)
    ecTdsPageTouched();
  else if (display::page == 6)
    phPageTouched();
  else if (display::page == 7)
    co2PageTouched();
  else if (display::page == 8)
    waterPageTouched();
  else if (display::page == 9)
    dosersPageTouched();
  else if (display::page == 10)
    lightingPageTouched();
  else if (display::page == 11)
    fansPageTouched();
  else if (display::page == 12)
    warningsPageTouched();
}

void graphsPageTouched() {
  if (display::touch_x >= 760 && display::touch_x <= 835) {
    if (display::touch_y >= 90 && display::touch_y <= 124)
      setGraphOption(1);
    else if ( display::touch_y >= 124 && display::touch_y <= 158)
      setGraphOption(2);
    else if ( display::touch_y >= 158 && display::touch_y <= 192)
      setGraphOption(3);
    else if ( display::touch_y >= 192 && display::touch_y <= 230)
      setGraphOption(4);
    else if ( display::touch_y >= 230 && display::touch_y <= 268)
      setGraphOption(5);
    else if ( display::touch_y >= 268 && display::touch_y <= 310)
      setGraphOption(6);
    else if ( display::touch_y >= 310 && display::touch_y <= 354)
      setGraphOption(7);
    else if ( display::touch_y >= 354 && display::touch_y <= 394)
      setGraphOption(8);
    else if ( display::touch_y >= 394 && display::touch_y <= 434)
      setGraphOption(9);
    else if ( display::touch_y >= 434 && display::touch_y <= 474)
      setGraphOption(10);
  }
}

void profilesPageTouched() {
  if (display::showSaveDialog && !display::showKeyboard) {
    if (display::touch_x >= 284 && display::touch_x <= 422 && display::touch_y >= 368 && display::touch_y <= 414) { // cancel saving profile
      beep();
      display::showSaveDialog = false;
      display::refreshPage = true;
      clearPage();
    }
    else if (display::touch_x >= 460 && display::touch_x <= 638 && display::touch_y >= 368 && display::touch_y <= 414) { // continue with saving profile
      beep();
      saveSystemEEPROM();
      device::usernameAdjusted = true;
      delay(20);
      EEPROM.put(7, device::userProfile);
      delay(20);
      device::lockSaveButtons = true;
      device::keyBoardClosedTime = millis();
      saveUserEEPROM(device::userProfile);
      display::showSaveDialog = false;
      display::refreshPage = true;
      clearPage();
    }
  }
  else if (device::lockSaveButtons) {
    if (millis() - device::keyBoardClosedTime >= 3000UL) {
      device::lockSaveButtons = false;
      Serial.println(F("Unlocking save/load buttons"));
    }
  }
  else if (!display::showKeyboard && !device::lockSaveButtons) {
    // Keybaord save, load, input box buttons
    if (display::touch_x >= 130 && display::touch_x <= 470 && display::touch_y >= 160 && display::touch_y <= 202)
      loadKeyBoard(1);
    else if (display::touch_x >= 130 && display::touch_x <= 470 && display::touch_y >= 222 && display::touch_y <= 262)
      loadKeyBoard(2);
    else if (display::touch_x >= 130 && display::touch_x <= 470 && display::touch_y >= 284 && display::touch_y <= 326)
      loadKeyBoard(3);
    else if (display::touch_x >= 130 && display::touch_x <= 470 && display::touch_y >= 346 && display::touch_y <= 388)
      loadKeyBoard(4);
    else if (display::touch_x >= 130 && display::touch_x <= 470 && display::touch_y >= 408 && display::touch_y <= 450)
      loadKeyBoard(5);
    else if (display::touch_x >= 490 && display::touch_x <= 620 && display::touch_y >= 160 && display::touch_y <= 202) {
      beep();
      device::userProfile = 0;
      display::refreshPage = true;
      display::showSaveDialog = true;
    }
    else if (display::touch_x >= 490 && display::touch_x <= 620 && display::touch_y >= 222 && display::touch_y <= 262) {
      beep();
      device::userProfile = 1;
      device::usernameAdjusted = true;
      display::refreshPage = true;
      display::showSaveDialog = true;
    }
    else if (display::touch_x >= 490 && display::touch_x <= 620 && display::touch_y >= 284 && display::touch_y <= 326) {
      beep();
      device::userProfile = 2;
      device::usernameAdjusted = true;
      display::refreshPage = true;
      display::showSaveDialog = true;;
    }
    else if (display::touch_x >= 490 && display::touch_x <= 620 && display::touch_y >= 346 && display::touch_y <= 388) {
      beep();
      device::userProfile = 3;
      device::usernameAdjusted = true;
      display::refreshPage = true;
      display::showSaveDialog = true;
    }
    else if (display::touch_x >= 490 && display::touch_x <= 620 && display::touch_y >= 408 && display::touch_y <= 450) {
      beep();
      device::userProfile = 4;
      device::usernameAdjusted = true;
      display::refreshPage = true;
      display::showSaveDialog = true;
    }
    else if (display::touch_x >= 630 && display::touch_x <= 760 && display::touch_y >= 160 && display::touch_y <= 202) {
      beep();
      device::userProfile = 0;
      device::usernameAdjusted = true;
      display::refreshPage = true;
      device::lockSaveButtons = true;
      device::keyBoardClosedTime = millis();
      loadUserEEPROM(device::userProfile);
      EEPROM.put(7, device::userProfile);
      delay(20);
    }
    else if (display::touch_x >= 630 && display::touch_x <= 760 && display::touch_y >= 222 && display::touch_y <= 262) {
      beep();
      device::userProfile = 1;
      device::usernameAdjusted = true;
      display::refreshPage = true;
      device::lockSaveButtons = true;
      device::keyBoardClosedTime = millis();
      loadUserEEPROM(device::userProfile);
      EEPROM.put(7, device::userProfile);
      delay(20);
    }
    else if (display::touch_x >= 630 && display::touch_x <= 760 && display::touch_y >= 284 && display::touch_y <= 326) {
      beep();
      device::userProfile = 2;
      device::usernameAdjusted = true;
      display::refreshPage = true;
      device::lockSaveButtons = true;
      device::keyBoardClosedTime = millis();
      loadUserEEPROM(device::userProfile);
      EEPROM.put(7, device::userProfile);
      delay(20);
    }
    else if (display::touch_x >= 630 && display::touch_x <= 760 && display::touch_y >= 346 && display::touch_y <= 388) {
      beep();
      device::userProfile = 3;
      device::usernameAdjusted = true;
      display::refreshPage = true;
      device::lockSaveButtons = true;
      device::keyBoardClosedTime = millis();
      loadUserEEPROM(device::userProfile);
      EEPROM.put(7, device::userProfile);
      delay(20);
    }
    else if (display::touch_x >= 630 && display::touch_x <= 760 && display::touch_y >= 408 && display::touch_y <= 450) {
      beep();
      device::userProfile = 4;
      device::usernameAdjusted = true;
      display::refreshPage = true;
      device::lockSaveButtons = true;
      device::keyBoardClosedTime = millis();
      loadUserEEPROM(device::userProfile);
      EEPROM.put(7, device::userProfile);
      delay(20);
    }
  }
  else if (display::showKeyboard) {
    static unsigned long previousKeyTouch;
    if (millis() - previousKeyTouch > 500UL) {
      keyboardTouched();
      previousKeyTouch = millis();
    }
  }
}

void settingsPageTouched() {
  // DISPLAY SETTINGS
  if (display::settingsPage == 0)
    settingsZeroPageTouched();
  else if (display::settingsPage == 1)
    settingsOnePageTouched();
  else if (display::settingsPage == 2)
    settingsTwoPageTouched();
  else if (display::settingsPage == 3)
    settingsThreePageTouched();
}

void settingsZeroPageTouched() {
  if (user::setRTCtime) {
    if (display::touch_x >= 145 && display::touch_x <= 210 && display::touch_y >= 340 && display::touch_y <= 420 ) // day of week down
      user::rtcDayOfWeek = adjustValue(user::rtcDayOfWeek, -1, 1, 7);
    else if (display::touch_x >= 225 && display::touch_x <= 290 && display::touch_y >= 340 && display::touch_y <= 420 ) // day of week up
      user::rtcDayOfWeek = adjustValue(user::rtcDayOfWeek, 1, 1, 7);
    else if (display::touch_x >= 350 && display::touch_x <= 415 && display::touch_y >= 340 && display::touch_y <= 420) // day month year down
      adjustDMY(-1);
    else if (display::touch_x >= 430 && display::touch_x <= 495 && display::touch_y >= 340 && display::touch_y <= 420) // day month year up
      adjustDMY(1);
    else if (display::touch_x >= 560 && display::touch_x <= 625 && display::touch_y >= 340 && display::touch_y <= 420) // hour min second down
      adjustTime(-1);
    else if (display::touch_x >= 640 && display::touch_x <= 705 && display::touch_y >= 340 && display::touch_y <= 420) // hour min second up
      adjustTime(1);
    else if (display::touch_x >= 260 && display::touch_x <= 420 && display::touch_y >= 430 && display::touch_y <= 470) { // cancel
      beep();
      user::setRTCtime = false;
      clearPage();
      display::refreshPage = true;
    }
    else if (display::touch_x >= 460 && display::touch_x <= 620 && display::touch_y >= 430 && display::touch_y <= 470) { // confirm
      beep();
      adjustRTCtime();
      user::setRTCtime = false;
      clearPage();
      display::refreshPage = true;
    }
  }
  else {
    int scrollMargin = display::settingsPageZeroScrollPos * 44;
    int touchX = display::touch_x, touchY = display::touch_y;
    if (display::settingsPageZeroScrollPos == 0) {
      if (touchX >= 540 && touchX <= 570 && touchY >= 168 - scrollMargin && touchY <= 198 - scrollMargin) // Change background color to light blue
        setBackground(display::RA8875_LIGHT_BLUE);
      else if (touchX >= 590 && touchX <= 620 && touchY >= 168 - scrollMargin && touchY <= 198 - scrollMargin) // Change background color to light green
        setBackground(display::RA8875_LIGHT_GREEN);
      else if (touchX >= 640 && touchX <= 670 && touchY >= 168 - scrollMargin && touchY <= 198 - scrollMargin) // Change background color to light purple
        setBackground(RA8875_PURPLE);
      else if (touchX >= 690 && touchX <= 720 && touchY >= 168 - scrollMargin && touchY <= 198 - scrollMargin) // Change background color to light pink
        setBackground(display::RA8875_LIGHT_PINK);
      else if (touchX >= 740 && touchX <= 770 && touchY >= 168 - scrollMargin && touchY <= 198 - scrollMargin) // Change background color to white
        setBackground(RA8875_WHITE);
    }
    if (display::settingsPageZeroScrollPos <= 1) {
      if (touchX >= 700 && touchX <= 715 && touchY >= 214 - scrollMargin && touchY <= 244 - scrollMargin) // afkTime time down
        user::afkTime = adjustValue(user::afkTime, -1, 0, 240);
      else if (touchX >= 755 && touchX <= 770 && touchY >= 214 - scrollMargin && touchY <= 244 - scrollMargin) // afkTime time up
        user::afkTime = adjustValue(user::afkTime, 1, 0, 240);
    }
    if (display::settingsPageZeroScrollPos <= 2) {
      if (touchX >= 700 && touchX <= 715 && touchY >= 258 - scrollMargin && touchY <= 288 - scrollMargin) // graphInterval time down
        user::graphInterval = adjustValue(user::graphInterval, -1, 1, 240);
      else if (touchX >= 755 && touchX <= 770 && touchY >= 258 - scrollMargin && touchY <= 288 - scrollMargin) // graphInterval time up
        user::graphInterval = adjustValue(user::graphInterval, 1, 1, 240);
    }
    if (display::settingsPageZeroScrollPos <= 3) {
      if (touchX >= 700 && touchX <= 770 && touchY >= 302 - scrollMargin && touchY <= 332 - scrollMargin) { // setRTCtime
        beep();
        rtcTime = rtc.getTime();
        user::rtcDayOfWeek = rtcTime.dow;
        user::rtcSecond = rtcTime.sec;
        user::rtcMinute = rtcTime.min;
        user::rtcHour = rtcTime.hour;
        user::rtcDay = rtcTime.date;
        user::rtcMonth = rtcTime.mon;
        if (rtcTime.year >= 2020)
          user::rtcYear = rtcTime.year;
        else
          user::rtcYear = 2020;
        user::setRTCtime = true;
        clearPage();
        display::refreshPage = true;
      }
    }
    if (display::settingsPageZeroScrollPos <= 4) {
      if (touchX >= 700 && touchX <= 770 && touchY >= 346 - scrollMargin && touchY <= 376 - scrollMargin) { // convertToFarenheit
        user::convertToF  = !user::convertToF;
        device::settingsAdjusted = true;
        beep();
      }
    }
    if (display::settingsPageZeroScrollPos <= 5) {
      if (touchX >= 700 && touchX <= 770 && touchY >= 390 - scrollMargin && touchY <= 420 - scrollMargin) { // convertToInches
        user::convertToInches = !user::convertToInches;
        device::settingsAdjusted = true;
        beep();
      }
    }
    if (display::settingsPageZeroScrollPos >= 1) {
      if (touchX >= 700 && touchX <= 770 && touchY >= 434 - scrollMargin && touchY <= 464 - scrollMargin) { // convertToTds
        beep();
        tft.fillRect(11, 11, 76, 74, user::backgroundColor);
        user::convertToTds = !user::convertToTds;
        device::settingsAdjusted = true;
        if (user::convertToTds)
          drawPageIconQuick(5, 23, 32, ppmIcon, 280, 64, 35);
        else
          drawPageIconQuick(5, 26, 32, ecIcon, 256, 57, 36);
      }
    }
    if (display::settingsPageZeroScrollPos >= 2) {
      if (touchX >= 700 && touchX <= 770 && touchY >= 478 - scrollMargin && touchY <= 508 - scrollMargin) { // EC conversion mode
        static unsigned long previousCtTime = millis();
        if (millis() - previousCtTime >= 500UL) {
          beep();
          if (device::conversionType == device::EU)
            device::conversionType = device::US;
          else if (device::conversionType == device::US)
            device::conversionType = device::AU;
          else if (device::conversionType == device::AU)
            device::conversionType = device::EU;
          previousCtTime = millis();
        }
      }
    }
  }
}

void settingsOnePageTouched() {
  if (display::showSystemLogs) {
    if (display::touch_x >= 690 && display::touch_x <= 799 && display::touch_y >= 430 && display::touch_y <= 479) { // exit
      beep();
      display::showSystemLogs = false;
      display::refreshPage = true;
      clearPage();
    }
  }
  else if (user::resetSettings) {
    if (display::touch_x >= 284 && display::touch_x <= 422 && display::touch_y >= 368 && display::touch_y <= 414) { // cancel factory reset
      beep();
      user::resetSettings = false;
      display::refreshPage = true;
      clearPage();
    }
    else if (display::touch_x >= 460 && display::touch_x <= 638 && display::touch_y >= 368 && display::touch_y <= 414) { // continue with factory reset
      beep();
      EEPROM.put(6, true); // set the stored device::systemReset to true for next boot
      delay(100);
      reset();
    }
  }
  else {
    int scrollMargin = display::settingsPageOneScrollPos * 44;  // spacing is 44px : +30 +14
    int touchX = display::touch_x, touchY = display::touch_y;
    if (display::settingsPageOneScrollPos == 0) {
      if (touchX >= 700 && touchX <= 770 && touchY >= 170 - scrollMargin && touchY <= 200 - scrollMargin) { // Show system logs
        beep();
        display::showSystemLogs = true;
        display::refreshPage = true;
        clearPage();
      }
    }
    if (display::settingsPageOneScrollPos <= 1) {
      if (touchX >= 700 && touchX <= 770 && touchY >= 214 - scrollMargin && touchY <= 244 - scrollMargin) { // enable Wifi
        static unsigned long previousWifiOnOffTime = millis();
        if (millis() >= previousWifiOnOffTime + 1000UL) {
          wifi::wifiEnabled = !wifi::wifiEnabled;
          beep();
          digitalWrite(pin::espTransistor, (wifi::wifiEnabled ? HIGH : LOW));
          wifi::wifiPreviousMillis = millis() + 5000UL;
          previousWifiOnOffTime = millis();
        }
      }
    }
    if (display::settingsPageOneScrollPos <= 2) {
      if (touchX >= 700 && touchX <= 715 && touchY >= 258 - scrollMargin && touchY <= 288 - scrollMargin) { // numberOfDosers down
        user::numberOfDosers = adjustValue(user::numberOfDosers, -1, 4, 6);
        device::settingsAdjusted = true;
      }
      else if (touchX >= 755 && touchX <= 770 && touchY >= 258 - scrollMargin && touchY <= 288 - scrollMargin) { // numberOfDosers up
        user::numberOfDosers = adjustValue(user::numberOfDosers, 1, 4, 6);
        device::settingsAdjusted = true;
      }
    }
    if (display::settingsPageOneScrollPos <= 3) {
      if (touchX >= 700 && touchX <= 715 && touchY >= 302 - scrollMargin && touchY <= 332 - scrollMargin) { // dosingInterval time down
        user::dosingInterval = adjustValue(user::dosingInterval, -1, 0, 240);
        device::settingsAdjusted = true;
      }
      else if (touchX >= 755 && touchX <= 770 && touchY >= 302 - scrollMargin && touchY <= 332 - scrollMargin) { // dosingInterval time up
        user::dosingInterval = adjustValue(user::dosingInterval, 1, 0, 240);
        device::settingsAdjusted = true;
      }
    }
    if (display::settingsPageOneScrollPos <= 4) {
      if (touchX >= 700 && touchX <= 770 && touchY >= 346 - scrollMargin && touchY <= 376 - scrollMargin) { // useEtapeSensor
        static unsigned long previousDMLTime = millis();
        if (millis() >= previousDMLTime + 500UL) {
          user::useEtapeSensor = !user::useEtapeSensor;
          beep();
          previousDMLTime = millis();
        }
      }
    }
    if (display::settingsPageOneScrollPos <= 5) {
      if (touchX >= 700 && touchX <= 770 && touchY >= 390 - scrollMargin && touchY <= 420 - scrollMargin) { // disableLED
        beep();
        user::disableLED = !user::disableLED;
        user::disableLED ? sendToSlave('L', 1) : sendToSlave('L', 0);
      }
    }
    if (display::settingsPageOneScrollPos >= 1 && display::settingsPageOneScrollPos <= 6) {
      if (touchX >= 700 && touchX <= 770 && touchY >= 434 - scrollMargin && touchY <= 464 - scrollMargin) { // disableBeeper
        user::disableBeeper = !user::disableBeeper;
        user::disableBeeper ? sendToSlave('B', 1) : sendToSlave('B', 0);
        beep();
      }
    }
    if (display::settingsPageOneScrollPos >= 2 && display::settingsPageOneScrollPos <= 7) {
      if (touchX >= 700 && touchX <= 770 && touchY >= 478 - scrollMargin && touchY <= 508 - scrollMargin) { // disableMachineLearning
        static unsigned long previousDMLTime = millis();
        if (millis() >= previousDMLTime + 500UL) {
          user::disableMachineLearning = !user::disableMachineLearning;
          beep();
          previousDMLTime = millis();
        }
      }
    }
    if (display::settingsPageOneScrollPos >= 3 && display::settingsPageOneScrollPos <= 8) {
      if (touchX >= 700 && touchX <= 770 && touchY >= 522 - scrollMargin && touchY <= 552 - scrollMargin) { // clear graph history
        device::graphArrayPos = 0;
        beep();
      }
    }
    if (display::settingsPageOneScrollPos >= 4 && display::settingsPageOneScrollPos <= 9) {
      if (touchX >= 700 && touchX <= 770 && touchY >= 566 - scrollMargin && touchY <= 596 - scrollMargin) { // clear max mins
        beep();
        clearMaxMins();
        setMaxMins();
      }
    }
    if (display::settingsPageOneScrollPos >= 5 && display::settingsPageOneScrollPos <= 10) {
      if (touchX >= 700 && touchX <= 770 && touchY >= 610 - scrollMargin && touchY <= 640 - scrollMargin) { // reset Machine learning
        beep();
        resetMachineLearning();
      }
    }
    if (display::settingsPageOneScrollPos >= 6 && display::settingsPageOneScrollPos <= 11) {
      if (touchX >= 700 && touchX <= 770 && touchY >= 654 - scrollMargin && touchY <= 684 - scrollMargin) { // reset system logs
        beep();
        clearSystemLogs();
      }
    }
    if (display::settingsPageOneScrollPos >= 7 && display::settingsPageOneScrollPos <= 12) {
      if (touchX >= 700 && touchX <= 770 && touchY >= 698 - scrollMargin && touchY <= 728 - scrollMargin) { // reset all device Settings
        user::resetSettings = true;
        display::refreshPage = true;
        beep();
      }
    }
  }
}

void settingsTwoPageTouched() {
  int scrollMargin = display::settingsPageTwoScrollPos * 44;
  int touchX = display::touch_x, touchY = display::touch_y;
  if (display::settingsPageTwoScrollPos == 0) {
    if (touchX >= 700 && touchX <= 770 && touchY >= 170 - scrollMargin && touchY <= 200 - scrollMargin) { // disable PPM warnings
      user::disablePpmWarnings = !user::disablePpmWarnings;
      device::settingsAdjusted = true;
      beep();
    }
  }
  if (display::settingsPageTwoScrollPos <= 1) {
    if (touchX >= 700 && touchX <= 770 && touchY >= 214 - scrollMargin && touchY <= 244 - scrollMargin) { // disable EC warnings
      user::disableEcWarnings = !user::disableEcWarnings;
      device::settingsAdjusted = true;
      beep();
    }
  }
  if (display::settingsPageTwoScrollPos <= 2) {
    if (touchX >= 700 && touchX <= 770 && touchY >= 258 - scrollMargin && touchY <= 288 - scrollMargin) { // disable PH warnings
      user::disablePhWarnings = !user::disablePhWarnings;
      device::settingsAdjusted = true;
      beep();
    }
    else if (touchX >= 700 && touchX <= 770 && touchY >= 302 - scrollMargin && touchY <= 332 - scrollMargin) { // disable Co2 warnings
      user::disableCo2Warnings = !user::disableCo2Warnings;
      device::settingsAdjusted = true;
      beep();
    }
    else if (touchX >= 700 && touchX <= 770 && touchY >= 346 - scrollMargin && touchY <= 376 - scrollMargin) { // disable water temp warnings
      user::disableWaterTempWarnings = !user::disableWaterTempWarnings;
      device::settingsAdjusted = true;
      beep();
    }
    else if (touchX >= 700 && touchX <= 770 && touchY >= 390 - scrollMargin && touchY <= 420 - scrollMargin) { // disable height warnings
      user::disableWaterHeightWarnings = !user::disableWaterHeightWarnings;
      device::settingsAdjusted = true;
      beep();
    }
  }
  if (display::settingsPageTwoScrollPos >= 1) {
    if (touchX >= 700 && touchX <= 770 && touchY >= 434 - scrollMargin && touchY <= 464 - scrollMargin) { // disable air temp warnings
      user::disableAirTempWarnings = !user::disableAirTempWarnings;
      device::settingsAdjusted = true;
      beep();
    }
  }
  if (display::settingsPageTwoScrollPos >= 2) {
    if (touchX >= 700 && touchX <= 770 && touchY >= 478 - scrollMargin && touchY <= 508 - scrollMargin) { // disable humidity warnings
      user::disableHumidityWarnings = !user::disableHumidityWarnings;
      device::settingsAdjusted = true;
      beep();
    }
  }
}

void settingsThreePageTouched() {
  static unsigned long etapePageOneContinuePressed = 0;
  if (user::showSr04HeightCalibration) {
    if (display::touch_x >= 284 && display::touch_x <= 422 && display::touch_y >= 368 && display::touch_y <= 414) { // cancel setting the water empty height / volume
      beep();
      user::showSr04HeightCalibration = false;
      display::refreshPage = true;
      clearPage();
    }
    else if (display::touch_x >= 460 && display::touch_x <= 638 && display::touch_y >= 368 && display::touch_y <= 414) { // continue with setting the water empty height / volume
      beep();
      setTankDepth();
      user::showSr04HeightCalibration = false;
      display::refreshPage = true;
      clearPage();
    }
  }
  else if (user::showTdsCalibration) {
    if (display::touch_x >= 284 && display::touch_x <= 422 && display::touch_y >= 368 && display::touch_y <= 414) { // cancel Tds Calibration
      beep();
      user::showTdsCalibration = false;
      display::refreshPage = true;
      clearPage();
    }
    else if (display::touch_x >= 460 && display::touch_x <= 638 && display::touch_y >= 368 && display::touch_y <= 414) { // continue with Tds Calibration
      beep();
      tdsCalibration();
      user::showTdsCalibration = false;
      display::refreshPage = true;
      clearPage();
    }
  }
  else if (user::showCo2Calibration) {
    if (display::touch_x >= 284 && display::touch_x <= 422 && display::touch_y >= 368 && display::touch_y <= 414) { // cancel calibrating the Co2 sensor
      beep();
      user::showCo2Calibration = false;
      display::refreshPage = true;
      clearPage();
    }
    else if (display::touch_x >= 460 && display::touch_x <= 638 && display::touch_y >= 368 && display::touch_y <= 414) { // continue with calibrating the Co2 sensor
      beep();
      display::refreshPage = true;
      calibrateCo2();
      user::showCo2Calibration = false;
    }
  }
  else if (user::showPhCalibration) {
    if (display::calPhPageScrollPos == 0) {
      static byte continueTouches = 0;
      if (device::phCalStage == 0) {
        if (display::touch_x >= 340 && display::touch_x <= 540 && display::touch_y >= 410 && display::touch_y <= 460) { // continue with PH 7 calibration
          beep();
          continueTouches = 0;
          device::phCalStage = 1;
          clearPage();
          display::refreshPage = true;
          drawPages();
          setPh7Voltage();
          device::phCalStage = 2;
          clearPage();
          display::refreshPage = true;
          display::currentTouch = millis();
        }
      }
      else if (device::phCalStage == 2) {
        if (display::touch_x >= 340 && display::touch_x <= 540 && display::touch_y >= 410 && display::touch_y <= 460) { // continue with PH 4 calibration
          if (continueTouches == 2) {
            beep();
            device::phCalStage = 3;
            clearPage();
            display::refreshPage = true;
            drawPages();
            setAveragePhVoltage(sensor::ph4Voltage);
            display::currentTouch = millis();
            Serial.print(F("Reading complete, PH 4 voltage = ")); Serial.println(sensor::ph4Voltage, 4);
            phTwoPartCalibration();
            saveSystemEEPROM();
            device::phCalStage = 4;
            clearPage();
            display::refreshPage = true;
            continueTouches = 0;
          }
          continueTouches++;
        }
      }
      else if (device::phCalStage == 4) {
        if (display::touch_x >= 400 && display::touch_x <= 500 && display::touch_y >= 410 && display::touch_y <= 460) { // exit
          if (continueTouches == 2) {
            beep();
            user::showPhCalibration = false;
            display::calPhPageScrollPos = 0;
            display::refreshPage = true;
            clearPage();
          }
          continueTouches++;
        }
      }
    }
    else if (display::calPhPageScrollPos == 1) {
      if (display::touch_x >= 145 && display::touch_x <= 210 && display::touch_y >= 340 && display::touch_y <= 420) { // ph 4 voltage down
        sensor::ph4Voltage = adjustValue(sensor::ph4Voltage, -0.001, 0.01, 5);
        phTwoPartCalibration();
      }
      else if (display::touch_x >= 225 && display::touch_x <= 290 && display::touch_y >= 340 && display::touch_y <= 420) { // ph 4 voltage up
        sensor::ph4Voltage = adjustValue(sensor::ph4Voltage, 0.001, 0.01,  5);
        phTwoPartCalibration();
      }
      else if (display::touch_x >= 350 && display::touch_x <= 415 && display::touch_y >= 340 && display::touch_y <= 420) { // ph 7 voltage down
        sensor::ph7Voltage = adjustValue(sensor::ph7Voltage, -0.001, 0.01, 5);
        phTwoPartCalibration();
      }
      else if (display::touch_x >= 430 && display::touch_x <= 495 && display::touch_y >= 340 && display::touch_y <= 420) { // ph 7 voltage up
        sensor::ph7Voltage = adjustValue(sensor::ph7Voltage, 0.001, 0.01, 5);
        phTwoPartCalibration();
      }
      else if (display::touch_x >= 400 && display::touch_x <= 500 && display::touch_y >= 410 && display::touch_y <= 460) { // exit
        beep();
        user::showPhCalibration = false;
        display::refreshPage = true;
        clearPage();
        saveSystemEEPROM();
      }
    }
    else {
      if (display::touch_x >= 534 && display::touch_x <= 614 && display::touch_y >= 356 && display::touch_y <= 436) { // ph sensor digital pot calibration value down
        sensor::phPotStep = adjustValue(sensor::phPotStep, -1, 0, 256);
        sendToSlave('P', sensor::phPotStep);
      }
      else if (display::touch_x >= 634 && display::touch_x <= 714 && display::touch_y >= 356 && display::touch_y <= 436) { // ph sensor digital pot calibration value up
        sensor::phPotStep = adjustValue(sensor::phPotStep, 1, 0, 256);
        sendToSlave('P', sensor::phPotStep);
      }
      else if (display::touch_x >= 400 && display::touch_x <= 500 && display::touch_y >= 410 && display::touch_y <= 460) { // exit
        beep();
        user::showPhCalibration = false;
        display::refreshPage = true;
        clearPage();
        saveSystemEEPROM();
      }
    }
  }
  else if (user::showDoserCalibration) {
    int startPosition = display::calDoserPageScrollPos * 178;
    int touchX = display::touch_x, touchY = display::touch_y;
    for (byte n = display::calDoserPageScrollPos; n < display::calDoserPageScrollPos + 4; n++) {
      if (n == 0) {
        if (touchX >= 105 - startPosition && touchX <= 180 - startPosition && touchY >= 370 && touchY <= 410) // doser speed 1 down
          user::doserOneSpeed = adjustValue(user::doserOneSpeed, -1, 1, 100);
        else if (touchX >= 190 - startPosition && touchX <= 268 - startPosition && touchY >= 370 && touchY <= 410) // doser speed 1 up
          user::doserOneSpeed = adjustValue(user::doserOneSpeed, 1, 1, 100);
        else if (touchX >= 120 - startPosition && touchX <= 240 - startPosition && touchY >= 310 && touchY <= 350) // doser 1 prime
          prime(1, pin::doserOne, user::doserOneSpeed);
      }
      else if (n == 1) {
        if (touchX >= 278 - startPosition && touchX <= 358 - startPosition && touchY >= 370 && touchY <= 410) // doser speed 2 down
          user::doserTwoSpeed = adjustValue(user::doserTwoSpeed, -1, 1, 100);
        else if (touchX >= 368 - startPosition && touchX <= 448 - startPosition && touchY >= 370 && touchY <= 410) // doser speed 2 up
          user::doserTwoSpeed = adjustValue(user::doserTwoSpeed, 1, 1, 100);
        else if (touchX >= 293 - startPosition && touchX <= 413 - startPosition && touchY >= 310 && touchY <= 350) // doser 2 prime
          prime(2, pin::doserTwo, user::doserTwoSpeed);
      }
      else if (n == 2) {
        if (touchX >= 458 - startPosition && touchX <= 538 - startPosition && touchY >= 370 && touchY <= 410) // doser speed 3 down
          user::doserThreeSpeed = adjustValue(user::doserThreeSpeed, -1, 1, 100);
        else if (touchX >= 548 - startPosition && touchX <= 625 - startPosition && touchY >= 370 && touchY <= 410) // doser speed 3 up
          user::doserThreeSpeed = adjustValue(user::doserThreeSpeed, 1, 1, 100);
        else if (touchX >= 473 - startPosition && touchX <= 593 - startPosition && touchY >= 310 && touchY <= 350) // doser 3 prime
          prime(3, pin::doserThree, user::doserThreeSpeed);
      }
      else if (n == 3) {
        if (touchX >= 635 - startPosition && touchX <= 702 - startPosition && touchY >= 370 && touchY <= 410) // doser speed 4 down
          user::doserFourSpeed = adjustValue(user::doserFourSpeed, -1, 1, 100);
        else if (touchX >= 712 - startPosition && touchX <= 800 - startPosition && touchY >= 370 && touchY <= 410) // doser speed 4 up
          user::doserFourSpeed = adjustValue(user::doserFourSpeed, 1, 1, 100);
        else if (touchX >= 650 - startPosition && touchX <= 770 - startPosition && touchY >= 310 && touchY <= 350) // doser 4 prime
          prime(4, pin::doserFour, user::doserFourSpeed);
      }
      else if (n == 4) {
        if (touchX >= 813 - startPosition && touchX <= 880 - startPosition && touchY >= 370 && touchY <= 410) // doser speed 5 down
          user::doserFiveSpeed = adjustValue(user::doserFiveSpeed, -1, 1, 100);
        else if (touchX >= 890 - startPosition && touchX <= 978 - startPosition && touchY >= 370 && touchY <= 410) // doser speed 5 up
          user::doserFiveSpeed = adjustValue(user::doserFiveSpeed, 1, 1, 100);
        else if (touchX >= 828 - startPosition && touchX <= 948 - startPosition && touchY >= 310 && touchY <= 350) // doser 5 prime
          prime(5, pin::doserFive, user::doserFiveSpeed);
      }
      else if (n == 5) {
        if (touchX >= 991 - startPosition && touchX <= 1058 - startPosition && touchY >= 370 && touchY <= 410) // doser speed 6 down
          user::doserSixSpeed = adjustValue(user::doserSixSpeed, -1, 1, 100);
        else if (touchX >= 1068 - startPosition && touchX <= 1156 - startPosition && touchY >= 370 && touchY <= 410) // doser speed 6 up
          user::doserSixSpeed = adjustValue(user::doserSixSpeed, 1, 1, 100);
        else if (touchX >= 1006 - startPosition && touchX <= 1126 - startPosition && touchY >= 310 && touchY <= 350) // doser 6 prime
          prime(6, pin::doserSix, user::doserSixSpeed);
      }
    }
    if (display::touch_x >= 400 && display::touch_x <= 500 && display::touch_y >= 410 && display::touch_y <= 460) { // exit
      beep();
      user::showDoserCalibration = false;
      display::refreshPage = true;
      clearPage();
      // save profile - speed could vary due to viscosity and volatge so per profile save thus no auto save ?
    }
  }
  else if (user::showEtapeCalibration == device::PAGE_ONE) {
    if (display::touch_x >= 284 && display::touch_x <= 422 && display::touch_y >= 368 && display::touch_y <= 414) { // cancel Etape Calibration
      beep();
      user::showEtapeCalibration = device::UNCHANGED;
      display::refreshPage = true;
      clearPage();
    }
    else if (display::touch_x >= 460 && display::touch_x <= 638 && display::touch_y >= 368 && display::touch_y <= 414) { // continue with Etape min reading Calibration
      beep();
      etapePageOneContinuePressed = millis() + 2000UL; // Stop the page 2 continue button being pressed by mistake after the page 1 is pressed
      setEtapeZeroVolumeResistance();
      user::showEtapeCalibration = device::PAGE_TWO;
      display::refreshPage = true;
    }
  }
  else if (user::showEtapeCalibration == device::PAGE_TWO && millis() >= etapePageOneContinuePressed) {
    if (display::touch_x >= 284 && display::touch_x <= 422 && display::touch_y >= 368 && display::touch_y <= 414) { // cancel Etape Calibration
      beep();
      user::showEtapeCalibration = device::UNCHANGED;
      sensor::temp_etapeZeroVolumeResistance = 0;
      display::refreshPage = true;
      clearPage();
    }
    else if (display::touch_x >= 460 && display::touch_x <= 638 && display::touch_y >= 368 && display::touch_y <= 414) { // continue with Etape max reading Calibration
      beep();
      setEtapeMaxVolumeResistance();
      saveSystemEEPROM();
      user::showEtapeCalibration = device::UNCHANGED;
      display::refreshPage = true;
      clearPage();
    }
  }
  else if (display::touch_x >= 700 && display::touch_x <= 770 && display::touch_y >= 170 && display::touch_y <= 200) { // showSr04HeightCalibration
    beep();
    user::showSr04HeightCalibration = true;
    display::refreshPage = true;
  }
  else if (display::touch_x >= 700 && display::touch_x <= 770 && display::touch_y >= 214 && display::touch_y <= 244) { // set tds sensor calibration
    beep();
    user::showTdsCalibration = true;
    display::refreshPage = true;
  }
  else if (display::touch_x >= 700 && display::touch_x <= 770 && display::touch_y >= 258 && display::touch_y <= 288) { // set co2 sensor calibration
    beep();
    user::showCo2Calibration = true;
    display::refreshPage = true;
  }
  else if (display::touch_x >= 700 && display::touch_x <= 770 && display::touch_y >= 302 && display::touch_y <= 332) { // set ph sensor calibration
    beep();
    device::phCalStage = 0;
    user::showPhCalibration = true;
    display::refreshPage = true;
    clearPage();
  }
  else if (display::touch_x >= 700 && display::touch_x <= 770 && display::touch_y >= 346 && display::touch_y <= 376) { // showDoserCalibration
    beep();
    user::showDoserCalibration = true;
    display::refreshPage = true;
    clearPage();
  }
  else if (display::touch_x >= 700 && display::touch_x <= 770 && display::touch_y >= 390 && display::touch_y <= 420) { // showEtapeCalibration
    beep();
    user::showEtapeCalibration = device::PAGE_ONE;
    display::refreshPage = true;
    clearPage();
  }
}

void ecTdsPageTouched() {
  if (user::convertToTds) {
    if (display::touch_x >= 190 && display::touch_x <= 250 && display::touch_y >= 340 && display::touch_y <= 420) {  // min target ppm down
      user::targetMinTds = adjustValue(user::targetMinTds, -1, 1, user::targetMaxTds - 1);
      device::ecValsAdjusted = true;
    }
    else if (display::touch_x >= 270 && display::touch_x <= 330 && display::touch_y >= 340 && display::touch_y <= 420) {  // min target ppm up
      user::targetMinTds = adjustValue(user::targetMinTds, 1, 1, user::targetMaxTds - 1);
      device::ecValsAdjusted = true;
    }
    else if (display::touch_x >= 530 && display::touch_x <= 590 && display::touch_y >= 340 && display::touch_y <= 420) {  // max target ppm down
      user::targetMaxTds = adjustValue(user::targetMaxTds, -1, user::targetMinTds + 1, 999);
      device::ecValsAdjusted = true;
    }
    else if (display::touch_x >= 610 && display::touch_x <= 670 && display::touch_y >= 340 && display::touch_y <= 420) {  // max target ppm up
      user::targetMaxTds = adjustValue(user::targetMaxTds, 1, user::targetMinTds + 1, 999);
      device::ecValsAdjusted = true;
    }
  }
  else {
    if (display::touch_x >= 190 && display::touch_x <= 250 && display::touch_y >= 340 && display::touch_y <= 420) {  // min target ec down
      user::targetMinEc = adjustValue(user::targetMinEc, -0.01, 0.1, user::targetMaxEc - 0.1);
      device::ecValsAdjusted = true;
    }
    else if (display::touch_x >= 270 && display::touch_x <= 330 && display::touch_y >= 340 && display::touch_y <= 420) {  // min target ec up
      user::targetMinEc = adjustValue(user::targetMinEc, 0.01, 0.1, user::targetMaxEc - 0.1);
      device::ecValsAdjusted = true;
    }
    else if (display::touch_x >= 530 && display::touch_x <= 590 && display::touch_y >= 340 && display::touch_y <= 420) {  // max target ec down
      user::targetMaxEc = adjustValue(user::targetMaxEc, -0.01, user::targetMinEc + 0.1, 6);
      device::ecValsAdjusted = true;
    }
    else if (display::touch_x >= 610 && display::touch_x <= 670 && display::touch_y >= 340 && display::touch_y <= 420) {  // max target ec up
      user::targetMaxEc = adjustValue(user::targetMaxEc, 0.01, user::targetMinEc + 0.1, 6);
      device::ecValsAdjusted = true;
    }
  }
}

void phPageTouched() {
  if (display::touch_x >= 190 && display::touch_x <= 250 && display::touch_y >= 340 && display::touch_y <= 420) { // min target ph down
    user::targetMinPh = adjustValue(user::targetMinPh, -0.01, 1, user::targetMaxPh - 0.1);
    device::phValsAdjusted = true;
  }
  else if (display::touch_x >= 270 && display::touch_x <= 330 && display::touch_y >= 340 && display::touch_y <= 420) { // min target ph up
    user::targetMinPh = adjustValue(user::targetMinPh, 0.01, 1,  user::targetMaxPh - 0.1);
    device::phValsAdjusted = true;
  }
  else if (display::touch_x >= 530 && display::touch_x <= 590 && display::touch_y >= 340 && display::touch_y <= 420) { // max target ph down
    user::targetMaxPh = adjustValue(user::targetMaxPh, -0.01, user::targetMinPh + 0.1, 14);
    device::phValsAdjusted = true;
  }
  else if (display::touch_x >= 310 && display::touch_x <= 670 && display::touch_y >= 340 && display::touch_y <= 420) { // max target ph up
    user::targetMaxPh = adjustValue(user::targetMaxPh, 0.01, user::targetMinPh + 0.1, 14);
    device::phValsAdjusted = true;
  }
}

void co2PageTouched() {
  if (display::co2PageScrollPos == 0) {
    if (display::touch_x >= 190 && display::touch_x <= 250 && display::touch_y >= 350 && display::touch_y <= 420) { // targetCo2 down
      user::targetCo2 = adjustValue(user::targetCo2, -1, 400, 5000);
      device::co2ValsAdjusted = true;
    }
    else if (display::touch_x >= 270 && display::touch_x <= 330 && display::touch_y >= 350 && display::touch_y <= 420) { // targetCo2 up
      user::targetCo2 = adjustValue(user::targetCo2, 1, 400, 5000);
      device::co2ValsAdjusted = true;
    }
    else if (display::touch_x >= 530 && display::touch_x <= 590 && display::touch_y >= 350 && display::touch_y <= 420) { // co2 tolerance down
      user::co2Offset = adjustValue(user::co2Offset, -1, 0, 100);
      device::co2ValsAdjusted = true;
    }
    else if (display::touch_x >= 610 && display::touch_x <= 670 && display::touch_y >= 350 && display::touch_y <= 420) { // co2 tolerance up
      user::co2Offset = adjustValue(user::co2Offset, 1, 0, 100);
      device::co2ValsAdjusted = true;
    }
  }
  else if (display::co2PageScrollPos == 1) {
    if (display::touch_x >= 145 && display::touch_x <= 210 && display::touch_y >= 340 && display::touch_y <= 420) { // roomLength down
      if (user::convertToInches)
        user::roomLengthInches = adjustValue(user::roomLengthInches, -1, 1, 9999);
      else
        user::roomLengthCm = adjustValue(user::roomLengthCm, -1, 1, 9999);
      device::co2ValsAdjusted = true;
    }
    else if (display::touch_x >= 225 && display::touch_x <= 290 && display::touch_y >= 340 && display::touch_y <= 420) { // roomLength up
      if (user::convertToInches)
        user::roomLengthInches = adjustValue(user::roomLengthInches, 1, 1, 9999);
      else
        user::roomLengthCm = adjustValue(user::roomLengthCm, 1, 1, 9999);
      device::co2ValsAdjusted = true;
    }
    else if (display::touch_x >= 350 && display::touch_x <= 415 && display::touch_y >= 340 && display::touch_y <= 420) { // roomWidth down
      if (user::convertToInches)
        user::roomWidthInches = adjustValue(user::roomWidthInches, -1, 1, 9999);
      else
        user::roomWidthCm = adjustValue(user::roomWidthCm, -1, 1, 9999);
      device::co2ValsAdjusted = true;
    }
    else if (display::touch_x >= 430 && display::touch_x <= 495 && display::touch_y >= 340 && display::touch_y <= 420) { // roomWidth up
      if (user::convertToInches)
        user::roomWidthInches = adjustValue(user::roomWidthInches, 1, 1, 9999);
      else
        user::roomWidthCm = adjustValue(user::roomWidthCm, 1, 1, 9999);
      device::co2ValsAdjusted = true;
    }
    else if (display::touch_x >= 560 && display::touch_x <= 625 && display::touch_y >= 340 && display::touch_y <= 420) { // roomHeight down
      if (user::convertToInches)
        user::roomHeightInches = adjustValue(user::roomHeightInches, -1, 1, 9999);
      else
        user::roomHeightCm = adjustValue(user::roomHeightCm, -1, 1, 9999);
      device::co2ValsAdjusted = true;
    }
    else if (display::touch_x >= 640 && display::touch_x <= 705 && display::touch_y >= 340 && display::touch_y <= 420) { // roomHeight up
      if (user::convertToInches)
        user::roomHeightInches = adjustValue(user::roomHeightInches, 1, 1, 9999);
      else
        user::roomHeightCm = adjustValue(user::roomHeightCm, 1, 1, 9999);
      device::co2ValsAdjusted = true;
    }
  }
  else if (display::co2PageScrollPos == 2) {
    if (display::touch_x >= 145 && display::touch_x <= 210 && display::touch_y >= 340 && display::touch_y <= 420) { // co2Flowrate down
      if (user::convertToInches)
        user::co2FlowrateFeet3 = adjustValue(user::co2FlowrateFeet3, -0.1, 0.1, 99);
      else
        user::co2FlowrateLtrs = adjustValue(user::co2FlowrateLtrs, -0.1, 0.1, 99);
      device::co2ValsAdjusted = true;
    }
    else if (display::touch_x >= 225 && display::touch_x <= 290 && display::touch_y >= 340 && display::touch_y <= 420) { // co2Flowrate up
      if (user::convertToInches)
        user::co2FlowrateFeet3 = adjustValue(user::co2FlowrateFeet3, 0.1, 0.1, 99);
      else
        user::co2FlowrateLtrs = adjustValue(user::co2FlowrateLtrs, 0.1, 0.1, 99);
      device::co2ValsAdjusted = true;
    }
    else if (display::touch_x >= 350 && display::touch_x <= 415 && display::touch_y >= 340 && display::touch_y <= 420) { // co2CheckTime down
      if (user::co2CheckTimeMinute >= 15) {
        user::co2CheckTimeMinute -= 15;
      }
      else if (user::co2CheckTimeMinute == 0) {
        user::co2CheckTimeMinute = 45;
        if (user::co2CheckTimeHour > 0)
          user::co2CheckTimeHour--;
        else
          user::co2CheckTimeHour = 23;
      }
      beep();
      device::co2ValsAdjusted = true;
    }
    else if (display::touch_x >= 430 && display::touch_x <= 495 && display::touch_y >= 340 && display::touch_y <= 420) { // co2CheckTime up
      if (user::co2CheckTimeMinute <= 30) {
        user::co2CheckTimeMinute += 15;
      }
      else {
        user::co2CheckTimeMinute = 0;
        if (user::co2CheckTimeHour < 23)
          user::co2CheckTimeHour++;
        else
          user::co2CheckTimeHour = 0;
      }
      beep();
      device::co2ValsAdjusted = true;
    }
    else if (display::touch_x >= 560 && display::touch_x <= 625 && display::touch_y >= 340 && display::touch_y <= 720) { // disableFansTimer down
      user::disableFansTimer = adjustValue(user::disableFansTimer, -1, 0, 999);
      device::co2ValsAdjusted = true;
    }
    else if (display::touch_x >= 640 && display::touch_x <= 705 && display::touch_y >= 340 && display::touch_y <= 720) { // disableFansTimer up
      user::disableFansTimer = adjustValue(user::disableFansTimer, 1, 0, 999);
      device::co2ValsAdjusted = true;
    }
  }
  else {
    if (display::touch_x >= 140 && display::touch_x <= 290 && display::touch_y >= 310 && display::touch_y <= 350) { // Enable Co2 manual duration
      user::enableManualCo2Duration = !user::enableManualCo2Duration;
      device::co2ValsAdjusted = true;
      beep();
    }
    else if (display::touch_x >= 390 && display::touch_x <= 430 && display::touch_y >= 400 && display::touch_y <= 470 && user::enableManualCo2Duration) { // Manual Co2 duration time down
      user::manualCo2GasDuration = adjustValue(user::manualCo2GasDuration, -1, 1, 999);
      device::co2ValsAdjusted = true;
    }
    else if (display::touch_x >= 478 && display::touch_x <= 518 && display::touch_y >= 400 && display::touch_y <= 470 && user::enableManualCo2Duration) { // Manual Co2 duration time up
      user::manualCo2GasDuration = adjustValue(user::manualCo2GasDuration, 1, 1, 999);
      device::co2ValsAdjusted = true;
    }
    else if (display::touch_x >= 585 && display::touch_x <= 735 && display::touch_y >= 310 && display::touch_y <= 350) { // Disable Co2 control
      user::disableCo2Control = !user::disableCo2Control;
      device::co2ValsAdjusted = true;
      beep();
    }
  }
}

void waterPageTouched() {
  if (display::waterPageScrollPos == 0) {
    if (display::touch_x >= 190 && display::touch_x <= 250 && display::touch_y >= 340 && display::touch_y <= 420) { // min target water level down
      if (user::convertToInches)
        user::targetMinWaterHeightInches = adjustValue(user::targetMinWaterHeightInches, -0.1, 1, user::targetMaxWaterHeightInches - 1);
      else
        user::targetMinWaterHeight = adjustValue(user::targetMinWaterHeight, -0.1, 1, user::targetMaxWaterHeight - 1);
      device::waterValsAdjusted = true;
    }
    else if (display::touch_x >= 270 && display::touch_x <= 330 && display::touch_y >= 340 && display::touch_y <= 420) { // min target water level up
      if (user::convertToInches)
        user::targetMinWaterHeightInches = adjustValue(user::targetMinWaterHeightInches, 0.1, 1, user::targetMaxWaterHeightInches - 1);
      else
        user::targetMinWaterHeight = adjustValue(user::targetMinWaterHeight, 0.1, 1, user::targetMaxWaterHeight - 1);
      device::waterValsAdjusted = true;
    }
    else if (display::touch_x >= 530 && display::touch_x <= 590 && display::touch_y >= 340 && display::touch_y <= 420) { // max target water level down
      if (user::convertToInches)
        user::targetMaxWaterHeightInches = adjustValue(user::targetMaxWaterHeightInches, -0.1, user::targetMinWaterHeightInches + 1, 99);
      else
        user::targetMaxWaterHeight = adjustValue(user::targetMaxWaterHeight, -0.1, user::targetMinWaterHeight + 1, 99);
      device::waterValsAdjusted = true;
    }
    else if (display::touch_x >= 610 && display::touch_x <= 670 && display::touch_y >= 340 && display::touch_y <= 420) { // max target water level up
      if (user::convertToInches)
        user::targetMaxWaterHeightInches = adjustValue(user::targetMaxWaterHeightInches, 0.1, user::targetMinWaterHeightInches + 1, 99);
      else
        user::targetMaxWaterHeight = adjustValue(user::targetMaxWaterHeight, 0.1, user::targetMinWaterHeight + 1, 99);
      device::waterValsAdjusted = true;
    }
  }
  else if (display::waterPageScrollPos == 1) {
    if (display::touch_x >= 190 && display::touch_x <= 250 && display::touch_y >= 340 && display::touch_y <= 420) { // min target temperature down
      if (user::convertToF)
        user::targetMinWaterTempF = adjustValue(user::targetMinWaterTempF, -0.1, 1, user::targetMaxWaterTempF - 0.1);
      else
        user::targetMinWaterTemp = adjustValue(user::targetMinWaterTemp, -0.1, 1, user::targetMaxWaterTemp - 0.1);
      device::waterValsAdjusted = true;
    }
    else if (display::touch_x >= 270 && display::touch_x <= 330 && display::touch_y >= 340 && display::touch_y <= 420) { // min target temperature up
      if (user::convertToF)
        user::targetMinWaterTempF = adjustValue(user::targetMinWaterTempF, 0.1, 1, user::targetMaxWaterTempF - 0.1);
      else
        user::targetMinWaterTemp = adjustValue(user::targetMinWaterTemp, 0.1, 1, user::targetMaxWaterTemp - 0.1);
      device::waterValsAdjusted = true;
    }
    else if (display::touch_x >= 530 && display::touch_x <= 590 && display::touch_y >= 340 && display::touch_y <= 420) { // max target temperature down
      if (user::convertToF)
        user::targetMaxWaterTempF = adjustValue(user::targetMaxWaterTempF, -0.1, user::targetMinWaterTempF + 0.1, 99);
      else
        user::targetMaxWaterTemp = adjustValue(user::targetMaxWaterTemp, -0.1, user::targetMinWaterTemp + 0.1, 99);
      device::waterValsAdjusted = true;
    }
    else if (display::touch_x >= 610 && display::touch_x <= 670 && display::touch_y >= 340 && display::touch_y <= 420) { // max target temperature up
      if (user::convertToF)
        user::targetMaxWaterTempF = adjustValue(user::targetMaxWaterTempF, 0.1, user::targetMinWaterTempF + 0.1, 199);
      else
        user::targetMaxWaterTemp = adjustValue(user::targetMaxWaterTemp, 0.1, user::targetMinWaterTemp + 0.1, 99);
      device::waterValsAdjusted = true;
    }
  }
  else if (display::waterPageScrollPos == 2) {
    uint16_t calanderX = 110;
    uint16_t calanderY = 205;
    for (byte i = 1; i < 32; i++) {
      if (display::touch_x >= calanderX && display::touch_x <= calanderX + 54 && display::touch_y >= calanderY && display::touch_y <= calanderY + 40) {
        static unsigned long previousCalanderMillis;
        if (millis() - previousCalanderMillis >= 1000UL) {
          user::autoFillDays[i - 1] = !user::autoFillDays[i - 1];
          Serial.print(F("calander ")); Serial.print(i - 1); Serial.print(F(" = ")); Serial.println(user::autoFillDays[i - 1]);
          beep();
          device::waterValsAdjusted = true;
          display::refreshCalander = true;
          previousCalanderMillis = millis();
        }
        break;
      }
      calanderX += 54;
      if (i % 7 == 0) {
        calanderX = 110;
        calanderY += 40;
      }
    }
    if (display::touch_x >= 380 && display::touch_x <= 420 && display::touch_y >= 410 && display::touch_y <= 460) { // AutoFill time down
      if (user::autoFillMinute >= 15) {
        user::autoFillMinute -= 15;
      }
      else if (user::autoFillMinute == 0) {
        user::autoFillMinute = 45;
        if (user::autoFillHour > 0)
          user::autoFillHour--;
        else
          user::autoFillHour = 23;
      }
      device::waterValsAdjusted = true;
      beep();
    }
    else if (display::touch_x >= 450 && display::touch_x <= 490 && display::touch_y >= 410 && display::touch_y <= 460) { // AutoFill time up
      if (user::autoFillMinute <= 30) {
        user::autoFillMinute += 15;
      }
      else {
        user::autoFillMinute = 0;
        if (user::autoFillHour < 23)
          user::autoFillHour++;
        else
          user::autoFillHour = 0;
      }
      device::waterValsAdjusted = true;
      beep();
    }
    else if (display::touch_x >= 565 && display::touch_x <= 720 && display::touch_y >= 320 && display::touch_y <= 364) { // disableDrainAndRefill
      user::disableDrainAndRefill = !user::disableDrainAndRefill;
      device::waterValsAdjusted = true;
      beep();
    }
  }
  else if (display::waterPageScrollPos == 3) {
    int startPosition = display::refillDoserPageScrollPos * 178;
    int touchX = display::touch_x, touchY = display::touch_y;
    for (byte n = display::refillDoserPageScrollPos; n < display::refillDoserPageScrollPos + 4; n++) {
      if (n == 0) {
        if (touchX >= (105 - startPosition) && touchX <= (180 - startPosition) && touchY >= 420 && touchY <= 480) { // refill doser 1 down
          user::refillDoserOneMills = adjustValue(user::refillDoserOneMills, -1, 0, 999);
          device::waterValsAdjusted = true;
        }
        else if (touchX >= (190 - startPosition) && touchX <= (268 - startPosition) && touchY >= 420 && touchY <= 480) { // refill doser 1 up
          user::refillDoserOneMills = adjustValue(user::refillDoserOneMills, 1, 0, 999);
          device::waterValsAdjusted = true;
        }
      }
      else if (n == 1) {
        if (touchX >= (278 - startPosition) && touchX <= (358 - startPosition) && touchY >= 420 && touchY <= 480) { // refill doser 2 down
          user::refillDoserTwoMills = adjustValue(user::refillDoserTwoMills, -1, 0, 999);
          device::waterValsAdjusted = true;
        }
        else if (touchX >= (368 - startPosition) && touchX <= (448 - startPosition) && touchY >= 420 && touchY <= 480) { // refill doser 2 up
          user::refillDoserTwoMills = adjustValue(user::refillDoserTwoMills, 1, 0, 999);
          device::waterValsAdjusted = true;
        }
      }
      else if (n == 2) {
        if (touchX >= (458 - startPosition) && touchX <= (538 - startPosition) && touchY >= 420 && touchY <= 480) { // refill doser 3 down
          user::refillDoserThreeMills = adjustValue(user::refillDoserThreeMills, -1, 0, 999);
          device::waterValsAdjusted = true;
        }
        else if (touchX >= (548 - startPosition) && touchX <= (625 - startPosition) && touchY >= 420 && touchY <= 480) { // refill doser 3 up
          user::refillDoserThreeMills = adjustValue(user::refillDoserThreeMills, 1, 0, 999);
          device::waterValsAdjusted = true;
        }
      }
      else if (n == 3) {
        if (touchX >= (635 - startPosition) && touchX <= (702 - startPosition) && touchY >= 420 && touchY <= 480) { // refill doser 4 down
          user::refillDoserFourMills = adjustValue(user::refillDoserFourMills, -1, 0, 999);
          device::waterValsAdjusted = true;
        }
        else if (touchX >= (712 - startPosition) && touchX <= (800 - startPosition) && touchY >= 420 && touchY <= 480) { // refill doser 4 up
          user::refillDoserFourMills = adjustValue(user::refillDoserFourMills, 1, 0, 999);
          device::waterValsAdjusted = true;
        }
      }
      else if (n == 4) {
        if (touchX >= (813 - startPosition) && touchX <= (880 - startPosition) && touchY >= 420 && touchY <= 480) { // refill doser 5 down
          user::refillDoserFiveMills = adjustValue(user::refillDoserFiveMills, -1, 0, 999);
          device::waterValsAdjusted = true;
        }
        else if (touchX >= (890 - startPosition) && touchX <= (978 - startPosition) && touchY >= 420 && touchY <= 480) { // refill doser 5 up
          user::refillDoserFiveMills = adjustValue(user::refillDoserFiveMills, 1, 0, 999);
          device::waterValsAdjusted = true;
        }
      }
      else if (n == 5) {
        if (touchX >= (991 - startPosition) && touchX <= (1058 - startPosition) && touchY >= 420 && touchY <= 480) { // refill doser 6 down
          user::refillDoserSixMills = adjustValue(user::refillDoserSixMills, -1, 0, 999);
          device::waterValsAdjusted = true;
        }
        else if (touchX >= (1068 - startPosition) && touchX <= (1156 - startPosition) && touchY >= 420 && touchY <= 480) { // refill doser 6 up
          user::refillDoserSixMills = adjustValue(user::refillDoserSixMills, 1, 0, 999);
          device::waterValsAdjusted = true;
        }
      }
    }
  }
}

void dosersPageTouched() {
  int startPosition = display::doserPageScrollPos * 178;
  int touchX = display::touch_x, touchY = display::touch_y;
  for (byte n = display::doserPageScrollPos; n < display::doserPageScrollPos + 4; n++) {
    if (n == 0) {
      if (touchX >= (105 - startPosition) && touchX <= (180 - startPosition) && touchY >= 310 && touchY <= 410) { // doser 1 down
        user::doserOneMills = adjustValue(user::doserOneMills, -1, 0, 999);
        device::doserValsAdjusted = true;
      }
      else if (touchX >= (190 - startPosition) && touchX <= (268 - startPosition) && touchY >= 310 && touchY <= 410) { // doser 1 up
        user::doserOneMills = adjustValue(user::doserOneMills, 1, 0, 999);
        device::doserValsAdjusted = true;
      }
      else if (touchX >= (130 - startPosition) && touchX <= (250 - startPosition) && touchY >= 420 && touchY <= 470) { // doser 1 prime
        prime(1, pin::doserOne, user::doserOneSpeed);
      }
      else if (touchX >= (155 - startPosition) && touchX <= (235 - startPosition) && touchY >= 230 && touchY <= 270) { // change doser 1 mode - OFF or EC or PH
        beep();
        if (user::doserOneMode == device::DOSER_OFF)
          user::doserOneMode = device::DOSER_EC;
        else if (user::doserOneMode == device::DOSER_EC)
          user::doserOneMode = device::DOSER_PH;
        else if (user::doserOneMode == device::DOSER_PH)
          user::doserOneMode = device::DOSER_OFF;
        device::doserValsAdjusted = true;
      }
    }
    else if (n == 1) {
      if (touchX >= (278 - startPosition) && touchX <= (358 - startPosition) && touchY >= 310 && touchY <= 410) { // doser 2 down
        user::doserTwoMills = adjustValue(user::doserTwoMills, -1, 0, 999);
        device::doserValsAdjusted = true;
      }
      else if (touchX >= (368 - startPosition) && touchX <= (448 - startPosition) && touchY >= 310 && touchY <= 410) { // doser 2 up
        user::doserTwoMills = adjustValue(user::doserTwoMills, 1, 0, 999);
        device::doserValsAdjusted = true;
      }
      else if (touchX >= (303 - startPosition) && touchX <= (423 - startPosition) && touchY >= 420 && touchY <= 470) { // doser 2 prime
        prime(2, pin::doserTwo, user::doserTwoSpeed);
      }
      else if (touchX >= (335 - startPosition) && touchX <= (415 - startPosition) && touchY >= 230 && touchY <= 270) { // change doser 2 mode - OFF or EC or PH
        beep();
        if (user::doserTwoMode == device::DOSER_OFF)
          user::doserTwoMode = device::DOSER_EC;
        else if (user::doserTwoMode == device::DOSER_EC)
          user::doserTwoMode = device::DOSER_PH;
        else if (user::doserTwoMode == device::DOSER_PH)
          user::doserTwoMode = device::DOSER_OFF;
        device::doserValsAdjusted = true;
      }
    }
    else if (n == 2) {
      if (touchX >= (458 - startPosition) && touchX <= (538 - startPosition) && touchY >= 310 && touchY <= 410) { // doser 3 down
        user::doserThreeMills = adjustValue(user::doserThreeMills, -1, 0, 999);
        device::doserValsAdjusted = true;
      }
      else if (touchX >= (548 - startPosition) && touchX <= (625 - startPosition) && touchY >= 310 && touchY <= 410) { // doser 3 up
        user::doserThreeMills = adjustValue(user::doserThreeMills, 1, 0, 999);
        device::doserValsAdjusted = true;
      }
      else if (touchX >= (483 - startPosition) && touchX <= (603 - startPosition) && touchY >= 420 && touchY <= 470) { // doser 3 prime
        prime(3, pin::doserThree, user::doserThreeSpeed);
      }
      else if (touchX >= (500 - startPosition) && touchX <= (580 - startPosition) && touchY >= 230 && touchY <= 270) { // change doser 3 mode - OFF or EC or PH
        beep();
        if (user::doserThreeMode == device::DOSER_OFF)
          user::doserThreeMode = device::DOSER_EC;
        else if (user::doserThreeMode == device::DOSER_EC)
          user::doserThreeMode = device::DOSER_PH;
        else if (user::doserThreeMode == device::DOSER_PH)
          user::doserThreeMode = device::DOSER_OFF;
        device::doserValsAdjusted = true;
      }
    }
    else if (n == 3) {
      if (touchX >= (635 - startPosition) && touchX <= (702 - startPosition) && touchY >= 310 && touchY <= 410) { // doser 4 down
        user::doserFourMills = adjustValue(user::doserFourMills, -1, 0, 999);
        device::doserValsAdjusted = true;
      }
      else if (touchX >= (712 - startPosition) && touchX <= (800 - startPosition) && touchY >= 310 && touchY <= 410) { // doser 4 up
        user::doserFourMills = adjustValue(user::doserFourMills, 1, 0, 999);
        device::doserValsAdjusted = true;
      }
      else if (touchX >= (660 - startPosition) && touchX <= (780 - startPosition) && touchY >= 420 && touchY <= 470) { // doser 4 prime
        prime(4, pin::doserFour, user::doserFourSpeed);
      }
      else if (touchX >= (678 - startPosition) && touchX <= (758 - startPosition) && touchY >= 230 && touchY <= 270) { // change doser 4 mode - OFF or EC or PH
        beep();
        if (user::doserFourMode == device::DOSER_OFF)
          user::doserFourMode = device::DOSER_EC;
        else if (user::doserFourMode == device::DOSER_EC)
          user::doserFourMode = device::DOSER_PH;
        else if (user::doserFourMode == device::DOSER_PH)
          user::doserFourMode = device::DOSER_OFF;
        device::doserValsAdjusted = true;
      }
    }
    else if (n == 4) {
      if (touchX >= (813 - startPosition) && touchX <= (880 - startPosition) && touchY >= 310 && touchY <= 410) { // doser 5 down
        user::doserFiveMills = adjustValue(user::doserFiveMills, -1, 0, 999);
        device::doserValsAdjusted = true;
      }
      else if (touchX >= (890 - startPosition) && touchX <= (978 - startPosition) && touchY >= 310 && touchY <= 410) { // doser 5 up
        user::doserFiveMills = adjustValue(user::doserFiveMills, 1, 0, 999);
        device::doserValsAdjusted = true;
      }
      else if (touchX >= (838 - startPosition) && touchX <= (958 - startPosition) && touchY >= 420 && touchY <= 470) { // doser 5 prime
        prime(5, pin::doserFive, user::doserFiveSpeed);
      }
      else if (touchX >= (856 - startPosition) && touchX <= (936 - startPosition) && touchY >= 230 && touchY <= 270) { // change doser 5 mode - OFF or EC or PH
        beep();
        if (user::doserFiveMode == device::DOSER_OFF)
          user::doserFiveMode = device::DOSER_EC;
        else if (user::doserFiveMode == device::DOSER_EC)
          user::doserFiveMode = device::DOSER_PH;
        else if (user::doserFiveMode == device::DOSER_PH)
          user::doserFiveMode = device::DOSER_OFF;
        device::doserValsAdjusted = true;
      }
    }
    else if (n == 5) {
      if (touchX >= (991 - startPosition) && touchX <= (1058 - startPosition) && touchY >= 310 && touchY <= 410) { // doser 6 down
        user::doserSixMills = adjustValue(user::doserSixMills, -1, 0, 999);
        device::doserValsAdjusted = true;
      }
      else if (touchX >= (1068 - startPosition) && touchX <= (1156 - startPosition) && touchY >= 310 && touchY <= 410) { // doser 6 up
        user::doserSixMills = adjustValue(user::doserSixMills, 1, 0, 999);
        device::doserValsAdjusted = true;
      }
      else if (touchX >= (1016 - startPosition) && touchX <= (1136 - startPosition) && touchY >= 420 && touchY <= 470) { // doser 6 prime
        prime(4, pin::doserSix, user::doserSixSpeed);
      }
      else if (touchX >= (1034 - startPosition) && touchX <= (1114 - startPosition) && touchY >= 230 && touchY <= 270) { // change doser 6 mode - OFF or EC or PH
        beep();
        if (user::doserSixMode == device::DOSER_OFF)
          user::doserSixMode = device::DOSER_EC;
        else if (user::doserSixMode == device::DOSER_EC)
          user::doserSixMode = device::DOSER_PH;
        else if (user::doserSixMode == device::DOSER_PH)
          user::doserSixMode = device::DOSER_OFF;
        device::doserValsAdjusted = true;
      }
    }
  }
}

void lightingPageTouched() {
  if (display::touch_x >= 115 && display::touch_x <= 185 && display::touch_y >= 330 && display::touch_y <= 400) { // on time down
    beep();
    if (user::lightOnTimeMin >= 15) {
      user::lightOnTimeMin -= 15;
    }
    else if (user::lightOnTimeMin == 0) {
      user::lightOnTimeMin = 45;
      if (user::lightOnTimeHour > 0)
        user::lightOnTimeHour--;
      else
        user::lightOnTimeHour = 23;
    }
    device::lightValsAdjusted = true;
  }
  else if (display::touch_x >= 195 && display::touch_x <= 265 && display::touch_y >= 330 && display::touch_y <= 400) { // on time up
    beep();
    if (user::lightOnTimeMin <= 30) {
      user::lightOnTimeMin += 15;
    }
    else {
      user::lightOnTimeMin = 0;
      if (user::lightOnTimeHour < 23)
        user::lightOnTimeHour++;
      else
        user::lightOnTimeHour = 0;
    }
    device::lightValsAdjusted = true;
  }
  else if (display::touch_x >= 325 && display::touch_x <= 395 && display::touch_y >= 330 && display::touch_y <= 400) { // off time down
    beep();
    if (user::lightOffTimeMin >= 15) {
      user::lightOffTimeMin -= 15;
    }
    else if (user::lightOffTimeMin == 0) {
      user::lightOffTimeMin = 45;
      if (user::lightOffTimeHour > 0)
        user::lightOffTimeHour--;
      else
        user::lightOffTimeHour = 23;
    }
    device::lightValsAdjusted = true;
  }
  else if (display::touch_x >= 410 && display::touch_x <= 480 && display::touch_y >= 330 && display::touch_y <= 400) { // off time up
    beep();
    if (user::lightOffTimeMin <= 30) {
      user::lightOffTimeMin += 15;
    }
    else {
      user::lightOffTimeMin = 0;
      if (user::lightOffTimeHour < 23)
        user::lightOffTimeHour++;
      else
        user::lightOffTimeHour = 0;
    }
    device::lightValsAdjusted = true;
  }
  else if (display::touch_x >= 570 && display::touch_x <= 629 && display::touch_y >= 230 && display::touch_y <= 270) { // light mode On
    beep();
    user::lightMode = 1;
    device::lightValsAdjusted = true;
  }
  else if (display::touch_x >= 630 && display::touch_x <= 689 && display::touch_y >= 230 && display::touch_y <= 270) { // light mode Auto
    beep();
    user::lightMode = 0;
    if (restartLightingTimer()) {
      Serial.println(F("Light auto on"));
      digitalWrite(pin::light, !device::relayOffState);
      device::lightOn = true;
      rtcTime = rtc.getTime();
      device::lightSwitchedOnHour = rtcTime.hour;
      device::lightSwitchedOnMin = rtcTime.min;
    }
    else {
      Serial.println(F("Light auto off"));
      digitalWrite(pin::light, device::relayOffState);
      device::lightOn = false;
    }
    device::lightValsAdjusted = true;
  }
  else if (display::touch_x >= 690 && display::touch_x <= 750 && display::touch_y >= 230 && display::touch_y <= 270) { // light mode Off
    beep();
    user::lightMode = 2;
    device::lightValsAdjusted = true;
  }
}

void fansPageTouched() {
  if (display::fansPage == 0) {
    if (user::fanOneFixedSpeed) {
      // fan one min speed down AND fan one max speed down
      if ((display::touch_x >= 190 && display::touch_x <= 250 && display::touch_y >= 350 && display::touch_y <= 420) || (display::touch_x >= 530 && display::touch_x <= 590 && display::touch_y >= 350 && display::touch_y <= 420))  {
        user::targetMinFanOneSpeed = adjustValue(user::targetMinFanOneSpeed, -1, 0, 100);
        user::targetMaxFanOneSpeed = adjustValue(user::targetMaxFanOneSpeed, -1, 0, 100);
        device::fanValsAdjusted = true;
        sendToSlave('Z', user::targetMinFanOneSpeed);
      }
      // fan one min speed up AND fan one max speed up if either is presed
      else if ((display::touch_x >= 270 && display::touch_x <= 330 && display::touch_y >= 350 && display::touch_y <= 420) || (display::touch_x >= 610 && display::touch_x <= 670 && display::touch_y >= 350 && display::touch_y <= 420)) {
        user::targetMinFanOneSpeed = adjustValue(user::targetMinFanOneSpeed, 1, 0, 100);
        user::targetMaxFanOneSpeed = adjustValue(user::targetMaxFanOneSpeed, 1, 0, 100);
        device::fanValsAdjusted = true;
        sendToSlave('Z', user::targetMinFanOneSpeed);
      }
    }
    else {
      if (display::touch_x >= 190 && display::touch_x <= 250 && display::touch_y >= 350 && display::touch_y <= 420) { // fan one min speed down
        user::targetMinFanOneSpeed = adjustValue(user::targetMinFanOneSpeed, -1, 0, 100);
        device::fanValsAdjusted = true;
      }
      else if (display::touch_x >= 270 && display::touch_x <= 330 && display::touch_y >= 350 && display::touch_y <= 420) { // fan one min speed up
        user::targetMinFanOneSpeed = adjustValue(user::targetMinFanOneSpeed, 1, 0, user::targetMaxFanOneSpeed - 1);
        device::fanValsAdjusted = true;
      }
      else if (display::touch_x >= 530 && display::touch_x <= 590 && display::touch_y >= 350 && display::touch_y <= 420) { // fan one max speed down
        user::targetMaxFanOneSpeed = adjustValue(user::targetMaxFanOneSpeed, -1, user::targetMinFanOneSpeed + 1, 100);
        device::fanValsAdjusted = true;
      }
      else if (display::touch_x >= 610 && display::touch_x <= 670 && display::touch_y >= 350 && display::touch_y <= 420) { // fan one max speed up
        user::targetMaxFanOneSpeed = adjustValue(user::targetMaxFanOneSpeed, 1, 0, 100);
        device::fanValsAdjusted = true;
      }
    }
  }
  else if (display::fansPage == 1) {
    if (user::fanTwoFixedSpeed) {
      // fan two min speed down AND fan two max speed down if either is presed
      if ((display::touch_x >= 190 && display::touch_x <= 250 && display::touch_y >= 350 && display::touch_y <= 420) || (display::touch_x >= 530 && display::touch_x <= 590 && display::touch_y >= 350 && display::touch_y <= 420))  {
        user::targetMinFanTwoSpeed = adjustValue(user::targetMinFanTwoSpeed, -1, 0, 100);
        user::targetMaxFanTwoSpeed = adjustValue(user::targetMaxFanTwoSpeed, -1, 0, 100);
        device::fanValsAdjusted = true;
        sendToSlave('X', user::targetMinFanTwoSpeed);
      }
      // fan two min speed up AND fan two max speed up if either is presed
      else if ((display::touch_x >= 270 && display::touch_x <= 330 && display::touch_y >= 350 && display::touch_y <= 420) || (display::touch_x >= 610 && display::touch_x <= 670 && display::touch_y >= 350 && display::touch_y <= 420)) {
        user::targetMinFanTwoSpeed = adjustValue(user::targetMinFanTwoSpeed, 1, 0, 100);
        user::targetMaxFanTwoSpeed = adjustValue(user::targetMaxFanTwoSpeed, 1, 0, 100);
        device::fanValsAdjusted = true;
        sendToSlave('X', user::targetMinFanTwoSpeed);
      }
    }
    else {
      if (display::touch_x >= 190 && display::touch_x <= 250 && display::touch_y >= 350 && display::touch_y <= 420) { // fan two min speed down
        user::targetMinFanTwoSpeed = adjustValue(user::targetMinFanTwoSpeed, -1, 0, 100);
        device::fanValsAdjusted = true;
      }
      else if (display::touch_x >= 270 && display::touch_x <= 330 && display::touch_y >= 350 && display::touch_y <= 420) { // fan two min speed up
        user::targetMinFanTwoSpeed = adjustValue(user::targetMinFanTwoSpeed, 1, 0, user::targetMaxFanTwoSpeed - 1);
        device::fanValsAdjusted = true;
      }
      else if (display::touch_x >= 530 && display::touch_x <= 590 && display::touch_y >= 350 && display::touch_y <= 420) { // fan two max speed down
        user::targetMaxFanTwoSpeed = adjustValue(user::targetMaxFanTwoSpeed, -1, user::targetMinFanTwoSpeed + 1, 100);
        device::fanValsAdjusted = true;
      }
      else if (display::touch_x >= 610 && display::touch_x <= 670 && display::touch_y >= 350 && display::touch_y <= 420) { // fan two max speed up
        user::targetMaxFanTwoSpeed = adjustValue(user::targetMaxFanTwoSpeed, 1, 0, 100);
        device::fanValsAdjusted = true;
      }
    }
  }
  else if (display::fansPage == 2) {
    if (display::touch_x >= 190 && display::touch_x <= 250 && display::touch_y >= 340 && display::touch_y <= 420) { // min target air temp down
      if (user::convertToF)
        user::targetMinAirTempF = adjustValue(user::targetMinAirTempF, -0.1, 1, 99);
      else
        user::targetMinAirTemp = adjustValue(user::targetMinAirTemp, -0.1, 1, 99);
      device::fanValsAdjusted = true;
    }
    else if (display::touch_x >= 270 && display::touch_x <= 330 && display::touch_y >= 340 && display::touch_y <= 420) { // min target air temp up
      if (user::convertToF)
        user::targetMinAirTempF = adjustValue(user::targetMinAirTempF, 0.1, 1, user::targetMaxAirTempF - 1);
      else
        user::targetMinAirTemp = adjustValue(user::targetMinAirTemp, 0.1, 1, user::targetMaxAirTemp - 1);
      device::fanValsAdjusted = true;
    }
    else if (display::touch_x >= 530 && display::touch_x <= 590 && display::touch_y >= 340 && display::touch_y <= 420) { // max target air temp down
      if (user::convertToF)
        user::targetMaxAirTempF = adjustValue(user::targetMaxAirTempF, -0.1, user::targetMinAirTempF + 1, 99);
      else
        user::targetMaxAirTemp = adjustValue(user::targetMaxAirTemp, -0.1, user::targetMinAirTemp + 1, 99);
      device::fanValsAdjusted = true;
    }
    else if (display::touch_x >= 610 && display::touch_x <= 670 && display::touch_y >= 340 && display::touch_y <= 420) { // max target air temp up
      if (user::convertToF)
        user::targetMaxAirTempF = adjustValue(user::targetMaxAirTempF, 0.1, 0, 199);
      else
        user::targetMaxAirTemp = adjustValue(user::targetMaxAirTemp, 0.1, 0, 99);
      device::fanValsAdjusted = true;
    }
  }
  else if (display::fansPage == 3) {
    if (display::touch_x >= 190 && display::touch_x <= 250 && display::touch_y >= 340 && display::touch_y <= 420) { // min target humidity down
      user::targetMinHumidity = adjustValue(user::targetMinHumidity, -0.1, 1, 100);
      device::fanValsAdjusted = true;
    }
    else if (display::touch_x >= 270 && display::touch_x <= 330 && display::touch_y >= 340 && display::touch_y <= 420) { // min target humidity up
      user::targetMinHumidity = adjustValue(user::targetMinHumidity, 0.1, 1, user::targetMaxHumidity - 1);
      device::fanValsAdjusted = true;
    }
    else if (display::touch_x >= 530 && display::touch_x <= 590 && display::touch_y >= 340 && display::touch_y <= 420) { // max target humidity down
      user::targetMaxHumidity = adjustValue(user::targetMaxHumidity, -0.1, user::targetMinHumidity + 1, 100);
      device::fanValsAdjusted = true;
    }
    else if (display::touch_x >= 610 && display::touch_x <= 670 && display::touch_y >= 340 && display::touch_y <= 420) { // max target humidity up
      user::targetMaxHumidity = adjustValue(user::targetMaxHumidity, 0.1, 1, 100);
      device::fanValsAdjusted = true;
    }
  }
  else if (display::fansPage == 4) {
    if (display::touch_x >= 640 && display::touch_x <= 790 && display::touch_y >= 180 && display::touch_y <= 220) { // disable/enable fansControlTemperature
      beep();
      device::fanValsAdjusted = true;
      user::fansControlTemperature = !user::fansControlTemperature;
    }
    else if (display::touch_x >= 640 && display::touch_x <= 790 && display::touch_y >= 230 && display::touch_y <= 270) { // disable/enable fansControlHumidity
      beep();
      device::fanValsAdjusted = true;
      user::fansControlHumidity = !user::fansControlHumidity;
    }
    else if (display::touch_x >= 640 && display::touch_x <= 790 && display::touch_y >= 280 && display::touch_y <= 320) { // disable/enable fanOneFixedSpeed
      beep();
      user::fanOneFixedSpeed = !user::fanOneFixedSpeed;
      if (user::fanOneFixedSpeed)
        user::targetMinFanOneSpeed = user::targetMaxFanOneSpeed;
      device::fanValsAdjusted = true;
    }
    else if (display::touch_x >= 640 && display::touch_x <= 790 && display::touch_y >= 330 && display::touch_y <= 370) { // disable/enable fanTwoFixedSpeed
      beep();
      user::fanTwoFixedSpeed = !user::fanTwoFixedSpeed;
      if (user::fanTwoFixedSpeed)
        user::targetMinFanTwoSpeed = user::targetMaxFanTwoSpeed;
      device::fanValsAdjusted = true;
    }
  }
}

void warningsPageTouched() {
  if (display::warningsPage == 0) {
    if (display::touch_x >= 534 && display::touch_x <= 614 && display::touch_y >= 356 && display::touch_y <= 436) { // tds warning down
      user::tdsErrorMargin = adjustValue(user::tdsErrorMargin, -1, 0, 99);
      device::warningValsAdjusted = true;
    }
    else if (display::touch_x >= 634 && display::touch_x <= 714 && display::touch_y >= 356 && display::touch_y <= 436) { // tds warning up
      user::tdsErrorMargin = adjustValue(user::tdsErrorMargin, 1, 0, 99);
      device::warningValsAdjusted = true;
    }
  }
  else if (display::warningsPage == 1) {
    if (display::touch_x >= 534 && display::touch_x <= 614 && display::touch_y >= 356 && display::touch_y <= 436) { // ec warning down
      user::ecErrorMargin = adjustValue(user::ecErrorMargin, -0.1, 0, 6);
      device::warningValsAdjusted = true;
    }
    else if (display::touch_x >= 634 && display::touch_x <= 714 && display::touch_y >= 356 && display::touch_y <= 436) { // ec warning up
      user::ecErrorMargin = adjustValue(user::ecErrorMargin, 0.1, 0, 6);
      device::warningValsAdjusted = true;
    }
  }
  else if (display::warningsPage == 2) {
    if (display::touch_x >= 534 && display::touch_x <= 614 && display::touch_y >= 356 && display::touch_y <= 436) { // ph warning down
      user::phErrorMargin = adjustValue(user::phErrorMargin, -0.1, 0, 14);
      device::warningValsAdjusted = true;
    }
    else if (display::touch_x >= 634 && display::touch_x <= 714 && display::touch_y >= 356 && display::touch_y <= 436) { // ph warning up
      user::phErrorMargin = adjustValue(user::phErrorMargin, 0.1, 0, 14);
      device::warningValsAdjusted = true;
    }
  }
  else if (display::warningsPage == 3) {
    if (display::touch_x >= 534 && display::touch_x <= 614 && display::touch_y >= 356 && display::touch_y <= 436) { // co2 warning down
      user::co2ErrorMargin = adjustValue(user::co2ErrorMargin, -1, 0, 99);
      device::warningValsAdjusted = true;
    }
    else if (display::touch_x >= 634 && display::touch_x <= 714 && display::touch_y >= 356 && display::touch_y <= 436) { // co2 warning up
      user::co2ErrorMargin = adjustValue(user::co2ErrorMargin, 1, 0, 99);
      device::warningValsAdjusted = true;
    }
  }
  else if (display::warningsPage == 4) {
    if (display::touch_x >= 534 && display::touch_x <= 614 && display::touch_y >= 356 && display::touch_y <= 436) { // water temp warning ErrorMargin down
      if (user::convertToF)
        user::waterTempErrorMarginF = adjustValue(user::waterTempErrorMarginF, -0.1, 0, 99);
      else
        user::waterTempErrorMargin = adjustValue(user::waterTempErrorMargin, -0.1, 0, 99);
      device::warningValsAdjusted = true;
    }
    else if (display::touch_x >= 634 && display::touch_x <= 714 && display::touch_y >= 356 && display::touch_y <= 436) { // water temp warning ErrorMargin up
      if (user::convertToF)
        user::waterTempErrorMarginF = adjustValue(user::waterTempErrorMarginF, 0.1, 0, 99);
      else
        user::waterTempErrorMargin = adjustValue(user::waterTempErrorMargin, 0.1, 0, 99);
      device::warningValsAdjusted = true;
    }
  }
  else if (display::warningsPage == 5) {
    if (display::touch_x >= 534 && display::touch_x <= 614 && display::touch_y >= 356 && display::touch_y <= 436) { // water lvl warning down
      if (user::convertToInches)
        user::waterHeightErrorMarginInches = adjustValue(user::waterHeightErrorMarginInches, -0.1, 0, 99);
      else
        user::waterHeightErrorMargin = adjustValue(user::waterHeightErrorMargin, -0.1, 0, 99);
      device::warningValsAdjusted = true;
    }
    else if (display::touch_x >= 634 && display::touch_x <= 714 && display::touch_y >= 356 && display::touch_y <= 436) { // water lvl warning up
      if (user::convertToInches)
        user::waterHeightErrorMarginInches = adjustValue(user::waterHeightErrorMarginInches, 0.1, 0, 99);
      else
        user::waterHeightErrorMargin = adjustValue(user::waterHeightErrorMargin, 0.1, 0, 99);
      device::warningValsAdjusted = true;
    }
  }
  else if (display::warningsPage == 6) {
    if (display::touch_x >= 534 && display::touch_x <= 614 && display::touch_y >= 356 && display::touch_y <= 436) { // air temp warning ErrorMargin down
      if (user::convertToF)
        user::airTempErrorMarginF = adjustValue(user::airTempErrorMarginF, -0.1, 0, 99);
      else
        user::airTempErrorMargin = adjustValue(user::airTempErrorMargin, -0.1, 0, 99);
      device::warningValsAdjusted = true;
    }
    else if (display::touch_x >= 634 && display::touch_x <= 714 && display::touch_y >= 356 && display::touch_y <= 436) { // air temp warning ErrorMargin up
      if (user::convertToF)
        user::airTempErrorMarginF = adjustValue(user::airTempErrorMarginF, 0.1, 0, 99);
      else
        user::airTempErrorMargin = adjustValue(user::airTempErrorMargin, 0.1, 0, 99);
      device::warningValsAdjusted = true;
    }
  }
  else if (display::warningsPage == 7) {
    if (display::touch_x >= 534 && display::touch_x <= 614 && display::touch_y >= 356 && display::touch_y <= 436) { // humidity warning down
      user::humidityErrorMargin = adjustValue(user::humidityErrorMargin, -0.1, 0, 99);
      device::warningValsAdjusted = true;
    }
    else if (display::touch_x >= 634 && display::touch_x <= 714 && display::touch_y >= 356 && display::touch_y <= 436) { // humidity warning up
      user::humidityErrorMargin = adjustValue(user::humidityErrorMargin, 0.1, 0, 99);
      device::warningValsAdjusted = true;
    }
  }
}

// This function is in charge of all keys pressed within the keybaord.
void keyboardTouched() {
  //Serial.println("keybaord tched fnc");
  unsigned int keyboardColumn = 135;
  unsigned int keyboardRow = 260;
  byte characters;
  byte numbers = '0';
  char* charPtr = 0;
  if (device::upper)
    characters = 'A';
  else
    characters = 'a';
  //
  if (device::profileInputNumber == 1)
    charPtr = user::profileOneName;
  else if (device::profileInputNumber == 2)
    charPtr = user::profileTwoName;
  else if (device::profileInputNumber == 3)
    charPtr = user::profileThreeName;
  else if (device::profileInputNumber == 4)
    charPtr = user::profileFourName;
  else if (device::profileInputNumber == 5)
    charPtr = user::profileFiveName;
  device::intputPosition = strlen(charPtr);
  if (device::intputPosition > 9)
    device::intputPosition = 9;
  //Serial.print(F("input pos: ")); Serial.println(strlen(charPtr));
  // DOT
  if (display::touch_x >= keyboardColumn && display::touch_x <= keyboardColumn + 50 && display::touch_y >= keyboardRow - 60 && display::touch_y <= keyboardRow - 2) {
    //Serial.println("Touched dot");
    beep();
    charPtr[device::intputPosition] = '.';
    if (device::intputPosition < 9)
      device::intputPosition++;
    device::updateKeyboardInput = true;
  }
  // 0-9
  keyboardColumn += 60;
  for (int i = 0; i < 10; i++) {
    if (display::touch_x >= keyboardColumn && display::touch_x <= keyboardColumn + 50 && display::touch_y >= keyboardRow - 60 && display::touch_y <= keyboardRow - 2) {
      //Serial.print("Touched num: "); Serial.println((char)numbers);
      beep();
      charPtr[device::intputPosition] = numbers;
      if (device::intputPosition < 9)
        device::intputPosition++;
      device::updateKeyboardInput = true;
      break;
    }
    keyboardColumn += 60;
    numbers++;
  }
  // a-z A-Z
  keyboardColumn = 135;
  keyboardRow = 260;
  for (int i = 0; i < 26; i++) {
    if (display::touch_x >= keyboardColumn && display::touch_x <= keyboardColumn + 50 && display::touch_y >= keyboardRow && display::touch_y <= keyboardRow + 58) {
      //Serial.print("Touched char: "); Serial.println((char)characters);
      beep();
      charPtr[device::intputPosition] = characters;
      if (device::intputPosition < 9)
        device::intputPosition++;
      device::updateKeyboardInput = true;
      break;
    }
    characters++;
    if (i < 10) {
      keyboardColumn += 60;
    }
    else if (i == 10 || i == 21) {
      keyboardColumn = 135;
      keyboardRow += 60;
    }
    else {
      keyboardColumn += 60;
    }
  }
  // UNDERSCORE
  keyboardColumn = 375;
  keyboardRow = 380;
  if (display::touch_x >= keyboardColumn && display::touch_x <= keyboardColumn + 50 && display::touch_y >= keyboardRow && display::touch_y <= keyboardRow + 58) {
    //Serial.println("Touched UnderScore");
    beep();
    charPtr[device::intputPosition] = '_'; // " " for space
    if (device::intputPosition < 9)
      device::intputPosition++;
    device::updateKeyboardInput = true;
  }
  // DELETE CHAR
  else if (display::touch_x >= (keyboardColumn += 60) && display::touch_x <= keyboardColumn + 80 && display::touch_y >= keyboardRow && display::touch_y <= keyboardRow + 58) {
    //Serial.println("Touched del");
    beep();
    if (device::intputPosition >= 9 && charPtr[device::intputPosition] != 0) {
      charPtr[device::intputPosition] = 0;
    }
    else {
      if (device::intputPosition > 0)
        charPtr[device::intputPosition - 1] = 0;
      charPtr[device::intputPosition] = 0;
      if (device::intputPosition > 0)
        device::intputPosition--;
    }
    device::updateKeyboardInput = true;
    //Serial.print("device::intputPosition = "); Serial.println(device::intputPosition);
  }
  // HOME
  else if (display::touch_x >= (keyboardColumn += 90) && display::touch_x <= keyboardColumn + 120 && display::touch_y >= keyboardRow && display::touch_y <= keyboardRow + 58) {
    beep();
    //Serial.println("Touched keybaord home");
    device::updateKeyboardInput = false;
    display::showKeyboard = false;
    device::keyBoardClosedTime = millis();
    device::lockSaveButtons = true;
    Serial.println(F("keyboard home"));
    clearPage();
    display::refreshPage = true;
  }
  // UPPER - LOWER CASE
  else if (display::touch_x >= (keyboardColumn += 130) && display::touch_x <= keyboardColumn + 120 && display::touch_y >= keyboardRow && display::touch_y <= keyboardRow + 58) {
    static unsigned long previousMillis = 0;
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 5000UL) { // time it takes for keyboard to display plus a healthy margin just in case
      //Serial.println(F("Touched caps"));
      beep();
      display::refreshPage = true;
      device::upper = !device::upper;
      previousMillis = currentMillis;
    }
  }
  //Serial.print("device::charPtr = "); Serial.println(charPtr);
}
