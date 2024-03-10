// !value,value
void esp8266DataHandler() {
  static bool getData = false, readData = false;
  static uint16_t value = 0, cmdsRecived = 0, charCnt = 0;
  static char buffer[32] {0};
  //uint32_t timeout = millis() + 100UL;
  if (wifi::wifiEnabled) {
    while (Serial1.available() > 0) {// && millis() < timeout
      char c = Serial1.read();
      // reset local variables
      if (c == '!') {
        memset(buffer, 0, 32);
        getData = false;
        value = 0;
        cmdsRecived = 0;
        charCnt = 0;
        readData = true;
      }
      // Print any debug mesages 
      //else if (c == '?') { 
      //  Serial.print(F("Dbgmsg: "));
      //  while (c != '\n' && c != '\0' && Serial1.available() > 0) {
      //    c = Serial1.read();
      //    Serial.print(c);
      //  }
      //}
      // value to be stored
      else if (c == ',' && readData) {
        //Serial.print(F("cmdsRecived: ")); Serial.print(cmdsRecived); Serial.print(F(", buffer: ")); Serial.println(buffer);
        if (cmdsRecived == 0) {
          getData = atoi(buffer);
        }
        else if (cmdsRecived == 1) {
          value = atoi(buffer);
        }
        else if (cmdsRecived == 2) {
          if (getData)
            getValues(value, atoi(buffer));
          else
            setValues(value, buffer);
          value = 0;
          cmdsRecived = 0;
          getData = false;
          readData = false;
        }
        memset(buffer, 0 , 32);
        cmdsRecived++;
        charCnt = 0;     
      }
      // add serial data to buffer
      else if (c >= 32 && c <= 126 && charCnt < 32 && readData) { // 'space' to '~'
        buffer[charCnt++] = c;
      }
    }
  }
}

