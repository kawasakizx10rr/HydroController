void handleRoot() {
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.sendHeader("Content-Length", (String)sizeof(htmlText));
  server.send(200, "text/html", "");
  // send webpage in sizeof(buffer) byte packets
  unsigned int byteCount = 0;
  memset(wifi::buffer, 0, sizeof(wifi::buffer));
  if (device::globalDebug) {
    Serial.print(F("Sending "));
    Serial.print(sizeof(htmlText));
    Serial.println(F(" bytes..."));
  }
  for (unsigned int i = 0; i < sizeof(htmlText);) {
    if (byteCount < sizeof(wifi::buffer)) {
      wifi::buffer[byteCount] = pgm_read_byte(htmlText + i++);
      byteCount++;
    } 
    else {
      server.sendContent_P(wifi::buffer, byteCount);
      //if (device::globalDebug)
      //Serial.print(wifi::buffer);
      byteCount = 0;
      memset(wifi::buffer, 0, sizeof(wifi::buffer));
      yield();
    }
  }
  if (byteCount > 0) {
    //if (device::globalDebug)
    //Serial.print(wifi::buffer);
    server.sendContent_P(wifi::buffer, byteCount);
  }
  yield();
  if (device::globalDebug)
    Serial.println(F("Packets sent!"));
}

