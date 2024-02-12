void touchEvent() {
   if (tft.touched()) {
    tft.touchReadPixel(&display::touch_x, &display::touch_y);
    display::lastTouchMillis = millis();
    if (display::touch_x > 0 && display::touch_y > 0) {     
      //tft.fillCircle(display::touch_x, display::touch_y, 2, RA8875_WHITE); // for touch debugging / calibration check
      sliders();
      static unsigned long previousTouchMillis = 0;
      if (millis() - previousTouchMillis >= display::debounceTime) {       
        if (display::touch_x >= 0 && display::touch_x <= 80 && display::touch_y >= 0 && display::touch_y <= 480
        || display::touch_x >= 0 && display::touch_x <= 800 && display::touch_y >= 0 && display::touch_y <= 80) {
          mainMenuNavigation();
        }
        else {
          adjustPageValues();
        }
        previousTouchMillis = millis();
      }      
    }
  }
  else if (millis() - display::lastTouchMillis >= 250UL) {
    display::touchStartMillis = millis();
    display::debounceTime = 200;
  }    
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
      display::co2PageScrollPos = 0;
      display::waterPageScrollPos = 0;
      display::doserPageScrollPos = 0;
      display::calDoserPageScrollPos = 0;
      display::calPhPageScrollPos = 0;
      display::calTdsPageScrollPos = 0;
      display::refillDoserPageScrollPos = 0;
      display::timerPagePageScrollPos = 0; ////////////////
      display::settingsPage = 0;
      display::settingsPageZeroScrollPos = 0;
      display::settingsPageOneScrollPos = 0;
      display::settingsPageTwoScrollPos = 0;
      display::settingsPageThreeScrollPos= 0;
      display::systemLogScrollPos = 0;
      display::setRtcTimePos = 0;
      display::maxMinsPage = 0;
      display::fansPage = 0;
      display:: warningsPage = 0;
      display::showCalErrorMessage = false;
      display::showingDialog = false;
      display::showInfoDialog = false;
      display::showSystemLogs = false;
      display::showAfkTime = false;
      display::showWifiSsid = false;
      display::showWifiPassword = false;
      display::showGraphInterval = false;
      display::showNumberOfDosers = false;
      display::showDosingInterval = false;
      display::showEcDosingMode = false;
      display::showPhDosingMode = false;
      display::showSr04HeightCalibration = false;
      display::showDoserCalibration = false;
      display::showTdsCalibration = false;
      display::showCo2Calibration = false;
      display::showPhCalibration = false;
      display::showRTCtime = false;
      display::showKeyboard = false;
      display::showSaveDialog = false;
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
    if (display::showDoserCalibration) {
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
    else if (display::showPhCalibration) {
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
      //Serial.print(F("systemLogScrollPos: ")); Serial.print(display::systemLogScrollPos); Serial.print(F(", max systemLogPos: ")); Serial.println(message::systemLogPos);
    }

    else if (display::settingsPage == 0) {
      if (!display::showRTCtime) {
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
        if (slideYIncrement == 1 && display::settingsPageOneScrollPos < 5) {
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
      if (slideYIncrement == 1 && display::settingsPageTwoScrollPos < 1) {
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
    else if (display::settingsPage == 3) {
      if (slideYIncrement == 1 && display::settingsPageThreeScrollPos < 2) {
        display::settingsPageThreeScrollPos++;
        display::refreshPage = true;
        beep();
        clearPage();
      }
      else if (slideYIncrement == -1 && display::settingsPageThreeScrollPos > 0) {
        display::settingsPageThreeScrollPos--;
        display::refreshPage = true;
        beep();
        clearPage();
      }
    }
    // Settings page X axsis slider
    if (!display::showingDialog) {
      if (slideXIncrement == 1 && display::settingsPage < 4) {
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
      if (display::waterPageScrollPos < 4) {
        display::waterPageScrollPos++;
        display::refreshPage = true;
        beep();
        clearPage();
      }
      else if (display::waterPageScrollPos == 4 && display::refillDoserPageScrollPos < user::numberOfDosers - 4) {
        display::refillDoserPageScrollPos++;
        display::refreshPage = true;
        beep();
        clearPage();
      }
    }
    else if (slideXIncrement == -1) {
      if (display::waterPageScrollPos == 4 && display::refillDoserPageScrollPos > 0) {
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
  // Timers page X axsis slider
  else if (display::page == 10) {
    if (slideXIncrement == 1 && display::timerPagePageScrollPos < 2) {
      display::timerPagePageScrollPos++;
      display::refreshPage = true;
      beep();
      clearPage();
    }
    else if (slideXIncrement == -1 && display::timerPagePageScrollPos > 0) {
      display::timerPagePageScrollPos--;
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
  if (display::showInfoDialog)
    infoDialogTouched();
  else if (display::page == 1)
    graphsPageTouched();
  else if (display::page == 2)
    maxMinsPageTouched();
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
    timerPagedTouched();
  else if (display::page == 11)
    fansPageTouched();
  else if (display::page == 12)
    warningsPageTouched();
}

void infoDialogTouched() {
  if (millis() - display::infoDialogDisplayTime > 3000UL) {
    if (display::touch_x >= 100 && display::touch_x <= 800 && display::touch_y >= 100 && display::touch_y <= 480) {
      Serial.println(F("Closing infoDialog..."));
      clearPage();
      display::showingDialog = false;
      display::showInfoDialog = false;
      display::refreshPage = true;
    }
  }
}

void graphsPageTouched() {
  static unsigned long graphTouchPreviousMillis = 0;
  if (millis() - graphTouchPreviousMillis >= 500UL) {
    if (display::touch_x >= 760 && display::touch_x <= 835) {
      if (display::touch_y >= 90 && display::touch_y <= 124)
        display::showTdsGraph = !display::showTdsGraph;
      else if ( display::touch_y >= 124 && display::touch_y <= 158)
        display::showPhGraph = !display::showPhGraph;
      else if ( display::touch_y >= 158 && display::touch_y <= 192)
        display::showEcGraph = !display::showEcGraph;
      else if ( display::touch_y >= 192 && display::touch_y <= 230)
        display::showCo2Graph = !display::showCo2Graph;
      else if ( display::touch_y >= 230 && display::touch_y <= 268)
        display::showWaterTempGraph = !display::showWaterTempGraph;
      else if ( display::touch_y >= 268 && display::touch_y <= 310)
        display::showWaterLvlGraph = !display::showWaterLvlGraph;
      else if ( display::touch_y >= 310 && display::touch_y <= 354)
        display::showFanOneGraph = !display::showFanOneGraph;
      else if ( display::touch_y >= 354 && display::touch_y <= 394)
        display::showFanTwoGraph = !display::showFanTwoGraph;
      else if ( display::touch_y >= 394 && display::touch_y <= 434)
        display::showAirTempGraph = !display::showAirTempGraph;
      else if ( display::touch_y >= 434 && display::touch_y <= 474)
        display::showHumidityGraph = !display::showHumidityGraph;
      beep();
      display::refreshPage = true;
    }
    graphTouchPreviousMillis = millis();
  }
}

void maxMinsPageTouched() {
  if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
    message::infoPos = 20;
    display::showingDialog = true;
    display::showInfoDialog = true;
    display::refreshPage = true;
    display::infoDialogDisplayTime = millis();
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
      getSetSystemEEPROM(device::EEPROM_SET);
      device::settingsAdjusted = true;
      delay(20);
      EEPROM.put(7, device::userProfile);
      delay(20);
      device::lockSaveButtons = true;
      device::keyBoardClosedTime = millis();
      getSetProfileEEPROM(device::userProfile, device::EEPROM_SET);
      display::showSaveDialog = false;
      display::refreshPage = true;
      clearPage();
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 21;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
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
      device::settingsAdjusted = true;
      display::refreshPage = true;
      display::showSaveDialog = true;
    }
    else if (display::touch_x >= 490 && display::touch_x <= 620 && display::touch_y >= 284 && display::touch_y <= 326) {
      beep();
      device::userProfile = 2;
      device::settingsAdjusted = true;
      display::refreshPage = true;
      display::showSaveDialog = true;;
    }
    else if (display::touch_x >= 490 && display::touch_x <= 620 && display::touch_y >= 346 && display::touch_y <= 388) {
      beep();
      device::userProfile = 3;
      device::settingsAdjusted = true;
      display::refreshPage = true;
      display::showSaveDialog = true;
    }
    else if (display::touch_x >= 490 && display::touch_x <= 620 && display::touch_y >= 408 && display::touch_y <= 450) {
      beep();
      device::userProfile = 4;
      device::settingsAdjusted = true;
      display::refreshPage = true;
      display::showSaveDialog = true;
    }
    else if (display::touch_x >= 630 && display::touch_x <= 760 && display::touch_y >= 160 && display::touch_y <= 202) {
      beep();
      device::userProfile = 0;
      device::settingsAdjusted = true;
      display::refreshPage = true;
      device::lockSaveButtons = true;
      device::keyBoardClosedTime = millis();
      getSetProfileEEPROM(device::userProfile, device::EEPROM_GET);
      EEPROM.put(7, device::userProfile);
      delay(20);
    }
    else if (display::touch_x >= 630 && display::touch_x <= 760 && display::touch_y >= 222 && display::touch_y <= 262) {
      beep();
      device::userProfile = 1;
      device::settingsAdjusted = true;
      display::refreshPage = true;
      device::lockSaveButtons = true;
      device::keyBoardClosedTime = millis();
      getSetProfileEEPROM(device::userProfile, device::EEPROM_GET);
      EEPROM.put(7, device::userProfile);
      delay(20);
    }
    else if (display::touch_x >= 630 && display::touch_x <= 760 && display::touch_y >= 284 && display::touch_y <= 326) {
      beep();
      device::userProfile = 2;
      device::settingsAdjusted = true;
      display::refreshPage = true;
      device::lockSaveButtons = true;
      device::keyBoardClosedTime = millis();
      getSetProfileEEPROM(device::userProfile, device::EEPROM_GET);
      EEPROM.put(7, device::userProfile);
      delay(20);
    }
    else if (display::touch_x >= 630 && display::touch_x <= 760 && display::touch_y >= 346 && display::touch_y <= 388) {
      beep();
      device::userProfile = 3;
      device::settingsAdjusted = true;
      display::refreshPage = true;
      device::lockSaveButtons = true;
      device::keyBoardClosedTime = millis();
      getSetProfileEEPROM(device::userProfile, device::EEPROM_GET);
      EEPROM.put(7, device::userProfile);
      delay(20);
    }
    else if (display::touch_x >= 630 && display::touch_x <= 760 && display::touch_y >= 408 && display::touch_y <= 450) {
      beep();
      device::userProfile = 4;
      device::settingsAdjusted = true;
      display::refreshPage = true;
      device::lockSaveButtons = true;
      device::keyBoardClosedTime = millis();
      getSetProfileEEPROM(device::userProfile, device::EEPROM_GET);
      EEPROM.put(7, device::userProfile);
      delay(20);
    }
  }
  else if (display::showKeyboard) {
    char* a_charPtr = NULL;
    if (device::profileInputNumber == 1)
      a_charPtr = user::profileOneName;
    else if (device::profileInputNumber == 2)
      a_charPtr = user::profileTwoName;
    else if (device::profileInputNumber == 3)
      a_charPtr = user::profileThreeName;
    else if (device::profileInputNumber == 4)
      a_charPtr = user::profileFourName;
    else if (device::profileInputNumber == 5)
      a_charPtr = user::profileFiveName;
    static unsigned long previousKeyTouch;
    if (millis() - previousKeyTouch > 500UL) {
      keyboardTouched(a_charPtr, 16);
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
  else if (display::settingsPage == 4)
    settingsFourPageTouched();
}


void settingsZeroPageTouched() {
  if (display::showRTCtime) {
    if (display::touch_x >= 295 && display::touch_x <= 345 && display::touch_y >= 375 && display::touch_y <= 425 ) { // setRtcTimePos right
      display::setRtcTimePos = adjustValue(display::setRtcTimePos, 1, 0, 7);
      display::refreshPage = true;
    }
    else if (display::touch_x >= 385 && display::touch_x <= 435 && display::touch_y >= 375 && display::touch_y <= 425 ) { // setRtcTimePos left
      display::setRtcTimePos = adjustValue(display::setRtcTimePos, -1, 0, 7);
      display::refreshPage = true;
    }
    if (display::setRtcTimePos == 7)
      display::setRtcTimePos = 0;

    if (display::setRtcTimePos == 0) {
      if (display::touch_x >= 495 && display::touch_x <= 545 && display::touch_y >= 375 && display::touch_y <= 425 ) // rtcDay down
        user::rtcDayOfWeek = adjustValue(user::rtcDayOfWeek, -1, 0, 6);
      else if (display::touch_x >= 585 && display::touch_x <= 635 && display::touch_y >= 375 && display::touch_y <= 425 ) // rtcDay up
        user::rtcDayOfWeek = adjustValue(user::rtcDayOfWeek, 1, 0, 7);
      if (user::rtcDayOfWeek == 7)
        user::rtcDayOfWeek = 0;
    }
    else if (display::setRtcTimePos == 1) {
      if (display::touch_x >= 495 && display::touch_x <= 545 && display::touch_y >= 375 && display::touch_y <= 425 ) // rtcDay down
        user::rtcDay = adjustValue(user::rtcDay, -1, 1, 31);
      else if (display::touch_x >= 585 && display::touch_x <= 635 && display::touch_y >= 375 && display::touch_y <= 425 ) // rtcDay up
        user::rtcDay = adjustValue(user::rtcDay, 1, 1, 31);      
    }
    else if (display::setRtcTimePos == 2) {
      if (display::touch_x >= 495 && display::touch_x <= 545 && display::touch_y >= 375 && display::touch_y <= 425 ) // rtcMonth down
        user::rtcMonth = adjustValue(user::rtcMonth, -1, 1, 12);
      else if (display::touch_x >= 585 && display::touch_x <= 635 && display::touch_y >= 375 && display::touch_y <= 425 ) // rtcMonth up
        user::rtcMonth = adjustValue(user::rtcMonth, 1, 1, 12);      
    }
    else if (display::setRtcTimePos == 3) {
      if (display::touch_x >= 495 && display::touch_x <= 545 && display::touch_y >= 375 && display::touch_y <= 425 ) // rtcYear down
        user::rtcYear = adjustValue(user::rtcYear, -1, 24, 99);
      else if (display::touch_x >= 585 && display::touch_x <= 635 && display::touch_y >= 375 && display::touch_y <= 425 ) // rtcYear up
        user::rtcYear = adjustValue(user::rtcYear, 1, 24, 99);      
    }
    else if (display::setRtcTimePos == 4) {
      if (display::touch_x >= 495 && display::touch_x <= 545 && display::touch_y >= 375 && display::touch_y <= 425 ) // rtcHour down
        user::rtcHour = adjustValue(user::rtcHour, -1, 0, 23);
      else if (display::touch_x >= 585 && display::touch_x <= 635 && display::touch_y >= 375 && display::touch_y <= 425 ) // rtcHour up
        user::rtcHour = adjustValue(user::rtcHour, 1, 0, 23);      
    }
    else if (display::setRtcTimePos == 5) {
      if (display::touch_x >= 495 && display::touch_x <= 545 && display::touch_y >= 375 && display::touch_y <= 425 ) // rtcMinute down
        user::rtcMinute = adjustValue(user::rtcMinute, -1, 0, 59);
      else if (display::touch_x >= 585 && display::touch_x <= 635 && display::touch_y >= 375 && display::touch_y <= 425 ) // rtcMinute up
        user::rtcMinute = adjustValue(user::rtcMinute, 1, 0, 59);      
    }
    else {
      if (display::touch_x >= 495 && display::touch_x <= 545 && display::touch_y >= 375 && display::touch_y <= 425 ) // rtcSecond down
        user::rtcSecond = adjustValue(user::rtcSecond, -1, 0, 59);
      else if (display::touch_x >= 585 && display::touch_x <= 635 && display::touch_y >= 375 && display::touch_y <= 425 ) // rtcSecond up
        user::rtcSecond = adjustValue(user::rtcSecond, 1, 0, 59);      
    }
    if (display::touch_x >= 260 && display::touch_x <= 420 && display::touch_y >= 430 && display::touch_y <= 470) { // cancel
      beep();
      display::showRTCtime = false;
      clearPage();
      display::showingDialog = false;
      display::refreshPage = true;
    }
    else if (display::touch_x >= 460 && display::touch_x <= 620 && display::touch_y >= 430 && display::touch_y <= 470) { // confirm
      beep();
      adjustrtc();
      display::showRTCtime = false;
      clearPage();
      display::showingDialog = false;
      display::refreshPage = true;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 23;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else if (display::showAfkTime) {
    if (display::touch_x >= 370 && display::touch_x <= 420 && display::touch_y >= 350 && display::touch_y <= 400) // afkTime time down
      user::afkTime = adjustValue(user::afkTime, -1, 0, 240);
    else if (display::touch_x >= 500 && display::touch_x <= 550 && display::touch_y >= 350 && display::touch_y <= 400) // afkTime time up
      user::afkTime = adjustValue(user::afkTime, 1, 0, 240);
    else if (display::touch_x >= 390 && display::touch_x <= 490 && display::touch_y >= 430 && display::touch_y <= 479) {// exit
      beep();
      clearPage();
      display::showAfkTime = false;
      display::refreshPage = true;    
      display::showingDialog = false;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 24;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else if (display::showBrightness) {
    if (display::touch_x >= 370 && display::touch_x <= 420 && display::touch_y >= 350 && display::touch_y <= 400) { // brightness down
      user::brightness = adjustValue(user::brightness, -1, 20, 100);
      tft.brightness(user::brightness);
    }
    else if (display::touch_x >= 500 && display::touch_x <= 550 && display::touch_y >= 350 && display::touch_y <= 400) { // brightness up
      user::brightness = adjustValue(user::brightness, 1, 20, 100);
      tft.brightness(user::brightness);
    }
    else if (display::touch_x >= 390 && display::touch_x <= 490 && display::touch_y >= 430 && display::touch_y <= 479) {// exit
      beep();
      clearPage();
      display::showBrightness = false;
      display::refreshPage = true;    
      display::showingDialog = false;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 38;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else if (display::showGraphInterval) {
    if (display::touch_x >= 370 && display::touch_x <= 420 && display::touch_y >= 350 && display::touch_y <= 400) // graphInterval time down
      user::graphInterval = adjustInterval(user::graphInterval, -1);
    else if (display::touch_x >= 500 && display::touch_x <= 550 && display::touch_y >= 350 && display::touch_y <= 400) // graphInterval time up
      user::graphInterval = adjustInterval(user::graphInterval, 1);
    else if (display::touch_x >= 390 && display::touch_x <= 490 && display::touch_y >= 430 && display::touch_y <= 479) {// exit
      beep();
      clearPage();
      display::showGraphInterval = false;
      display::refreshPage = true;    
      display::showingDialog = false;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 25;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else {
    int scrollMargin = display::settingsPageZeroScrollPos * 50;
    // vertical menu slider
    static unsigned long lastTouchTime = 0;
    if (millis() - lastTouchTime >= 1500UL) {
      if (display::touch_x >= 760 && display::touch_x <= 800 && display::touch_y >= 100 && display::touch_y <= 140) {
        if (display::settingsPageZeroScrollPos > 0) {
          display::settingsPageZeroScrollPos--;
          display::refreshPage = true;
          beep();
          clearPage();
        }
      } 
      else if (display::touch_x >= 760 && display::touch_x <= 800 && display::touch_y > 140 && display::touch_y < 430) {
        display::settingsPageZeroScrollPos = map(display::touch_y, 140, 430, 0, 4);
        if (display::settingsPageZeroScrollPos > 3)
          display::settingsPageZeroScrollPos = 3;
        //Serial.print(F("settingsPageZero slider menu pos = ")); Serial.println(display::settingsPageZeroScrollPos);
        display::refreshPage = true;
        beep();
        clearPage();
      } 
      else if (display::touch_x >= 760 && display::touch_x <= 800 && display::touch_y >= 430 && display::touch_y <= 470) {
        if (display::settingsPageZeroScrollPos < 3) {
          display::settingsPageZeroScrollPos++;
          display::refreshPage = true;
          beep();
          clearPage();
        }
      }
      lastTouchTime = millis();
    }
    //
    if (display::settingsPageZeroScrollPos == 0) {
      if (display::touch_x >= 508 && display::touch_x <= 538 && display::touch_y >= 168 - scrollMargin && display::touch_y <= 198 - scrollMargin) // Change background color to light blue
        setBackground(display::RA8875_LIGHT_BLUE);
      else if (display::touch_x >= 558 && display::touch_x <= 588 && display::touch_y >= 168 - scrollMargin && display::touch_y <= 198 - scrollMargin) // Change background color to light green
        setBackground(display::RA8875_LIGHT_GREEN);
      else if (display::touch_x >= 608 && display::touch_x <= 638 && display::touch_y >= 168 - scrollMargin && display::touch_y <= 198 - scrollMargin) // Change background color to light purple
        setBackground(RA8875_MAGENTA);
      else if (display::touch_x >= 658 && display::touch_x <= 688 && display::touch_y >= 168 - scrollMargin && display::touch_y <= 198 - scrollMargin) // Change background color to light pink
        setBackground(display::RA8875_LIGHT_PINK);
      else if (display::touch_x >= 708 && display::touch_x <= 738 && display::touch_y >= 168 - scrollMargin && display::touch_y <= 198 - scrollMargin) // Change background color to white
        setBackground(RA8875_WHITE);
    }
    if (display::settingsPageZeroScrollPos <= 1) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 220 - scrollMargin && display::touch_y <= 250 - scrollMargin) { // AfkTime
        beep();
        display::showAfkTime = true;
        display::showingDialog = true;
        clearPage();
        display::refreshPage = true;
      }
    }
    if (display::settingsPageZeroScrollPos <= 2) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 270 - scrollMargin && display::touch_y <= 300 - scrollMargin) { // display brightness
        beep();
        display::showBrightness = true;
        display::showingDialog = true;
        clearPage();
        display::refreshPage = true;
      }
    }
    if (display::settingsPageZeroScrollPos <= 3) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 320 - scrollMargin && display::touch_y <= 350 - scrollMargin) { // graphInterval
        beep();
        display::showGraphInterval = true;
        display::showingDialog = true;
        clearPage();
        display::refreshPage = true;
      }
    }
    if (display::settingsPageZeroScrollPos <= 4) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 370 - scrollMargin && display::touch_y <= 400 - scrollMargin) { // setrtc
        beep();
        rtc.refresh();
        user::rtcDayOfWeek = rtc.dayOfTheWeek();
        user::rtcSecond = rtc.second();
        user::rtcMinute = rtc.minute();
        user::rtcHour = rtc.hour();
        user::rtcDay = rtc.day();
        user::rtcMonth = rtc.month();
        if (rtc.year() >= 23)
          user::rtcYear = rtc.year();
        else
          user::rtcYear = 23;
        display::showRTCtime = true;
        clearPage();
        display::refreshPage = true;
      }
    }
    if (display::settingsPageZeroScrollPos <= 5) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 420 - scrollMargin && display::touch_y <= 450 - scrollMargin) { // convertToFarenheit
        user::convertToF  = !user::convertToF;
        device::settingsAdjusted = true;
        beep();
      }
    }
    if (display::settingsPageZeroScrollPos >= 1 && display::settingsPageZeroScrollPos <= 6) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 470 - scrollMargin && display::touch_y <= 500 - scrollMargin) { // convertToInches
        user::convertToInches = !user::convertToInches;
        device::settingsAdjusted = true;
        beep();
      }
    }
    if (display::settingsPageZeroScrollPos >= 2 && display::settingsPageZeroScrollPos <= 7) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 520 - scrollMargin && display::touch_y <= 550 - scrollMargin) { // convertToTds
        beep();
        tft.fillRect(11, 11, 76, 74, user::backgroundColor);
        user::convertToTds = !user::convertToTds;
        device::settingsAdjusted = true;
        if (user::convertToTds)
          drawPageIcon(5, 23, 32, ppmIcon, 280, 64, 35);
        else
          drawPageIcon(5, 26, 32, ecIcon, 256, 57, 36);
      }
    }
    if (display::settingsPageZeroScrollPos >= 2 && display::settingsPageZeroScrollPos <= 7) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 570 - scrollMargin && display::touch_y <= 600 - scrollMargin) { // EC conversion mode
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
    // vertical menu slider
    if (message::systemLogPos > 6) {
      static unsigned long lastTouchTime = 0;
      if (millis() - lastTouchTime >= 1500UL) {
        if (display::touch_x >= 760 && display::touch_x <= 800 && display::touch_y >= 100 && display::touch_y <= 140) {
          if (display::systemLogScrollPos > 0) {
            display::systemLogScrollPos--;
            display::refreshPage = true;
            beep();
            clearPage();
          }
        } 
        else if (display::touch_x >= 760 && display::touch_x <= 800 && display::touch_y > 140 && display::touch_y < 430) {
          display::systemLogScrollPos = map(display::touch_y, 140, 430, 0, message::systemLogPos - 5);
          if (display::systemLogScrollPos > message::systemLogPos - 6)
            display::systemLogScrollPos = message::systemLogPos - 6;
          //Serial.print(F("settingsPageZero slider menu pos = ")); Serial.println(display::systemLogScrollPos);
          display::refreshPage = true;
          beep();
          clearPage();
        } 
        else if (display::touch_x >= 760 && display::touch_x <= 800 && display::touch_y >= 430 && display::touch_y <= 470) {
          if (display::systemLogScrollPos < message::systemLogPos - 6) {
            display::systemLogScrollPos++;
            display::refreshPage = true;
            beep();
            clearPage();
          }
        }
        lastTouchTime = millis();
      }
    }
    // exit
    if (display::touch_x >= 390 && display::touch_x <= 490 && display::touch_y >= 430 && display::touch_y <= 479) {
      beep();
      display::showSystemLogs = false;
      display::refreshPage = true;
      clearPage();
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 28;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else if (display::showWifiSsid) {
    if (!display::showKeyboard) {
      if (display::touch_x >= 235 && display::touch_x <= 580 && display::touch_y >= 250 && display::touch_y <= 300) {
        loadKeyBoard(0);
        display::showingDialog = true;
      }
      else if (millis() - device::keyBoardClosedTime >= 1500) {
        if (display::touch_x >= 260 && display::touch_x <= 420 && display::touch_y >= 430 && display::touch_y <= 470) { // cancel
          beep();
          display::showWifiSsid = false;
          clearPage();
          display::showingDialog = false;
          display::refreshPage = true;
        }
        else if (display::touch_x >= 460 && display::touch_x <= 620 && display::touch_y >= 430 && display::touch_y <= 470) { // confirm
          beep();
          display::showWifiSsid = false;
          clearPage();
          display::showingDialog = false;
          display::refreshPage = true;
        }
      }
      else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
        message::infoPos = 33;
        display::showingDialog = true;
        display::showInfoDialog = true;
        display::refreshPage = true;
        display::infoDialogDisplayTime = millis();
      }
    }
    else {
      static unsigned long previousKeyTouch;
      if (millis() - previousKeyTouch > 500UL) {
        keyboardTouched(wifi::ssid, 16);
        previousKeyTouch = millis();
      }
    }
  }
  else if (display::showWifiPassword) {
    if (!display::showKeyboard) {
      if (display::touch_x >= 235 && display::touch_x <= 580 && display::touch_y >= 250 && display::touch_y <= 300) {
        loadKeyBoard(0);
        display::showingDialog = true;
      }
      else if (millis() - device::keyBoardClosedTime >= 1500) {
        if (display::touch_x >= 260 && display::touch_x <= 420 && display::touch_y >= 430 && display::touch_y <= 470) { // cancel
          beep();
          display::showWifiPassword = false;
          clearPage();
          display::showingDialog = false;
          display::refreshPage = true;
        }
        else if (display::touch_x >= 460 && display::touch_x <= 620 && display::touch_y >= 430 && display::touch_y <= 470) { // confirm
          beep();
          display::showWifiPassword = false;
          clearPage();
          display::showingDialog = false;
          display::refreshPage = true;
        }
        else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
          message::infoPos = 34;
          display::showingDialog = true;
          display::showInfoDialog = true;
          display::refreshPage = true;
          display::infoDialogDisplayTime = millis();
        }
      }
    }
    else {
      static unsigned long previousKeyTouch;
      if (millis() - previousKeyTouch > 500UL) {
        keyboardTouched(wifi::password, 16);
        previousKeyTouch = millis();
      }
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
      reset();
      // TO DO
    }
  }
  else {
    // vertical menu slider
    static unsigned long lastTouchTime = 0;
    if (millis() - lastTouchTime >= 1500UL) {
      if (display::touch_x >= 760 && display::touch_x <= 800 && display::touch_y >= 100 && display::touch_y <= 140) {
        if (display::settingsPageOneScrollPos > 0) {
          display::settingsPageOneScrollPos--;
          display::refreshPage = true;
          beep();
          clearPage();
        }
      } 
      else if (display::touch_x >= 760 && display::touch_x <= 800 && display::touch_y > 140 && display::touch_y < 430) {
        display::settingsPageOneScrollPos = map(display::touch_y, 140, 430, 0, 6);
        if (display::settingsPageOneScrollPos > 5)
          display::settingsPageOneScrollPos = 5;
        //Serial.print(F("settingsPageZero slider menu pos = ")); Serial.println(display::settingsPageOneScrollPos);
        display::refreshPage = true;
        beep();
        clearPage();
      } 
      else if (display::touch_x >= 760 && display::touch_x <= 800 && display::touch_y >= 430 && display::touch_y <= 470) {
        if (display::settingsPageOneScrollPos < 5) {
          display::settingsPageOneScrollPos++;
          display::refreshPage = true;
          beep();
          clearPage();
        }
      }
      lastTouchTime = millis();
    }
    int scrollMargin = display::settingsPageOneScrollPos * 50;  // spacing is 50px
    if (display::settingsPageOneScrollPos == 0) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 170 - scrollMargin && display::touch_y <= 200 - scrollMargin) { // Show system logs
        beep();
        display::showSystemLogs = true;
        display::refreshPage = true;
        clearPage();
      }
    }
    if (display::settingsPageOneScrollPos <= 1) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 220 - scrollMargin && display::touch_y <= 250 - scrollMargin) { // enable Wifi
        static unsigned long previousWifiOnOffTime = millis();
        if (millis() >= previousWifiOnOffTime + 1000UL) {
          wifi::wifiEnabled = !wifi::wifiEnabled;
          Serial.print(F("Wifi AP ")); Serial.println(wifi::wifiEnabled  ? F("enabled") : F("disabled"));
          beep();
          previousWifiOnOffTime = millis();
        }
      }
    }
    if (display::settingsPageOneScrollPos <= 2) {
     if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 270 - scrollMargin && display::touch_y <= 300 - scrollMargin) { // Set WiFi SSID
        beep();
        display::showWifiSsid = true;
        display::refreshPage = true;
        clearPage();
      }
    }
    if (display::settingsPageOneScrollPos <= 3) {
     if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 320 - scrollMargin && display::touch_y <= 350 - scrollMargin) { // Set WiFi password
        beep();
        display::showWifiPassword = true;
        display::refreshPage = true;
        clearPage();
      }
    }
    if (display::settingsPageOneScrollPos <= 4) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 370 - scrollMargin && display::touch_y <= 400 - scrollMargin) { // enable WiFi hidden network
        wifi::hiddenNetwork = !wifi::hiddenNetwork;
        beep();
      }
    }
    if (display::settingsPageOneScrollPos <= 5) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 420 - scrollMargin && display::touch_y <= 450 - scrollMargin) { // disableLED
        beep();
        user::disableLED = !user::disableLED;
        user::disableLED ? sendToSlave('L', 1) : sendToSlave('L', 0);
      }
    }
    if (display::settingsPageOneScrollPos >= 1 && display::settingsPageOneScrollPos <= 6) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 470 - scrollMargin && display::touch_y <= 500 - scrollMargin) { // disableBeeper
        user::disableBeeper = !user::disableBeeper;
        beep();
      }
    }
    if (display::settingsPageOneScrollPos >= 2 && display::settingsPageOneScrollPos <= 7) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 520 - scrollMargin && display::touch_y <= 550 - scrollMargin) { // clear graph history
        sensor::sensorArrayPos = 0;
        beep();
      }
    }
    if (display::settingsPageOneScrollPos >= 3 && display::settingsPageOneScrollPos <= 8) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 570 - scrollMargin && display::touch_y <= 600 - scrollMargin) { // clear max mins
        beep();
        clearMaxMins();
        setMaxMins();
      }
    }
    if (display::settingsPageOneScrollPos >= 4 && display::settingsPageOneScrollPos <= 9) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 620 - scrollMargin && display::touch_y <= 650 - scrollMargin) { // reset system logs
        beep();
        clearSystemLogs();
      }
    }
    if (display::settingsPageOneScrollPos >= 5 && display::settingsPageOneScrollPos <= 10) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 670 - scrollMargin && display::touch_y <= 720 - scrollMargin) { // reset all device Settings
        user::resetSettings = true;
        display::refreshPage = true;
        beep();
      }
    }
  }
}

