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
              int val = atoi(buffer);
              if (!device::ecValsAdjusted)
                user::targetMinTds = val;
            }
            else if (serialArrayCount == 19 && acceptData) {
              int val = atoi(buffer);
              if (!device::ecValsAdjusted)
                user::targetMaxTds = val;
            }
            else if (serialArrayCount == 20 && acceptData) {
              float val = atof(buffer);
              if (!device::phValsAdjusted)
                user::targetMinPh = val;
            }
            else if (serialArrayCount == 21 && acceptData) {
              float val = atof(buffer);
              if (!device::phValsAdjusted)
                user::targetMaxPh = val;
            }
            else if (serialArrayCount == 22 && acceptData) {
              int val = atoi(buffer);
              if (!device::co2ValsAdjusted)
                user::targetCo2 = val;
            }
            else if (serialArrayCount == 23 && acceptData) {
              int val = atoi(buffer);
              if (!device::co2ValsAdjusted)
                user::co2Offset = val;
            }
            else if (serialArrayCount == 24 && acceptData) {
              int val = atoi(buffer);
              if (!device::co2ValsAdjusted) {
                if (user::convertToInches)
                  user::roomWidthInches = val;
                else
                  user::roomWidthCm = val;
              }
            }
            else if (serialArrayCount == 25 && acceptData) {
              int val = atoi(buffer);
              if (!device::co2ValsAdjusted) {
                if (user::convertToInches)
                  user::roomLengthInches = val;
                else
                  user::roomLengthCm = val;
              }
            }
            else if (serialArrayCount == 26 && acceptData) {
              int val = atoi(buffer);
              if (!device::co2ValsAdjusted) {
                if (user::convertToInches)
                  user::roomHeightInches = val;
                else
                  user::roomHeightCm = val;
              }
            }
            else if (serialArrayCount == 27 && acceptData) {
              float val = atof(buffer);
              if (!device::co2ValsAdjusted) {
                if (user::convertToInches)
                  user::co2FlowrateFeet3 = val;
                else
                  user::co2FlowrateLtrs = val;
              }
            }
            else if (serialArrayCount == 28 && acceptData) {
              int val = atoi(buffer);
              if (!device::co2ValsAdjusted)
                user::co2CheckTimeMinute = val;
            }
            else if (serialArrayCount == 29 && acceptData) {
              int val = atoi(buffer);
              if (!device::co2ValsAdjusted)
                user::co2CheckTimeHour = val;
            }
            else if (serialArrayCount == 30 && acceptData) {
              int val = atoi(buffer);
              if (!device::co2ValsAdjusted)
                user::disableFansTimer = val;
            }
            else if (serialArrayCount == 31 && acceptData) {
              bool val = atoi(buffer);
              if (!device::co2ValsAdjusted)
                user::enableManualCo2Duration = val;
            }
            else if (serialArrayCount == 32 && acceptData) {
              int val = atoi(buffer);
              if (!device::co2ValsAdjusted)
                user::manualCo2GasDuration = val;
            }
            else if (serialArrayCount == 33 && acceptData) {
              bool val = atoi(buffer);
              if (!device::co2ValsAdjusted)
                user::disableCo2Control = val;
            }
            else if (serialArrayCount == 34 && acceptData) {
              float val = atof(buffer);
              if (!device::waterValsAdjusted) {
                if (user::convertToInches)
                  user::targetMinWaterHeightInches = val;
                else
                  user::targetMinWaterHeight = val;
              }
            }
            else if (serialArrayCount == 35 && acceptData) {
              float val = atof(buffer);
              if (!device::waterValsAdjusted) {
                if (user::convertToInches)
                  user::targetMaxWaterHeightInches = val;
                else
                  user::targetMaxWaterHeight = val;
              }
            }
            else if (serialArrayCount == 36 && acceptData) {
              float val = atof(buffer);
              if (!device::waterValsAdjusted) {
                if (user::convertToF)
                  user::targetMinWaterTempF = val;
                else
                  user::targetMinWaterTemp = val;
              }
            }
            else if (serialArrayCount == 37 && acceptData) {
              float val = atof(buffer);
              if (!device::waterValsAdjusted) {
                if (user::convertToF)
                  user::targetMaxWaterTempF = val;
                else
                  user::targetMaxWaterTemp = val;
              }
            }
            else if (serialArrayCount == 38 && acceptData) {
              if (!device::waterValsAdjusted) {
                byte i = 0;
                char* data = strtok(buffer, "|");
                while (data != NULL && i < 31) {
                  user::autoFillDays[i++] = atoi(data);
                  data = strtok(NULL, "|");
                }
              }
            }
            else if (serialArrayCount == 39 && acceptData) {
              int val = atoi(buffer);
              if (!device::waterValsAdjusted)
                user::autoFillMinute = val;
            }
            else if (serialArrayCount == 40 && acceptData) {
              int val = atoi(buffer);
              if (!device::waterValsAdjusted)
                user::autoFillHour = val;
            }
            else if (serialArrayCount == 41 && acceptData) {
              bool val = atoi(buffer);
              if (!device::waterValsAdjusted)
                user::disableDrainAndRefill = val;
            }
            else if (serialArrayCount == 42 && acceptData) {
              int val = atoi(buffer);
              if (!device::waterValsAdjusted)
                user::refillDoserOneMills = val;
            }
            else if (serialArrayCount == 43 && acceptData) {
              int val = atoi(buffer);
              if (!device::waterValsAdjusted)
                user::refillDoserTwoMills = val;
            }
            else if (serialArrayCount == 44 && acceptData) {
              int val = atoi(buffer);
              if (!device::waterValsAdjusted)
                user::refillDoserThreeMills = val;
            }
            else if (serialArrayCount == 45 && acceptData) {
              int val = atoi(buffer);
              if (!device::waterValsAdjusted)
                user::refillDoserFourMills = val;
            }
            else if (serialArrayCount == 46 && acceptData) {
              int val = atoi(buffer);
              if (!device::waterValsAdjusted)
                user::refillDoserFiveMills = val;
            }
            else if (serialArrayCount == 47 && acceptData) {
              int val = atoi(buffer);
              if (!device::waterValsAdjusted)
                user::refillDoserSixMills = val;
            }
            else if (serialArrayCount == 48 && acceptData) {
              byte val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserOneMode = val;
            }
            else if (serialArrayCount == 49 && acceptData) {
              byte val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserTwoMode = val;
            }
            else if (serialArrayCount == 50 && acceptData) {
              byte val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserThreeMode = val;
            }
            else if (serialArrayCount == 51 && acceptData) {
              byte val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserFourMode = val;
            }
            else if (serialArrayCount == 52 && acceptData) {
              byte val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserFiveMode = val;
            }
            else if (serialArrayCount == 53 && acceptData) {
              byte val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserSixMode = val;
            }
            else if (serialArrayCount == 54 && acceptData) {
              int val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserOneMills = val;
            }
            else if (serialArrayCount == 55 && acceptData) {
              int val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserTwoMills = val;
            }
            else if (serialArrayCount == 56 && acceptData) {
              int val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserThreeMills = val;
            }
            else if (serialArrayCount == 57 && acceptData) {
              int val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserFourMills = val;
            }
            else if (serialArrayCount == 58 && acceptData) {
              int val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserFiveMills = val;
            }
            else if (serialArrayCount == 59 && acceptData) {
              int val = atoi(buffer);
              if (!device::doserValsAdjusted)
                user::doserSixMills = val;
            }
            else if (serialArrayCount == 60 && acceptData) {
              byte val = atoi(buffer);
              if (!device::lightValsAdjusted)
                user::lightOnTimeHour = val;
            }
            else if (serialArrayCount == 61 && acceptData) {
              byte val = atoi(buffer);
              if (!device::lightValsAdjusted)
                user::lightOnTimeMin = val;
            }
            else if (serialArrayCount == 62 && acceptData) {
              byte val = atoi(buffer);
              if (!device::lightValsAdjusted)
                user::lightOffTimeHour = val;
            }
            else if (serialArrayCount == 63 && acceptData) {
              byte val = atoi(buffer);
              if (!device::lightValsAdjusted)
                user::lightOffTimeMin = val;
            }
            else if (serialArrayCount == 64 && acceptData) {
              byte val = atoi(buffer);
              if (!device::lightValsAdjusted)
                user::lightMode = val;
            }
            else if (serialArrayCount == 65 && acceptData) {
              byte val = atoi(buffer);
              if (!device::fanValsAdjusted)
                user::targetMinFanOneSpeed = val;
            }
            else if (serialArrayCount == 66 && acceptData) {
              byte val = atoi(buffer);
              if (!device::fanValsAdjusted)
                user::targetMaxFanOneSpeed = val;
            }
            else if (serialArrayCount == 67 && acceptData) {
              byte val = atoi(buffer);
              if (!device::fanValsAdjusted)
                user::targetMinFanTwoSpeed = val;
            }
            else if (serialArrayCount == 68 && acceptData) {
              byte val = atoi(buffer);
              if (!device::fanValsAdjusted)
                user::targetMaxFanTwoSpeed = val;
            }
            else if (serialArrayCount == 69 && acceptData) {
              float val = atof(buffer);
              if (!device::fanValsAdjusted) {
                if (user::convertToF)
                  user::targetMinAirTempF = val;
                else
                  user::targetMinAirTemp = val;
              }
            }
            else if (serialArrayCount == 70 && acceptData) {
              float val = atof(buffer);
              if (!device::fanValsAdjusted) {
                if (user::convertToF)
                  user::targetMaxAirTempF = val;
                else
                  user::targetMaxAirTemp = val;
              }
            }
            else if (serialArrayCount == 71 && acceptData) {
              float val = atof(buffer);
              if (!device::fanValsAdjusted)
                user::targetMinHumidity = val;
            }
            else if (serialArrayCount == 72 && acceptData) {
              float val = atof(buffer);
              if (!device::fanValsAdjusted)
                user::targetMaxHumidity = val;
            }
            else if (serialArrayCount == 73 && acceptData) {
              bool val = atoi(buffer);
              if (!device::fanValsAdjusted)
                user::fansControlTemperature = val;
            }
            else if (serialArrayCount == 74 && acceptData) {
              bool val = atoi(buffer);
              if (!device::fanValsAdjusted)
                user::fansControlHumidity = val;
            }
            else if (serialArrayCount == 75 && acceptData) {
              bool val = atoi(buffer);
              if (!device::fanValsAdjusted)
                user::fanOneFixedSpeed = val;
            }
            else if (serialArrayCount == 76 && acceptData) {
              bool val = atoi(buffer);
              if (!device::fanValsAdjusted)
                user::fanTwoFixedSpeed = val;
            }
            else if (serialArrayCount == 77 && acceptData) {
              int val = atoi(buffer);
              if (!device::warningValsAdjusted)
                user::tdsErrorMargin = val;
            }
            else if (serialArrayCount == 78 && acceptData) {
              float val = atof(buffer);
              if (!device::warningValsAdjusted)
                user::ecErrorMargin = val;
            }
            else if (serialArrayCount == 79 && acceptData) {
              int val = atoi(buffer);
              if (!device::warningValsAdjusted)
                user::tdsErrorMargin = val;
            }
            else if (serialArrayCount == 80 && acceptData) {
              float val = atof(buffer);
              if (!device::warningValsAdjusted)
                user::phErrorMargin = val;
            }
            else if (serialArrayCount == 81 && acceptData) {
              float val = atof(buffer);
              if (!device::warningValsAdjusted) {
                if (user::convertToF)
                  user::waterTempErrorMarginF = val;
                else
                  user::waterTempErrorMargin = val;
              }
            }
            else if (serialArrayCount == 82 && acceptData) {
              float val = atof(buffer);
              if (!device::warningValsAdjusted) {
                if (user::convertToInches)
                  user::waterHeightErrorMarginInches = val;
                else
                  user::waterHeightErrorMargin = val;
              }
            }
            else if (serialArrayCount == 83 && acceptData) {
              float val = atof(buffer);
              if (!device::warningValsAdjusted) {
                if (user::convertToF)
                  user::airTempErrorMarginF = val;
                else
                  user::airTempErrorMargin = val;
              }
            }
            else if (serialArrayCount == 84 && acceptData) {
              float val = atof(buffer);
              if (!device::warningValsAdjusted)
                user::humidityErrorMargin = val;
            }
            else if (serialArrayCount == 85 && acceptData) {
              int val = atoi(buffer);
              if (!device::usernameAdjusted) {
                device::userProfile = val;
                loadUserProfile = val;
              }
            }
            else if (serialArrayCount == 86 && acceptData) {
              int val = atoi(buffer);
              if (!device::usernameAdjusted) {
                device::userProfile = val;
                saveUserProfile = val;
              }
            }
            else if (serialArrayCount == 87 && acceptData) {
              bool val = atoi(buffer);
              if (!device::settingsAdjusted)
                user::convertToF = val;
            }
            else if (serialArrayCount == 88 && acceptData) {
              bool val = atoi(buffer);
              if (!device::settingsAdjusted)
                user::convertToInches = val;
            }
            else if (serialArrayCount == 89) {
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
          else if ((c >= '-' && c <= '9') || (c >= 'A' && c <= '|')) {
            if (buffPos < 64) {
              buffer[buffPos++] = c;
            }
          }
          // Send data to ESP
          if (sendDataBack) {
            Serial.println("Sending data back...");
            // Once data is read send ack back '@'
            Serial1.print("@");
            Serial1.print(user::convertToF); commaDelay(); // 0
            Serial1.print(user::convertToInches); commaDelay(); // 1
            Serial1.print(user::convertToTds); commaDelay(); // 2
            printFloatArray(device::ecArray, 12, 2); // 3
            printFloatArray(device::tdsArray, 12, 0); // 4
            printFloatArray(device::phArray, 12, 2); // 5
            printFloatArray(device::co2Array, 12, 0); // 6
            printFloatArray((user::convertToF ? device::waterTemperatureArrayF : device::waterTemperatureArray), 12, 1); // 7
            printFloatArray((user::convertToInches ? device::waterLevelArrayInInches : device::waterLevelArray), 12, 1); // 8
            printFloatArray((user::convertToF ? device::airTemperatureArrayF : device::airTemperatureArray), 12, 1); // 9
            printFloatArray(device::humidityArray, 12, 1); // 10
            printFloatArray(device::fanOneSpeedArray, 12, 0); // 11
            printFloatArray(device::fanTwoSpeedArray, 12, 0); // 12
            printBoolArray(user::autoFillDays, 31); // 13
            Serial1.print(device::graphArrayPos); commaDelay(); // 14
            Serial1.print(user::profileOneName); commaDelay(); // 15
            Serial1.print(user::profileTwoName); commaDelay(); // 16
            Serial1.print(user::profileThreeName); commaDelay(); // 17
            Serial1.print(user::profileFourName); commaDelay(); // 18
            Serial1.print(user::profileFiveName); commaDelay(); // 19
            Serial1.print(user::lightOnTimeHour); commaDelay(); // 20
            Serial1.print(user::lightOnTimeMin); commaDelay(); // 21
            Serial1.print(user::lightOffTimeHour); commaDelay(); // 22
            Serial1.print(user::lightOffTimeMin); commaDelay(); // 23
            Serial1.print(user::lightMode); commaDelay(); // 24
            Serial1.print(sensor::ec, 2); commaDelay(); // 25
            Serial1.print(sensor::tds); commaDelay(); // 26
            Serial1.print(sensor::ph, 2); commaDelay(); // 27
            Serial1.print(sensor::co2); commaDelay(); // 28
            printEither(sensor::waterTempF, sensor::waterTemp, 1, user::convertToF);// 29
            printEither(sensor::waterLevelInches, sensor::waterLevel, 1, user::convertToInches);// 30
            printEither(convertToF(sensor::airTemp), sensor::airTemp, 1, user::convertToF);// 31
            Serial1.print(sensor::humidity, 1); commaDelay(); // 32
            Serial1.print(device::fanOneSpeed); commaDelay(); // 33
            Serial1.print(device::fanTwoSpeed); commaDelay(); // 34
            Serial1.print(user::targetMinFanOneSpeed); commaDelay(); // 35
            Serial1.print(user::targetMaxFanOneSpeed); commaDelay(); // 36
            Serial1.print(user::targetMinFanTwoSpeed); commaDelay(); // 37
            Serial1.print(user::targetMaxFanTwoSpeed); commaDelay(); // 38
            Serial1.print(user::fansControlTemperature); commaDelay(); // 39
            Serial1.print(user::fansControlHumidity); commaDelay(); // 40
            Serial1.print(user::fanOneFixedSpeed); commaDelay(); // 41
            Serial1.print(user::fanTwoFixedSpeed); commaDelay(); // 42
            printEither(user::targetMinAirTempF, user::targetMinAirTemp, 1, user::convertToF);// 43
            printEither(user::targetMaxAirTempF, user::targetMaxAirTemp, 1, user::convertToF);// 44
            Serial1.print(user::targetMinHumidity, 1); commaDelay(); // 45
            Serial1.print(user::targetMaxHumidity, 1); commaDelay(); // 46
            Serial1.print(user::targetMinTds); commaDelay(); // 47
            Serial1.print(user::targetMaxTds); commaDelay(); // 48
            Serial1.print(user::targetCo2); commaDelay(); // 49
            Serial1.print(user::co2Offset); commaDelay(); // 50
            printEither(user::roomWidthInches, user::roomWidthCm, 1, user::convertToInches);// 51
            printEither(user::roomLengthInches, user::roomLengthCm, 1, user::convertToInches);// 52
            printEither(user::roomHeightInches, user::roomHeightCm, 1, user::convertToInches);// 53
            printEither(user::co2FlowrateFeet3, user::co2FlowrateLtrs, 2, user::convertToInches);// 54
            Serial1.print(user::co2CheckTimeMinute); commaDelay(); // 55
            Serial1.print(user::co2CheckTimeHour); commaDelay(); // 56
            Serial1.print(user::disableFansTimer); commaDelay(); // 57
            Serial1.print(user::enableManualCo2Duration); commaDelay(); // 58
            Serial1.print(user::manualCo2GasDuration); commaDelay(); // 59
            Serial1.print(user::disableCo2Control); commaDelay(); // 60
            Serial1.print(user::targetMinPh, 2); commaDelay(); // 61
            Serial1.print(user::targetMaxPh, 2); commaDelay(); // 63
            Serial1.print(user::targetMinEc, 1); commaDelay(); // 63
            Serial1.print(user::targetMaxEc, 1); commaDelay(); // 64
            printEither(user::targetMinWaterTempF, user::targetMinWaterTemp, 1, user::convertToF);// 65
            printEither(user::targetMaxWaterTempF, user::targetMaxWaterTemp, 1, user::convertToF);// 66
            printEither(user::targetMinWaterHeightInches, user::targetMinWaterHeight, 1, user::convertToInches);// 67
            printEither(user::targetMaxWaterHeightInches, user::targetMaxWaterHeight, 1, user::convertToInches);// 68
            Serial1.print(device::minPh, 2); commaDelay(); // 69
            Serial1.print(device::maxPh, 2); commaDelay(); // 70
            Serial1.print(device::minCo2); commaDelay(); // 71
            Serial1.print(device::maxCo2); commaDelay(); // 72
            Serial1.print(device::minEc, 2); commaDelay(); // 73
            Serial1.print(device::maxEc, 2); commaDelay(); // 74
            printEither(convertToF(device::minWaterTemp), device::minWaterTemp, 1, user::convertToF);// 75
            printEither(convertToF(device::maxWaterTemp), device::maxWaterTemp, 1, user::convertToF);// 76
            printEither(convertToInch(device::minWaterLevel), device::minWaterLevel, 1, user::convertToInches);// 77
            printEither(convertToInch(device::maxWaterLevel), device::maxWaterLevel, 1, user::convertToInches);// 78
            Serial1.print(device::minTds); commaDelay(); // 79
            Serial1.print(device::maxTds); commaDelay(); // 80
            printEither(convertToF(device::minAirTemp), device::minAirTemp, 1, user::convertToF);// 81
            printEither(convertToF(device::maxAirTemp), device::maxAirTemp, 1, user::convertToF);// 82
            Serial1.print(device::minHumidity, 1); commaDelay(); // 83
            Serial1.print(device::maxHumidity, 1); commaDelay(); // 84
            Serial1.print(device::minFanOneSpeed); commaDelay(); // 85
            Serial1.print(device::maxFanOneSpeed); commaDelay(); // 86
            Serial1.print(device::minFanTwoSpeed); commaDelay(); // 87
            Serial1.print(device::maxFanTwoSpeed); commaDelay(); // 88
            Serial1.print(user::numberOfDosers); commaDelay(); // 89
            Serial1.print(user::doserOneMode); commaDelay(); // 90
            Serial1.print(user::doserTwoMode); commaDelay(); // 91
            Serial1.print(user::doserThreeMode); commaDelay(); // 92
            Serial1.print(user::doserFourMode); commaDelay(); //93
            Serial1.print(user::doserFiveMode); commaDelay(); // 94
            Serial1.print(user::doserSixMode); commaDelay(); // 95
            Serial1.print(user::doserOneMills); commaDelay(); // 96
            Serial1.print(user::doserTwoMills); commaDelay(); // 97
            Serial1.print(user::doserThreeMills); commaDelay(); // 98
            Serial1.print(user::doserFourMills); commaDelay(); // 99
            Serial1.print(user::doserFiveMills); commaDelay(); // 100
            Serial1.print(user::doserSixMills); commaDelay(); // 101
            Serial1.print(user::dosingInterval); commaDelay(); // 102
            Serial1.print(user::swapInterval); commaDelay(); // 103
            Serial1.print(user::refillDoserOneMills); commaDelay(); // 104
            Serial1.print(user::refillDoserTwoMills); commaDelay(); // 105
            Serial1.print(user::refillDoserThreeMills); commaDelay(); // 106
            Serial1.print(user::refillDoserFourMills); commaDelay(); // 107
            Serial1.print(user::refillDoserFiveMills); commaDelay(); // 108
            Serial1.print(user::refillDoserSixMills); commaDelay(); // 109
            Serial1.print(user::autoFillHour); commaDelay(); // 110
            Serial1.print(user::autoFillMinute); commaDelay(); // 111
            Serial1.print(user::tdsErrorMargin); commaDelay(); // 112
            Serial1.print(user::ecErrorMargin, 2); commaDelay(); // 113
            Serial1.print(user::phErrorMargin, 2); commaDelay(); // 114
            Serial1.print(user::co2ErrorMargin); commaDelay(); // 115
            printEither(user::waterTempErrorMarginF, user::waterTempErrorMargin, 1, user::convertToF);// 116
            printEither(user::waterHeightErrorMarginInches, user::waterHeightErrorMargin, 1, user::convertToInches);// 117
            printEither(user::airTempErrorMarginF, user::airTempErrorMargin, 1, user::convertToF);// 118
            Serial1.print(user::humidityErrorMargin, 1); commaDelay(); // 119
            Serial1.print(user::disablePpmWarnings); commaDelay(); // 120
            Serial1.print(user::disableEcWarnings); commaDelay(); // 121
            Serial1.print(user::disableCo2Warnings); commaDelay(); // 122
            Serial1.print(user::disablePhWarnings); commaDelay(); // 123
            Serial1.print(user::disableWaterTempWarnings); commaDelay(); // 124
            Serial1.print(user::disableWaterHeightWarnings); commaDelay(); // 125
            Serial1.print(user::disableAirTempWarnings); commaDelay(); // 126
            Serial1.print(user::disableHumidityWarnings); commaDelay(); // 127
            Serial1.print(user::disableDrainAndRefill); commaDelay(); // 128
            Serial1.print(device::ppmErrorState); commaDelay(); // 129
            Serial1.print(device::ecErrorState); commaDelay(); // 130
            Serial1.print(device::phErrorState); commaDelay(); // 131
            Serial1.print(device::co2ErrorState); commaDelay(); // 132
            Serial1.print(device::waterTempErrorState); commaDelay(); // 133
            Serial1.print(device::waterLevelErrorState); commaDelay(); // 134
            Serial1.print(device::airTempErrorState); commaDelay(); // 135
            Serial1.print(device::humidityErrorState); commaDelay(); // 136
            Serial1.print(device::userProfile); commaDelay(); // 137
            Serial1.print(device::systemStatus); commaDelay(); // 138
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

void printEither(const float& a_val, const float& a_val2, const byte& a_precison, const bool& a_convert) {
  if (a_convert)
    Serial1.print(a_val, a_precison);
  else
    Serial1.print(a_val2, a_precison);
  commaDelay();
}

void commaDelay() {
  Serial1.print(",");
  delay(1);
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