void getValues(const uint16_t a_page, const uint16_t a_slide) {
  Serial.print(F("getValues: ")); Serial.print(a_page); Serial.print(F(", ")); Serial.println(a_slide); // __FUNCTION__ is not stored in PROGMEM
  Serial1.print(F("!")); delay(1); // Start of transmission
  // Page 0 (Home page)
  if (a_page == 0) {
    printInt(device::lightOn);
    printInt(user::convertToF);
    printInt(user::convertToInches); 
    printInt(sensor::sensorArrayPos); 
    if (a_slide == 0) { // home page slide 0
      printFloat(sensor::ec, 2);
      printFloat(user::targetMinEc, 2); 
      printFloat(user::targetMaxEc, 2); 
      printShortArray(sensor::ecArray, 24, true);   
    }
    else if (a_slide == 1) { // home page slide 1
      printInt(sensor::tds);
      printInt(user::targetMinTds); 
      printInt(user::targetMaxTds); 
      printShortArray(sensor::tdsArray, 24, false);   
    }
    else if (a_slide == 2) { // home page slide 2
      printFloat(sensor::ph, 2); 
      printFloat(user::targetMinPh, 2); 
      printFloat(user::targetMaxPh, 2); 
      printShortArray(sensor::phArray, 24, true);
    }
    else if (a_slide == 3) { // home page slide 3
      printInt(sensor::co2); 
      printInt(user::targetCo2); 
      printInt(user::targetCo2); 
      printShortArray(sensor::co2Array, 24, false);
    }
    else if (a_slide == 4) { // home page slide 4
      if (user::convertToF) {
        printFloat(convertToF(sensor::waterTemp), 1); 
        printFloat(user::targetMinWaterTempF, 1); 
        printFloat(user::targetMaxWaterTempF, 1); 
        printShortArray(sensor::waterTemperatureArrayF, 24, true);
      }
      else {
        printFloat(sensor::waterTemp, 1); 
        printFloat(user::targetMinWaterTemp, 1); 
        printFloat(user::targetMaxWaterTemp, 1); 
        printShortArray(sensor::waterTemperatureArray, 24, true);
      }
    }
    else if (a_slide == 5) { // home page slide 5
      if (user::convertToInches) {
        printFloat(convertToInches(sensor::waterLevel), 1); 
        printFloat(user::targetMinWaterHeightInches, 1); 
        printFloat(user::targetMaxWaterHeightInches, 1); 
        printShortArray(sensor::waterLevelArrayInInches, 24, true);
      }
      else {
        printFloat(sensor::waterLevel, 1); 
        printFloat(user::targetMinWaterHeight, 1); 
        printFloat(user::targetMaxWaterHeight, 1); 
        printShortArray(sensor::waterLevelArray, 24, true);
      }
    }
    else if (a_slide == 6) { // home page slide 6      
      if (user::convertToF) {    
        printFloat(convertToF(sensor::airTemp), 1); 
        device::lightOn ? printFloat(user::targetDayMinAirTempF, 1) : printFloat(user::targetNightMinAirTempF, 1); 
        device::lightOn ? printFloat(user::targetDayMaxAirTempF, 1) : printFloat(user::targetNightMaxAirTempF, 1); 
        printShortArray(sensor::airTemperatureArrayF, 24, true);
      }
      else {
        printFloat(sensor::airTemp, 1); 
        device::lightOn ? printFloat(user::targetDayMinAirTemp, 1) : printFloat(user::targetNightMinAirTemp, 1); 
        device::lightOn ? printFloat(user::targetDayMaxAirTemp, 1) : printFloat(user::targetNightMaxAirTemp, 1);        
        printShortArray(sensor::airTemperatureArray, 24, true);
      }
    }
    else if (a_slide == 7) { // home page slide 7
      printFloat(sensor::humidity, 1); 
      device::lightOn ? printFloat(user::targetDayMinHumidity, 1) : printFloat(user::targetNightMinHumidity, 1); 
      device::lightOn ? printFloat(user::targetDayMaxHumidity, 1) : printFloat(user::targetNightMaxHumidity, 1); 
      printShortArray(sensor::humidityArray, 24, true);
    }
    else if (a_slide == 8) { // home page slide 8
      printInt(device::fanOneSpeed); 
      printInt(user::targetMinFanOneSpeed); 
      printInt(user::targetMaxFanOneSpeed); 
      printShortArray(sensor::fanOneSpeedArray, 24, false);
    }
    else if (a_slide == 9) { // home page slide 9
      printInt(device::fanTwoSpeed); 
      printInt(user::targetMinFanTwoSpeed); 
      printInt(user::targetMaxFanTwoSpeed); 
      printShortArray(sensor::fanTwoSpeedArray, 24, false);
    }
  }
  // Page 1 (Graphs page)
  else if (a_page == 1) {
    printInt(user::convertToF); 
    printInt(user::convertToInches); 
    printInt(sensor::sensorArrayPos); 
    if (a_slide == 0) {
      printShortArray(sensor::ecArray, 24, true);     
    }
    else if (a_slide == 1) {
       printShortArray(sensor::tdsArray, 24, false); 
    }
    else if (a_slide == 2) {
      printShortArray(sensor::phArray, 24, true);
    }
    else if (a_slide == 3) {
      printShortArray(sensor::co2Array, 24, false);
    }
    else if (a_slide == 4) {
      if (user::convertToF)
        printShortArray(sensor::waterTemperatureArrayF, 24, true);
      else
        printShortArray(sensor::waterTemperatureArray, 24, true);
    }
    else if (a_slide == 5) {
      if (user::convertToInches) 
        printShortArray(sensor::waterLevelArrayInInches, 24, true);
      else
        printShortArray(sensor::waterLevelArray, 24, true);
    }
    else if (a_slide == 6) {
      if (user::convertToF)
        printShortArray(sensor::airTemperatureArrayF, 24, true);
      else
        printShortArray(sensor::airTemperatureArray, 24, true);
    }
    else if (a_slide == 7) {
      printShortArray(sensor::humidityArray, 24, true);
    }
    else if (a_slide == 8) {
      printShortArray(sensor::fanOneSpeedArray, 24, false);
    }
    else if (a_slide == 9) {
      printShortArray(sensor::fanTwoSpeedArray, 24, false);
    }
  }
   // Page 2 (Max mins page)
  else if (a_page == 2) {
    printInt(user::convertToF); 
    printInt(user::convertToInches); 
    if (a_slide == 0) {
      printFloat(device::minEc, 2); 
      printFloat(device::maxEc, 2); 
    }
    else if (a_slide == 1) {
      printInt(device::minTds); 
      printInt(device::maxTds); 
    }
    else if (a_slide == 2) {
      printFloat(device::minPh, 2); 
      printFloat(device::maxPh, 2); 
    }
    else if (a_slide == 3) {
      printInt(device::minCo2); 
      printInt(device::maxCo2); 
    }
    else if (a_slide == 4) {
      if (user::convertToF) {
        printFloat(convertToF(device::minWaterTemp), 1); 
        printFloat(convertToF(device::maxWaterTemp), 1); 
      }
      else {
        printFloat(device::minWaterTemp, 1); 
        printFloat(device::maxWaterTemp, 1); 
      }
    }
    else if (a_slide == 5) {
      if (user::convertToInches) {
        printFloat(convertToInches(device::minWaterLevel), 1); 
        printFloat(convertToInches(device::maxWaterLevel), 1); 
      }
      else {
        printFloat(device::minWaterLevel, 1); 
        printFloat(device::maxWaterLevel, 1); 
      }
    }
    else if (a_slide == 6) {
      if (user::convertToF) {
        printFloat(convertToF(device::minAirTemp), 1); 
        printFloat(convertToF(device::maxAirTemp), 1); 
      }
      else {
        printFloat(device::minAirTemp, 1); 
        printFloat(device::maxAirTemp, 1); 
      }
    }
    else if (a_slide == 7) {
      printFloat(device::minHumidity, 1); 
      printFloat(device::maxHumidity, 1); 
    }
    else if (a_slide == 8) {
      printInt(device::minFanOneSpeed); 
      printInt(device::maxFanOneSpeed); 
    }
    else if (a_slide == 9) {
      printFloat(device::minFanTwoSpeed, 1); 
      printFloat(device::maxFanTwoSpeed, 1); 
    }
  }
  // Page 3 (Profiles page)
  else if (a_page == 3) {
    Serial1.print(user::profileOneName); Serial1.print(F(",")); delay(1);
    Serial1.print(user::profileTwoName); Serial1.print(F(",")); delay(1);
    Serial1.print(user::profileThreeName); Serial1.print(F(",")); delay(1);
    Serial1.print(user::profileFourName); Serial1.print(F(",")); delay(1);
    Serial1.print(user::profileFiveName); Serial1.print(F(",")); delay(1);
  }
  // Page 4 (Settings page)
  else if (a_page == 4) {
    printInt(user::numberOfDosers); 
    printInt(user::dosingInterval); 
    printInt(user::convertToF); 
    printInt(user::convertToInches); 
    printInt(user::convertToTds); 
    printInt(user::disablePpmWarnings);
    printInt(user::disableEcWarnings);
    printInt(user::disableCo2Warnings);
    printInt(user::disablePhWarnings);
    printInt(user::disableWaterTempWarnings);
    printInt(user::disableWaterHeightWarnings);
    printInt(user::disableAirTempWarnings);
    printInt(user::disableHumidityWarnings);
  }
  // Page 5 (EC\TDS page)
  else if (a_page == 5) {
    printInt(user::convertToTds); 
    printInt(user::targetMinTds); 
    printInt(user::targetMaxTds); 
    printFloat(user::targetMinEc, 2); 
    printFloat(user::targetMaxEc, 2); 
  }
  // Page 6 (PH page)
  else if (a_page == 6) {
    printFloat(user::targetMinPh, 2); 
    printFloat(user::targetMaxPh, 2); 
  }
  // Page 7 (Co2 page) 
  else if (a_page == 7) { // small amount of data so no need to segment based on slider 
    printInt(user::convertToInches); 
    if (user::convertToInches) {
      printFloat(user::roomLengthInches, 1);
      printFloat(user::roomWidthInches, 1);
      printFloat(user::roomHeightInches, 1);
      printFloat(user::co2FlowrateFeet3, 1);
    }
    else {
      printFloat(user::roomLengthCm, 1);
      printFloat(user::roomWidthCm, 1);
      printFloat(user::roomHeightCm, 1);
      printFloat(user::co2FlowrateLtrs, 1);
    }
    printInt(user::co2CheckTimeMinute);
    printInt(user::co2CheckTimeHour);
    printInt(user::disableFansTimer);
    printInt(user::enableManualCo2Duration);
    printInt(user::disableCo2Control);
    printInt(user::manualCo2GasDuration);
  }
  // Page 8 (Water page)
  else if (a_page == 8) { // small amount of data so no need to segment based on slider
    printInt(user::convertToF); 
    printInt(user::convertToInches); 
    if (user::convertToInches) {
      printFloat(user::targetMinWaterHeightInches, 1);
      printFloat(user::targetMaxWaterHeightInches, 1);
    }
    else {
      printFloat(user::targetMinWaterHeight, 1);
      printFloat(user::targetMaxWaterHeight, 1);
    }
    if (user::convertToF) {
      printFloat(user::targetMinWaterTempF, 1);
      printFloat(user::targetMaxWaterTempF, 1);
    }
    else {
      printFloat(user::targetMinWaterTemp, 1);
      printFloat(user::targetMaxWaterTemp, 1);
    }
    if (user::convertToInches) {
      printFloat(user::waterTankLengthInches, 1);
      printFloat(user::waterTankWidthInches, 1);
    }
    else {
      printFloat(user::waterTankLength, 1);
      printFloat(user::waterTankWidth, 1);
    }
    printInt(user::autoFillMinute);
    printInt(user::autoFillHour);
    printInt(user::disableDrainAndRefill);
    printInt(user::refillDoserOneMills);
    printInt(user::refillDoserTwoMills);
    printInt(user::refillDoserThreeMills);
    printInt(user::refillDoserFourMills);
    printInt(user::refillDoserFiveMills);
    printInt(user::refillDoserSixMills);
    printBitArray(user::autoFillDays, 31);
  }
  // Page 9 (Doser page) 
  else if (a_page == 9) {
    printInt(user::numberOfDosers); 
    printInt(user::doserOneMode);
    printInt(user::doserTwoMode);
    printInt(user::doserThreeMode);
    printInt(user::doserFourMode);
    printInt(user::doserFiveMode);
    printInt(user::doserSixMode);
    printInt(user::doserOneMills);
    printInt(user::doserTwoMills);
    printInt(user::doserThreeMills);
    printInt(user::doserFourMills);
    printInt(user::doserFiveMills);
    printInt(user::doserSixMills);
    //doserOneSpeed = values[cnt++];
    //doserTwoSpeed = values[cnt++];
    //doserThreeSpeed = values[cnt++];
    //doserFourSpeed = values[cnt++];
    //doserFiveSpeed = values[cnt++];
    //doserSixSpeed = values[cnt++];
  }
  // Page 10 (Lighting page) 
  else if (a_page == 10) {
    printInt(user::lightOnTimeHour); // 0
    printInt(user::lightOnTimeMin); // 0
    printInt(user::lightOffTimeHour); // 0
    printInt(user::lightOffTimeMin); // 0
    printInt(user::lightState); // 0
    printInt(user::lightMode); // 0
    printInt(user::lightOffDuration); // 0
    printInt(user::lightOnDuration); // 0
    printInt(user::auxRelayOneOnTimeHour); // 0
    printInt(user::auxRelayOneOnTimeHour); // 0
    printInt(user::auxRelayOneOffTimeHour); // 0
    printInt(user::auxRelayOneOffTimeMin); // 0
    printInt(user::auxRelayOneState); // 0
    printInt(user::auxRelayOneMode); // 0
    printInt(user::auxRelayOneOnDuration); // 0
    printInt(user::auxRelayOneOffDuration); // 0
    printInt(user::auxRelayTwoOnTimeHour); // 0
    printInt(user::auxRelayTwoOnTimeMin); // 0
    printInt(user::auxRelayTwoOffTimeHour); // 0
    printInt(user::auxRelayTwoOffTimeMin); // 0
    printInt(user::auxRelayTwoState); // 0
    printInt(user::auxRelayTwoMode); // 0
    printInt(user::auxRelayTwoOnDuration); // 0
    printInt(user::auxRelayTwoOffDuration); // 0
  }
  // Page 11 (Fans page)
  else if (a_page == 11) { // small amount of data so no need to segment based on slider
    printInt(user::convertToF); 
    printInt(user::targetMinFanOneSpeed);
    printInt(user::targetMaxFanOneSpeed);
    printInt(user::targetMinFanTwoSpeed);
    printInt(user::targetMaxFanTwoSpeed);
    if (user::convertToF) {
      printFloat(user::targetDayMinAirTempF, 1);
      printFloat(user::targetDayMaxAirTempF, 1);
      printFloat(user::targetNightMinAirTempF, 1);
      printFloat(user::targetNightMaxAirTempF, 1);
    }
    else {
      printFloat(user::targetDayMinAirTemp, 1);
      printFloat(user::targetDayMaxAirTemp, 1);
      printFloat(user::targetNightMinAirTemp, 1);
      printFloat(user::targetNightMaxAirTemp, 1);
    }
    printFloat(user::targetDayMinHumidity, 1);
    printFloat(user::targetDayMaxHumidity, 1);
    printFloat(user::targetNightMinHumidity, 1);
    printFloat(user::targetNightMaxHumidity, 1);

    printInt(user::fansControlTemperature);
    printInt(user::fansControlHumidity);
    printInt(user::fanOneFixedSpeed);
    printInt(user::fanTwoFixedSpeed);
  }
  // Page 12 (Warnings page)
  else if (a_page == 12) {  // small amount of data so no need to segment based on slider
    printInt(user::convertToF); 
    printInt(user::convertToInches); 
    printInt(sensor::tds);
    printInt(user::targetMinTds);
    printInt(user::targetMaxTds);
    printInt(user::tdsErrorMargin);
    printFloat(sensor::ec, 2);
    printFloat(user::targetMinEc, 2);
    printFloat(user::targetMaxEc, 2);
    printFloat(user::ecErrorMargin, 2);
    printFloat(sensor::ph, 2);
    printFloat(user::targetMinPh, 2);
    printFloat(user::targetMaxPh, 2);
    printFloat(user::phErrorMargin, 2);
    printInt(sensor::co2);
    printInt(user::targetCo2);
    printInt(user::targetCo2);
    printInt(user::co2ErrorMargin);
    if (user::convertToF) {
      printFloat(convertToF(sensor::waterTemp), 1);
      printFloat(user::targetMinWaterTempF, 1);
      printFloat(user::targetMaxWaterTempF, 1);
      printFloat(user::waterTempErrorMarginF, 1);
    }
    else {
      printFloat(sensor::waterTemp, 1);
      printFloat(user::targetMinWaterTemp, 1);
      printFloat(user::targetMaxWaterTemp, 1);
      printFloat(user::waterTempErrorMargin, 1);
    }
    if (user::convertToInches) {
      printFloat(convertToInches(sensor::waterLevel), 1);
      printFloat(user::targetMinWaterHeightInches, 1);
      printFloat(user::targetMaxWaterHeightInches, 1);
      printFloat(user::waterHeightErrorMarginInches, 1);
    }
    else {
      printFloat(sensor::waterLevel, 1);
      printFloat(user::targetMinWaterHeight, 1);
      printFloat(user::targetMaxWaterHeight, 1);
      printFloat(user::waterHeightErrorMargin, 1);
    }
    if (user::convertToF) {
      printFloat(convertToF(sensor::airTemp), 1);
      device::lightOn ? printFloat(user::targetDayMinAirTempF, 1) : printFloat(user::targetNightMinAirTempF, 1);
      device::lightOn ? printFloat(user::targetDayMaxAirTempF, 1) : printFloat(user::targetNightMaxAirTempF, 1);
      printFloat(user::airTempErrorMarginF, 1);
    }
    else {
      printFloat(sensor::airTemp, 1);
      device::lightOn ? printFloat(user::targetDayMinAirTemp, 1) : printFloat(user::targetNightMinAirTemp, 1);
      device::lightOn ? printFloat(user::targetDayMaxAirTemp, 1) : printFloat(user::targetNightMaxAirTemp, 1);
      printFloat(user::airTempErrorMargin, 1);
    }
    printFloat(sensor::humidity, 1);
    device::lightOn ? printFloat(user::targetDayMinHumidity, 1) : printFloat(user::targetNightMinHumidity, 1);
    device::lightOn ? printFloat(user::targetDayMaxHumidity, 1) : printFloat(user::targetNightMaxHumidity, 1);
    printFloat(user::humidityErrorMargin, 1);
  }
  else {
    Serial.println(F("Page not found!"));
  }
  Serial1.print(F("#")); // End of transmission
}

