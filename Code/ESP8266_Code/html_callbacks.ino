void handleRoot() {
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.sendHeader("Content-Length", (String)sizeof(MAIN_page));
  server.send(200, "text/html", "");
  // send webpage in 2048-bit packets
  unsigned int byteCount = 0;
  char packetBuffer[2049];
  for (int n = 0; n < 2049; n++) {
    packetBuffer[n] = 0;
  }
  if (debugEnabled) {
    Serial.print(F("Sending ")); Serial.print(sizeof(MAIN_page)); Serial.println(F(" bytes..."));
  }
  for (unsigned int i = 0; i < sizeof(MAIN_page);) {
    if (byteCount < 2048) {
      packetBuffer[byteCount] = pgm_read_byte(MAIN_page + i++);
      byteCount++;
    }
    else {
      server.sendContent_P(packetBuffer, byteCount);
      //if (debugEnabled)
      //Serial.print(packetBuffer);
      byteCount = 0;
      for (int n = 0; n < 2049; n++) {
        packetBuffer[n] = 0;
      }
      yield();
    }
  }
  if (byteCount > 0) {
    //if (debugEnabled)
    //Serial.print(packetBuffer);
    server.sendContent_P(packetBuffer, byteCount);
  }
  yield();
  if (debugEnabled)
    Serial.println(F("Packets sent!"));
}

void getData() {
  buildArgs();
  server.send(200, "text/plane", serialData);
  yield();
}

