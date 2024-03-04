void initializeEEPROM() {
  char versionNum[6] {'\0'};
  for (uint8_t i = 0; i < 5; i++) {
    versionNum[i] = EEPROM.read(i);
    delay(5);
  }
  device::systemEEPROMSize = getSetSystemEEPROM(device::EEPROM_VERIFY);
  delay(50);
  device::profileEEPROMSize = getSetProfile(0, device::EEPROM_VERIFY);
  delay(50);
  if (device::globalDebug) {
    Serial.println(__FUNCTION__);
    Serial.print(F("Current version number: ")); Serial.println(versionNum);
    Serial.print(F("Software version number: ")); Serial.println(device::versionNumber);
    Serial.print(F("System EEPROM size: ")); Serial.println(device::systemEEPROMSize);
    Serial.print(F("Profile EEPROM size: ")); Serial.println(device::profileEEPROMSize);
  }
  delay(50);
  if (strcmp(versionNum, device::versionNumber) == 0) {
    device::userProfile = EEPROM.read(6);
    getSetSystemEEPROM(device::EEPROM_GET);
    delay(50);
    getSetProfileEEPROM(device::userProfile, device::EEPROM_GET);
  }
  else {
    //showResetMessage();
    for (uint8_t i = 0; i < 6; i++) {
      EEPROM.put(i, device::versionNumber[i]);
      delay(5);
    }
    EEPROM.put(6, device::userProfile);
    delay(5);
    getSetSystemEEPROM(device::EEPROM_SET);
    delay(50);
    for (uint8_t i = 0; i < 5; i++) {
      getSetProfileEEPROM(i, device::EEPROM_SET);
      delay(50);
    }
    //clearPage();
  }
}

template <typename T> void eepromGetSet(uint16_t& a_address, const device::eepromModes a_mode, T &t) {
  if (a_mode == device::EEPROM_GET)
    EEPROM.get(a_address, t);
  else if (a_mode == device::EEPROM_SET)
    EEPROM.put(a_address, t);
  a_address += sizeof(T);
}

