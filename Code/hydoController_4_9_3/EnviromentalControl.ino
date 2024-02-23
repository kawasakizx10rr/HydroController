void envriomentalControl() {
 // Get current date time
 static uint32_t previousMillis = millis();
  if (millis() - previousMillis >= 1000UL) {
    rtc.refresh();
    previousMillis = millis();
  }
  lightingControl();
  cyclicTimers();
  waterLevelControl();
  waterTemperatureControl();
  co2Control();
  airControl();
  waterEcPhControl();
}

// Control the water level
void waterLevelControl() {
  bool startDraining = false, startRefilling = false;
  static uint8_t previousDate = 0;
  uint8_t continueRefilling = device::NOT_SET;
  uint8_t continueDraining = device::NOT_SET;

  if (device::sensorsReady) {
    // auto drain and refill tank on set dates and time
    if (!user::disableDrainAndRefill && rtc.day() != previousDate) {
      for (uint8_t i = 0; i < 31; i++) {
        if (bitRead(user::autoFillDays, i) && rtc.day() == i + 1 && rtc.hour() == user::autoFillHour && rtc.minute() >= user::autoFillMinute) {
          if (device::globalDebug)
            Serial.println(F("Auto drain time matched, about to start the auto drain process"));
          saveLogMessage(1);
          startDraining = true;
          previousDate = rtc.day();
          break;
        }
      }
    }

    // Show a dialog asking the user if they want to cancel the auto drain process before it starts
    if (startDraining) {
      launchDrainNotification(continueDraining, startDraining);
      display::lastTouchMillis = millis();
    }

    // Show a dialog asking the user if they want to abort the drain process while its draining
    if (startDraining && continueDraining != device::CANCEL) {
      if (device::globalDebug)
        Serial.println(F("About to show the drain abort dialog, and start draining"));
      const uint16_t waterTarget = user::convertToInches ? user::targetMinWaterHeight / 2.5 : user::targetMinWaterHeight;
      abortMessage(message::cancelDraining, user::convertToInches ? "\"" : "cm", waterTarget, -1, -1, 1, false);
    }

    // Start draining the water from the tank
    int16_t previousWaterLevel = sensor::waterLevel;
    device::prevMillis = millis();
    while (startDraining && continueDraining != device::CANCEL) {
      if (sensor::waterLevel > 0) { // drain tank till empty
        // Turn drain pump relay on
        digitalWrite(pin::outletPump, !device::relayOffState);
        // timer checking water level is still decresing else bail after 1 minute * drainTimeout
        if (millis() - device::prevMillis >= 60000UL * user::drainTimeout) { // put timer value in settings GUI !
          digitalWrite(pin::outletPump, device::relayOffState);
          startDraining = false;
          startRefilling = true;
          if (device::globalDebug)
            Serial.println(F("Failed to pump any further water out of the tank, quiting drain process"));
        }
        int16_t waterHeight = sensor::emptyWaterTankDepth - getWaterHeight();
        if (waterHeight >= 0)
          sensor::waterLevel = waterHeight;

        if (sensor::waterLevel < previousWaterLevel) {
          previousWaterLevel = sensor::waterLevel;
          device::prevMillis = millis();
        }
      }
      else {
        if (device::globalDebug)
          Serial.println(F("Draining complete"));
        digitalWrite(pin::outletPump, device::relayOffState);
        startDraining = false;
        startRefilling = true;
      }

      // cancel button touch event for abort draining dialog
      const uint16_t startX = 166, startY = 166;
      if (tft.touched()) {
        tft.touchReadPixel(&display::touch_x, &display::touch_y);
        if (millis() - display::lastTouchMillis >= 5000UL) {
          if (display::touch_x >= startX + 200 && display::touch_x <= startX + 400 && display::touch_y >= startY + 200 && display::touch_y <= startY + 250) { // Cancel
            if (device::globalDebug)
              Serial.println(F("Water drain aborted"));
            digitalWrite(pin::outletPump, device::relayOffState);
            beep();
            startDraining = false;
            clearPage();
            display::refreshPage = true;
          }
        }
      }
      updateCyclicTimers(); 
    }

    // refill tank when water is low at any date or time
    if (!user::disableDrainAndRefill && !startRefilling) {
      static uint32_t previousDelayMillis = millis(); // give the user 5 minutes to disable "refill When Low" else if the water level is below min this will keep showing a dialog over and over..
      if (millis() - previousDelayMillis >= 300000UL) {
        if ((sensor::waterLevel < user::targetMinWaterHeight && !user::convertToInches) || (convertToInches(sensor::waterLevel) < user::targetMinWaterHeightInches && user::convertToInches)) {
          startRefilling = true;
          if (device::globalDebug)
            Serial.println(F("The water level is below the min target, starting refill process"));
        }
        previousDelayMillis = millis();
      }
    }

    // Show a dialog asking the user if they want to cancel the refill process before it starts
    launchRefillNotification(startRefilling, continueRefilling);

    // Show a dialog asking the user if they want to abort the refill process while its refilling
    if (startRefilling && continueRefilling != device::CANCEL) {
      if (device::globalDebug)
        Serial.println(F("About to show refill abort dialog, and refill tank"));
      const uint16_t waterTarget = user::convertToInches ? user::targetMaxWaterHeightInches : user::targetMaxWaterHeight;
      abortMessage(message::cancelRefilling, user::convertToInches ? "\"" : "cm", waterTarget, -1, -1, 1, false);
      saveLogMessage(2);

      previousWaterLevel = sensor::waterLevel;
      display::lastTouchMillis = millis();
      device::prevMillis = millis();
      while (startRefilling && continueRefilling != device::CANCEL) {
        if (!refillTank(device::prevMillis, previousWaterLevel)) {
          startRefilling = false;
          clearPage();
          display::refreshPage = true;
        }

      }
    }
  }
}