void setValue() {
  yield();
  uint8_t cmdIndex = 0;
  String cmd = server.arg("cmd");
  char val[64]{ 0 };
  server.arg("val").toCharArray(val, 64);
  if (cmd == "" || val[0] == 0) {  //Parameters not found
    if (device::globalDebug)
      Serial.println(F("Web argument not found!"));
  } 
  else {
    if (device::globalDebug) {
      Serial.print(F("Received cmd: "));
      Serial.print(cmd);
      Serial.print(F(" and val: "));
      Serial.println(val);
    }
    // ============== PROFILES PAGE ==============
    // profileOneName
    if (cmd == F("profileOneName")) {
      cmdIndex = 0;
      buffToCharArray(val, user::profileOneName, 11);
    }
    // profileTwoName
    else if (cmd == F("profileTwoName")) {
      cmdIndex = 1;
      buffToCharArray(val, user::profileTwoName, 11);
    }
    // profileThreeName
    else if (cmd == F("profileThreeName")) {
      cmdIndex = 2;
      buffToCharArray(val, user::profileThreeName, 11);
    }
    // profileFourName
    else if (cmd == F("profileFourName")) {
      cmdIndex = 3;
      buffToCharArray(val, user::profileFourName, 11);
    }
    // profileFiveName
    else if (cmd == F("profileFiveName")) {
      cmdIndex = 4;
      buffToCharArray(val, user::profileFiveName, 11);
    }
    // loadData
    else if (cmd == F("loadData")) {
      cmdIndex = 5;
      device::remotlyLoadUserProfile = atoi(val);
    }
    // saveData
    else if (cmd == F("saveData")) {
      cmdIndex = 6;
      device::remotlySaveUserProfile = atoi(val);
    }
    // ============== SETTINGS PAGE  ==============
    // numberOfDosers
    else if (cmd == F("numDosers")) {
      cmdIndex = 7;
      user::numberOfDosers = atoi(val);
    }
    // dosingInterval
    else if (cmd == F("dosingInt")) {
      cmdIndex = 8;
      user::dosingInterval = atoi(val);
    }
    // convertToFarenheit
    else if (cmd == F("convertToF")) {
      cmdIndex = 9;
      user::convertToF = strBool(val);
    }
    // convertToInchs
    else if (cmd == F("convertToInchs")) {
      cmdIndex = 10;
      user::convertToInches = strBool(val);
    }
    // convertToTds
    else if (cmd == F("convertToTds")) {
			cmdIndex = 11;
      user::convertToTds = strBool(val);
    }
    // disableCo2Warnings
    else if (cmd == F("disableCo2Warn")) {
			cmdIndex = 12;
      user::disableCo2Warnings = strBool(val);
    }
    // disableEcWarnings
    else if (cmd == F("disableEcWarn")) {
			cmdIndex = 13;
      user::disableEcWarnings = strBool(val);
    }
    // disablePpmWarnings
    else if (cmd == F("disablePpmWarn")) {
			cmdIndex = 14;
      user::disablePpmWarnings = strBool(val);
    }
    // disablePhWarnings
    else if (cmd == F("disablePhWarn")) {
			cmdIndex = 15;
      user::disablePhWarnings = strBool(val);
    }
    // disableWaterTempWarnings
    else if (cmd == F("disableWaterTempWarn")) {
			cmdIndex = 16;
      user::disableWaterTempWarnings = strBool(val);
    }
    // disableWaterHeightWarnings
    else if (cmd == F("disableWaterHeightWarn")) {
			cmdIndex = 17;
      user::disableWaterHeightWarnings = strBool(val);
    }
    // disableAirTempWarnings
    else if (cmd == F("disableAirTempWarn")) {
			cmdIndex = 18;
      user::disableAirTempWarnings = strBool(val);
    }
    // disableHumidityWarnings
    else if (cmd == F("disableHumidityWarn")) {
			cmdIndex = 19;
      user::disableHumidityWarnings = strBool(val);
    }
    // ============== EC PAGE ==============
    // targetMinEc
    else if (cmd == F("targetMinEc")) {
			cmdIndex = 20;
      user::targetMinEc = atof(val);
    }
    // targetMaxEc
    else if (cmd == F("targetMaxEc")) {
			cmdIndex = 21;
      user::targetMaxEc = atof(val);
    }
    // ============== TDS/PPM PAGE ==============
    // targetMinTds
    else if (cmd == F("targetMinTds")) {
			cmdIndex = 022;
      user::targetMinTds = atof(val);
    }
    // targetMaxTds
    else if (cmd == F("targetMaxTds")) {
			cmdIndex = 23;
      user::targetMaxTds = atof(val);
    }
    // ============== PH PAGE ==============
    // targetMinPh
    else if (cmd == F("targetMinPh")) {
			cmdIndex = 24;
      user::targetMinPh = atof(val);
    }
    // targetMaxPh
    else if (cmd == F("targetMaxPh")) {
			cmdIndex = 25;
      user::targetMaxPh = atof(val);
    }
    // ============== Co2 PAGE ==============
    // targetCo2
    else if (cmd == F("targetCo2")) {
			cmdIndex = 26;
      user::targetCo2 = atoi(val);
    }
    // co2Offset
    else if (cmd == F("co2Offset")) {
			cmdIndex = 27;
      user::co2Offset = atoi(val);
    }
    // roomLength
    else if (cmd == F("roomLength")) {
			cmdIndex = 28;
      user::roomLength = atoi(val);
    }
    // roomWidth
    else if (cmd == F("roomWidth")) {
			cmdIndex = 29;
      user::roomWidth = atoi(val);
    }
    // roomHeight
    else if (cmd == F("roomHeight")) {
			cmdIndex = 30;
      user::roomHeight = atoi(val);
    }
    // co2Flowrate
    else if (cmd == F("co2Flowrate")) {
			cmdIndex = 31;
      user::co2Flowrate = atof(val);
    }
    // co2CheckTimeMinute
    else if (cmd == F("co2CheckTimeMin")) {
			cmdIndex = 32;
      user::co2CheckTimeMinute = atoi(val);
    }
    // co2CheckTimeHour
    else if (cmd == F("co2CheckTimeHour")) {
			cmdIndex = 33;
      user::co2CheckTimeHour = atoi(val);
    }
    // disableFansTimer
    else if (cmd == F("disableFansTimer")) {
			cmdIndex = 34;
      user::disableFansTimer = atoi(val);
    }
    // manualCo2Duration
    else if (cmd == F("gasTimeEnabled")) {
			cmdIndex = 35;
      user::enableManualCo2Duration = strBool(val);
    }
    // manualCo2GasDuration
    else if (cmd == F("gasTime")) {
			cmdIndex = 36;
      user::manualCo2GasDuration = atoi(val);
    }
    // disableCo2Control
    else if (cmd == F("disableCo2Control")) {
			cmdIndex = 37;
      user::disableCo2Control = strBool(val);
    }
    // ============== WATER PAGE ==============
    // targetMinWaterHeight
    else if (cmd == F("targetMinWaterLvl")) {
			cmdIndex = 38;
      user::targetMinWaterHeight = atof(val);
    }
    // targetMaxWaterHeight
    else if (cmd == F("targetMaxWaterLvl")) {
			cmdIndex = 39;
      user::targetMaxWaterHeight = atof(val);
    }
    // targetMinWaterTemp
    else if (cmd == F("targetMinWaterTemp")) {
			cmdIndex = 40;
      user::targetMinWaterTemp = atof(val);
    }
    // targetMaxWaterTemp
    else if (cmd == F("targetMaxWaterTemp")) {
			cmdIndex = 41;
      user::targetMaxWaterTemp = atof(val);
    }
    // targetWaterTankLength
    else if (cmd == F("targetWaterTankLength")) {
			cmdIndex = 42;
      user::waterTankLength = atof(val);
    }
    // targetWaterTankWidth
    else if (cmd == F("targetWaterTankWidth")) {
			cmdIndex = 43;
      user::waterTankWidth = atof(val);
    }
    // autoFillDays
    else if (cmd == F("autoFillDays")) {
			cmdIndex = 44;
      buffToBoolArray(val, user::autoFillDays, 31);
    }
    // autoFillMinute
    else if (cmd == F("autoFillMinute")) {
			cmdIndex = 45;
      user::autoFillMinute = atoi(val);
    }
    // autoFillHour
    else if (cmd == F("autoFillHour")) {
			cmdIndex = 46;
      user::autoFillHour = atoi(val);
    }
    // disableDrainAndRefill
    else if (cmd == F("disableRefillDrain")) {
			cmdIndex = 47;
      user::disableDrainAndRefill = strBool(val);
    }
    // refillDoserOneMills
    else if (cmd == F("d1mls")) {
			cmdIndex = 48;
      user::refillDoserOneMills = atoi(val);
    }
    // refillDoserTwoMills
    else if (cmd == F("d2mls")) {
			cmdIndex = 49;
      user::refillDoserTwoMills = atoi(val);
    }
    // refillDoserThreeMills
    else if (cmd == F("d3mls")) {
			cmdIndex = 50;
      user::refillDoserThreeMills = atoi(val);
    }
    // refillDoserFourMills
    else if (cmd == F("d4mls")) {
			cmdIndex = 51;
      user::refillDoserFourMills = atoi(val);
    }
    // refillDoserFiveMills
    else if (cmd == F("d5mls")) {
			cmdIndex = 52;
      user::refillDoserFiveMills = atoi(val);
    }
    // refillDoserSixMills
    else if (cmd == F("d6mls")) {
			cmdIndex = 53;
      user::refillDoserSixMills = atoi(val);
    }
    // ============== DOSER PAGE ==============
    // doserOneMills
    else if (cmd == F("doserMls_1")) {
			cmdIndex = 54;
      user::doserOneMills = atoi(val);
    }
    // doserTwoMills
    else if (cmd == F("doserMls_2")) {
			cmdIndex = 55;
      user::doserTwoMills = atoi(val);
    }
    // doserThreeMills
    else if (cmd == F("doserMls_3")) {
			cmdIndex = 56;
      user::doserThreeMills = atoi(val);
    }
    // doserFourMills
    else if (cmd == F("doserMls_4")) {
			cmdIndex = 57;
      user::doserFourMills = atoi(val);
    }
    // doserFiveMills
    else if (cmd == F("doserMls_5")) {
			cmdIndex = 58;
      user::doserFiveMills = atoi(val);
    }
    // doserSixMills
    else if (cmd == F("doserMls_6")) {
			cmdIndex = 59;
      user::doserSixMills = atoi(val);
    }
    // doserOneMode
    else if (cmd == F("doserOneMode")) {
			cmdIndex = 60;
      user::doserOneMode = atoi(val);
    }
    // doserTwoMode
    else if (cmd == F("doserTwoMode")) {
			cmdIndex = 61;
      user::doserTwoMode = atoi(val);
    }
    // doserThreeMode
    else if (cmd == F("doserThreeMode")) {
			cmdIndex = 62;
      user::doserThreeMode = atoi(val);
    }
    // doserFourMode
    else if (cmd == F("doserFourMode")) {
			cmdIndex = 63;
      user::doserFourMode = atoi(val);
    }
    // doserFiveMode
    else if (cmd == F("doserFiveMode")) {
			cmdIndex = 64;
      user::doserFiveMode = atoi(val);
    }
    // doserSixMode
    else if (cmd == F("doserSixMode")) {
			cmdIndex = 65;
      user::doserSixMode = atoi(val);
    }
    // ============== LIGHT PAGE ==============
    // lightOnTimeMin
    else if (cmd == F("onTimeMin")) {
			cmdIndex = 66;
      user::lightOnTimeMin = atoi(val);
    }
    // lightOnTimeHour
    else if (cmd == F("onTimeHour")) {
			cmdIndex = 67;
      user::lightOnTimeHour = atoi(val);
    }
    // lightOffTimeMin
    else if (cmd == F("offTimeMin")) {
			cmdIndex = 68;
      user::lightOffTimeMin = atoi(val);
    }
    // lightOffTimeHour
    else if (cmd == F("offTimeHour")) {
			cmdIndex = 69;
      user::lightOffTimeHour = atoi(val);
    }
    // lightMode
    else if (cmd == F("lightMode")) {
			cmdIndex = 70;
      user::lightMode = atoi(val);
    }
    // ============== FANS PAGE ==============
    // targetMinFanOneSpeed
    else if (cmd == F("fanOneMinSpeed")) {
			cmdIndex = 71;
      user::targetMinFanOneSpeed = atoi(val);
    }
    // targetMaxFanOneSpeed
    else if (cmd == F("fanOneMaxSpeed")) {
			cmdIndex = 72;
      user::targetMaxFanOneSpeed = atoi(val);
    }
    // targetMinFanTwoSpeed
    else if (cmd == F("fanTwoMinSpeed")) {
			cmdIndex = 73;
      user::targetMinFanTwoSpeed = atoi(val);
    }
    // targetMaxFanTwoSpeed
    else if (cmd == F("fanTwoMaxSpeed")) {
			cmdIndex = 74;
      user::targetMaxFanTwoSpeed = atoi(val);
    }
    // targetMinAirTemp
    else if (cmd == F("targetMinAirTemp")) {
			cmdIndex = 75;
      user::targetMinAirTemp = atof(val);
    }
    // targetMaxAirTemp
    else if (cmd == F("targetMaxAirTemp")) {
			cmdIndex = 76;
      user::targetMaxAirTemp = atof(val);
    }
    // targetMinHumidity
    else if (cmd == F("targetMinHumidity")) {
			cmdIndex = 77;
      user::targetMinHumidity = atof(val);
    }
    // targetMaxHumidity
    else if (cmd == F("targetMaxHumidity")) {
			cmdIndex = 78;
      user::targetMaxHumidity = atof(val);
    }
    // fansControlTemp
    else if (cmd == F("fansControlTemp")) {
			cmdIndex = 79;
      user::fansControlTemperature = strBool(val);
    }
    // fansControlHum
    else if (cmd == F("fansControlHum")) {
			cmdIndex = 80;
      user::fansControlHumidity = strBool(val);
    }
    // fanOneFixedSpeed
    else if (cmd == F("fanOneFixedSpeed")) {
			cmdIndex = 81;
      user::fanOneFixedSpeed = strBool(val);
    }
    // fanTwoFixedSpeed
    else if (cmd == F("fanTwoFixedSpeed")) {
			cmdIndex = 82;
      user::fanTwoFixedSpeed = strBool(val);
    }
    // ============== WARNINGS PAGE ==============
    // ecErrorMargin
    else if (cmd == F("ecErrorMargin")) {
			cmdIndex = 83;
      user::ecErrorMargin = atof(val);
    }
    // tdsErrorMargin
    else if (cmd == F("tdsErrorMargin")) {
			cmdIndex = 84;
      user::tdsErrorMargin = atoi(val);
    }
    // phErrorMargin
    else if (cmd == F("phErrorMargin")) {
			cmdIndex = 85;
      user::phErrorMargin = atof(val);
    }
    // co2ErrorMargin
    else if (cmd == F("co2ErrorMargin")) {
			cmdIndex = 86;
      user::co2ErrorMargin = atoi(val);
    }
    // waterHeightErrorMargin
    else if (cmd == F("waterHeightErrorMargin")) {
			cmdIndex = 87;
      user::waterHeightErrorMargin = atof(val);
    }
    // waterTempErrorMargin
    else if (cmd == F("waterTempErrorMargin")) {
			cmdIndex = 88;
      user::waterTempErrorMargin = atof(val);
    }
    // airTempErrorMargin
    else if (cmd == F("airTempErrorMargin")) {
			cmdIndex = 89;
      user::airTempErrorMargin = atof(val);
    }
    // humidityErrorMargin
    else if (cmd == F("humidityErrorMargin")) {
			cmdIndex = 90;
      user::humidityErrorMargin = atof(val);
    } 
    else if (device::globalDebug)
      Serial.println(F("server arg not recognised!"));
    yield();
    // Send updated value back to the Mega2560
    Serial.print(F("!0,"));
    Serial.print(cmdIndex);
    Serial.print(F(","));
    Serial.print(val);
  }
  server.send(200, "text/plane", "OK");
}