void setValue() {
  yield();
  String cmd = server.arg("cmd");
  char val[64] {0};
  server.arg("val").toCharArray(val, 64);
  if (cmd == "" || val[0] == 0) { //Parameters not found
    if (debugEnabled)
      Serial.println(F("Arguments not found!"));
  }
  else {
    //float tempVal = atof(val);
    if (debugEnabled) {
      Serial.print(F("Received cmd:")); Serial.print(cmd); Serial.print(F(" and val: ")); Serial.println(val);
    }
    // ============== PROFILES PAGE  ==============
    // profileOneName
    if (cmd == "profileOneName") {
      buffToCharArray(val, profileOneName, 11);
    }
    // profileTwoName
    else if (cmd == "profileTwoName") {
      buffToCharArray(val, profileTwoName, 11);
    }
    // profileThreeName
    else if (cmd == "profileThreeName") {
      buffToCharArray(val, profileThreeName, 11);
    }
    // profileFourName
    else if (cmd == "profileFourName") {
      buffToCharArray(val, profileFourName, 11);
    }
    // profileFiveName
    else if (cmd == "profileFiveName") {
      buffToCharArray(val, profileFiveName, 11);
    }
    // loadData
    else if (cmd == "loadData") {
      loadUserProfile = atoi(val);
    }
    // saveData
    else if (cmd == "saveData") {
      saveUserProfile = atoi(val);
    }
    // ============== SETTINGS PAGE  ==============
    // numberOfDosers
    else if (cmd == "numDosers") {
      numberOfDosers = atoi(val);
    }
    // dosingInterval
    else if (cmd == "dosingInt") {
      dosingInterval = atoi(val);
    }
    // convertToFarenheit
    else if (cmd == "convertToF") {
      convertToFarenheit = strBool(val);
    }
    // convertToInchs
    else if (cmd == "convertToInchs") {
      convertToInchs = strBool(val);
    }
    // convertToTds
    else if (cmd == "convertToTds") {
      convertToTds = strBool(val);
    }
    // disableCo2Warnings
    else if (cmd == "disableCo2Warn") {
      disableCo2Warnings = strBool(val);
    }
    // disableEcWarnings
    else if (cmd == "disableEcWarn") {
      disableEcWarnings = strBool(val);
    }
    // disablePpmWarnings
    else if (cmd == "disablePpmWarn") {
      disablePpmWarnings = strBool(val);
    }
    // disablePhWarnings
    else if (cmd == "disablePhWarn") {
      disablePhWarnings = strBool(val);
    }
    // disableWaterTempWarnings
    else if (cmd == "disableWaterTempWarn") {
      disableWaterTempWarnings = strBool(val);
    }
    // disableWaterHeightWarnings
    else if (cmd == "disableWaterHeightWarn") {
      disableWaterHeightWarnings = strBool(val);
    }
    // disableAirTempWarnings
    else if (cmd == "disableAirTempWarn") {
      disableAirTempWarnings = strBool(val);
    }
    // disableHumidityWarnings
    else if (cmd == "disableHumidityWarn") {
      disableHumidityWarnings = strBool(val);
    }
    // ============== EC PAGE ==============
    // targetMinEc
    else if (cmd == "targetMinEc") {
      targetMinEc = atof(val);
    }
    // targetMaxEc
    else if (cmd == "targetMaxEc") {
      targetMaxEc = atof(val);
    }
    // ecOffset
    else if (cmd == "ecOffset") {
      ecOffset = atof(val);
    }
    // ============== TDS/PPM PAGE ==============
    // targetMinTds
    else if (cmd == "targetMinTds") {
      targetMinTds = atof(val);
    }
    // targetMaxTds
    else if (cmd == "targetMaxTds") {
      targetMaxTds = atof(val);
    }
    // tdsOffset
    else if (cmd == "tdsOffset") {
      tdsOffset = atof(val);
    }
    // ============== PH PAGE ==============
    // targetMinPh
    else if (cmd == "targetMinPh") {
      targetMinPh = atof(val);
    }
    // targetMaxPh
    else if (cmd == "targetMaxPh") {
      targetMaxPh = atof(val);
    }
    // phOffset
    else if (cmd == "phOffset") {
      phOffset = atof(val);
    }
    // ============== Co2 PAGE ==============
    // targetCo2
    else if (cmd == "targetCo2") {
      targetCo2 = atoi(val);
    }
    // co2Offset
    else if (cmd == "co2Offset") {
      co2Offset = atoi(val);
    }
    // roomLength
    else if (cmd == "roomLength") {
      roomLength = atoi(val);
    }
    // roomWidth
    else if (cmd == "roomWidth") {
      roomWidth = atoi(val);
    }
    // roomHeight
    else if (cmd == "roomHeight") {
      roomHeight = atoi(val);
    }
    // co2Flowrate
    else if (cmd == "co2Flowrate") {
      co2Flowrate = atof(val);
    }
    // co2CheckTimeMinute
    else if (cmd == "co2CheckTimeMin") {
      co2CheckTimeMinute = atoi(val);
    }
    // co2CheckTimeHour
    else if (cmd == "co2CheckTimeHour") {
      co2CheckTimeHour = atoi(val);
    }
    // disableFansTimer
    else if (cmd == "disableFansTimer") {
      disableFansTimer = atoi(val);
    }
    // manualCo2Duration
    else if (cmd == "gasTimeEnabled") {
      manualCo2Duration = strBool(val);
    }
    // manualCo2GasDuration
    else if (cmd == "gasTime") {
      manualCo2GasDuration = atoi(val);
    }
    // disableCo2Control
    else if (cmd == "disableCo2Control") {
      disableCo2Control = strBool(val);
    }
    // ============== WATER PAGE ==============
    // targetMinWaterHeight
    else if (cmd == "targetMinWaterLvl") {
      targetMinWaterHeight = atof(val);
    }
    // targetMaxWaterHeight
    else if (cmd == "targetMaxWaterLvl") {
      targetMaxWaterHeight = atof(val);
    }
    // waterHeightOffset
    else if (cmd == "targetWaterLvlOffset") {
      waterHeightOffset = atof(val);
    }
    // targetMinWaterTemp
    else if (cmd == "targetMinWaterTemp") {
      targetMinWaterTemp = atof(val);
    }
    // targetMaxWaterTemp
    else if (cmd == "targetMaxWaterTemp") {
      targetMaxWaterTemp = atof(val);
    }
    // waterTempOffset
    else if (cmd == "targetWaterTempOffset") {
      waterTempOffset = atof(val);
    }
    // autoFillDays
    else if (cmd == "autoFillDays") {
      buffToBoolArray(val, autoFillDays, 31);
    }
    // autoFillMinute
    else if (cmd == "autoFillMinute") {
      autoFillMinute = atoi(val);
    }
    // autoFillHour
    else if (cmd == "autoFillHour") {
      autoFillHour = atoi(val);
    }
    // disableDrainAndRefill
    else if (cmd == "disableRefillDrain") {
      disableDrainAndRefill = strBool(val);
    }
    // refillDoserOneMills
    else if (cmd == "d1mls") {
      refillDoserOneMills = atoi(val);
    }
    // refillDoserTwoMills
    else if (cmd == "d2mls") {
      refillDoserTwoMills = atoi(val);
    }
    // refillDoserThreeMills
    else if (cmd == "d3mls") {
      refillDoserThreeMills = atoi(val);
    }
    // refillDoserFourMills
    else if (cmd == "d4mls") {
      refillDoserFourMills = atoi(val);
    }
    // refillDoserFiveMills
    else if (cmd == "d5mls") {
      refillDoserFiveMills = atoi(val);
    }
    // refillDoserSixMills
    else if (cmd == "d6mls") {
      refillDoserSixMills = atoi(val);
    }
    // ============== DOSER PAGE ==============
    // doserOneMills
    else if (cmd == "doserMls_1") {
      doserOneMills = atoi(val);
    }
    // doserTwoMills
    else if (cmd == "doserMls_2") {
      doserTwoMills = atoi(val);
    }
    // doserThreeMills
    else if (cmd == "doserMls_3") {
      doserThreeMills = atoi(val);
    }
    // doserFourMills
    else if (cmd == "doserMls_4") {
      doserFourMills = atoi(val);
    }
    // doserFiveMills
    else if (cmd == "doserMls_5") {
      doserFiveMills = atoi(val);
    }
    // doserSixMills
    else if (cmd == "doserMls_6") {
      doserSixMills = atoi(val);
    }
    // doserOneMode
    else if (cmd == "doserOneMode") {
      doserOneMode = atoi(val);
    }
    // doserTwoMode
    else if (cmd == "doserTwoMode") {
      doserTwoMode = atoi(val);
    }
    // doserThreeMode
    else if (cmd == "doserThreeMode") {
      doserThreeMode = atoi(val);
    }
    // doserFourMode
    else if (cmd == "doserFourMode") {
      doserFourMode = atoi(val);
    }
    // doserFiveMode
    else if (cmd == "doserFiveMode") {
      doserFiveMode = atoi(val);
    }
    // doserSixMode
    else if (cmd == "doserSixMode") {
      doserSixMode = atoi(val);
    }
    // ============== LIGHT PAGE ==============
    // lightOnTimeMin
    else if (cmd == "onTimeMin") {
      lightOnTimeMin = atoi(val);
    }
    // lightOnTimeHour
    else if (cmd == "onTimeHour") {
      lightOnTimeHour = atoi(val);
    }
    // lightOffTimeMin
    else if (cmd == "offTimeMin") {
      lightOffTimeMin = atoi(val);
    }
    // lightOffTimeHour
    else if (cmd == "offTimeHour") {
      lightOffTimeHour = atoi(val);
    }
    // lightMode
    else if (cmd == "lightMode") {
      lightMode = atoi(val);
    }
    // ============== FANS PAGE ==============
    // targetMinFanOneSpeed
    else if (cmd == "fanOneMinSpeed") {
      targetMinFanOneSpeed = atoi(val);
    }
    // targetMaxFanOneSpeed
    else if (cmd == "fanOneMaxSpeed") {
      targetMaxFanOneSpeed = atoi(val);
    }
    // targetMinFanTwoSpeed
    else if (cmd == "fanTwoMinSpeed") {
      targetMinFanTwoSpeed = atoi(val);
    }
    // targetMaxFanTwoSpeed
    else if (cmd == "fanTwoMaxSpeed") {
      targetMaxFanTwoSpeed = atoi(val);
    }
    // targetMinAirTemp
    else if (cmd == "targetMinAirTemp") {
      targetMinAirTemp = atof(val);
    }
    // targetMaxAirTemp
    else if (cmd == "targetMaxAirTemp") {
      targetMaxAirTemp = atof(val);
    }
    // airTempOffset
    else if (cmd == "airTempOffset") {
      airTempOffset = atof(val);
    }
    // targetMinHumidity
    else if (cmd == "targetMinHumidity") {
      targetMinHumidity = atof(val);
    }
    // targetMaxHumidity
    else if (cmd == "targetMaxHumidity") {
      targetMaxHumidity = atof(val);
    }
    // humidityOffset
    else if (cmd == "humidityOffset") {
      humidityOffset = atof(val);
    }
    // fansControlTemp
    else if (cmd == "fansControlTemp") {
      fansControlTemperature = strBool(val);
    }
    // fansControlHum
    else if (cmd == "fansControlHum") {
      fansControlHumidity = strBool(val);
    }
    // fanOneFixedSpeed
    else if (cmd == "fanOneFixedSpeed") {
      fanOneFixedSpeed = strBool(val);
    }
    // fanTwoFixedSpeed
    else if (cmd == "fanTwoFixedSpeed") {
      fanTwoFixedSpeed = strBool(val);
    }
    // ============== WARNINGS PAGE ==============
    // ecErrorMargin
    else if (cmd == "ecErrorMargin") {
      ecErrorMargin = atof(val);
    }
    // tdsErrorMargin
    else if (cmd == "tdsErrorMargin") {
      tdsErrorMargin = atoi(val);
    }
    // phErrorMargin
    else if (cmd == "phErrorMargin") {
      phErrorMargin = atof(val);
    }
    // co2ErrorMargin
    else if (cmd == "co2ErrorMargin") {
      co2ErrorMargin = atoi(val);
    }
    // waterHeightErrorMargin
    else if (cmd == "waterHeightErrorMargin") {
      waterHeightErrorMargin = atof(val);
    }
    // waterTempErrorMargin
    else if (cmd == "waterTempErrorMargin") {
      waterTempErrorMargin = atof(val);
    }
    // airTempErrorMargin
    else if (cmd == "airTempErrorMargin") {
      airTempErrorMargin = atof(val);
    }
    // humidityErrorMargin
    else if (cmd == "humidityErrorMargin") {
      humidityErrorMargin = atof(val);
    }
    else if (debugEnabled)
      Serial.println(F("server arg not recognised!"));
  }
  server.send(200, "text/plane", "OK");
}