// Refill the tanks water and run doers all available dosers
bool refillTank(uint32_t& a_previousMillis, int16_t& a_previousWaterLevel) {
  static bool startRefilling = true;
  static bool lockPump = false;
  static bool enabledDosers[6] {false, false, false, false, false, false};
  const float waterLevel = user::convertToInches ? convertToInches(sensor::waterLevel) : sensor::waterLevel;
  const uint16_t waterTarget = user::convertToInches ? user::targetMaxWaterHeightInches : user::targetMaxWaterHeight;

  if (waterLevel < waterTarget && startRefilling) {
    // turn on the inlet water pump
    digitalWrite(pin::inletPump, !device::relayOffState);
    // timer checking water level is still decresing else bail after 1 minute * drainTimeout
    if (millis() - a_previousMillis >= 60000UL * user::drainTimeout) {
      if (device::globalDebug)
        Serial.println(F("Failed to pump in any further water, quiting refill process and starting dosing"));
      digitalWrite(pin::inletPump, device::relayOffState);
      startRefilling = false;
    }
    else if (waterLevel >= waterTarget) {
      if (device::globalDebug)
        Serial.println(F("Finished pumping in water, quiting refill process and starting dosing"));
      digitalWrite(pin::inletPump, device::relayOffState);
      startRefilling = false;
    }
  }

  // get current water height
  sensor::waterLevel = sensor::emptyWaterTankDepth - getWaterHeight();
  if (sensor::waterLevel < a_previousWaterLevel) {
    a_previousWaterLevel = sensor::waterLevel;
    a_previousMillis = millis();
  }
  else if (!startRefilling && !lockPump) {
    if (device::globalDebug)
      Serial.println(F("Refilling complete, starting dosing"));
    for (uint8_t i = 0; i < user::numberOfDosers; i++)
      enabledDosers[i] = true;
    lockPump = true;
  }

  // run dosers
  if (lockPump) {
    if (enabledDosers[0] && user::doserOneMode != device::DOSER_OFF)
      enabledDosers[0] = runDoser(1, pin::doserOne, user::doserOneSpeed, user::refillDoserOneMills);
    else if (enabledDosers[1] && user::doserTwoMode != device::DOSER_OFF)
      enabledDosers[1] = runDoser(2, pin::doserTwo, user::doserTwoSpeed, user::refillDoserTwoMills);
    else if (enabledDosers[2] && user::doserThreeMode != device::DOSER_OFF)
      enabledDosers[2] = runDoser(3, pin::doserThree, user::doserThreeSpeed, user::refillDoserThreeMills);
    else if (enabledDosers[3] && user::doserFourMode != device::DOSER_OFF)
      enabledDosers[3] = runDoser(4, pin::doserFour, user::doserFourSpeed, user::refillDoserFourMills);
    else if (enabledDosers[4] && user::doserFiveMode != device::DOSER_OFF)
      enabledDosers[4] = runDoser(5, pin::doserFive, user::doserFiveSpeed, user::refillDoserFiveMills);
    else if (enabledDosers[5] && user::doserSixMode != device::DOSER_OFF)
      enabledDosers[5] = runDoser(6, pin::doserSix, user::doserSixSpeed, user::refillDoserSixMills);
    else {
      if (device::globalDebug)
        Serial.println(F("Dosing is complete"));
      lockPump = false;
      device::dosingTimerHourCounter = 0;
      return false;
    }
  }

  // touch event - cancel button
  const uint16_t startX = 166, startY = 166;
  if (tft.touched()) {
    tft.touchReadPixel(&display::touch_x, &display::touch_y);
    if (millis() - display::lastTouchMillis >= 3000UL) {
      if (display::touch_x >= startX + 200 && display::touch_x <= startX + 400 && display::touch_y >= startY + 200 && display::touch_y <= startY + 250) { // Cancel
        // digitalWrite(pin::inletPump, device::relayOffState);
        // Serial.println(F("Water refill and or dosing aborted"));
        digitalWrite(pin::doserOne, LOW);
        digitalWrite(pin::doserTwo, LOW);
        digitalWrite(pin::doserThree, LOW);
        digitalWrite( pin::doserFour, LOW);
        digitalWrite( pin::doserFive, LOW);
        digitalWrite( pin::doserSix, LOW);
        beep();
        return false;
      }
    }
  }

  return true;
}

// Control the water temperature
void waterTemperatureControl() {
  // Control the water heater
  float waterTemp = sensor::waterTemp;
  float minTemp = user::targetMinWaterTemp;
  static uint8_t prevWaterHeaterMin = 69;
  if (rtc.minute() != prevWaterHeaterMin) {
    if (user::convertToF) {
      waterTemp = convertToF(sensor::waterTemp);
      minTemp = user::targetMinWaterTempF;
    } 
    if (waterTemp <= minTemp && !device::waterHeaterIsOn) {
      device::waterHeaterIsOn = true;
      if (device::globalDebug)
        Serial.println(F("Water heater on"));
      digitalWrite(pin::waterHeater, !device::relayOffState);
      saveLogMessage(13); // save log message, water heater on
    }
    else if (waterTemp > minTemp && device::waterHeaterIsOn) {
      device::waterHeaterIsOn = false;
      if (device::globalDebug)
        Serial.println(F("Water heater off"));
      digitalWrite(pin::waterHeater, device::relayOffState);
      saveLogMessage(14); // save log message, water heater off
    }
    prevWaterHeaterMin = rtc.minute();
  }
}

// Control the external lighting
void lightingControl() {
  using namespace user;
  static uint8_t prviousLightMode = 3;
  if (lightMode == 0) { // timer mode
    if (lightOnTimeHour == rtc.hour() && lightOnTimeMin == rtc.minute() && !device::lightOn) {
      digitalWrite(pin::light, !device::relayOffState);
      if (device::globalDebug)
        Serial.println(F("Light on"));
      device::lightOn = true;
      device::lightSwitchedOnHour = rtc.hour();
      device::lightSwitchedOnMin = rtc.minute();
      saveLogMessage(4);
    }
    else if (lightOffTimeHour == rtc.hour() && lightOffTimeMin == rtc.minute() && device::lightOn) {
      digitalWrite(pin::light, device::relayOffState);
      if (device::globalDebug)
        Serial.println(F("Light off"));
      device::lightOn = false;
      saveLogMessage(5);
    }
  }
  else if (lightMode == 1 && lightMode != prviousLightMode) { // constant on
    digitalWrite(pin::light, !device::relayOffState);
    if (device::globalDebug)
      Serial.println(F("Light const on"));
    device::lightOn = true;
    device::lightSwitchedOnHour = rtc.hour();
    device::lightSwitchedOnMin = rtc.minute();
  }
  else if (lightMode == 2 && lightMode != prviousLightMode) { // constant off
    if (device::globalDebug)
      Serial.println(F("Light const off"));
    digitalWrite(pin::light, device::relayOffState);
    device::lightOn = false;
  }
  prviousLightMode = lightMode;
  // Log any occourance of the light turning off when it should be on "once per day"
  static int16_t previousLightOnDay = 0;
  if (device::lightOn && previousLightOnDay != rtc.day()) {
    if (device::lightSwitchedOnMin <= 57 && rtc.minute() >= device::lightSwitchedOnMin + 2) {
      if (sensor::ldr < 1000)
        saveLogMessage(6);
    }
    else if (device::lightSwitchedOnMin == 58 && rtc.minute() == 0)  {
      if (sensor::ldr < 1000)
        saveLogMessage(6);
    }
    else if (device::lightSwitchedOnMin > 58 && rtc.minute() >= 2)  {
      if (sensor::ldr < 1000)
        saveLogMessage(6);
    }
    previousLightOnDay = rtc.day();
  }
}

