// !value,value
void esp8266DataHandler() {
  bool getData = false, readData = false;
  uint8_t value = 0, cmdsRecived = 0, charCnt = 0;
  char buffer[16] {0};
  uint32_t timeout = millis() + 100UL;
  if (wifi::wifiEnabled) {
    while (Serial1.available() > 0 && millis() < timeout) {
      char c = Serial1.read();
      // reset local variables
      if (c == '!') {
        memset(buffer, 0 , 16);
        getData = false;
        value = 0;
        cmdsRecived = 0;
        charCnt = 0;
        readData = true;
      }
      // value to be stored
      else if (c == ',' && readData) {
        if (cmdsRecived == 0) {
          getData = atoi(buffer);
        }
        else if (cmdsRecived == 1) {
          value = atoi(buffer);
        }
        else if (cmdsRecived == 2) {
          if (getData)
            setValues(value, buffer);
          else
            getValues(value, atoi(buffer));
        }
        memset(buffer, 0 , 16);
        cmdsRecived++;
        charCnt = 0;
      }
      // add serial data to buffer
      else if (c >= 32 && c <= 126 && charCnt < 16 && readData) { // 'space' to '~'
        buffer[charCnt++] = c;
      }
    }
  }
}

void getValues(const uint8_t a_page, const uint8_t a_slide) {
  Serial.print(__FUNCTION__); Serial.print(F(" page: ")); Serial.print(a_page); Serial.print(F(", slide: ")); Serial.println(a_slide);
    // Page 0 (Home page)
  if (a_page == 0) {
    Serial1.print(user::convertToF); 
    Serial1.print(user::convertToInches); 
    if (a_slide == 0) { // home page slide 0
      Serial1.print(sensor::ec, 2);
      Serial1.print(user::targetMinEc, 2); 
      Serial1.print(user::targetMaxEc, 2); 
      printShortArray(sensor::ecArray, 24);   
    }
    else if (a_slide == 1) { // home page slide 1
      Serial1.print(sensor::tds);
      Serial1.print(user::targetMinTds); 
      Serial1.print(user::targetMaxTds); 
      printShortArray(sensor::tdsArray, 24);   
    }
    else if (a_slide == 2) { // home page slide 2
      Serial1.print(sensor::ph, 2); 
      Serial1.print(user::targetMinPh, 2); 
      Serial1.print(user::targetMaxPh, 2); 
      printShortArray(sensor::phArray, 24);
    }
    else if (a_slide == 3) { // home page slide 3
      Serial1.print(sensor::co2); 
      Serial1.print(user::targetCo2); 
      Serial1.print(user::targetCo2); 
      printShortArray(sensor::co2Array, 24);
    }
    else if (a_slide == 4) { // home page slide 4
      if (user::convertToF) {
        Serial1.print(convertToF(sensor::waterTemp), 1); 
        Serial1.print(user::targetMinWaterTempF, 1); 
        Serial1.print(user::targetMaxWaterTempF, 1); 
        printShortArray(sensor::waterTemperatureArrayF, 24);
      }
      else {
        Serial1.print(sensor::waterTemp, 1); 
        Serial1.print(user::targetMinWaterTemp, 1); 
        Serial1.print(user::targetMaxWaterTemp, 1); 
        printShortArray(sensor::waterTemperatureArray, 24);
      }
    }
    else if (a_slide == 5) { // home page slide 5
      if (user::convertToInches) {
        Serial1.print(convertToInches(sensor::waterLevel), 1); 
        Serial1.print(user::targetMinWaterHeightInches, 1); 
        Serial1.print(user::targetMaxWaterHeightInches, 1); 
        printShortArray(sensor::waterLevelArrayInInches, 24);
      }
      else {
        Serial1.print(sensor::waterLevel, 1); 
        Serial1.print(user::targetMinWaterHeight, 1); 
        Serial1.print(user::targetMaxWaterHeight, 1); 
        printShortArray(sensor::waterLevelArray, 24);
      }
    }
    else if (a_slide == 6) { // home page slide 6
      if (user::convertToF) {
        Serial1.print(convertToF(sensor::airTemp), 1); 
        Serial1.print(user::targetMinAirTempF, 1); 
        Serial1.print(user::targetMaxAirTempF, 1); 
        printShortArray(sensor::airTemperatureArrayF, 24);
      }
      else {
        Serial1.print(sensor::airTemp, 1); 
        Serial1.print(user::targetMinAirTemp, 1); 
        Serial1.print(user::targetMaxAirTemp, 1); 
        printShortArray(sensor::airTemperatureArray, 24);
      }
    }
    else if (a_slide == 7) { // home page slide 7
      Serial1.print(sensor::humidity, 1); 
      Serial1.print(user::targetMinHumidity, 1); 
      Serial1.print(user::targetMaxHumidity, 1); 
      printShortArray(sensor::humidityArray, 24);
    }
    else if (a_slide == 8) { // home page slide 8
      Serial1.print(device::fanOneSpeed); 
      Serial1.print(user::targetMinFanOneSpeed); 
      Serial1.print(user::targetMaxFanOneSpeed); 
      printShortArray(sensor::fanOneSpeedArray, 24);
    }
    else if (a_slide == 9) { // home page slide 9
      Serial1.print(device::fanTwoSpeed); 
      Serial1.print(user::targetMinFanTwoSpeed); 
      Serial1.print(user::targetMaxFanTwoSpeed); 
      printShortArray(sensor::fanTwoSpeedArray, 24);
    }
  }
  // Page 1 (Graphs page)
  else if (a_page == 1) {
    Serial1.print(user::convertToF); 
    Serial1.print(user::convertToInches); 
    if (a_slide == 0) {
      printShortArray(sensor::ecArray, 24);     
    }
    else if (a_slide == 1) {
       printShortArray(sensor::tdsArray, 24); 
    }
    else if (a_slide == 2) {
      printShortArray(sensor::phArray, 24);
    }
    else if (a_slide == 3) {
      printShortArray(sensor::co2Array, 24);
    }
    else if (a_slide == 4) {
      if (user::convertToF)
        printShortArray(sensor::waterTemperatureArrayF, 24);
      else
        printShortArray(sensor::waterTemperatureArray, 24);
    }
    else if (a_slide == 5) {
      if (user::convertToInches) 
        printShortArray(sensor::waterLevelArrayInInches, 24);
      else
        printShortArray(sensor::waterLevelArray, 24);
    }
    else if (a_slide == 6) {
      if (user::convertToF)
        printShortArray(sensor::airTemperatureArrayF, 24);
      else
        printShortArray(sensor::airTemperatureArray, 24);
    }
    else if (a_slide == 7) {
      printShortArray(sensor::humidityArray, 24);
    }
    else if (a_slide == 8) {
      printShortArray(sensor::fanOneSpeedArray, 24);
    }
    else if (a_slide == 9) {
      printShortArray(sensor::fanTwoSpeedArray, 24);
    }
  }
   // Page 2 (Max mins page)
  else if (a_page == 2) {
    Serial1.print(user::convertToF); 
    Serial1.print(user::convertToInches); 
    if (a_slide == 0) {
      Serial1.print(device::minEc, 2); 
      Serial1.print(device::maxEc, 2); 
    }
    else if (a_slide == 1) {
      Serial1.print(device::minTds); 
      Serial1.print(device::maxTds); 
    }
    else if (a_slide == 2) {
      Serial1.print(device::minPh, 2); 
      Serial1.print(device::maxPh, 2); 
    }
    else if (a_slide == 3) {
      Serial1.print(device::minCo2); 
      Serial1.print(device::maxCo2); 
    }
    else if (a_slide == 4) {
      if (user::convertToF) {
        Serial1.print(convertToF(device::minWaterTemp), 1); 
        Serial1.print(convertToF(device::maxWaterTemp), 1); 
      }
      else {
        Serial1.print(device::minWaterTemp, 1); 
        Serial1.print(device::maxWaterTemp, 1); 
      }
    }
    else if (a_slide == 5) {
      if (user::convertToInches) {
        Serial1.print(convertToInches(device::minWaterLevel), 1); 
        Serial1.print(convertToInches(device::maxWaterLevel), 1); 
      }
      else {
        Serial1.print(device::minWaterLevel, 1); 
        Serial1.print(device::maxWaterLevel, 1); 
      }
    }
    else if (a_slide == 6) {
      if (user::convertToF) {
        Serial1.print(convertToF(device::minAirTemp), 1); 
        Serial1.print(convertToF(device::maxAirTemp), 1); 
      }
      else {
        Serial1.print(device::minAirTemp, 1); 
        Serial1.print(device::maxAirTemp, 1); 
      }
    }
    else if (a_slide == 7) {
      Serial1.print(device::minHumidity, 1); 
      Serial1.print(device::maxHumidity, 1); 
    }
    else if (a_slide == 8) {
      Serial1.print(device::minFanOneSpeed); 
      Serial1.print(device::maxFanOneSpeed); 
    }
    else if (a_slide == 9) {
      Serial1.print(device::minFanTwoSpeed, 1); 
      Serial1.print(device::maxFanTwoSpeed, 1); 
    }
  }
  // Page 3 (Profiles page)
  else if (a_page == 3) {
    Serial1.print(user::profileOneName);
    Serial1.print(user::profileTwoName);
    Serial1.print(user::profileThreeName);
    Serial1.print(user::profileFourName);
    Serial1.print(user::profileOneName);
  }
  // Page 4 (Settings page)
  else if (a_page == 4) {
    Serial1.print(user::numberOfDosers); 
    Serial1.print(user::dosingInterval); 
    Serial1.print(user::convertToF); 
    Serial1.print(user::convertToInches); 
    Serial1.print(user::convertToTds); 
    Serial1.print(user::disablePpmWarnings);
    Serial1.print(user::disableEcWarnings);
    Serial1.print(user::disableCo2Warnings);
    Serial1.print(user::disablePhWarnings);
    Serial1.print(user::disableWaterTempWarnings);
    Serial1.print(user::disableWaterHeightWarnings);
    Serial1.print(user::disableAirTempWarnings);
    Serial1.print(user::disableHumidityWarnings);
  }
  // Page 5 (EC\TDS page)
  else if (a_page == 5) {
    Serial1.print(user::convertToTds); 
    Serial1.print(user::targetMinTds); 
    Serial1.print(user::targetMaxTds); 
    Serial1.print(user::targetMinEc, 2); 
    Serial1.print(user::targetMaxEc, 2); 
  }
  // Page 6 (PH page)
  else if (a_page == 6) {
    Serial1.print(user::targetMinPh, 2); 
    Serial1.print(user::targetMaxPh, 2); 
  }
  // Page 7 (Co2 page) 
  else if (a_page == 7) { // small amount of data so no need to segment based on slider 
    Serial1.print(user::convertToInches); 
    if (user::convertToInches) {
      Serial1.print(user::roomLengthInches, 1);
      Serial1.print(user::roomWidthInches, 1);
      Serial1.print(user::roomHeightInches, 1);
      Serial1.print(user::co2FlowrateFeet3, 1);
    }
    else {
      Serial1.print(user::roomLengthCm, 1);
      Serial1.print(user::roomWidthCm, 1);
      Serial1.print(user::roomHeightCm, 1);
      Serial1.print(user::co2FlowrateLtrs, 1);
    }
    Serial1.print(user::co2CheckTimeMinute);
    Serial1.print(user::co2CheckTimeHour);
    Serial1.print(user::disableFansTimer);
    Serial1.print(user::enableManualCo2Duration);
    Serial1.print(user::disableCo2Control);
    Serial1.print(user::manualCo2GasDuration);
  }
  // Page 8 (Water page)
  else if (a_page == 8) { // small amount of data so no need to segment based on slider
    Serial1.print(user::convertToF); 
    Serial1.print(user::convertToInches); 
    if (user::convertToInches) {
      Serial1.print(user::targetMinWaterHeightInches, 1);
      Serial1.print(user::targetMaxWaterHeightInches, 1);
    }
    else {
      Serial1.print(user::targetMinWaterHeight, 1);
      Serial1.print(user::targetMaxWaterHeight, 1);
    }
    if (user::convertToF) {
      Serial1.print(user::targetMinWaterTempF, 1);
      Serial1.print(user::targetMaxWaterTempF, 1);
    }
    else {
      Serial1.print(user::targetMinWaterTemp, 1);
      Serial1.print(user::targetMaxWaterTemp, 1);
    }
    if (user::convertToInches) {
      Serial1.print(user::waterTankLengthInches, 1);
      Serial1.print(user::waterTankWidthInches, 1);
    }
    else {
      Serial1.print(user::waterTankLength, 1);
      Serial1.print(user::waterTankWidth, 1);
    }
    Serial1.print(user::autoFillDays);
    Serial1.print(user::autoFillMinute);
    Serial1.print(user::autoFillHour);
    Serial1.print(user::disableDrainAndRefill);
    Serial1.print(user::refillDoserOneMills);
    Serial1.print(user::refillDoserTwoMills);
    Serial1.print(user::refillDoserThreeMills);
    Serial1.print(user::refillDoserFourMills);
    Serial1.print(user::refillDoserFiveMills);
    Serial1.print(user::refillDoserSixMills);
  }
  // Page 9 (Doser page) 
  else if (a_page == 9) {
    Serial1.print(user::numberOfDosers); 
    Serial1.print(user::doserOneMode);
    Serial1.print(user::doserTwoMode);
    Serial1.print(user::doserThreeMode);
    Serial1.print(user::doserFourMode);
    Serial1.print(user::doserFiveMode);
    Serial1.print(user::doserSixMode);
    Serial1.print(user::doserOneMills);
    Serial1.print(user::doserTwoMills);
    Serial1.print(user::doserThreeMills);
    Serial1.print(user::doserFourMills);
    Serial1.print(user::doserFiveMills);
    Serial1.print(user::doserSixMills);
    //doserOneSpeed = values[cnt++];
    //doserTwoSpeed = values[cnt++];
    //doserThreeSpeed = values[cnt++];
    //doserFourSpeed = values[cnt++];
    //doserFiveSpeed = values[cnt++];
    //doserSixSpeed = values[cnt++];
  }
  // Page 10 (Lighting page) 
  else if (a_page == 10) {
    Serial1.print(user::lightOnTimeMin);
    Serial1.print(user::lightOnTimeHour);
    Serial1.print(user::lightOffTimeMin);
    Serial1.print(user::lightOffTimeHour);
    Serial1.print(user::lightMode);
  }
  // Page 11 (Fans page)
  else if (a_page == 11) { // small amount of data so no need to segment based on slider
    Serial1.print(user::convertToF); 
    Serial1.print(user::targetMinFanOneSpeed);
    Serial1.print(user::targetMaxFanOneSpeed);
    Serial1.print(user::targetMinFanTwoSpeed);
    Serial1.print(user::targetMaxFanTwoSpeed);
    if (user::convertToF) {
      Serial1.print(user::targetMinAirTempF, 1);
      Serial1.print(user::targetMaxAirTempF, 1);
    }
    else {
      Serial1.print(user::targetMinAirTemp, 1);
      Serial1.print(user::targetMaxAirTemp, 1);
    }
    Serial1.print(user::targetMinHumidity, 1);
    Serial1.print(user::targetMaxHumidity, 1);
    Serial1.print(user::fansControlTemperature);
    Serial1.print(user::fansControlHumidity);
    Serial1.print(user::fanOneFixedSpeed);
    Serial1.print(user::fanTwoFixedSpeed);
  }
  // Page 12 (Warnings page)
  else if (a_page == 12) {  // small amount of data so no need to segment based on slider
    Serial1.print(user::convertToF); 
    Serial1.print(user::convertToInches); 
    Serial1.print(sensor::tds);
    Serial1.print(user::targetMinTds);
    Serial1.print(user::targetMaxTds);
    Serial1.print(user::tdsErrorMargin);
    Serial1.print(sensor::ec, 2);
    Serial1.print(user::targetMinEc, 2);
    Serial1.print(user::targetMaxEc, 2);
    Serial1.print(user::ecErrorMargin, 2);
    Serial1.print(sensor::ph, 2);
    Serial1.print(user::targetMinPh, 2);
    Serial1.print(user::targetMaxPh, 2);
    Serial1.print(user::phErrorMargin, 2);
    Serial1.print(sensor::co2);
    Serial1.print(user::targetCo2);
    Serial1.print(user::targetCo2);
    Serial1.print(user::co2ErrorMargin);
    if (user::convertToF) {
      Serial1.print(convertToF(sensor::waterTemp), 1);
      Serial1.print(user::targetMinWaterTempF, 1);
      Serial1.print(user::targetMaxWaterTempF, 1);
      Serial1.print(user::waterTempErrorMarginF, 1);
    }
    else {
      Serial1.print(sensor::waterTemp, 1);
      Serial1.print(user::targetMinWaterTemp, 1);
      Serial1.print(user::targetMaxWaterTemp, 1);
      Serial1.print(user::waterTempErrorMargin, 1);
    }
    if (user::convertToInches) {
      Serial1.print(convertToInches(sensor::waterLevel), 1);
      Serial1.print(user::targetMinWaterHeightInches, 1);
      Serial1.print(user::targetMaxWaterHeightInches, 1);
      Serial1.print(user::waterHeightErrorMarginInches, 1);
    }
    else {
      Serial1.print(sensor::waterLevel, 1);
      Serial1.print(user::targetMinWaterHeight, 1);
      Serial1.print(user::targetMaxWaterHeight, 1);
      Serial1.print(user::waterHeightErrorMargin, 1);
    }
    if (user::convertToF) {
      Serial1.print(convertToF(sensor::airTemp), 1);
      Serial1.print(user::targetMinAirTempF, 1);
      Serial1.print(user::targetMaxAirTempF, 1);
      Serial1.print(user::airTempErrorMarginF, 1);
    }
    else {
      Serial1.print(sensor::airTemp, 1);
      Serial1.print(user::targetMinAirTemp, 1);
      Serial1.print(user::targetMaxAirTemp, 1);
      Serial1.print(user::airTempErrorMargin, 1);
    }
    Serial1.print(sensor::humidity, 1);
    Serial1.print(user::targetMinHumidity, 1);
    Serial1.print(user::targetMaxHumidity, 1);
    Serial1.print(user::humidityErrorMargin, 1);
  }
  else {
    Serial.print(F("Page not found!"));
  }
}

