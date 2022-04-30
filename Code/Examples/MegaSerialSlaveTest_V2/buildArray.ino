void buildArgs() {
  if (debugEnabled)
    Serial.println(F("Building args..."));
  for (unsigned int i = 0; i < sizeof(serialData); i++) {
    serialData[i] = 0;
  }
  //ESP.wdtFeed();
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
  // water temperature array 12 floats (vals 36 - 47)
  for (byte i = 0; i < maxGraphArrayValues; i++) {
    copyIntToArrray(co2Array[i], 1);
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
  //ESP.wdtFeed();
  //yield();
  if (debugEnabled) {
    Serial.print(F("built serialData array:")); Serial.println(serialData);
  }
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
}

void copyFloatToArray(float& a_value, const byte& a_precison) {
  char buffer[8] {0, 0, 0, 0, 0, 0, 0, 0};
  dtostrf(a_value, 0, a_precison, buffer);
  strcat(serialData, buffer);
  strcat(serialData, ",");
}

void buffToCharArray(const char* a_src, char* a_dest, const int& a_size) {
  for (byte i = 0; i < a_size; i++) {
    a_dest[i] = a_src[i];
  }
}