// Check if the current time is in between the users start and end time
bool restartLightingTimer() {
  using namespace user;
  int16_t startMinutesSinceMidnight = lightOnTimeMin + 60 * lightOnTimeHour;
  int16_t endMinutesSinceMidnight = lightOnTimeHour + 60 * lightOffTimeHour;
  int16_t currentMinutesSinceMidnight = rtc.minute() + 60 * rtc.hour();
  if (startMinutesSinceMidnight < endMinutesSinceMidnight) {
    return ((currentMinutesSinceMidnight >= startMinutesSinceMidnight) && (currentMinutesSinceMidnight < endMinutesSinceMidnight));
  }
  else {
    return !((currentMinutesSinceMidnight >= endMinutesSinceMidnight) && (currentMinutesSinceMidnight < startMinutesSinceMidnight));
  }
}

// called in while loops
void updateCyclicTimers() {
  static uint32_t previousMillis = millis();
  if (millis() - previousMillis >= 1000UL) {
    rtc.refresh();
    cyclicTimers();
    previousMillis = millis();
  }  
}

void cyclicTimers() {
  // Aux relay 1
  if (user::auxRelayOneMode == 0) {
    int16_t offHour = device::auxRelayOneSwitchedOnHour + (int16_t)(user::auxRelayOneTimer / 60);
    int16_t offMin =  device::auxRelayOneSwitchedOnMin + user::auxRelayOneTimer % 60;
    if (offMin >= 60) {
      offHour++;
      offMin -= 60;
    }
    if (offHour >= 24 && rtc.day() != device::auxRelayOneSwitchedOnDay) {
      offHour -= 24;
      if (rtc.minute() == offMin && rtc.hour() == offHour) {
        device::auxRelayOneOn = !device::auxRelayOneOn;
        device::auxRelayOneSwitchedOnHour = rtc.hour();
        device::auxRelayOneSwitchedOnMin = rtc.minute();
        digitalWrite(pin::auxRelayOnePin, device::auxRelayOneOn);
        if (device::globalDebug) {
          Serial.print(F("Aux relay 1 turned ")); device::auxRelayOneOn != device::relayOffState ? Serial.println(F("on")) : Serial.println(F("off"));
        }         
      }
    }
    else {
      if (rtc.minute() == offMin && rtc.hour() == offHour) {
        device::auxRelayOneOn = !device::auxRelayOneOn;
        device::auxRelayOneSwitchedOnHour = rtc.hour();
        device::auxRelayOneSwitchedOnMin = rtc.minute();
        digitalWrite(pin::auxRelayOnePin, device::auxRelayOneOn);
        if (device::globalDebug) {
          Serial.print(F("Aux relay 1 turned ")); device::auxRelayOneOn != device::relayOffState ? Serial.println(F("on")) : Serial.println(F("off"));
        }
      }
    }
  }
  // If aux relay 2 is set to auto mode check timer
  if (user::auxRelayTwoMode == 0) {
    int16_t offHour = device::auxRelayTwoSwitchedOnHour + (int16_t)(user::auxRelayTwoTimer / 60);
    int16_t offMin =  device::auxRelayTwoSwitchedOnMin + user::auxRelayTwoTimer % 60;
    if (offMin >= 60) {
      offHour++;
      offMin -= 60;
    }
    if (offHour >= 24 && rtc.day() != device::auxRelayTwoSwitchedOnDay) {
      offHour -= 24;
      if (rtc.minute() == offMin && rtc.hour() == offHour) {
        device::auxRelayTwoOn = !device::auxRelayTwoOn;
        device::auxRelayTwoSwitchedOnHour = rtc.hour();
        device::auxRelayTwoSwitchedOnMin = rtc.minute();
        digitalWrite(pin::auxRelayTwoPin, device::auxRelayTwoOn);
        if (device::globalDebug) {
          Serial.print(F("Aux relay 2 turned ")); device::auxRelayTwoOn ? Serial.println(F("on")) : Serial.println(F("off"));
        }         
      }
    }
    else {
      if (rtc.minute() == offMin && rtc.hour() == offHour) {
        device::auxRelayTwoOn = !device::auxRelayTwoOn;
        device::auxRelayTwoSwitchedOnHour = rtc.hour();
        device::auxRelayTwoSwitchedOnMin = rtc.minute();
        digitalWrite(pin::auxRelayTwoPin, device::auxRelayTwoOn);
        if (device::globalDebug) {
          Serial.print(F("Aux relay 2 turned ")); device::auxRelayTwoOn ? Serial.println(F("on")) : Serial.println(F("off"));
        }
      }
    }
  }
}

// controls the enviroments co2 levels
void co2Control() {
  bool startCo2Relay = false;
  static uint8_t previousDate = 0;
  uint8_t continueCo2Control = device::NOT_SET;
  sensor::co2GasTime = 0;
  if (device::sensorsReady && !user::disableCo2Control) {
    // Check if it is time to start the Co2 adjustment
    if (user::co2CheckTimeHour == rtc.hour() && user::co2CheckTimeMinute >= rtc.minute() && rtc.day() != previousDate) {
      // set the gas time, manual time or calculated
      if (sensor::co2 < (user::targetCo2 - user::co2Offset)) {
        if (user::enableManualCo2Duration)
          sensor::co2GasTime = user::manualCo2GasDuration * 60000UL;
        else
          generateCo2GasTime();
      }
      // start Co2 adjustment
      if (sensor::co2GasTime > 0) {
        // show a notification asking the user if they want to cancel the Co2 adjustment before it starts
        launchCo2Notification(continueCo2Control, previousDate);
        // If not canceled by the user continue with Co2 adjustment
        if (continueCo2Control != device::CANCEL) {
          // if the user has disableFansTimer > 0 then turn off the fans for x minutes
          if (user::disableFansTimer > 0) {
            setTimer(device::co2TurnFansBackOnMinute, device::co2TurnFansBackOnHour, user::disableFansTimer);
            sendToSlave('Z', 0);
            sendToSlave('X', 0);
            device::co2DisabledFans = true;
          }
          previousDate = rtc.day();
          startCo2Relay = true;
          // turn on the Co2 solinode
          digitalWrite(pin::co2Solenoid, !device::relayOffState);
          if (device::globalDebug)
            Serial.println(F("About to show Co2 abort dialog, and start pumping Co2"));
          // Show a dialog asking the user if they want to abort the Co2 adjustment while its pumping
          continueMessage(message::cancelCo2, sensor::co2GasTime / 60000, 0, false, true, false);
          display::lastTouchMillis = millis();
        }
      }
    }
    device::prevMillis = millis();
    while (startCo2Relay) {
      if (abortCo2Notification(display::lastTouchMillis, sensor::co2GasTime / 60000))
        break;
      if (millis() - device::prevMillis >= 1000UL) {
        if (sensor::co2GasTime >= 1000UL)
          sensor::co2GasTime -= 1000UL;
        else {
          sensor::co2GasTime = 0;
          break;
        }
        device::prevMillis = millis();
      }
      updateCyclicTimers();
    }
    digitalWrite(pin::co2Solenoid, device::relayOffState);
  }
}

