void initializeEEPROM() {
  char versionNum[6] {'0', '0', '0', '0', '0', '\0'};
  for (byte i = 0; i < 6; i++)
    versionNum[i] = EEPROM.read(i);
  EEPROM.get(6, device::systemReset);
  Serial.print(F("System reset:")); Serial.println(device::systemReset);
  Serial.print(F("Current version number:")); Serial.println(versionNum);
  Serial.print(F("Software version number:")); Serial.println(device::versionNumber);
  if (strcmp(versionNum, device::versionNumber) == 0 && !device::systemReset) {
    device::userProfile = EEPROM.read(7);
    loadSystemEEPROM();
    loadUserEEPROM(device::userProfile);
  }
  else {
    showResetMessage();
    saveSystemEEPROM();
    device::systemReset = false;
    for (byte i = 0; i < 5; i++)
      saveUserEEPROM(i);
    for (byte i = 0; i < 6; i++)
      EEPROM.put(i, device::versionNumber[i]);
    EEPROM.put(6, device::systemReset);
    EEPROM.put(7, device::userProfile); // set profile to 0
    clearPage();
  }
}

void saveSystemEEPROM() {
  int address = 8;
  Serial.print(F("Saving system settings\nEEPROM starting at address: ")); Serial.println(address);
  EEPROM.put(address, sensor::phPotStep);                 address += sizeof(sensor::phPotStep);
  EEPROM.put(address, sensor::phVoltagePerUnit);          address += sizeof(sensor::phVoltagePerUnit);
  EEPROM.put(address, sensor::ph4Voltage);                address += sizeof(sensor::ph4Voltage);
  EEPROM.put(address, sensor::ph7Voltage);                address += sizeof(sensor::ph7Voltage);
  EEPROM.put(address, sensor::ecCalibration);             address += sizeof(sensor::ecCalibration);
  EEPROM.put(address, sensor::emptyWaterTankDepth);       address += sizeof(sensor::emptyWaterTankDepth);
  EEPROM.put(address, sensor::etapeZeroVolumeResistance); address += sizeof(sensor::etapeZeroVolumeResistance);
  EEPROM.put(address, sensor::etapeMaxVolumeResistance);  address += sizeof(sensor::etapeMaxVolumeResistance);
  //
  for (const char& val : user::profileOneName) {
    EEPROM.put(address, val);
    address += sizeof(val);
  }
  for (const char& val : user::profileTwoName) {
    EEPROM.put(address, val);
    address += sizeof(val);
  }
  for (const char& val : user::profileThreeName) {
    EEPROM.put(address, val);
    address += sizeof(val);
  }
  for (const char& val : user::profileFourName) {
    EEPROM.put(address, val);
    address += sizeof(val);
  }
  for (const char& val : user::profileFiveName) {
    EEPROM.put(address, val);
    address += sizeof(val);
  }
  for (byte i = 0; i < message::maxLogs; i++) {
    EEPROM.put(address++, message::logTypeArray[i]);
  }
  for (byte i = 0; i < message::maxLogs; i++) {
    for (byte n = 0; n < message::maxCharsPerLog; n++) {
      EEPROM.put(address++, message::timeStrArray[i][n]);
    }
  }
  // compressed arrays
  for (const float& val : device::phArray) {
    EEPROM.put(address, fltToInt(val));
    address += sizeof(int);
  }
  for (const float& val : device::ecArray) {
    EEPROM.put(address, fltToInt(val));
    address += sizeof(int);
  }
  for (const float& val : device::tdsArray) {
    EEPROM.put(address, int(val));
    address += sizeof(int);
  }
  for (const float& val : device::co2Array) {
    EEPROM.put(address, int(val));
    address += sizeof(int);
  }
  for (const float& val : device::waterTemperatureArray) {
    EEPROM.put(address, fltToInt(val));
    address += sizeof(int);
  }
  for (const float& val : device::waterTemperatureArrayF) {
    EEPROM.put(address, fltToInt(val));
    address += sizeof(int);
  }
  for (const float& val : device::waterLevelArray) {
    EEPROM.put(address, int(val));
    address += sizeof(int);
  }
  for (const float& val : device::waterLevelArrayInInches) {
    EEPROM.put(address, fltToInt(val));
    address += sizeof(int);
  }
  for (const float& val : device::fanOneSpeedArray) {
    EEPROM.put(address, int(val));
    address += sizeof(int);
  }
  for (const float& val : device::fanTwoSpeedArray) {
    EEPROM.put(address, int(val));
    address += sizeof(int);
  }
  for (const float& val : device::airTemperatureArray) {
    EEPROM.put(address, fltToInt(val));
    address += sizeof(int);
  }
  for (const float& val : device::airTemperatureArrayF) {
    EEPROM.put(address, fltToInt(val));
    address += sizeof(int);
  }
  for (const float& val : device::humidityArray) {
    EEPROM.put(address, int(val));
    address += sizeof(int);
  }
  Serial.print(F("EEPROM ending at address: ")); Serial.println(address);
}

