void setValue(const int a_cmd, const char* a_val) {
  Serial.print(F("Received cmd: ")); Serial.print(a_cmd); Serial.print(F(" and val: ")); Serial.println(a_val);
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
  // convertToInchs
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

void buffToCharArray(const char* a_src, char* a_dest, const int& a_size) {
  for (uint8_t i = 0; i < a_size; i++) {
    a_dest[i] = a_src[i];
  }
  delay(1);
}

void buffToBoolArray(const char* a_src, bool* a_dest, const int& a_size) {
  uint8_t i = 0;
  char* data = strtok((char*)a_src, ",");
  while (data != NULL) {
    if (i < a_size)
      a_dest[i++] = atoi(data);
    data = strtok(NULL, ",");
  }
  delay(1);
}

bool strBool(const char* buffer) {
  if (strstr(buffer, "true") != NULL)
    return true;
  else
    return false;
}