// Control one or two 3rd party AC fans and a 3rd party AC air heater
// to maintain the air temperature and or humidity.
void airControl() {
  static uint32_t previousMillis = millis();
  static float previousFanOneSpeed = 200;
  static float previousFanTwoSpeed = 200;
  float tempPercent = 0, humPercent = 0;
  const bool fanDebug = false; // device::globalDebug; // debug hidden for now while testing other functions

  if (device::sensorsReady && millis() - previousMillis >= 2000UL) {
    if (fanDebug)   
      Serial.println(__FUNCTION__);
    // If Co2 has disabled the fans for x durations, check to see if we can turn the fans back on else do nothing
    if (device::co2DisabledFans && device::co2TurnFansBackOnHour == rtc.hour() && device::co2TurnFansBackOnMinute >= rtc.minute())
      device::co2DisabledFans = false;
    else if (device::co2DisabledFans)
      return;

    // Check to see if the temperature and or humidity is out of range of the user targets
    device::controlOptions tempOption = device::SPEED_IDLE;
    device::controlOptions humOption = device::SPEED_IDLE;
    device::controlOptions fanMode = device::SPEED_IDLE;
    const float maxAirTemp = user::convertToF ? user::targetMaxAirTempF : user::targetMaxAirTemp;
    const float minAirTemp = user::convertToF ? user::targetMinAirTempF : user::targetMinAirTemp;
    const float airTemp = user::convertToF ? convertToF(sensor::airTemp) : sensor::airTemp;
    // Check to see if the temperature is out of range
    if (airTemp > maxAirTemp && user::fansControlTemperature) {
      float tempOutOfRange = abs(maxAirTemp - airTemp); 
      tempPercent = tempOutOfRange / (maxAirTemp / 100.0);
    }
    else if (airTemp < minAirTemp && user::fansControlTemperature) {
      float tempOutOfRange = abs(minAirTemp - airTemp);
      tempPercent = tempOutOfRange / (minAirTemp / 100.0);
      //Serial.println(F("SPEED_DOWN tempPercent: %d", tempPercent);
    }

    // Check to see if the humidity is out of range
    if (sensor::humidity > user::targetMaxHumidity && user::fansControlHumidity)  {
      float humOutOfRange = abs(user::targetMaxHumidity - sensor::humidity);
      humPercent = humOutOfRange / (user::targetMaxHumidity / 100.0);
      //Serial.println(F("SPEED_UP humPercent: %d", humPercent);
    }
    else if (sensor::humidity < user::targetMinHumidity && user::fansControlHumidity)  {
      float humOutOfRange = abs(user::targetMinHumidity - sensor::humidity);
      humPercent = humOutOfRange / (user::targetMinHumidity / 100.0);
      //Serial.println(F("SPEED_DOWN humPercent: %d", humPercent);
    }

    // Control the air heater
    static uint8_t prevAirHeaterMin = 69;
    if (rtc.minute() != prevAirHeaterMin) {
      if (airTemp <= minAirTemp && !device::airHeaterIsOn) {
        device::airHeaterIsOn = true;
        if (fanDebug)   
          Serial.println(F("air heater on"));
        digitalWrite(pin::airHeater, !device::relayOffState);
        saveLogMessage(11); // save log message, air heater on
      }
      else if (airTemp > minAirTemp &&device::airHeaterIsOn) {
        device::airHeaterIsOn = false;
        if (fanDebug)   
          Serial.println(F("air heater off"));
        digitalWrite(pin::airHeater, device::relayOffState);
        saveLogMessage(12); // save log message, air heater off
      }
      prevAirHeaterMin = rtc.minute();
    }

    float averageTemp = averageShortToFltArray(sensor::fanTemperatureArray, sensor::fanArrayPos);
    sensor::airStates tempAirState = sensor::IS_SAME;
    if (hasChanged(airTemp, averageTemp, 1)) {
      //Serial.print(F("Has changed, tempAirState: "));
      if (airTemp < averageTemp)
        tempAirState = sensor::IS_FALLING;
      else if (airTemp > averageTemp)
        tempAirState = sensor::IS_RISING;
      //Serial.println(tempAirState);
    }
    float averageHum = averageShortToFltArray(sensor::fanHumidityArray, sensor::fanArrayPos);
    sensor::airStates humAirState = sensor::IS_SAME;
    if (hasChanged(sensor::humidity, averageHum, 1)) {
      //Serial.print(F("Has changed, humAirState: "));
      if (sensor::humidity < averageHum)
        humAirState = sensor::IS_FALLING;
      else if (sensor::humidity > averageHum)
        humAirState = sensor::IS_RISING;
      //Serial.println(humAirState);
    }

    if (user::fansControlTemperature) {
      if (fanDebug) {
        Serial.print(F("Air temp: ")); Serial.print(airTemp, 2); Serial.print(F("c, average temp: ")); Serial.print(averageTemp, 2); Serial.print(F("c, over ")); Serial.print(sensor::fanArrayPos); Serial.println(F(" reads @ 2 sec per read"));
      }     
      adjustFanMode(airTemp, tempOption, tempAirState, "air temp", tempPercent, minAirTemp, maxAirTemp, fanDebug);
    }
    if (user::fansControlHumidity) {
      if (fanDebug) {
        Serial.print(F("Humidity: ")); Serial.print(sensor::humidity, 2); Serial.print(F("%, averageHum: ")); Serial.print(averageHum, 2); Serial.println(F("%, over ")); Serial.print(sensor::fanArrayPos); Serial.println(F(" reads @ 2 sec per read"));
      }   
      adjustFanMode(sensor::humidity, humOption, humAirState, "humidity", humPercent, user::targetMinHumidity, user::targetMaxHumidity, fanDebug);
    }

    // Temperature has priority over humidity
    if (tempOption != device::SPEED_IDLE || !user::fansControlHumidity) {
      fanMode = tempOption;
      if (fanDebug) {
        Serial.print(F("Fans set to control temperature")); Serial.print(user::fansControlHumidity ? F(", as temperature takes priority\n") : F("\n"));
      }     
    }
    else {
      fanMode = humOption;
      if (fanDebug) {
        Serial.print(F("Fans set to control humidity")); Serial.print(user::fansControlTemperature ? F(", as the temperature is in range\n") : F("\n"));
      }        
    }
    
    // adjust the fan speeds
    if (fanMode == device::SPEED_MAX)
      device::fanOneSpeed = user::targetMaxFanOneSpeed;
    else if (fanMode == device::SPEED_MIN)
      device::fanOneSpeed = user::targetMinFanOneSpeed;
    else if (fanMode == device::SPEED_UP && device::fanOneSpeed < user::targetMaxFanOneSpeed)
      device::fanOneSpeed++;
    else if (fanMode == device::SPEED_UP_SLOW && device::fanOneSpeed < user::targetMaxFanOneSpeed)
      device::fanOneSpeed += 0.25;
    else if (fanMode == device::SPEED_DOWN && device::fanOneSpeed > user::targetMinFanOneSpeed)
      device::fanOneSpeed--;
    else if (fanMode == device::SPEED_DOWN_SLOW && device::fanOneSpeed > user::targetMinFanOneSpeed)
      device::fanOneSpeed -= 0.25;

    if (fanMode == device::SPEED_MAX)
      device::fanTwoSpeed = user::targetMaxFanTwoSpeed;
    else if (fanMode == device::SPEED_MIN)
      device::fanTwoSpeed = user::targetMinFanTwoSpeed;
    else if (fanMode == device::SPEED_UP && device::fanTwoSpeed < user::targetMaxFanTwoSpeed)
      device::fanTwoSpeed++;
    else if (fanMode == device::SPEED_UP_SLOW && device::fanTwoSpeed < user::targetMaxFanTwoSpeed)
      device::fanTwoSpeed += 0.25;
    else if (fanMode == device::SPEED_DOWN && device::fanTwoSpeed > user::targetMinFanTwoSpeed)
      device::fanTwoSpeed--;
    else if (fanMode == device::SPEED_DOWN_SLOW && device::fanTwoSpeed > user::targetMinFanTwoSpeed)
      device::fanTwoSpeed -= 0.25;

    // Check to see if the fan speed is less than or greater than the min or max speed and if so adjust the current fans speed
    if (device::fanOneSpeed < user::targetMinFanOneSpeed) 
      device::fanOneSpeed = user::targetMinFanOneSpeed;   
    else if (device::fanOneSpeed > user::targetMaxFanOneSpeed) 
      device::fanOneSpeed = user::targetMaxFanOneSpeed;   
    if (device::fanTwoSpeed < user::targetMinFanTwoSpeed) 
      device::fanTwoSpeed = user::targetMinFanTwoSpeed;  
    else if (device::fanTwoSpeed > user::targetMaxFanTwoSpeed) 
      device::fanTwoSpeed = user::targetMaxFanTwoSpeed;

    // Send the new fan speeds to the Atmel328P
    if (device::fanOneSpeed != previousFanOneSpeed) {
      sendToSlave('Z', device::fanOneSpeed);
      if (fanDebug) {
        Serial.print(F("Fan one speed:")); Serial.println(device::fanOneSpeed);
      }
      previousFanOneSpeed = device::fanOneSpeed;
    }
    if (device::fanTwoSpeed != previousFanTwoSpeed) {
      sendToSlave('X', device::fanTwoSpeed);
      if (fanDebug) {
        Serial.print(F("Fan two speed:")); Serial.println(device::fanTwoSpeed);
      }
      previousFanTwoSpeed = device::fanTwoSpeed;
    }
    previousMillis = millis();
  }
}