void setValues(const uint8_t a_cmd, const char* a_val) {
  Serial.print(F("setValues: ")); Serial.print(a_cmd); Serial.print(F(", ")); Serial.println(a_val);
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
    buffToBitArray(a_val, user::autoFillDays, 31);
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
  else if (a_cmd == 66) {
    user::lightOnTimeMin = atoi(a_val);
  }
  // lightOnTimeHour
  else if (a_cmd == 67) {
    user::lightOnTimeHour = atoi(a_val);
  }
  // lightOffTimeMin
  else if (a_cmd == 68) {
    user::lightOffTimeMin = atoi(a_val);
  }
  // lightOffTimeHour
  else if (a_cmd == 69) {
    user::lightOffTimeHour = atoi(a_val);
  }
  // lightOnDuration
  else if (a_cmd == 70) {
    user::lightOnDuration = atoi(a_val);
  }
  // lightOffDuration
  else if (a_cmd == 71) {
    user::lightOffDuration = atoi(a_val);
  }
  // lightState
  else if (a_cmd == 72) {
    user::lightState = atoi(a_val);
  }
  // lightMode
  else if (a_cmd == 73) {
    user::lightMode = atoi(a_val);
  }
  // auxRelayOneOnTimeHour
  else if (a_cmd == 74) {
    user::auxRelayOneOnTimeHour = atoi(a_val);
  }
  // auxRelayOneOnTimeMin
  else if (a_cmd == 75) {
    user::auxRelayOneOnTimeMin = atoi(a_val);
  }
  // auxRelayOneOffTimeHour
  else if (a_cmd == 76) {
    user::auxRelayOneOffTimeHour = atoi(a_val);
  }
  // auxRelayOneOffTimeMin
  else if (a_cmd == 77) {
    user::auxRelayOneOffTimeMin = atoi(a_val);
  }
  // auxRelayOneOnDuration
  else if (a_cmd == 78) {
    user::auxRelayOneOnDuration = atoi(a_val);
  }
  // auxRelayOneOffDuration
  else if (a_cmd == 79) {
    user::auxRelayOneOffDuration = atoi(a_val);
  }
  // auxRelayOneState
  else if (a_cmd == 80) {
    user::auxRelayOneState = atoi(a_val);
  }
  // auxRelayOneMode
  else if (a_cmd == 81) {
    user::auxRelayOneMode = atoi(a_val);
  }
  // auxRelayTwoOnTimeHour
  else if (a_cmd == 82) {
    user::auxRelayTwoOnTimeHour = atoi(a_val);
  }
  // auxRelayTwoOnTimeMin
  else if (a_cmd == 83) {
    user::auxRelayTwoOnTimeMin = atoi(a_val);
  }
  // auxRelayTwoOffTimeHour
  else if (a_cmd == 84) {
    user::auxRelayTwoOffTimeHour = atoi(a_val);
  }
  // auxRelayTwoOffTimeMin
  else if (a_cmd == 85) {
    user::auxRelayTwoOffTimeMin = atoi(a_val);
  }
  // auxRelayTwoOnDuration
  else if (a_cmd == 86) {
    user::auxRelayTwoOnDuration = atoi(a_val);
  }
  // auxRelayTwoOffDuration
  else if (a_cmd == 87) {
    user::auxRelayTwoOffDuration = atoi(a_val);
  }
  // auxRelayTwoState
  else if (a_cmd == 88) {
    user::auxRelayTwoState = atoi(a_val);
  }
  // auxRelayTwoMode
  else if (a_cmd == 89) {
    user::auxRelayTwoMode = atoi(a_val);
  }
  // ============== FANS PAGE ==============
  // targetMinFanOneSpeed
  else if (a_cmd == 90) {
    user::targetMinFanOneSpeed = atoi(a_val);
  }
  // targetMaxFanOneSpeed
  else if (a_cmd == 91) {
    user::targetMaxFanOneSpeed = atoi(a_val);
  }
  // targetMinFanTwoSpeed
  else if (a_cmd == 92) {
    user::targetMinFanTwoSpeed = atoi(a_val);
  }
  // targetMaxFanTwoSpeed
  else if (a_cmd == 93) {
    user::targetMaxFanTwoSpeed = atoi(a_val);
  }
  // targetDayMinAirTemp
  else if (a_cmd == 94) {
    (user::convertToF ? user::targetDayMinAirTempF : user::targetDayMinAirTemp) = atof(a_val);
  }
  // targetDayMaxAirTemp
  else if (a_cmd == 95) {
    (user::convertToF ? user::targetDayMaxAirTempF : user::targetDayMaxAirTemp) = atof(a_val);
  }
  // targetDayMinHumidity
  else if (a_cmd == 96) {
    user::targetDayMinHumidity = atof(a_val);
  }
  // targetDayMaxHumidity
  else if (a_cmd == 97) {
    user::targetDayMaxHumidity = atof(a_val);
  }
  // targetNightMinAirTemp
  else if (a_cmd == 98) {
    (user::convertToF ? user::targetNightMinAirTempF : user::targetNightMinAirTemp) = atof(a_val);
  }
  // targetNightMaxAirTemp
  else if (a_cmd == 99) {
    (user::convertToF ? user::targetNightMaxAirTempF : user::targetNightMaxAirTemp) = atof(a_val);
  }
  // targetNightMinHumidity
  else if (a_cmd == 100) {
    user::targetNightMinHumidity = atof(a_val);
  }
  // targetNightMaxHumidity
  else if (a_cmd == 101) {
    user::targetNightMaxHumidity = atof(a_val);
  }
  // fansControlTemp
  else if (a_cmd == 102) {
    user::fansControlTemperature = strBool(a_val);
  }
  // fansControlHum
  else if (a_cmd == 103) {
    user::fansControlHumidity = strBool(a_val);
  }
  // fanOneFixedSpeed
  else if (a_cmd == 104) {
    user::fanOneFixedSpeed = strBool(a_val);
  }
  // fanTwoFixedSpeed
  else if (a_cmd == 105) {
    user::fanTwoFixedSpeed = strBool(a_val);
  }
  // ============== WARNINGS PAGE ==============
  // ecErrorMargin
  else if (a_cmd == 106) {
    user::ecErrorMargin = atof(a_val);
  }
  // tdsErrorMargin
  else if (a_cmd == 107) {
    user::tdsErrorMargin = atoi(a_val);
  }
  // phErrorMargin
  else if (a_cmd == 108) {
    user::phErrorMargin = atof(a_val);
  }
  // co2ErrorMargin
  else if (a_cmd == 109) {
    user::co2ErrorMargin = atoi(a_val);
  }
  // waterHeightErrorMargin
  else if (a_cmd == 110) {
    (user::convertToInches ? user::waterHeightErrorMarginInches :user::waterHeightErrorMargin) = atof(a_val);
  }
  // waterTempErrorMargin
  else if (a_cmd == 111) {
    (user::convertToF ? user::waterTempErrorMarginF :user::waterTempErrorMargin) = atof(a_val);
  }
  // airTempErrorMargin
  else if (a_cmd == 112) {
    (user::convertToF ? user::airTempErrorMarginF :user::airTempErrorMargin) = atof(a_val);
  }
  // humidityErrorMargin
  else if (a_cmd == 113) {
    user::humidityErrorMargin = atof(a_val);
  }
  else
    Serial.println(F("cmd not recognised!"));
  //server.send(200, "text/plane", "OK");
}