uint16_t getSetSystemEEPROM(const device::eepromModes a_mode) {
  uint16_t address = 7;
  if (device::globalDebug) {
    Serial.println(__FUNCTION__);
    if (a_mode == device::EEPROM_VERIFY)
      Serial.print(F("Verifying")); 
    else if (a_mode == device::EEPROM_GET)
      Serial.print(F("Loading")); 
    else
      Serial.print(F("Saving")); 
    Serial.print(F(" system settings\nEEPROM starting at address: ")); Serial.println(address);
  }
  eepromGetSet(address, a_mode, sensor::emptyWaterTankDepth);
  eepromGetSet(address, a_mode, sensor::etapeZeroVolumeResistance);
  eepromGetSet(address, a_mode, sensor::etapeMaxVolumeResistance);
  eepromGetSet(address, a_mode, sensor::tdsKvalueLow);
  eepromGetSet(address, a_mode, sensor::tdsKvalueHigh);
  eepromGetSet(address, a_mode, sensor::phNeutralVoltage);
  eepromGetSet(address, a_mode, sensor::phAcidicVoltage);
  eepromGetSet(address, a_mode, device::calMinX);
  eepromGetSet(address, a_mode, device::calMaxX);
  eepromGetSet(address, a_mode, device::calMinY);
  eepromGetSet(address, a_mode, device::calMaxY);
  //
  for (char& val : user::profileOneName) {
    eepromGetSet(address, a_mode, val);
  }
  for (char& val : user::profileTwoName) {
    eepromGetSet(address, a_mode, val);
  }
  for (char& val : user::profileThreeName) {
    eepromGetSet(address, a_mode, val);
  }
  for (char& val : user::profileFourName) {
    eepromGetSet(address, a_mode, val);
  }
  for (char& val : user::profileFiveName) {
    eepromGetSet(address, a_mode, val);
  }
  for (uint8_t i = 0; i < message::maxLogs; i++) {
    eepromGetSet(address, a_mode, message::logTypeArray[i]);
  }
  for (uint8_t i = 0; i < message::maxLogs; i++) {
    for (uint8_t n = 0; n < message::maxCharsPerLog; n++) {
      eepromGetSet(address, a_mode, message::timeStrArray[i][n]);
    }
  }
  for (uint16_t& val : sensor::phArray) {
    eepromGetSet(address, a_mode, val);
  }
  for (uint16_t& val : sensor::ecArray) {
    eepromGetSet(address, a_mode, val);
  }
  for (uint16_t& val : sensor::tdsArray) {
    eepromGetSet(address, a_mode, val);
  }
  for (uint16_t& val : sensor::co2Array) {
    eepromGetSet(address, a_mode, val);
  }
  for (uint16_t& val : sensor::waterTemperatureArray) {
    eepromGetSet(address, a_mode, val);
  }
  for (uint16_t& val : sensor::waterTemperatureArrayF) {
    eepromGetSet(address, a_mode, val);
  }
  for (uint16_t& val : sensor::waterLevelArray) {
    eepromGetSet(address, a_mode, val);
  }
  for (uint16_t& val : sensor::waterLevelArrayInInches) {
    eepromGetSet(address, a_mode, val);
  }
  for (uint16_t& val : sensor::fanOneSpeedArray) {
    eepromGetSet(address, a_mode, val);
  }
  for (uint16_t& val : sensor::fanTwoSpeedArray) {
    eepromGetSet(address, a_mode, val);
  }
  for (uint16_t& val : sensor::airTemperatureArray) {
    eepromGetSet(address, a_mode, val);
  }
  for (uint16_t& val : sensor::airTemperatureArrayF) {
    eepromGetSet(address, a_mode, val);
  }
  for (uint16_t& val : sensor::humidityArray) {
    eepromGetSet(address, a_mode, val);
  }
  if (device::globalDebug) {
    Serial.print(F("EEPROM ending at address: ")); Serial.println(address);
  }
  return address;
}

void getSetProfileEEPROM(const uint8_t a_profile, const device::eepromModes a_mode) {
  if (device::globalDebug) {
    Serial.println(__FUNCTION__);
    if (a_mode == device::EEPROM_VERIFY)
      Serial.print(F("Verifying")); 
    else if (a_mode == device::EEPROM_GET)
      Serial.print(F("Loading")); 
    else
      Serial.print(F("Saving")); 
    Serial.print(F(" profile: ")); Serial.println(a_profile);
  }
  uint16_t address = device::systemEEPROMSize + (a_profile * device::profileEEPROMSize); 
  getSetProfile(address, a_mode);
  delay(250);
}