// Control the waters EC and or PH and or PMM
void waterEcPhControl() {
  enum adjustmentModes {UNADJUSTED, ADJUSTED_EC, ADJUSTED_PH};
  static adjustmentModes adjustmentMode = UNADJUSTED;
  //static adjustmentModes lastAdjustmentMode = UNADJUSTED;
  // Every hour increment the dosing hour counter
  if (device::previousDosingHour != rtc.hour() && rtc.minute() >= device::previousDosingMinute) {
    device::dosingTimerHourCounter++;
    device::previousDosingHour = rtc.hour();
    device::previousDosingMinute = rtc.minute();
    if (device::globalDebug) {
      Serial.println(__FUNCTION__);
      Serial.print(F("Dosing Timer Hour Counter: ")); Serial.println(device::dosingTimerHourCounter);
    }
  }
  // If the dosing hour counter is greather than or equals the dosingInterval then run the dosing logic
  if (device::dosingTimerHourCounter >= user::dosingInterval) {
    if (!device::currentlyDosing && device::sensorsReady) {
      if (adjustmentMode == UNADJUSTED || adjustmentMode == ADJUSTED_PH) {
        if (user::convertToTds)
          adjustWaterTds();
        else
          adjustWaterEc();
        adjustmentMode = ADJUSTED_EC;
      }
      else {
        adjustWaterPh();
        adjustmentMode = ADJUSTED_PH;
      }
      clearPage();
      display::refreshPage = true;
    }
  }
}