void loadSystemEEPROM() {
  int address = 8;
  Serial.print(F("Loading system settings\nEEPROM starting at address: ")); Serial.println(address);
  EEPROM.get(address, sensor::phPotStep);                 address += sizeof(sensor::phPotStep);
  EEPROM.get(address, sensor::phVoltagePerUnit);          address += sizeof(sensor::phVoltagePerUnit);
  EEPROM.get(address, sensor::ph4Voltage);                address += sizeof(sensor::ph4Voltage);
  EEPROM.get(address, sensor::ph7Voltage);                address += sizeof(sensor::ph7Voltage);
  EEPROM.get(address, sensor::ecCalibration);             address += sizeof(sensor::ecCalibration);
  EEPROM.get(address, sensor::emptyWaterTankDepth);       address += sizeof(sensor::emptyWaterTankDepth);
  EEPROM.get(address, sensor::etapeZeroVolumeResistance); address += sizeof(sensor::etapeZeroVolumeResistance);
  EEPROM.get(address, sensor::etapeMaxVolumeResistance);  address += sizeof(sensor::etapeMaxVolumeResistance);
  for (const char& val : user::profileOneName) {
    EEPROM.get(address, val);
    address += sizeof(val);
  }
  for (const char& val : user::profileTwoName) {
    EEPROM.get(address, val);
    address += sizeof(val);
  }
  for (const char& val : user::profileThreeName) {
    EEPROM.get(address, val);
    address += sizeof(val);
  }
  for (const char& val : user::profileFourName) {
    EEPROM.get(address, val);
    address += sizeof(val);
  }
  for (const char& val : user::profileFiveName) {
    EEPROM.get(address, val);
    address += sizeof(val);
  }
  for (byte i = 0; i < message::maxLogs; i++) {
    EEPROM.get(address++, message::logTypeArray[i]);
  }
  for (byte i = 0; i < message::maxLogs; i++) {
    for (byte n = 0; n < message::maxCharsPerLog; n++) {
      EEPROM.get(address++, message::timeStrArray[i][n]);
    }
  }
  // compressed arrays
  loadEepromFltArray(device::phArray,                    address);
  loadEepromFltArray(device::ecArray,                    address);
  loadEepromIntArray(device::tdsArray,                   address);
  loadEepromIntArray(device::co2Array,                   address);
  loadEepromFltArray(device::waterTemperatureArray,      address);
  loadEepromFltArray(device::waterTemperatureArrayF,     address);
  loadEepromIntArray(device::waterLevelArray,            address);
  loadEepromFltArray(device::waterLevelArrayInInches,     address);
  loadEepromIntArray(device::fanOneSpeedArray,           address);
  loadEepromIntArray(device::fanTwoSpeedArray,           address);
  loadEepromFltArray(device::airTemperatureArray,        address);
  loadEepromFltArray(device::airTemperatureArrayF,       address);
  loadEepromIntArray(device::humidityArray,              address);
  Serial.print(F("EEPROM ending at address: ")); Serial.println(address);
}

void loadUserEEPROM(const byte a_profile) {
  Serial.print(F("Loading profile: ")); Serial.println(a_profile);
  if (a_profile == 0)
    loadProfile(685);
  else if (a_profile == 1)
    loadProfile(1216); // user eeprom size per profile = 531 bytes
  else if (a_profile == 2)
    loadProfile(1747);
  else if (a_profile == 3)
    loadProfile(2278);
  else if (a_profile == 4)
    loadProfile(2809); // to 3340 "756 bytes spare of 4096"
  delay(250);
}

void saveUserEEPROM(const byte a_profile) {
  Serial.print(F("Saving profile: ")); Serial.println(a_profile);
  if (a_profile == 0)
    saveProfile(685);
  else if (a_profile == 1)
    saveProfile(1216);
  else if (a_profile == 2)
    saveProfile(1747);
  else if (a_profile == 3)
    saveProfile(2278);
  else if (a_profile == 4)
    saveProfile(2809); // to 3340 "756 bytes spare of 4096"
  delay(250);
}

void loadEepromFltArray(float* a_array, int& a_address) {
  int buffer;
  for (byte i = 0; i < device::maxGraphArrayValues; i++) {
    EEPROM.get(a_address, buffer);
    a_array[i] = buffer / 100.0;
    a_address += sizeof(int);
  }
}

void loadEepromIntArray(float* a_array, int& a_address) {
  int buffer;
  for (byte i = 0; i < device::maxGraphArrayValues; i++) {
    EEPROM.get(a_address, buffer);
    a_array[i] = buffer;
    a_address += sizeof(int);
  }
}