void buildArgs() {
  if (debugEnabled)
    Serial.println(F("Building args..."));
  for (unsigned int i = 0; i < sizeof(serialData); i++) {
    serialData[i] = 0;
  }
  ESP.wdtFeed();
  // ec array val 0
  char buffer[8] {0, 0, 0, 0, 0, 0, 0, 0};
  dtostrf(ecArray[0], 0, 2, buffer);
  strcpy(serialData, buffer);
  strcat(serialData, ",");
  // ec array 12 floats (vals 1 - 11)
  for (byte i = 1; i < maxGraphArrayValues; i++) {
    copyFloatToArray(ecArray[i], 2);
  }
  // tds array 12 ints (vals 12 - 23)
  for (byte i = 0; i < maxGraphArrayValues; i++) {
    copyIntToArrray(tdsArray[i]);
  }
  // ph array 12 floats (vals 24 - 35)
  for (byte i = 0; i < maxGraphArrayValues; i++) {
    copyFloatToArray(phArray[i], 2);
  }
  // co2 array 12 floats (vals 36 - 47)
  for (byte i = 0; i < maxGraphArrayValues; i++) {
    copyIntToArrray(co2Array[i]);
  }
  // water Level array 12 ints (vals 48 - 59)
  for (byte i = 0; i < maxGraphArrayValues; i++) {
    copyFloatToArray(waterTempArray[i], 1);
  }
    // air temperature array 12 floats (vals 60 - 71)
  for (byte i = 0; i < maxGraphArrayValues; i++) {
    copyFloatToArray(waterLevelArray[i], 1);
  }
 // humidity array 12 ints (vals 72 - 83)
  for (byte i = 0; i < maxGraphArrayValues; i++) {
    copyFloatToArray(airTempArray[i], 1);
  }
   // fan one speed array 12 ints (vals 84 - 95)
  for (byte i = 0; i < maxGraphArrayValues; i++) {
    copyFloatToArray(humidityArray[i], 1);
  }
 // fan two speed array 12 ints (vals 96 - 107)
  for (byte i = 0; i < maxGraphArrayValues; i++) {
    copyIntToArrray(fanOneSpeedArray[i]);
  }
 // fan two speed array 12 ints (vals 108 - 119)
  for (byte i = 0; i < maxGraphArrayValues; i++) {
    copyIntToArrray(fanTwoSpeedArray[i]);
  }
  // auto fill days array 31 ints (vals 120 - 150)
  for (byte i = 0; i < 31; i++) {
    copyIntToArrray(autoFillDays[i]);
  }
  // graph array position (val 151)  OK
  copyIntToArrray(graphArrayPos);
  // profile 1 name (val 152)
  strcat(serialData, profileOneName);
  strcat(serialData, ",");
  // profile 2 name (val 153)
  strcat(serialData, profileTwoName);
  strcat(serialData, ",");
  // profile 3 name (vals 154)
  strcat(serialData, profileThreeName);
  strcat(serialData, ",");
  // profile 4 name (vals 155)
  strcat(serialData, profileFourName);
  strcat(serialData, ",");
  // profile 5 name (vals 156)
  strcat(serialData, profileFiveName);
  strcat(serialData, ",");
  // Light on time hour (val 157)
  copyIntToArrray(lightOnTimeHour);
  // Light on time min (val 158)
  copyIntToArrray(lightOnTimeMin);
  // Light off time hour (val 159)
  copyIntToArrray(lightOffTimeHour);
  // Light off time min (val 160)
  copyIntToArrray(lightOffTimeMin);
  // Light mode (val 161)
  copyIntToArrray(lightMode);
  // ec sensor (val 162)
  copyFloatToArray(ec, 2);
  // tds sensor (val 163)
  copyIntToArrray(tds);
  // ph sensor (val 164)
  copyFloatToArray(ph, 2);
  // co2 sensor (val 165)
  copyIntToArrray(co2);
  // water temp sensor (val 166)
  copyFloatToArray(waterTemp, 1);
  // water level sensor (val 167)
  copyFloatToArray(waterLevel, 1);
  // air temp sensor (val 168)
  copyFloatToArray(airTemp, 1);
  // humidity sensor (val 169)
  copyFloatToArray(humidity, 1);
  // fan One Speed (val 170)
  copyIntToArrray(fanOneSpeed);
  // fan two Speed (val 171)
  copyIntToArrray(fanTwoSpeed);
  // fan One Min Speed (val 172)
  copyIntToArrray(targetMinFanOneSpeed);
  // fan One Max Speed (val 173)
  copyIntToArrray(targetMaxFanOneSpeed);
  // fan Two Min Speed (val 174)
  copyIntToArrray(targetMinFanTwoSpeed);
  // fan Two Max Speed (val 175)
  copyIntToArrray(targetMaxFanTwoSpeed);
  // fans Control Temp (val 176)
  copyIntToArrray(fansControlTemperature);
  // fans Control Hum (val 177)
  copyIntToArrray(fansControlHumidity);
  // fan One Fixed Speed (val 178)
  copyIntToArrray(fanOneFixedSpeed);
  // fan Two Fixed Speed (val 179)
  copyIntToArrray(fanTwoFixedSpeed);
  // target Min Air Temp (val 180)
  copyFloatToArray(targetMinAirTemp, 1);
  // target Max Air Temp (val 181)
  copyFloatToArray(targetMaxAirTemp, 1);
  // air Temp Offset (val 182)
  copyFloatToArray(airTempOffset, 1);
  // target Min Humidity (val 183)
  copyFloatToArray(targetMinHumidity, 1);
  // target Max Humidity (val 184)
  copyFloatToArray(targetMaxHumidity, 1);
  // humidity Offset (val 185)
  copyFloatToArray(humidityOffset, 1);
  // target Min Tds (val 186)
  copyIntToArrray(targetMinTds);
  // target Max Tds (val 187)
  copyIntToArrray(targetMaxTds);
  // tds Offset (val 188)
  copyIntToArrray(tdsOffset);
  // target Co2 (val 189)
  copyIntToArrray(targetCo2);
  // co2 Offset (val 190)
  copyIntToArrray(co2Offset);
  // room Width (val 191)
  copyIntToArrray(roomWidth);
  // room Length (val 192)
  copyIntToArrray(roomLength);
  // room Height (val 193)
  copyIntToArrray(roomHeight);
  // co2 Flow rate (val 194)
  copyFloatToArray(co2Flowrate, 1);
  // co2 Check Time Minute (val 195)
  copyIntToArrray(co2CheckTimeMinute);
  // co2 Check Time Hour (val 196)
  copyIntToArrray(co2CheckTimeHour);
  // disable Fans Timer (val 197)
  copyIntToArrray(disableFansTimer);
  // manual Co2 Gas Time Enabled (val 198)
  copyIntToArrray(manualCo2Duration);
  // manual Gas Duration Time (val 199)
  copyIntToArrray(manualCo2GasDuration);
  // co2 Control Disabled (val 200)
  copyIntToArrray(disableCo2Control);
  // target Min Ph (val 201)
  copyFloatToArray(targetMinPh, 2);
  // target Max Ph (val 202)
  copyFloatToArray(targetMaxPh, 2);
  // ph Offset (val 203)
  copyFloatToArray(phOffset, 2);
  // target Min Ec (val 204)
  copyFloatToArray(targetMinEc, 2);
  // target Max Ec (val 205)
  copyFloatToArray(targetMaxEc, 2);
  // ec Offset (val 206)
  copyFloatToArray(ecOffset, 2);
  // target Min Water Temp (val 207)
  copyFloatToArray(targetMinWaterTemp, 1);
  // target Max Water Temp (val 208)
  copyFloatToArray(targetMaxWaterTemp, 1);
  // water Temp Offset (val 209)
  copyFloatToArray(waterTempOffset, 1);
  // target Min Water Height (val 210)
  copyFloatToArray(targetMinWaterHeight, 1);
  // target Max Water Height (val 211)
  copyFloatToArray(targetMaxWaterHeight, 1);
  // water Height Offset (val 212)
  copyFloatToArray(waterHeightOffset, 1);
  // min Ph (val 213)
  copyFloatToArray(minPh, 2);
  // max Ph (val 214)
  copyFloatToArray(maxPh, 2);
  // min Co2 (val 215)
  copyIntToArrray(minCo2);
  // max Co2 (val 216)
  copyIntToArrray(maxCo2);
  // min Ec (val 217)
  copyFloatToArray(minEc, 2);
  // max Ec (val 218)
  copyFloatToArray(maxEc, 2);
  // min Water Temp (val 219)
  copyFloatToArray(minWaterTemp, 1);
  // max Water Temp (val 220)
  copyFloatToArray(maxWaterTemp, 1);
  // min Water Level (val 221)
  copyFloatToArray(minWaterLevel, 1);
  // max Water Level (val 222)
  copyFloatToArray(maxWaterLevel, 1);
  // min Tds (val 223)
  copyIntToArrray(minTds);
  // max Tds (val 224)
  copyIntToArrray(maxTds);
  // min Air Temp (val 225)
  copyFloatToArray(minAirTemp, 1);
  // max Air Temp (val 226)
  copyFloatToArray(maxAirTemp, 1);
  // min Humidity (val 227)
  copyFloatToArray(minHumidity, 1);
  // max Humidity (val 228)
  copyFloatToArray(maxHumidity, 1);
  // min Fan One Speed (val 229)
  copyIntToArrray(minFanOneSpeed);
  // max Fan One Speed (val 230)
  copyIntToArrray(maxFanOneSpeed);
  // min Fan Two Speed (val 231)
  copyIntToArrray(minFanTwoSpeed);
  // max Fan Two Speed (val 232)
  copyIntToArrray(maxFanTwoSpeed);
  // num Dosers (val 233)
  copyIntToArrray(numberOfDosers);
  // doser One Mode (val 234)
  copyIntToArrray(doserOneMode);
  // doser Two Mode (val 235)
  copyIntToArrray(doserTwoMode);
  // doser Three Mode (val 236)
  copyIntToArrray(doserThreeMode);
  // doser Four Mode (val 237)
  copyIntToArrray(doserFourMode);
  // doser Five Mode (val 238)
  copyIntToArrray(doserFiveMode);
  // doser Six Mode (val 239)
  copyIntToArrray(doserSixMode);
  // doser One Mills (val 240)
  copyIntToArrray(doserOneMills);
  // doser Two Mills (val 241)
  copyIntToArrray(doserTwoMills);
  // doser Three Mills (val 242)
  copyIntToArrray(doserThreeMills);
  // doser Four Mills (val 243)
  copyIntToArrray(doserFourMills);
  // doser Five Mills (val 244)
  copyIntToArrray(doserFiveMills);
  // doser Six Mills (val 245)
  copyIntToArrray(doserSixMills);
  // doser One Speed
  //copyIntToArrray(doserOneSpeed);
  // doser TwoSpeed
  //copyIntToArrray(doserTwoSpeed);
  // doser ThreeSpeed
  //copyIntToArrray(doserThreeSpeed);
  // doser FourSpeed
  //copyIntToArrray(doserFourSpeed);
  // doser Five Speed
  //copyIntToArrray(doserFiveSpeed);
  // doser Six Speed
  //copyIntToArrray(doserSixSpeed);
  // dosing Interval (val 246)
  copyIntToArrray(dosingInterval);
  // swap Interval (val 247)
  copyIntToArrray(swapInterval);
  // refill Doser One Mills (val 248)
  copyIntToArrray(refillDoserOneMills);
  // refill Doser Two Mills (val 249)
  copyIntToArrray(refillDoserTwoMills);
  // refill Doser Three Mills (val 250)
  copyIntToArrray(refillDoserThreeMills);
  // refill Doser Four Mills (val 251)
  copyIntToArrray(refillDoserFourMills);
  // refill Doser Five Mills (val 252)
  copyIntToArrray(refillDoserFiveMills);
  // refill Doser Six Mills (val 253)
  copyIntToArrray(refillDoserSixMills);
  // auto Fill Hour (val 254)
  copyIntToArrray(autoFillHour);
  // auto Fill Minute (val 255)
  copyIntToArrray(autoFillMinute);
  // tds Error Margin (val 256)
  copyIntToArrray(tdsErrorMargin);
  // ec Error Margin (val 257)
  copyFloatToArray(ecErrorMargin, 2);
  // ph Error Margin (val 258)
  copyFloatToArray(phErrorMargin, 2);
  // co2 Error Margin (val 259)
  copyIntToArrray(co2ErrorMargin);
  // water Temp Error Margin (val 260)
  copyFloatToArray(waterTempErrorMargin, 1);
  // water Height Error Margin (val 261)
  copyFloatToArray(waterHeightErrorMargin, 1);
  // air Temp Error Margin (val 262)
  copyFloatToArray(airTempErrorMargin, 1);
  // humidity Error Margin (val 263)
  copyFloatToArray(humidityErrorMargin, 1);
  // disable Ppm Warning (val 264)
  copyIntToArrray(disablePpmWarnings);
  // disable Ec Warnings (val 265)
  copyIntToArrray(disableEcWarnings);
  // disable Co2 Warnings (val 266)
  copyIntToArrray(disableCo2Warnings);
  // disable Ph Warnings (val 267)
  copyIntToArrray(disablePhWarnings);
  // disable Water Temp Warnings (val 268)
  copyIntToArrray(disableWaterTempWarnings);
  // disable Water Height Warnings (val 269)
  copyIntToArrray(disableWaterHeightWarnings);
  // disable Air Temp Warnings (val 270)
  copyIntToArrray(disableAirTempWarnings);
  // disable Humidity Warnings (val 271)
  copyIntToArrray(disableHumidityWarnings);
  // disable Drain And Refill (val 272)
  copyIntToArrray(disableDrainAndRefill);
  // convert Celcius To Farenheit (val 273)
  copyIntToArrray(convertToFarenheit);
  // convert cm To Inchs (val 274)
  copyIntToArrray(convertToInchs);
  // convert To Tds (val 2275)
  copyIntToArrray(convertToTds);
  // ppm Error State (val 276)
  copyIntToArrray(ppmErrorState);
  // ec Error State (val 277)
  copyIntToArrray(ecErrorState);
  // ph Error State (val 278)
  copyIntToArrray(phErrorState);
  // co2 Error State (val 279)
  copyIntToArrray(co2ErrorState);
  // water Level Error State (val 280)
  copyIntToArrray(waterTempErrorState);
  // water Level Error State (val 281)
  copyIntToArrray(waterLevelErrorState);
  // air Temp Error State (val 282)
  copyIntToArrray(airTempErrorState);
  // humidity Error State (val 283)
  copyIntToArrray(humidityErrorState);
  // user Profile (val 284)
  copyIntToArrray(userProfile);
  ESP.wdtFeed();
  yield();
  //if (debugEnabled) {
  //Serial.print(F("built serialData array:")); Serial.println(serialData);
  //}
}