void setValues(const uint8_t a_cmd, const char* a_val) {
  Serial.print(__FUNCTION__); Serial.print(F(" cmd: ")); Serial.print(a_cmd); Serial.print(F(", val: ")); Serial.println(a_val);
  // ============== PROFILES PAGE  ==============
  // profileOneName
  if (a_cmd ==0) {
    buffToCharArray(a_val, user::profileOneName, 11);
  }
  // profileTwoName
  else if (a_cmd ==1) {
    buffToCharArray(a_val, user::profileTwoName, 11);
  }
  // profileThreeName
  else if (a_cmd ==2) {
    buffToCharArray(a_val, user::profileThreeName, 11);
  }
  // profileFourName
  else if (a_cmd ==3) {
    buffToCharArray(a_val, user::profileFourName, 11);
  }
  // profileFiveName
  else if (a_cmd ==4) {
    buffToCharArray(a_val, user::profileFiveName, 11);
  }
  // loadData
  else if (a_cmd ==5) {
    wifi::remotlyLoadUserProfile = atoi(a_val);
  }
  // saveData
  else if (a_cmd ==6) {
    wifi::remotlySaveUserProfile = atoi(a_val);
  }
  // ============== SETTINGS PAGE  ==============
  // numberOfDosers
  else if (a_cmd ==7) {
    user::numberOfDosers = atoi(a_val);
  }
  // dosingInterval
  else if (a_cmd ==8) {
    user::dosingInterval = atoi(a_val);
  }
  // convertToFarenheit
  else if (a_cmd ==9) {
    user::convertToF = strBool(a_val);
  }
  // convertToInchess
  else if (a_cmd ==10) {
    user::convertToInches = strBool(a_val);
  }
  // convertToTds
  else if (a_cmd ==11) {
    user::convertToTds = strBool(a_val);
  }
  // disableCo2Warnings
  else if (a_cmd ==12) {
    user::disableCo2Warnings = strBool(a_val);
  }
  // disableEcWarnings
  else if (a_cmd ==13) {
    user::disableEcWarnings = strBool(a_val);
  }
  // disablePpmWarnings
  else if (a_cmd ==14) {
    user::disablePpmWarnings = strBool(a_val);
  }
  // disablePhWarnings
  else if (a_cmd ==15) {
    user::disablePhWarnings = strBool(a_val);
  }
  // disableWaterTempWarnings
  else if (a_cmd ==16) {
    user::disableWaterTempWarnings = strBool(a_val);
  }
  // disableWaterHeightWarnings
  else if (a_cmd ==17) {
    user::disableWaterHeightWarnings = strBool(a_val);
  }
  // disableAirTempWarnings
  else if (a_cmd ==18) {
    user::disableAirTempWarnings = strBool(a_val);
  }
  // disableHumidityWarnings
  else if (a_cmd ==19) {
    user::disableHumidityWarnings = strBool(a_val);
  }
  // ============== EC PAGE ==============
  // targetMinEc
  else if (a_cmd ==20) {
    user::targetMinEc = atof(a_val);
  }
  // targetMaxEc
  else if (a_cmd ==21) {
    user::targetMaxEc = atof(a_val);
  }
  // ============== TDS/PPM PAGE ==============
  // targetMinTds
  else if (a_cmd ==22) {
    user::targetMinTds = atof(a_val);
  }
  // targetMaxTds
  else if (a_cmd ==23) {
    user::targetMaxTds = atof(a_val);
  }
  // ============== PH PAGE ==============
  // targetMinPh
  else if (a_cmd ==24) {
    user::targetMinPh = atof(a_val);
  }
  // targetMaxPh
  else if (a_cmd ==25) {
    user::targetMaxPh = atof(a_val);
  }
  // ============== Co2 PAGE ==============
  // targetCo2
  else if (a_cmd ==26) {
    user::targetCo2 = atoi(a_val);
  }
  // co2Offset
  else if (a_cmd ==27) {
    user::co2Offset = atoi(a_val);
  }
  // roomLength
  else if (a_cmd ==28) {
    (user::convertToInches ? user::roomLengthInches : user::roomLengthCm) = atoi(a_val);
  }
  // roomWidth
  else if (a_cmd ==29) {
      (user::convertToInches ? user::roomWidthInches : user::roomWidthCm) = atoi(a_val);
  }
  // roomHeight
  else if (a_cmd ==30) {
    (user::convertToInches ? user::roomHeightInches : user::roomHeightCm) = atoi(a_val);
  }
  // co2Flowrate
  else if (a_cmd ==31) {
    (user::convertToInches ? user::co2FlowrateFeet3 : user::co2FlowrateLtrs) = atof(a_val);
  }
  // co2CheckTimeMinute
  else if (a_cmd ==32) {
    user::co2CheckTimeMinute = atoi(a_val);
  }
  // co2CheckTimeHour
  else if (a_cmd ==33) {
    user::co2CheckTimeHour = atoi(a_val);
  }
  // disableFansTimer
  else if (a_cmd ==34) {
    user::disableFansTimer = atoi(a_val);
  }
  // manualCo2Duration
  else if (a_cmd ==35) {
    user::enableManualCo2Duration = strBool(a_val);
  }
  // manualCo2GasDuration
  else if (a_cmd ==36) {
    user::manualCo2GasDuration = atoi(a_val);
  }
  // disableCo2Control
  else if (a_cmd ==37) {
    user::disableCo2Control = strBool(a_val);
  }
  // ============== WATER PAGE ==============
  // targetMinWaterHeight
  else if (a_cmd ==38) {
    (user::convertToInches ? user::targetMinWaterHeightInches : user::targetMinWaterHeight) = atof(a_val);
  }
  // targetMaxWaterHeight
  else if (a_cmd ==39) {
    (user::convertToInches ? user::targetMaxWaterHeightInches : user::targetMaxWaterHeight) = atof(a_val);
  }
  // targetMinWaterTemp
  else if (a_cmd ==40) {
    (user::convertToF ? user::targetMinWaterTempF : user::targetMinWaterTemp) = atof(a_val);
  }
  // targetMaxWaterTemp
  else if (a_cmd ==41) {
    (user::convertToF ? user::targetMaxWaterTempF : user::targetMaxWaterTemp) = atof(a_val);
  }
  // targetWaterTankLength
  else if (a_cmd ==42) {
    (user::convertToInches ? user::waterTankLengthInches : user::waterTankLength) = atof(a_val);
  }
  // targetWaterTankWidth
  else if (a_cmd ==43) {
    (user::convertToInches ? user::waterTankWidthInches : user::waterTankWidth) = atof(a_val);
  }
  // autoFillDays
  else if (a_cmd ==44) {
    user::autoFillDays = atol(a_val);
  }
  // autoFillMinute
  else if (a_cmd ==45) {
    user::autoFillMinute = atoi(a_val);
  }
  // autoFillHour
  else if (a_cmd ==46) {
    user::autoFillHour = atoi(a_val);
  }
  // disableDrainAndRefill
  else if (a_cmd ==47) {
    user::disableDrainAndRefill = strBool(a_val);
  }
  // refillDoserOneMills
  else if (a_cmd ==48) {
    user::refillDoserOneMills = atoi(a_val);
  }
  // refillDoserTwoMills
  else if (a_cmd ==49) {
    user::refillDoserTwoMills = atoi(a_val);
  }
  // refillDoserThreeMills
  else if (a_cmd ==50) {
    user::refillDoserThreeMills = atoi(a_val);
  }
  // refillDoserFourMills
  else if (a_cmd ==51) {
    user::refillDoserFourMills = atoi(a_val);
  }
  // refillDoserFiveMills
  else if (a_cmd ==52) {
    user::refillDoserFiveMills = atoi(a_val);
  }
  // refillDoserSixMills
  else if (a_cmd ==53) {
    user::refillDoserSixMills = atoi(a_val);
  }
  // ============== DOSER PAGE ==============
  // doserOneMills
  else if (a_cmd ==54) {
    user::doserOneMills = atoi(a_val);
  }
  // doserTwoMills
  else if (a_cmd ==55) {
    user::doserTwoMills = atoi(a_val);
  }
  // doserThreeMills
  else if (a_cmd ==56) {
    user::doserThreeMills = atoi(a_val);
  }
  // doserFourMills
  else if (a_cmd ==57) {
    user::doserFourMills = atoi(a_val);
  }
  // doserFiveMills
  else if (a_cmd ==58) {
    user::doserFiveMills = atoi(a_val);
  }
  // doserSixMills
  else if (a_cmd ==59) {
    user::doserSixMills = atoi(a_val);
  }
  // doserOneMode
  else if (a_cmd ==60) {
    user::doserOneMode = atoi(a_val);
  }
  // doserTwoMode
  else if (a_cmd ==61) {
    user::doserTwoMode = atoi(a_val);
  }
  // doserThreeMode
  else if (a_cmd ==62) {
    user::doserThreeMode = atoi(a_val);
  }
  // doserFourMode
  else if (a_cmd ==63) {
    user::doserFourMode = atoi(a_val);
  }
  // doserFiveMode
  else if (a_cmd ==64) {
    user::doserFiveMode = atoi(a_val);
  }
  // doserSixMode
  else if (a_cmd ==65) {
    user::doserSixMode = atoi(a_val);
  }
  // ============== LIGHT PAGE ==============
  // lightOnTimeMin
  else if (a_cmd ==66) {
    user::lightOnTimeMin = atoi(a_val);
  }
  // lightOnTimeHour
  else if (a_cmd ==67) {
    user::lightOnTimeHour = atoi(a_val);
  }
  // lightOffTimeMin
  else if (a_cmd ==68) {
    user::lightOffTimeMin = atoi(a_val);
  }
  // lightOffTimeHour
  else if (a_cmd ==69) {
    user::lightOffTimeHour = atoi(a_val);
  }
  // lightMode
  else if (a_cmd ==70) {
    user::lightMode = atoi(a_val);
  }
  // ============== FANS PAGE ==============
  // targetMinFanOneSpeed
  else if (a_cmd ==71) {
    user::targetMinFanOneSpeed = atoi(a_val);
  }
  // targetMaxFanOneSpeed
  else if (a_cmd ==72) {
    user::targetMaxFanOneSpeed = atoi(a_val);
  }
  // targetMinFanTwoSpeed
  else if (a_cmd ==73) {
    user::targetMinFanTwoSpeed = atoi(a_val);
  }
  // targetMaxFanTwoSpeed
  else if (a_cmd ==74) {
    user::targetMaxFanTwoSpeed = atoi(a_val);
  }
  // targetMinAirTemp
  else if (a_cmd ==75) {
    (user::convertToF ? user::targetMinAirTempF : user::targetMinAirTemp) = atof(a_val);
  }
  // targetMaxAirTemp
  else if (a_cmd ==76) {
    (user::convertToF ? user::targetMaxAirTempF : user::targetMaxAirTemp) = atof(a_val);
  }
  // targetMinHumidity
  else if (a_cmd ==77) {
    user::targetMinHumidity = atof(a_val);
  }
  // targetMaxHumidity
  else if (a_cmd ==78) {
    user::targetMaxHumidity = atof(a_val);
  }
  // fansControlTemp
  else if (a_cmd ==79) {
    user::fansControlTemperature = strBool(a_val);
  }
  // fansControlHum
  else if (a_cmd ==80) {
    user::fansControlHumidity = strBool(a_val);
  }
  // fanOneFixedSpeed
  else if (a_cmd ==81) {
    user::fanOneFixedSpeed = strBool(a_val);
  }
  // fanTwoFixedSpeed
  else if (a_cmd ==82) {
    user::fanTwoFixedSpeed = strBool(a_val);
  }
  // ============== WARNINGS PAGE ==============
  // ecErrorMargin
  else if (a_cmd ==83) {
    user::ecErrorMargin = atof(a_val);
  }
  // tdsErrorMargin
  else if (a_cmd ==84) {
    user::tdsErrorMargin = atoi(a_val);
  }
  // phErrorMargin
  else if (a_cmd ==85) {
    user::phErrorMargin = atof(a_val);
  }
  // co2ErrorMargin
  else if (a_cmd ==86) {
    user::co2ErrorMargin = atoi(a_val);
  }
  // waterHeightErrorMargin
  else if (a_cmd ==87) {
    (user::convertToInches ? user::waterHeightErrorMarginInches :user::waterHeightErrorMargin) = atof(a_val);
  }
  // waterTempErrorMargin
  else if (a_cmd ==88) {
    (user::convertToF ? user::waterTempErrorMarginF :user::waterTempErrorMargin) = atof(a_val);
  }
  // airTempErrorMargin
  else if (a_cmd ==89) {
    (user::convertToF ? user::airTempErrorMarginF :user::airTempErrorMargin) = atof(a_val);
  }
  // humidityErrorMargin
  else if (a_cmd ==90) {
    user::humidityErrorMargin = atof(a_val);
  }
  else
    Serial.println(F("cmd not recognised!"));
  //server.send(200, "text/plane", "OK");
}

bool strBool(const char* buffer) {
  if (strstr(buffer, "true") != NULL)
    return true;
  else
    return false;
}

void printShortArray(const uint16_t* a_array, const int16_t a_len) {
  for (int i = 0; i < a_len; i++) {
    Serial1.print(a_array[i]);
  }
}

void buffToCharArray(const char* a_src, char* a_dest, const int16_t a_size) {
  for (uint8_t i = 0; i < a_size; i++) {
    a_dest[i] = a_src[i];
  }
  delay(1);
}