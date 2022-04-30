void espSerial() {
  static byte serialArrayCount = 0;
  static byte buffPos = 0;
  static bool readingSerialData = false;
  static char buffer[64] {0};
  static bool sendDataBack = false;
  static bool acceptData = false;
  bool sendRequest = false;
  bool sendSuccessfull = false;
  int loadUserProfile = -1;
  int saveUserProfile = -1;
  const byte delayTime = 1;

  if (wifi::wifiEnabled && device::sensorsReady) {
    if (millis() - wifi::wifiPreviousMillis >= 2500UL) {
      unsigned long currentTime = millis();
      while (millis() - currentTime < 500UL) {
        // Ask for ESP data
        if (!sendRequest) {
          Serial.println(F("\nAsking for esp data"));
          Serial1.print("!");
          sendRequest = true;
        }
        // Got responce
        if (Serial1.available() > 0) {
          char c = Serial1.read();
          // Read ESP data
          if (c == '#') {
            Serial.println(F("Reading data..."));
            buffPos = 0;
            serialArrayCount = 0;
            for (char& buff : buffer) buff = 0;
            readingSerialData = true;
          }
          else if (c == ',' && readingSerialData) {
            if (serialArrayCount == 0) {
              bool val = atoi(buffer);
              acceptData = val;
            }
            else if (serialArrayCount == 1 && acceptData) {
              if (!device::usernameAdjusted)
                strcpy_s(user::profileOneName, buffer, 11);
            }
            else if (serialArrayCount == 2 && acceptData) {
              if (!device::usernameAdjusted)
                strcpy_s(user::profileTwoName, buffer, 11);
            }
            else if (serialArrayCount == 3 && acceptData) {
              if (!device::usernameAdjusted)
                strcpy_s(user::profileThreeName, buffer, 11);
            }
            else if (serialArrayCount == 4 && acceptData) {
              if (!device::usernameAdjusted)
                strcpy_s(user::profileFourName, buffer, 11);
            }
            else if (serialArrayCount == 5 && acceptData) {
              if (!device::usernameAdjusted)
                strcpy_s(user::profileFiveName, buffer, 11);
            }
            else if (serialArrayCount == 6 && acceptData) {
              int val = atoi(buffer);
              if (!device::settingsAdjusted)
                user::numberOfDosers = val;
            }
            else if (serialArrayCount == 7 && acceptData) {
              int val = atoi(buffer);
              if (!device::settingsAdjusted)
                user::dosingInterval = val;
            }
            else if (serialArrayCount == 8 && acceptData) {
              bool val = atoi(buffer);
              if (!device::settingsAdjusted)
                user::disablePpmWarnings = val;
            }
            else if (serialArrayCount == 9 && acceptData) {
              bool val = atoi(buffer);
              if (!device::settingsAdjusted)
                user::disableEcWarnings = val;
            }
            else if (serialArrayCount == 10 && acceptData) {
              bool val = atoi(buffer);
              if (!device::settingsAdjusted)
                user::disableCo2Warnings = val;
            }
            else if (serialArrayCount == 11 && acceptData) {
              bool val = atoi(buffer);
              if (!device::settingsAdjusted)
                user::disablePhWarnings = val;
            }
            else if (serialArrayCount == 12 && acceptData) {
              bool val = atoi(buffer);
              if (!device::settingsAdjusted)
                user::disableWaterTempWarnings = val;
            }
            else if (serialArrayCount == 13 && acceptData) {
              bool val = atoi(buffer);
              if (!device::settingsAdjusted)
                user::disableWaterHeightWarnings = val;
            }
            else if (serialArrayCount == 14 && acceptData) {
              bool val = atoi(buffer);
              if (!device::settingsAdjusted)
                user::disableAirTempWarnings = val;
            }
            else if (serialArrayCount == 15 && acceptData) {
              bool val = atoi(buffer);
              if (!device::settingsAdjusted)
                user::disableHumidityWarnings = val;
            }
            else if (serialArrayCount == 16 && acceptData) {
              float val = atof(buffer);
              if (!device::ecValsAdjusted)
                user::targetMinEc = val;
            }
            else if (serialArrayCount == 17 && acceptData) {
              float val = atof(buffer);
              if (!device::ecValsAdjusted)
                user::targetMaxEc = val;
            }
            else if (serialArrayCount == 18 && acceptData) {
              float val = atof(buffer);
              if (!device::ecValsAdjusted)
                user::ecOffset = val;
            }
            else if (serialArrayCount == 19 && acceptData) {
              int val = atoi(buffer);
              if (!device::ecValsAdjusted)
                user::targetMinTds = val;
            }
            else if (serialArrayCount == 20 && acceptData) {
              int val = atoi(buffer);
              if (!device::ecValsAdjusted)
                user::targetMaxTds = val;
            }
            else if (serialArrayCount == 21 && acceptData) {
              int val = atoi(buffer);
              if (!device::ecValsAdjusted)
                user::tdsOffset = val;
            }
            else if (serialArrayCount == 22 && acceptData) {
              float val = atof(buffer);
              if (!device::phValsAdjusted)
                user::targetMinPh = val;
            }
            else if (serialArrayCount == 23 && acceptData) {
              float val = atof(buffer);
              if (!device::phValsAdjusted)
                user::targetMaxPh = val;
            }
            else if (serialArrayCount == 24 && acceptData) {
              float val = atof(buffer);
              if (!device::phValsAdjusted)
                user::phOffset = val;
            }
            else if (serialArrayCount == 25 && acceptData) {
              int val = atoi(buffer);
              if (!device::co2ValsAdjusted)
                user::targetCo2 = val;
            }
            else if (serialArrayCount == 26 && acceptData) {
              int val = atoi(buffer);
              if (!device::co2ValsAdjusted)
                user::co2Offset = val;
            }
            else if (serialArrayCount == 27 && acceptData) {
              int val = atoi(buffer);
              if (!device::co2ValsAdjusted) {
                if (user::convertToInches)
                  user::roomWidthInches = val;
                else
                  user::roomWidthCm = val;
              }
            }
            else if (serialArrayCount == 28 && acceptData) {
              int val = atoi(buffer);
              if (!device::co2ValsAdjusted) {
                if (user::convertToInches)
                  user::roomLengthInches = val;
                else
                  user::roomLengthCm = val;
              }
            }
            else if (serialArrayCount == 29 && acceptData) {
              int val = atoi(buffer);
              if (!device::co2ValsAdjusted) {
                if (user::convertToInches)
                  user::roomHeightInches = val;
                else
                  user::roomHeightCm = val;
              }
            }
            else if (serialArrayCount == 30 && acceptData) {
              float val = atof(buffer);
              if (!device::co2ValsAdjusted) {
                if (user::convertToInches)
                  user::co2FlowrateFeet3 = val;
                else
                  user::co2FlowrateLtrs = val;
              }
            }
            else if (serialArrayCount == 31 && acceptData) {
              int val = atoi(buffer);
              if (!device::co2ValsAdjusted)
                user::co2CheckTimeMinute = val;
            }
            else if (serialArrayCount == 32 && acceptData) {
              int val = atoi(buffer);
              if (!device::co2ValsAdjusted)
                user::co2CheckTimeHour = val;
            }
            else if (serialArrayCount == 33 && acceptData) {
              int val = atoi(buffer);
              if (!device::co2ValsAdjusted)
                user::disableFansTimer = val;
            }
            else if (serialArrayCount == 34 && acceptData) {
              bool val = atoi(buffer);
              if (!device::co2ValsAdjusted)
                user::enableManualCo2Duration = val;
            }
            else if (serialArrayCount == 35 && acceptData) {
              int val = atoi(buffer);
              if (!device::co2ValsAdjusted)
                user::manualCo2GasDuration = val;
            }
            else if (serialArrayCount == 36 && acceptData) {
              bool val = atoi(buffer);
              if (!device::co2ValsAdjusted)
                user::disableCo2Control = val;
            }
            else if (serialArrayCount == 37 && acceptData) {
              float val = atof(buffer);
              if (!device::waterValsAdjusted) {
                if (user::convertToInches)
                  user::targetMinWaterHeightInches = val;
                else
                  user::targetMinWaterHeight = val;
              }
            }
            else if (serialArrayCount == 38 && acceptData) {
              float val = atof(buffer);
              if (!device::waterValsAdjusted) {
                if (user::convertToInches)
                  user::targetMaxWaterHeightInches = val;
                else
                  user::targetMaxWaterHeight = val;
              }
            }
            else if (serialArrayCount == 39 && acceptData) {
              float val = atof(buffer);
              if (!device::waterValsAdjusted) {
                if (user::convertToInches)
                  user::waterHeightOffsetInches = val;
                else
                  user::waterHeightOffset = val;
              }
            }
            else if (serialArrayCount == 40 && acceptData) {
              float val = atof(buffer);
              if (!device::waterValsAdjusted) {
                if (user::convertToFarenheit)
                  user::targetMinWaterTempF = val;
                else
                  user::targetMinWaterTemp = val;
              }
            }
            else if (serialArrayCount == 41 && acceptData) {
              float val = atof(buffer);
              if (!device::waterValsAdjusted) {
                if (user::convertToFarenheit)
                  user::targetMaxWaterTempF = val;
                else
                  user::targetMaxWaterTemp = val;
              }
            }
            else if (serialArrayCount == 42 && acceptData) {
              float val = atof(buffer);
              if (!device::waterValsAdjusted) {
                if (user::convertToFarenheit)
                  user::waterTempOffsetF = val;
                else
                  user::waterTempOffset = val;
              }
            }
            else if (serialArrayCount == 43 && acceptData) {
              if (!device::waterValsAdjusted) {
                byte i = 0;
                char* data = strtok(buffer, "|");
                while (data != NULL && i < 31) {
                  user::autoFillDays[i++] = atoi(data);
                  data = strtok(NULL, "|");
                }
              }
            }
            else if (serialArrayCount == 44 && acceptData) {
              int val = atoi(buffer);
              if (!device::waterValsAdjusted)
                user::autoFillMinute = val;
            }
            else if (serialArrayCount == 45 && acceptData) {
              int val = atoi(buffer);
              if (!device::waterValsAdjusted)
                user::autoFillHour = val;
            }
            else if (serialArrayCount == 46 && acceptData) {
              bool val = atoi(buffer);
              if (!device::waterValsAdjusted)
                user::disableDrainAndRefill = val;
            }
            else if (serialArrayCount == 47 && acceptData) {
              int val = atoi(buffer);
              if (!device::waterValsAdjusted)
                user::refillDoserOneMills = val;
            }
            else if (serialArrayCount == 48 && acceptData) {
              int val = atoi(buffer);
              if (!device::waterValsAdjusted)
                user::refillDoserTwoMills = val;
            }
            else if (serialArrayCount == 49 && acceptData) {
              int val = atoi(buffer);
              if (!device::waterValsAdjusted)
                user::refillDoserThreeMills = val;
            }
            else if (serialArrayCount == 50 && acceptData) {
              int val = atoi(buffer);
              if (!device::waterValsAdjusted)
                user::refillDoserFourMills = val;
            }
            else if (serialArrayCount == 51 && acceptData) {
              int val = atoi(buffer);
              if (!device::waterValsAdjusted)
                user::refillDoserFiveMills = val;
            }
            else if (serialArrayCount == 52 && acceptData) {
              int val = atoi(buffer);
              if (!device::waterValsAdjusted)
                user::refillDoserSixMills = val;
            }
            else if (serialArrayCount == 53 && acceptData) {
              byte val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserOneMode = val;
            }
            else if (serialArrayCount == 54 && acceptData) {
              byte val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserTwoMode = val;
            }
            else if (serialArrayCount == 55 && acceptData) {
              byte val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserThreeMode = val;
            }
            else if (serialArrayCount == 56 && acceptData) {
              byte val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserFourMode = val;
            }
            else if (serialArrayCount == 57 && acceptData) {
              byte val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserFiveMode = val;
            }
            else if (serialArrayCount == 58 && acceptData) {
              byte val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserSixMode = val;
            }
            else if (serialArrayCount == 59 && acceptData) {
              int val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserOneMills = val;
            }
            else if (serialArrayCount == 60 && acceptData) {
              int val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserTwoMills = val;
            }
            else if (serialArrayCount == 61 && acceptData) {
              int val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserThreeMills = val;
            }
            else if (serialArrayCount == 62 && acceptData) {
              int val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserFourMills = val;
            }
            else if (serialArrayCount == 63 && acceptData) {
              int val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserFiveMills = val;
            }
            else if (serialArrayCount == 64 && acceptData) {
              int val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserSixMills = val;
            }
            else if (serialArrayCount == 65 && acceptData) {
              byte val = atoi(buffer);
              if (!device::lightValsAdjusted)
                user::lightOnTimeHour = val;
            }
            else if (serialArrayCount == 66 && acceptData) {
              byte val = atoi(buffer);
              if (!device::lightValsAdjusted)
                user::lightOnTimeMin = val;
            }
            else if (serialArrayCount == 67 && acceptData) {
              byte val = atoi(buffer);
              if (!device::lightValsAdjusted)
                user::lightOffTimeHour = val;
            }
            else if (serialArrayCount == 68 && acceptData) {
              byte val = atoi(buffer);
              if (!device::lightValsAdjusted)
                user::lightOffTimeMin = val;
            }
            else if (serialArrayCount == 69 && acceptData) {
              byte val = atoi(buffer);
              if (!device::lightValsAdjusted)
                user::lightMode = val;
            }
            else if (serialArrayCount == 70 && acceptData) {
              byte val = atoi(buffer);
              if (!device::fanValsAdjusted)
                user::targetMinFanOneSpeed = val;
            }
            else if (serialArrayCount == 71 && acceptData) {
              byte val = atoi(buffer);
              if (!device::fanValsAdjusted)
                user::targetMaxFanOneSpeed = val;
            }
            else if (serialArrayCount == 72 && acceptData) {
              byte val = atoi(buffer);
              if (!device::fanValsAdjusted)
                user::targetMinFanTwoSpeed = val;
            }
            else if (serialArrayCount == 73 && acceptData) {
              byte val = atoi(buffer);
              if (!device::fanValsAdjusted)
                user::targetMaxFanTwoSpeed = val;
            }
            else if (serialArrayCount == 74 && acceptData) {
              float val = atof(buffer);
              if (!device::fanValsAdjusted) {
                if (user::convertToFarenheit)
                  user::targetMinAirTempF = val;
                else
                  user::targetMinAirTemp = val;
              }
            }
            else if (serialArrayCount == 75 && acceptData) {
              float val = atof(buffer);
              if (!device::fanValsAdjusted) {
                if (user::convertToFarenheit)
                  user::targetMaxAirTempF = val;
                else
                  user::targetMaxAirTemp = val;
              }
            }
            else if (serialArrayCount == 76 && acceptData) {
              float val = atof(buffer);
              if (!device::fanValsAdjusted && user::airTempOffset >= 0) {
                if (user::convertToFarenheit)
                  user::airTempOffsetF = val;
                else
                  user::airTempOffset = val;
              }
            }
            else if (serialArrayCount == 77 && acceptData) {
              float val = atof(buffer);
              if (!device::fanValsAdjusted)
                user::targetMinHumidity = val;
            }
            else if (serialArrayCount == 78 && acceptData) {
              float val = atof(buffer);
              if (!device::fanValsAdjusted)
                user::targetMaxHumidity = val;
            }
            else if (serialArrayCount == 79 && acceptData) {
              float val = atof(buffer);
              if (!device::fanValsAdjusted)
                user::humidityOffset = val;
            }
            else if (serialArrayCount == 80 && acceptData) {
              bool val = atoi(buffer);
              if (!device::fanValsAdjusted)
                user::fansControlTemperature = val;
            }
            else if (serialArrayCount == 81 && acceptData) {
              bool val = atoi(buffer);
              if (!device::fanValsAdjusted)
                user::fansControlHumidity = val;
            }
            else if (serialArrayCount == 82 && acceptData) {
              bool val = atoi(buffer);
              if (!device::fanValsAdjusted)
                user::fanOneFixedSpeed = val;
            }
            else if (serialArrayCount == 83 && acceptData) {
              bool val = atoi(buffer);
              if (!device::fanValsAdjusted)
                user::fanTwoFixedSpeed = val;
            }
            else if (serialArrayCount == 84 && acceptData) {
              int val = atoi(buffer);
              if (!device::warningValsAdjusted)
                user::tdsErrorMargin = val;
            }
            else if (serialArrayCount == 85 && acceptData) {
              float val = atof(buffer);
              if (!device::warningValsAdjusted)
                user::ecErrorMargin = val;
            }
            else if (serialArrayCount == 86 && acceptData) {
              int val = atoi(buffer);
              if (!device::warningValsAdjusted)
                user::tdsErrorMargin = val;
            }
            else if (serialArrayCount == 87 && acceptData) {
              float val = atof(buffer);
              if (!device::warningValsAdjusted)
                user::phErrorMargin = val;
            }
            else if (serialArrayCount == 88 && acceptData) {
              float val = atof(buffer);
              if (!device::warningValsAdjusted) {
                if (user::convertToFarenheit)
                  user::waterTempErrorMarginF = val;
                else
                  user::waterTempErrorMargin = val;
              }
            }
            else if (serialArrayCount == 89 && acceptData) {
              float val = atof(buffer);
              if (!device::warningValsAdjusted) {
                if (user::convertToInches)
                  user::waterHeightErrorMarginInches = val;
                else
                  user::waterHeightErrorMargin = val;
              }
            }
            else if (serialArrayCount == 90 && acceptData) {
              float val = atof(buffer);
              if (!device::warningValsAdjusted) {
                if (user::convertToFarenheit)
                  user::airTempErrorMarginF = val;
                else
                  user::airTempErrorMargin = val;
              }
            }
            else if (serialArrayCount == 91 && acceptData) {
              float val = atof(buffer);
              if (!device::warningValsAdjusted)
                user::humidityErrorMargin = val;
            }
            else if (serialArrayCount == 92 && acceptData) {
              int val = atoi(buffer);
              if (!device::usernameAdjusted) {
                device::userProfile = val;
                loadUserProfile = val;
              }
            }
            else if (serialArrayCount == 93 && acceptData) {
              int val = atoi(buffer);
              if (!device::usernameAdjusted) {
                device::userProfile = val;
                saveUserProfile = val;
              }
            }
            else if (serialArrayCount == 94 && acceptData) {
              bool val = atoi(buffer);
              if (!device::settingsAdjusted)
                user::convertToFarenheit = val;
            }
            else if (serialArrayCount == 95 && acceptData) {
              bool val = atoi(buffer);
              if (!device::settingsAdjusted)
                user::convertToInches = val;
            }
            else if (serialArrayCount == 96) {
              if (acceptData) {
                bool val = atoi(buffer);
                if (!device::settingsAdjusted)
                  user::convertToTds = val;
                Serial.println(F("Updated variables"));
              }
              sendDataBack = true;
            }
            // reset buufer and increment data counter
            buffPos = 0;
            serialArrayCount++;
            for (char& buff : buffer) buff = 0;
          }
          else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '.' || c == '-' || c == '_' || c == '|') {
            if (buffPos < 64) {
              buffer[buffPos++] = c;
            }
          }
          // Send data to ESP
          if (sendDataBack) {
            Serial.println("Sending data back...");
            // Once data is read send ack back '@'
            Serial1.print("@");
            Serial1.print(user::convertToFarenheit); Serial1.print(","); delay(delayTime); // 0
            Serial1.print(user::convertToInches); Serial1.print(","); delay(delayTime); // 1
            Serial1.print(user::convertToTds); Serial1.print(","); delay(delayTime); // 2
            printFloatArray(device::ecArray, 12, 2); // 3
            printFloatArray(device::tdsArray, 12, 0); // 4
            printFloatArray(device::phArray, 12, 2); // 5
            printFloatArray(device::co2Array, 12, 0); // 6
            if (user::convertToFarenheit)
              printFloatArray(device::waterTemperatureArrayF, 12, 1); // 7
            else
              printFloatArray(device::waterTemperatureArray, 12, 1);
            if (user::convertToInches)
              printFloatArray(device::waterLevelArrayInInches, 12, 1); // 8
            else
              printFloatArray(device::waterLevelArray, 12, 1);
            if (user::convertToFarenheit)
              printFloatArray(device::airTemperatureArrayF, 12, 1); // 9
            else
              printFloatArray(device::airTemperatureArray, 12, 1);
            printFloatArray(device::humidityArray, 12, 1); // 10
            printFloatArray(device::fanOneSpeedArray, 12, 0); // 11
            printFloatArray(device::fanTwoSpeedArray, 12, 0); // 12
            printBoolArray(user::autoFillDays, 31); // 13
            Serial1.print(device::graphArrayPos); Serial1.print(","); delay(delayTime); // 14
            Serial1.print(user::profileOneName); Serial1.print(","); delay(delayTime); // 15
            Serial1.print(user::profileTwoName); Serial1.print(","); delay(delayTime); // 16
            Serial1.print(user::profileThreeName); Serial1.print(","); delay(delayTime); // 17
            Serial1.print(user::profileFourName); Serial1.print(","); delay(delayTime); // 18
            Serial1.print(user::profileFiveName); Serial1.print(","); delay(delayTime); // 19
            Serial1.print(user::lightOnTimeHour); Serial1.print(","); delay(delayTime); // 20
            Serial1.print(user::lightOnTimeMin); Serial1.print(","); delay(delayTime); // 21
            Serial1.print(user::lightOffTimeHour); Serial1.print(","); delay(delayTime); // 22
            Serial1.print(user::lightOffTimeMin); Serial1.print(","); delay(delayTime); // 23
            Serial1.print(user::lightMode); Serial1.print(","); delay(delayTime); // 24
            Serial1.print(sensor::ec, 2); Serial1.print(","); delay(delayTime); // 25
            Serial1.print(sensor::tds); Serial1.print(","); delay(delayTime); // 26
            Serial1.print(sensor::ph, 2); Serial1.print(","); delay(delayTime); // 27
            Serial1.print(sensor::co2); Serial1.print(","); delay(delayTime); // 28
            if (user::convertToFarenheit)
              Serial1.print(sensor::waterTempF, 1); // 29
            else
              Serial1.print(sensor::waterTemp, 1);
            Serial1.print(","); delay(delayTime);

            if (user::convertToInches)
              Serial1.print(sensor::waterLevelInches, 1); // 30
            else
              Serial1.print(sensor::waterLevel, 1);
            Serial1.print(","); delay(delayTime);

            if (user::convertToFarenheit)
              Serial1.print(convertToF(sensor::airTemp), 1); // 31
            else
              Serial1.print(sensor::airTemp, 1);
            Serial1.print(","); delay(delayTime);

            Serial1.print(sensor::humidity, 1); Serial1.print(","); delay(delayTime); // 32
            Serial1.print(device::fanOneSpeed); Serial1.print(","); delay(delayTime); // 33
            Serial1.print(device::fanTwoSpeed); Serial1.print(","); delay(delayTime); // 34
            Serial1.print(user::targetMinFanOneSpeed); Serial1.print(","); delay(delayTime); // 35
            Serial1.print(user::targetMaxFanOneSpeed); Serial1.print(","); delay(delayTime); // 36
            Serial1.print(user::targetMinFanTwoSpeed); Serial1.print(","); delay(delayTime); // 37
            Serial1.print(user::targetMaxFanTwoSpeed); Serial1.print(","); delay(delayTime); // 38
            Serial1.print(user::fansControlTemperature); Serial1.print(","); delay(delayTime); // 39
            Serial1.print(user::fansControlHumidity); Serial1.print(","); delay(delayTime); // 40
            Serial1.print(user::fanOneFixedSpeed); Serial1.print(","); delay(delayTime); // 41
            Serial1.print(user::fanTwoFixedSpeed); Serial1.print(","); delay(delayTime); // 42
            if (user::convertToFarenheit)
              Serial1.print(user::targetMinAirTempF, 1); // 43
            else
              Serial1.print(user::targetMinAirTemp, 1);
            Serial1.print(","); delay(delayTime);
            if (user::convertToFarenheit)
              Serial1.print(user::targetMaxAirTempF, 1); // 44
            else
              Serial1.print(user::targetMaxAirTemp, 1);
            Serial1.print(","); delay(delayTime);
            if (user::convertToFarenheit)
              Serial1.print(user::airTempOffsetF, 1); // 45
            else
              Serial1.print(user::airTempOffset, 1);
            Serial1.print(","); delay(delayTime);
            Serial1.print(user::targetMinHumidity, 1); Serial1.print(","); delay(delayTime); // 46
            Serial1.print(user::targetMaxHumidity, 1); Serial1.print(","); delay(delayTime); // 47
            Serial1.print(user::humidityOffset, 1); Serial1.print(","); delay(delayTime); // 48
            Serial1.print(user::targetMinTds); Serial1.print(","); delay(delayTime); // 49
            Serial1.print(user::targetMaxTds); Serial1.print(","); delay(delayTime); // 50
            Serial1.print(user::tdsOffset); Serial1.print(","); delay(delayTime); // 51
            Serial1.print(user::targetCo2); Serial1.print(","); delay(delayTime); // 52
            Serial1.print(user::co2Offset); Serial1.print(","); delay(delayTime); // 53
            if (user::convertToInches)
              Serial1.print(user::roomWidthInches); // 54
            else
              Serial1.print(user::roomWidthCm);
            Serial1.print(","); delay(delayTime);
            if (user::convertToInches)
              Serial1.print(user::roomLengthInches); // 55
            else
              Serial1.print(user::roomLengthCm);
            Serial1.print(","); delay(delayTime);
            if (user::convertToInches)
              Serial1.print(user::roomHeightInches); // 56
            else
              Serial1.print(user::roomHeightCm);
            Serial1.print(","); delay(delayTime);
            if (user::convertToInches)
              Serial1.print(user::co2FlowrateFeet3, 2); // 57
            else
              Serial1.print(user::co2FlowrateLtrs, 2);
            Serial1.print(","); delay(delayTime);
            Serial1.print(user::co2CheckTimeMinute); Serial1.print(","); delay(delayTime); // 58
            Serial1.print(user::co2CheckTimeHour); Serial1.print(","); delay(delayTime); // 59
            Serial1.print(user::disableFansTimer); Serial1.print(","); delay(delayTime); // 60
            Serial1.print(user::enableManualCo2Duration); Serial1.print(","); delay(delayTime); // 61
            Serial1.print(user::manualCo2GasDuration); Serial1.print(","); delay(delayTime); // 62
            Serial1.print(user::disableCo2Control); Serial1.print(","); delay(delayTime); // 63
            Serial1.print(user::targetMinPh, 2); Serial1.print(","); delay(delayTime); // 64
            Serial1.print(user::targetMaxPh, 2); Serial1.print(","); delay(delayTime); // 65
            Serial1.print(user::phOffset, 2); Serial1.print(","); delay(delayTime); // 66
            Serial1.print(user::targetMinEc, 1); Serial1.print(","); delay(delayTime); // 67
            Serial1.print(user::targetMaxEc, 1); Serial1.print(","); delay(delayTime); // 68
            Serial1.print(user::ecOffset, 1); Serial1.print(","); delay(delayTime); // 69
            if (user::convertToFarenheit)
              Serial1.print(user::targetMinWaterTempF, 1); // 70
            else
              Serial1.print(user::targetMinWaterTemp, 1);
            Serial1.print(","); delay(delayTime);
            if (user::convertToFarenheit)
              Serial1.print(user::targetMaxWaterTempF, 1); // 71
            else
              Serial1.print(user::targetMaxWaterTemp, 1);
            Serial1.print(","); delay(delayTime);
            if (user::convertToFarenheit)
              Serial1.print(user::waterTempOffsetF, 1); // 72
            else
              Serial1.print(user::waterTempOffset, 1);
            Serial1.print(","); delay(delayTime);
            if (user::convertToInches)
              Serial1.print(user::targetMinWaterHeightInches, 1); // 73
            else
              Serial1.print(user::targetMinWaterHeight, 1);
            Serial1.print(","); delay(delayTime);
            if (user::convertToInches)
              Serial1.print(user::targetMaxWaterHeightInches, 1); // 74
            else
              Serial1.print(user::targetMaxWaterHeight, 1);
            Serial1.print(","); delay(delayTime);
            if (user::convertToInches)
              Serial1.print(user::waterHeightOffsetInches, 1); // 75
            else
              Serial1.print(user::waterHeightOffset, 1);
            Serial1.print(","); delay(delayTime);
            Serial1.print(device::minPh, 2); Serial1.print(","); delay(delayTime); // 76
            Serial1.print(device::maxPh, 2); Serial1.print(","); delay(delayTime); // 77
            Serial1.print(device::minCo2); Serial1.print(","); delay(delayTime); // 78
            Serial1.print(device::maxCo2); Serial1.print(","); delay(delayTime); // 79
            Serial1.print(device::minEc, 2); Serial1.print(","); delay(delayTime); // 80
            Serial1.print(device::maxEc, 2); Serial1.print(","); delay(delayTime); // 81
            if (user::convertToFarenheit)
              Serial1.print(convertToF(device::minWaterTemp), 1); // 82
            else
              Serial1.print(device::minWaterTemp, 1);
            Serial1.print(","); delay(delayTime);
            if (user::convertToFarenheit)
              Serial1.print(convertToF(device::maxWaterTemp), 1); // 83
            else
              Serial1.print(device::maxWaterTemp, 1);
            Serial1.print(","); delay(delayTime);
            if (user::convertToInches)
              Serial1.print(convertToInch(device::minWaterLevel), 1); // 84
            else
              Serial1.print(device::minWaterLevel, 1);
            Serial1.print(","); delay(delayTime);
            if (user::convertToInches)
              Serial1.print(convertToInch(device::maxWaterLevel), 1); // 85
            else
              Serial1.print(device::maxWaterLevel, 1);
            Serial1.print(","); delay(delayTime);
            Serial1.print(device::minTds); Serial1.print(","); delay(delayTime); // 86
            Serial1.print(device::maxTds); Serial1.print(","); delay(delayTime); // 87
            if (user::convertToFarenheit)
              Serial1.print(convertToF(device::minAirTemp), 1); // 88
            else
              Serial1.print(device::minAirTemp, 1);
            Serial1.print(","); delay(delayTime);
            if (user::convertToFarenheit)
              Serial1.print(convertToF(device::maxAirTemp), 1); // 89
            else
              Serial1.print(device::maxAirTemp, 1);
            Serial1.print(","); delay(delayTime);
            Serial1.print(device::minHumidity, 1); Serial1.print(","); delay(delayTime); // 90
            Serial1.print(device::maxHumidity, 1); Serial1.print(","); delay(delayTime); // 91
            Serial1.print(device::minFanOneSpeed); Serial1.print(","); delay(delayTime); // 92
            Serial1.print(device::maxFanOneSpeed); Serial1.print(","); delay(delayTime); // 93
            Serial1.print(device::minFanTwoSpeed); Serial1.print(","); delay(delayTime); // 94
            Serial1.print(device::maxFanTwoSpeed); Serial1.print(","); delay(delayTime); // 95
            Serial1.print(user::numberOfDosers); Serial1.print(","); delay(delayTime); // 96
            Serial1.print(user::doserOneMode); Serial1.print(","); delay(delayTime); // 97
            Serial1.print(user::doserTwoMode); Serial1.print(","); delay(delayTime); // 98
            Serial1.print(user::doserThreeMode); Serial1.print(","); delay(delayTime); // 99
            Serial1.print(user::doserFourMode); Serial1.print(","); delay(delayTime); //100
            Serial1.print(user::doserFiveMode); Serial1.print(","); delay(delayTime); // 101
            Serial1.print(user::doserSixMode); Serial1.print(","); delay(delayTime); // 102
            Serial1.print(user::doserOneMills); Serial1.print(","); delay(delayTime); // 103
            Serial1.print(user::doserTwoMills); Serial1.print(","); delay(delayTime); // 104
            Serial1.print(user::doserThreeMills); Serial1.print(","); delay(delayTime); // 105
            Serial1.print(user::doserFourMills); Serial1.print(","); delay(delayTime); // 106
            Serial1.print(user::doserFiveMills); Serial1.print(","); delay(delayTime); // 107
            Serial1.print(user::doserSixMills); Serial1.print(","); delay(delayTime); // 108
            Serial1.print(user::dosingInterval); Serial1.print(","); delay(delayTime); // 109
            Serial1.print(user::swapInterval); Serial1.print(","); delay(delayTime); // 110
            Serial1.print(user::refillDoserOneMills); Serial1.print(","); delay(delayTime); // 111
            Serial1.print(user::refillDoserTwoMills); Serial1.print(","); delay(delayTime); // 112
            Serial1.print(user::refillDoserThreeMills); Serial1.print(","); delay(delayTime); // 113
            Serial1.print(user::refillDoserFourMills); Serial1.print(","); delay(delayTime); // 114
            Serial1.print(user::refillDoserFiveMills); Serial1.print(","); delay(delayTime); // 115
            Serial1.print(user::refillDoserSixMills); Serial1.print(","); delay(delayTime); // 116
            Serial1.print(user::autoFillHour); Serial1.print(","); delay(delayTime); // 117
            Serial1.print(user::autoFillMinute); Serial1.print(","); delay(delayTime); // 118
            Serial1.print(user::tdsErrorMargin); Serial1.print(","); delay(delayTime); // 119
            Serial1.print(user::ecErrorMargin, 2); Serial1.print(","); delay(delayTime); // 120
            Serial1.print(user::phErrorMargin, 2); Serial1.print(","); delay(delayTime); // 121
            Serial1.print(user::co2ErrorMargin); Serial1.print(","); delay(delayTime); // 122
            if (user::convertToFarenheit)
              Serial1.print(user::waterTempErrorMarginF, 1); // 123
            else
              Serial1.print(user::waterTempErrorMargin, 1);
            Serial1.print(","); delay(delayTime);
            if (user::convertToInches)
              Serial1.print(user::waterHeightErrorMarginInches, 1); // 124
            else
              Serial1.print(user::waterHeightErrorMargin, 1);
            Serial1.print(","); delay(delayTime);
            if (user::convertToFarenheit)
              Serial1.print(user::airTempErrorMarginF, 1); // 125
            else
              Serial1.print(user::airTempErrorMargin, 1);
            Serial1.print(","); delay(delayTime);
            Serial1.print(user::humidityErrorMargin, 1); Serial1.print(","); delay(delayTime); // 126
            Serial1.print(user::disablePpmWarnings); Serial1.print(","); delay(delayTime); // 127
            Serial1.print(user::disableEcWarnings); Serial1.print(","); delay(delayTime); // 128
            Serial1.print(user::disableCo2Warnings); Serial1.print(","); delay(delayTime); // 129
            Serial1.print(user::disablePhWarnings); Serial1.print(","); delay(delayTime); // 130
            Serial1.print(user::disableWaterTempWarnings); Serial1.print(","); delay(delayTime); // 131
            Serial1.print(user::disableWaterHeightWarnings); Serial1.print(","); delay(delayTime); // 132
            Serial1.print(user::disableAirTempWarnings); Serial1.print(","); delay(delayTime); // 133
            Serial1.print(user::disableHumidityWarnings); Serial1.print(","); delay(delayTime); // 134
            Serial1.print(user::disableDrainAndRefill); Serial1.print(","); delay(delayTime); // 135
            Serial1.print(device::ppmErrorState); Serial1.print(","); delay(delayTime); // 136
            Serial1.print(device::ecErrorState); Serial1.print(","); delay(delayTime); // 137
            Serial1.print(device::phErrorState); Serial1.print(","); delay(delayTime); // 138
            Serial1.print(device::co2ErrorState); Serial1.print(","); delay(delayTime); // 139
            Serial1.print(device::waterTempErrorState); Serial1.print(","); delay(delayTime); // 140
            Serial1.print(device::waterLevelErrorState); Serial1.print(","); delay(delayTime); // 141
            Serial1.print(device::airTempErrorState); Serial1.print(","); delay(delayTime); // 142
            Serial1.print(device::humidityErrorState); Serial1.print(","); delay(delayTime); // 143
            Serial1.print(device::userProfile); Serial1.print(","); delay(delayTime); // 144
            device::usernameAdjusted = false;
            device::settingsAdjusted = false;
            device::ecValsAdjusted = false;
            device::phValsAdjusted = false;
            device::co2ValsAdjusted = false;
            device::waterValsAdjusted = false;
            device::lightValsAdjusted = false;
            device::doserValsAdjusted = false;
            device::fanValsAdjusted = false;
            device::warningValsAdjusted = false;
            sendSuccessfull = true;
            sendDataBack = false;
            Serial.print(F("Data sent\n"));
            break;
          }
        }
      }
      if (!sendSuccessfull) {
        Serial.print(F("Serial request for ESP8266 data timed out at value cnt: ")); Serial.println(serialArrayCount);
      }
      else {
        if (loadUserProfile != -1) {
          Serial.print(F("Got cmd to loadUserProfile: ")); Serial.println(loadUserProfile);
          loadUserEEPROM(loadUserProfile);
        }
        else if (saveUserProfile != -1) {
          Serial.print(F("Got cmd to saveUserEEPROM: ")); Serial.println(saveUserProfile);
          saveUserEEPROM(saveUserProfile);
          saveSystemEEPROM();
        }
      }
      wifi::wifiPreviousMillis = millis();
    }
  }
}

void strcpy_s(char* a_dest, const char* a_src, const int& a_size) {
  for (byte i = 0; i < a_size; i++) {
    a_dest[i] = a_src[i];
  }
}

void printIntArray(const int* a_array, const int& a_size) {
  for (int i = 0; i < a_size; i++) {
    Serial1.print(a_array[i]);
    Serial1.print("|");
  }
  Serial1.print(",");
  delay(5);
}

void printFloatArray(const float* a_array, const int& a_size, const byte& a_pre) {
  for (int i = 0; i < a_size; i++) {
    Serial1.print(a_array[i], a_pre);
    Serial1.print("|");
  }
  Serial1.print(",");
  delay(5);
}

void printBoolArray(const bool* a_array, const int& a_size) {
  for (int i = 0; i < a_size; i++) {
    Serial1.print(a_array[i]);
    Serial1.print("|");
  }
  Serial1.print(",");
  delay(5);
}
