// Protocol
// 1: Arduino sends '!' to request data
// 2: ESP sends '#' to confirm request, followed by CSV data
// 3: Arduino sends '@' to confirm data collection, followed by CSV data

void handleSerial1Data () {
  static byte Serial1ArrayCount = 0;
  static byte buffPos = 0;
  static bool receivedSerial1Request = false;
  static bool readingSerial1Data = false;
  static bool dataLoaded = false;
  static char buffer[64] {};
  const byte delayTime = 1;
  if (Serial1.available() > 0) {
    //Serial1.println(F("Got Serial1Stream"));
    unsigned long currentTime = millis();
    while (millis() - currentTime < 500UL) {
      char c = Serial1.read();
      if (c == '!') {
        if (debugEnabled)
          Serial.println(F("\nReceived data request.\nSending data..."));
        buffPos = 0;
        Serial1ArrayCount = 0;
        for (char& buff : buffer) buff = 0;
        readingSerial1Data = false;
        receivedSerial1Request = true;
        Serial1.print("#");
        // dataLoaded when true the mega2560 will overwrite any unadjusted local variables
        Serial1.print(dataLoaded); Serial1.print(","); delay(delayTime);
        // profiles page
        Serial1.print(profileOneName); Serial1.print(","); delay(delayTime); // 0
        Serial1.print(profileTwoName); Serial1.print(","); delay(delayTime);
        Serial1.print(profileThreeName); Serial1.print(","); delay(delayTime);
        Serial1.print(profileFourName); Serial1.print(","); delay(delayTime);
        Serial1.print(profileFiveName); Serial1.print(","); delay(delayTime);
        // settings page
        Serial1.print(numberOfDosers); Serial1.print(","); delay(delayTime);
        Serial1.print(dosingInterval); Serial1.print(","); delay(delayTime);
        Serial1.print(disablePpmWarnings); Serial1.print(","); delay(delayTime);
        Serial1.print(disableEcWarnings); Serial1.print(","); delay(delayTime);
        Serial1.print(disableCo2Warnings); Serial1.print(","); delay(delayTime);
        Serial1.print(disablePhWarnings); Serial1.print(","); delay(delayTime);
        Serial1.print(disableWaterTempWarnings); Serial1.print(","); delay(delayTime);
        Serial1.print(disableWaterHeightWarnings); Serial1.print(","); delay(delayTime);
        Serial1.print(disableAirTempWarnings); Serial1.print(","); delay(delayTime);
        Serial1.print(disableHumidityWarnings); Serial1.print(","); delay(delayTime);
        // ec-tds page
        Serial1.print(targetMinEc, 2); Serial1.print(","); delay(delayTime);
        Serial1.print(targetMaxEc, 2); Serial1.print(","); delay(delayTime);
        Serial1.print(ecOffset, 2); Serial1.print(","); delay(delayTime);
        Serial1.print(targetMinTds); Serial1.print(","); delay(delayTime);
        Serial1.print(targetMaxTds); Serial1.print(","); delay(delayTime);
        Serial1.print(tdsOffset); Serial1.print(","); delay(delayTime);
        // ph page
        Serial1.print(targetMinPh, 2); Serial1.print(","); delay(delayTime);
        Serial1.print(targetMaxPh, 2); Serial1.print(","); delay(delayTime);
        Serial1.print(phOffset, 2); Serial1.print(","); delay(delayTime);
        // co2 page
        Serial1.print(targetCo2); Serial1.print(","); delay(delayTime);
        Serial1.print(co2Offset); Serial1.print(","); delay(delayTime);
        Serial1.print(roomWidth); Serial1.print(","); delay(delayTime);
        Serial1.print(roomLength); Serial1.print(","); delay(delayTime);
        Serial1.print(roomHeight); Serial1.print(","); delay(delayTime);
        Serial1.print(co2Flowrate, 1); Serial1.print(","); delay(delayTime);
        Serial1.print(co2CheckTimeMinute); Serial1.print(","); delay(delayTime);
        Serial1.print(co2CheckTimeHour); Serial1.print(","); delay(delayTime);
        Serial1.print(disableFansTimer); Serial1.print(","); delay(delayTime);
        Serial1.print(manualCo2Duration); Serial1.print(","); delay(delayTime);
        Serial1.print(manualCo2GasDuration); Serial1.print(","); delay(delayTime);
        Serial1.print(disableCo2Control); Serial1.print(","); delay(delayTime);
        // water page
        Serial1.print(targetMinWaterHeight, 1); Serial1.print(","); delay(delayTime);
        Serial1.print(targetMaxWaterHeight, 1); Serial1.print(","); delay(delayTime);
        Serial1.print(waterHeightOffset, 1); Serial1.print(","); delay(delayTime);
        Serial1.print(targetMinWaterTemp, 1); Serial1.print(","); delay(delayTime);
        Serial1.print(targetMaxWaterTemp, 1); Serial1.print(","); delay(delayTime);
        Serial1.print(waterTempOffset, 1); Serial1.print(","); delay(delayTime);
        printBoolArray(autoFillDays, 31);
        Serial1.print(autoFillMinute); Serial1.print(","); delay(delayTime);
        Serial1.print(autoFillHour); Serial1.print(","); delay(delayTime);
        Serial1.print(disableDrainAndRefill); Serial1.print(","); delay(delayTime);
        Serial1.print(refillDoserOneMills); Serial1.print(","); delay(delayTime);
        Serial1.print(refillDoserTwoMills); Serial1.print(","); delay(delayTime);
        Serial1.print(refillDoserThreeMills); Serial1.print(","); delay(delayTime);
        Serial1.print(refillDoserFourMills); Serial1.print(","); delay(delayTime);
        Serial1.print(refillDoserFiveMills); Serial1.print(","); delay(delayTime);
        Serial1.print(refillDoserSixMills); Serial1.print(","); delay(delayTime);
        // doser page
        Serial1.print(doserOneMode); Serial1.print(","); delay(delayTime);
        Serial1.print(doserTwoMode); Serial1.print(","); delay(delayTime);
        Serial1.print(doserThreeMode); Serial1.print(","); delay(delayTime);
        Serial1.print(doserFourMode); Serial1.print(","); delay(delayTime);
        Serial1.print(doserFiveMode); Serial1.print(","); delay(delayTime);
        Serial1.print(doserSixMode); Serial1.print(","); delay(delayTime);
        Serial1.print(doserOneMills); Serial1.print(","); delay(delayTime);
        Serial1.print(doserTwoMills); Serial1.print(","); delay(delayTime);
        Serial1.print(doserThreeMills); Serial1.print(","); delay(delayTime);
        Serial1.print(doserFourMills); Serial1.print(","); delay(delayTime);
        Serial1.print(doserFiveMills); Serial1.print(","); delay(delayTime);
        Serial1.print(doserSixMills); Serial1.print(","); delay(delayTime);
        //Serial1.print(doserOneSpeed); Serial1.print(","); delay(delayTime);
        //Serial1.print(doserTwoSpeed); Serial1.print(","); delay(delayTime);
        //Serial1.print(doserThreeSpeed); Serial1.print(","); delay(delayTime);
        //Serial1.print(doserFourSpeed); Serial1.print(","); delay(delayTime);
        //Serial1.print(doserFiveSpeed); Serial1.print(","); delay(delayTime);
        //Serial1.print(doserSixSpeed); Serial1.print(","); delay(delayTime);
        //Serial1.print(swapInterval); Serial1.print(","); delay(delayTime);
        // light page
        Serial1.print(lightOnTimeHour); Serial1.print(","); delay(delayTime);
        Serial1.print(lightOnTimeMin); Serial1.print(","); delay(delayTime);
        Serial1.print(lightOffTimeHour); Serial1.print(","); delay(delayTime);
        Serial1.print(lightOffTimeMin); Serial1.print(","); delay(delayTime);
        Serial1.print(lightMode); Serial1.print(","); delay(delayTime);
        // fans page
        Serial1.print(targetMinFanOneSpeed); Serial1.print(","); delay(delayTime);
        Serial1.print(targetMaxFanOneSpeed); Serial1.print(","); delay(delayTime);
        Serial1.print(targetMinFanTwoSpeed); Serial1.print(","); delay(delayTime);
        Serial1.print(targetMaxFanTwoSpeed); Serial1.print(","); delay(delayTime);
        Serial1.print(targetMinAirTemp, 1); Serial1.print(","); delay(delayTime);
        Serial1.print(targetMaxAirTemp, 1); Serial1.print(","); delay(delayTime);
        Serial1.print(airTempOffset, 1); Serial1.print(","); delay(delayTime);
        Serial1.print(targetMinHumidity, 1); Serial1.print(","); delay(delayTime);
        Serial1.print(targetMaxHumidity, 1); Serial1.print(","); delay(delayTime);
        Serial1.print(humidityOffset, 1); Serial1.print(","); delay(delayTime);
        Serial1.print(fansControlTemperature); Serial1.print(","); delay(delayTime);
        Serial1.print(fansControlHumidity); Serial1.print(","); delay(delayTime);
        Serial1.print(fanOneFixedSpeed); Serial1.print(","); delay(delayTime);
        Serial1.print(fanTwoFixedSpeed); Serial1.print(","); delay(delayTime);
        // warnings page
        Serial1.print(tdsErrorMargin); Serial1.print(","); delay(delayTime);
        Serial1.print(ecErrorMargin, 2); Serial1.print(","); delay(delayTime);
        Serial1.print(phErrorMargin, 2); Serial1.print(","); delay(delayTime);
        Serial1.print(co2ErrorMargin); Serial1.print(","); delay(delayTime);
        Serial1.print(waterTempErrorMargin, 1); Serial1.print(","); delay(delayTime);
        Serial1.print(waterHeightErrorMargin, 1); Serial1.print(","); delay(delayTime);
        Serial1.print(airTempErrorMargin, 1); Serial1.print(","); delay(delayTime);
        Serial1.print(humidityErrorMargin, 1); Serial1.print(","); delay(delayTime);
        // system
        Serial1.print(loadUserProfile); Serial1.print(","); delay(delayTime);
        Serial1.print(saveUserProfile); Serial1.print(","); delay(delayTime);
        // user settings but they must be the last values sent
        Serial1.print(convertToFarenheit); Serial1.print(","); delay(delayTime);
        Serial1.print(convertToInchs); Serial1.print(","); delay(delayTime);
        Serial1.print(convertToTds); Serial1.print(","); delay(delayTime);
        loadUserProfile = -1;
        saveUserProfile = -1;
      }
      else if (c == '@' && receivedSerial1Request) {
        // reset Serial1 helpers
        buffPos = 0;
        Serial1ArrayCount = 0;
        for (char& buff : buffer) buff = 0;
        //saveWebData = false;
        readingSerial1Data = true;
        receivedSerial1Request = false;
        if (debugEnabled)
          Serial.println(F("Received confirmation.\nAwaiting data..."));
      }
      else if (c == ',' && readingSerial1Data) {
        // if (debugEnabled) {
        //Serial1.print(F("Got val: ")); Serial1.print(buffer); Serial1.print(F(" Serial1ArrayCount: ")); Serial1.println(Serial1ArrayCount);
        // }
        if (Serial1ArrayCount == 0) {
          convertToFarenheit = atoi(buffer);
        }
        else if (Serial1ArrayCount == 1) {
          convertToInchs = atoi(buffer);
        }
        else if (Serial1ArrayCount == 2) {
          convertToTds = atoi(buffer);
        }
        else if (Serial1ArrayCount == 3) {
          byte i = 0;
          char* data = strtok(buffer, "|");
          while (data != NULL && i < maxGraphArrayValues) {
            ecArray[i++] = atof(data);
            data = strtok(NULL, "|");
          }
        }
        else if (Serial1ArrayCount == 4) {
          byte i = 0;
          char* data = strtok(buffer, "|");
          while (data != NULL && i < maxGraphArrayValues) {
            tdsArray[i++] = atof(data);
            data = strtok(NULL, "|");
          }
        }
        else if (Serial1ArrayCount == 5) {
          byte i = 0;
          char* data = strtok(buffer, "|");
          while (data != NULL && i < maxGraphArrayValues) {
            phArray[i++] = atof(data);
            data = strtok(NULL, "|");
          }
        }
        else if (Serial1ArrayCount == 6) {
          byte i = 0;
          char* data = strtok(buffer, "|");
          while (data != NULL && i < maxGraphArrayValues) {
            co2Array[i++] = atof(data);
            data = strtok(NULL, "|");
          }
        }
        else if (Serial1ArrayCount == 7) {
          byte i = 0;
          char* data = strtok(buffer, "|");
          while (data != NULL && i < maxGraphArrayValues) {
            waterTempArray[i++] = atof(data);
            data = strtok(NULL, "|");
          }
        }
        else if (Serial1ArrayCount == 8) {
          byte i = 0;
          char* data = strtok(buffer, "|");
          while (data != NULL && i < maxGraphArrayValues) {
            waterLevelArray[i++] = atof(data);
            data = strtok(NULL, "|");
          }
        }
        else if (Serial1ArrayCount == 9) {
          byte i = 0;
          char* data = strtok(buffer, "|");
          while (data != NULL && i < maxGraphArrayValues) {
            airTempArray[i++] = atof(data);
            data = strtok(NULL, "|");
          }
        }
        else if (Serial1ArrayCount == 10) {
          byte i = 0;
          char* data = strtok(buffer, "|");
          while (data != NULL && i < maxGraphArrayValues) {
            humidityArray[i++] = atof(data);
            data = strtok(NULL, "|");
          }
        }
        else if (Serial1ArrayCount == 11) {
          byte i = 0;
          char* data = strtok(buffer, "|");
          while (data != NULL && i < maxGraphArrayValues) {
            fanOneSpeedArray[i++] = atof(data);
            data = strtok(NULL, "|");
          }
        }
        else if (Serial1ArrayCount == 12) {
          byte i = 0;
          char* data = strtok(buffer, "|");
          while (data != NULL && i < maxGraphArrayValues) {
            fanTwoSpeedArray[i++] = atof(data);
            data = strtok(NULL, "|");
          }
        }
        else if (Serial1ArrayCount == 13) {
          byte i = 0;
          char* data = strtok(buffer, "|");
          while (data != NULL && i < 31) {
            autoFillDays[i++] = atoi(data);
            data = strtok(NULL, "|");
          }
        }
        else if (Serial1ArrayCount == 14) {
          graphArrayPos = atoi(buffer);
        }
        else if (Serial1ArrayCount == 15) {
          strcpy_s(profileOneName, buffer, 11);
        }
        else if (Serial1ArrayCount == 16) {
          strcpy_s(profileTwoName, buffer, 11);
        }
        else if (Serial1ArrayCount == 17) {
          strcpy_s(profileThreeName, buffer, 11);
        }
        else if (Serial1ArrayCount == 18) {
          strcpy_s(profileFourName, buffer, 11);
        }
        else if (Serial1ArrayCount == 19) {
          strcpy_s(profileFiveName, buffer, 11);
        }
        else if (Serial1ArrayCount == 20) {
          lightOnTimeHour = atoi(buffer);
        }
        else if (Serial1ArrayCount == 21) {
          lightOnTimeMin = atoi(buffer);
        }
        else if (Serial1ArrayCount == 22) {
          lightOffTimeHour = atoi(buffer);
        }
        else if (Serial1ArrayCount == 23) {
          lightOffTimeMin = atoi(buffer);
        }
        else if (Serial1ArrayCount == 24) {
          lightMode = atoi(buffer);
        }
        else if (Serial1ArrayCount == 25) {
          ec = atof(buffer);
        }
        else if (Serial1ArrayCount == 26) {
          tds = atoi(buffer);
        }
        else if (Serial1ArrayCount == 27) {
          ph = atof(buffer);
        }
        else if (Serial1ArrayCount == 28) {
          co2 = atoi(buffer);
        }
        else if (Serial1ArrayCount == 29) {
          waterTemp = atof(buffer);
        }
        else if (Serial1ArrayCount == 30) {
          waterLevel = atof(buffer);
        }
        else if (Serial1ArrayCount == 31) {
          airTemp = atof(buffer);
        }
        else if (Serial1ArrayCount == 32) {
          humidity = atof(buffer);
        }
        else if (Serial1ArrayCount == 33) {
          fanOneSpeed = atoi(buffer);
        }
        else if (Serial1ArrayCount == 34) {
          fanTwoSpeed = atoi(buffer);
        }
        else if (Serial1ArrayCount == 35) {
          targetMinFanOneSpeed = atoi(buffer);
        }
        else if (Serial1ArrayCount == 36) {
          targetMaxFanOneSpeed = atoi(buffer);
        }
        else if (Serial1ArrayCount == 37) {
          targetMinFanTwoSpeed = atoi(buffer);
        }
        else if (Serial1ArrayCount == 38) {
          targetMaxFanTwoSpeed = atoi(buffer);
        }
        else if (Serial1ArrayCount == 39) {
          fansControlTemperature = atoi(buffer);
        }
        else if (Serial1ArrayCount == 40) {
          fansControlHumidity = atoi(buffer);
        }
        else if (Serial1ArrayCount == 41) {
          fanOneFixedSpeed = atoi(buffer);
        }
        else if (Serial1ArrayCount == 42) {
          fanTwoFixedSpeed = atoi(buffer);
        }
        else if (Serial1ArrayCount == 43) {
          targetMinAirTemp = atof(buffer);
        }
        else if (Serial1ArrayCount == 44) {
          targetMaxAirTemp = atof(buffer);
        }
        else if (Serial1ArrayCount == 45) {
          airTempOffset = atof(buffer);
        }
        else if (Serial1ArrayCount == 46) {
          targetMinHumidity = atof(buffer);
        }
        else if (Serial1ArrayCount == 47) {
          targetMaxHumidity = atof(buffer);
        }
        else if (Serial1ArrayCount == 48) {
          humidityOffset = atof(buffer);
        }
        else if (Serial1ArrayCount == 49) {
          targetMinTds = atoi(buffer);
        }
        else if (Serial1ArrayCount == 50) {
          targetMaxTds = atoi(buffer);
        }
        else if (Serial1ArrayCount == 51) {
          tdsOffset = atoi(buffer);
        }
        else if (Serial1ArrayCount == 52) {
          targetCo2 = atoi(buffer);
        }
        else if (Serial1ArrayCount == 53) {
          co2Offset = atoi(buffer);
        }
        else if (Serial1ArrayCount == 54) {
          roomWidth = atoi(buffer);
        }
        else if (Serial1ArrayCount == 55) {
          roomLength = atoi(buffer);
        }
        else if (Serial1ArrayCount == 56) {
          roomHeight = atoi(buffer);
        }
        else if (Serial1ArrayCount == 57) {
          co2Flowrate = atof(buffer);
        }
        else if (Serial1ArrayCount == 58) {
          co2CheckTimeMinute = atoi(buffer);
        }
        else if (Serial1ArrayCount == 59) {
          co2CheckTimeHour = atoi(buffer);
        }
        else if (Serial1ArrayCount == 60) {
          disableFansTimer = atoi(buffer);
        }
        else if (Serial1ArrayCount == 61) {
          manualCo2Duration = atoi(buffer);
        }
        else if (Serial1ArrayCount == 62) {
          manualCo2GasDuration = atoi(buffer);
        }
        else if (Serial1ArrayCount == 63) {
          disableCo2Control = atoi(buffer);
        }
        else if (Serial1ArrayCount == 64) {
          targetMinPh = atof(buffer);
        }
        else if (Serial1ArrayCount == 65) {
          targetMaxPh = atof(buffer);
        }
        else if (Serial1ArrayCount == 66) {
          phOffset = atof(buffer);
        }
        else if (Serial1ArrayCount == 67) {
          targetMinEc = atof(buffer);
        }
        else if (Serial1ArrayCount == 68) {
          targetMaxEc = atof(buffer);
        }
        else if (Serial1ArrayCount == 69) {
          ecOffset = atof(buffer);
        }
        else if (Serial1ArrayCount == 70) {
          targetMinWaterTemp = atof(buffer);
        }
        else if (Serial1ArrayCount == 71) {
          targetMaxWaterTemp = atof(buffer);
        }
        else if (Serial1ArrayCount == 72) {
          waterTempOffset = atof(buffer);
        }
        else if (Serial1ArrayCount == 73) {
          targetMinWaterHeight = atof(buffer);
        }
        else if (Serial1ArrayCount == 74) {
          targetMaxWaterHeight = atof(buffer);
        }
        else if (Serial1ArrayCount == 75) {
          waterHeightOffset = atof(buffer);
        }
        else if (Serial1ArrayCount == 76) {
          minPh = atof(buffer);
        }
        else if (Serial1ArrayCount == 77) {
          maxPh = atof(buffer);
        }
        else if (Serial1ArrayCount == 78) {
          minCo2 = atoi(buffer);
        }
        else if (Serial1ArrayCount == 79) {
          maxCo2 = atoi(buffer);
        }
        else if (Serial1ArrayCount == 80) {
          minEc = atof(buffer);
        }
        else if (Serial1ArrayCount == 81) {
          maxEc = atof(buffer);
        }
        else if (Serial1ArrayCount == 82) {
          minWaterTemp = atof(buffer);
        }
        else if (Serial1ArrayCount == 83) {
          maxWaterTemp = atof(buffer);
        }
        else if (Serial1ArrayCount == 84) {
          minWaterLevel = atof(buffer);
        }
        else if (Serial1ArrayCount == 85) {
          maxWaterLevel = atof(buffer);
        }
        else if (Serial1ArrayCount == 86) {
          minTds = atoi(buffer);
        }
        else if (Serial1ArrayCount == 87) {
          maxTds = atoi(buffer);
        }
        else if (Serial1ArrayCount == 88) {
          minAirTemp = atof(buffer);
        }
        else if (Serial1ArrayCount == 89) {
          maxAirTemp = atof(buffer);
        }
        else if (Serial1ArrayCount == 90) {
          minHumidity = atof(buffer);
        }
        else if (Serial1ArrayCount == 91) {
          maxHumidity = atof(buffer);
        }
        else if (Serial1ArrayCount == 92) {
          minFanOneSpeed = atoi(buffer);
        }
        else if (Serial1ArrayCount == 93) {
          maxFanOneSpeed = atoi(buffer);
        }
        else if (Serial1ArrayCount == 94) {
          minFanTwoSpeed = atoi(buffer);
        }
        else if (Serial1ArrayCount == 95) {
          maxFanTwoSpeed = atoi(buffer);
        }
        else if (Serial1ArrayCount == 96) {
          numberOfDosers = atoi(buffer);
        }
        else if (Serial1ArrayCount == 97) {
          doserOneMode = atoi(buffer);
        }
        else if (Serial1ArrayCount == 98) {
          doserTwoMode = atoi(buffer);
        }
        else if (Serial1ArrayCount == 99) {
          doserThreeMode = atoi(buffer);
        }
        else if (Serial1ArrayCount == 100) {
          doserFourMode = atoi(buffer);
        }
        else if (Serial1ArrayCount == 101) {
          doserFiveMode = atoi(buffer);
        }
        else if (Serial1ArrayCount == 102) {
          doserSixMode = atoi(buffer);
        }
        else if (Serial1ArrayCount == 103) {
          doserOneMills = atoi(buffer);
        }
        else if (Serial1ArrayCount == 104) {
          doserTwoMills = atoi(buffer);
        }
        else if (Serial1ArrayCount == 105) {
          doserThreeMills = atoi(buffer);
        }
        else if (Serial1ArrayCount == 106) {
          doserFourMills = atoi(buffer);
        }
        else if (Serial1ArrayCount == 107) {
          doserFiveMills = atoi(buffer);
        }
        else if (Serial1ArrayCount == 108) {
          doserSixMills = atoi(buffer);
        }
        //else if (Serial1ArrayCount == ?) {
        //  doserOneSpeed = atoi(buffer);
        //}
        //else if (Serial1ArrayCount == ?) {
        //  doserTwoSpeed = atoi(buffer);
        //}
        //else if (Serial1ArrayCount == ?) {
        //  doserThreeSpeed = atoi(buffer);
        //}
        //else if (Serial1ArrayCount == ?) {
        //  doserFourSpeed = atoi(buffer);
        //}
        //else if (Serial1ArrayCount == ?) {
        //  doserFiveSpeed = atoi(buffer);
        //}
        //else if (Serial1ArrayCount == ?) {
        //  doserSixSpeed = atoi(buffer);
        //}
        else if (Serial1ArrayCount == 109) {
          dosingInterval = atoi(buffer);
        }
        else if (Serial1ArrayCount == 110) {
          swapInterval = atoi(buffer);
        }
        else if (Serial1ArrayCount == 111) {
          refillDoserOneMills = atoi(buffer);
        }
        else if (Serial1ArrayCount == 112) {
          refillDoserTwoMills = atoi(buffer);
        }
        else if (Serial1ArrayCount == 113) {
          refillDoserThreeMills = atoi(buffer);
        }
        else if (Serial1ArrayCount == 114) {
          refillDoserFourMills = atoi(buffer);
        }
        else if (Serial1ArrayCount == 115) {
          refillDoserFiveMills = atoi(buffer);
        }
        else if (Serial1ArrayCount == 116) {
          refillDoserSixMills = atoi(buffer);
        }
        else if (Serial1ArrayCount == 117) {
          autoFillHour = atoi(buffer);
        }
        else if (Serial1ArrayCount == 118) {
          autoFillMinute = atoi(buffer);
        }
        else if (Serial1ArrayCount == 119) {
          tdsErrorMargin = atoi(buffer);
        }
        else if (Serial1ArrayCount == 120) {
          ecErrorMargin = atof(buffer);
        }
        else if (Serial1ArrayCount == 121) {
          phErrorMargin = atof(buffer);
        }
        else if (Serial1ArrayCount == 122) {
          co2ErrorMargin = atoi(buffer);
        }
        else if (Serial1ArrayCount == 123) {
          waterTempErrorMargin = atof(buffer);
        }
        else if (Serial1ArrayCount == 124) {
          waterHeightErrorMargin = atof(buffer);
        }
        else if (Serial1ArrayCount == 125) {
          airTempErrorMargin = atof(buffer);
        }
        else if (Serial1ArrayCount == 126) {
          humidityErrorMargin = atof(buffer);
        }
        else if (Serial1ArrayCount == 127) {
          disablePpmWarnings = atoi(buffer);
        }
        else if (Serial1ArrayCount == 128) {
          disableEcWarnings = atoi(buffer);
        }
        else if (Serial1ArrayCount == 129) {
          disableCo2Warnings = atoi(buffer);
        }
        else if (Serial1ArrayCount == 130) {
          disablePhWarnings = atoi(buffer);
        }
        else if (Serial1ArrayCount == 131) {
          disableWaterTempWarnings = atoi(buffer);
        }
        else if (Serial1ArrayCount == 132) {
          disableWaterHeightWarnings = atoi(buffer);
        }
        else if (Serial1ArrayCount == 133) {
          disableAirTempWarnings = atoi(buffer);
        }
        else if (Serial1ArrayCount == 134) {
          disableHumidityWarnings = atoi(buffer);
        }
        else if (Serial1ArrayCount == 135) {
          disableDrainAndRefill = atoi(buffer);
        }
        else if (Serial1ArrayCount == 136) {
          ppmErrorState = atoi(buffer);
        }
        else if (Serial1ArrayCount == 137) {
          ecErrorState = atoi(buffer);
        }
        else if (Serial1ArrayCount == 138) {
          phErrorState = atoi(buffer);
        }
        else if (Serial1ArrayCount == 139) {
          co2ErrorState = atoi(buffer);
        }
        else if (Serial1ArrayCount == 140) {
          waterTempErrorState = atoi(buffer);
        }
        else if (Serial1ArrayCount == 141) {
          waterLevelErrorState = atoi(buffer);
        }
        else if (Serial1ArrayCount == 142) {
          airTempErrorState = atoi(buffer);
        }
        else if (Serial1ArrayCount == 143) {
          humidityErrorState = atoi(buffer);
        }
        else if (Serial1ArrayCount == 144) {
          userProfile = atoi(buffer);
          readingSerial1Data = false;
          dataLoaded = true;
          if (debugEnabled)
            Serial.println(F("Received data."));
            buildArgs();
          break;
        }
        buffPos = 0;
        Serial1ArrayCount++;
        for (char& buff : buffer) buff = 0;
      }
      else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '.' || c == '-' || c == '_' || c == '|') {
        if (buffPos < 64)
          buffer[buffPos++] = c;
      }
      //ESP.wdtFeed();
      //yield();
    }
  }
  //yield();
}

