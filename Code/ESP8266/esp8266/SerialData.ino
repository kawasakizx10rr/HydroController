void receiveSerialData(const uint8_t a_page, const uint8_t a_slide) {
  bool readData = false;
  uint16_t index = 0, charCnt = 0;
  char buffer[64] {0};
  //uint32_t timeout = millis() + 2000UL;
  while (true) { // millis() <= timeout
    if (Serial.available() > 0) {
      char c = Serial.read();
      // reset local variables
      if (c == '!') {
        memset(buffer, 0, 64);
        //Serial.print("?SOT\n"); // Start of transmission
        index = 0;
        charCnt = 0;
        readData = true;
        timeout = millis();
      }
      else if (c == '#' && readData) {
        //Serial.print("?EOT\n");  // End of transmission
        readData = false;
        break;
      }
      // value to be stored
      else if (c == ',' && readData) {
        updateValue(a_page, a_slide, buffer, index);
        //printf("?P %d, I %d\n", a_page, index); // P = page , I = index
        memset(buffer, 0, 64);    
        index++;
        charCnt = 0;
        //timeout = millis();
      }
      // add serial data to buffer
      else if (c >= 32 && c <= 126 && charCnt < 64 && readData) {
        buffer[charCnt++] = c;
      }
    }
    yield();
  } 
}