void adjustWaterEc() {
  if (device::globalDebug)
    Serial.println(F("Attempting to adjust the EC..."));
  if (sensor::ec < user::targetMinEc) {
    float percentage = percentOutOfRange(user::targetMinEc, sensor::ec);
    if (device::globalDebug) {
      Serial.print(F("EC ")); Serial.print(sensor::ec); Serial.print(F(" is lower than min target ")); Serial.print(user::targetMinEc); Serial.print(F(" by ")); Serial.print(percentage, 2); Serial.println(F("%"));
    }
    // Display a notification to cancel or continue with the starting of the dosing
    display::lastTouchMillis = millis();
    launchDosingNotification(percentage, 1, display::lastTouchMillis);
    // Work out dosing amount
    float dosingAmount = 0;
    if (device::continueDosing != device::CANCEL) {
      float dosingMls[6] {0, 0, 0, 0, 0, 0};
      bool enabledDosers[6] {false, false, false, false, false, false};
      const uint8_t currentDoserModes[] {user::doserOneMode, user::doserTwoMode, user::doserThreeMode, user::doserFourMode, user::doserFiveMode, user::doserSixMode};
      const uint16_t currentDoserMls[] {user::doserOneMills, user::doserTwoMills, user::doserThreeMills, user::doserFourMills, user::doserFiveMills, user::doserSixMills};
      if (user::doserOneMode == device::DOSER_EC || user::doserOneMode == device::DOSER_EC_OP) 
        enabledDosers[0] = true;
      if (user::doserTwoMode == device::DOSER_EC || user::doserTwoMode == device::DOSER_EC_OP)
        enabledDosers[1] = true;
      if (user::doserThreeMode == device::DOSER_EC || user::doserThreeMode == device::DOSER_EC_OP)
        enabledDosers[2] = true;
      if (user::doserFourMode == device::DOSER_EC || user::doserFourMode == device::DOSER_EC_OP) 
        enabledDosers[3] = true;
      if (user::numberOfDosers >= 5 && (user::doserFiveMode == device::DOSER_EC || user::doserFiveMode == device::DOSER_EC_OP)) 
        enabledDosers[4] = true;
      if (user::numberOfDosers >= 6 && (user::doserSixMode == device::DOSER_EC || user::doserSixMode == device::DOSER_EC_OP))
        enabledDosers[5] = true;
      //
      if (device::globalDebug)
        Serial.print(F("Enabled dosers = "));
      int16_t numEnabledDosers = 0;
      for (uint8_t i = 0; i < 6; i++) {
        if (enabledDosers[i]) {
          if (device::globalDebug) {
            Serial.print(i); Serial.print(F(" "));
          }
          numEnabledDosers++;
        }
      }
      if (device::globalDebug)
        Serial.println();
      //    
      saveLogMessage(9);
      //
      if (user::ecDosingMode == user::PRECISE) {
        if (device::globalDebug) {
          Serial.println(F("EC/TDS dosing mode set to precise"));
          Serial.print(F("waterVolumeLtrs = ")); Serial.println(sensor::waterVolumeLtrs, 2);
        }
        // Work out the exact amout of nutrients to dose
        dosingAmount = (user::targetMinEc - sensor::ec) * sensor::waterVolumeLtrs / sensor::ecSolution;
        float mlsPerDoser = (float)dosingAmount / numEnabledDosers;
        for (uint16_t i = 0; i < 6; i++) {
          if (enabledDosers[i] && currentDoserModes[i] == device::DOSER_EC)
            dosingMls[i] = mlsPerDoser;
          else if (enabledDosers[i] && currentDoserModes[i] == device::DOSER_EC_OP)
            dosingMls[i] = currentDoserMls[i];
        }
        if (device::globalDebug) {
          Serial.print(F("Calculated ")); Serial.print(dosingAmount, 2); Serial.print(F(" mls of nutrients, divided by ")); Serial.print(numEnabledDosers); Serial.print(F(" EC dosers = ")); Serial.println(mlsPerDoser, 2);
        }
      }
      //
      else {
        if (device::globalDebug)
          Serial.println(F("EC/TDS dosing mode set to incremental"));
        for (int16_t i = 0; i < 6; i++) {
          if (enabledDosers[i]) {
            dosingAmount += dosingMls[i] = currentDoserMls[i];
          }
        }
      }
      // Run the dosing pumps   
      if (dosingAmount > 0) {
        if (device::globalDebug)
          Serial.println(F("Starting dosing..."));
        device::currentlyDosing = true;
        runDosers(enabledDosers, dosingMls, percentage, 1, display::lastTouchMillis);
      }     
    }
    device::dosingTimerHourCounter = 0;
  }
}

void adjustWaterTds() {
  if (device::globalDebug)
    Serial.println(F("Attempting to adjust the TDS..."));
  if (sensor::tds < user::targetMinTds) {
    float percentage = percentOutOfRange(user::targetMinTds, sensor::tds);
    if (device::globalDebug) {
      Serial.print(F("TDS ")); Serial.print(sensor::tds); Serial.print(F(" is lower than min target ")); Serial.print(user::targetMinTds); Serial.print(F(" by ")); Serial.print(percentage, 2); Serial.println(F("%"));
    }
    // Display a notification to cancel or continue with the starting of the dosing
    display::lastTouchMillis = millis();
    launchDosingNotification(percentage, 1, display::lastTouchMillis);
    // Work out dosing amount
    float dosingAmount = 0;
    if (device::continueDosing != device::CANCEL) {
      float dosingMls[6] {0, 0, 0, 0, 0, 0};
      bool enabledDosers[6] {false, false, false, false, false, false};
      const uint8_t currentDoserModes[] {user::doserOneMode, user::doserTwoMode, user::doserThreeMode, user::doserFourMode, user::doserFiveMode, user::doserSixMode};
      const uint16_t currentDoserMls[] {user::doserOneMills, user::doserTwoMills, user::doserThreeMills, user::doserFourMills, user::doserFiveMills, user::doserSixMills};
      if (user::doserOneMode == device::DOSER_EC || user::doserOneMode == device::DOSER_EC_OP) 
        enabledDosers[0] = true;
      if (user::doserTwoMode == device::DOSER_EC || user::doserTwoMode == device::DOSER_EC_OP)
        enabledDosers[1] = true;
      if (user::doserThreeMode == device::DOSER_EC || user::doserThreeMode == device::DOSER_EC_OP)
        enabledDosers[2] = true;
      if (user::doserFourMode == device::DOSER_EC || user::doserFourMode == device::DOSER_EC_OP) 
        enabledDosers[3] = true;
      if (user::numberOfDosers >= 5 && (user::doserFiveMode == device::DOSER_EC || user::doserFiveMode == device::DOSER_EC_OP)) 
        enabledDosers[4] = true;
      if (user::numberOfDosers >= 6 && (user::doserSixMode == device::DOSER_EC || user::doserSixMode == device::DOSER_EC_OP))
        enabledDosers[5] = true;
      //
      if (device::globalDebug) 
        Serial.print(F("Enabled dosers = "));
      int16_t numEnabledDosers = 0;
      for (uint8_t i = 0; i < 6; i++) {
        if (enabledDosers[i]) {
          if (device::globalDebug) {
            Serial.print(i); Serial.print(F(" "));
          }
          numEnabledDosers++;
        }
      }
      if (device::globalDebug)
        Serial.println();
      //    
      saveLogMessage(9);
      //
      if (user::ecDosingMode == user::PRECISE) {
        if (device::globalDebug) {
          Serial.println(F("EC/TDS dosing mode set to precise"));
          Serial.print(F("waterVolumeLtrs = ")); Serial.println(sensor::waterVolumeLtrs, 2);
        }
        // Work out the exact amout of nutrients to dose
        dosingAmount = (user::targetMinTds - sensor::tds) * sensor::waterVolumeLtrs / sensor::tdsSolution;
        float mlsPerDoser = (float)dosingAmount / numEnabledDosers;
        for (uint8_t i = 0; i < 6; i++) {
          if (enabledDosers[i] && currentDoserModes[i] == device::DOSER_EC)
            dosingMls[i] = mlsPerDoser;
          else if (enabledDosers[i] && currentDoserModes[i] == device::DOSER_EC_OP)
            dosingMls[i] = currentDoserMls[i];
        }
        if (device::globalDebug) {
          Serial.print(F("Calculated ")); Serial.print(dosingAmount, 2); Serial.print(F(" mls of nutrients, divided by ")); Serial.print(numEnabledDosers); Serial.print(F(" EC dosers = ")); Serial.println(mlsPerDoser, 2);
        }
      }
      //
      else {
        if (device::globalDebug)
          Serial.println(F("EC/TDS dosing mode set to incremental"));
        for (uint8_t i = 0; i < 6; i++) {
          if (enabledDosers[i]) {
            dosingAmount += dosingMls[i] = currentDoserMls[i];
          }
        }
      }
      // Run the dosing pumps   
      if (dosingAmount > 0) {
        if (device::globalDebug)
          Serial.println(F("Starting dosing..."));
        device::currentlyDosing = true;
        runDosers(enabledDosers, dosingMls, percentage, 1, display::lastTouchMillis);
      }     
    }
    device::dosingTimerHourCounter = 0;
  }
}

