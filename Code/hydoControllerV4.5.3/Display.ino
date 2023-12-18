// Draw the given page
void drawPages() {
  if (display::page == 0)
    homePage();
  else if (display::page == 1)
    graphsPage();
  else if (display::page == 2)
    maxminsPage();
  else if (display::page == 3)
    profilesPage();
  else if (display::page == 4)
    settingsPage();
  else if (display::page == 5)
    user::convertToTds ? tdsPage() : ecPage();
  else if (display::page == 6)
    phPage();
  else if (display::page == 7)
    co2Page();
  else if (display::page == 8)
    waterPage();
  else if (display::page == 9)
    dosersPage();
  else if (display::page == 10)
    lightingPage();
  else if (display::page == 11)
    fansPage();
  else if (display::page == 12)
    warningsPage();
  display::refreshPage = false;
}
// =============================================================================================================================================================================================
// =============================================================================================================================================================================================
// Even though the pages are only called from one place
// it is much easier to seperate each page into its
// own function rather than combing thousands of lines.
void homePage() {
  if (millis() - device::slideShowpreviousMillis >= 10000UL) {
    display::homePage < 9 ? display::homePage++ : display::homePage = 0;
    device::slideShowpreviousMillis = millis();
    clearPage();
    display::refreshPage = true;
  }
  //
  if (display::refreshPage) {
    // Draw slideShow icons
    drawSlideIcons(110, 112, display::homePage, 10);
    // print the title
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK);
    if (display::homePage == 0) {
      tft.print(140, 140, F("TDS sensor"));
    }
    else if (display::homePage == 1) {
      tft.print(160, 140, F("EC sensor"));
    }
    else if (display::homePage == 2) {
      tft.print(160, 140, F("PH sensor"));
    }
    else if (display::homePage == 3) {
      tft.print(140, 140, F("Co2 sensor"));
    }
    else if (display::homePage == 4) {
      tft.print(130, 140, F("Water temp"));
      tft.setCursor(tft.getFontX() + 6, 130);
      !user::convertToF ? tft.print(F("c")) : tft.print(F("f"));
    }
    else if (display::homePage == 5) {
      !user::convertToInches ? tft.setCursor(100, 140) : tft.setCursor(120, 140);
      tft.print(F("Water height"));
      tft.setCursor(tft.getFontX() + 6, 140);
      !user::convertToInches ? tft.print(F("cm")) : tft.print(F("\""));
    }
    else if (display::homePage == 6) {
      tft.print(110, 140, F("Fan one speed"));
    }
    else if (display::homePage == 7) {
      tft.print(110, 140, F("Fan two speed"));
    }
    else if (display::homePage == 8) {
      tft.print(170, 140, F("Air temp"));
      tft.setCursor(tft.getFontX() + 6, 130);
      !user::convertToF ? tft.print(F("c")) : tft.print(F("f"));
    }
    else if (display::homePage == 9) {
      tft.print(180, 140, F("Humidity"));
    }
  }
  // Wifi symbol
  static byte previousConnectionState = wifi::UNCONNECTED;
  if (wifi::connectionState != previousConnectionState) {
    tft.fillRect(540, 90, 40, 40, user::backgroundColor);
    if (wifi::connectionState == wifi::CONNECTED) {
      tft.drawXbmpArray(miniWifiIcon, 205, 540, 90, 40, 40, RA8875_GREEN, 1);
    }
    else if (wifi::connectionState == wifi::TIMEOUT) {
      tft.drawXbmpArray(miniWifiIcon, 205, 540, 90, 40, 40, RA8875_YELLOW, 1);
    }
    else if (wifi::connectionState == wifi::FAILED) {
      tft.drawXbmpArray(miniWifiIcon, 205, 540, 90, 40, 40, RA8875_RED, 1);
    }
    previousConnectionState = wifi::connectionState;
  }
  // RTC clock
  static byte previousRTCMinute;
  rtcTime = rtc.getTime();
  if (display::refreshPage || rtcTime.min != previousRTCMinute) {
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK);
    tft.fillRect(594, 92, 202, 34, user::backgroundColor);
    tft.print(596, 92, rtc.getTimeStr(FORMAT_SHORT));
    rtcTime.hour <= 12 ? tft.print(F(" AM")) : tft.print(F(" PM"));
    previousRTCMinute = rtcTime.min;
  }
  // draw values and graph
  if (display::homePage == 0) {
    drawSensorSlide(sensor::tds, user::targetMinTds, user::targetMaxTds, device::tdsArray, 0);
  }
  else if (display::homePage == 1) {
    drawSensorSlide(sensor::ec, user::targetMinEc, user::targetMaxEc, device::ecArray, 2);
  }
  else if (display::homePage == 2) {
    drawSensorSlide(sensor::ph, user::targetMinPh, user::targetMaxPh, device::phArray, 2);
  }
  else if (display::homePage == 3) {
    drawSensorSlide(sensor::co2, user::targetCo2, user::targetCo2, device::co2Array, 0);
  }
  else if (display::homePage == 4) {
    if (user::convertToF)
      drawSensorSlide(sensor::waterTempF, user::targetMinWaterTempF, user::targetMaxWaterTempF, device::waterTemperatureArrayF, 1);
    else
      drawSensorSlide(sensor::waterTemp, user::targetMinWaterTemp, user::targetMaxWaterTemp, device::waterTemperatureArray, 1);
  }
  else if (display::homePage == 5) {
    if (user::convertToInches)
      drawSensorSlide(sensor::waterLevelInches, user::targetMinWaterHeightInches, user::targetMaxWaterHeightInches, device::waterLevelArrayInInches, 1);
    else
      drawSensorSlide(sensor::waterLevel, user::targetMinWaterHeight, user::targetMaxWaterHeight, device::waterLevelArray, 1);
  }
  else if (display::homePage == 6) {
    drawSensorSlide(device::fanOneSpeed, user::targetMinFanOneSpeed, user::targetMaxFanOneSpeed, device::fanOneSpeedArray, 0);
  }
  else if (display::homePage == 7) {
    drawSensorSlide(device::fanTwoSpeed, user::targetMinFanTwoSpeed, user::targetMaxFanTwoSpeed, device::fanTwoSpeedArray, 0);
  }
  else if (display::homePage == 8) {
    if (user::convertToF)
      drawSensorSlide(convertToF(sensor::airTemp), user::targetMinAirTempF, user::targetMaxAirTempF, device::airTemperatureArrayF, 1);
    else
      drawSensorSlide(sensor::airTemp, user::targetMinAirTemp, user::targetMaxAirTemp, device::airTemperatureArray, 1);
  }
  else if (display::homePage == 9) {
    drawSensorSlide(sensor::humidity, user::targetMinHumidity, user::targetMaxHumidity, device::humidityArray, 1);
  }
}
// =============================================================================================================================================================================================
// =============================================================================================================================================================================================
void graphsPage() {
  if (display::refreshPage || device::newGraphData) {
    if (device::newGraphData) {
      device::newGraphData = false;
      clearPage();
    }
    drawGraphIcons();
    if (display::graphOption == 1) {
      drawGraph(100, 100, 350, tft.width() - 40, 10, true, device::tdsArray, device::graphArrayPos, 0);
    }
    else if (display::graphOption == 2) {
      drawGraph(100, 100, 350, tft.width() - 40, 10, true, device::phArray, device::graphArrayPos, 2);
    }
    else if (display::graphOption == 3) {
      drawGraph(100, 100, 350, tft.width() - 40, 10, true, device::ecArray, device::graphArrayPos, 2);
    }
    else if (display::graphOption == 4) {
      drawGraph(100, 100, 350, tft.width() - 40, 10, true, device::co2Array, device::graphArrayPos, 0);
    }
    else if (display::graphOption == 5) {
      if (user::convertToF)
        drawGraph(100, 100, 350, tft.width() - 40, 10, true, device::waterTemperatureArrayF, device::graphArrayPos, 1);
      else
        drawGraph(100, 100, 350, tft.width() - 40, 10, true, device::waterTemperatureArray, device::graphArrayPos, 1);
    }
    else if (display::graphOption == 6) {
      if (user::convertToInches)
        drawGraph(100, 100, 350, tft.width() - 40, 10, true, device::waterLevelArrayInInches, device::graphArrayPos, 1);
      else
        drawGraph(100, 100, 350, tft.width() - 40, 10, true, device::waterLevelArray, device::graphArrayPos, 1);
    }
    else if (display::graphOption == 7) {
      drawGraph(100, 100, 350, tft.width() - 40, 10, true, device::fanOneSpeedArray, device::graphArrayPos, 0);
    }
    else if (display::graphOption == 8) {
      drawGraph(100, 100, 350, tft.width() - 40, 10, true, device::fanTwoSpeedArray, device::graphArrayPos, 0);
    }
    else if (display::graphOption == 9) {
      if (user::convertToF)
        drawGraph(100, 100, 350, tft.width() - 40, 10, true, device::airTemperatureArrayF, device::graphArrayPos, 1);
      else
        drawGraph(100, 100, 350, tft.width() - 40, 10, true, device::airTemperatureArray, device::graphArrayPos, 1);
    }
    else if (display::graphOption == 10) {
      drawGraph(100, 100, 350, tft.width() - 40, 10, true, device::humidityArray, device::graphArrayPos, 1);
    }
  }
}
// =============================================================================================================================================================================================
// =============================================================================================================================================================================================
void maxminsPage() {
  unsigned int color = 0;
  unsigned int color2 = 0;
  if (display::refreshPage) {
    drawSlideIcons(110, 112, display::maxMinsPage, 10);
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK);
    tft.print(176, 170, F("Minimum"));
    tft.print(530, 170, F("Maximum"));
  }
  if (display::maxMinsPage == 0) {
    if (display::refreshPage)
      tft.print(330, 126, F("PPM history"));
    drawTwoValues(285, device::minTds, RA8875_BLACK, 0, 650, device::maxTds, RA8875_BLACK, 0, 0);
  }
  else if (display::maxMinsPage == 1) {
    if (display::refreshPage)
      tft.print(340, 126, F("EC history"));
    drawTwoValues(285, device::minEc, RA8875_BLACK, 2, 650, device::maxEc, RA8875_BLACK, 2, 0);
  }
  else if (display::maxMinsPage == 2) {
    if (display::refreshPage)
      tft.print(340, 126, F("PH history"));
    drawTwoValues(285, device::minPh, RA8875_BLACK, 2, 650, device::maxPh, RA8875_BLACK, 2, 0);
  }
  else if (display::maxMinsPage == 3) {
    if (display::refreshPage)
      tft.print(330, 126, F("Co2 history"));
    drawTwoValues(285, device::minCo2, RA8875_BLACK, 0, 650, device::maxCo2, RA8875_BLACK, 0, 0);
  }
  else if (display::maxMinsPage == 4) {
    if (display::refreshPage)
      tft.print(250, 126, F("Water temp history"));
    if (user::convertToF)
      drawTwoValues(285, convertToF(device::minWaterTemp), RA8875_BLACK, 1, 650, convertToF(device::maxWaterTemp), RA8875_BLACK, 1, 2);
    else
      drawTwoValues(285, device::minWaterTemp, RA8875_BLACK, 1, 650, device::maxWaterTemp, RA8875_BLACK, 1, 1);
  }
  else if (display::maxMinsPage == 5) {
    if (display::refreshPage)
      tft.print(234, 126, F("Water height history"));
    if (user::convertToInches) {
      drawTwoValues(285, convertToInch(device::minWaterLevel), RA8875_BLACK, 1, 650, convertToInch(device::maxWaterLevel), RA8875_BLACK, 1, 4);
    }
    else {
      drawTwoValues(285, device::minWaterLevel, RA8875_BLACK, 1, 650, device::maxWaterLevel, RA8875_BLACK, 1, 3);
    }
  }
  else if (display::maxMinsPage == 6) {
    if (display::refreshPage)
      tft.print(214, 126, F("Fan one speed history"));
    drawTwoValues(285, device::minFanOneSpeed, RA8875_BLACK, 0, 650, device::maxFanOneSpeed, RA8875_BLACK, 0, 5);
  }
  else if (display::maxMinsPage == 7) {
    if (display::refreshPage)
      tft.print(214, 126, F("Fan two speed history"));
    drawTwoValues(285, device::minFanTwoSpeed, RA8875_BLACK, 0, 650, device::maxFanTwoSpeed, RA8875_BLACK, 0, 5);
  }
  else if (display::maxMinsPage == 8) {
    if (display::refreshPage)
      tft.print(286, 126, F("Air temp history"));
    if (user::convertToF)
      drawTwoValues(285, convertToF(device::minAirTemp), RA8875_BLACK, 1, 650, convertToF(device::maxAirTemp), RA8875_BLACK, 1, 2);
    else
      drawTwoValues(285, device::minAirTemp, RA8875_BLACK, 1, 650, device::maxAirTemp, RA8875_BLACK, 1, 1);
  }
  else if (display::maxMinsPage == 9) {
    if (display::refreshPage)
      tft.print(282, 126, F("Humidity history"));
    drawTwoValues(285, device::minHumidity, color, 1, 650, device::maxHumidity, color2, 1, 5);
  }
}
// =============================================================================================================================================================================================
// =============================================================================================================================================================================================
void profilesPage() {
  if (display::refreshPage && display::showSaveDialog && !display::showKeyboard) {
    continueMessage(message::saveProfile, device::userProfile + 1, 0, true, true, false);
  }
  else if (display::refreshPage && !display::showKeyboard) {
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK);
    tft.print(200, 100, F("Save and load profiles"));
    // Input box and buttons for profile 1
    inputBox(130, 160, user::profileOneName);
    saveButton(493, 160);
    loadButton(631, 160);
    // Input box and buttons for profile 2
    inputBox(130, 222, user::profileTwoName);
    saveButton(493, 222);
    loadButton(631, 222);
    // Input box and buttons for profile 3
    inputBox(130, 284, user::profileThreeName);
    saveButton(493, 284);
    loadButton(631, 284);
    // Input box and buttons for profile 4
    inputBox(130, 346, user::profileFourName);
    saveButton(493, 346);
    loadButton(631, 346);
    // Input box and buttons for profile 5
    inputBox(130, 408, user::profileFiveName);
    saveButton(493, 408);
    loadButton(631, 408);
    const int position = 160 + (device::userProfile * 62);
    tft.drawRect(125, position - 5, 350, 51, RA8875_BLUE);
    tft.drawRect(126, position - 4, 348, 49, RA8875_BLUE);
    tft.drawRect(127, position - 3, 346, 47, RA8875_BLUE);
  }
  else if (display::refreshPage && display::showKeyboard) {
    // draw the keybaord
    keyBoard();
  }
  if ((display::refreshPage && display::showKeyboard) || (display::showKeyboard && device::updateKeyboardInput)) {
    // draw input box
    if (device::profileInputNumber == 1)
      keyBoardInput(user::profileOneName);
    else if (device::profileInputNumber == 2)
      keyBoardInput(user::profileTwoName);
    else if (device::profileInputNumber == 3)
      keyBoardInput(user::profileThreeName);
    else if (device::profileInputNumber == 4)
      keyBoardInput(user::profileFourName);
    else if (device::profileInputNumber == 5)
      keyBoardInput(user::profileFiveName);
    device::updateKeyboardInput = false;
  }
  if (display::showKeyboard)
    showCursor();
}
// =============================================================================================================================================================================================
// =============================================================================================================================================================================================
void settingsPage() {
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(1);
  // Draw page navigation buttons
  if (display::refreshPage && !user::showSr04HeightCalibration && !user::showDoserCalibration && !display::showSystemLogs
      && !user::showTdsCalibration && !user::showCo2Calibration && !user::showPhCalibration && !user::setRTCtime)
  {
    drawSlideIcons(110, 112, display::settingsPage, 4);
  }
  // settings slider pages
  if (display::settingsPage == 0)
    drawSettingsPageZero();
  else if (display::settingsPage == 1)
    drawSettingsPageOne();
  else if (display::settingsPage == 2)
    drawSettingsPageTwo();
  else if (display::settingsPage == 3)
    drawSettingsPageThree();
}
// =============================================================================================================================================================================================
// =============================================================================================================================================================================================
void tdsPage() {
  if (display::refreshPage) {
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK);
    tft.print(300, 116, F("Target PPM"));
    tft.print(176, 170, F("Minimum"));
    tft.print(530, 170, F("Maximum"));
    drawUpDownButtons(230, 400, 310, 400, RA8875_BLUE);
    drawUpDownButtons(585, 400, 665, 400, RA8875_BLUE);
  }
  drawTwoValues(285, user::targetMinTds, RA8875_BLACK, 0, 650, user::targetMaxTds, RA8875_BLACK, 0, 0);
}
// =============================================================================================================================================================================================
// =============================================================================================================================================================================================
void phPage() {
  if (display::refreshPage) {
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK);
    tft.print(326, 116, F("Target PH"));
    tft.print(176, 170, F("Minimum"));
    tft.print(530, 170, F("Maximum"));
    drawUpDownButtons(230, 400, 310, 400, RA8875_BLUE);
    drawUpDownButtons(585, 400, 665, 400, RA8875_BLUE);
  }
  drawTwoValues(285, user::targetMinPh, RA8875_BLACK, 2, 650, user::targetMaxPh, RA8875_BLACK, 2, 0);
}
// =============================================================================================================================================================================================
// =============================================================================================================================================================================================
void ecPage() {
  if (display::refreshPage) {
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK);
    tft.print(326, 116, F("Target EC"));
    tft.print(176, 170, F("Minimum"));
    tft.print(530, 170, F("Maximum"));
    drawUpDownButtons(230, 400, 310, 400, RA8875_BLUE);
    drawUpDownButtons(585, 400, 665, 400, RA8875_BLUE);
  }
  drawTwoValues(285, user::targetMinEc, RA8875_BLACK, 2, 650, user::targetMaxEc, RA8875_BLACK, 2, 0);
}
// =============================================================================================================================================================================================
// =============================================================================================================================================================================================
void co2Page() {
  drawSlideIcons(110, 112, display::co2PageScrollPos, 4);
  if (display::co2PageScrollPos == 0) {
    if (display::refreshPage) {
      tft.setFont(&akashi_36px_Regular);
      tft.setFontScale(1);
      tft.setTextColor(RA8875_BLACK);
      tft.print(330, 116, F("Target Co2"));
      tft.print(156, 166, F("Target PPM"));
      tft.print(530, 166, F("Tolerance"));
      drawUpDownButtons(230, 400, 310, 400, RA8875_BLUE);
      drawUpDownButtons(585, 400, 665, 400, RA8875_BLUE);
    }
    drawTwoValues(285, user::targetCo2, RA8875_BLACK, 0, 650, user::co2Offset, RA8875_BLACK, 0, 0);
  }
  else if (display::co2PageScrollPos == 1) {
    if (display::refreshPage) {
      tft.setFont(&akashi_36px_Regular);
      tft.setFontScale(1);
      tft.setTextColor(RA8875_BLACK);
      tft.print(330, 116, F("Dimensions"));
      tft.print(160, 166, F("Length"));
      tft.print(380, 166, F("Width"));
      tft.print(590, 166, F("Height"));
      drawThreeUpDownButtons();
    }
    tft.setTextColor(RA8875_BLACK);
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setFontScale(1);
    // ROOM LENGTH
    static int roomLengthStartPos = 0, roomLengthEndPos = 0;
    static float previousRoomLength;
    float roomLength = user::convertToInches ? user::roomLengthInches : user::roomLengthCm;
    if (display::refreshPage || roomLength != previousRoomLength) {
      tft.fillRect(roomLengthStartPos, 236, roomLengthEndPos - roomLengthStartPos, 50, user::backgroundColor);
      if (user::convertToInches) {
        roomLengthStartPos = 225 - ((tft.getStringWidth(roomLength, 1) / 2) + 8);
        tft.print(roomLengthStartPos, 240, roomLength, 1);
        tft.setFont(&akashi_36px_Regular);
        tft.setFontScale(1);
        tft.print(tft.getFontX() + 6, 230, F("\""));
      }
      else {
        roomLengthStartPos = 225 - ((tft.getStringWidth(roomLength, 0) / 2) + 23);
        tft.print(roomLengthStartPos, 240, roomLength, 0);
        tft.setFont(&akashi_36px_Regular);
        tft.setFontScale(1);
        tft.print(tft.getFontX() + 6, 250, F("cm"));
      }
      previousRoomLength = roomLength;
      roomLengthEndPos = tft.getFontX();
    }
    // ROOM WIDTH
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setFontScale(1);
    static int roomWidthStartPos = 0, roomWidthEndPos = 0;
    static float previousRoomWidth;
    float roomWidth = user::convertToInches ? user::roomWidthInches : user::roomWidthCm;
    if (display::refreshPage || roomWidth != previousRoomWidth) {
      tft.fillRect(roomWidthStartPos, 236, roomWidthEndPos - roomWidthStartPos, 50, user::backgroundColor);
      if (user::convertToInches) {
        roomWidthStartPos = 430 - ((tft.getStringWidth(roomWidth, 1) / 2) + 8);
        tft.print(roomWidthStartPos, 240, roomWidth, 1);
        tft.setFont(&akashi_36px_Regular);
        tft.setFontScale(1);
        tft.print(tft.getFontX() + 6, 230, F("\""));
      }
      else {
        roomWidthStartPos = 430 - ((tft.getStringWidth(roomWidth, 0) / 2) + 23);
        tft.print(roomWidthStartPos, 240, roomWidth, 0);
        tft.setFont(&akashi_36px_Regular);
        tft.setFontScale(1);
        tft.print(tft.getFontX() + 6, 250, F("cm"));
      }
      previousRoomWidth = roomWidth;
      roomWidthEndPos = tft.getFontX();
    }
    // ROOM HEIGHT
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setFontScale(1);
    static int roomHeightStartPos = 0, roomHeightEndPos = 0;
    static float previousRoomHeight;
    float roomHeight = user::convertToInches ? user::roomHeightInches : user::roomHeightCm;
    if (display::refreshPage || roomHeight != previousRoomHeight) {
      tft.fillRect(roomHeightStartPos, 236, roomHeightEndPos - roomHeightStartPos, 50, user::backgroundColor);
      if (user::convertToInches) {
        roomHeightStartPos = 640 - ((tft.getStringWidth(roomHeight, 1) / 2) + 8);
        tft.print(roomHeightStartPos, 240, roomHeight, 1);
        tft.setFont(&akashi_36px_Regular);
        tft.setFontScale(1);
        tft.print(tft.getFontX() + 6, 230, F("\""));
      }
      else {
        roomHeightStartPos = 640 - ((tft.getStringWidth(roomHeight, 0) / 2) + 23);
        tft.print(roomHeightStartPos, 240, roomHeight, 0);
        tft.setFont(&akashi_36px_Regular);
        tft.setFontScale(1);
        tft.print(tft.getFontX() + 6, 250, F("cm"));
      }
      previousRoomHeight = roomHeight;
      roomHeightEndPos = tft.getFontX();
    }
  }
  else if (display::co2PageScrollPos == 2) {
    if (display::refreshPage) {
      tft.setFont(&akashi_36px_Regular);
      tft.setFontScale(1);
      tft.setTextColor(RA8875_BLACK);
      tft.print(330, 116, F("Co2 settings"));
      tft.print(130, 166, F("Flow rate"));
      tft.print(380, 166, F("Time"));
      tft.print(580, 166, F("Fans off"));
      drawThreeUpDownButtons();
    }
    tft.setTextColor(RA8875_BLACK);
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setFontScale(1);
    // CO2 FLOW RATE LTR/MIN
    static int co2FlowrateStartPos = 0, co2FlowrateEndPos = 0;
    static float previousco2Flowrate;
    float co2Flowrate = user::convertToInches ? user::co2FlowrateFeet3 : user::co2FlowrateLtrs;
    if (display::refreshPage || co2Flowrate != previousco2Flowrate) {
      tft.fillRect(co2FlowrateStartPos, 238, co2FlowrateEndPos - co2FlowrateStartPos, 50, user::backgroundColor);
      if (co2Flowrate < 10)
        co2FlowrateStartPos = 140;
      else
        co2FlowrateStartPos = 120;
      tft.setCursor(co2FlowrateStartPos, 240);
      if (user::convertToInches) {
        tft.print(co2Flowrate, 1);
        tft.setFont(&akashi_36px_Regular);
        tft.setFontScale(1);
        tft.setCursor(tft.getFontX() + 4, 236);
        tft.print(F("Ft3"));
      }
      else {
        tft.print(co2Flowrate, 1);
        tft.setFont(&akashi_36px_Regular);
        tft.setFontScale(1);
        tft.setCursor(tft.getFontX() + 4, 250);
        co2Flowrate == 1 ? tft.print(F("ltr")) : tft.print(F("ltrs"));
      }
      previousco2Flowrate = co2Flowrate;
      co2FlowrateEndPos = tft.getFontX();
    }
    // CO2 CHECK TIME
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setFontScale(1);
    static int co2CheckTimePosition;
    static int previousCo2CheckTimeMinute;
    if (display::refreshPage || user::co2CheckTimeMinute != previousCo2CheckTimeMinute) {
      tft.fillRect(338, 238, co2CheckTimePosition - 336, 50, user::backgroundColor);
      tft.setCursor(350, 240);
      if (user::co2CheckTimeHour < 10)
        tft.print(F("0"));
      tft.print(user::co2CheckTimeHour);
      tft.setFont(&akashi_36px_Regular);
      tft.setFontScale(1);
      tft.print(F(":"));
      tft.setFont(&HallfeticaLargenum_42px_Regular);
      tft.setFontScale(1);
      if (user::co2CheckTimeMinute < 10)
        tft.print(F("0"));
      tft.print(user::co2CheckTimeMinute);
      co2CheckTimePosition = tft.getFontX();
      previousCo2CheckTimeMinute = user::co2CheckTimeMinute;
    }
    // DISABLE FANS TIMER
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setFontScale(1);
    static int disableFansTimerStartPos = 0, disableFansTimerEndPos = 0;
    static int previousDisableFansTimer;

    if (display::refreshPage || user::disableFansTimer != previousDisableFansTimer) {
      tft.fillRect(disableFansTimerStartPos, 238, disableFansTimerEndPos - disableFansTimerStartPos, 50, user::backgroundColor);
      if (user::disableFansTimer < 10)
        disableFansTimerStartPos = 600;
      else if (user::disableFansTimer >= 10 && user::disableFansTimer < 100)
        disableFansTimerStartPos = 580;
      else
        disableFansTimerStartPos = 570;
      tft.setCursor(disableFansTimerStartPos, 240);
      if (user::disableFansTimer == 0) {
        tft.setFont(&akashi_36px_Regular);
        tft.setFontScale(1);
        tft.print(F("Disabled"));
      }
      else {
        tft.print(user::disableFansTimer);
        tft.setFont(&akashi_36px_Regular);
        tft.setFontScale(1);
        tft.setCursor(tft.getFontX() + 6, 250);
        tft.print(F("mins"));
      }
      disableFansTimerEndPos = tft.getFontX();
      previousDisableFansTimer = user::disableFansTimer;
    }
  }
  else {
    if (display::refreshPage) {
      tft.setFont(&akashi_36px_Regular);
      tft.setFontScale(1);
      tft.setTextColor(RA8875_BLACK);
      tft.print(330, 116, F("Co2 settings"));
      tft.print(142, 166, F("Manual"));
      tft.print(130, 196, F("duration"));
      tft.print(380, 166, F("Gas"));
      tft.print(358, 196, F("duration"));
      tft.print(592, 166, F("Disable"));
      tft.print(624, 196, F("Co2"));
      tft.setFont(&akashi_36px_Regular);
      tft.setTextColor(RA8875_BLACK);
    }
    // ENABLE MANUAL CO2 DURATION
    static bool previousenableManualCo2Duration;
    if (display::refreshPage || user::enableManualCo2Duration != previousenableManualCo2Duration) {
      tft.fillRoundRect(139, 309, 152, 42, 5, RA8875_BLACK);
      if (!user::enableManualCo2Duration) {
        tft.fillRoundRect(140, 310, 75, 40, 5, RA8875_RED);
        tft.fillRoundRect(215, 310, 75, 40, 5, display::RA8875_GREY);
      }
      else {
        tft.fillRoundRect(140, 310, 75, 40, 5, display::RA8875_GREY);
        tft.fillRoundRect(215, 310, 75, 40, 5, RA8875_GREEN);
      }

      if (user::enableManualCo2Duration)
        drawUpDownButtons(390, 410, 478, 410, RA8875_BLUE);
      else
        drawUpDownButtons(390, 410, 478, 410, display::RA8875_GREY);

      previousenableManualCo2Duration = user::enableManualCo2Duration;
    }
    // DISABLE CO2 CONTROL
    static bool previousDisableCo2Control;
    if (display::refreshPage || user::disableCo2Control != previousDisableCo2Control) {
      tft.fillRoundRect(584, 309, 152, 42, 5, RA8875_BLACK);
      if (!user::disableCo2Control) {
        tft.fillRoundRect(585, 310, 75, 40, 5, RA8875_RED);
        tft.fillRoundRect(660, 310, 75, 40, 5, display::RA8875_GREY);
      }
      else {
        tft.fillRoundRect(585, 310, 75, 40, 5, display::RA8875_GREY);
        tft.fillRoundRect(660, 310, 75, 40, 5, RA8875_GREEN);
      }
      previousDisableCo2Control = user::disableCo2Control;
    }
    // CO2 MANUAL GAS DURATION TIMER
    static int manualCo2GasDurationStartPos = 0, manualCo2GasDurationEndPos = 0;
    static float previousManualCo2GasDuration;
    if (display::refreshPage || user::manualCo2GasDuration != previousManualCo2GasDuration) {
      tft.setTextColor(RA8875_BLACK);
      tft.setFont(&HallfeticaLargenum_42px_Regular);
      tft.setFontScale(1);
      tft.fillRect(manualCo2GasDurationStartPos, 306, manualCo2GasDurationEndPos - manualCo2GasDurationStartPos, 50, user::backgroundColor);
      if (user::manualCo2GasDuration < 10)
        manualCo2GasDurationStartPos = 376;
      else
        manualCo2GasDurationStartPos = 356;
      tft.print(manualCo2GasDurationStartPos, 308, user::manualCo2GasDuration);
      tft.setFont(&akashi_36px_Regular);
      tft.setFontScale(1);
      tft.setCursor(tft.getFontX() + 4, 316);
      user::manualCo2GasDuration == 1 ? tft.print(F("min")) : tft.print(F("mins"));
      manualCo2GasDurationEndPos = tft.getFontX();
      previousManualCo2GasDuration = user::manualCo2GasDuration;
    }

  }
}
// =============================================================================================================================================================================================
// =============================================================================================================================================================================================
void waterPage() {
  if (display::refreshPage) {
    drawSlideIcons(110, 112, display::waterPageScrollPos, 4);
  }
  if (display::waterPageScrollPos == 0) {
    if (display::refreshPage) {
      tft.setFont(&akashi_36px_Regular);
      tft.setFontScale(1);
      tft.setTextColor(RA8875_BLACK);
      tft.print(260, 116, F("Target water level"));
      tft.print(176, 170, F("Minimum"));
      tft.print(530, 170, F("Maximum"));
      drawUpDownButtons(230, 400, 310, 400, RA8875_BLUE);
      drawUpDownButtons(585, 400, 665, 400, RA8875_BLUE);
    }
    if (user::convertToInches)
      drawTwoValues(285, user::targetMinWaterHeightInches, RA8875_BLACK, 1, 650, user::targetMaxWaterHeightInches, RA8875_BLACK, 1, 4);
    else
      drawTwoValues(285, user::targetMinWaterHeight, RA8875_BLACK, 1, 650, user::targetMaxWaterHeight, RA8875_BLACK, 1, 3);
  }
  else if (display::waterPageScrollPos == 1) {
    if (display::refreshPage) {
      tft.setFont(&akashi_36px_Regular);
      tft.setFontScale(1);
      tft.setTextColor(RA8875_BLACK);
      tft.print(260, 116, F("Target water temp"));
      tft.print(176, 170, F("Minimum"));
      tft.print(530, 170, F("Maximum"));
      drawUpDownButtons(230, 400, 310, 400, RA8875_BLUE);
      drawUpDownButtons(585, 400, 665, 400, RA8875_BLUE);
    }
    if (user::convertToF)
      drawTwoValues(285, user::targetMinWaterTempF, RA8875_BLACK, 1, 650, user::targetMaxWaterTempF, RA8875_BLACK, 1, 2);
    else
      drawTwoValues(285, user::targetMinWaterTemp, RA8875_BLACK, 1, 650, user::targetMaxWaterTemp, RA8875_BLACK, 1, 1);
  }
  else if (display::waterPageScrollPos == 2) {
    if (display::refreshPage) {
      tft.setFont(&akashi_36px_Regular);
      tft.setFontScale(1);
      tft.setTextColor(RA8875_BLACK);
      tft.print(140, 140, F("Auto drain dates"));
      tft.print(510, 200, F("Disable drain"));
      tft.print(510, 250, F("and refill"));
      tft.print(100, 420, F("Time"));
      drawMiniButtonIncrements(385, 440);
    }
    if (display::refreshPage || display::refreshCalander) {
      tft.setFont(&akashi_36px_Regular);
      tft.setFontScale(1);
      tft.setTextColor(RA8875_BLACK);
      int calanderX = 110, calanderY = 205;
      for (byte i = 1; i < 32; i++) {
        if (i < 10)
          tft.setCursor(calanderX + 14, calanderY);
        else if (i < 19)
          tft.setCursor(calanderX + 6, calanderY);
        else if (i == 21 || i == 31)
          tft.setCursor(calanderX + 6, calanderY);
        else
          tft.setCursor(calanderX + 2, calanderY);
        if (user::autoFillDays[i - 1])
          tft.fillRect(calanderX - 1, calanderY + 1, 52, 38, RA8875_BLUE);
        else
          tft.fillRect(calanderX - 1, calanderY + 1, 52, 38, RA8875_WHITE);
        tft.drawRect(calanderX - 2, calanderY, 54, 40, RA8875_BLACK);
        tft.print(i);
        calanderX += 54;
        if (i % 7 == 0) {
          calanderX = 110;
          calanderY += 40;
        }
      }
      display::refreshCalander = false;
    }
    static byte previousAutoFillMinute;
    static int autoFillTimePosition;
    if (display::refreshPage || user::autoFillMinute != previousAutoFillMinute) {
      tft.setFont(&akashi_36px_Regular);
      tft.setFontScale(1);
      tft.setTextColor(RA8875_BLACK);
      tft.fillRect(204, 418, autoFillTimePosition - 204, 42, user::backgroundColor);
      tft.setCursor(206, 420);
      if (user::autoFillHour < 10)
        tft.print(F("0"));
      tft.print(user::autoFillHour);
      tft.print(F(":"));
      if (user::autoFillMinute < 10)
        tft.print(F("0"));
      tft.print(user::autoFillMinute);
      previousAutoFillMinute = user::autoFillMinute;
      autoFillTimePosition = tft.getFontX();
    }
    static bool previousdisableDrainAndRefill;
    if (display::refreshPage || user::disableDrainAndRefill != previousdisableDrainAndRefill) {
      drawRadioButton(565, 320, user::disableDrainAndRefill);
      previousdisableDrainAndRefill = user::disableDrainAndRefill;
    }
  }
  else if (display::waterPageScrollPos == 3) {
    if (display::refreshPage) {
      tft.setFont(&akashi_36px_Regular);
      tft.setFontScale(1);
      tft.setTextColor(RA8875_BLACK);
      tft.print(276, 110, F("Refill doser mls"));
      drawFourUpDownButtons(60);
    }
    if (display::refillDoserPageScrollPos > 0) {
      tft.fillTriangle(95, 240, 115, 200, 115, 280, RA8875_BLUE);
    }
    if (display::refillDoserPageScrollPos < user::numberOfDosers - 4) {
      tft.fillTriangle(779, 200, 799, 240, 779, 280, RA8875_BLUE);
    }
    int startPosition = display::refillDoserPageScrollPos * 178;
    // The below method may look odd but it is much faster than a for loop "after testing"
    if (display::refillDoserPageScrollPos == 0) { // DOSER 1
      static byte previousDoserOneMode;
      static int doserOneMlsPosition, previousDoserOneMls;
      drawDoser(100 - startPosition, 150, 1, user::refillDoserOneMills, previousDoserOneMls, doserOneMlsPosition, 0, 0);
      drawEcPhButton(149 - startPosition, 300, user::doserOneMode, previousDoserOneMode, true, false);
    }
    if (display::refillDoserPageScrollPos <= 1) { // DOSER 2
      static byte previousDoserTwoMode;
      static int doserTwoMlsPosition, previousDoserTwoMls;
      drawDoser(278 - startPosition, 150, 2, user::refillDoserTwoMills, previousDoserTwoMls, doserTwoMlsPosition, 0, 0);
      drawEcPhButton(327 - startPosition, 300, user::doserTwoMode, previousDoserTwoMode, false, false);
    }
    if (display::refillDoserPageScrollPos <= 2) { // DOSER 3
      static byte previousDoserThreeMode;
      static int doserThreeMlsPosition, previousDoserThreeMls;
      drawDoser(456 - startPosition, 150, 3, user::refillDoserThreeMills, previousDoserThreeMls, doserThreeMlsPosition, 0, 0);
      drawEcPhButton(505 - startPosition, 300, user::doserThreeMode, previousDoserThreeMode, true, false);
    }
    if (display::refillDoserPageScrollPos <= 2) { // DOSER 4
      static byte previousDoserFourMode;
      static int doserFourMlsPosition, previousDoserFourMls;
      drawDoser(634 - startPosition, 150, 4, user::refillDoserFourMills, previousDoserFourMls, doserFourMlsPosition, 0, 0);
      drawEcPhButton(683 - startPosition, 300, user::doserFourMode, previousDoserFourMode, false, false);
    }
    if (display::refillDoserPageScrollPos >= 1) { // DOSER 5
      static byte previousDoserFiveMode;
      static int doserFiveMlsPosition, previousDoserFiveMls;
      drawDoser(812 - startPosition, 150, 5, user::refillDoserFiveMills, previousDoserFiveMls, doserFiveMlsPosition, 0, 0);
      drawEcPhButton(861 - startPosition, 300, user::doserFiveMode, previousDoserFiveMode, true, false);
    }
    if (display::refillDoserPageScrollPos == 2) { // DOSER 6
      static byte previousDoserSixMode;
      static int doserSixMlsPosition, previousDoserSixMls;
      drawDoser(990 - startPosition, 150, 6, user::refillDoserSixMills, previousDoserSixMls, doserSixMlsPosition, 0, 0);
      drawEcPhButton(1039 - startPosition, 300, user::doserSixMode, previousDoserSixMode, false, false);
    }
  }
}
// =============================================================================================================================================================================================
// =============================================================================================================================================================================================
void dosersPage() {
  if (display::refreshPage) {
    drawFourUpDownButtons(0);
    primeButton(136, 443);
    primeButton(309, 443);
    primeButton(489, 443);
    primeButton(666, 443);
    if (user::numberOfDosers > 4) {
      if (display::doserPageScrollPos > 0) {
        tft.fillTriangle(95, 240, 115, 200, 115, 280, RA8875_BLUE);
      }
      if (display::doserPageScrollPos < user::numberOfDosers - 4) {
        tft.fillTriangle(779, 200, 799, 240, 779, 280, RA8875_BLUE);
      }
    }
  }
  int startPosition = display::doserPageScrollPos * 178;
  if (display::doserPageScrollPos == 0) { // DOSER 1
    static byte previousDoserOneMode;
    static int doserOneMlsPosition, previousDoserOneMls;
    drawDoser(100 - startPosition, 90, 1, user::doserOneMills, previousDoserOneMls, doserOneMlsPosition, 0, 0);
    drawEcPhButton(149 - startPosition, 255, user::doserOneMode, previousDoserOneMode, true, true);
  }
  if (display::doserPageScrollPos <= 1) { // DOSER 2
    static byte previousDoserTwoMode;
    static int doserTwoMlsPosition, previousDoserTwoMls;
    drawDoser(278 - startPosition, 90, 2, user::doserTwoMills, previousDoserTwoMls, doserTwoMlsPosition, 0, 0);
    drawEcPhButton(327 - startPosition, 255, user::doserTwoMode, previousDoserTwoMode, true, true);
  }
  if (display::doserPageScrollPos <= 2) { // DOSER 3
    static byte previousDoserThreeMode;
    static int doserThreeMlsPosition, previousDoserThreeMls;
    drawDoser(456 - startPosition, 90, 3, user::doserThreeMills, previousDoserThreeMls, doserThreeMlsPosition, 0, 0);
    drawEcPhButton(505 - startPosition, 255, user::doserThreeMode, previousDoserThreeMode, true, true);
  }
  if (display::doserPageScrollPos <= 2) { // DOSER 4
    static byte previousDoserFourMode;
    static int doserFourMlsPosition, previousDoserFourMls;
    drawDoser(634 - startPosition, 90, 4, user::doserFourMills, previousDoserFourMls, doserFourMlsPosition, 0, 0);
    drawEcPhButton(683 - startPosition, 255, user::doserFourMode, previousDoserFourMode, false, true);
  }
  if (display::doserPageScrollPos >= 1) { // DOSER 5
    static byte previousDoserFiveMode;
    static int doserFiveMlsPosition, previousDoserFiveMls;
    drawDoser(812 - startPosition, 90, 5, user::doserFiveMills, previousDoserFiveMls, doserFiveMlsPosition, 0, 0);
    drawEcPhButton(861 - startPosition, 255, user::doserFiveMode, previousDoserFiveMode, true, true);
  }
  if (display::doserPageScrollPos >= 2) { // DOSER 6
    static byte previousDoserSixMode;
    static int doserSixMlsPosition, previousDoserSixMls;
    drawDoser(990 - startPosition, 90, 6, user::doserSixMills, previousDoserSixMls, doserSixMlsPosition, 0, 0);
    drawEcPhButton(1039 - startPosition, 255, user::doserSixMode, previousDoserSixMode, false, true);
  }
}
// =============================================================================================================================================================================================
// =============================================================================================================================================================================================
void lightingPage() {
  if (display::refreshPage) {
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK);
    tft.print(300, 116, F("Light settings"));
    tft.print(110, 166, F("Light on"));
    tft.print(335, 166, F("Light off"));
    tft.print(550, 166, F("On/Auto/Off"));
    drawUpDownButtons(164, 366, 243, 366, RA8875_BLUE);
    drawUpDownButtons(374, 366, 457, 366, RA8875_BLUE);
  }
  // LIGHT ON TIME
  static byte previousOnTime;
  if (display::refreshPage || previousOnTime != user::lightOnTimeHour + user::lightOnTimeMin) {
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setTextColor(RA8875_BLACK);
    tft.setFontScale(1);
    tft.fillRect(105, 238, 200, 50, user::backgroundColor);
    tft.setCursor(110, 240);
    if (user::lightOnTimeHour < 10)
      tft.print(0);
    tft.print(user::lightOnTimeHour);
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.print(190, 240, F(":"));
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setFontScale(1);
    tft.setCursor(210, 240);
    if (user::lightOnTimeMin < 10)
      tft.print(0);
    tft.print(user::lightOnTimeMin);
    previousOnTime = user::lightOnTimeHour + user::lightOnTimeMin;
  }
  // LIGHT OFF TIME
  static byte previousOffTime;
  if (display::refreshPage || previousOffTime != user::lightOffTimeHour + user::lightOffTimeMin) {
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setFontScale(1);
    tft.fillRect(325, 238, 200, 50, user::backgroundColor);
    tft.setCursor(330, 240);
    if (user::lightOffTimeHour < 10)
      tft.print(0);
    tft.print(user::lightOffTimeHour);
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.print(410, 240, F(":"));
    tft.setFont(&HallfeticaLargenum_42px_Regular);
    tft.setFontScale(1);
    tft.setCursor(430, 240);
    if (user::lightOffTimeMin < 10)
      tft.print(0);
    tft.print(user::lightOffTimeMin);
    previousOffTime = user::lightOffTimeHour + user::lightOffTimeMin;
  }
  // Light mode 3-state radio button
  static byte previousLightMode;
  if (display::refreshPage || user::lightMode != previousLightMode) {
    tft.fillRoundRect(568, 238, 184, 44, 5, RA8875_BLACK);
    if (user::lightMode == 0) { // auto
      tft.fillRoundRect(570, 240, 60, 40, 5, display::RA8875_GREY);
      tft.fillRoundRect(630, 240, 60, 40, 5, RA8875_GREEN);
      tft.fillRoundRect(690, 240, 60, 40, 5, display::RA8875_GREY);
    }
    else if (user::lightMode == 1) { // on
      tft.fillRoundRect(570, 240, 60, 40, 5, RA8875_GREEN);
      tft.fillRoundRect(630, 240, 60, 40, 5, display::RA8875_GREY);
      tft.fillRoundRect(690, 240, 60, 40, 5, display::RA8875_GREY);
      tft.drawLine(690, 240, 690, 280, RA8875_BLACK);
    }
    else { // off
      tft.fillRoundRect(570, 240, 60, 40, 5, display::RA8875_GREY);
      tft.fillRoundRect(630, 240, 60, 40, 5, display::RA8875_GREY);
      tft.fillRoundRect(690, 240, 60, 40, 5, RA8875_RED);
      tft.drawLine(630, 240, 630, 280, RA8875_BLACK);
    }
    previousLightMode = user::lightMode;
  }
}
// =============================================================================================================================================================================================
// =============================================================================================================================================================================================
void fansPage() {
  if (display::refreshPage)
    drawSlideIcons(110, 112, display::fansPage, 5);
  if (display::fansPage == 0) {
    if (display::refreshPage) {
      tft.setFont(&akashi_36px_Regular);
      tft.setFontScale(1);
      tft.setTextColor(RA8875_BLACK);
      tft.print(300, 116, F("Fan one target"));
      tft.print(180, 166, F("Min speed"));
      tft.print(506, 166, F("Max speed"));
      drawUpDownButtons(230, 400, 310, 400, RA8875_BLUE);
      drawUpDownButtons(570, 400, 650, 400, RA8875_BLUE);
    }
    drawTwoValues(285, user::targetMinFanOneSpeed, RA8875_BLACK, 0, 620, user::targetMaxFanOneSpeed, RA8875_BLACK, 0, 5);
  }
  else if (display::fansPage == 1) {
    if (display::refreshPage) {
      tft.setFont(&akashi_36px_Regular);
      tft.setFontScale(1);
      tft.setTextColor(RA8875_BLACK);
      tft.print(300, 116, F("Fan two target"));
      tft.print(180, 166, F("Min speed"));
      tft.print(506, 166, F("Max speed"));
      drawUpDownButtons(230, 400, 310, 400, RA8875_BLUE);
      drawUpDownButtons(570, 400, 650, 400, RA8875_BLUE);
    }
    drawTwoValues(285, user::targetMinFanTwoSpeed, RA8875_BLACK, 0, 620, user::targetMaxFanTwoSpeed, RA8875_BLACK, 0, 5);
  }
  else if (display::fansPage == 2) {
    if (display::refreshPage) {
      tft.setFont(&akashi_36px_Regular);
      tft.setFontScale(1);
      tft.setTextColor(RA8875_BLACK);
      tft.print(296, 116, F("Target air temp"));
      tft.print(176, 170, F("Minimum"));
      tft.print(530, 170, F("Maximum"));
      drawUpDownButtons(230, 400, 310, 400, RA8875_BLUE);
      drawUpDownButtons(585, 400, 665, 400, RA8875_BLUE);
    }
    if (user::convertToF)
      drawTwoValues(285, user::targetMinAirTempF, RA8875_BLACK, 1, 650, user::targetMaxAirTempF, RA8875_BLACK, 1, 2);
    else
      drawTwoValues(285, user::targetMinAirTemp, RA8875_BLACK, 1, 650, user::targetMaxAirTemp, RA8875_BLACK, 1, 1);
  }
  else if (display::fansPage == 3) {
    if (display::refreshPage) {
      tft.setFont(&akashi_36px_Regular);
      tft.setFontScale(1);
      tft.setTextColor(RA8875_BLACK);
      tft.print(290, 116, F("Target humidity"));
      tft.print(176, 170, F("Minimum"));
      tft.print(530, 170, F("Maximum"));
      drawUpDownButtons(230, 400, 310, 400, RA8875_BLUE);
      drawUpDownButtons(585, 400, 665, 400, RA8875_BLUE);
    }
    drawTwoValues(285, user::targetMinHumidity, RA8875_BLACK, 1, 650, user::targetMaxHumidity, RA8875_BLACK, 1, 5);
  }
  else if (display::fansPage == 4) {
    if (display::refreshPage) {
      tft.setFont(&akashi_36px_Regular);
      tft.setFontScale(1);
      tft.setTextColor(RA8875_BLUE);
      tft.print(324, 126, F("Fan settings"));
      tft.setTextColor(RA8875_BLACK);
      tft.print(110, 180, F("Control temperature"));
      tft.print(110, 230, F("Control humidity"));
      tft.print(110, 280, F("Fan one fixed speed"));
      tft.print(110, 330, F("Fan two fixed speed"));
    }
    // FAN MODES
    static bool previousFansControlTemperature;
    if (display::refreshPage || user::fansControlTemperature != previousFansControlTemperature) {
      drawRadioButton(640, 180, user::fansControlTemperature);
      previousFansControlTemperature = user::fansControlTemperature;
    }
    static bool previousFansControlHumidity;
    if (display::refreshPage || user::fansControlHumidity != previousFansControlHumidity) {
      drawRadioButton(640, 230, user::fansControlHumidity);
      previousFansControlHumidity = user::fansControlHumidity;
    }
    static bool previousFanOneFixedSpeed;
    if (display::refreshPage || user::fanOneFixedSpeed != previousFanOneFixedSpeed) {
      drawRadioButton(640, 280, user::fanOneFixedSpeed);
      previousFanOneFixedSpeed = user::fanOneFixedSpeed;
    }
    static bool previousFanTwoFixedSpeed;
    if (display::refreshPage || user::fanTwoFixedSpeed != previousFanTwoFixedSpeed) {
      drawRadioButton(640, 330, user::fanTwoFixedSpeed);
      previousFanTwoFixedSpeed = user::fanTwoFixedSpeed;
    }
  }
}
// =============================================================================================================================================================================================
// =============================================================================================================================================================================================
void warningsPage() {
  unsigned int color;
  if (display::refreshPage) {
    drawSlideIcons(110, 112, display::warningsPage, 8);
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLACK);
    tft.print(116, 170, F("Current reading"));
    tft.print(470, 170, F("Error margin"));
    drawUpDownButtons(554, 400, 664, 400, RA8875_BLUE);
  }
  if (display::warningsPage == 0) {
    if (display::refreshPage)
      tft.print(310, 126, F("PPM warning"));
    color = setWarningColor(sensor::tds, user::targetMinTds, user::targetMaxTds, user::tdsErrorMargin);
    drawTwoValues(285, sensor::tds, color, 0, 620, user::tdsErrorMargin, RA8875_BLACK, 0, 0);
  }
  else if (display::warningsPage == 1) {
    if (display::refreshPage)
      tft.print(324, 126, F("EC warning"));
    color = setWarningColor(sensor::ec, user::targetMinEc, user::targetMaxEc, user::ecErrorMargin);
    drawTwoValues(285, sensor::ec, color, 2, 620, user::ecErrorMargin, RA8875_BLACK, 2, 0);
  }
  else if (display::warningsPage == 2) {
    if (display::refreshPage)
      tft.print(324, 126, F("PH warning"));
    color = setWarningColor(sensor::ph, user::targetMinPh, user::targetMaxPh, user::phErrorMargin);
    drawTwoValues(285, sensor::ph, color, 2, 620, user::phErrorMargin, RA8875_BLACK, 2, 0);
  }
  else if (display::warningsPage == 3) {
    if (display::refreshPage)
      tft.print(310, 126, F("Co2 warning"));
    color = setWarningColor(sensor::co2, user::targetCo2, user::targetCo2, user::co2ErrorMargin);
    drawTwoValues(285, sensor::co2, color, 0, 620, user::co2ErrorMargin, RA8875_BLACK, 0, 0);
  }
  else if (display::warningsPage == 4) {
    if (display::refreshPage)
      tft.print(246, 126, F("Water temp warning"));
    if (user::convertToF) {
      color = setWarningColor(sensor::waterTempF, user::targetMinWaterTempF, user::targetMaxWaterTempF, user::waterTempErrorMarginF);
      drawTwoValues(285, sensor::waterTempF, color, 1, 620, user::waterTempErrorMarginF, RA8875_BLACK, 1, 2);
    }
    else {
      color = setWarningColor(sensor::waterTemp, user::targetMinWaterTemp, user::targetMaxWaterTemp, user::waterTempErrorMargin);
      drawTwoValues(285, sensor::waterTemp, color, 1, 620, user::waterTempErrorMargin, RA8875_BLACK, 1, 1);
    }
  }
  else if (display::warningsPage == 5) {
    if (display::refreshPage)
      tft.print(224, 126, F("Water height warning"));
    if (user::convertToInches) {
      color = setWarningColor(sensor::waterLevelInches, user::targetMinWaterHeightInches, user::targetMaxWaterHeightInches, user::waterHeightErrorMarginInches);
      drawTwoValues(285, sensor::waterLevelInches, color, 1, 620, user::waterHeightErrorMarginInches, RA8875_BLACK, 1, 4);
    }
    else {
      color = setWarningColor(sensor::waterLevel, user::targetMinWaterHeight, user::targetMaxWaterHeight, user::waterHeightErrorMargin);
      drawTwoValues(285, sensor::waterLevel, color, 1, 620, user::waterHeightErrorMargin, RA8875_BLACK, 1, 3);
    }
  }
  else if (display::warningsPage == 6) {
    if (display::refreshPage)
      tft.print(284, 126, F("Air temp warning"));
    if (user::convertToF) {
      color = setWarningColor(convertToF(sensor::airTemp), user::targetMinAirTempF, user::targetMaxAirTempF, user::airTempErrorMarginF);
      drawTwoValues(285, convertToF(sensor::airTemp), color, 1, 620, user::airTempErrorMarginF, RA8875_BLACK, 1, 2);
    }
    else {
      color = setWarningColor(sensor::airTemp, user::targetMinAirTemp, user::targetMaxAirTemp, user::airTempErrorMargin);
      drawTwoValues(285, sensor::airTemp, color, 1, 620, user::airTempErrorMargin, RA8875_BLACK, 1, 1);
    }
  }
  else if (display::warningsPage == 7) {
    if (display::refreshPage)
      tft.print(280, 126, F("Humidity warning"));
    color = setWarningColor(sensor::humidity, user::targetMinHumidity, user::targetMaxHumidity, user::humidityErrorMargin);
    drawTwoValues(285, sensor::humidity, color, 1, 620, user::humidityErrorMargin, RA8875_BLACK, 1, 5);
  }
}
// =============================================================================================================================================================================================
// =============================================================================================================================================================================================
// settings pages
void drawSettingsPageZero() {
  if (user::setRTCtime)
    displaySetRTC();
  else {
    if (display::refreshPage) {
      tft.setFont(&akashi_36px_Regular);
      tft.setFontScale(1);
      tft.setTextColor(RA8875_BLUE);
      tft.print(110, 130, F("Display"));
      if (display::settingsPageZeroScrollPos > 0)
        tft.fillTriangle(360, 450, 400, 440, 440, 450, RA8875_BLUE);
      if (display::settingsPageZeroScrollPos < 2)
        tft.fillTriangle(360, 455, 400, 465, 440, 455, RA8875_BLUE);
    }
    int scrollMargin = display::settingsPageZeroScrollPos * 44;
    if (display::settingsPageZeroScrollPos == 0) {
      if (display::refreshPage) {
        tft.setFont(&akashi_36px_Regular);
        tft.setTextColor(RA8875_BLACK);
        tft.print(110, 170 - scrollMargin, F("Background color"));
        backGroundColorButton(538, 168 - scrollMargin, display::RA8875_LIGHT_BLUE);
        backGroundColorButton(588, 168 - scrollMargin, display::RA8875_LIGHT_GREEN);
        backGroundColorButton(638, 168 - scrollMargin, RA8875_PURPLE);
        backGroundColorButton(688, 168 - scrollMargin, display::RA8875_LIGHT_PINK);
        backGroundColorButton(738, 168 - scrollMargin, RA8875_WHITE);
      }
    }
    if (display::settingsPageZeroScrollPos <= 1) {
      static int previousAfkPosition;
      static unsigned long previousAfkTime;
      if (display::refreshPage || previousAfkTime != user::afkTime) {
        tft.setFont(&akashi_36px_Regular);
        tft.setTextColor(RA8875_BLACK);
        tft.print(110, 214 - scrollMargin, F("Display timeout"));
        tft.setFontScale(1);
        tft.fillRect(508, 217 - scrollMargin, previousAfkPosition - 506, 32, user::backgroundColor);
        tft.setCursor(510, 214 - scrollMargin);
        if (user::afkTime != 0)
          tft.print(user::afkTime);
        previousAfkPosition = tft.getFontX();
        tft.setCursor(previousAfkPosition + 4, 214 - scrollMargin);
        if (user::afkTime == 0)
          tft.print(F("OFF"));
        else if (user::afkTime == 1)
          tft.print(F("min"));
        else
          tft.print(F("mins"));
        previousAfkPosition = tft.getFontX();
        drawMiniButtonIncrements(680, 232 - scrollMargin);
        previousAfkTime = user::afkTime;
      }
    }
    if (display::settingsPageZeroScrollPos <= 2) {
      static int previousGraphIntervalPosition;
      static unsigned long previousGraphInterval;
      if (display::refreshPage || previousGraphInterval != user::graphInterval) {
        tft.setFont(&akashi_36px_Regular);
        tft.setTextColor(RA8875_BLACK);
        tft.print(110, 258 - scrollMargin, F("Graph interval"));
        tft.fillRect(508, 261 - scrollMargin, previousGraphIntervalPosition - 506, 32, user::backgroundColor);
        tft.print(510, 258 - scrollMargin, user::graphInterval);
        previousGraphIntervalPosition = tft.getFontX();
        tft.setCursor(previousGraphIntervalPosition + 4, 258 - scrollMargin);
        user::graphInterval == 1 ? tft.print(F("min")) : tft.print(F("mins"));
        previousGraphIntervalPosition = tft.getFontX();
        drawMiniButtonIncrements(680, 276 - scrollMargin);
        previousGraphInterval = user::graphInterval;
      }
    }
    if (display::settingsPageZeroScrollPos <= 3) {
      if (display::refreshPage) {
        tft.setFont(&akashi_36px_Regular);
        tft.setTextColor(RA8875_BLACK);
        tft.print(110, 302 - scrollMargin, F("Set datetime"));
        drawMiniConfirmButton(712, 302 - scrollMargin);
      }
    }
    if (display::settingsPageZeroScrollPos <= 4) {
      static bool previousTempInF;
      if (display::refreshPage || user::convertToF  != previousTempInF) {
        tft.setFont(&akashi_36px_Regular);
        tft.setTextColor(RA8875_BLACK);
        tft.print(110, 346 - scrollMargin, F("Temperature In F"));
        drawMiniRadioButton(682, 346 - scrollMargin, user::convertToF);
        previousTempInF = user::convertToF ;
      }
    }
    if (display::settingsPageZeroScrollPos <= 5) {
      static bool previousHeightInInches;
      if (display::refreshPage || user::convertToInches != previousHeightInInches) {
        tft.setFont(&akashi_36px_Regular);
        tft.setTextColor(RA8875_BLACK);
        tft.print(110, 390 - scrollMargin, F("Height In Inches"));
        drawMiniRadioButton(682, 390 - scrollMargin, user::convertToInches);
        previousHeightInInches = user::convertToInches;
      }
    }
    if (display::settingsPageZeroScrollPos >= 1) {
      static bool previousConvertToTds;
      if (display::refreshPage || user::convertToTds != previousConvertToTds) {
        tft.setFont(&akashi_36px_Regular);
        tft.setTextColor(RA8875_BLACK);
        tft.print(110, 434 - scrollMargin, F("EC to TDS"));
        drawMiniRadioButton(682, 434 - scrollMargin, user::convertToTds);
        previousConvertToTds = user::convertToTds;
      }
    }
    if (display::settingsPageZeroScrollPos >= 2) {
      static byte previousConversionType;
      if (display::refreshPage || previousConversionType != device::conversionType) {
        tft.setFont(&akashi_36px_Regular);
        tft.setTextColor(RA8875_BLACK);
        tft.print(110, 478 - scrollMargin, F("PPM to EC conversion"));
        drawEcConversionButton(712, 478 - scrollMargin);
        previousConversionType = device::conversionType;
      }
    }
  }
}
// =============================================================================================================================================================================================
// =============================================================================================================================================================================================
void drawSettingsPageOne() {
  if (display::showSystemLogs) {
    if (display::refreshPage) {
      tft.setFont(&akashi_36px_Regular);
      tft.setFontScale(1);
      tft.setTextColor(RA8875_BLUE);
      tft.print(320, 110, F("System logs"));
      tft.setTextColor(RA8875_BLACK);
      exitButton(690, 430);
      if (display::systemLogScrollPos > 0) {
        tft.fillTriangle(360, 450, 400, 440, 440, 450, RA8875_BLUE);
      }
      if (message::systemLogPos > 5 && display::systemLogScrollPos < message::systemLogPos - 6) {
        tft.fillTriangle(360, 455, 400, 465, 440, 455, RA8875_BLUE);
      }
      int startX = 160;
      int maxLogsToDsiplay = ((display::systemLogScrollPos + 6) <= message::systemLogPos ? (display::systemLogScrollPos + 6) : message::systemLogPos);
      for (byte i = display::systemLogScrollPos; i < maxLogsToDsiplay; i++) {
        const char* logPretext = PROGMEM_read(&message::notificationsArray[message::logTypeArray[i]]);
        Serial.print(F("Printing log[")); Serial.print(i); Serial.print(F("] ")); Serial.print((const __FlashStringHelper *)logPretext); Serial.print(F(" ")); Serial.println(message::timeStrArray[i]);
        tft.print(105, startX, (const __FlashStringHelper *)logPretext);
        tft.print(F(" "));
        tft.print(message::timeStrArray[i]);
        startX += 44;
      }
    }
  }
  else if (display::refreshPage && user::resetSettings) {
    continueMessage(message::resetDevice, -1, 0, true, true, false);
  }
  else {
    if (display::refreshPage) {
      tft.setFont(&akashi_36px_Regular);
      tft.setFontScale(1);
      tft.setTextColor(RA8875_BLUE);
      tft.print(110, 130, F("System"));
      if (display::settingsPageOneScrollPos > 0)
        tft.fillTriangle(360, 450, 400, 440, 440, 450, RA8875_BLUE);
      if (display::settingsPageOneScrollPos < 7)
        tft.fillTriangle(360, 455, 400, 465, 440, 455, RA8875_BLUE);
      tft.setTextColor(RA8875_BLACK);
    }
    int scrollMargin = display::settingsPageOneScrollPos * 44;
    if (display::settingsPageOneScrollPos == 0) {
      if (display::refreshPage) {
        tft.print(110, 170 - scrollMargin, F("Show system logs"));
        drawMiniConfirmButton(712, 170 - scrollMargin);
      }
    }
    if (display::settingsPageOneScrollPos <= 1) {
      static bool previousEnableWifi;
      if (display::refreshPage || wifi::wifiEnabled != previousEnableWifi) {
        tft.print(110, 214 - scrollMargin, F("Enable Wifi"));
        drawMiniRadioButton(682, 214 - scrollMargin, wifi::wifiEnabled);
        previousEnableWifi = wifi::wifiEnabled;
      }
    }
    if (display::settingsPageOneScrollPos <= 2) {
      static int previousNumberOfDosersPosition;
      static byte previousNumberOfDosers;
      if (display::refreshPage || previousNumberOfDosers != user::numberOfDosers) {
        tft.print(110, 258 - scrollMargin, F("Number of dosers"));
        tft.fillRect(508, 258 - scrollMargin, previousNumberOfDosersPosition - 506, 34, user::backgroundColor);
        tft.print(510, 258 - scrollMargin, user::numberOfDosers);
        previousNumberOfDosersPosition = tft.getFontX();
        if (display::refreshPage)
          drawMiniButtonIncrements(680, 278 - scrollMargin);
        tft.setFont(&akashi_36px_Regular);
        tft.setTextColor(RA8875_BLACK);
        previousNumberOfDosers = user::numberOfDosers;
      }
    }
    if (display::settingsPageOneScrollPos <= 3) {
      static int previousDosingIntervalPosition;
      static byte previousDosingInterval;
      if (display::refreshPage || previousDosingInterval != user::dosingInterval) {
        tft.print(110, 302 - scrollMargin, F("Dosing interval"));
        tft.fillRect(508, 302 - scrollMargin, previousDosingIntervalPosition - 506, 34, user::backgroundColor);
        tft.print(510, 302 - scrollMargin, user::dosingInterval);
        previousDosingIntervalPosition = tft.getFontX();
        tft.setCursor(previousDosingIntervalPosition + 4, 302 - scrollMargin);
        user::dosingInterval == 1 ? tft.print(F("hr")) : tft.print(F("hrs"));
        previousDosingIntervalPosition = tft.getFontX();
        if (display::refreshPage)
          drawMiniButtonIncrements(680, 322 - scrollMargin);
        tft.setFont(&akashi_36px_Regular);
        tft.setTextColor(RA8875_BLACK);
        previousDosingInterval = user::dosingInterval;
      }
    }
    if (display::settingsPageOneScrollPos <= 4) {
      static bool previousUseEtapeSensor;
      if (display::refreshPage || user::useEtapeSensor != previousUseEtapeSensor) {
        tft.print(110, 346 - scrollMargin, F("Water height sensor"));
        drawMiniEtapeButton(642, 346 - scrollMargin, user::useEtapeSensor);
        previousUseEtapeSensor = user::useEtapeSensor;
      }
    }
    if (display::settingsPageOneScrollPos <= 5) {
      static bool previousDisableLED;
      if (display::refreshPage || user::disableLED != previousDisableLED) {
        tft.print(110, 390 - scrollMargin, F("Disable LED"));
        drawMiniRadioButton(682, 390 - scrollMargin, user::disableLED);
        previousDisableLED = user::disableLED;
      }
    }
    if (display::settingsPageOneScrollPos >= 1 && display::settingsPageOneScrollPos <= 6) {
      static bool previousDisableBeeper;
      if (display::refreshPage || user::disableBeeper != previousDisableBeeper) {
        tft.print(110, 434 - scrollMargin, F("Disable speaker"));
        drawMiniRadioButton(682, 434 - scrollMargin, user::disableBeeper);
        previousDisableBeeper = user::disableBeeper;
      }
    }
    if (display::settingsPageOneScrollPos >= 2 && display::settingsPageOneScrollPos <= 7) {
      static bool previousDisableMachineLearning;
      if (display::refreshPage || user::disableMachineLearning != previousDisableMachineLearning) {
        tft.print(110, 478 - scrollMargin, F("Disable Machine Learning"));
        drawMiniRadioButton(682, 478 - scrollMargin, user::disableMachineLearning);
        previousDisableMachineLearning = user::disableMachineLearning;
      }
    }
    if (display::settingsPageOneScrollPos >= 3 && display::settingsPageOneScrollPos <= 8) {
      if (display::refreshPage) {
        tft.print(110, 522 - scrollMargin, F("Clear graph history"));
        drawMiniConfirmButton(712, 522 - scrollMargin);
      }
    }
    if (display::settingsPageOneScrollPos >= 4 && display::settingsPageOneScrollPos <= 9) {
      if (display::refreshPage) {
        tft.print(110, 566 - scrollMargin, F("Clear max mins"));
        drawMiniConfirmButton(712, 566 - scrollMargin);
      }
    }
    if (display::settingsPageOneScrollPos >= 5 && display::settingsPageOneScrollPos <= 10) {
      if (display::refreshPage) {
        tft.print(110, 610 - scrollMargin, F("Clear machine learning"));
        drawMiniConfirmButton(712, 610 - scrollMargin);
      }
    }
    if (display::settingsPageOneScrollPos >= 6 && display::settingsPageOneScrollPos <= 11) {
      if (display::refreshPage) {
        tft.print(110, 654 - scrollMargin, F("Clear system logs"));
        drawMiniConfirmButton(712, 654 - scrollMargin);
      }
    }
    if (display::settingsPageOneScrollPos >= 7 && display::settingsPageOneScrollPos <= 12) {
      if (display::refreshPage) {
        tft.print(110, 698 - scrollMargin, F("Factory reset"));
        drawMiniConfirmButton(712, 698 - scrollMargin);
      }
    }
  }
}
// =============================================================================================================================================================================================
// =============================================================================================================================================================================================
void drawSettingsPageTwo() {
  if (display::refreshPage) {
    tft.setFont(&akashi_36px_Regular);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_BLUE);
    tft.print(110, 130, F("Warnings"));
    if (display::settingsPageTwoScrollPos > 0)
      tft.fillTriangle(360, 450, 400, 440, 440, 450, RA8875_BLUE);
    if (display::settingsPageTwoScrollPos < 2)
      tft.fillTriangle(360, 455, 400, 465, 440, 455, RA8875_BLUE);
    tft.setTextColor(RA8875_BLACK);
  }
  int scrollMargin = display::settingsPageTwoScrollPos * 44;
  // Mini radio buttons
  if (display::settingsPageTwoScrollPos == 0) {
    static bool previousDisablePpmWarnings;
    if (display::refreshPage || user::disablePpmWarnings != previousDisablePpmWarnings) {
      setTextWarningColor(device::ppmErrorState);
      tft.print(110, 170 - scrollMargin, F("Disable PPM"));
      drawMiniRadioButton(682, 170 - scrollMargin, user::disablePpmWarnings);
      previousDisablePpmWarnings = user::disablePpmWarnings;
    }
  }
  if (display::settingsPageTwoScrollPos <= 1) {
    static bool previousDisableEcWarnings;
    if (display::refreshPage || user::disableEcWarnings != previousDisableEcWarnings) {
      setTextWarningColor(device::ecErrorState);
      tft.print(110, 214 - scrollMargin, F("Disable EC"));
      drawMiniRadioButton(682, 214 - scrollMargin, user::disableEcWarnings);
      previousDisableEcWarnings = user::disableEcWarnings;
    }
  }
  if (display::settingsPageTwoScrollPos <= 2) {
    static bool previousDisablePhWarnings;
    if (display::refreshPage || user::disablePhWarnings != previousDisablePhWarnings) {
      setTextWarningColor(device::phErrorState);
      tft.print(110, 258 - scrollMargin, F("Disable PH"));
      drawMiniRadioButton(682, 258 - scrollMargin, user::disablePhWarnings);
      previousDisablePhWarnings = user::disablePhWarnings;
    }
    static bool previousDisableCo2Warnings;
    if (display::refreshPage || user::disableCo2Warnings != previousDisableCo2Warnings) {
      setTextWarningColor(device::co2ErrorState);
      tft.print(110, 302 - scrollMargin, F("Disable Co2"));
      drawMiniRadioButton(682, 302 - scrollMargin, user::disableCo2Warnings);
      previousDisableCo2Warnings = user::disableCo2Warnings;
    }
    static bool previousDisableWaterTempWarnings;
    if (display::refreshPage || user::disableWaterTempWarnings != previousDisableWaterTempWarnings) {
      setTextWarningColor(device::waterTempErrorState);
      tft.print(110, 346 - scrollMargin, F("Disable water temp"));
      drawMiniRadioButton(682, 346 - scrollMargin, user::disableWaterTempWarnings);
      previousDisableWaterTempWarnings = user::disableWaterTempWarnings;
    }
    static bool previousDisableWaterHeightWarnings;
    if (display::refreshPage || user::disableWaterHeightWarnings != previousDisableWaterHeightWarnings) {
      setTextWarningColor(device::waterLevelErrorState);
      tft.print(110, 390 - scrollMargin, F("Diable water height"));
      drawMiniRadioButton(682, 390 - scrollMargin, user::disableWaterHeightWarnings);
      previousDisableWaterHeightWarnings = user::disableWaterHeightWarnings;
    }
  }
  if (display::settingsPageTwoScrollPos >= 1) {
    static bool previousDisableAirTempWarnings;
    if (display::refreshPage || user::disableAirTempWarnings != previousDisableAirTempWarnings) {
      setTextWarningColor(device::airTempErrorState);
      tft.print(110, 434 - scrollMargin, F("Disable air temp"));
      drawMiniRadioButton(682, 434 - scrollMargin, user::disableAirTempWarnings);
      previousDisableAirTempWarnings = user::disableAirTempWarnings;
    }
  }
  if (display::settingsPageTwoScrollPos >= 2) {
    static bool previousDisableHumidityWarnings;
    if (display::refreshPage || user::disableHumidityWarnings != previousDisableHumidityWarnings) {
      setTextWarningColor(device::humidityErrorState);
      tft.print(110, 478 - scrollMargin, F("Disable humidity"));
      drawMiniRadioButton(682, 478 - scrollMargin, user::disableHumidityWarnings);
      previousDisableHumidityWarnings = user::disableHumidityWarnings;
    }
  }
}
// =============================================================================================================================================================================================
// =============================================================================================================================================================================================
void drawSettingsPageThree() {
  tft.setTextColor(RA8875_BLACK);
  if (user::showSr04HeightCalibration) {
    if (display::refreshPage)
      continueMessage(message::setWaterHeight, -1, 0, true, true, false);
  }
  else if (user::showEtapeCalibration == device::PAGE_ONE) {
    if (display::refreshPage)
      continueMessage(message::setEtapeMinReading, -1, 0, true, true, false);
  }
  else if (user::showEtapeCalibration == device::PAGE_TWO) {
    if (display::refreshPage)
      continueMessage(message::setEtapeMaxReading, -1, 0, true, true, false);
  }
  else if (user::showCo2Calibration) {
    if (display::refreshPage)
      continueMessage(message::calibrateCo2, -1, 0, true, true, false);
  }
  else if (user::showDoserCalibration) {
    if (display::calDoserPageScrollPos > 0) {
      tft.fillTriangle(95, 240, 115, 200, 115, 280, RA8875_BLUE);
    }
    if (display::calDoserPageScrollPos < user::numberOfDosers - 4) {
      tft.fillTriangle(779, 200, 799, 240, 779, 280, RA8875_BLUE);
    }
    int startPosition = display::calDoserPageScrollPos * 178;
    if (display::calDoserPageScrollPos == 0) { // DOSER 1
      static int doserOnSpeedPosition, previousDoserOneSpeed;
      drawDoser(100 - startPosition, 90, 1, user::doserOneSpeed, previousDoserOneSpeed, doserOnSpeedPosition, 1, -45);
    }
    if (display::calDoserPageScrollPos <= 1) { // DOSER 2
      static int doserTwoSpeedPosition, previousDoserTwoSpeed;
      drawDoser(278 - startPosition, 90, 2, user::doserTwoSpeed, previousDoserTwoSpeed, doserTwoSpeedPosition, 1, -45);
    }
    if (display::calDoserPageScrollPos <= 2) { // DOSER 3
      static int doserThreeSpeedPosition, previousDoserThreeSpeed;
      drawDoser(456 - startPosition, 90, 3, user::doserThreeSpeed, previousDoserThreeSpeed, doserThreeSpeedPosition, 1, -45);
    }
    if (display::calDoserPageScrollPos <= 2) { // DOSER 4
      static int doserFourSpeedPosition, previousDoserFourSpeed;
      drawDoser(634 - startPosition, 90, 4, user::doserFourSpeed, previousDoserFourSpeed, doserFourSpeedPosition, 1, -45);
    }
    if (display::calDoserPageScrollPos >= 1) { // DOSER 5
      static int doserFiveSpeedPosition, previousDoserFiveSpeed;
      drawDoser(812 - startPosition, 90, 5, user::doserFiveSpeed, previousDoserFiveSpeed, doserFiveSpeedPosition, 1, -45);
    }
    if (display::calDoserPageScrollPos == 2) { // DOSER 6
      static int doserSixSpeedPosition, previousDoserSixSpeed;
      drawDoser(990 - startPosition, 90, 6, user::doserSixSpeed, previousDoserSixSpeed, doserSixSpeedPosition, 1, -45);
    }
    // buttons
    if (display::refreshPage) {
      drawFourUpDownButtons(0);
      primeButton(141, 308);
      primeButton(319, 308);
      primeButton(499, 308);
      primeButton(676, 308);
      exitButton(384, 415);
    }
  }
  else if (user::showTdsCalibration) {
    if (display::refreshPage) {
      continueMessage(message::calibrateTds, sensor::ecCalibrationSolution, 2, true, true, false);
    }
  }
  else if (user::showPhCalibration) {
    if (display::refreshPage)
      drawSlideIcons(110, 112, display::calPhPageScrollPos, 3);
    if (display::calPhPageScrollPos == 0) {
      if (display::refreshPage) {
        tft.setFont(&akashi_36px_Regular);
        tft.setFontScale(1);
        tft.print(255, 110, F("PH 2-part calibration"));
        if (device::phCalStage == 0) {
          tft.print(120, 170, F("Please place the PH sensor in"));
          tft.print(120, 208, F("PH 7 buffer solution and let it"));
          tft.print(120, 246, F("rest for 1 minute and then"));
          tft.print(120, 284, F("press the continue button"));
          continueButton(340, 415);
        }
        else if (device::phCalStage == 1) {
          tft.print(120, 170, F("Please wait while the system is"));
          tft.print(120, 208, F("calibrating the PH sensor, this"));
          tft.print(120, 246, F("should only take 3 minutes on"));
          tft.print(120, 284, F("average to complete but could"));
          tft.print(120, 322, F("take up to 4 hours."));
        }
        else if (device::phCalStage == 2) {
          tft.print(120, 170, F("Please clean the PH sensor and"));
          tft.print(120, 208, F("then place the PH sensor in PH"));
          tft.print(120, 246, F("4 buffer solution and let it"));
          tft.print(120, 284, F("rest for 1 minute and then"));
          tft.print(120, 322, F("press the continue button."));
          continueButton(340, 415);
        }
        else if (device::phCalStage == 3) {
          tft.print(120, 170, F("Please wait while the system"));
          tft.print(120, 208, F("is calibrating the PH sensor"));
          tft.print(120, 246, F("this will take about 1 minute"));
          tft.print(120, 284, F("to complete."));
        }
        else if (device::phCalStage == 4) {
          tft.print(220, 250, F("PH calibration complete."));
          exitButton(404, 415);
        }
      }
    }
    else if (display::calPhPageScrollPos == 1) {
      int startPosition = 0;
      if (display::refreshPage) {
        tft.setFont(&akashi_36px_Regular);
        tft.setFontScale(1);
        tft.setTextColor(RA8875_BLACK);
        tft.print(242, 110, F("PH advanced calibration"));
        tft.print(132, 166, F("PH4 volts"));
        tft.print(340, 166, F("PH7 volts"));
        tft.print(560, 166, F("PH volts"));
        drawUpDownButtons(184, 366, 262, 366, RA8875_BLUE);
        drawUpDownButtons(389, 366, 467, 366, RA8875_BLUE);
        exitButton(404, 415);
      }
      tft.setTextColor(RA8875_BLACK);
      tft.setFont(&HallfeticaLargenum_42px_Regular);
      tft.setFontScale(1);
      // PH 4 VOLTAGE
      static int ph4VoltagePosition;
      static float previousPh4Voltage;
      if (display::refreshPage || sensor::ph4Voltage != previousPh4Voltage) {
        sensor::ph4Voltage < 10 ? startPosition = 150 : startPosition = 130;
        tft.fillRect(128, 238, ph4VoltagePosition - 126, 50, user::backgroundColor);
        tft.print(startPosition, 240, sensor::ph4Voltage, 3);
        ph4VoltagePosition = tft.getFontX();
        previousPh4Voltage = sensor::ph4Voltage;
      }
      // PH 7 VOLTAGE
      static int ph7VoltagePosition;
      static float previousPh7Voltage;
      if (display::refreshPage || sensor::ph7Voltage != previousPh7Voltage) {
        sensor::ph7Voltage < 10 ? startPosition = 350 : startPosition = 330;
        tft.fillRect(328, 238, ph7VoltagePosition - 326, 50, user::backgroundColor);
        tft.print(startPosition, 240, sensor::ph7Voltage, 3);
        ph7VoltagePosition = tft.getFontX();
        previousPh7Voltage = sensor::ph7Voltage;
      }
      // CURRENT PH VOLTAGE
      static int phVoltagePosition;
      static float previousPhVoltage;
      if (display::refreshPage || sensor::phVoltage != previousPhVoltage) {
        sensor::phVoltage < 10 ? startPosition = 558 : startPosition = 538;
        tft.fillRect(536, 238, phVoltagePosition - 534, 50, user::backgroundColor);
        tft.print(startPosition, 240, sensor::phVoltage, 3);
        phVoltagePosition = tft.getFontX();
        previousPhVoltage = sensor::phVoltage;
      }
    }
    else {
      if (display::refreshPage) {
        tft.setFont(&akashi_36px_Regular);
        tft.setFontScale(1);
        tft.print(242, 110, F("PH advanced calibration"));
        tft.print(116, 170, F("Sensor reading"));
        tft.print(556, 170, F("pot step"));
        drawUpDownButtons(580, 386, 690, 386, RA8875_BLUE);
        exitButton(404, 415);
      }
      drawTwoValues(285, sensor::ph, RA8875_BLACK, 2, 620, sensor::phPotStep, RA8875_BLACK, 0, 0);
    }
  }
  else {
    if (display::refreshPage) {
      tft.setFont(&akashi_36px_Regular);
      tft.setFontScale(1);
      tft.setTextColor(RA8875_BLUE);
      tft.print(110, 130, F("Calibration"));
      tft.setTextColor(RA8875_BLACK);
      tft.print(110, 170, F("Calibrate SR-04"));
      tft.print(110, 214, F("Calibrate TDS"));
      tft.print(110, 258, F("Calibrate Co2"));
      tft.print(110, 302, F("Calibrate PH"));
      tft.print(110, 346, F("Calibrate dosers"));
      tft.print(110, 390, F("Calibrate Etape"));
      drawMiniConfirmButton(712, 170);
      drawMiniConfirmButton(712, 214);
      drawMiniConfirmButton(712, 258);
      drawMiniConfirmButton(712, 302);
      drawMiniConfirmButton(712, 346);
      drawMiniConfirmButton(712, 390);
    }
  }
}