void updateValue(const uint8_t a_page, const uint8_t a_slide, const char* a_buffer, const uint16_t a_index) {
  if (a_page == 0) {
    if (a_index == 0)
      bufferToBool(user::convertToF, a_buffer);
    else if (a_index == 1)
      bufferToBool(user::convertToInches, a_buffer);
    else if (a_index == 2)
        bufferToUint16(sensor::sensorArrayPos, a_buffer);
    if (a_slide == 0) {  // home page slide 0
      if (a_index == 3)
        bufferToFloat(sensor::ec, a_buffer);
      else if (a_index == 4)
        bufferToFloat(user::targetMinEc, a_buffer);
      else if (a_index == 5)
        bufferToFloat(user::targetMaxEc, a_buffer);
      else 
        copyBufferToArray(sensor::ecArray, sensor::maxSensorArrayVals, a_index - 6, a_buffer);
    } 
    else if (a_slide == 1) {  // home page slide 1
      if (a_index == 3)
        bufferToUint16(sensor::tds, a_buffer);
      else if (a_index == 4)
        bufferToUint16(user::targetMinTds, a_buffer);
      else if (a_index == 5)
        bufferToUint16(user::targetMaxTds, a_buffer);
      else
        copyBufferToArray(sensor::tdsArray, sensor::maxSensorArrayVals, a_index - 6, a_buffer);
    } 
    else if (a_slide == 2) {  // home page slide 2
      if (a_index == 3)
        bufferToFloat(sensor::ph, a_buffer);
      else if (a_index == 4)
        bufferToFloat(user::targetMinPh, a_buffer);
      else if (a_index == 5)
        bufferToFloat(user::targetMaxPh, a_buffer);
      else 
        copyBufferToArray(sensor::phArray, sensor::maxSensorArrayVals, a_index - 6, a_buffer);
    } 
    else if (a_slide == 3) {  // home page slide 3
      if (a_index == 3)
        bufferToUint16(sensor::co2, a_buffer);
      else if (a_index == 4)
        bufferToUint16(user::targetCo2, a_buffer);
      else if (a_index == 5)
        bufferToUint16(user::targetCo2, a_buffer);
      else 
        copyBufferToArray(sensor::co2Array, sensor::maxSensorArrayVals, a_index - 6, a_buffer);
    } 
    else if (a_slide == 4) {  // home page slide 4
      if (a_index == 3)
        bufferToFloat(sensor::waterTemp, a_buffer);
      else if (a_index == 4)
        bufferToFloat(user::targetMinWaterTemp, a_buffer);
      else if (a_index == 5
        bufferToFloat(user::targetMaxWaterTemp, a_buffer);
      else 
        copyBufferToArray(sensor::waterTemperatureArray, sensor::maxSensorArrayVals, a_index - 6, a_buffer);
    } 
    else if (a_slide == 5) {  // home page slide 5
      if (a_index == 3)
        bufferToFloat(sensor::waterLevel, a_buffer);
      else if (a_index == 4)
        bufferToFloat(user::targetMinWaterHeight, a_buffer);
      else if (a_index == 5)
        bufferToFloat(user::targetMaxWaterHeight, a_buffer);
      else 
        copyBufferToArray(sensor::waterLevelArray, sensor::maxSensorArrayVals, a_index - 6, a_buffer);
    }
    else if (a_slide == 6) {  // home page slide 6
      if (a_index == 3)
        bufferToFloat(sensor::airTemp, a_buffer);
      else if (a_index == 4)
        bufferToFloat(user::targetMinAirTemp, a_buffer);
      else if (a_index == 5)
        bufferToFloat(user::targetMaxAirTemp, a_buffer);
      else 
        copyBufferToArray(sensor::airTemperatureArray, sensor::maxSensorArrayVals, a_index - 6, a_buffer);
    } 
    else if (a_slide == 7) {  // home page slide 7
      if (a_index == 3)
        bufferToFloat(sensor::humidity, a_buffer);
      else if (a_index == 4)
        bufferToFloat(user::targetMinHumidity, a_buffer);
      else if (a_index == 5)
        bufferToFloat(user::targetMaxHumidity, a_buffer);
      else 
        copyBufferToArray(sensor::humidityArray, sensor::maxSensorArrayVals, a_index - 6, a_buffer);
    } 
    else if (a_slide == 8) {  // home page slide 8
      if (a_index == 3)
        bufferToUint8(device::fanOneSpeed, a_buffer);
      else if (a_index == 4)
        bufferToUint8(user::targetMinFanOneSpeed, a_buffer);
      else if (a_index == 5
        bufferToUint8(user::targetMaxFanOneSpeed, a_buffer);
      else 
        copyBufferToArray(sensor::fanOneSpeedArray, sensor::maxSensorArrayVals, a_index - 6, a_buffer);
    } 
    else if (a_slide == 9) {  // home page slide 9
      if (a_index == 3)
        bufferToUint8(device::fanTwoSpeed, a_buffer);
      else if (a_index == 4)
        bufferToUint8(user::targetMinFanTwoSpeed, a_buffer);
      else if (a_index == 5)
        bufferToUint8(user::targetMaxFanTwoSpeed, a_buffer);
      else
        copyBufferToArray(sensor::fanTwoSpeedArray, sensor::maxSensorArrayVals, a_index - 6, a_buffer);
    }
  }
  // Page 1 (Graphs page)
  else if (a_page == 1) {
    if (a_index == 0)
      bufferToBool(user::convertToF, a_buffer);
    else if (a_index == 1)
      bufferToBool(user::convertToInches, a_buffer);
    else if (a_index == 2)
      bufferToUint16(sensor::sensorArrayPos, a_buffer);
    //
    else {
      if (a_slide == 0) {
        copyBufferToArray(sensor::ecArray, sensor::maxSensorArrayVals, a_index - 3, a_buffer);
      } 
      else if (a_slide == 1) {
        copyBufferToArray(sensor::tdsArray, sensor::maxSensorArrayVals, a_index - 3, a_buffer);
      } 
      else if (a_slide == 2) {
        copyBufferToArray(sensor::phArray, sensor::maxSensorArrayVals, a_index - 3, a_buffer);
      } 
      else if (a_slide == 3) {
        copyBufferToArray(sensor::co2Array, sensor::maxSensorArrayVals, a_index - 3, a_buffer);
      } 
      else if (a_slide == 4) {
        copyBufferToArray(sensor::waterTemperatureArray, sensor::maxSensorArrayVals, a_index - 3, a_buffer);
      } 
      else if (a_slide == 5) {
        copyBufferToArray(sensor::waterLevelArray, sensor::maxSensorArrayVals, a_index - 3, a_buffer);
      } 
      else if (a_slide == 6) {
        copyBufferToArray(sensor::airTemperatureArray, sensor::maxSensorArrayVals, a_index - 3, a_buffer);
      } 
      else if (a_slide == 7) {
        copyBufferToArray(sensor::humidityArray, sensor::maxSensorArrayVals, a_index - 3, a_buffer);
      } 
      else if (a_slide == 8) {
        copyBufferToArray(sensor::fanOneSpeedArray, sensor::maxSensorArrayVals, a_index - 3, a_buffer);
      } 
      else if (a_slide == 9) {
        copyBufferToArray(sensor::fanTwoSpeedArray, sensor::maxSensorArrayVals, a_index - 3, a_buffer);
      }
    }
  }
  // Page 2 (Max mins page)
  else if (a_page == 2) {
    if (a_index == 0)
      bufferToBool(user::convertToF, a_buffer);
    else if (a_index == 1)
      bufferToBool(user::convertToInches, a_buffer);
    //
    if (a_slide == 0) {
      if (a_index == 2)
        bufferToFloat(device::minEc, a_buffer);
      else if (a_index == 3) 
        bufferToFloat(device::maxEc, a_buffer);
    } 
    else if (a_slide == 1) {
      if (a_index == 2)
        bufferToUint16(device::minTds, a_buffer);
      else if (a_index == 3)
        bufferToUint16(device::maxTds, a_buffer);
    } 
    else if (a_slide == 2) {
      if (a_index == 2)
        bufferToFloat(device::minPh, a_buffer);
      else if (a_index == 3)
        bufferToFloat(device::maxPh, a_buffer);
    } 
    else if (a_slide == 3) {
      if (a_index == 2)
        bufferToUint16(device::minCo2, a_buffer);
      else if (a_index == 3) 
        bufferToUint16(device::maxCo2, a_buffer);
    } 
    else if (a_slide == 4) {
      if (a_index == 2)
        bufferToFloat(device::minWaterTemp, a_buffer);
      else if (a_index == 3) 
        bufferToFloat(device::maxWaterTemp, a_buffer);
    } 
    else if (a_slide == 5) {
      if (a_index == 2)
        bufferToFloat(device::minWaterLevel, a_buffer);
      else if (a_index == 3) 
        bufferToFloat(device::maxWaterLevel, a_buffer);
    }
    else if (a_slide == 6) {
      if (a_index == 2)
        bufferToFloat(device::minAirTemp, a_buffer);
      else if (a_index == 3) 
        bufferToFloat(device::maxAirTemp, a_buffer);
    } 
    else if (a_slide == 7) {
      if (a_index == 2)
        bufferToFloat(device::minHumidity, a_buffer);
      else if (a_index == 3) 
        bufferToFloat(device::maxHumidity, a_buffer);
    } 
    else if (a_slide == 8) {
      if (a_index == 2)
        bufferToUint8(device::minFanOneSpeed, a_buffer);
      else if (a_index == 3) 
        bufferToUint8(device::maxFanOneSpeed, a_buffer);
    } 
    else if (a_slide == 9) {
      if (a_index == 2)
        bufferToUint8(device::minFanTwoSpeed, a_buffer);
      else if (a_index == 3) 
        bufferToUint8(device::maxFanTwoSpeed, a_buffer);
    }
  }
  // Page 3 (Profiles page)
  else if (a_page == 3) {
    if (a_index == 0)
      strcpy(user::profileOneName, a_buffer);
    else if (a_index == 1)
      strcpy(user::profileTwoName, a_buffer);
    else if (a_index == 2)
      strcpy(user::profileThreeName, a_buffer);
    else if (a_index == 3)
      strcpy(user::profileFourName, a_buffer);
    else if (a_index == 4) 
      strcpy(user::profileFiveName, a_buffer);
  }
  // Page 4 (Settings page)
  else if (a_page == 4) {
    if (a_index == 0)
      bufferToUint8(user::numberOfDosers, a_buffer);
    else if (a_index == 1)
      bufferToUint8(user::dosingInterval, a_buffer);
    else if (a_index == 2)
      bufferToBool(user::convertToF, a_buffer);
    else if (a_index == 3)
      bufferToBool(user::convertToInches, a_buffer);
    else if (a_index == 4)
      bufferToBool(user::convertToTds, a_buffer);
    else if (a_index == 5)
      bufferToBool(user::disablePpmWarnings, a_buffer);
    else if (a_index == 6)
      bufferToBool(user::disableEcWarnings, a_buffer);
    else if (a_index == 7)
      bufferToBool(user::disableCo2Warnings, a_buffer);
    else if (a_index == 8)
      bufferToBool(user::disablePhWarnings, a_buffer);
    else if (a_index == 9)
      bufferToBool(user::disableWaterTempWarnings, a_buffer);
    else if (a_index == 10)
      bufferToBool(user::disableWaterHeightWarnings, a_buffer);
    else if (a_index == 11)
      bufferToBool(user::disableAirTempWarnings, a_buffer);
    else if (a_index == 12) 
      bufferToBool(user::disableHumidityWarnings, a_buffer);
  }
  // Page 5 (EC\TDS page)
  else if (a_page == 5) {
    if (a_index == 0)
      bufferToBool(user::convertToTds, a_buffer);
    else if (a_index == 1)
      bufferToUint16(user::targetMinTds, a_buffer);
    else if (a_index == 2)
      bufferToUint16(user::targetMaxTds, a_buffer);
    else if (a_index == 3)
      bufferToFloat(user::targetMinEc, a_buffer);
    else if (a_index == 4) 
      bufferToFloat(user::targetMaxEc, a_buffer);
  }
  // Page 6 (PH page)
  else if (a_page == 6) {
    if (a_index == 0)
      bufferToFloat(user::targetMinPh, a_buffer);
    else if (a_index == 1) 
      bufferToFloat(user::targetMaxPh, a_buffer);
  }
    // Page 7 (Co2 page)
  else if (a_page == 7) {  // small amount of data so no need to segment based on slider
    if (a_index == 0)
      bufferToBool(user::convertToInches, a_buffer);
    else if (a_index == 1)
      bufferToUint16(user::roomLength, a_buffer);
    else if (a_index == 2)
      bufferToUint16(user::roomWidth, a_buffer);
    else if (a_index == 3)
      bufferToUint16(user::roomHeight, a_buffer);
    else if (a_index == 4)
      bufferToFloat(user::co2Flowrate, a_buffer);
    else if (a_index == 5)
      bufferToUint8(user::co2CheckTimeMinute, a_buffer);
    else if (a_index == 6)
      bufferToUint8(user::co2CheckTimeHour, a_buffer);
    else if (a_index == 7)
      bufferToUint16(user::disableFansTimer, a_buffer);
    else if (a_index == 8)
      bufferToBool(user::enableManualCo2Duration, a_buffer);
    else if (a_index == 9)
      bufferToBool(user::disableCo2Control, a_buffer);
    else if (a_index == 10)
      bufferToUint16(user::manualCo2GasDuration, a_buffer);
  }
  // Page 8 (Water page)
  else if (a_page == 8) {  // small amount of data so no need to segment based on slider
    if (a_index == 0)
      bufferToBool(user::convertToF, a_buffer);
    else if (a_index == 1)
      bufferToBool(user::convertToInches, a_buffer);
    else if (a_index == 2)
      bufferToFloat(user::targetMinWaterHeight, a_buffer);
    else if (a_index == 3)
      bufferToFloat(user::targetMaxWaterHeight, a_buffer);
    else if (a_index == 4)
      bufferToFloat(user::targetMinWaterTemp, a_buffer);
    else if (a_index == 5)
      bufferToFloat(user::targetMaxWaterTemp, a_buffer);
    else if (a_index == 6)
      bufferToFloat(user::waterTankLength, a_buffer);
    else if (a_index == 7)
      bufferToFloat(user::waterTankWidth, a_buffer);
    else if (a_index == 8)
      bufferToUint8(user::autoFillMinute, a_buffer);
    else if (a_index == 9)
      bufferToUint8(user::autoFillHour, a_buffer);
    else if (a_index == 10)
      bufferToBool(user::disableDrainAndRefill, a_buffer);
    else if (a_index == 11)
      bufferToUint16(user::refillDoserOneMills, a_buffer);
    else if (a_index == 12)
      bufferToUint16(user::refillDoserTwoMills, a_buffer);
    else if (a_index == 13)
      bufferToUint16(user::refillDoserThreeMills, a_buffer);
    else if (a_index == 14)
      bufferToUint16(user::refillDoserFourMills, a_buffer);
    else if (a_index == 15)
      bufferToUint16(user::refillDoserFiveMills, a_buffer);
    else if (a_index == 16)
      bufferToUint16(user::refillDoserSixMills, a_buffer);
    else 
      copyBuffToBoolArray(user::autoFillDays, 31, a_index - 17, a_buffer);
  }
  // Page 9 (Doser page)
  else if (a_page == 9) {
    if (a_index == 0)
      bufferToUint8(user::numberOfDosers, a_buffer);
    else if (a_index == 1)
      bufferToUint8(user::doserOneMode, a_buffer);
    else if (a_index == 2)
      bufferToUint8(user::doserTwoMode, a_buffer);
    else if (a_index == 3)
      bufferToUint8(user::doserThreeMode, a_buffer);
    else if (a_index == 4)
      bufferToUint8(user::doserFourMode, a_buffer);
    else if (a_index == 5)
      bufferToUint8(user::doserFiveMode, a_buffer);
    else if (a_index == 6)
      bufferToUint8(user::doserSixMode, a_buffer);
    else if (a_index == 7)
      bufferToUint16(user::doserOneMills, a_buffer);
    else if (a_index == 8)
      bufferToUint16(user::doserTwoMills, a_buffer);
    else if (a_index == 9)
      bufferToUint16(user::doserThreeMills, a_buffer);
    else if (a_index == 10)
      bufferToUint16(user::doserFourMills, a_buffer);
    else if (a_index == 11)
      bufferToUint16(user::doserFiveMills, a_buffer);
    else if (a_index == 12) 
      bufferToUint16(user::doserSixMills, a_buffer);
      //doserOneSpeed = values[cnt++];
      //doserTwoSpeed = values[cnt++];
      //doserThreeSpeed = values[cnt++];
      //doserFourSpeed = values[cnt++];
      //doserFiveSpeed = values[cnt++];
      //doserSixSpeed = values[cnt++];
  }
  // Page 10 (Lighting page)
  else if (a_page == 10) {
    if (a_index == 0)
      bufferToUint8(user::lightOnTimeMin, a_buffer);
    else if (a_index == 1)
      bufferToUint8(user::lightOnTimeHour, a_buffer);
    else if (a_index == 2)
      bufferToUint8(user::lightOffTimeMin, a_buffer);
    else if (a_index == 3)
      bufferToUint8(user::lightOffTimeHour, a_buffer);
    else if (a_index == 4)
      bufferToUint8(user::lightMode, a_buffer);
  }
    // Page 11 (Fans page)
  else if (a_page == 11) {  // small amount of data so no need to segment based on slider
    if (a_index == 0)
      bufferToBool(user::convertToF, a_buffer);
    else if (a_index == 1)
      bufferToUint8(user::targetMinFanOneSpeed, a_buffer);
    else if (a_index == 2)
      bufferToUint8(user::targetMaxFanOneSpeed, a_buffer);
    else if (a_index == 3)
      bufferToUint8(user::targetMinFanTwoSpeed, a_buffer);
    else if (a_index == 4)
      bufferToUint8(user::targetMaxFanTwoSpeed, a_buffer);
    else if (a_index == 5)
      bufferToFloat(user::targetMinAirTemp, a_buffer);
    else if (a_index == 6)
      bufferToFloat(user::targetMaxAirTemp, a_buffer);
    else if (a_index == 7)
      bufferToFloat(user::targetMinHumidity, a_buffer);
    else if (a_index == 8)
      bufferToFloat(user::targetMaxHumidity, a_buffer);
    else if (a_index == 9)
      bufferToBool(user::fansControlTemperature, a_buffer);
    else if (a_index == 10)
      bufferToBool(user::fansControlHumidity, a_buffer);
    else if (a_index == 11)
      bufferToBool(user::fanOneFixedSpeed, a_buffer);
    else if (a_index == 12) 
      bufferToBool(user::fanTwoFixedSpeed, a_buffer);
  }
    // Page 12 (Warnings page)
  else if (a_page == 12) {  // small amount of data so no need to segment based on slider
    if (a_index == 0)
      bufferToBool(user::convertToF, a_buffer); 
    else if (a_index == 1)
      bufferToBool(user::convertToInches, a_buffer); 
    else if (a_index == 2)
      bufferToUint16(sensor::tds, a_buffer);
    else if (a_index == 3)
      bufferToUint16(user::targetMinTds, a_buffer);
    else if (a_index == 4)
      bufferToUint16(user::targetMaxTds, a_buffer);
    else if (a_index == 5)
      bufferToUint16(user::tdsErrorMargin, a_buffer);
    else if (a_index == 6)
      bufferToFloat(sensor::ec, a_buffer);
    else if (a_index == 7)
      bufferToFloat(user::targetMinEc, a_buffer);
    else if (a_index == 8)
      bufferToFloat(user::targetMaxEc, a_buffer);
    else if (a_index == 9)
      bufferToFloat(user::ecErrorMargin, a_buffer);
    else if (a_index == 10)
      bufferToFloat(sensor::ph, a_buffer);
    else if (a_index == 11)
      bufferToFloat(user::targetMinPh, a_buffer);
    else if (a_index == 12)
      bufferToFloat(user::targetMaxPh, a_buffer);
    else if (a_index == 13)
      bufferToFloat(user::phErrorMargin, a_buffer);
    else if (a_index == 14)
      bufferToUint16(sensor::co2, a_buffer);
    else if (a_index == 15)
      bufferToUint16(user::targetCo2, a_buffer);
    else if (a_index == 16)
      bufferToUint16(user::targetCo2, a_buffer);
    else if (a_index == 17)
      bufferToUint16(user::co2ErrorMargin, a_buffer);
    else if (a_index == 18)   
      bufferToFloat(sensor::waterTemp, a_buffer);
    else if (a_index == 19)      
      bufferToFloat(user::targetMinWaterTemp, a_buffer);
    else if (a_index == 20)    
      bufferToFloat(user::targetMaxWaterTemp, a_buffer);
    else if (a_index == 21)     
      bufferToFloat(user::waterTempErrorMargin, a_buffer);
    else if (a_index == 22) 
      bufferToFloat(sensor::waterLevel, a_buffer);
    else if (a_index == 23)   
      bufferToFloat(user::targetMinWaterHeight, a_buffer);
    else if (a_index == 24)   
      bufferToFloat(user::targetMaxWaterHeight, a_buffer);
    else if (a_index == 25)   
      bufferToFloat(user::waterHeightErrorMargin, a_buffer);
    else if (a_index == 26)     
      bufferToFloat(sensor::airTemp, a_buffer);
    else if (a_index == 27)    
      bufferToFloat(user::targetMinAirTemp, a_buffer);
    else if (a_index == 28)    
      bufferToFloat(user::targetMaxAirTemp, a_buffer);
    else if (a_index == 29)  
      bufferToFloat(user::airTempErrorMargin, a_buffer);
    else if (a_index == 30)  
      bufferToFloat(sensor::humidity, a_buffer);
    else if (a_index == 31)  
      bufferToFloat(user::targetMinHumidity, a_buffer);
    else if (a_index == 32)  
      bufferToFloat(user::targetMaxHumidity, a_buffer);
    else if (a_index == 33) 
      bufferToFloat(user::humidityErrorMargin, a_buffer);
  }
  else if (device::globalDebug) {
    Serial.print(F("Page not found!"));
  }
}

void bufferToFloat(float& a_value, const char* a_buffer) {
  a_value = atof(a_buffer);
}

void bufferToBool(bool& a_value, const char* a_buffer) {
  a_value = atoi(a_buffer);
}

void bufferToUint8(uint8_t& a_value, const char* a_buffer) {
  a_value = atoi(a_buffer);
}

void bufferToUint16(uint16_t& a_value, const char* a_buffer) {
  a_value = atoi(a_buffer);
}

void bufferToUint32(uint32_t& a_value, const char* a_buffer) {
  a_value = atol(a_buffer);
}

void copyBufferToArray(float* a_array, const uint8_t a_len, const uint8_t a_index, const char* a_buffer) {
  if (a_index < a_len)
    a_array[a_index] = atof(a_buffer);
}

void copyBuffToBoolArray(bool* a_array, const uint8_t a_len, const uint8_t a_index, const char* a_buffer) {
  if (a_index < a_len)
    a_array[a_index] = atoi(a_buffer);
}