void settingsTwoPageTouched() {
  if (display::showNumberOfDosers) {
    if (display::touch_x >= 370 && display::touch_x <= 420 && display::touch_y >= 350 && display::touch_y <= 400) {// numberOfDosers time down
      user::numberOfDosers = adjustValue(user::numberOfDosers, -1, 4, 6);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 500 && display::touch_x <= 550 && display::touch_y >= 350 && display::touch_y <= 400) {// numberOfDosers time up
      user::numberOfDosers = adjustValue(user::numberOfDosers, 1, 4, 6);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 390 && display::touch_x <= 490 && display::touch_y >= 430 && display::touch_y <= 479) {// exit
      beep();
      clearPage();
      display::showNumberOfDosers = false;
      display::refreshPage = true;    
      display::showingDialog = false;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 27;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else if (display::showDosingInterval) {
    if (display::touch_x >= 370 && display::touch_x <= 420 && display::touch_y >= 350 && display::touch_y <= 400) {// dosingInterval time down
      user::dosingInterval = adjustValue(user::dosingInterval, -1, 1, 240);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 500 && display::touch_x <= 550 && display::touch_y >= 350 && display::touch_y <= 400) {// dosingInterval time up
      user::dosingInterval = adjustValue(user::dosingInterval, 1, 1, 240);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 390 && display::touch_x <= 490 && display::touch_y >= 430 && display::touch_y <= 479) {// exit
      beep();
      clearPage();
      display::showDosingInterval = false;
      display::refreshPage = true;    
      display::showingDialog = false;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 26;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else if (display::showEcDosingMode) {
    if (display::touch_x >= 190 && display::touch_x <= 700 && display::touch_y >= 220 && display::touch_y <= 270) { // EC dosing mode precise
      beep();
      user::ecDosingMode = user::PRECISE;
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 190 && display::touch_x <= 700 && display::touch_y >= 320 && display::touch_y <= 370) { // EC dosing mode incremental
      beep();
      user::ecDosingMode = user::INCREMENTAL;
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 390 && display::touch_x <= 490 && display::touch_y >= 430 && display::touch_y <= 479) {// exit
      beep();
      clearPage();
      display::showEcDosingMode = false;
      display::refreshPage = true;    
      display::showingDialog = false;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 29;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else if (display::showPhDosingMode) {
    if (display::touch_x >= 190 && display::touch_x <= 700 && display::touch_y >= 220 && display::touch_y <= 270) { // PH dosing mode precise
      beep();
      user::phDosingMode = user::PRECISE;
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 190 && display::touch_x <= 700 && display::touch_y >= 320 && display::touch_y <= 370) { // PH dosing mode incremental
      beep();
      user::phDosingMode = user::INCREMENTAL;
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 390 && display::touch_x <= 490 && display::touch_y >= 430 && display::touch_y <= 479) {// exit
      beep();
      clearPage();
      display::showPhDosingMode = false;
      display::refreshPage = true;    
      display::showingDialog = false;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 30;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else if (display::showEcTdsValue) {
    if (display::touch_x >= 210 && display::touch_x <= 260 && display::touch_y >= 400 && display::touch_y <= 450) {  // ec solution down
      sensor::ecSolution = adjustValue(sensor::ecSolution, -0.1, 1, 10);
      sensor::tdsSolution = convertEcToTds(sensor::ecSolution);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 310 && display::touch_x <= 360 && display::touch_y >= 400 && display::touch_y <= 450) {  // ec solution up
      sensor::ecSolution = adjustValue(sensor::ecSolution, 1, 0.1, 10);
      sensor::tdsSolution = convertEcToTds(sensor::ecSolution);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 580 && display::touch_x <= 630 && display::touch_y >= 400 && display::touch_y <= 450) {  // tds solution down
      sensor::tdsSolution = adjustValue(sensor::tdsSolution, -0.1, 1, 9999);
      sensor::ecSolution = convertTdsToEc(sensor::tdsSolution);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 680 && display::touch_x <= 730 && display::touch_y >= 400 && display::touch_y <= 450) {  // tds solution up
      sensor::tdsSolution = adjustValue(sensor::tdsSolution, 0.1, 1, 9999);
      sensor::ecSolution = convertTdsToEc(sensor::tdsSolution);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 390 && display::touch_x <= 490 && display::touch_y >= 430 && display::touch_y <= 479) {// exit
      beep();
      clearPage();
      display::showEcTdsValue = false;
      display::refreshPage = true;    
      display::showingDialog = false;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 31;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else if (display::showPhDownUpValue) {
    if (display::touch_x >= 210 && display::touch_x <= 260 && display::touch_y >= 400 && display::touch_y <= 450) {  // ph down solution down
      sensor::phDownSolution = adjustValue(sensor::phDownSolution, -0.1, 1, 14);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 310 && display::touch_x <= 360 && display::touch_y >= 400 && display::touch_y <= 450) {  // ph down solution up
      sensor::phDownSolution = adjustValue(sensor::phDownSolution, 0.1, 1, 14);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 580 && display::touch_x <= 630 && display::touch_y >= 400 && display::touch_y <= 450) {  // ph up solution down
      sensor::phUpSolution = adjustValue(sensor::phUpSolution, -0.1, 1, 14);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 680 && display::touch_x <= 730 && display::touch_y >= 400 && display::touch_y <= 450) {  // ph up solution up
      sensor::phUpSolution = adjustValue(sensor::phUpSolution, 0.1, 1, 14);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 390 && display::touch_x <= 490 && display::touch_y >= 430 && display::touch_y <= 479) {// exit
      beep();
      clearPage();
      display::showPhDownUpValue = false;
      display::refreshPage = true;    
      display::showingDialog = false;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 32;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else {
    // vertical menu slider
    static unsigned long lastTouchTime = 0;
    if (millis() - lastTouchTime >= 1500UL) {
      if (display::touch_x >= 760 && display::touch_x <= 800 && display::touch_y >= 100 && display::touch_y <= 140) {
        if (display::settingsPageTwoScrollPos > 0) {
          display::settingsPageTwoScrollPos--;
          display::refreshPage = true;
          beep();
          clearPage();
        }
      } 
      else if (display::touch_x >= 760 && display::touch_x <= 800 && display::touch_y > 140 && display::touch_y < 430) {
        display::settingsPageTwoScrollPos = map(display::touch_y, 140, 430, 0, 2);
        if (display::settingsPageTwoScrollPos > 1)
          display::settingsPageTwoScrollPos = 1;
        //Serial.print(F("settingsPageZero slider menu pos = ")); Serial.println(display::settingsPageTwoScrollPos);
        display::refreshPage = true;
        beep();
        clearPage();
      } 
      else if (display::touch_x >= 760 && display::touch_x <= 800 && display::touch_y >= 430 && display::touch_y <= 470) {
        if (display::settingsPageTwoScrollPos < 1) {
          display::settingsPageTwoScrollPos++;
          display::refreshPage = true;
          beep();
          clearPage();
        }
      }
      lastTouchTime = millis();
    }
    int scrollMargin = display::settingsPageTwoScrollPos * 50;  // spacing is 50px
    if (display::settingsPageTwoScrollPos <= 0) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 170 - scrollMargin && display::touch_y <= 200 - scrollMargin) { // NumberOfDosers
        beep();
        display::showNumberOfDosers = true;
        display::showingDialog = true;
        clearPage();
        display::refreshPage = true;
      }
    }
    if (display::settingsPageTwoScrollPos <= 1) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 220 - scrollMargin && display::touch_y <= 250 - scrollMargin) { // DosingInterval
        beep();
        display::showDosingInterval = true;
        display::showingDialog = true;
        clearPage();
        display::refreshPage = true;
      }
    }
    if (display::settingsPageTwoScrollPos <= 2) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 270 - scrollMargin && display::touch_y <= 300 - scrollMargin) { // ecDosingMode
        beep();
        display::showEcDosingMode = true;
        display::showingDialog = true;
        clearPage();
        display::refreshPage = true;
      }
    }
    if (display::settingsPageTwoScrollPos <= 3) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 320 - scrollMargin && display::touch_y <= 350 - scrollMargin) { // phDosingMode
        beep();
        display::showPhDosingMode = true;
        display::showingDialog = true;
        clearPage();
        display::refreshPage = true;
      }
    }
    if (display::settingsPageTwoScrollPos <= 4) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 370 - scrollMargin && display::touch_y <= 400 - scrollMargin) { // Set EC/TDS value
        beep();
        display::showEcTdsValue = true;
        display::showingDialog = true;
        clearPage();
        display::refreshPage = true;
      }
    }
    if (display::settingsPageTwoScrollPos <= 5) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 420 - scrollMargin && display::touch_y <= 450 - scrollMargin) { // Set PH down/up value
        beep();
        display::showPhDownUpValue = true;
        display::showingDialog = true;
        clearPage();
        display::refreshPage = true;
      }
    }
    if (display::settingsPageTwoScrollPos >= 1 && display::settingsPageTwoScrollPos <= 6) {
      if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 470 - scrollMargin && display::touch_y <= 500 - scrollMargin) { // useEtapeSensor
        static unsigned long previousDMLTime = millis();
        if (millis() >= previousDMLTime + 500UL) {
          if (user::heightSensor == user::SR04)
            user::heightSensor = user::ETAPE;
          else if (user::heightSensor == user::ETAPE)
            user::heightSensor = (user::disableVL53L0X ? user::SR04 : user::VL53L0X);
          else if (user::heightSensor == user::VL53L0X)
            user::heightSensor = user::SR04;
          beep();
          previousDMLTime = millis();
        }
      }
    }
  }
}