void adjustWaterPh() {
  if (device::globalDebug)
    Serial.println(F("Attempting to adjust the PH..."));
  if (sensor::ph < user::targetMinPh || sensor::ph > user::targetMaxPh) {
    float percentage = percentOutOfRange(sensor::ph < user::targetMinPh ? user::targetMinPh : user::targetMaxPh, sensor::ph);
    if (device::globalDebug) {
      Serial.print(F("PH ")); Serial.print(sensor::ph); 
      if (sensor::ph < user::targetMinPh) {
        Serial.print(F(" is lower than min target ")); Serial.print(user::targetMinPh, 2);
      }
      else {
        Serial.print(F(" is higher than max target ")); Serial.print(user::targetMaxPh, 2);
      }      
      Serial.print(F(" by ")); Serial.print(percentage, 2); Serial.println(F("%"));
    }
    // Display a notification to cancel or continue with the starting of the dosing
    display::lastTouchMillis = millis();
    launchDosingNotification(percentage, 0, display::lastTouchMillis);
    // Work out dosing amount
    float dosingAmount = 0;
    if (device::continueDosing != device::CANCEL) {
      bool enabledDosers[6] {false, false, false, false, false, false};
      float dosingMls[6] {0, 0, 0, 0, 0, 0};
      // Adjust PH up
      if (sensor::ph < user::targetMinPh) {
        if (user::doserOneMode == device::DOSER_PH_UP) 
          enabledDosers[0] = true;
        if (user::doserTwoMode == device::DOSER_PH_UP) 
          enabledDosers[1] = true;
        if (user::doserThreeMode == device::DOSER_PH_UP) 
          enabledDosers[2] = true;
        if (user::doserFourMode == device::DOSER_PH_UP) 
          enabledDosers[3] = true;
        if (user::numberOfDosers >= 5 && user::doserFiveMode == device::DOSER_PH_UP) 
          enabledDosers[4] = true;
        if (user::numberOfDosers >= 6 && user::doserSixMode == device::DOSER_PH_UP) 
          enabledDosers[5] = true;
      }
      // Adjust PH down
      else if (sensor::ph > user::targetMinPh) {
        if (user::doserOneMode == device::DOSER_PH_DOWN) 
          enabledDosers[0] = true;
        if (user::doserTwoMode == device::DOSER_PH_DOWN) 
          enabledDosers[1] = true;
        if (user::doserThreeMode == device::DOSER_PH_DOWN) 
          enabledDosers[2] = true;
        if (user::doserFourMode == device::DOSER_PH_DOWN) 
          enabledDosers[3] = true;
        if (user::numberOfDosers >= 5 && user::doserFiveMode == device::DOSER_PH_DOWN) 
          enabledDosers[4] = true;
        if (user::numberOfDosers >= 6 && user::doserSixMode == device::DOSER_PH_DOWN) 
          enabledDosers[5] = true;
      }   
      if (device::globalDebug)
         Serial.print(F("Enabled dosers = "));
      int16_t numEnabledDosers = 0;
      for (uint8_t i = 0; i < 6; i++) {
        if (enabledDosers[i]) {
          if (device::globalDebug) {
            Serial.print(i); Serial.print(F(" "));
          }
          numEnabledDosers++;
        }
      }
      if (device::globalDebug)
        Serial.println();
      sensor::ph < user::targetMinPh ? saveLogMessage(8) : saveLogMessage(7);
      if (user::phDosingMode == user::PRECISE) {
        if (device::globalDebug) {
          Serial.println(F("PH dosing mode set to precise"));
          Serial.print(F("waterVolumeLtrs = ")); Serial.println(sensor::waterVolumeLtrs, 2);
        }
        if (sensor::ph < user::targetMinPh) {
          // Work out the exact amout of PH up to dose
          dosingAmount = (user::targetMinPh - sensor::ph) * sensor::waterVolumeLtrs / sensor::phDownSolution;
          float mlsPerDoser = (float)dosingAmount / numEnabledDosers;
          for (uint8_t i = 0; i < 6; i++) {
            if (enabledDosers[i]) {
              dosingAmount += dosingMls[i] = currentDoserMls[i];
            }
          }
          if (device::globalDebug) {
            Serial.print(F("Calculated ")); Serial.print(dosingAmount, 2); Serial.print(F(" mls of PH up solution, divided by ")); Serial.print(numEnabledDosers); Serial.print(F(" PH up dosers = ")); Serial.println(mlsPerDoser, 2);
          }
        }
        else if (sensor::ph > user::targetMaxPh) {
          // Work out the exact amout of PH down to dose
          dosingAmount = (sensor::ph - user::targetMaxPh) * sensor::waterVolumeLtrs / sensor::phUpSolution;
          float mlsPerDoser = (float)dosingAmount / numEnabledDosers;
          for (uint8_t i = 0; i < 6; i++) {
            if (enabledDosers[i]) {
              dosingAmount += dosingMls[i] = currentDoserMls[i];
            }
          }
          if (device::globalDebug) {
            Serial.print(F("Calculated ")); Serial.print(dosingAmount, 2); Serial.print(F(" mls of PH down solution, divided by ")); Serial.print(numEnabledDosers); Serial.print(F(" PH down dosers = ")); Serial.println(mlsPerDoser, 2);
          }
        } 
      }
      else {
        Serial.println(F("PH dosing mode set to incremental"));
        if (enabledDosers[0]) 
          dosingMls[0] = user::doserOneMills;
        if (enabledDosers[1]) 
          dosingMls[1] = user::doserTwoMills;
        if (enabledDosers[2]) 
          dosingMls[2] = user::doserThreeMills;
        if (enabledDosers[3]) 
          dosingMls[3] = user::doserFourMills;
        if (enabledDosers[4]) 
          dosingMls[4] = user::doserFiveMills;
        if (enabledDosers[5]) 
          dosingMls[5] = user::doserSixMills; 
      }
      // Run the dosing pumps   
      if (dosingAmount > 0) {
        if (device::globalDebug)
          Serial.println(F("Starting dosing..."));
        device::currentlyDosing = true;
        runDosers(enabledDosers, dosingMls, percentage, 0, display::lastTouchMillis);
      }        
    }
    device::dosingTimerHourCounter = 0;
  }
}

