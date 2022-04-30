// Protocol
// 1: Arduino sends '!' to request data
// 2: ESP sends '#' to confirm request, followed by CSV data
// 3: Arduino sends '@' to confirm data collection, followed by CSV data

void handleSerialData () {
  static byte serialArrayCount = 0;
  static byte buffPos = 0;
  static bool receivedSerialRequest = false;
  static bool readingSerialData = false;
  static bool dataLoaded = false;
  static char buffer[64] {};
  const byte delayTime = 1;
  if (Serial.available() > 0) {
    //Serial.println(F("Got SerialStream"));
    unsigned long currentTime = millis();
    while (millis() - currentTime < 500UL) {
      char c = Serial.read();
      if (c == '!') {
        if (debugEnabled)
          Serial.println(F("\nReceived data request.\nSending data..."));
        buffPos = 0;
        serialArrayCount = 0;
        for (char& buff : buffer) buff = 0;
        readingSerialData = false;
        receivedSerialRequest = true;
        Serial.print("#");
        // dataLoaded when true the mega2560 will overwrite any unadjusted local variables
        Serial.print(dataLoaded); Serial.print(","); delay(delayTime);
        // profiles page
        Serial.print(profileOneName); Serial.print(","); delay(delayTime); // 0
        Serial.print(profileTwoName); Serial.print(","); delay(delayTime);
        Serial.print(profileThreeName); Serial.print(","); delay(delayTime);
        Serial.print(profileFourName); Serial.print(","); delay(delayTime);
        Serial.print(profileFiveName); Serial.print(","); delay(delayTime);
        // settings page
        Serial.print(numberOfDosers); Serial.print(","); delay(delayTime);
        Serial.print(dosingInterval); Serial.print(","); delay(delayTime);
        Serial.print(disablePpmWarnings); Serial.print(","); delay(delayTime);
        Serial.print(disableEcWarnings); Serial.print(","); delay(delayTime);
        Serial.print(disableCo2Warnings); Serial.print(","); delay(delayTime);
        Serial.print(disablePhWarnings); Serial.print(","); delay(delayTime);
        Serial.print(disableWaterTempWarnings); Serial.print(","); delay(delayTime);
        Serial.print(disableWaterHeightWarnings); Serial.print(","); delay(delayTime);
        Serial.print(disableAirTempWarnings); Serial.print(","); delay(delayTime);
        Serial.print(disableHumidityWarnings); Serial.print(","); delay(delayTime);
        // ec-tds page
        Serial.print(targetMinEc, 2); Serial.print(","); delay(delayTime);
        Serial.print(targetMaxEc, 2); Serial.print(","); delay(delayTime);
        Serial.print(ecOffset, 2); Serial.print(","); delay(delayTime);
        Serial.print(targetMinTds); Serial.print(","); delay(delayTime);
        Serial.print(targetMaxTds); Serial.print(","); delay(delayTime);
        Serial.print(tdsOffset); Serial.print(","); delay(delayTime);
        // ph page
        Serial.print(targetMinPh, 2); Serial.print(","); delay(delayTime);
        Serial.print(targetMaxPh, 2); Serial.print(","); delay(delayTime);
        Serial.print(phOffset, 2); Serial.print(","); delay(delayTime);
        // co2 page
        Serial.print(targetCo2); Serial.print(","); delay(delayTime);
        Serial.print(co2Offset); Serial.print(","); delay(delayTime);
        Serial.print(roomWidth); Serial.print(","); delay(delayTime);
        Serial.print(roomLength); Serial.print(","); delay(delayTime);
        Serial.print(roomHeight); Serial.print(","); delay(delayTime);
        Serial.print(co2Flowrate, 1); Serial.print(","); delay(delayTime);
        Serial.print(co2CheckTimeMinute); Serial.print(","); delay(delayTime);
        Serial.print(co2CheckTimeHour); Serial.print(","); delay(delayTime);
        Serial.print(disableFansTimer); Serial.print(","); delay(delayTime);
        Serial.print(manualCo2Duration); Serial.print(","); delay(delayTime);
        Serial.print(manualCo2GasDuration); Serial.print(","); delay(delayTime);
        Serial.print(disableCo2Control); Serial.print(","); delay(delayTime);
        // water page
        Serial.print(targetMinWaterHeight, 1); Serial.print(","); delay(delayTime);
        Serial.print(targetMaxWaterHeight, 1); Serial.print(","); delay(delayTime);
        Serial.print(waterHeightOffset, 1); Serial.print(","); delay(delayTime);
        Serial.print(targetMinWaterTemp, 1); Serial.print(","); delay(delayTime);
        Serial.print(targetMaxWaterTemp, 1); Serial.print(","); delay(delayTime);
        Serial.print(waterTempOffset, 1); Serial.print(","); delay(delayTime);
        printBoolArray(autoFillDays, 31);
        Serial.print(autoFillMinute); Serial.print(","); delay(delayTime);
        Serial.print(autoFillHour); Serial.print(","); delay(delayTime);
        Serial.print(disableDrainAndRefill); Serial.print(","); delay(delayTime);
        Serial.print(refillDoserOneMills); Serial.print(","); delay(delayTime);
        Serial.print(refillDoserTwoMills); Serial.print(","); delay(delayTime);
        Serial.print(refillDoserThreeMills); Serial.print(","); delay(delayTime);
        Serial.print(refillDoserFourMills); Serial.print(","); delay(delayTime);
        Serial.print(refillDoserFiveMills); Serial.print(","); delay(delayTime);
        Serial.print(refillDoserSixMills); Serial.print(","); delay(delayTime);
        // doser page
        Serial.print(doserOneMode); Serial.print(","); delay(delayTime);
        Serial.print(doserTwoMode); Serial.print(","); delay(delayTime);
        Serial.print(doserThreeMode); Serial.print(","); delay(delayTime);
        Serial.print(doserFourMode); Serial.print(","); delay(delayTime);
        Serial.print(doserFiveMode); Serial.print(","); delay(delayTime);
        Serial.print(doserSixMode); Serial.print(","); delay(delayTime);
        Serial.print(doserOneMills); Serial.print(","); delay(delayTime);
        Serial.print(doserTwoMills); Serial.print(","); delay(delayTime);
        Serial.print(doserThreeMills); Serial.print(","); delay(delayTime);
        Serial.print(doserFourMills); Serial.print(","); delay(delayTime);
        Serial.print(doserFiveMills); Serial.print(","); delay(delayTime);
        Serial.print(doserSixMills); Serial.print(","); delay(delayTime);
        //Serial.print(doserOneSpeed); Serial.print(","); delay(delayTime);
        //Serial.print(doserTwoSpeed); Serial.print(","); delay(delayTime);
        //Serial.print(doserThreeSpeed); Serial.print(","); delay(delayTime);
        //Serial.print(doserFourSpeed); Serial.print(","); delay(delayTime);
        //Serial.print(doserFiveSpeed); Serial.print(","); delay(delayTime);
        //Serial.print(doserSixSpeed); Serial.print(","); delay(delayTime);
        //Serial.print(swapInterval); Serial.print(","); delay(delayTime);
        // light page
        Serial.print(lightOnTimeHour); Serial.print(","); delay(delayTime);
        Serial.print(lightOnTimeMin); Serial.print(","); delay(delayTime);
        Serial.print(lightOffTimeHour); Serial.print(","); delay(delayTime);
        Serial.print(lightOffTimeMin); Serial.print(","); delay(delayTime);
        Serial.print(lightMode); Serial.print(","); delay(delayTime);
        // fans page
        Serial.print(targetMinFanOneSpeed); Serial.print(","); delay(delayTime);
        Serial.print(targetMaxFanOneSpeed); Serial.print(","); delay(delayTime);
        Serial.print(targetMinFanTwoSpeed); Serial.print(","); delay(delayTime);
        Serial.print(targetMaxFanTwoSpeed); Serial.print(","); delay(delayTime);
        Serial.print(targetMinAirTemp, 1); Serial.print(","); delay(delayTime);
        Serial.print(targetMaxAirTemp, 1); Serial.print(","); delay(delayTime);
        Serial.print(airTempOffset, 1); Serial.print(","); delay(delayTime);
        Serial.print(targetMinHumidity, 1); Serial.print(","); delay(delayTime);
        Serial.print(targetMaxHumidity, 1); Serial.print(","); delay(delayTime);
        Serial.print(humidityOffset, 1); Serial.print(","); delay(delayTime);
        Serial.print(fansControlTemperature); Serial.print(","); delay(delayTime);
        Serial.print(fansControlHumidity); Serial.print(","); delay(delayTime);
        Serial.print(fanOneFixedSpeed); Serial.print(","); delay(delayTime);
        Serial.print(fanTwoFixedSpeed); Serial.print(","); delay(delayTime);
        // warnings page
        Serial.print(tdsErrorMargin); Serial.print(","); delay(delayTime);
        Serial.print(ecErrorMargin, 2); Serial.print(","); delay(delayTime);
        Serial.print(phErrorMargin, 2); Serial.print(","); delay(delayTime);
        Serial.print(co2ErrorMargin); Serial.print(","); delay(delayTime);
        Serial.print(waterTempErrorMargin, 1); Serial.print(","); delay(delayTime);
        Serial.print(waterHeightErrorMargin, 1); Serial.print(","); delay(delayTime);
        Serial.print(airTempErrorMargin, 1); Serial.print(","); delay(delayTime);
        Serial.print(humidityErrorMargin, 1); Serial.print(","); delay(delayTime);
        // system
        Serial.print(loadUserProfile); Serial.print(","); delay(delayTime);
        Serial.print(saveUserProfile); Serial.print(","); delay(delayTime);
        // user settings but they must be the last values sent
        Serial.print(convertToFarenheit); Serial.print(","); delay(delayTime);
        Serial.print(convertToInchs); Serial.print(","); delay(delayTime);
        Serial.print(convertToTds); Serial.print(","); delay(delayTime);
        loadUserProfile = -1;
        saveUserProfile = -1;
      }
      else if (c == '@' && receivedSerialRequest) {
        // reset Serial helpers
        buffPos = 0;
        serialArrayCount = 0;
        for (char& buff : buffer) buff = 0;
        //saveWebData = false;
        readingSerialData = true;
        receivedSerialRequest = false;
        if (debugEnabled)
          Serial.println(F("Received confirmation.\nAwaiting data..."));
      }
      else if (c == ',' && readingSerialData) {
        // if (debugEnabled) {
        //Serial.print(F("Got val: ")); Serial.print(buffer); Serial.print(F(" serialArrayCount: ")); Serial.println(serialArrayCount);
        // }
        if (serialArrayCount == 0) {
          convertToFarenheit = atoi(buffer);
        }
        else if (serialArrayCount == 1) {
          convertToInchs = atoi(buffer);
        }
        else if (serialArrayCount == 2) {
          convertToTds = atoi(buffer);
        }
        else if (serialArrayCount == 3) {
          byte i = 0;
          char* data = strtok(buffer, "|");
          while (data != NULL && i < maxGraphArrayValues) {
            ecArray[i++] = atof(data);
            data = strtok(NULL, "|");
          }
        }
        else if (serialArrayCount == 4) {
          byte i = 0;
          char* data = strtok(buffer, "|");
          while (data != NULL && i < maxGraphArrayValues) {
            tdsArray[i++] = atof(data);
            data = strtok(NULL, "|");
          }
        }
        else if (serialArrayCount == 5) {
          byte i = 0;
          char* data = strtok(buffer, "|");
          while (data != NULL && i < maxGraphArrayValues) {
            phArray[i++] = atof(data);
            data = strtok(NULL, "|");
          }
        }
        else if (serialArrayCount == 6) {
          byte i = 0;
          char* data = strtok(buffer, "|");
          while (data != NULL && i < maxGraphArrayValues) {
            co2Array[i++] = atof(data);
            data = strtok(NULL, "|");
          }
        }
        else if (serialArrayCount == 7) {
          byte i = 0;
          char* data = strtok(buffer, "|");
          while (data != NULL && i < maxGraphArrayValues) {
            waterTempArray[i++] = atof(data);
            data = strtok(NULL, "|");
          }
        }
        else if (serialArrayCount == 8) {
          byte i = 0;
          char* data = strtok(buffer, "|");
          while (data != NULL && i < maxGraphArrayValues) {
            waterLevelArray[i++] = atof(data);
            data = strtok(NULL, "|");
          }
        }
        else if (serialArrayCount == 9) {
          byte i = 0;
          char* data = strtok(buffer, "|");
          while (data != NULL && i < maxGraphArrayValues) {
            airTempArray[i++] = atof(data);
            data = strtok(NULL, "|");
          }
        }
        else if (serialArrayCount == 10) {
          byte i = 0;
          char* data = strtok(buffer, "|");
          while (data != NULL && i < maxGraphArrayValues) {
            humidityArray[i++] = atof(data);
            data = strtok(NULL, "|");
          }
        }
        else if (serialArrayCount == 11) {
          byte i = 0;
          char* data = strtok(buffer, "|");
          while (data != NULL && i < maxGraphArrayValues) {
            fanOneSpeedArray[i++] = atof(data);
            data = strtok(NULL, "|");
          }
        }
        else if (serialArrayCount == 12) {
          byte i = 0;
          char* data = strtok(buffer, "|");
          while (data != NULL && i < maxGraphArrayValues) {
            fanTwoSpeedArray[i++] = atof(data);
            data = strtok(NULL, "|");
          }
        }
        else if (serialArrayCount == 13) {
          byte i = 0;
          char* data = strtok(buffer, "|");
          while (data != NULL && i < 31) {
            autoFillDays[i++] = atoi(data);
            data = strtok(NULL, "|");
          }
        }
        else if (serialArrayCount == 14) {
          graphArrayPos = atoi(buffer);
        }
        else if (serialArrayCount == 15) {
          strcpy_s(profileOneName, buffer, 11);
        }
        else if (serialArrayCount == 16) {
          strcpy_s(profileTwoName, buffer, 11);
        }
        else if (serialArrayCount == 17) {
          strcpy_s(profileThreeName, buffer, 11);
        }
        else if (serialArrayCount == 18) {
          strcpy_s(profileFourName, buffer, 11);
        }
        else if (serialArrayCount == 19) {
          strcpy_s(profileFiveName, buffer, 11);
        }
        else if (serialArrayCount == 20) {
          lightOnTimeHour = atoi(buffer);
        }
        else if (serialArrayCount == 21) {
          lightOnTimeMin = atoi(buffer);
        }
        else if (serialArrayCount == 22) {
          lightOffTimeHour = atoi(buffer);
        }
        else if (serialArrayCount == 23) {
          lightOffTimeMin = atoi(buffer);
        }
        else if (serialArrayCount == 24) {
          lightMode = atoi(buffer);
        }
        else if (serialArrayCount == 25) {
          ec = atof(buffer);
        }
        else if (serialArrayCount == 26) {
          tds = atoi(buffer);
        }
        else if (serialArrayCount == 27) {
          ph = atof(buffer);
        }
        else if (serialArrayCount == 28) {
          co2 = atoi(buffer);
        }
        else if (serialArrayCount == 29) {
          waterTemp = atof(buffer);
        }
        else if (serialArrayCount == 30) {
          waterLevel = atof(buffer);
        }
        else if (serialArrayCount == 31) {
          airTemp = atof(buffer);
        }
        else if (serialArrayCount == 32) {
          humidity = atof(buffer);
        }
        else if (serialArrayCount == 33) {
          fanOneSpeed = atoi(buffer);
        }
        else if (serialArrayCount == 34) {
          fanTwoSpeed = atoi(buffer);
        }
        else if (serialArrayCount == 35) {
          targetMinFanOneSpeed = atoi(buffer);
        }
        else if (serialArrayCount == 36) {
          targetMaxFanOneSpeed = atoi(buffer);
        }
        else if (serialArrayCount == 37) {
          targetMinFanTwoSpeed = atoi(buffer);
        }
        else if (serialArrayCount == 38) {
          targetMaxFanTwoSpeed = atoi(buffer);
        }
        else if (serialArrayCount == 39) {
          fansControlTemperature = atoi(buffer);
        }
        else if (serialArrayCount == 40) {
          fansControlHumidity = atoi(buffer);
        }
        else if (serialArrayCount == 41) {
          fanOneFixedSpeed = atoi(buffer);
        }
        else if (serialArrayCount == 42) {
          fanTwoFixedSpeed = atoi(buffer);
        }
        else if (serialArrayCount == 43) {
          targetMinAirTemp = atof(buffer);
        }
        else if (serialArrayCount == 44) {
          targetMaxAirTemp = atof(buffer);
        }
        else if (serialArrayCount == 45) {
          airTempOffset = atof(buffer);
        }
        else if (serialArrayCount == 46) {
          targetMinHumidity = atof(buffer);
        }
        else if (serialArrayCount == 47) {
          targetMaxHumidity = atof(buffer);
        }
        else if (serialArrayCount == 48) {
          humidityOffset = atof(buffer);
        }
        else if (serialArrayCount == 49) {
          targetMinTds = atoi(buffer);
        }
        else if (serialArrayCount == 50) {
          targetMaxTds = atoi(buffer);
        }
        else if (serialArrayCount == 51) {
          tdsOffset = atoi(buffer);
        }
        else if (serialArrayCount == 52) {
          targetCo2 = atoi(buffer);
        }
        else if (serialArrayCount == 53) {
          co2Offset = atoi(buffer);
        }
        else if (serialArrayCount == 54) {
          roomWidth = atoi(buffer);
        }
        else if (serialArrayCount == 55) {
          roomLength = atoi(buffer);
        }
        else if (serialArrayCount == 56) {
          roomHeight = atoi(buffer);
        }
        else if (serialArrayCount == 57) {
          co2Flowrate = atof(buffer);
        }
        else if (serialArrayCount == 58) {
          co2CheckTimeMinute = atoi(buffer);
        }
        else if (serialArrayCount == 59) {
          co2CheckTimeHour = atoi(buffer);
        }
        else if (serialArrayCount == 60) {
          disableFansTimer = atoi(buffer);
        }
        else if (serialArrayCount == 61) {
          manualCo2Duration = atoi(buffer);
        }
        else if (serialArrayCount == 62) {
          manualCo2GasDuration = atoi(buffer);
        }
        else if (serialArrayCount == 63) {
          disableCo2Control = atoi(buffer);
        }
        else if (serialArrayCount == 64) {
          targetMinPh = atof(buffer);
        }
        else if (serialArrayCount == 65) {
          targetMaxPh = atof(buffer);
        }
        else if (serialArrayCount == 66) {
          phOffset = atof(buffer);
        }
        else if (serialArrayCount == 67) {
          targetMinEc = atof(buffer);
        }
        else if (serialArrayCount == 68) {
          targetMaxEc = atof(buffer);
        }
        else if (serialArrayCount == 69) {
          ecOffset = atof(buffer);
        }
        else if (serialArrayCount == 70) {
          targetMinWaterTemp = atof(buffer);
        }
        else if (serialArrayCount == 71) {
          targetMaxWaterTemp = atof(buffer);
        }
        else if (serialArrayCount == 72) {
          waterTempOffset = atof(buffer);
        }
        else if (serialArrayCount == 73) {
          targetMinWaterHeight = atof(buffer);
        }
        else if (serialArrayCount == 74) {
          targetMaxWaterHeight = atof(buffer);
        }
        else if (serialArrayCount == 75) {
          waterHeightOffset = atof(buffer);
        }
        else if (serialArrayCount == 76) {
          minPh = atof(buffer);
        }
        else if (serialArrayCount == 77) {
          maxPh = atof(buffer);
        }
        else if (serialArrayCount == 78) {
          minCo2 = atoi(buffer);
        }
        else if (serialArrayCount == 79) {
          maxCo2 = atoi(buffer);
        }
        else if (serialArrayCount == 80) {
          minEc = atof(buffer);
        }
        else if (serialArrayCount == 81) {
          maxEc = atof(buffer);
        }
        else if (serialArrayCount == 82) {
          minWaterTemp = atof(buffer);
        }
        else if (serialArrayCount == 83) {
          maxWaterTemp = atof(buffer);
        }
        else if (serialArrayCount == 84) {
          minWaterLevel = atof(buffer);
        }
        else if (serialArrayCount == 85) {
          maxWaterLevel = atof(buffer);
        }
        else if (serialArrayCount == 86) {
          minTds = atoi(buffer);
        }
        else if (serialArrayCount == 87) {
          maxTds = atoi(buffer);
        }
        else if (serialArrayCount == 88) {
          minAirTemp = atof(buffer);
        }
        else if (serialArrayCount == 89) {
          maxAirTemp = atof(buffer);
        }
        else if (serialArrayCount == 90) {
          minHumidity = atof(buffer);
        }
        else if (serialArrayCount == 91) {
          maxHumidity = atof(buffer);
        }
        else if (serialArrayCount == 92) {
          minFanOneSpeed = atoi(buffer);
        }
        else if (serialArrayCount == 93) {
          maxFanOneSpeed = atoi(buffer);
        }
        else if (serialArrayCount == 94) {
          minFanTwoSpeed = atoi(buffer);
        }
        else if (serialArrayCount == 95) {
          maxFanTwoSpeed = atoi(buffer);
        }
        else if (serialArrayCount == 96) {
          numberOfDosers = atoi(buffer);
        }
        else if (serialArrayCount == 97) {
          doserOneMode = atoi(buffer);
        }
        else if (serialArrayCount == 98) {
          doserTwoMode = atoi(buffer);
        }
        else if (serialArrayCount == 99) {
          doserThreeMode = atoi(buffer);
        }
        else if (serialArrayCount == 100) {
          doserFourMode = atoi(buffer);
        }
        else if (serialArrayCount == 101) {
          doserFiveMode = atoi(buffer);
        }
        else if (serialArrayCount == 102) {
          doserSixMode = atoi(buffer);
        }
        else if (serialArrayCount == 103) {
          doserOneMills = atoi(buffer);
        }
        else if (serialArrayCount == 104) {
          doserTwoMills = atoi(buffer);
        }
        else if (serialArrayCount == 105) {
          doserThreeMills = atoi(buffer);
        }
        else if (serialArrayCount == 106) {
          doserFourMills = atoi(buffer);
        }
        else if (serialArrayCount == 107) {
          doserFiveMills = atoi(buffer);
        }
        else if (serialArrayCount == 108) {
          doserSixMills = atoi(buffer);
        }
        //else if (serialArrayCount == ?) {
        //  doserOneSpeed = atoi(buffer);
        //}
        //else if (serialArrayCount == ?) {
        //  doserTwoSpeed = atoi(buffer);
        //}
        //else if (serialArrayCount == ?) {
        //  doserThreeSpeed = atoi(buffer);
        //}
        //else if (serialArrayCount == ?) {
        //  doserFourSpeed = atoi(buffer);
        //}
        //else if (serialArrayCount == ?) {
        //  doserFiveSpeed = atoi(buffer);
        //}
        //else if (serialArrayCount == ?) {
        //  doserSixSpeed = atoi(buffer);
        //}
        else if (serialArrayCount == 109) {
          dosingInterval = atoi(buffer);
        }
        else if (serialArrayCount == 110) {
          swapInterval = atoi(buffer);
        }
        else if (serialArrayCount == 111) {
          refillDoserOneMills = atoi(buffer);
        }
        else if (serialArrayCount == 112) {
          refillDoserTwoMills = atoi(buffer);
        }
        else if (serialArrayCount == 113) {
          refillDoserThreeMills = atoi(buffer);
        }
        else if (serialArrayCount == 114) {
          refillDoserFourMills = atoi(buffer);
        }
        else if (serialArrayCount == 115) {
          refillDoserFiveMills = atoi(buffer);
        }
        else if (serialArrayCount == 116) {
          refillDoserSixMills = atoi(buffer);
        }
        else if (serialArrayCount == 117) {
          autoFillHour = atoi(buffer);
        }
        else if (serialArrayCount == 118) {
          autoFillMinute = atoi(buffer);
        }
        else if (serialArrayCount == 119) {
          tdsErrorMargin = atoi(buffer);
        }
        else if (serialArrayCount == 120) {
          ecErrorMargin = atof(buffer);
        }
        else if (serialArrayCount == 121) {
          phErrorMargin = atof(buffer);
        }
        else if (serialArrayCount == 122) {
          co2ErrorMargin = atoi(buffer);
        }
        else if (serialArrayCount == 123) {
          waterTempErrorMargin = atof(buffer);
        }
        else if (serialArrayCount == 124) {
          waterHeightErrorMargin = atof(buffer);
        }
        else if (serialArrayCount == 125) {
          airTempErrorMargin = atof(buffer);
        }
        else if (serialArrayCount == 126) {
          humidityErrorMargin = atof(buffer);
        }
        else if (serialArrayCount == 127) {
          disablePpmWarnings = atoi(buffer);
        }
        else if (serialArrayCount == 128) {
          disableEcWarnings = atoi(buffer);
        }
        else if (serialArrayCount == 129) {
          disableCo2Warnings = atoi(buffer);
        }
        else if (serialArrayCount == 130) {
          disablePhWarnings = atoi(buffer);
        }
        else if (serialArrayCount == 131) {
          disableWaterTempWarnings = atoi(buffer);
        }
        else if (serialArrayCount == 132) {
          disableWaterHeightWarnings = atoi(buffer);
        }
        else if (serialArrayCount == 133) {
          disableAirTempWarnings = atoi(buffer);
        }
        else if (serialArrayCount == 134) {
          disableHumidityWarnings = atoi(buffer);
        }
        else if (serialArrayCount == 135) {
          disableDrainAndRefill = atoi(buffer);
        }
        else if (serialArrayCount == 136) {
          ppmErrorState = atoi(buffer);
        }
        else if (serialArrayCount == 137) {
          ecErrorState = atoi(buffer);
        }
        else if (serialArrayCount == 138) {
          phErrorState = atoi(buffer);
        }
        else if (serialArrayCount == 139) {
          co2ErrorState = atoi(buffer);
        }
        else if (serialArrayCount == 140) {
          waterTempErrorState = atoi(buffer);
        }
        else if (serialArrayCount == 141) {
          waterLevelErrorState = atoi(buffer);
        }
        else if (serialArrayCount == 142) {
          airTempErrorState = atoi(buffer);
        }
        else if (serialArrayCount == 143) {
          humidityErrorState = atoi(buffer);
        }
        else if (serialArrayCount == 144) {
          userProfile = atoi(buffer);
          readingSerialData = false;
          dataLoaded = true;
          if (debugEnabled)
            Serial.println(F("Received data."));
          break;
        }
        buffPos = 0;
        serialArrayCount++;
        for (char& buff : buffer) buff = 0;
      }
      else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '.' || c == '-' || c == '_' || c == '|') {
        if (buffPos < 64)
          buffer[buffPos++] = c;
      }
      ESP.wdtFeed();
      //yield();
    }
  }
  yield();
}

void strcpy_s(char* a_dest, const char* a_src, const int& a_size) {
  for (byte i = 0; i < a_size; i++) {
    a_dest[i] = a_src[i];
  }
}

void printBoolArray(const bool* a_array, const int& a_size) {
  for (byte i = 0; i < a_size; i++) {
    Serial.print(a_array[i]); Serial.print("|");
  }
  Serial.print(",");
  delay(5);
}