void settingsThreePageTouched() {
  // vertical menu slider
  static unsigned long lastTouchTime = 0;
  if (millis() - lastTouchTime >= 1500UL) {
    if (display::touch_x >= 760 && display::touch_x <= 800 && display::touch_y >= 100 && display::touch_y <= 140) {
      if (display::settingsPageThreeScrollPos > 0) {
        display::settingsPageThreeScrollPos--;
        display::refreshPage = true;
        beep();
        clearPage();
      }
    } 
    else if (display::touch_x >= 760 && display::touch_x <= 800 && display::touch_y > 140 && display::touch_y < 430) {
      display::settingsPageThreeScrollPos = map(display::touch_y, 140, 430, 0, 3);
      if (display::settingsPageThreeScrollPos > 2)
        display::settingsPageThreeScrollPos = 2;
      //Serial.print(F("settingsPageZero slider menu pos = ")); Serial.println(display::settingsPageThreeScrollPos);
      display::refreshPage = true;
      beep();
      clearPage();
    } 
    else if (display::touch_x >= 760 && display::touch_x <= 800 && display::touch_y >= 430 && display::touch_y <= 470) {
      if (display::settingsPageThreeScrollPos < 2) {
        display::settingsPageThreeScrollPos++;
        display::refreshPage = true;
        beep();
        clearPage();
      }
    }
    lastTouchTime = millis();
  }
  int scrollMargin = display::settingsPageThreeScrollPos * 50;
  if (display::settingsPageThreeScrollPos == 0) {
    if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 170 - scrollMargin && display::touch_y <= 200 - scrollMargin) { // disable PPM warnings
      user::disablePpmWarnings = !user::disablePpmWarnings;
      device::settingsAdjusted = true;
      beep();
    }
  }
  if (display::settingsPageThreeScrollPos <= 1) {
    if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 220 - scrollMargin && display::touch_y <= 250 - scrollMargin) { // disable EC warnings
      user::disableEcWarnings = !user::disableEcWarnings;
      device::settingsAdjusted = true;
      beep();
    }
  }
  if (display::settingsPageThreeScrollPos <= 2) {
    if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 270 - scrollMargin && display::touch_y <= 300 - scrollMargin) { // disable PH warnings
      user::disablePhWarnings = !user::disablePhWarnings;
      device::settingsAdjusted = true;
      beep();
    }
  }
  if (display::settingsPageThreeScrollPos <= 3) {
    if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 320 - scrollMargin && display::touch_y <= 350 - scrollMargin) { // disable Co2 warnings
      user::disableCo2Warnings = !user::disableCo2Warnings;
      device::settingsAdjusted = true;
      beep();
    }
  }
  if (display::settingsPageThreeScrollPos <= 4) {
    if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 370 - scrollMargin && display::touch_y <= 400 - scrollMargin) { // disable water temp warnings
      user::disableWaterTempWarnings = !user::disableWaterTempWarnings;
      device::settingsAdjusted = true;
      beep();
    }
  }
  if (display::settingsPageThreeScrollPos <= 5) {
    if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 420 - scrollMargin && display::touch_y <= 450 - scrollMargin) { // disable height warnings
      user::disableWaterHeightWarnings = !user::disableWaterHeightWarnings;
      device::settingsAdjusted = true;
      beep();
    }
  }
  if (display::settingsPageThreeScrollPos >= 1 && display::settingsPageThreeScrollPos <= 6) {
    if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 470 - scrollMargin && display::touch_y <= 500 - scrollMargin) { // disable air temp warnings
      user::disableAirTempWarnings = !user::disableAirTempWarnings;
      device::settingsAdjusted = true;
      beep();
    }
  }
  if (display::settingsPageThreeScrollPos >= 2 && display::settingsPageThreeScrollPos <= 7) {
    if (display::touch_x >= 680 && display::touch_x <= 750 && display::touch_y >= 520 - scrollMargin && display::touch_y <= 550 - scrollMargin) { // disable humidity warnings
      user::disableHumidityWarnings = !user::disableHumidityWarnings;
      device::settingsAdjusted = true;
      beep();
    }
  }
}