uint16_t getSetProfile(uint16_t a_address, const device::eepromModes a_mode) {
  if (device::globalDebug) {
    Serial.println(__FUNCTION__);
    Serial.print(F("EEPROM starting at address: ")); Serial.println(a_address);
  }
  eepromGetSet(a_address, a_mode, device::minPh);                      
  eepromGetSet(a_address, a_mode, device::maxPh);                      
  eepromGetSet(a_address, a_mode, device::minEc);                      
  eepromGetSet(a_address, a_mode, device::maxEc);                      
  eepromGetSet(a_address, a_mode, device::minWaterTemp);               
  eepromGetSet(a_address, a_mode, device::maxWaterTemp);               
  eepromGetSet(a_address, a_mode, device::minWaterLevel);              
  eepromGetSet(a_address, a_mode, device::maxWaterLevel);              
  eepromGetSet(a_address, a_mode, device::minTds);                     
  eepromGetSet(a_address, a_mode, device::maxTds);                     
  eepromGetSet(a_address, a_mode, device::minAirTemp);                 
  eepromGetSet(a_address, a_mode, device::maxAirTemp);                 
  eepromGetSet(a_address, a_mode, device::minHumidity);                
  eepromGetSet(a_address, a_mode, device::maxHumidity);                
  eepromGetSet(a_address, a_mode, device::minCo2);                     
  eepromGetSet(a_address, a_mode, device::maxCo2);                     
  eepromGetSet(a_address, a_mode, device::minFanOneSpeed);             
  eepromGetSet(a_address, a_mode, device::maxFanOneSpeed);             
  eepromGetSet(a_address, a_mode, device::minFanTwoSpeed);             
  eepromGetSet(a_address, a_mode, device::maxFanTwoSpeed); 

  eepromGetSet(a_address, a_mode, user::lightOnTimeHour);              
  eepromGetSet(a_address, a_mode, user::lightOnTimeMin); 
  eepromGetSet(a_address, a_mode, user::lightOffTimeHour);              
  eepromGetSet(a_address, a_mode, user::lightOffTimeMin);               
  eepromGetSet(a_address, a_mode, user::lightOffDuration);
  eepromGetSet(a_address, a_mode, user::lightOnDuration);          
  eepromGetSet(a_address, a_mode, user::lightState); 
  eepromGetSet(a_address, a_mode, user::lightMode); 

  eepromGetSet(a_address, a_mode, user::auxRelayOneOnTimeHour);              
  eepromGetSet(a_address, a_mode, user::auxRelayOneOnTimeMin);   
  eepromGetSet(a_address, a_mode, user::auxRelayOneOffTimeHour);              
  eepromGetSet(a_address, a_mode, user::auxRelayOneOffTimeMin);             
  eepromGetSet(a_address, a_mode, user::auxRelayOneOffDuration);
  eepromGetSet(a_address, a_mode, user::auxRelayOneOnDuration);          
  eepromGetSet(a_address, a_mode, user::auxRelayOneState); 
  eepromGetSet(a_address, a_mode, user::auxRelayOneMode); 

  eepromGetSet(a_address, a_mode, user::auxRelayTwoOnTimeHour);              
  eepromGetSet(a_address, a_mode, user::auxRelayTwoOnTimeMin);      
  eepromGetSet(a_address, a_mode, user::auxRelayTwoOffTimeHour);              
  eepromGetSet(a_address, a_mode, user::auxRelayTwoOffTimeMin);           
  eepromGetSet(a_address, a_mode, user::auxRelayTwoOffDuration);
  eepromGetSet(a_address, a_mode, user::auxRelayTwoOnDuration);          
  eepromGetSet(a_address, a_mode, user::auxRelayTwoState); 
  eepromGetSet(a_address, a_mode, user::auxRelayTwoMode); 

  eepromGetSet(a_address, a_mode, user::doserOneMode);                 
  eepromGetSet(a_address, a_mode, user::doserTwoMode);                 
  eepromGetSet(a_address, a_mode, user::doserThreeMode);               
  eepromGetSet(a_address, a_mode, user::doserFourMode);                
  eepromGetSet(a_address, a_mode, user::doserFiveMode);                
  eepromGetSet(a_address, a_mode, user::doserSixMode);                 
  eepromGetSet(a_address, a_mode, user::doserOneMills);                
  eepromGetSet(a_address, a_mode, user::doserTwoMills);                
  eepromGetSet(a_address, a_mode, user::doserThreeMills);              
  eepromGetSet(a_address, a_mode, user::doserFourMills);               
  eepromGetSet(a_address, a_mode, user::doserFiveMills);               
  eepromGetSet(a_address, a_mode, user::doserSixMills);                
  eepromGetSet(a_address, a_mode, user::targetMinTds);                 
  eepromGetSet(a_address, a_mode, user::targetMaxTds);                 
  eepromGetSet(a_address, a_mode, user::targetMinPh);                  
  eepromGetSet(a_address, a_mode, user::targetMaxPh);                  
  eepromGetSet(a_address, a_mode, user::targetMinEc);                  
  eepromGetSet(a_address, a_mode, user::targetMaxEc);                  
  eepromGetSet(a_address, a_mode, user::targetCo2);                    
  eepromGetSet(a_address, a_mode, user::co2Offset);                    
  eepromGetSet(a_address, a_mode, user::roomWidthCm);                  
  eepromGetSet(a_address, a_mode, user::roomLengthCm);                 
  eepromGetSet(a_address, a_mode, user::roomHeightCm);                 
  eepromGetSet(a_address, a_mode, user::co2FlowrateLtrs);              
  eepromGetSet(a_address, a_mode, user::roomWidthInches);              
  eepromGetSet(a_address, a_mode, user::roomLengthInches);             
  eepromGetSet(a_address, a_mode, user::roomHeightInches);             
  eepromGetSet(a_address, a_mode, user::co2FlowrateFeet3);             
  eepromGetSet(a_address, a_mode, user::co2CheckTimeMinute);           
  eepromGetSet(a_address, a_mode, user::co2CheckTimeHour);             
  eepromGetSet(a_address, a_mode, user::enableManualCo2Duration);      
  eepromGetSet(a_address, a_mode, user::disableCo2Control);            
  eepromGetSet(a_address, a_mode, user::manualCo2GasDuration);         
  eepromGetSet(a_address, a_mode, user::disableFansTimer);             
  eepromGetSet(a_address, a_mode, user::targetMinWaterTemp);           
  eepromGetSet(a_address, a_mode, user::targetMaxWaterTemp);           
  eepromGetSet(a_address, a_mode, user::targetMinWaterHeight);         
  eepromGetSet(a_address, a_mode, user::targetMaxWaterHeight);         
  eepromGetSet(a_address, a_mode, user::targetMinWaterHeightInches);   
  eepromGetSet(a_address, a_mode, user::targetMaxWaterHeightInches);   
  eepromGetSet(a_address, a_mode, user::waterTankLength);              
  eepromGetSet(a_address, a_mode, user::waterTankLengthInches);        
  eepromGetSet(a_address, a_mode, user::waterTankWidth);               
  eepromGetSet(a_address, a_mode, user::waterTankWidthInches);         
  eepromGetSet(a_address, a_mode, user::targetMinFanOneSpeed);         
  eepromGetSet(a_address, a_mode, user::targetMaxFanOneSpeed);         
  eepromGetSet(a_address, a_mode, user::targetMinFanTwoSpeed);         
  eepromGetSet(a_address, a_mode, user::targetMaxFanTwoSpeed);         
  eepromGetSet(a_address, a_mode, user::targetMinAirTemp);             
  eepromGetSet(a_address, a_mode, user::targetMaxAirTemp);             
  eepromGetSet(a_address, a_mode, user::targetMinAirTempF);            
  eepromGetSet(a_address, a_mode, user::targetMaxAirTempF);            
  eepromGetSet(a_address, a_mode, user::targetMinWaterTempF);          
  eepromGetSet(a_address, a_mode, user::targetMaxWaterTempF);          
  eepromGetSet(a_address, a_mode, user::targetMinHumidity);            
  eepromGetSet(a_address, a_mode, user::targetMaxHumidity);            
  eepromGetSet(a_address, a_mode, wifi::wifiEnabled);                  
  eepromGetSet(a_address, a_mode, user::doserOneSpeed);                
  eepromGetSet(a_address, a_mode, user::doserTwoSpeed);                
  eepromGetSet(a_address, a_mode, user::doserThreeSpeed);              
  eepromGetSet(a_address, a_mode, user::doserFourSpeed);               
  eepromGetSet(a_address, a_mode, user::doserFiveSpeed);               
  eepromGetSet(a_address, a_mode, user::doserSixSpeed);                
  eepromGetSet(a_address, a_mode, user::dosingInterval);               
  eepromGetSet(a_address, a_mode, user::refillDoserOneMills);          
  eepromGetSet(a_address, a_mode, user::refillDoserTwoMills);          
  eepromGetSet(a_address, a_mode, user::refillDoserThreeMills);        
  eepromGetSet(a_address, a_mode, user::refillDoserFourMills);         
  eepromGetSet(a_address, a_mode, user::refillDoserFiveMills);         
  eepromGetSet(a_address, a_mode, user::refillDoserSixMills);          
  eepromGetSet(a_address, a_mode, user::disableDrainAndRefill);        
  eepromGetSet(a_address, a_mode, user::autoFillHour);                 
  eepromGetSet(a_address, a_mode, user::autoFillMinute);               
  eepromGetSet(a_address, a_mode, user::drainTimeout);                 
  eepromGetSet(a_address, a_mode, user::backgroundColor);              
  eepromGetSet(a_address, a_mode, user::graphInterval);                
  eepromGetSet(a_address, a_mode, user::afkTime);      
  eepromGetSet(a_address, a_mode, user::brightness);                              
  eepromGetSet(a_address, a_mode, user::convertToF);                   
  eepromGetSet(a_address, a_mode, user::convertToInches);              
  eepromGetSet(a_address, a_mode, user::convertToTds);                 
  eepromGetSet(a_address, a_mode, user::tdsErrorMargin);               
  eepromGetSet(a_address, a_mode, user::phErrorMargin);                
  eepromGetSet(a_address, a_mode, user::ecErrorMargin);                
  eepromGetSet(a_address, a_mode, user::co2ErrorMargin);               
  eepromGetSet(a_address, a_mode, user::airTempErrorMargin);           
  eepromGetSet(a_address, a_mode, user::airTempErrorMarginF);          
  eepromGetSet(a_address, a_mode, user::humidityErrorMargin);          
  eepromGetSet(a_address, a_mode, user::waterTempErrorMargin);         
  eepromGetSet(a_address, a_mode, user::waterTempErrorMarginF);        
  eepromGetSet(a_address, a_mode, user::waterHeightErrorMargin);       
  eepromGetSet(a_address, a_mode, user::waterHeightErrorMarginInches);
  eepromGetSet(a_address, a_mode, user::disableAirTempWarnings);      
  eepromGetSet(a_address, a_mode, user::disableHumidityWarnings);      
  eepromGetSet(a_address, a_mode, user::disablePpmWarnings);      
  eepromGetSet(a_address, a_mode, user::disableEcWarnings);         
  eepromGetSet(a_address, a_mode, user::disablePhWarnings);          
  eepromGetSet(a_address, a_mode, user::disableCo2Warnings);         
  eepromGetSet(a_address, a_mode, user::disableWaterTempWarnings);    
  eepromGetSet(a_address, a_mode, user::disableWaterHeightWarnings);  
  eepromGetSet(a_address, a_mode, user::disableLED);         
  eepromGetSet(a_address, a_mode, user::disableBeeper);  
  eepromGetSet(a_address, a_mode, user::fansControlTemperature);      
  eepromGetSet(a_address, a_mode, user::fansControlHumidity);        
  eepromGetSet(a_address, a_mode, user::fanOneFixedSpeed);           
  eepromGetSet(a_address, a_mode, user::fanTwoFixedSpeed);      
  eepromGetSet(a_address, a_mode, user::numberOfDosers);      
  eepromGetSet(a_address, a_mode, user::heightSensor);  
  eepromGetSet(a_address, a_mode, sensor::sensorArrayPos);     
  eepromGetSet(a_address, a_mode, device::conversionType);
  eepromGetSet(a_address, a_mode, sensor::phDownSolution);
  eepromGetSet(a_address, a_mode, sensor::phUpSolution);
  eepromGetSet(a_address, a_mode, sensor::ecSolution);
  eepromGetSet(a_address, a_mode, sensor::tdsSolution);
  eepromGetSet(a_address, a_mode, user::ecDosingMode);
  eepromGetSet(a_address, a_mode, user::phDosingMode);
  eepromGetSet(a_address, a_mode, user::autoFillDays);
  if (device::globalDebug) {
    Serial.print(F("EEPROM ending at address: ")); Serial.println(a_address);
  }
  return a_address;
}