// Return the percent out of range
float percentOutOfRange(const float& a_setPoint, const float& a_val) {
  float outOfRange = abs(a_setPoint - a_val);
  float percent = a_setPoint / 100.0;
  return outOfRange / percent;
}

void runDosers(bool* a_enabledDosers, float* a_dosingMls, const float a_percent, const int16_t a_dosingMode, uint32_t a_lastTouch) {
  uint8_t previousDoserNum = 0;
  device::currentDoserNum = 1;
  device::currentDoserMls = 0;
  bool updateMlsText = false;
  static float prevCurrentDoserMls = -1;
  const char* str[3] = {"PH", "EC", "TDS"};
  
  while (device::currentlyDosing) { // timing is critical we must use a while loop
    // show continue dialog
    if (device::currentDoserNum != previousDoserNum || prevCurrentDoserMls != device::currentDoserMls) {
      if (device::currentDoserNum != previousDoserNum)
        updateMlsText = false;
      else
        updateMlsText = true;
      abortMessage(message::cancelDosing, str[a_dosingMode], a_percent, device::currentDoserNum, device::currentDoserMls, 2, updateMlsText);
      previousDoserNum = device::currentDoserNum;
      prevCurrentDoserMls = device::currentDoserMls;
    }
    if (a_enabledDosers[0])
      a_enabledDosers[0] = runDoser(1, pin::doserOne, user::doserOneSpeed, a_dosingMls[0]);
    else if (a_enabledDosers[1])
      a_enabledDosers[1] = runDoser(2, pin::doserTwo, user::doserTwoSpeed, a_dosingMls[1]);
    else if (a_enabledDosers[2])
      a_enabledDosers[2] = runDoser(3, pin::doserThree, user::doserThreeSpeed, a_dosingMls[2]);
    else if (a_enabledDosers[3])
      a_enabledDosers[3] = runDoser(4, pin::doserFour, user::doserFourSpeed, a_dosingMls[3]);
    else if (a_enabledDosers[4])
      a_enabledDosers[4] = runDoser(5, pin::doserFive, user::doserFiveSpeed, a_dosingMls[4]);
    else if (a_enabledDosers[5])
      a_enabledDosers[5] = runDoser(6, pin::doserSix, user::doserSixSpeed, a_dosingMls[5]);
    else if (device::currentlyDosing) {
      if (device::globalDebug)
        Serial.println(F("Dosing complete"));
      display::refreshPage = true;
      device::currentlyDosing = false;
    }
    // touch event - cancel button
    const uint16_t startX = 166, startY = 166;
    if (tft.touched()) {
      tft.touchReadPixel(&display::touch_x, &display::touch_y);
      if (millis() >= a_lastTouch) {
        if (display::touch_x >= startX + 200 && display::touch_x <= startX + 400 && display::touch_y >= startY + 200 && display::touch_y <= startY + 250) { // Cancel
        if (device::globalDebug)
            Serial.println(F("Dosing aborted"));
          beep();
          device::currentlyDosing = false;
          display::refreshPage = true;
          for (uint8_t i = 0; i < 6; i++)
            a_enabledDosers[i] = false;
        }
      }
    }
    updateCyclicTimers();
  }
}

// Run a given doser for a_mls * 1000 on a_doserPin at a_doserSpeed
bool runDoser(const uint8_t& a_doserNum, const uint8_t& a_doserPin, const int16_t& a_doserSpeed, const float& a_mls) {
  static uint8_t previousDoserNum = 255;
  static uint32_t previousDoserMillis = millis(); 
  if (previousDoserNum != a_doserNum) {
    if (device::globalDebug) {
      Serial.print(F("Starting doser: ")); Serial.print(a_doserNum); Serial.print(F(", pumping ")); Serial.print(a_mls, 2); Serial.println(F("mls"));
    }
    device::currentDoserMls = a_mls;
    previousDoserNum = a_doserNum;
    analogWrite(a_doserPin, a_doserSpeed);
    device::currentDoserNum = a_doserNum;
  }
  if (millis() - previousDoserMillis >= 1000UL && device::currentDoserMls >= 1) {
    device::currentDoserMls--;
    previousDoserMillis = millis();
  }
  else if (device::currentDoserMls > 0 && device::currentDoserMls < 1) {
    delay(device::currentDoserMls);
    device::currentDoserMls = 0;
  }
  if (device::currentDoserMls <= 0) {
    if (device::globalDebug) {
      Serial.print(F("Stopping doser: ")); Serial.println(a_doserNum);
    }
    analogWrite(a_doserPin, 0);
    previousDoserNum = 255;  
    return false;
  }
  return true;
}