void settingsFourPageTouched() {
  static unsigned long etapePageOneContinuePressed = 0;
  if (display::showSr04HeightCalibration) {
    if (display::touch_x >= 284 && display::touch_x <= 422 && display::touch_y >= 368 && display::touch_y <= 414) { // cancel setting the water empty height / volume
      beep();
      display::showSr04HeightCalibration = false;
      display::showingDialog = false;
      display::refreshPage = true;
      clearPage();
    }
    else if (display::touch_x >= 460 && display::touch_x <= 638 && display::touch_y >= 368 && display::touch_y <= 414) { // continue with setting the water empty height / volume
      beep();
      setTankDepth();
      display::showSr04HeightCalibration = false;
      display::showingDialog = false;
      display::refreshPage = true;
      clearPage();
    }
  }
  //
  else if (display::showEtapeCalibration == device::PAGE_ONE) {
    if (display::touch_x >= 284 && display::touch_x <= 422 && display::touch_y >= 368 && display::touch_y <= 414) { // cancel Etape Calibration
      beep();
      display::showEtapeCalibration = device::UNCHANGED;
      display::showingDialog = false;
      display::refreshPage = true;
      clearPage();
    }
    else if (display::touch_x >= 460 && display::touch_x <= 638 && display::touch_y >= 368 && display::touch_y <= 414) { // continue with Etape min reading Calibration
      beep();
      etapePageOneContinuePressed = millis() + 2000UL; // Stop the page 2 continue button being pressed by mistake after the page 1 is pressed
      setEtapeZeroVolumeResistance();
      display::showEtapeCalibration = device::PAGE_TWO;
      display::refreshPage = true;
    }
  }
  else if (display::showEtapeCalibration == device::PAGE_TWO && millis() >= etapePageOneContinuePressed) {
    if (display::touch_x >= 284 && display::touch_x <= 422 && display::touch_y >= 368 && display::touch_y <= 414) { // cancel Etape Calibration
      beep();
      display::showEtapeCalibration = device::UNCHANGED;
      sensor::temp_etapeZeroVolumeResistance = 0;
      display::showingDialog = false;
      display::refreshPage = true;
      clearPage();
    }
    else if (display::touch_x >= 460 && display::touch_x <= 638 && display::touch_y >= 368 && display::touch_y <= 414) { // continue with Etape max reading Calibration
      beep();
      setEtapeMaxVolumeResistance();
      display::showEtapeCalibration = device::UNCHANGED;
      display::showingDialog = false;
      display::refreshPage = true;
      clearPage();
    }
  }
  //
  else if (display::showTdsCalibration) {
    if (display::showCalErrorMessage) {
      if (display::touch_x >= 366 && display::touch_x <= 504 && display::touch_y >= 368 && display::touch_y <= 414) { // confirm error message
        display::showCalErrorMessage = false;
        display::refreshPage = true;
      }
    }
    else {
      if (display::touch_x >= 284 && display::touch_x <= 422 && display::touch_y >= 368 && display::touch_y <= 414) { // cancel Tds Calibration
        beep();
        display::calTdsPageScrollPos = 0;
        display::showTdsCalibration = false;
        display::showingDialog = false;
        display::refreshPage = true;
        clearPage();
      }
      else if (display::touch_x >= 460 && display::touch_x <= 638 && display::touch_y >= 368 && display::touch_y <= 414) { // continue with Tds Calibration
        beep();
        if (tdsCalibration()) {
          if (display::calTdsPageScrollPos < 1) {
            display::calTdsPageScrollPos++;
          }
          else {
            display::showTdsCalibration = false;
            display::showingDialog = false;
            sensor::ecKvalueLow = ec.getKvalueLow();
            sensor::ecKvalueHigh = ec.getKvalueHigh();
            clearPage();
          }
        }
        else {       
          display::showCalErrorMessage = true;  // show error message
        }
        display::refreshPage = true;      
      }
    }
  }
  //
  else if (display::showCo2Calibration) {
    if (display::touch_x >= 284 && display::touch_x <= 422 && display::touch_y >= 368 && display::touch_y <= 414) { // cancel calibrating the Co2 sensor
      beep();
      display::showCo2Calibration = false;
      display::showingDialog = false;
      display::refreshPage = true;
      clearPage();
    }
    else if (display::touch_x >= 460 && display::touch_x <= 638 && display::touch_y >= 368 && display::touch_y <= 414) { // continue with calibrating the Co2 sensor
      beep();
      display::refreshPage = true;
      calibrateCo2();
      display::showCo2Calibration = false;
      display::showingDialog = false;
    }
  }
  //
  else if (display::showPhCalibration) {
    if (display::showCalErrorMessage) {
      if (display::touch_x >= 366 && display::touch_x <= 504 && display::touch_y >= 368 && display::touch_y <= 414) { // confirm error message
        display::showCalErrorMessage = false;
        display::showingDialog = false;
        display::refreshPage = true;
      }
    }
    else {
      if (display::touch_x >= 284 && display::touch_x <= 422 && display::touch_y >= 368 && display::touch_y <= 414) { // cancel Ph Calibration
        beep();
        display::calPhPageScrollPos = 0;
        display::showPhCalibration = false;
        display::showingDialog = false;
        display::refreshPage = true;
        clearPage();
      }
      else if (display::touch_x >= 460 && display::touch_x <= 638 && display::touch_y >= 368 && display::touch_y <= 414) { // continue with Ph Calibration
        beep();
        if (phCallbration()) {
          if (display::calPhPageScrollPos < 1) {
            display::calPhPageScrollPos++;
          }
          else {
            display::showPhCalibration = false;
            display::showingDialog = false;
            sensor::phAcidicVoltage = ph.getAcidicVoltage();
            sensor::phNeutralVoltage = ph.getNeutralVoltage();
            clearPage();
          }
        }
        else {       
          display::showCalErrorMessage = true;  // show error message
        }
        display::refreshPage = true;      
      }
    }
  }
  //
  else if (display::showDoserCalibration) {
    int startPosition = display::calDoserPageScrollPos * 178;
    for (uint8_t n = display::calDoserPageScrollPos; n < display::calDoserPageScrollPos + 4; n++) {
      if (n == 0) {
        if (display::touch_x >= 105 - startPosition && display::touch_x <= 180 - startPosition && display::touch_y >= 370 && display::touch_y <= 410) // doser speed 1 down
          user::doserOneSpeed = adjustValue(user::doserOneSpeed, -1, 1, 255);
        else if (display::touch_x >= 190 - startPosition && display::touch_x <= 268 - startPosition && display::touch_y >= 370 && display::touch_y <= 410) // doser speed 1 up
          user::doserOneSpeed = adjustValue(user::doserOneSpeed, 1, 1, 255);
        else if (display::touch_x >= 120 - startPosition && display::touch_x <= 240 - startPosition && display::touch_y >= 310 && display::touch_y <= 350) // doser 1 prime
          if (!device::doserIsPriming[n] && millis() - device::primeTouchTime >= 1500UL)
            prime(1, pin::doserOne, user::doserOneSpeed);
          else if (millis() - device::primeTouchTime >= 1500UL)
            prime(1, pin::doserOne, 0);
      }
      else if (n == 1) {
        if (display::touch_x >= 278 - startPosition && display::touch_x <= 358 - startPosition && display::touch_y >= 370 && display::touch_y <= 410) // doser speed 2 down
          user::doserTwoSpeed = adjustValue(user::doserTwoSpeed, -1, 1, 255);
        else if (display::touch_x >= 368 - startPosition && display::touch_x <= 448 - startPosition && display::touch_y >= 370 && display::touch_y <= 410) // doser speed 2 up
          user::doserTwoSpeed = adjustValue(user::doserTwoSpeed, 1, 1, 255);
        else if (display::touch_x >= 293 - startPosition && display::touch_x <= 413 - startPosition && display::touch_y >= 310 && display::touch_y <= 350) // doser 2 prime
          if (!device::doserIsPriming[n] && millis() - device::primeTouchTime >= 1500UL)
            prime(2, pin::doserTwo, user::doserTwoSpeed);
          else if (millis() - device::primeTouchTime >= 1500UL)
            prime(2, pin::doserTwo, 0);
      }
      else if (n == 2) {
        if (display::touch_x >= 458 - startPosition && display::touch_x <= 538 - startPosition && display::touch_y >= 370 && display::touch_y <= 410) // doser speed 3 down
          user::doserThreeSpeed = adjustValue(user::doserThreeSpeed, -1, 1, 255);
        else if (display::touch_x >= 548 - startPosition && display::touch_x <= 625 - startPosition && display::touch_y >= 370 && display::touch_y <= 410) // doser speed 3 up
          user::doserThreeSpeed = adjustValue(user::doserThreeSpeed, 1, 1, 255);
        else if (display::touch_x >= 473 - startPosition && display::touch_x <= 593 - startPosition && display::touch_y >= 310 && display::touch_y <= 350) // doser 3 prime
          if (!device::doserIsPriming[n] && millis() - device::primeTouchTime >= 1500UL)
            prime(3, pin::doserThree, user::doserThreeSpeed);
          else if (millis() - device::primeTouchTime >= 1500UL)
            prime(3, pin::doserThree, 0);
      }
      else if (n == 3) {
        if (display::touch_x >= 635 - startPosition && display::touch_x <= 702 - startPosition && display::touch_y >= 370 && display::touch_y <= 410) // doser speed 4 down
          user::doserFourSpeed = adjustValue(user::doserFourSpeed, -1, 1, 255);
        else if (display::touch_x >= 712 - startPosition && display::touch_x <= 800 - startPosition && display::touch_y >= 370 && display::touch_y <= 410) // doser speed 4 up
          user::doserFourSpeed = adjustValue(user::doserFourSpeed, 1, 1, 255);
        else if (display::touch_x >= 650 - startPosition && display::touch_x <= 770 - startPosition && display::touch_y >= 310 && display::touch_y <= 350) // doser 4 prime
          if (!device::doserIsPriming[n] && millis() - device::primeTouchTime >= 1500UL)
            prime(4, pin::doserFour, user::doserFourSpeed);
          else if (millis() - device::primeTouchTime >= 1500UL)
            prime(4, pin::doserFour, 0);
      }
      else if (n == 4) {
        if (display::touch_x >= 813 - startPosition && display::touch_x <= 880 - startPosition && display::touch_y >= 370 && display::touch_y <= 410) // doser speed 5 down
          user::doserFiveSpeed = adjustValue(user::doserFiveSpeed, -1, 1, 255);
        else if (display::touch_x >= 890 - startPosition && display::touch_x <= 978 - startPosition && display::touch_y >= 370 && display::touch_y <= 410) // doser speed 5 up
          user::doserFiveSpeed = adjustValue(user::doserFiveSpeed, 1, 1, 255);
        else if (display::touch_x >= 828 - startPosition && display::touch_x <= 948 - startPosition && display::touch_y >= 310 && display::touch_y <= 350) // doser 5 prime
          if (!device::doserIsPriming[n] && millis() - device::primeTouchTime >= 1500UL)
            prime(5, pin::doserFive, user::doserFiveSpeed);
          else if (millis() - device::primeTouchTime >= 1500UL)
            prime(5, pin::doserFive, 0);
      }
      else if (n == 5) {
        if (display::touch_x >= 991 - startPosition && display::touch_x <= 1058 - startPosition && display::touch_y >= 370 && display::touch_y <= 410) // doser speed 6 down
          user::doserSixSpeed = adjustValue(user::doserSixSpeed, -1, 1, 255);
        else if (display::touch_x >= 1068 - startPosition && display::touch_x <= 1156 - startPosition && display::touch_y >= 370 && display::touch_y <= 410) // doser speed 6 up
          user::doserSixSpeed = adjustValue(user::doserSixSpeed, 1, 1, 255);
        else if (display::touch_x >= 1006 - startPosition && display::touch_x <= 1126 - startPosition && display::touch_y >= 310 && display::touch_y <= 350) // doser 6 prime
          if (!device::doserIsPriming[n] && millis() - device::primeTouchTime >= 1500UL)
            prime(6, pin::doserSix, user::doserSixSpeed);
          else if (millis() - device::primeTouchTime >= 1500UL)
            prime(6, pin::doserSix, 0);
      }
    }
    if (display::touch_x >= 400 && display::touch_x <= 500 && display::touch_y >= 410 && display::touch_y <= 460) { // exit
      beep();
      display::showDoserCalibration = false;
      display::showingDialog = false;
      display::refreshPage = true;
      clearPage();
      // save profile - speed could vary due to fluids viscosity and volatge so per profile save thus no auto save ?
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 22;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  //
  else if (display::touch_x >= 700 && display::touch_x <= 770 && display::touch_y >= 170 && display::touch_y <= 200) { // showSr04HeightCalibration
    if (user::heightSensor == user::SR04 || user::heightSensor == user::VL53L0X) {
      beep();
      display::showSr04HeightCalibration = true;
      display::showingDialog = true;
      display::refreshPage = true;
    }
    else {
      beep();
      display::showEtapeCalibration = device::PAGE_ONE;
      display::showingDialog = true;
      display::refreshPage = true;
      clearPage();
    }
  }
  else if (display::touch_x >= 700 && display::touch_x <= 770 && display::touch_y >= 220 && display::touch_y <= 250) { // set tds sensor calibration
    beep();
    display::showTdsCalibration = true;
    display::showingDialog = true;
    display::refreshPage = true;
  }
  else if (display::touch_x >= 700 && display::touch_x <= 770 && display::touch_y >= 270 && display::touch_y <= 300) { // set co2 sensor calibration
    beep();
    display::showCo2Calibration = true;
    display::showingDialog = true;
    display::refreshPage = true;
  }
  else if (display::touch_x >= 700 && display::touch_x <= 770 && display::touch_y >= 320 && display::touch_y <= 350) { // set ph sensor calibration
    beep();
    device::phCalStage = 0;
    display::showPhCalibration = true;
    display::showingDialog = true;
    display::refreshPage = true;
    clearPage();
  }
  else if (display::touch_x >= 700 && display::touch_x <= 770 && display::touch_y >= 370 && display::touch_y <= 400) { // showDoserCalibration
    beep();
    display::showDoserCalibration = true;
    display::showingDialog = true;
    display::refreshPage = true;
    clearPage();
  }
}

void ecTdsPageTouched() {
  if (user::convertToTds) {
    if (display::touch_x >= 210 && display::touch_x <= 260 && display::touch_y >= 400 && display::touch_y <= 450) {  // min target ppm down
      user::targetMinTds = adjustValue(user::targetMinTds, -1, 1, user::targetMaxTds - 1);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 310 && display::touch_x <= 360 && display::touch_y >= 400 && display::touch_y <= 450) {  // min target ppm up
      user::targetMinTds = adjustValue(user::targetMinTds, 1, 1, user::targetMaxTds - 1);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 580 && display::touch_x <= 630 && display::touch_y >= 400 && display::touch_y <= 450) {  // max target ppm down
      user::targetMaxTds = adjustValue(user::targetMaxTds, -1, user::targetMinTds + 1, 999);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 680 && display::touch_x <= 730 && display::touch_y >= 400 && display::touch_y <= 450) {  // max target ppm up
      user::targetMaxTds = adjustValue(user::targetMaxTds, 1, user::targetMinTds + 1, 999);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 0;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else {
    if (display::touch_x >= 210 && display::touch_x <= 260 && display::touch_y >= 400 && display::touch_y <= 450) {  // min target ec down
      user::targetMinEc = adjustValue(user::targetMinEc, -0.01, 0.1, user::targetMaxEc - 0.1);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 310 && display::touch_x <= 360 && display::touch_y >= 400 && display::touch_y <= 450) {  // min target ec up
      user::targetMinEc = adjustValue(user::targetMinEc, 0.01, 0.1, user::targetMaxEc - 0.1);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 580 && display::touch_x <= 630 && display::touch_y >= 400 && display::touch_y <= 450) {  // max target ec down
      user::targetMaxEc = adjustValue(user::targetMaxEc, -0.01, user::targetMinEc + 0.1, 99);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 680 && display::touch_x <= 730 && display::touch_y >= 400 && display::touch_y <= 450) {  // max target ec up
      user::targetMaxEc = adjustValue(user::targetMaxEc, 0.01, user::targetMinEc + 0.1, 99);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 1;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
}

void phPageTouched() {
  if (display::touch_x >= 210 && display::touch_x <= 260 && display::touch_y >= 400 && display::touch_y <= 450) { // min target ph down
    user::targetMinPh = adjustValue(user::targetMinPh, -0.01, 1, user::targetMaxPh - 0.1);
    device::settingsAdjusted = true;
  }
  else if (display::touch_x >= 310 && display::touch_x <= 360 && display::touch_y >= 400 && display::touch_y <= 450) { // min target ph up
    user::targetMinPh = adjustValue(user::targetMinPh, 0.01, 1,  user::targetMaxPh - 0.1);
    device::settingsAdjusted = true;
  }
  else if (display::touch_x >= 580 && display::touch_x <= 630 && display::touch_y >= 400 && display::touch_y <= 450) { // max target ph down
    user::targetMaxPh = adjustValue(user::targetMaxPh, -0.01, user::targetMinPh + 0.1, 14);
    device::settingsAdjusted = true;
  }
  else if (display::touch_x >= 680 && display::touch_x <= 730 && display::touch_y >= 400 && display::touch_y <= 450) { // max target ph up
    user::targetMaxPh = adjustValue(user::targetMaxPh, 0.01, user::targetMinPh + 0.1, 14);
    device::settingsAdjusted = true;
  }
  else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
    message::infoPos = 2;
    display::showingDialog = true;
    display::showInfoDialog = true;
    display::refreshPage = true;
    display::infoDialogDisplayTime = millis();
  }
}

void co2PageTouched() {
  if (display::co2PageScrollPos == 0) {
    if (display::touch_x >= 210 && display::touch_x <= 260 && display::touch_y >= 400 && display::touch_y <= 450) { // targetCo2 down
      user::targetCo2 = adjustValue(user::targetCo2, -1, 400, 5000);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 310 && display::touch_x <= 360 && display::touch_y >= 400 && display::touch_y <= 450) { // targetCo2 up
      user::targetCo2 = adjustValue(user::targetCo2, 1, 400, 5000);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 580 && display::touch_x <= 630 && display::touch_y >= 400 && display::touch_y <= 450) { // co2 tolerance down
      user::co2Offset = adjustValue(user::co2Offset, -1, 0, 100);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 680 && display::touch_x <= 730 && display::touch_y >= 400 && display::touch_y <= 450) { // co2 tolerance up
      user::co2Offset = adjustValue(user::co2Offset, 1, 0, 100);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 3;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else if (display::co2PageScrollPos == 1) {
    if (display::touch_x >= 145 && display::touch_x <= 210 && display::touch_y >= 340 && display::touch_y <= 420) { // roomLength down
      if (user::convertToInches)
        user::roomLengthInches = adjustValue(user::roomLengthInches, -1, 1, 9999);
      else
        user::roomLengthCm = adjustValue(user::roomLengthCm, -1, 1, 9999);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 225 && display::touch_x <= 290 && display::touch_y >= 340 && display::touch_y <= 420) { // roomLength up
      if (user::convertToInches)
        user::roomLengthInches = adjustValue(user::roomLengthInches, 1, 1, 9999);
      else
        user::roomLengthCm = adjustValue(user::roomLengthCm, 1, 1, 9999);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 350 && display::touch_x <= 415 && display::touch_y >= 340 && display::touch_y <= 420) { // roomWidth down
      if (user::convertToInches)
        user::roomWidthInches = adjustValue(user::roomWidthInches, -1, 1, 9999);
      else
        user::roomWidthCm = adjustValue(user::roomWidthCm, -1, 1, 9999);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 430 && display::touch_x <= 495 && display::touch_y >= 340 && display::touch_y <= 420) { // roomWidth up
      if (user::convertToInches)
        user::roomWidthInches = adjustValue(user::roomWidthInches, 1, 1, 9999);
      else
        user::roomWidthCm = adjustValue(user::roomWidthCm, 1, 1, 9999);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 560 && display::touch_x <= 625 && display::touch_y >= 340 && display::touch_y <= 420) { // roomHeight down
      if (user::convertToInches)
        user::roomHeightInches = adjustValue(user::roomHeightInches, -1, 1, 9999);
      else
        user::roomHeightCm = adjustValue(user::roomHeightCm, -1, 1, 9999);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 640 && display::touch_x <= 705 && display::touch_y >= 340 && display::touch_y <= 420) { // roomHeight up
      if (user::convertToInches)
        user::roomHeightInches = adjustValue(user::roomHeightInches, 1, 1, 9999);
      else
        user::roomHeightCm = adjustValue(user::roomHeightCm, 1, 1, 9999);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 4;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else if (display::co2PageScrollPos == 2) {
    if (display::touch_x >= 145 && display::touch_x <= 210 && display::touch_y >= 340 && display::touch_y <= 420) { // co2Flowrate down
      if (user::convertToInches)
        user::co2FlowrateFeet3 = adjustValue(user::co2FlowrateFeet3, -0.1, 0.1, 99);
      else
        user::co2FlowrateLtrs = adjustValue(user::co2FlowrateLtrs, -0.1, 0.1, 99);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 225 && display::touch_x <= 290 && display::touch_y >= 340 && display::touch_y <= 420) { // co2Flowrate up
      if (user::convertToInches)
        user::co2FlowrateFeet3 = adjustValue(user::co2FlowrateFeet3, 0.1, 0.1, 99);
      else
        user::co2FlowrateLtrs = adjustValue(user::co2FlowrateLtrs, 0.1, 0.1, 99);
      device::settingsAdjusted = true;
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
      device::settingsAdjusted = true;
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
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 560 && display::touch_x <= 625 && display::touch_y >= 340 && display::touch_y <= 720) { // disableFansTimer down
      user::disableFansTimer = adjustValue(user::disableFansTimer, -1, 0, 999);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 640 && display::touch_x <= 705 && display::touch_y >= 340 && display::touch_y <= 720) { // disableFansTimer up
      user::disableFansTimer = adjustValue(user::disableFansTimer, 1, 0, 999);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 5;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else {
    if (display::touch_x >= 140 && display::touch_x <= 290 && display::touch_y >= 310 && display::touch_y <= 350) { // Enable Co2 manual duration
      user::enableManualCo2Duration = !user::enableManualCo2Duration;
      device::settingsAdjusted = true;
      beep();
    }
    else if (display::touch_x >= 390 && display::touch_x <= 430 && display::touch_y >= 400 && display::touch_y <= 470 && user::enableManualCo2Duration) { // Manual Co2 duration time down
      user::manualCo2GasDuration = adjustValue(user::manualCo2GasDuration, -1, 1, 999);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 478 && display::touch_x <= 518 && display::touch_y >= 400 && display::touch_y <= 470 && user::enableManualCo2Duration) { // Manual Co2 duration time up
      user::manualCo2GasDuration = adjustValue(user::manualCo2GasDuration, 1, 1, 999);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 585 && display::touch_x <= 735 && display::touch_y >= 310 && display::touch_y <= 350) { // Disable Co2 control
      user::disableCo2Control = !user::disableCo2Control;
      device::settingsAdjusted = true;
      beep();
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 6;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
}

void waterPageTouched() {
  if (display::waterPageScrollPos == 0) {
    if (display::touch_x >= 210 && display::touch_x <= 260 && display::touch_y >= 400 && display::touch_y <= 450) { // min target water level down
      if (user::convertToInches)
        user::targetMinWaterHeightInches = adjustValue(user::targetMinWaterHeightInches, -1, 1, user::targetMaxWaterHeightInches - 1);
      else
        user::targetMinWaterHeight = adjustValue(user::targetMinWaterHeight, -1, 1, user::targetMaxWaterHeight - 1);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 310 && display::touch_x <= 360 && display::touch_y >= 400 && display::touch_y <= 450) { // min target water level up
      if (user::convertToInches)
        user::targetMinWaterHeightInches = adjustValue(user::targetMinWaterHeightInches, 1, 1, user::targetMaxWaterHeightInches - 1);
      else
        user::targetMinWaterHeight = adjustValue(user::targetMinWaterHeight, 1, 1, user::targetMaxWaterHeight - 1);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 580 && display::touch_x <= 630 && display::touch_y >= 400 && display::touch_y <= 450) { // max target water level down
      if (user::convertToInches)
        user::targetMaxWaterHeightInches = adjustValue(user::targetMaxWaterHeightInches, -1, user::targetMinWaterHeightInches + 1, 99);
      else
        user::targetMaxWaterHeight = adjustValue(user::targetMaxWaterHeight, -1, user::targetMinWaterHeight + 1, 999);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 680 && display::touch_x <= 730 && display::touch_y >= 400 && display::touch_y <= 450) { // max target water level up
      if (user::convertToInches)
        user::targetMaxWaterHeightInches = adjustValue(user::targetMaxWaterHeightInches, 1, user::targetMinWaterHeightInches + 1, 99);
      else
        user::targetMaxWaterHeight = adjustValue(user::targetMaxWaterHeight, 1, user::targetMinWaterHeight + 1, 999);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 7;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else if (display::waterPageScrollPos == 1) {
    if (display::touch_x >= 210 && display::touch_x <= 260 && display::touch_y >= 400 && display::touch_y <= 450) { // min target temperature down
      if (user::convertToF)
        user::targetMinWaterTempF = adjustValue(user::targetMinWaterTempF, -0.1, 1, user::targetMaxWaterTempF - 0.1);
      else
        user::targetMinWaterTemp = adjustValue(user::targetMinWaterTemp, -0.1, 1, user::targetMaxWaterTemp - 0.1);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 310 && display::touch_x <= 360 && display::touch_y >= 400 && display::touch_y <= 450) { // min target temperature up
      if (user::convertToF)
        user::targetMinWaterTempF = adjustValue(user::targetMinWaterTempF, 0.1, 1, user::targetMaxWaterTempF - 0.1);
      else
        user::targetMinWaterTemp = adjustValue(user::targetMinWaterTemp, 0.1, 1, user::targetMaxWaterTemp - 0.1);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 580 && display::touch_x <= 630 && display::touch_y >= 400 && display::touch_y <= 450) { // max target temperature down
      if (user::convertToF)
        user::targetMaxWaterTempF = adjustValue(user::targetMaxWaterTempF, -0.1, user::targetMinWaterTempF + 0.1, 210);
      else
        user::targetMaxWaterTemp = adjustValue(user::targetMaxWaterTemp, -0.1, user::targetMinWaterTemp + 0.1, 99);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 680 && display::touch_x <= 730 && display::touch_y >= 400 && display::touch_y <= 450) { // max target temperature up
      if (user::convertToF)
        user::targetMaxWaterTempF = adjustValue(user::targetMaxWaterTempF, 0.1, user::targetMinWaterTempF + 0.1, 210);
      else
        user::targetMaxWaterTemp = adjustValue(user::targetMaxWaterTemp, 0.1, user::targetMinWaterTemp + 0.1, 99);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 8;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else if (display::waterPageScrollPos == 2) {
    if (display::touch_x >= 230 && display::touch_x <= 280 && display::touch_y >= 400 && display::touch_y <= 450) { // water tank length down
      if (user::convertToInches)
        user::waterTankLengthInches = adjustValue(user::waterTankLengthInches, -1, 1, 999);
      else
        user::waterTankLength = adjustValue(user::waterTankLength, -1, 1, 999);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 310 && display::touch_x <= 360 && display::touch_y >= 400 && display::touch_y <= 450) { // water tank length up
      if (user::convertToInches)
        user::waterTankLengthInches = adjustValue(user::waterTankLengthInches, 1, 1, 999);
      else
        user::waterTankLength = adjustValue(user::waterTankLength, 1, 1, 999);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 585 && display::touch_x <= 635 && display::touch_y >= 400 && display::touch_y <= 450) { // water tank width down
      if (user::convertToInches)
        user::waterTankWidthInches = adjustValue(user::waterTankWidthInches, -1, 1, 999);
      else
        user::waterTankWidth = adjustValue(user::waterTankWidth, -1, 1, 999);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 665 && display::touch_x <= 715 && display::touch_y >= 400 && display::touch_y <= 450) { // water tank width up
      if (user::convertToInches)
        user::waterTankWidthInches = adjustValue(user::waterTankWidthInches, 1, 1, 999);
      else
        user::waterTankWidth = adjustValue(user::waterTankWidth, 1, 1, 999);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 9;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else if (display::waterPageScrollPos == 3) {
    uint16_t calanderX = 110;
    uint16_t calanderY = 205;
    for (uint8_t i = 1; i < 32; i++) {
      if (display::touch_x >= calanderX && display::touch_x <= calanderX + 54 && display::touch_y >= calanderY && display::touch_y <= calanderY + 40) {
        static unsigned long previousCalanderMillis;
        if (millis() - previousCalanderMillis >= 1000UL) {
          user::autoFillDays[i - 1] = !user::autoFillDays[i - 1];
          Serial.print(F("calander ")); Serial.print(i - 1); Serial.print(F(" = ")); Serial.println(user::autoFillDays[i - 1] ? F("true") : F("false"));
          beep();
          device::settingsAdjusted = true;
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
      device::settingsAdjusted = true;
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
      device::settingsAdjusted = true;
      beep();
    }
    else if (display::touch_x >= 565 && display::touch_x <= 720 && display::touch_y >= 320 && display::touch_y <= 364) { // disableDrainAndRefill
      user::disableDrainAndRefill = !user::disableDrainAndRefill;
      device::settingsAdjusted = true;
      beep();
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 10;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else if (display::waterPageScrollPos == 4) {
    int startPosition = display::refillDoserPageScrollPos * 178;
    for (uint8_t n = display::refillDoserPageScrollPos; n < display::refillDoserPageScrollPos + 4; n++) {
      if (n == 0) {
        if (display::touch_x >= (105 - startPosition) && display::touch_x <= (180 - startPosition) && display::touch_y >= 420 && display::touch_y <= 480) { // refill doser 1 down
          user::refillDoserOneMills = adjustValue(user::refillDoserOneMills, -1, 0, 999);
          device::settingsAdjusted = true;
        }
        else if (display::touch_x >= (190 - startPosition) && display::touch_x <= (268 - startPosition) && display::touch_y >= 420 && display::touch_y <= 480) { // refill doser 1 up
          user::refillDoserOneMills = adjustValue(user::refillDoserOneMills, 1, 0, 999);
          device::settingsAdjusted = true;
        }
      }
      else if (n == 1) {
        if (display::touch_x >= (278 - startPosition) && display::touch_x <= (358 - startPosition) && display::touch_y >= 420 && display::touch_y <= 480) { // refill doser 2 down
          user::refillDoserTwoMills = adjustValue(user::refillDoserTwoMills, -1, 0, 999);
          device::settingsAdjusted = true;
        }
        else if (display::touch_x >= (368 - startPosition) && display::touch_x <= (448 - startPosition) && display::touch_y >= 420 && display::touch_y <= 480) { // refill doser 2 up
          user::refillDoserTwoMills = adjustValue(user::refillDoserTwoMills, 1, 0, 999);
          device::settingsAdjusted = true;
        }
      }
      else if (n == 2) {
        if (display::touch_x >= (458 - startPosition) && display::touch_x <= (538 - startPosition) && display::touch_y >= 420 && display::touch_y <= 480) { // refill doser 3 down
          user::refillDoserThreeMills = adjustValue(user::refillDoserThreeMills, -1, 0, 999);
          device::settingsAdjusted = true;
        }
        else if (display::touch_x >= (548 - startPosition) && display::touch_x <= (625 - startPosition) && display::touch_y >= 420 && display::touch_y <= 480) { // refill doser 3 up
          user::refillDoserThreeMills = adjustValue(user::refillDoserThreeMills, 1, 0, 999);
          device::settingsAdjusted = true;
        }
      }
      else if (n == 3) {
        if (display::touch_x >= (635 - startPosition) && display::touch_x <= (702 - startPosition) && display::touch_y >= 420 && display::touch_y <= 480) { // refill doser 4 down
          user::refillDoserFourMills = adjustValue(user::refillDoserFourMills, -1, 0, 999);
          device::settingsAdjusted = true;
        }
        else if (display::touch_x >= (712 - startPosition) && display::touch_x <= (800 - startPosition) && display::touch_y >= 420 && display::touch_y <= 480) { // refill doser 4 up
          user::refillDoserFourMills = adjustValue(user::refillDoserFourMills, 1, 0, 999);
          device::settingsAdjusted = true;
        }
      }
      else if (n == 4) {
        if (display::touch_x >= (813 - startPosition) && display::touch_x <= (880 - startPosition) && display::touch_y >= 420 && display::touch_y <= 480) { // refill doser 5 down
          user::refillDoserFiveMills = adjustValue(user::refillDoserFiveMills, -1, 0, 999);
          device::settingsAdjusted = true;
        }
        else if (display::touch_x >= (890 - startPosition) && display::touch_x <= (978 - startPosition) && display::touch_y >= 420 && display::touch_y <= 480) { // refill doser 5 up
          user::refillDoserFiveMills = adjustValue(user::refillDoserFiveMills, 1, 0, 999);
          device::settingsAdjusted = true;
        }
      }
      else if (n == 5) {
        if (display::touch_x >= (991 - startPosition) && display::touch_x <= (1058 - startPosition) && display::touch_y >= 420 && display::touch_y <= 480) { // refill doser 6 down
          user::refillDoserSixMills = adjustValue(user::refillDoserSixMills, -1, 0, 999);
          device::settingsAdjusted = true;
        }
        else if (display::touch_x >= (1068 - startPosition) && display::touch_x <= (1156 - startPosition) && display::touch_y >= 420 && display::touch_y <= 480) { // refill doser 6 up
          user::refillDoserSixMills = adjustValue(user::refillDoserSixMills, 1, 0, 999);
          device::settingsAdjusted = true;
        }
      }
    }
    if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 11;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
}

void dosersPageTouched() {
  static unsigned long prevPrimeTouchTime = 0;
  int startPosition = display::doserPageScrollPos * 178;
  for (uint8_t n = display::doserPageScrollPos; n < display::doserPageScrollPos + 4; n++) {
    if (n == 0) {
      if (display::touch_x >= (105 - startPosition) && display::touch_x <= (180 - startPosition) && display::touch_y >= 310 && display::touch_y <= 410) { // doser 1 down
        user::doserOneMills = adjustValue(user::doserOneMills, -1, 0, 999);
        device::settingsAdjusted = true;
      }
      else if (display::touch_x >= (190 - startPosition) && display::touch_x <= (268 - startPosition) && display::touch_y >= 310 && display::touch_y <= 410) { // doser 1 up
        user::doserOneMills = adjustValue(user::doserOneMills, 1, 0, 999);
        device::settingsAdjusted = true;
      }
      else if (display::touch_x >= (130 - startPosition) && display::touch_x <= (250 - startPosition) && display::touch_y >= 420 && display::touch_y <= 470) { // doser 1 prime
        if (!device::doserIsPriming[n] && millis() - device::primeTouchTime >= 1500UL)
          prime(1, pin::doserOne, user::doserOneSpeed);
        else if (millis() - device::primeTouchTime >= 1500UL)
          prime(1, pin::doserOne, 0);
      }
      else if (display::touch_x >= (155 - startPosition) && display::touch_x <= (235 - startPosition) && display::touch_y >= 230 && display::touch_y <= 270) { // change doser 1 mode - OFF or EC or PH
        beep();
        if (user::doserOneMode == device::DOSER_OFF)
          user::doserOneMode = device::DOSER_EC;
        else if (user::doserOneMode == device::DOSER_EC)
          user::doserOneMode = device::DOSER_EC_OP;
        else if (user::doserOneMode == device::DOSER_EC_OP)
          user::doserOneMode = device::DOSER_PH_DOWN;
        else if (user::doserOneMode == device::DOSER_PH_DOWN)
          user::doserOneMode = device::DOSER_PH_UP;
        else if (user::doserOneMode == device::DOSER_PH_UP)
          user::doserOneMode = device::DOSER_OFF;
        device::settingsAdjusted = true;
      }
    }
    else if (n == 1) {
      if (display::touch_x >= (278 - startPosition) && display::touch_x <= (358 - startPosition) && display::touch_y >= 310 && display::touch_y <= 410) { // doser 2 down
        user::doserTwoMills = adjustValue(user::doserTwoMills, -1, 0, 999);
        device::settingsAdjusted = true;
      }
      else if (display::touch_x >= (368 - startPosition) && display::touch_x <= (448 - startPosition) && display::touch_y >= 310 && display::touch_y <= 410) { // doser 2 up
        user::doserTwoMills = adjustValue(user::doserTwoMills, 1, 0, 999);
        device::settingsAdjusted = true;
      }
      else if (display::touch_x >= (303 - startPosition) && display::touch_x <= (423 - startPosition) && display::touch_y >= 420 && display::touch_y <= 470) { // doser 2 prime       
        if (!device::doserIsPriming[n] && millis() - device::primeTouchTime >= 1500UL)
          prime(2, pin::doserTwo, user::doserTwoSpeed);
        else if (millis() - device::primeTouchTime >= 1500UL)
          prime(2, pin::doserTwo, 0);
      }
      else if (display::touch_x >= (335 - startPosition) && display::touch_x <= (415 - startPosition) && display::touch_y >= 230 && display::touch_y <= 270) { // change doser 2 mode - OFF or EC or PH
        beep();
        if (user::doserTwoMode == device::DOSER_OFF)
          user::doserTwoMode = device::DOSER_EC;
        else if (user::doserTwoMode == device::DOSER_EC)
          user::doserTwoMode = device::DOSER_EC_OP;
        else if (user::doserTwoMode == device::DOSER_EC_OP)
          user::doserTwoMode = device::DOSER_PH_DOWN;
        else if (user::doserTwoMode == device::DOSER_PH_DOWN)
          user::doserTwoMode = device::DOSER_PH_UP;
        else if (user::doserTwoMode == device::DOSER_PH_UP)
          user::doserTwoMode = device::DOSER_OFF;
        device::settingsAdjusted = true;
      }
    }
    else if (n == 2) {
      if (display::touch_x >= (458 - startPosition) && display::touch_x <= (538 - startPosition) && display::touch_y >= 310 && display::touch_y <= 410) { // doser 3 down
        user::doserThreeMills = adjustValue(user::doserThreeMills, -1, 0, 999);
        device::settingsAdjusted = true;
      }
      else if (display::touch_x >= (548 - startPosition) && display::touch_x <= (625 - startPosition) && display::touch_y >= 310 && display::touch_y <= 410) { // doser 3 up
        user::doserThreeMills = adjustValue(user::doserThreeMills, 1, 0, 999);
        device::settingsAdjusted = true;
      }
      else if (display::touch_x >= (483 - startPosition) && display::touch_x <= (603 - startPosition) && display::touch_y >= 420 && display::touch_y <= 470) { // doser 3 prime
        if (!device::doserIsPriming[n] && millis() - device::primeTouchTime >= 1500UL)
          prime(3, pin::doserThree, user::doserThreeSpeed);
        else if (millis() - device::primeTouchTime >= 1500UL)
          prime(3, pin::doserThree, 0);
      }
      else if (display::touch_x >= (500 - startPosition) && display::touch_x <= (580 - startPosition) && display::touch_y >= 230 && display::touch_y <= 270) { // change doser 3 mode - OFF or EC or PH
        beep();
        if (user::doserThreeMode == device::DOSER_OFF)
          user::doserThreeMode = device::DOSER_EC;
        else if (user::doserThreeMode == device::DOSER_EC)
          user::doserThreeMode = device::DOSER_EC_OP;
        else if (user::doserThreeMode == device::DOSER_EC_OP)
          user::doserThreeMode = device::DOSER_PH_DOWN;
        else if (user::doserThreeMode == device::DOSER_PH_DOWN)
          user::doserThreeMode = device::DOSER_PH_UP;
        else if (user::doserThreeMode == device::DOSER_PH_UP)
          user::doserThreeMode = device::DOSER_OFF;
        device::settingsAdjusted = true;
      }
    }
    else if (n == 3) {
      if (display::touch_x >= (635 - startPosition) && display::touch_x <= (702 - startPosition) && display::touch_y >= 310 && display::touch_y <= 410) { // doser 4 down
        user::doserFourMills = adjustValue(user::doserFourMills, -1, 0, 999);
        device::settingsAdjusted = true;
      }
      else if (display::touch_x >= (712 - startPosition) && display::touch_x <= (800 - startPosition) && display::touch_y >= 310 && display::touch_y <= 410) { // doser 4 up
        user::doserFourMills = adjustValue(user::doserFourMills, 1, 0, 999);
        device::settingsAdjusted = true;
      }
      else if (display::touch_x >= (660 - startPosition) && display::touch_x <= (780 - startPosition) && display::touch_y >= 420 && display::touch_y <= 470) { // doser 4 prime
        if (!device::doserIsPriming[n] && millis() - device::primeTouchTime >= 1500UL)
          prime(4, pin::doserFour, user::doserFourSpeed);
        else if (millis() - device::primeTouchTime >= 1500UL)
          prime(4, pin::doserFour, 0);
      }
      else if (display::touch_x >= (678 - startPosition) && display::touch_x <= (758 - startPosition) && display::touch_y >= 230 && display::touch_y <= 270) { // change doser 4 mode - OFF or EC or PH
        beep();
        if (user::doserFourMode == device::DOSER_OFF)
          user::doserFourMode = device::DOSER_EC;
        else if (user::doserFourMode == device::DOSER_EC)
          user::doserFourMode = device::DOSER_EC_OP;
        else if (user::doserFourMode == device::DOSER_EC_OP)
          user::doserFourMode = device::DOSER_PH_DOWN;
        else if (user::doserFourMode == device::DOSER_PH_DOWN)
          user::doserFourMode = device::DOSER_PH_UP;
        else if (user::doserFourMode == device::DOSER_PH_UP)
          user::doserFourMode = device::DOSER_OFF;
        device::settingsAdjusted = true;
      }
    }
    else if (n == 4) {
      if (display::touch_x >= (813 - startPosition) && display::touch_x <= (880 - startPosition) && display::touch_y >= 310 && display::touch_y <= 410) { // doser 5 down
        user::doserFiveMills = adjustValue(user::doserFiveMills, -1, 0, 999);
        device::settingsAdjusted = true;
      }
      else if (display::touch_x >= (890 - startPosition) && display::touch_x <= (978 - startPosition) && display::touch_y >= 310 && display::touch_y <= 410) { // doser 5 up
        user::doserFiveMills = adjustValue(user::doserFiveMills, 1, 0, 999);
        device::settingsAdjusted = true;
      }
      else if (display::touch_x >= (838 - startPosition) && display::touch_x <= (958 - startPosition) && display::touch_y >= 420 && display::touch_y <= 470) { // doser 5 prime
        if (!device::doserIsPriming[n] && millis() - device::primeTouchTime >= 1500UL)
          prime(5, pin::doserFive, user::doserFiveSpeed);
        else if (millis() - device::primeTouchTime >= 1500UL)
          prime(5, pin::doserFive, 0);
      }
      else if (display::touch_x >= (856 - startPosition) && display::touch_x <= (936 - startPosition) && display::touch_y >= 230 && display::touch_y <= 270) { // change doser 5 mode - OFF or EC or PH
        beep();
        if (user::doserFiveMode == device::DOSER_OFF)
          user::doserFiveMode = device::DOSER_EC;
        else if (user::doserFiveMode == device::DOSER_EC)
          user::doserFiveMode = device::DOSER_EC_OP;
        else if (user::doserFiveMode == device::DOSER_EC_OP)
          user::doserFiveMode = device::DOSER_PH_DOWN;
        else if (user::doserFiveMode == device::DOSER_PH_DOWN)
          user::doserFiveMode = device::DOSER_PH_UP;
        else if (user::doserFiveMode == device::DOSER_PH_UP)
          user::doserFiveMode = device::DOSER_OFF;
        device::settingsAdjusted = true;
      }
    }
    else if (n == 5) {
      if (display::touch_x >= (991 - startPosition) && display::touch_x <= (1058 - startPosition) && display::touch_y >= 310 && display::touch_y <= 410) { // doser 6 down
        user::doserSixMills = adjustValue(user::doserSixMills, -1, 0, 999);
        device::settingsAdjusted = true;
      }
      else if (display::touch_x >= (1068 - startPosition) && display::touch_x <= (1156 - startPosition) && display::touch_y >= 310 && display::touch_y <= 410) { // doser 6 up
        user::doserSixMills = adjustValue(user::doserSixMills, 1, 0, 999);
        device::settingsAdjusted = true;
      }
      else if (display::touch_x >= (1016 - startPosition) && display::touch_x <= (1136 - startPosition) && display::touch_y >= 420 && display::touch_y <= 470) { // doser 6 prime
        if (!device::doserIsPriming[n] && millis() - device::primeTouchTime >= 1500UL)
          prime(6, pin::doserSix, user::doserSixSpeed);
        else if (millis() - device::primeTouchTime >= 1500UL)
          prime(6, pin::doserSix, 0);
      }
      else if (display::touch_x >= (1034 - startPosition) && display::touch_x <= (1114 - startPosition) && display::touch_y >= 230 && display::touch_y <= 270) { // change doser 6 mode - OFF or EC or PH
        beep();
        if (user::doserSixMode == device::DOSER_OFF)
          user::doserSixMode = device::DOSER_EC;
        else if (user::doserSixMode == device::DOSER_EC)
          user::doserSixMode = device::DOSER_EC_OP;
        else if (user::doserSixMode == device::DOSER_EC_OP)
          user::doserSixMode = device::DOSER_PH_DOWN;
        else if (user::doserSixMode == device::DOSER_PH_DOWN)
          user::doserSixMode = device::DOSER_PH_UP;
        else if (user::doserSixMode == device::DOSER_PH_UP)
          user::doserSixMode = device::DOSER_OFF;
        device::settingsAdjusted = true;
      }
    }
  }
  if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
    message::infoPos = 12;
    display::showingDialog = true;
    display::showInfoDialog = true;
    display::refreshPage = true;
    display::infoDialogDisplayTime = millis();
  }
}

void timerPagedTouched() {
  if (display::timerPagePageScrollPos == 0) {
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
      device::settingsAdjusted = true;
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
      device::settingsAdjusted = true;
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
      device::settingsAdjusted = true;
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
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 570 && display::touch_x <= 629 && display::touch_y >= 230 && display::touch_y <= 270) { // light mode On
      beep();
      user::lightMode = 1;
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 630 && display::touch_x <= 689 && display::touch_y >= 230 && display::touch_y <= 270) { // light mode Auto
      beep();
      user::lightMode = 0;
      if (restartLightingTimer()) {
        Serial.println(F("Light auto on"));
        digitalWrite(pin::light, !device::relayOffState);
        device::lightOn = true;
        rtc.refresh();
        device::lightSwitchedOnHour = rtc.hour();
        device::lightSwitchedOnMin = rtc.minute();
      }
      else {
        Serial.println(F("Light auto off"));
        digitalWrite(pin::light, device::relayOffState);
        device::lightOn = false;
      }
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 690 && display::touch_x <= 750 && display::touch_y >= 230 && display::touch_y <= 270) { // light mode Off
      beep();
      user::lightMode = 2;
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 13;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else if (display::timerPagePageScrollPos == 1) {
    if (display::touch_x >= 260 && display::touch_x <= 310 && display::touch_y >= 330 && display::touch_y <= 400) { // timer down
      user::auxRelayOneTimer = adjustValue(user::auxRelayOneTimer, -1, 1, 1440);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 360 && display::touch_x <= 410 && display::touch_y >= 330 && display::touch_y <= 400) { // timer up
      user::auxRelayOneTimer = adjustValue(user::auxRelayOneTimer, 1, 1, 1440);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 530 && display::touch_x <= 590 && display::touch_y >= 230 && display::touch_y <= 270) { // Aux relay 1 mode On
      beep();
      user::auxRelayOneMode = 1;
      device::auxRelayOneOn = true;
      digitalWrite(pin::auxRelayOnePin, !device::relayOffState);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 590 && display::touch_x <= 650 && display::touch_y >= 230 && display::touch_y <= 270) { // Aux relay 1 mode Auto
      beep();
      digitalWrite(pin::auxRelayOnePin, !device::relayOffState);
      user::auxRelayOneMode = 0;
      device::auxRelayOneOn = true;
      device::auxRelayOneSwitchedOnDay = rtc.day();
      if (rtc.minute() < 59) {
        device::auxRelayOneSwitchedOnMin = rtc.minute() + 1;
        device::auxRelayOneSwitchedOnHour = rtc.hour();
      }
      else {
        device::auxRelayOneSwitchedOnMin = 0;
        rtc.hour() < 23 ? device::auxRelayOneSwitchedOnHour = rtc.hour()+1 : device::auxRelayOneSwitchedOnHour = 0;
      }
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 650 && display::touch_x <= 710 && display::touch_y >= 230 && display::touch_y <= 270) { // Aux relay 1 mode Off
      beep();
      user::auxRelayOneMode = 2;
      device::auxRelayOneOn = false;
      digitalWrite(pin::auxRelayOnePin, device::relayOffState);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 35;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else {
    if (display::touch_x >= 260 && display::touch_x <= 310 && display::touch_y >= 330 && display::touch_y <= 400) { // timer down
      user::auxRelayTwoTimer = adjustValue(user::auxRelayTwoTimer, -1, 1, 1440);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 360 && display::touch_x <= 410 && display::touch_y >= 330 && display::touch_y <= 400) { // timer up
      user::auxRelayTwoTimer = adjustValue(user::auxRelayTwoTimer, 1, 1, 1440);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 530 && display::touch_x <= 590 && display::touch_y >= 230 && display::touch_y <= 270) { // Aux relay 2 mode On
      beep();
      user::auxRelayTwoMode = 1;
      device::auxRelayTwoOn = true;
      digitalWrite(pin::auxRelayTwoPin, !device::relayOffState);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 590 && display::touch_x <= 650 && display::touch_y >= 230 && display::touch_y <= 270) { // Aux relay 2 mode Auto
      beep();
      digitalWrite(pin::auxRelayTwoPin, !device::relayOffState);
      user::auxRelayTwoMode = 0;
      device::auxRelayTwoOn = true;
      device::auxRelayTwoSwitchedOnDay = rtc.day();
      if (rtc.minute() < 59) {
        device::auxRelayTwoSwitchedOnMin = rtc.minute() + 1;
        device::auxRelayTwoSwitchedOnHour = rtc.hour();
      }
      else {
        device::auxRelayTwoSwitchedOnMin = 0;
        rtc.hour() < 23 ? device::auxRelayTwoSwitchedOnHour = rtc.hour()+1 : device::auxRelayTwoSwitchedOnHour = 0;
      }
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 650 && display::touch_x <= 710 && display::touch_y >= 230 && display::touch_y <= 270) { // Aux relay 2 mode Off
      beep();
      user::auxRelayTwoMode = 2;
      device::auxRelayTwoOn = false;
      digitalWrite(pin::auxRelayTwoPin, device::relayOffState);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 35;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
}

void fansPageTouched() {
  if (display::fansPage == 0) {
    if (user::fanOneFixedSpeed) {
      // fan one min speed down AND fan one max speed down
      if ((display::touch_x >= 190 && display::touch_x <= 250 && display::touch_y >= 350 && display::touch_y <= 420) || (display::touch_x >= 530 && display::touch_x <= 590 && display::touch_y >= 350 && display::touch_y <= 420))  {
        user::targetMinFanOneSpeed = adjustValue(user::targetMinFanOneSpeed, -1, 0, 100);
        user::targetMaxFanOneSpeed = adjustValue(user::targetMaxFanOneSpeed, -1, 0, 100);
        sendToSlave('Z', user::targetMinFanOneSpeed);
        device::settingsAdjusted = true;
      }
      // fan one min speed up AND fan one max speed up if either is presed
      else if ((display::touch_x >= 270 && display::touch_x <= 330 && display::touch_y >= 350 && display::touch_y <= 420) || (display::touch_x >= 610 && display::touch_x <= 670 && display::touch_y >= 350 && display::touch_y <= 420)) {
        user::targetMinFanOneSpeed = adjustValue(user::targetMinFanOneSpeed, 1, 0, 100);
        user::targetMaxFanOneSpeed = adjustValue(user::targetMaxFanOneSpeed, 1, 0, 100);
        sendToSlave('Z', user::targetMinFanOneSpeed);
        device::settingsAdjusted = true;
      }
    }
    else {
      if (display::touch_x >= 190 && display::touch_x <= 250 && display::touch_y >= 350 && display::touch_y <= 420) { // fan one min speed down
        user::targetMinFanOneSpeed = adjustValue(user::targetMinFanOneSpeed, -1, 0, 100);
        device::settingsAdjusted = true;
      }
      else if (display::touch_x >= 270 && display::touch_x <= 330 && display::touch_y >= 350 && display::touch_y <= 420) { // fan one min speed up
        user::targetMinFanOneSpeed = adjustValue(user::targetMinFanOneSpeed, 1, 0, user::targetMaxFanOneSpeed - 1);
        device::settingsAdjusted = true;
      }
      else if (display::touch_x >= 530 && display::touch_x <= 590 && display::touch_y >= 350 && display::touch_y <= 420) { // fan one max speed down
        user::targetMaxFanOneSpeed = adjustValue(user::targetMaxFanOneSpeed, -1, user::targetMinFanOneSpeed + 1, 100);
        device::settingsAdjusted = true;
      }
      else if (display::touch_x >= 610 && display::touch_x <= 670 && display::touch_y >= 350 && display::touch_y <= 420) { // fan one max speed up
        user::targetMaxFanOneSpeed = adjustValue(user::targetMaxFanOneSpeed, 1, 0, 100);
        device::settingsAdjusted = true;
      }
    }
    if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 14;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else if (display::fansPage == 1) {
    if (user::fanTwoFixedSpeed) {
      // fan two min speed down AND fan two max speed down if either is presed
      if ((display::touch_x >= 190 && display::touch_x <= 250 && display::touch_y >= 350 && display::touch_y <= 420) || (display::touch_x >= 530 && display::touch_x <= 590 && display::touch_y >= 350 && display::touch_y <= 420))  {
        user::targetMinFanTwoSpeed = adjustValue(user::targetMinFanTwoSpeed, -1, 0, 100);
        user::targetMaxFanTwoSpeed = adjustValue(user::targetMaxFanTwoSpeed, -1, 0, 100);
        sendToSlave('X', user::targetMinFanTwoSpeed);
        device::settingsAdjusted = true;
      }
      // fan two min speed up AND fan two max speed up if either is presed
      else if ((display::touch_x >= 270 && display::touch_x <= 330 && display::touch_y >= 350 && display::touch_y <= 420) || (display::touch_x >= 610 && display::touch_x <= 670 && display::touch_y >= 350 && display::touch_y <= 420)) {
        user::targetMinFanTwoSpeed = adjustValue(user::targetMinFanTwoSpeed, 1, 0, 100);
        user::targetMaxFanTwoSpeed = adjustValue(user::targetMaxFanTwoSpeed, 1, 0, 100);
        sendToSlave('X', user::targetMinFanTwoSpeed);
        device::settingsAdjusted = true;
      }
    }
    else {
      if (display::touch_x >= 190 && display::touch_x <= 250 && display::touch_y >= 350 && display::touch_y <= 420) { // fan two min speed down
        user::targetMinFanTwoSpeed = adjustValue(user::targetMinFanTwoSpeed, -1, 0, 100);
        device::settingsAdjusted = true;
      }
      else if (display::touch_x >= 270 && display::touch_x <= 330 && display::touch_y >= 350 && display::touch_y <= 420) { // fan two min speed up
        user::targetMinFanTwoSpeed = adjustValue(user::targetMinFanTwoSpeed, 1, 0, user::targetMaxFanTwoSpeed - 1);
        device::settingsAdjusted = true;
      }
      else if (display::touch_x >= 530 && display::touch_x <= 590 && display::touch_y >= 350 && display::touch_y <= 420) { // fan two max speed down
        user::targetMaxFanTwoSpeed = adjustValue(user::targetMaxFanTwoSpeed, -1, user::targetMinFanTwoSpeed + 1, 100);
        device::settingsAdjusted = true;
      }
      else if (display::touch_x >= 610 && display::touch_x <= 670 && display::touch_y >= 350 && display::touch_y <= 420) { // fan two max speed up
        user::targetMaxFanTwoSpeed = adjustValue(user::targetMaxFanTwoSpeed, 1, 0, 100);
        device::settingsAdjusted = true;
      }
    }
    if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 15;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else if (display::fansPage == 2) {
    if (display::touch_x >= 210 && display::touch_x <= 260 && display::touch_y >= 400 && display::touch_y <= 450) { // min target air temp down
      if (user::convertToF)
        user::targetMinAirTempF = adjustValue(user::targetMinAirTempF, -0.1, 1, 210);
      else
        user::targetMinAirTemp = adjustValue(user::targetMinAirTemp, -0.1, 1, 99);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 310 && display::touch_x <= 360 && display::touch_y >= 400 && display::touch_y <= 450) { // min target air temp up
      if (user::convertToF)
        user::targetMinAirTempF = adjustValue(user::targetMinAirTempF, 0.1, 1, user::targetMaxAirTempF - 1);
      else
        user::targetMinAirTemp = adjustValue(user::targetMinAirTemp, 0.1, 1, user::targetMaxAirTemp - 1);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 580 && display::touch_x <= 630 && display::touch_y >= 400 && display::touch_y <= 450) { // max target air temp down
      if (user::convertToF)
        user::targetMaxAirTempF = adjustValue(user::targetMaxAirTempF, -0.1, user::targetMinAirTempF + 1, 210);
      else
        user::targetMaxAirTemp = adjustValue(user::targetMaxAirTemp, -0.1, user::targetMinAirTemp + 1, 99);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 680 && display::touch_x <= 730 && display::touch_y >= 400 && display::touch_y <= 450) { // max target air temp up
      if (user::convertToF)
        user::targetMaxAirTempF = adjustValue(user::targetMaxAirTempF, 0.1, 0, 210);
      else
        user::targetMaxAirTemp = adjustValue(user::targetMaxAirTemp, 0.1, 0, 99);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 16;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else if (display::fansPage == 3) {
    if (display::touch_x >= 210 && display::touch_x <= 260 && display::touch_y >= 400 && display::touch_y <= 450) { // min target humidity down
      user::targetMinHumidity = adjustValue(user::targetMinHumidity, -0.1, 1, 100);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 310 && display::touch_x <= 360 && display::touch_y >= 400 && display::touch_y <= 450) { // min target humidity up
      user::targetMinHumidity = adjustValue(user::targetMinHumidity, 0.1, 1, user::targetMaxHumidity - 1);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 580 && display::touch_x <= 630 && display::touch_y >= 400 && display::touch_y <= 450) { // max target humidity down
      user::targetMaxHumidity = adjustValue(user::targetMaxHumidity, -0.1, user::targetMinHumidity + 1, 100);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 680 && display::touch_x <= 730 && display::touch_y >= 400 && display::touch_y <= 450) { // max target humidity up
      user::targetMaxHumidity = adjustValue(user::targetMaxHumidity, 0.1, 1, 100);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 17;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
  else if (display::fansPage == 4) {
    if (display::touch_x >= 640 && display::touch_x <= 790 && display::touch_y >= 180 && display::touch_y <= 220) { // disable/enable fansControlTemperature
      beep();
      device::settingsAdjusted = true;
      user::fansControlTemperature = !user::fansControlTemperature;
    }
    else if (display::touch_x >= 640 && display::touch_x <= 790 && display::touch_y >= 230 && display::touch_y <= 270) { // disable/enable fansControlHumidity
      beep();
      device::settingsAdjusted = true;
      user::fansControlHumidity = !user::fansControlHumidity;
    }
    else if (display::touch_x >= 640 && display::touch_x <= 790 && display::touch_y >= 280 && display::touch_y <= 320) { // disable/enable fanOneFixedSpeed
      beep();
      user::fanOneFixedSpeed = !user::fanOneFixedSpeed;
      if (user::fanOneFixedSpeed)
        user::targetMinFanOneSpeed = user::targetMaxFanOneSpeed;
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 640 && display::touch_x <= 790 && display::touch_y >= 330 && display::touch_y <= 370) { // disable/enable fanTwoFixedSpeed
      beep();
      user::fanTwoFixedSpeed = !user::fanTwoFixedSpeed;
      if (user::fanTwoFixedSpeed)
        user::targetMinFanTwoSpeed = user::targetMaxFanTwoSpeed;
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
      message::infoPos = 18;
      display::showingDialog = true;
      display::showInfoDialog = true;
      display::refreshPage = true;
      display::infoDialogDisplayTime = millis();
    }
  }
}

void warningsPageTouched() {
  if (display::warningsPage == 0) {
    if (display::touch_x >= 534 && display::touch_x <= 614 && display::touch_y >= 356 && display::touch_y <= 436) { // tds warning down
      user::tdsErrorMargin = adjustValue(user::tdsErrorMargin, -1, 0, 99);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 634 && display::touch_x <= 714 && display::touch_y >= 356 && display::touch_y <= 436) { // tds warning up
      user::tdsErrorMargin = adjustValue(user::tdsErrorMargin, 1, 0, 99);
      device::settingsAdjusted = true;
    }
  }
  else if (display::warningsPage == 1) {
    if (display::touch_x >= 534 && display::touch_x <= 614 && display::touch_y >= 356 && display::touch_y <= 436) { // ec warning down
      user::ecErrorMargin = adjustValue(user::ecErrorMargin, -0.1, 0, 6);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 634 && display::touch_x <= 714 && display::touch_y >= 356 && display::touch_y <= 436) { // ec warning up
      user::ecErrorMargin = adjustValue(user::ecErrorMargin, 0.1, 0, 6);
      device::settingsAdjusted = true;
    }
  }
  else if (display::warningsPage == 2) {
    if (display::touch_x >= 534 && display::touch_x <= 614 && display::touch_y >= 356 && display::touch_y <= 436) { // ph warning down
      user::phErrorMargin = adjustValue(user::phErrorMargin, -0.1, 0, 14);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 634 && display::touch_x <= 714 && display::touch_y >= 356 && display::touch_y <= 436) { // ph warning up
      user::phErrorMargin = adjustValue(user::phErrorMargin, 0.1, 0, 14);
      device::settingsAdjusted = true;
    }
  }
  else if (display::warningsPage == 3) {
    if (display::touch_x >= 534 && display::touch_x <= 614 && display::touch_y >= 356 && display::touch_y <= 436) { // co2 warning down
      user::co2ErrorMargin = adjustValue(user::co2ErrorMargin, -1, 0, 99);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 634 && display::touch_x <= 714 && display::touch_y >= 356 && display::touch_y <= 436) { // co2 warning up
      user::co2ErrorMargin = adjustValue(user::co2ErrorMargin, 1, 0, 99);
      device::settingsAdjusted = true;
    }
  }
  else if (display::warningsPage == 4) {
    if (display::touch_x >= 534 && display::touch_x <= 614 && display::touch_y >= 356 && display::touch_y <= 436) { // water temp warning ErrorMargin down
      if (user::convertToF)
        user::waterTempErrorMarginF = adjustValue(user::waterTempErrorMarginF, -0.1, 0, 99);
      else
        user::waterTempErrorMargin = adjustValue(user::waterTempErrorMargin, -0.1, 0, 99);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 634 && display::touch_x <= 714 && display::touch_y >= 356 && display::touch_y <= 436) { // water temp warning ErrorMargin up
      if (user::convertToF)
        user::waterTempErrorMarginF = adjustValue(user::waterTempErrorMarginF, 0.1, 0, 99);
      else
        user::waterTempErrorMargin = adjustValue(user::waterTempErrorMargin, 0.1, 0, 99);
      device::settingsAdjusted = true;
    }
  }
  else if (display::warningsPage == 5) {
    if (display::touch_x >= 534 && display::touch_x <= 614 && display::touch_y >= 356 && display::touch_y <= 436) { // water lvl warning down
      if (user::convertToInches)
        user::waterHeightErrorMarginInches = adjustValue(user::waterHeightErrorMarginInches, -0.1, 0, 99);
      else
        user::waterHeightErrorMargin = adjustValue(user::waterHeightErrorMargin, -0.1, 0, 99);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 634 && display::touch_x <= 714 && display::touch_y >= 356 && display::touch_y <= 436) { // water lvl warning up
      if (user::convertToInches)
        user::waterHeightErrorMarginInches = adjustValue(user::waterHeightErrorMarginInches, 0.1, 0, 99);
      else
        user::waterHeightErrorMargin = adjustValue(user::waterHeightErrorMargin, 0.1, 0, 99);
      device::settingsAdjusted = true;
    }
  }
  else if (display::warningsPage == 6) {
    if (display::touch_x >= 534 && display::touch_x <= 614 && display::touch_y >= 356 && display::touch_y <= 436) { // air temp warning ErrorMargin down
      if (user::convertToF)
        user::airTempErrorMarginF = adjustValue(user::airTempErrorMarginF, -0.1, 0, 99);
      else
        user::airTempErrorMargin = adjustValue(user::airTempErrorMargin, -0.1, 0, 99);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 634 && display::touch_x <= 714 && display::touch_y >= 356 && display::touch_y <= 436) { // air temp warning ErrorMargin up
      if (user::convertToF)
        user::airTempErrorMarginF = adjustValue(user::airTempErrorMarginF, 0.1, 0, 99);
      else
        user::airTempErrorMargin = adjustValue(user::airTempErrorMargin, 0.1, 0, 99);
      device::settingsAdjusted = true;
    }
  }
  else if (display::warningsPage == 7) {
    if (display::touch_x >= 534 && display::touch_x <= 614 && display::touch_y >= 356 && display::touch_y <= 436) { // humidity warning down
      user::humidityErrorMargin = adjustValue(user::humidityErrorMargin, -0.1, 0, 99);
      device::settingsAdjusted = true;
    }
    else if (display::touch_x >= 634 && display::touch_x <= 714 && display::touch_y >= 356 && display::touch_y <= 436) { // humidity warning up
      user::humidityErrorMargin = adjustValue(user::humidityErrorMargin, 0.1, 0, 99);
      device::settingsAdjusted = true;
    }
  }
  if (display::touch_x >= 770 && display::touch_x <= 800 && display::touch_y >= 120 && display::touch_y <= 140) { // show info dialog
    message::infoPos = 19;
    display::showingDialog = true;
    display::showInfoDialog = true;
    display::refreshPage = true;
    display::infoDialogDisplayTime = millis();
  }
}

// This function is in charge of all keys pressed within the keybaord.
void keyboardTouched(char* a_charPtr, const int a_arrayLen) {
  //Serial.println(F("keyboard touched"));
  unsigned int keyboardColumn = 135;
  unsigned int keyboardRow = 260;
  uint8_t characters;
  uint8_t numbers = '0';
  if (device::charType != device::LOWER)
    characters = 'A';
  else
    characters = 'a';
  //
  device::intputPosition = strlen(a_charPtr);
  if (device::intputPosition > a_arrayLen - 2)
    device::intputPosition = a_arrayLen - 2;
  //Serial.print(F("input pos: ")); Serial.println(strlen(charPtr));
  if (device::charType != device::SPECIAL) {
    // DOT
    if (display::touch_x >= keyboardColumn && display::touch_x <= keyboardColumn + 50 && display::touch_y >= keyboardRow - 60 && display::touch_y <= keyboardRow - 2) {
      //Serial.println(F("Touched dot"));
      beep();
      addCharToStr(a_charPtr, a_arrayLen, '.');
    }
    // 0-9
    keyboardColumn += 60;
    for (int i = 0; i < 10; i++) {
      if (display::touch_x >= keyboardColumn && display::touch_x <= keyboardColumn + 50 && display::touch_y >= keyboardRow - 60 && display::touch_y <= keyboardRow - 2) {
        //Serial.print(F("Touched num: ")); Serial.println((char)numbers);
        beep();
        addCharToStr(a_charPtr, a_arrayLen, numbers);
        break;
      }
      keyboardColumn += 60;
      numbers++;
    }
  }
  else {
    for (int i = 0; i < 11; i++) {
      if (display::touch_x >= keyboardColumn && display::touch_x <= keyboardColumn + 50 && display::touch_y >= keyboardRow - 60 && display::touch_y <= keyboardRow - 2) {
        //Serial.print(F("Touched special char: ")); Serial.println(device::specialSymbols[i]);
        beep();  
        addCharToStr(a_charPtr, a_arrayLen, device::specialSymbols[i]);
        break;
      }
      keyboardColumn += 60;
    }     
  }
  // a-z A-Z
  keyboardColumn = 135;
  keyboardRow = 260;
  for (int i = 0; i < 26; i++) {
    if (display::touch_x >= keyboardColumn && display::touch_x <= keyboardColumn + 50 && display::touch_y >= keyboardRow && display::touch_y <= keyboardRow + 58) {
      //Serial.print(F("Touched char: ")); Serial.println((char)characters);
      beep();
      addCharToStr(a_charPtr, a_arrayLen, characters);
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
    //Serial.println(F("Touched UnderScore"));
    beep();
    addCharToStr(a_charPtr, a_arrayLen, '_'); 
  }
  // DELETE CHAR
  else if (display::touch_x >= (keyboardColumn += 60) && display::touch_x <= keyboardColumn + 80 && display::touch_y >= keyboardRow && display::touch_y <= keyboardRow + 58) {
    //Serial.println(F("Touched del"));
    beep();
    if (device::intputPosition >= a_arrayLen - 2 && a_charPtr[device::intputPosition] != 0) {
      a_charPtr[device::intputPosition] = 0;
    }
    else {
      if (device::intputPosition > 0)
        a_charPtr[device::intputPosition - 1] = 0;
      a_charPtr[device::intputPosition] = 0;
      if (device::intputPosition > 0)
        device::intputPosition--;
    }
    device::updateKeyboardInput = true;
    //Serial.print(F("device::intputPosition = ")); Serial.println(device::intputPosition);
  }
  // HOME
  else if (display::touch_x >= (keyboardColumn += 90) && display::touch_x <= keyboardColumn + 120 && display::touch_y >= keyboardRow && display::touch_y <= keyboardRow + 58) {
    beep();
    //Serial.println(F("Touched keybaord home"));
    device::updateKeyboardInput = false;
    display::showKeyboard = false;
    device::keyBoardClosedTime = millis();
    device::lockSaveButtons = true;
    Serial.println(F("keyboard home"));
    clearPage();
    display::refreshPage = true;
  }
  // LOWER CASE - UPPER - SPECIAL
  else if (display::touch_x >= (keyboardColumn += 130) && display::touch_x <= keyboardColumn + 120 && display::touch_y >= keyboardRow && display::touch_y <= keyboardRow + 58) {
    static unsigned long previousMillis = 0;
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 4000UL) { // time it takes for keyboard to display plus a healthy margin just in case
      //Serial.println(F("Touched caps"));
      beep();
      display::refreshPage = true;
      if (device::charType == device::LOWER)
        device::charType = device::UPPER;
      else if (device::charType == device::UPPER)
        device::charType = device::SPECIAL;
      else
        device::charType = device::LOWER;
      previousMillis = currentMillis;
    }
  }
  //Serial.print(F("device::charPtr = ")); Serial.println(charPtr);
}