void getPageData() {
  yield();
  uint8_t page = atoi(server.arg("page").c_str());
  uint8_t slide = atoi(server.arg("slide").c_str());
  // Request data from Mega2560
  Serial.print("!1,");
  Serial.print(page);
  Serial.print(",");
  Serial.print(slide);
  receiveSerialData(page, slide);
  // Copy data to a buffer and send to the server
  if (device::globalDebug) {
    Serial.print(F("Received web request for page "));
    Serial.print(page);
    Serial.print(F(" and slide "));
    Serial.println(slide);
  }
  memset(wifi::buffer, 0, sizeof(wifi::buffer));
  // Page 0 (Home page)
  if (page == 0) {
    copyIntToArray(user::convertToF);
    copyIntToArray(user::convertToInches);
    if (slide == 0) {  // home page slide 0
      copyFloatToArray(sensor::ec, 2);
      copyFloatToArray(user::targetMinEc, 2);
      copyFloatToArray(user::targetMaxEc, 2);
      copyFloatArrayToArray(sensor::ecArray, sensor::maxSensorArrayVals, 2);
    } 
    else if (slide == 1) {  // home page slide 1
      copyIntToArray(sensor::tds);
      copyIntToArray(user::targetMinTds);
      copyIntToArray(user::targetMaxTds);
      copyFloatArrayToArray(sensor::tdsArray, sensor::maxSensorArrayVals, 0);
    } 
    else if (slide == 2) {  // home page slide 2
      copyFloatToArray(sensor::ph, 2);
      copyFloatToArray(user::targetMinPh, 2);
      copyFloatToArray(user::targetMaxPh, 2);
      copyFloatArrayToArray(sensor::phArray, sensor::maxSensorArrayVals, 2);
    } 
    else if (slide == 3) {  // home page slide 3
      copyIntToArray(sensor::co2);
      copyIntToArray(user::targetCo2);
      copyIntToArray(user::targetCo2);
      copyFloatArrayToArray(sensor::co2Array, sensor::maxSensorArrayVals, 0);
    } 
    else if (slide == 4) {  // home page slide 4
      copyFloatToArray(sensor::waterTemp, 1);
      copyFloatToArray(user::targetMinWaterTemp, 1);
      copyFloatToArray(user::targetMaxWaterTemp, 1);
      copyFloatArrayToArray(sensor::waterTemperatureArray, sensor::maxSensorArrayVals, 1);
    } 
    else if (slide == 5) {  // home page slide 5
      copyFloatToArray(sensor::waterLevel, 1);
      copyFloatToArray(user::targetMinWaterHeight, 1);
      copyFloatToArray(user::targetMaxWaterHeight, 1);
      copyFloatArrayToArray(sensor::waterLevelArray, sensor::maxSensorArrayVals, 1);
    } 
    else if (slide == 6) {  // home page slide 6
      copyFloatToArray(sensor::airTemp, 1);
      copyFloatToArray(user::targetMinAirTemp, 1);
      copyFloatToArray(user::targetMaxAirTemp, 1);
      copyFloatArrayToArray(sensor::airTemperatureArray, sensor::maxSensorArrayVals, 1);
    } 
    else if (slide == 7) {  // home page slide 7
      copyFloatToArray(sensor::humidity, 1);
      copyFloatToArray(user::targetMinHumidity, 1);
      copyFloatToArray(user::targetMaxHumidity, 1);
      copyFloatArrayToArray(sensor::humidityArray, sensor::maxSensorArrayVals, 1);
    } 
    else if (slide == 8) {  // home page slide 8
      copyIntToArray(device::fanOneSpeed);
      copyIntToArray(user::targetMinFanOneSpeed);
      copyIntToArray(user::targetMaxFanOneSpeed);
      copyFloatArrayToArray(sensor::fanOneSpeedArray, sensor::maxSensorArrayVals, 0);
    } 
    else if (slide == 9) {  // home page slide 9
      copyIntToArray(device::fanTwoSpeed);
      copyIntToArray(user::targetMinFanTwoSpeed);
      copyIntToArray(user::targetMaxFanTwoSpeed);
      copyFloatArrayToArray(sensor::fanTwoSpeedArray, sensor::maxSensorArrayVals, 0);
    }
  }
  // Page 1 (Graphs page)
  else if (page == 1) {
    copyIntToArray(user::convertToF);
    copyIntToArray(user::convertToInches);
    if (slide == 0) {
      copyFloatArrayToArray(sensor::ecArray, sensor::maxSensorArrayVals, 2);
    } 
    else if (slide == 1) {
      copyFloatArrayToArray(sensor::tdsArray, sensor::maxSensorArrayVals, 0);
    } 
    else if (slide == 2) {
      copyFloatArrayToArray(sensor::phArray, sensor::maxSensorArrayVals, 2);
    } 
    else if (slide == 3) {
      copyFloatArrayToArray(sensor::co2Array, sensor::maxSensorArrayVals, 0);
    } 
    else if (slide == 4) {
      copyFloatArrayToArray(sensor::waterTemperatureArray, sensor::maxSensorArrayVals, 1);
    } 
    else if (slide == 5) {
      copyFloatArrayToArray(sensor::waterLevelArray, sensor::maxSensorArrayVals, 1);
    } 
    else if (slide == 6) {
      copyFloatArrayToArray(sensor::airTemperatureArray, sensor::maxSensorArrayVals, 1);
    } 
    else if (slide == 7) {
      copyFloatArrayToArray(sensor::humidityArray, sensor::maxSensorArrayVals, 1);
    } 
    else if (slide == 8) {
      copyFloatArrayToArray(sensor::fanOneSpeedArray, sensor::maxSensorArrayVals, 0);
    } 
    else if (slide == 9) {
      copyFloatArrayToArray(sensor::fanTwoSpeedArray, sensor::maxSensorArrayVals, 0);
    }
  }
  // Page 2 (Max mins page)
  else if (page == 2) {
    copyIntToArray(user::convertToF);
    copyIntToArray(user::convertToInches);
    if (slide == 0) {
      copyFloatToArray(device::minEc, 2);
      copyFloatToArray(device::maxEc, 2);
    } 
    else if (slide == 1) {
      copyIntToArray(device::minTds);
      copyIntToArray(device::maxTds);
    } 
    else if (slide == 2) {
      copyFloatToArray(device::minPh, 2);
      copyFloatToArray(device::maxPh, 2);
    } 
    else if (slide == 3) {
      copyIntToArray(device::minCo2);
      copyIntToArray(device::maxCo2);
    } 
    else if (slide == 4) {
      copyFloatToArray(device::minWaterTemp, 1);
      copyFloatToArray(device::maxWaterTemp, 1);
    } 
    else if (slide == 5) {
      copyFloatToArray(device::minWaterLevel, 1);
      copyFloatToArray(device::maxWaterLevel, 1);
    } 
    else if (slide == 6) {
      copyFloatToArray(device::minAirTemp, 1);
      copyFloatToArray(device::maxAirTemp, 1);
    } 
    else if (slide == 7) {
      copyFloatToArray(device::minHumidity, 1);
      copyFloatToArray(device::maxHumidity, 1);
    } 
    else if (slide == 8) {
      copyIntToArray(device::minFanOneSpeed);
      copyIntToArray(device::maxFanOneSpeed);
    } 
    else if (slide == 9) {
      copyFloatToArray(device::minFanTwoSpeed, 1);
      copyFloatToArray(device::maxFanTwoSpeed, 1);
    }
  }
  // Page 3 (Profiles page)
  else if (page == 3) {
    strcpy(wifi::buffer, user::profileOneName); strcat(wifi::buffer, ",");
    strcat(wifi::buffer, user::profileTwoName); strcat(wifi::buffer, ",");
    strcat(wifi::buffer, user::profileThreeName); strcat(wifi::buffer, ",");
    strcat(wifi::buffer, user::profileFourName); strcat(wifi::buffer, ",");
    strcat(wifi::buffer, user::profileOneName); strcat(wifi::buffer, ",");
  }
  // Page 4 (Settings page)
  else if (page == 4) {
    copyIntToArray(user::numberOfDosers);
    copyIntToArray(user::dosingInterval);
    copyIntToArray(user::convertToF);
    copyIntToArray(user::convertToInches);
    copyIntToArray(user::convertToTds);
    copyIntToArray(user::disablePpmWarnings);
    copyIntToArray(user::disableEcWarnings);
    copyIntToArray(user::disableCo2Warnings);
    copyIntToArray(user::disablePhWarnings);
    copyIntToArray(user::disableWaterTempWarnings);
    copyIntToArray(user::disableWaterHeightWarnings);
    copyIntToArray(user::disableAirTempWarnings);
    copyIntToArray(user::disableHumidityWarnings);
  }
  // Page 5 (EC\TDS page)
  else if (page == 5) {
    copyIntToArray(user::convertToTds);
    copyIntToArray(user::targetMinTds);
    copyIntToArray(user::targetMaxTds);
    copyFloatToArray(user::targetMinEc, 2);
    copyFloatToArray(user::targetMaxEc, 2);
  }
  // Page 6 (PH page)
  else if (page == 6) {
    copyFloatToArray(user::targetMinPh, 2);
    copyFloatToArray(user::targetMaxPh, 2);
  }
  // Page 7 (Co2 page)
  else if (page == 7) {  // small amount of data so no need to segment based on slider
    copyIntToArray(user::convertToInches);
    copyFloatToArray(user::roomLength, 1);
    copyFloatToArray(user::roomWidth, 1);
    copyFloatToArray(user::roomHeight, 1);
    copyFloatToArray(user::co2Flowrate, 1);
    copyIntToArray(user::co2CheckTimeMinute);
    copyIntToArray(user::co2CheckTimeHour);
    copyIntToArray(user::disableFansTimer);
    copyIntToArray(user::enableManualCo2Duration);
    copyIntToArray(user::disableCo2Control);
    copyIntToArray(user::manualCo2GasDuration);
  }
  // Page 8 (Water page)
  else if (page == 8) {  // small amount of data so no need to segment based on slider
    copyIntToArray(user::convertToF);
    copyIntToArray(user::convertToInches);
    copyFloatToArray(user::targetMinWaterHeight, 1);
    copyFloatToArray(user::targetMaxWaterHeight, 1);
    copyFloatToArray(user::targetMinWaterTemp, 1);
    copyFloatToArray(user::targetMaxWaterTemp, 1);
    copyFloatToArray(user::waterTankLength, 1);
    copyFloatToArray(user::waterTankWidth, 1);    
    copyBoolArrayToArray(user::autoFillDays, 31);
    copyIntToArray(user::autoFillMinute);
    copyIntToArray(user::autoFillHour);
    copyIntToArray(user::disableDrainAndRefill);
    copyIntToArray(user::refillDoserOneMills);
    copyIntToArray(user::refillDoserTwoMills);
    copyIntToArray(user::refillDoserThreeMills);
    copyIntToArray(user::refillDoserFourMills);
    copyIntToArray(user::refillDoserFiveMills);
    copyIntToArray(user::refillDoserSixMills);
  }
  // Page 9 (Doser page)
  else if (page == 9) {
    copyIntToArray(user::numberOfDosers);
    copyIntToArray(user::doserOneMode);
    copyIntToArray(user::doserTwoMode);
    copyIntToArray(user::doserThreeMode);
    copyIntToArray(user::doserFourMode);
    copyIntToArray(user::doserFiveMode);
    copyIntToArray(user::doserSixMode);
    copyIntToArray(user::doserOneMills);
    copyIntToArray(user::doserTwoMills);
    copyIntToArray(user::doserThreeMills);
    copyIntToArray(user::doserFourMills);
    copyIntToArray(user::doserFiveMills);
    copyIntToArray(user::doserSixMills);
    //doserOneSpeed = values[cnt++];
    //doserTwoSpeed = values[cnt++];
    //doserThreeSpeed = values[cnt++];
    //doserFourSpeed = values[cnt++];
    //doserFiveSpeed = values[cnt++];
    //doserSixSpeed = values[cnt++];
  }
  // Page 10 (Lighting page)
  else if (page == 10) {
    copyIntToArray(user::lightOnTimeMin);
    copyIntToArray(user::lightOnTimeHour);
    copyIntToArray(user::lightOffTimeMin);
    copyIntToArray(user::lightOffTimeHour);
    copyIntToArray(user::lightMode);
  }
  // Page 11 (Fans page)
  else if (page == 11) {  // small amount of data so no need to segment based on slider
    copyIntToArray(user::convertToF);
    copyIntToArray(user::targetMinFanOneSpeed);
    copyIntToArray(user::targetMaxFanOneSpeed);
    copyIntToArray(user::targetMinFanTwoSpeed);
    copyIntToArray(user::targetMaxFanTwoSpeed);
    copyFloatToArray(user::targetMinAirTemp, 1);
    copyFloatToArray(user::targetMaxAirTemp, 1);
    copyFloatToArray(user::targetMinHumidity, 1);
    copyFloatToArray(user::targetMaxHumidity, 1);
    copyIntToArray(user::fansControlTemperature);
    copyIntToArray(user::fansControlHumidity);
    copyIntToArray(user::fanOneFixedSpeed);
    copyIntToArray(user::fanTwoFixedSpeed);
  }
  // Page 12 (Warnings page)
  else if (page == 12) {  // small amount of data so no need to segment based on slider
    copyIntToArray(user::convertToF); 
    copyIntToArray(user::convertToInches); 
    copyIntToArray(sensor::tds);
    copyIntToArray(user::targetMinTds);
    copyIntToArray(user::targetMaxTds);
    copyIntToArray(user::tdsErrorMargin);
    copyFloatToArray(sensor::ec, 2);
    copyFloatToArray(user::targetMinEc, 2);
    copyFloatToArray(user::targetMaxEc, 2);
    copyFloatToArray(user::ecErrorMargin, 2);
    copyFloatToArray(sensor::ph, 2);
    copyFloatToArray(user::targetMinPh, 2);
    copyFloatToArray(user::targetMaxPh, 2);
    copyFloatToArray(user::phErrorMargin, 2);
    copyIntToArray(sensor::co2);
    copyIntToArray(user::targetCo2);
    copyIntToArray(user::targetCo2);
    copyIntToArray(user::co2ErrorMargin);
    copyFloatToArray(sensor::waterTemp, 1);
    copyFloatToArray(user::targetMinWaterTemp, 1);
    copyFloatToArray(user::targetMaxWaterTemp, 1);
    copyFloatToArray(user::waterTempErrorMargin, 1);
    copyFloatToArray(sensor::waterLevel, 1);
    copyFloatToArray(user::targetMinWaterHeight, 1);
    copyFloatToArray(user::targetMaxWaterHeight, 1);
    copyFloatToArray(user::waterHeightErrorMargin, 1);
    copyFloatToArray(sensor::airTemp, 1);
    copyFloatToArray(user::targetMinAirTemp, 1);
    copyFloatToArray(user::targetMaxAirTemp, 1);
    copyFloatToArray(user::airTempErrorMargin, 1);
    copyFloatToArray(sensor::humidity, 1);
    copyFloatToArray(user::targetMinHumidity, 1);
    copyFloatToArray(user::targetMaxHumidity, 1);
    copyFloatToArray(user::humidityErrorMargin, 1);
  }
  else if (device::globalDebug) {
    Serial.print(F("Page not found!"));
  }
  // Send responce to the server
  if (strlen(wifi::buffer) > 0)
    wifi::buffer[strlen(wifi::buffer) - 1] = 0;
  server.send(200, "text/plane", wifi::buffer);
  yield();
}