void strcpy_s(char* a_dest, const char* a_src, const int& a_size) {
  for (byte i = 0; i < a_size; i++) {
    a_dest[i] = a_src[i];
  }
}

void printBoolArray(const bool* a_array, const int& a_size) {
  for (byte i = 0; i < a_size; i++) {
    Serial1.print(a_array[i]); Serial1.print("|");
  }
  Serial1.print(",");
  delay(5);
}

void printValues() {
  Serial.print("convertToFarenheit: "); Serial.println(convertToFarenheit);
  Serial.print("convertToInches: "); Serial.println(convertToInchs);
  Serial.print("convertToTds: "); Serial.println(convertToTds);
  Serial.print("FOOBAR: "); Serial.println(1234);
  Serial.print("FOOBAR: "); Serial.println(1234);
  Serial.print("FOOBAR: "); Serial.println(1234);
  Serial.print("FOOBAR: "); Serial.println(1234);
  Serial.print("FOOBAR: "); Serial.println(1234);
  Serial.print("FOOBAR: "); Serial.println(1234);
  Serial.print("FOOBAR: "); Serial.println(1234);
  Serial.print("FOOBAR: "); Serial.println(1234);
  Serial.print("FOOBAR: "); Serial.println(1234);
  Serial.print("FOOBAR: "); Serial.println(1234);
  Serial.print("FOOBAR: "); Serial.println(1234);
  Serial.print("FOOBAR: "); Serial.println(1234);
  Serial.print("FOOBAR: "); Serial.println(1234);
  Serial.print("FOOBAR: "); Serial.println(1234);
}