void printFloat(const float a_value, const int16_t a_precison) {
 Serial1.print(a_value, a_precison); 
 Serial1.print(F(","));
 delay(1);
}

void printInt(const uint16_t a_value) {
 Serial1.print(a_value); 
 Serial1.print(F(","));
 delay(5);
}

void printShortArray(const uint16_t* a_array, const int16_t a_len, const bool a_isCompressed) {
  for (int i = 0; i < a_len; i++) {
    Serial1.print(uncompressShort(a_array[i], a_isCompressed)); 
    Serial1.print(F(","));
    delay(1);
  }
}

void printBitArray(const uint32_t a_value, const int8_t a_len) {
  for (int i = a_len - 1; i >= 0; i--) {
    Serial1.print(bitRead(a_value, i)); 
    Serial1.print(F(","));
    delay(1);
  }
}

bool strBool(const char* buffer) {
  if (strstr(buffer, "true") != NULL)
    return true;
  else
    return false;
}

void buffToCharArray(const char* a_src, char* a_dest, const int8_t a_size) {
  for (uint8_t i = 0; i < a_size; i++) {
    a_dest[i] = a_src[i];
  }
}

void buffToBitArray(const char* a_src, uint32_t& a_value, const int8_t a_len) {
  uint8_t pos = 0;
   for (int i = a_len; i >= 0; i--) {
    bool bitVal = a_src[pos++] == '1' ? true : false;
    bitWrite(a_value, i, bitVal);
  }
}