void loadProfile(int a_address) {
  Serial.print(F("EEPROM starting at address: ")); Serial.println(a_address);
  EEPROM.get(a_address, device::minPh);                      a_address += sizeof(device::minPh);
  EEPROM.get(a_address, device::maxPh);                      a_address += sizeof(device::maxPh);
  EEPROM.get(a_address, device::minEc);                      a_address += sizeof(device::minEc);
  EEPROM.get(a_address, device::maxEc);                      a_address += sizeof(device::maxEc);
  EEPROM.get(a_address, device::minWaterTemp);               a_address += sizeof(device::minWaterTemp);
  EEPROM.get(a_address, device::maxWaterTemp);               a_address += sizeof(device::maxWaterTemp);
  EEPROM.get(a_address, device::minWaterLevel);              a_address += sizeof(device::minWaterLevel);
  EEPROM.get(a_address, device::maxWaterLevel);              a_address += sizeof(device::maxWaterLevel);
  EEPROM.get(a_address, device::minTds);                     a_address += sizeof(device::minTds);
  EEPROM.get(a_address, device::maxTds);                     a_address += sizeof(device::maxTds);
  EEPROM.get(a_address, device::minAirTemp);                 a_address += sizeof(device::minAirTemp);
  EEPROM.get(a_address, device::maxAirTemp);                 a_address += sizeof(device::maxAirTemp);
  EEPROM.get(a_address, device::minHumidity);                a_address += sizeof(device::minHumidity);
  EEPROM.get(a_address, device::maxHumidity);                a_address += sizeof(device::maxHumidity);
  EEPROM.get(a_address, device::minCo2);                     a_address += sizeof(device::minCo2);
  EEPROM.get(a_address, device::maxCo2);                     a_address += sizeof(device::maxCo2);
  EEPROM.get(a_address, device::minFanOneSpeed);             a_address += sizeof(device::minFanOneSpeed);
  EEPROM.get(a_address, device::maxFanOneSpeed);             a_address += sizeof(device::maxFanOneSpeed);
  EEPROM.get(a_address, device::minFanTwoSpeed);             a_address += sizeof(device::minFanTwoSpeed);
  EEPROM.get(a_address, device::maxFanTwoSpeed);             a_address += sizeof(device::maxFanTwoSpeed);
  EEPROM.get(a_address, user::lightOnTimeHour);              a_address += sizeof(user::lightOnTimeHour);
  EEPROM.get(a_address, user::lightOnTimeMin);               a_address += sizeof(user::lightOnTimeMin);
  EEPROM.get(a_address, user::lightOffTimeHour);             a_address += sizeof(user::lightOffTimeHour);
  EEPROM.get(a_address, user::lightOffTimeMin);              a_address += sizeof(user::lightOffTimeMin);
  EEPROM.get(a_address, user::lightMode);                    a_address += sizeof(user::lightMode);
  EEPROM.get(a_address, user::doserOneMode);                 a_address += sizeof(user::doserOneMode);
  EEPROM.get(a_address, user::doserTwoMode);                 a_address += sizeof(user::doserTwoMode);
  EEPROM.get(a_address, user::doserThreeMode);               a_address += sizeof(user::doserThreeMode);
  EEPROM.get(a_address, user::doserFourMode);                a_address += sizeof(user::doserFourMode);
  EEPROM.get(a_address, user::doserFiveMode);                a_address += sizeof(user::doserFiveMode);
  EEPROM.get(a_address, user::doserSixMode);                 a_address += sizeof(user::doserSixMode);
  EEPROM.get(a_address, user::doserOneMills);                a_address += sizeof(user::doserOneMills);
  EEPROM.get(a_address, user::doserTwoMills);                a_address += sizeof(user::doserTwoMills);
  EEPROM.get(a_address, user::doserThreeMills);              a_address += sizeof(user::doserThreeMills);
  EEPROM.get(a_address, user::doserFourMills);               a_address += sizeof(user::doserFourMills);
  EEPROM.get(a_address, user::doserFiveMills);               a_address += sizeof(user::doserFiveMills);
  EEPROM.get(a_address, user::doserSixMills);                a_address += sizeof(user::doserSixMills);
  EEPROM.get(a_address, user::targetMinTds);                 a_address += sizeof(user::targetMinTds);
  EEPROM.get(a_address, user::targetMaxTds);                 a_address += sizeof(user::targetMaxTds);
  EEPROM.get(a_address, user::tdsOffset);                    a_address += sizeof(user::tdsOffset);
  EEPROM.get(a_address, user::targetMinPh);                  a_address += sizeof(user::targetMinPh);
  EEPROM.get(a_address, user::targetMaxPh);                  a_address += sizeof(user::targetMaxPh);
  EEPROM.get(a_address, user::phOffset);                     a_address += sizeof(user::phOffset);
  EEPROM.get(a_address, user::targetMinEc);                  a_address += sizeof(user::targetMinEc);
  EEPROM.get(a_address, user::targetMaxEc);                  a_address += sizeof(user::targetMaxEc);
  EEPROM.get(a_address, user::ecOffset);                     a_address += sizeof(user::ecOffset);
  EEPROM.get(a_address, user::targetCo2);                    a_address += sizeof(user::targetCo2);
  EEPROM.get(a_address, user::co2Offset);                    a_address += sizeof(user::co2Offset);
  EEPROM.put(a_address, user::roomWidthCm);                  a_address += sizeof(user::roomWidthCm);
  EEPROM.put(a_address, user::roomLengthCm);                 a_address += sizeof(user::roomLengthCm);
  EEPROM.put(a_address, user::roomHeightCm);                 a_address += sizeof(user::roomHeightCm);
  EEPROM.put(a_address, user::co2FlowrateLtrs);              a_address += sizeof(user::co2FlowrateLtrs);
  EEPROM.put(a_address, user::roomWidthInches);              a_address += sizeof(user::roomWidthInches);
  EEPROM.put(a_address, user::roomLengthInches);             a_address += sizeof(user::roomLengthInches);
  EEPROM.put(a_address, user::roomHeightInches);             a_address += sizeof(user::roomHeightInches);
  EEPROM.put(a_address, user::co2FlowrateFeet3);             a_address += sizeof(user::co2FlowrateFeet3);
  EEPROM.get(a_address, user::co2CheckTimeMinute);           a_address += sizeof(user::co2CheckTimeMinute);
  EEPROM.get(a_address, user::co2CheckTimeHour);             a_address += sizeof(user::co2CheckTimeHour);
  EEPROM.get(a_address, user::enableManualCo2Duration);      a_address += sizeof(user::enableManualCo2Duration);
  EEPROM.get(a_address, user::disableCo2Control);            a_address += sizeof(user::disableCo2Control);
  EEPROM.get(a_address, user::manualCo2GasDuration);         a_address += sizeof(user::manualCo2GasDuration);
  EEPROM.get(a_address, user::disableFansTimer);             a_address += sizeof(user::disableFansTimer);
  EEPROM.get(a_address, user::targetMinWaterTemp);           a_address += sizeof(user::targetMinWaterTemp);
  EEPROM.get(a_address, user::targetMaxWaterTemp);           a_address += sizeof(user::targetMaxWaterTemp);
  EEPROM.get(a_address, user::waterTempOffset);              a_address += sizeof(user::waterTempOffset);
  EEPROM.get(a_address, user::targetMinWaterHeight);         a_address += sizeof(user::targetMinWaterHeight);
  EEPROM.get(a_address, user::targetMaxWaterHeight);         a_address += sizeof(user::targetMaxWaterHeight);
  EEPROM.get(a_address, user::waterHeightOffset);            a_address += sizeof(user::waterHeightOffset);
  EEPROM.get(a_address, user::targetMinWaterHeightInches);   a_address += sizeof(user::targetMinWaterHeightInches);
  EEPROM.get(a_address, user::targetMaxWaterHeightInches);   a_address += sizeof(user::targetMaxWaterHeightInches);
  EEPROM.get(a_address, user::waterHeightOffsetInches);      a_address += sizeof(user::waterHeightOffsetInches);
  EEPROM.get(a_address, user::targetMinFanOneSpeed);         a_address += sizeof(user::targetMinFanOneSpeed);
  EEPROM.get(a_address, user::targetMaxFanOneSpeed);         a_address += sizeof(user::targetMaxFanOneSpeed);
  EEPROM.get(a_address, user::targetMinFanTwoSpeed);         a_address += sizeof(user::targetMinFanTwoSpeed);
  EEPROM.get(a_address, user::targetMaxFanTwoSpeed);         a_address += sizeof(user::targetMaxFanTwoSpeed);
  EEPROM.get(a_address, user::targetMinAirTemp);             a_address += sizeof(user::targetMinAirTemp);
  EEPROM.get(a_address, user::targetMaxAirTemp);             a_address += sizeof(user::targetMaxAirTemp);
  EEPROM.get(a_address, user::airTempOffset);                a_address += sizeof(user::airTempOffset);
  EEPROM.get(a_address, user::targetMinAirTempF);            a_address += sizeof(user::targetMinAirTempF);
  EEPROM.get(a_address, user::targetMaxAirTempF);            a_address += sizeof(user::targetMaxAirTempF);
  EEPROM.get(a_address, user::airTempOffsetF);               a_address += sizeof(user::airTempOffsetF);
  EEPROM.get(a_address, user::targetMinWaterTempF);          a_address += sizeof(user::targetMinWaterTempF);
  EEPROM.get(a_address, user::targetMaxWaterTempF);          a_address += sizeof(user::targetMaxWaterTempF);
  EEPROM.get(a_address, user::waterTempOffsetF);             a_address += sizeof(user::waterTempOffsetF);
  EEPROM.get(a_address, user::targetMinHumidity);            a_address += sizeof(user::targetMinHumidity);
  EEPROM.get(a_address, user::targetMaxHumidity);            a_address += sizeof(user::targetMaxHumidity);
  EEPROM.get(a_address, user::humidityOffset);               a_address += sizeof(user::humidityOffset);
  EEPROM.get(a_address, wifi::wifiEnabled);                  a_address += sizeof(wifi::wifiEnabled);
  EEPROM.get(a_address, user::doserOneSpeed);                a_address += sizeof(user::doserOneSpeed);
  EEPROM.get(a_address, user::doserTwoSpeed);                a_address += sizeof(user::doserTwoSpeed);
  EEPROM.get(a_address, user::doserThreeSpeed);              a_address += sizeof(user::doserThreeSpeed);
  EEPROM.get(a_address, user::doserFourSpeed);               a_address += sizeof(user::doserFourSpeed);
  EEPROM.get(a_address, user::doserFiveSpeed);               a_address += sizeof(user::doserFiveSpeed);
  EEPROM.get(a_address, user::doserSixSpeed);                a_address += sizeof(user::doserSixSpeed);
  EEPROM.get(a_address, user::dosingInterval);               a_address += sizeof(user::dosingInterval);
  EEPROM.get(a_address, user::swapInterval);                 a_address += sizeof(user::swapInterval);
  EEPROM.get(a_address, user::refillDoserOneMills);          a_address += sizeof(user::refillDoserOneMills);
  EEPROM.get(a_address, user::refillDoserTwoMills);          a_address += sizeof(user::refillDoserTwoMills);
  EEPROM.get(a_address, user::refillDoserThreeMills);        a_address += sizeof(user::refillDoserThreeMills);
  EEPROM.get(a_address, user::refillDoserFourMills);         a_address += sizeof(user::refillDoserFourMills);
  EEPROM.get(a_address, user::refillDoserFiveMills);         a_address += sizeof(user::refillDoserFiveMills);
  EEPROM.get(a_address, user::refillDoserSixMills);          a_address += sizeof(user::refillDoserSixMills);
  EEPROM.get(a_address, user::disableDrainAndRefill);        a_address += sizeof(user::disableDrainAndRefill);
  EEPROM.get(a_address, user::autoFillHour);                 a_address += sizeof(user::autoFillHour);
  EEPROM.get(a_address, user::autoFillMinute);               a_address += sizeof(user::autoFillMinute);
  EEPROM.get(a_address, user::drainTimeout);                 a_address += sizeof(user::drainTimeout);
  EEPROM.get(a_address, user::backgroundColor);              a_address += sizeof(user::backgroundColor);
  EEPROM.get(a_address, user::graphInterval);                a_address += sizeof(user::graphInterval);
  EEPROM.get(a_address, user::afkTime);                      a_address += sizeof(user::afkTime);
  EEPROM.get(a_address, user::convertToFarenheit);           a_address += sizeof(user::convertToFarenheit);
  EEPROM.get(a_address, user::convertToInches);              a_address += sizeof(user::convertToInches);
  EEPROM.get(a_address, user::convertToTds);                 a_address += sizeof(user::convertToTds);
  EEPROM.get(a_address, user::tdsErrorMargin);               a_address += sizeof(user::tdsErrorMargin);
  EEPROM.get(a_address, user::phErrorMargin);                a_address += sizeof(user::phErrorMargin);
  EEPROM.get(a_address, user::ecErrorMargin);                a_address += sizeof(user::ecErrorMargin);
  EEPROM.get(a_address, user::co2ErrorMargin);               a_address += sizeof(user::co2ErrorMargin);
  EEPROM.get(a_address, user::airTempErrorMargin);           a_address += sizeof(user::airTempErrorMargin);
  EEPROM.get(a_address, user::airTempErrorMarginF);          a_address += sizeof(user::airTempErrorMarginF);
  EEPROM.get(a_address, user::humidityErrorMargin);          a_address += sizeof(user::humidityErrorMargin);
  EEPROM.get(a_address, user::waterTempErrorMargin);         a_address += sizeof(user::waterTempErrorMargin);
  EEPROM.get(a_address, user::waterTempErrorMarginF);        a_address += sizeof(user::waterTempErrorMarginF);
  EEPROM.get(a_address, user::waterHeightErrorMargin);       a_address += sizeof(user::waterHeightErrorMargin);
  EEPROM.get(a_address, user::waterHeightErrorMarginInches); a_address += sizeof(user::waterHeightErrorMarginInches);
  EEPROM.get(a_address, user::disableAirTempWarnings);       a_address += sizeof(user::disableAirTempWarnings);
  EEPROM.get(a_address, user::disableHumidityWarnings);      a_address += sizeof(user::disableHumidityWarnings);
  EEPROM.get(a_address, user::disablePpmWarnings);           a_address += sizeof(user::disablePpmWarnings);
  EEPROM.get(a_address, user::disableEcWarnings);            a_address += sizeof(user::disableEcWarnings);
  EEPROM.get(a_address, user::disablePhWarnings);            a_address += sizeof(user::disablePhWarnings);
  EEPROM.get(a_address, user::disableCo2Warnings);           a_address += sizeof(user::disableCo2Warnings);
  EEPROM.get(a_address, user::disableWaterTempWarnings);     a_address += sizeof(user::disableWaterTempWarnings);
  EEPROM.get(a_address, user::disableWaterHeightWarnings);   a_address += sizeof(user::disableWaterHeightWarnings);
  EEPROM.get(a_address, user::disableLED);                   a_address += sizeof(user::disableLED);
  EEPROM.get(a_address, user::disableBeeper);                a_address += sizeof(user::disableBeeper);
  EEPROM.get(a_address, user::disableMachineLearning);       a_address += sizeof(user::disableMachineLearning);
  EEPROM.get(a_address, user::fansControlTemperature);       a_address += sizeof(user::fansControlTemperature);
  EEPROM.get(a_address, user::fansControlHumidity);          a_address += sizeof(user::fansControlHumidity);
  EEPROM.get(a_address, user::fanOneFixedSpeed);             a_address += sizeof(user::fanOneFixedSpeed);
  EEPROM.get(a_address, user::fanTwoFixedSpeed);             a_address += sizeof(user::fanTwoFixedSpeed);
  EEPROM.get(a_address, user::numberOfDosers);               a_address += sizeof(user::numberOfDosers);
  EEPROM.get(a_address, user::useEtapeSensor);               a_address += sizeof(user::useEtapeSensor);
  EEPROM.get(a_address, device::graphArrayPos);              a_address += sizeof(device::graphArrayPos);
  EEPROM.get(a_address, device::conversionType);             a_address += sizeof(device::conversionType);
  EEPROM.get(a_address, machineLearning::phDownArrayBlock);  a_address += sizeof(machineLearning::phDownArrayBlock);
  EEPROM.get(a_address, machineLearning::phUpArrayBlock);    a_address += sizeof(machineLearning::phUpArrayBlock);
  EEPROM.get(a_address, machineLearning::ecArrayBlock);      a_address += sizeof(machineLearning::ecArrayBlock);
  EEPROM.get(a_address, machineLearning::tdsArrayBlock);     a_address += sizeof(machineLearning::tdsArrayBlock);
  // compressed arrays but retrive normally
  for (const unsigned int& val : machineLearning::phDownDosingInc) {
    EEPROM.get(a_address, val);
    a_address += sizeof(val);
  }
  for (const unsigned int& val : machineLearning::phUpDosingInc) {
    EEPROM.get(a_address, val);
    a_address += sizeof(val);
  }
  for (const unsigned int& val : machineLearning::ecDosingInc) {
    EEPROM.get(a_address, val);
    a_address += sizeof(val);
  }
  for (const unsigned int& val : machineLearning::tdsDosingInc) {
    EEPROM.get(a_address, val);
    a_address += sizeof(val);
  }
  for (const bool& val : user::autoFillDays) {
    EEPROM.get(a_address, val);
    a_address += sizeof(val);
  }
  Serial.print(F("EEPROM ending at address: ")); Serial.println(a_address);
}