bool strBool(const char* buffer) {
  if (strstr(buffer, "true") != NULL)
    return true;
  else
    return false;
}

void copyIntToArrray(const int& a_value) {
  char buffer[8] {0, 0, 0, 0, 0, 0, 0, 0};
  itoa(a_value, buffer, 10);
  strcat(serialData, buffer);
  strcat(serialData, ",");
  ESP.wdtFeed();
  yield();
}

void copyFloatToArray(float& a_value, const byte& a_precison) {
  char buffer[8] {0, 0, 0, 0, 0, 0, 0, 0};
  dtostrf(a_value, 0, a_precison, buffer);
  strcat(serialData, buffer);
  strcat(serialData, ",");
  ESP.wdtFeed();
  yield();
}

void buffToCharArray(const char* a_src, char* a_dest, const int& a_size) {
  for (byte i = 0; i < a_size; i++) {
    a_dest[i] = a_src[i];
  }
  ESP.wdtFeed();
  yield();
}

void buffToBoolArray(char* a_src, bool* a_dest, const int& a_size) {
  byte i = 0;
  char* data = strtok(a_src, ",");
  while (data != NULL) {
    if (i < a_size)
      a_dest[i++] = atoi(data);
    data = strtok(NULL, ",");
  }
  ESP.wdtFeed();
  yield();
}