bool strBool(const char* buffer) {
  if (strstr(buffer, "true") != NULL)
    return true;
  else
    return false;
}

void copyBoolArrayToArray(const bool* a_array, const int& a_len) {
  for (int i = 0; i < a_len; i++) {
    copyIntToArray(a_array[i]);
  }
}

void copyIntToArray(const int a_value) {
  char buffer[8]{ 0, 0, 0, 0, 0, 0, 0, 0 };
  itoa(a_value, buffer, 10);
  strcat(wifi::buffer, buffer);
  strcat(wifi::buffer, ",");
  delay(1);
  yield();
}

void copyFloatArrayToArray(const float* a_array, const int& a_len, const uint8_t& a_precison) {
  for (int i = 0; i < a_len; i++) {
    copyFloatToArray(a_array[i], a_precison);
  }
}

void copyFloatToArray(const float a_value, const uint8_t& a_precison) {
  char buffer[8]{ 0, 0, 0, 0, 0, 0, 0, 0 };
  dtostrf(a_value, 0, a_precison, buffer);
  strcat(wifi::buffer, buffer);
  strcat(wifi::buffer, ",");
  delay(1);
  yield();
}

void buffToCharArray(const char* a_src, char* a_dest, const int& a_size) {
  for (uint8_t i = 0; i < a_size; i++) {
    a_dest[i] = a_src[i];
  }
  delay(1);
  yield();
}

void buffToBoolArray(char* a_src, bool* a_dest, const int& a_size) {
  uint8_t i = 0;
  char* data = strtok(a_src, ",");
  while (data != NULL) {
    if (i < a_size)
      a_dest[i++] = atoi(data);
    data = strtok(NULL, ",");
  }
  delay(1);
  yield();
}

// Convert a float to int, with a precison of 2 decimal places
int fltToInt(const float& a_value) {
  int val = a_value * 100;
  return val;
}

// Convert a float to int, with a precison of 3 decimal places
int doubleToInt(const double& a_value) {
  int val = a_value * 1000;
  return val;
}
/*
// convert a celsius value to ferinheight
float convertToF(const float& a_value) {
  return (a_value * 1.8) + 32;
}

// convert a centimeter value to inches
float convertToInch(const float& a_value) {
  return a_value / 2.54;
}
*/