void saveProfile(int a_address) {
  Serial.print(F("EEPROM starting at address: ")); Serial.println(a_address);
  EEPROM.put(a_address, device::minPh);                      a_address += sizeof(device::minPh);
  EEPROM.put(a_address, device::maxPh);                      a_address += sizeof(device::maxPh);
  EEPROM.put(a_address, device::minEc);                      a_address += sizeof(device::minEc);
  EEPROM.put(a_address, device::maxEc);                      a_address += sizeof(device::maxEc);
  EEPROM.put(a_address, device::minWaterTemp);               a_address += sizeof(device::minWaterTemp);
  EEPROM.put(a_address, device::maxWaterTemp);               a_address += sizeof(device::maxWaterTemp);
  EEPROM.put(a_address, device::minWaterLevel);              a_address += sizeof(device::minWaterLevel);
  EEPROM.put(a_address, device::maxWaterLevel);              a_address += sizeof(device::maxWaterLevel);
  EEPROM.put(a_address, device::minTds);                     a_address += sizeof(device::minTds);
  EEPROM.put(a_address, device::maxTds);                     a_address += sizeof(device::maxTds);
  EEPROM.put(a_address, device::minAirTemp);                 a_address += sizeof(device::minAirTemp);
  EEPROM.put(a_address, device::maxAirTemp);                 a_address += sizeof(device::maxAirTemp);
  EEPROM.put(a_address, device::minHumidity);                a_address += sizeof(device::minHumidity);
  EEPROM.put(a_address, device::maxHumidity);                a_address += sizeof(device::maxHumidity);
  EEPROM.put(a_address, device::minCo2);                     a_address += sizeof(device::minCo2);
  EEPROM.put(a_address, device::maxCo2);                     a_address += sizeof(device::maxCo2);
  EEPROM.put(a_address, device::minFanOneSpeed);             a_address += sizeof(device::minFanOneSpeed);
  EEPROM.put(a_address, device::maxFanOneSpeed);             a_address += sizeof(device::maxFanOneSpeed);
  EEPROM.put(a_address, device::minFanTwoSpeed);             a_address += sizeof(device::minFanTwoSpeed);
  EEPROM.put(a_address, device::maxFanTwoSpeed);             a_address += sizeof(device::maxFanTwoSpeed);
  EEPROM.put(a_address, user::lightOnTimeHour);              a_address += sizeof(user::lightOnTimeHour);
  EEPROM.put(a_address, user::lightOnTimeMin);               a_address += sizeof(user::lightOnTimeMin);
  EEPROM.put(a_address, user::lightOffTimeHour);             a_address += sizeof(user::lightOffTimeHour);
  EEPROM.put(a_address, user::lightOffTimeMin);              a_address += sizeof(user::lightOffTimeMin);
  EEPROM.put(a_address, user::lightMode);                    a_address += sizeof(user::lightMode);
  EEPROM.put(a_address, user::doserOneMode);                 a_address += sizeof(user::doserOneMode);
  EEPROM.put(a_address, user::doserTwoMode);                 a_address += sizeof(user::doserTwoMode);
  EEPROM.put(a_address, user::doserThreeMode);               a_address += sizeof(user::doserThreeMode);
  EEPROM.put(a_address, user::doserFourMode);                a_address += sizeof(user::doserFourMode);
  EEPROM.put(a_address, user::doserFiveMode);                a_address += sizeof(user::doserFiveMode);
  EEPROM.put(a_address, user::doserSixMode);                 a_address += sizeof(user::doserSixMode);
  EEPROM.put(a_address, user::doserOneMills);                a_address += sizeof(user::doserOneMills);
  EEPROM.put(a_address, user::doserTwoMills);                a_address += sizeof(user::doserTwoMills);
  EEPROM.put(a_address, user::doserThreeMills);              a_address += sizeof(user::doserThreeMills);
  EEPROM.put(a_address, user::doserFourMills);               a_address += sizeof(user::doserFourMills);
  EEPROM.put(a_address, user::doserFiveMills);               a_address += sizeof(user::doserFiveMills);
  EEPROM.put(a_address, user::doserSixMills);                a_address += sizeof(user::doserSixMills);
  EEPROM.put(a_address, user::targetMinTds);                 a_address += sizeof(user::targetMinTds);
  EEPROM.put(a_address, user::targetMaxTds);                 a_address += sizeof(user::targetMaxTds);
  EEPROM.put(a_address, user::tdsOffset);                    a_address += sizeof(user::tdsOffset);
  EEPROM.put(a_address, user::targetMinPh);                  a_address += sizeof(user::targetMinPh);
  EEPROM.put(a_address, user::targetMaxPh);                  a_address += sizeof(user::targetMaxPh);
  EEPROM.put(a_address, user::phOffset);                     a_address += sizeof(user::phOffset);
  EEPROM.put(a_address, user::targetMinEc);                  a_address += sizeof(user::targetMinEc);
  EEPROM.put(a_address, user::targetMaxEc);                  a_address += sizeof(user::targetMaxEc);
  EEPROM.put(a_address, user::ecOffset);                     a_address += sizeof(user::ecOffset);
  EEPROM.put(a_address, user::targetCo2);                    a_address += sizeof(user::targetCo2);
  EEPROM.put(a_address, user::co2Offset);                    a_address += sizeof(user::co2Offset);
  EEPROM.put(a_address, user::roomWidthCm);                  a_address += sizeof(user::roomWidthCm);
  EEPROM.put(a_address, user::roomLengthCm);                 a_address += sizeof(user::roomLengthCm);
  EEPROM.put(a_address, user::roomHeightCm);                 a_address += sizeof(user::roomHeightCm);
  EEPROM.put(a_address, user::co2FlowrateLtrs);              a_address += sizeof(user::co2FlowrateLtrs);
  EEPROM.put(a_address, user::roomWidthInches);              a_address += sizeof(user::roomWidthInches);
  EEPROM.put(a_address, user::roomLengthInches);             a_address += sizeof(user::roomLengthInches);
  EEPROM.put(a_address, user::roomHeightInches);             a_address += sizeof(user::roomHeightInches);
  EEPROM.put(a_address, user::co2FlowrateFeet3);             a_address += sizeof(user::co2FlowrateFeet3);
  EEPROM.put(a_address, user::co2CheckTimeMinute);           a_address += sizeof(user::co2CheckTimeMinute);
  EEPROM.put(a_address, user::co2CheckTimeHour);             a_address += sizeof(user::co2CheckTimeHour);
  EEPROM.put(a_address, user::enableManualCo2Duration);      a_address += sizeof(user::enableManualCo2Duration);
  EEPROM.put(a_address, user::disableCo2Control);            a_address += sizeof(user::disableCo2Control);
  EEPROM.put(a_address, user::manualCo2GasDuration);         a_address += sizeof(user::manualCo2GasDuration);
  EEPROM.put(a_address, user::disableFansTimer);             a_address += sizeof(user::disableFansTimer);
  EEPROM.put(a_address, user::targetMinWaterTemp);           a_address += sizeof(user::targetMinWaterTemp);
  EEPROM.put(a_address, user::targetMaxWaterTemp);           a_address += sizeof(user::targetMaxWaterTemp);
  EEPROM.put(a_address, user::waterTempOffset);              a_address += sizeof(user::waterTempOffset);
  EEPROM.put(a_address, user::targetMinWaterHeight);         a_address += sizeof(user::targetMinWaterHeight);
  EEPROM.put(a_address, user::targetMaxWaterHeight);         a_address += sizeof(user::targetMaxWaterHeight);
  EEPROM.put(a_address, user::waterHeightOffset);            a_address += sizeof(user::waterHeightOffset);
  EEPROM.put(a_address, user::targetMinWaterHeightInches);   a_address += sizeof(user::targetMinWaterHeightInches);
  EEPROM.put(a_address, user::targetMaxWaterHeightInches);   a_address += sizeof(user::targetMaxWaterHeightInches);
  EEPROM.put(a_address, user::waterHeightOffsetInches);      a_address += sizeof(user::waterHeightOffsetInches);
  EEPROM.put(a_address, user::targetMinFanOneSpeed);         a_address += sizeof(user::targetMinFanOneSpeed);
  EEPROM.put(a_address, user::targetMaxFanOneSpeed);         a_address += sizeof(user::targetMaxFanOneSpeed);
  EEPROM.put(a_address, user::targetMinFanTwoSpeed);         a_address += sizeof(user::targetMinFanTwoSpeed);
  EEPROM.put(a_address, user::targetMaxFanTwoSpeed);         a_address += sizeof(user::targetMaxFanTwoSpeed);
  EEPROM.put(a_address, user::targetMinAirTemp);             a_address += sizeof(user::targetMinAirTemp);
  EEPROM.put(a_address, user::targetMaxAirTemp);             a_address += sizeof(user::targetMaxAirTemp);
  EEPROM.put(a_address, user::airTempOffset);                a_address += sizeof(user::airTempOffset);
  EEPROM.put(a_address, user::targetMinAirTempF);            a_address += sizeof(user::targetMinAirTempF);
  EEPROM.put(a_address, user::targetMaxAirTempF);            a_address += sizeof(user::targetMaxAirTempF);
  EEPROM.put(a_address, user::airTempOffsetF);               a_address += sizeof(user::airTempOffsetF);
  EEPROM.put(a_address, user::targetMinWaterTempF);          a_address += sizeof(user::targetMinWaterTempF);
  EEPROM.put(a_address, user::targetMaxWaterTempF);          a_address += sizeof(user::targetMaxWaterTempF);
  EEPROM.put(a_address, user::waterTempOffsetF);             a_address += sizeof(user::waterTempOffsetF);
  EEPROM.put(a_address, user::targetMinHumidity);            a_address += sizeof(user::targetMinHumidity);
  EEPROM.put(a_address, user::targetMaxHumidity);            a_address += sizeof(user::targetMaxHumidity);
  EEPROM.put(a_address, user::humidityOffset);               a_address += sizeof(user::humidityOffset);
  EEPROM.put(a_address, wifi::wifiEnabled);                  a_address += sizeof(wifi::wifiEnabled);
  EEPROM.put(a_address, user::doserOneSpeed);                a_address += sizeof(user::doserOneSpeed);
  EEPROM.put(a_address, user::doserTwoSpeed);                a_address += sizeof(user::doserTwoSpeed);
  EEPROM.put(a_address, user::doserThreeSpeed);              a_address += sizeof(user::doserThreeSpeed);
  EEPROM.put(a_address, user::doserFourSpeed);               a_address += sizeof(user::doserFourSpeed);
  EEPROM.put(a_address, user::doserFiveSpeed);               a_address += sizeof(user::doserFiveSpeed);
  EEPROM.put(a_address, user::doserSixSpeed);                a_address += sizeof(user::doserSixSpeed);
  EEPROM.put(a_address, user::dosingInterval);               a_address += sizeof(user::dosingInterval);
  EEPROM.put(a_address, user::swapInterval);                 a_address += sizeof(user::swapInterval);
  EEPROM.put(a_address, user::refillDoserOneMills);          a_address += sizeof(user::refillDoserOneMills);
  EEPROM.put(a_address, user::refillDoserTwoMills);          a_address += sizeof(user::refillDoserTwoMills);
  EEPROM.put(a_address, user::refillDoserThreeMills);        a_address += sizeof(user::refillDoserThreeMills);
  EEPROM.put(a_address, user::refillDoserFourMills);         a_address += sizeof(user::refillDoserFourMills);
  EEPROM.put(a_address, user::refillDoserFiveMills);         a_address += sizeof(user::refillDoserFiveMills);
  EEPROM.put(a_address, user::refillDoserSixMills);          a_address += sizeof(user::refillDoserSixMills);
  EEPROM.put(a_address, user::disableDrainAndRefill);        a_address += sizeof(user::disableDrainAndRefill);
  EEPROM.put(a_address, user::autoFillHour);                 a_address += sizeof(user::autoFillHour);
  EEPROM.put(a_address, user::autoFillMinute);               a_address += sizeof(user::autoFillMinute);
  EEPROM.put(a_address, user::drainTimeout);                 a_address += sizeof(user::drainTimeout);
  EEPROM.put(a_address, user::backgroundColor);              a_address += sizeof(user::backgroundColor);
  EEPROM.put(a_address, user::graphInterval);                a_address += sizeof(user::graphInterval);
  EEPROM.put(a_address, user::afkTime);                      a_address += sizeof(user::afkTime);
  EEPROM.put(a_address, user::convertToFarenheit);           a_address += sizeof(user::convertToFarenheit);
  EEPROM.put(a_address, user::convertToInches);              a_address += sizeof(user::convertToInches);
  EEPROM.put(a_address, user::convertToTds);                 a_address += sizeof(user::convertToTds);
  EEPROM.put(a_address, user::tdsErrorMargin);               a_address += sizeof(user::tdsErrorMargin);
  EEPROM.put(a_address, user::phErrorMargin);                a_address += sizeof(user::phErrorMargin);
  EEPROM.put(a_address, user::ecErrorMargin);                a_address += sizeof(user::ecErrorMargin);
  EEPROM.put(a_address, user::co2ErrorMargin);               a_address += sizeof(user::co2ErrorMargin);
  EEPROM.put(a_address, user::airTempErrorMargin);           a_address += sizeof(user::airTempErrorMargin);
  EEPROM.put(a_address, user::airTempErrorMarginF);          a_address += sizeof(user::airTempErrorMarginF);
  EEPROM.put(a_address, user::humidityErrorMargin);          a_address += sizeof(user::humidityErrorMargin);
  EEPROM.put(a_address, user::waterTempErrorMargin);         a_address += sizeof(user::waterTempErrorMargin);
  EEPROM.put(a_address, user::waterTempErrorMarginF);        a_address += sizeof(user::waterTempErrorMarginF);
  EEPROM.put(a_address, user::waterHeightErrorMargin);       a_address += sizeof(user::waterHeightErrorMargin);
  EEPROM.put(a_address, user::waterHeightErrorMarginInches); a_address += sizeof(user::waterHeightErrorMarginInches);
  EEPROM.put(a_address, user::disableAirTempWarnings);       a_address += sizeof(user::disableAirTempWarnings);
  EEPROM.put(a_address, user::disableHumidityWarnings);      a_address += sizeof(user::disableHumidityWarnings);
  EEPROM.put(a_address, user::disablePpmWarnings);           a_address += sizeof(user::disablePpmWarnings);
  EEPROM.put(a_address, user::disableEcWarnings);            a_address += sizeof(user::disableEcWarnings);
  EEPROM.put(a_address, user::disablePhWarnings);            a_address += sizeof(user::disablePhWarnings);
  EEPROM.put(a_address, user::disableCo2Warnings);           a_address += sizeof(user::disableCo2Warnings);
  EEPROM.put(a_address, user::disableWaterTempWarnings);     a_address += sizeof(user::disableWaterTempWarnings);
  EEPROM.put(a_address, user::disableWaterHeightWarnings);   a_address += sizeof(user::disableWaterHeightWarnings);
  EEPROM.put(a_address, user::disableLED);                   a_address += sizeof(user::disableLED);
  EEPROM.put(a_address, user::disableBeeper);                a_address += sizeof(user::disableBeeper);
  EEPROM.put(a_address, user::disableMachineLearning);       a_address += sizeof(user::disableMachineLearning);
  EEPROM.put(a_address, user::fansControlTemperature);       a_address += sizeof(user::fansControlTemperature);
  EEPROM.put(a_address, user::fansControlHumidity);          a_address += sizeof(user::fansControlHumidity);
  EEPROM.put(a_address, user::fanOneFixedSpeed);             a_address += sizeof(user::fanOneFixedSpeed);
  EEPROM.put(a_address, user::fanTwoFixedSpeed);             a_address += sizeof(user::fanTwoFixedSpeed);
  EEPROM.put(a_address, user::numberOfDosers);               a_address += sizeof(user::numberOfDosers);
  EEPROM.put(a_address, user::useEtapeSensor);               a_address += sizeof(user::useEtapeSensor);
  EEPROM.put(a_address, device::graphArrayPos);              a_address += sizeof(device::graphArrayPos);
  EEPROM.put(a_address, device::conversionType);             a_address += sizeof(device::conversionType);
  EEPROM.put(a_address, machineLearning::phDownArrayBlock);  a_address += sizeof(machineLearning::phDownArrayBlock);
  EEPROM.put(a_address, machineLearning::phUpArrayBlock);    a_address += sizeof(machineLearning::phUpArrayBlock);
  EEPROM.put(a_address, machineLearning::ecArrayBlock);      a_address += sizeof(machineLearning::ecArrayBlock);
  EEPROM.put(a_address, machineLearning::tdsArrayBlock);     a_address += sizeof(machineLearning::tdsArrayBlock);
  // compressed arrays but store normally
  for (const unsigned int& val : machineLearning::phDownDosingInc) {
    EEPROM.put(a_address, val);
    a_address += sizeof(val);
  }
  for (const unsigned int& val : machineLearning::phUpDosingInc) {
    EEPROM.put(a_address, val);
    a_address += sizeof(val);
  }
  for (const unsigned int& val : machineLearning::ecDosingInc) {
    EEPROM.put(a_address, val);
    a_address += sizeof(val);
  }
  for (const unsigned int& val : machineLearning::tdsDosingInc) {
    EEPROM.put(a_address, val);
    a_address += sizeof(val);
  }
  for (const bool& val : user::autoFillDays) {
    EEPROM.put(a_address, val);
    a_address += sizeof(val);
  }
  Serial.print(F("EEPROM ending at address: ")); Serial.println(a_address);
}
