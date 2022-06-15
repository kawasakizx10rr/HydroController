void envriomentalControl() {
  lightingControl();
  waterLevelControl();
  waterTemperatureControl();
  co2Control();
  airControl();
  waterEnvriomentControl();
}

// Control the water level
void waterLevelControl() {
  bool startDraining = false, startRefilling = false;
  static byte previousDate = 0;
  unsigned long previousMillis = 0;
  byte continueRefilling = device::NOT_SET;
  byte continueDraining = device::NOT_SET;
  unsigned long lastTouch = millis();

  if (device::sensorsReady) {
    // auto drain and refill tank on set dates and time
    rtcTime = rtc.getTime();
    if (!user::disableDrainAndRefill && rtcTime.date != previousDate) {
      for (byte i = 0; i < 31; i++) {
        if (user::autoFillDays[i] && rtcTime.date == i + 1 && rtcTime.hour == user::autoFillHour && rtcTime.min >= user::autoFillMinute) {
          Serial.println(F("Auto drain time matched, about to start the auto drain process"));
          saveLogMessage(1);
          startDraining = true;
          previousDate = rtcTime.date;
          break;
        }
      }
    }

    // Show a dialog asking the user if they want to cancel the auto drain process before it starts
    if (startDraining) {
      launchDrainNotification(continueDraining, startDraining);
      lastTouch = millis() + 5000UL;
    }

    // Show a dialog asking the user if they want to abort the drain process while its draining
    if (startDraining && continueDraining != device::CANCEL) {
      Serial.println(F("About to show the drain abort dialog, and start draining"));
      const float waterTarget = user::convertToInches ? user::targetMinWaterHeight / 2.5 : user::targetMinWaterHeight;
      abortMessage(message::cancelDraining, user::convertToInches ? "\"" : "cm", waterTarget, -1, 1);
    }

    // Start draining the water from the tank
    int previousWaterLevel = sensor::waterLevel;
    previousMillis = millis();
    while (startDraining && continueDraining != device::CANCEL) {
      if (sensor::waterLevel > 0) { // drain tank till empty
        // Turn drain pump relay on
        digitalWrite(pin::outletPump, !device::relayOffState);
        // timer checking water level is still decresing else bail after 1 minute * drainTimeout
        if (millis() - previousMillis >= 60000UL * user::drainTimeout) { // put timer value in settings GUI !
          digitalWrite(pin::outletPump, device::relayOffState);
          startDraining = false;
          startRefilling = true;
          Serial.println(F("Failed to pump any further water out of the tank, quiting drain process"));
        }
        int waterHeight = getWaterHeight();
        if (waterHeight >= 0)
          sensor::waterLevel = waterHeight;

        if (sensor::waterLevel < previousWaterLevel) {
          previousWaterLevel = sensor::waterLevel;
          previousMillis = millis();
        }
      }
      else {
        Serial.println("Draining complete");
        digitalWrite(pin::outletPump, device::relayOffState);
        startDraining = false;
        startRefilling = true;
      }

      // cancel button touch event for abort draining dialog
      const uint16_t startX = 166, startY = 166;
      if (tft.touched()) {
        tft.touchReadPixel(&display::touch_x, &display::touch_y);
        if (millis() >= lastTouch) {
          if (display::touch_x >= startX + 200 && display::touch_x <= startX + 400 && display::touch_y >= startY + 200 && display::touch_y <= startY + 250) { // Cancel
            Serial.println(F("Water drain aborted"));
            digitalWrite(pin::outletPump, device::relayOffState);
            beep();
            startDraining = false;
            clearPage();
            display::refreshPage = true;
          }
        }
      }
    }

    // refill tank when water is low at any date or time
    if (!user::disableDrainAndRefill && !startRefilling) {
      static unsigned long previousDelayMillis = millis(); // give the user 5 minutes to disable "refill When Low" else if the water level is below min this will keep showing a dialog over and over..
      if (millis() - previousDelayMillis >= 300000UL) {
        if ((sensor::waterLevel < user::targetMinWaterHeight && !user::convertToInches) || (sensor::waterLevelInches < user::targetMinWaterHeightInches && user::convertToInches)) {
          startRefilling = true;
          Serial.println(F("The water level is below the min target, starting refill process"));
        }
        previousDelayMillis = millis();
      }
    }

    // Show a dialog asking the user if they want to cancel the refill process before it starts
    launchRefillNotification(startRefilling, continueRefilling);

    // Show a dialog asking the user if they want to abort the refill process while its refilling
    if (startRefilling && continueRefilling != device::CANCEL) {
      Serial.println(F("About to show refill abort dialog, and refill tank"));
      const float waterTarget = user::convertToInches ? user::targetMaxWaterHeightInches : user::targetMaxWaterHeight;
      abortMessage(message::cancelRefilling, user::convertToInches ? "\"" : "cm", waterTarget, -1, 1);
      saveLogMessage(2);

      lastTouch = millis() + 5000UL;
      previousWaterLevel = sensor::waterLevel;
      previousMillis = millis();

      while (startRefilling && continueRefilling != device::CANCEL) {
        if (!refillTank(lastTouch, previousMillis, previousWaterLevel)) {
          startRefilling = false;
          clearPage();
          display::refreshPage = true;
        }
      }

    }

  }
}

// Refill the tanks water and run doers all available dosers
bool refillTank(const unsigned long& a_lastTouch, unsigned long& a_previousMillis, int& a_previousWaterLevel) {
  static bool startRefilling = true;
  static bool lockPump = false;
  static bool startDoser[6] {false, false, false, false, false, false};
  static unsigned long previousDoserMillis = 0;
  const float waterLevel = user::convertToInches ? sensor::waterLevelInches : sensor::waterLevel;
  const float waterTarget = user::convertToInches ? user::targetMaxWaterHeightInches : user::targetMaxWaterHeight;

  if (waterLevel < waterTarget && startRefilling) {
    // turn on the inlet water pump
    digitalWrite(pin::inletPump, !device::relayOffState);
    // timer checking water level is still decresing else bail after 1 minute * drainTimeout
    if (millis() - a_previousMillis >= 60000UL * user::drainTimeout) {
      Serial.println(F("Failed to pump in any further water, quiting refill process and starting dosing"));
      digitalWrite(pin::inletPump, device::relayOffState);
      startRefilling = false;
    }
    else if (waterLevel >= waterTarget) {
      Serial.println(F("Finished pumping in water, quiting refill process and starting dosing"));
      digitalWrite(pin::inletPump, device::relayOffState);
      startRefilling = false;
    }
  }

  // get current water height
  sensor::waterLevel = getWaterHeight();
  if (sensor::waterLevel < a_previousWaterLevel) {
    a_previousWaterLevel = sensor::waterLevel;
    a_previousMillis = millis();
  }
  else if (!startRefilling && !lockPump) {
    Serial.println(F("Refilling complete, starting dosing"));
    for (byte i = 0; i < user::numberOfDosers; i++)
      startDoser[i] = true;
    lockPump = true;
    previousDoserMillis = millis();
  }

  // run dosers
  if (lockPump) {
    if (startDoser[0] && user::doserOneMode != device::DOSER_OFF)
      startDoser[0] = runDoser(1, pin::doserOne, user::doserOneSpeed, user::refillDoserOneMills, previousDoserMillis);
    else if (startDoser[1] && user::doserTwoMode != device::DOSER_OFF)
      startDoser[1] = runDoser(2, pin::doserTwo, user::doserTwoSpeed, user::refillDoserTwoMills, previousDoserMillis);
    else if (startDoser[2] && user::doserThreeMode != device::DOSER_OFF)
      startDoser[2] = runDoser(3, pin::doserThree, user::doserThreeSpeed, user::refillDoserThreeMills, previousDoserMillis);
    else if (startDoser[3] && user::doserFourMode != device::DOSER_OFF)
      startDoser[3] = runDoser(4, pin::doserFour, user::doserFourSpeed, user::refillDoserFourMills, previousDoserMillis);
    else if (startDoser[4] && user::doserFiveMode != device::DOSER_OFF)
      startDoser[4] = runDoser(5, pin::doserFive, user::doserFiveSpeed, user::refillDoserFiveMills, previousDoserMillis);
    else if (startDoser[5] && user::doserSixMode != device::DOSER_OFF)
      startDoser[5] = runDoser(6, pin::doserSix, user::doserSixSpeed, user::refillDoserSixMills, previousDoserMillis);
    else {
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
    if (millis() > a_lastTouch) {
      if (display::touch_x >= startX + 200 && display::touch_x <= startX + 400 && display::touch_y >= startY + 200 && display::touch_y <= startY + 250) { // Cancel
        digitalWrite(pin::inletPump, device::relayOffState);
        Serial.println(F("Water refill and or dosing aborted"));
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
  static byte previousWaterHeaterState = 0;
  static byte waterHeaterState = 3; // 1 = on, 2 = off, 3 = unset
  if (device::sensorsReady) {
    if (user::convertToFarenheit) {
      if (sensor::waterTempF <= (user::targetMinWaterTempF - user::waterTempOffsetF) && waterHeaterState != 1)
        waterHeaterState = 1;
      else if (sensor::waterTempF >= user::targetMaxWaterTempF && waterHeaterState != 2)
        waterHeaterState = 2;
    }
    else {
      if (sensor::waterTemp <= (user::targetMinWaterTemp - user::waterTempOffset) && waterHeaterState != 1)
        waterHeaterState = 1;
      else if (sensor::waterTemp >= user::targetMaxWaterTemp && waterHeaterState != 2)
        waterHeaterState = 2;
    }
    if (waterHeaterState != previousWaterHeaterState) {
      if (waterHeaterState == 1) {
        digitalWrite(pin::waterHeater, !device::relayOffState);
        Serial.println(F("Water heater on"));
      }
      else if (waterHeaterState == 2) {
        digitalWrite(pin::waterHeater, device::relayOffState);
        Serial.println(F("Water heater off"));
      }
      previousWaterHeaterState = waterHeaterState;
    }
  }
}

// Control the external lighting
void lightingControl() {
  using namespace user;
  static byte prviousLightMode = 3;
  if (lightMode == 0) { // timer mode
    if (lightOnTimeHour == rtcTime.hour && lightOnTimeMin == rtcTime.min && !device::lightOn) {
      digitalWrite(pin::light, !device::relayOffState);
      Serial.println(F("Light on"));
      device::lightOn = true;
      device::lightSwitchedOnHour = rtcTime.hour;
      device::lightSwitchedOnMin = rtcTime.min;
      saveLogMessage(4);
    }
    else if (lightOffTimeHour == rtcTime.hour && lightOffTimeMin == rtcTime.min && device::lightOn) {
      digitalWrite(pin::light, device::relayOffState);
      Serial.println(F("Light off"));
      device::lightOn = false;
      saveLogMessage(5);
    }
  }
  else if (lightMode == 1 && lightMode != prviousLightMode) { // constant on
    digitalWrite(pin::light, !device::relayOffState);
    Serial.println(F("Light const on"));
    device::lightOn = true;
    device::lightSwitchedOnHour = rtcTime.hour;
    device::lightSwitchedOnMin = rtcTime.min;
  }
  else if (lightMode == 2 && lightMode != prviousLightMode) { // constant off
    Serial.println(F("Light const off"));
    digitalWrite(pin::light, device::relayOffState);
    device::lightOn = false;
  }
  prviousLightMode = lightMode;
  // Log any occourance of the light turning off when it should be on "once per day"
  static int previousLightOnDay = 0;
  if (device::lightOn && previousLightOnDay != rtcTime.date) {
    if (device::lightSwitchedOnMin <= 57 && rtcTime.min >= device::lightSwitchedOnMin + 2) {
      if (sensor::ldr < 400)
        saveLogMessage(6);
    }
    else if (device::lightSwitchedOnMin == 58 && rtcTime.min == 0)  {
      if (sensor::ldr < 400)
        saveLogMessage(6);
    }
    else if (device::lightSwitchedOnMin > 58 && rtcTime.min >= 2)  {
      if (sensor::ldr < 400)
        saveLogMessage(6);
    }
    previousLightOnDay = rtcTime.date;
  }
}

// Check if the current time is in between the users start and end time
bool restartLightingTimer() {
  using namespace user;
  int startMinutesSinceMidnight = lightOnTimeMin + 60 * lightOnTimeHour;
  int endMinutesSinceMidnight = lightOnTimeHour + 60 * lightOffTimeHour;
  int currentMinutesSinceMidnight = rtcTime.min + 60 * rtcTime.hour;
  if (startMinutesSinceMidnight < endMinutesSinceMidnight) {
    return ((currentMinutesSinceMidnight >= startMinutesSinceMidnight) && (currentMinutesSinceMidnight < endMinutesSinceMidnight));
  }
  else {
    return !((currentMinutesSinceMidnight >= endMinutesSinceMidnight) && (currentMinutesSinceMidnight < startMinutesSinceMidnight));
  }
}

// controls the enviroments co2 levels
void co2Control() {
  bool startCo2Relay = false;
  static byte previousDate = 0;
  unsigned long co2RunTime = millis();
  byte continueCo2Control = device::NOT_SET;
  unsigned long lastTouch = millis();
  sensor::co2GasTime = 0;
  if (device::sensorsReady && !user::disableCo2Control) {
    // Get current date time
    rtcTime = rtc.getTime();
    // Check if it is time to start the Co2 adjustment
    if (user::co2CheckTimeHour == rtcTime.hour && user::co2CheckTimeMinute >= rtcTime.min && rtcTime.date != previousDate) {
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
          previousDate = rtcTime.date;
          startCo2Relay = true;
          // turn of the Co2 solnoide
          digitalWrite(pin::co2Solenoid, !device::relayOffState);
          Serial.println(F("About to show Co2 abort dialog, and start pumping Co2"));
          // Show a dialog asking the user if they want to abort the Co2 adjustment while its pumping
          continueMessage(message::cancelCo2, sensor::co2GasTime / 60000.0, 0, false, true, false);
          co2RunTime = millis() + sensor::co2GasTime;
          lastTouch = millis() + 5000UL;
        }
      }
    }
    // Refresh the dialog with the remaining gas time
    abortCo2Notification(startCo2Relay, lastTouch, co2RunTime);
  }
}

// Control one or two 3rd party AC fans and a 3rd party AC air heater
// to maintain the air temperature and or humidity.
void airControl() {
  static unsigned long previousMillis = millis();
  static byte previousFanOneSpeed = 200;
  static byte previousFanTwoSpeed = 200;
  float tempPercent = 0, humPercent = 0;
  float tempOutOfRange = 0, humOutOfRange = 0;

  if (millis() - previousMillis >= 2000UL) {
    // If Co2 has disabled the fans for x durations, check to see if we can turn the fans back on else do nothing
    if (device::co2DisabledFans && device::co2TurnFansBackOnHour == rtcTime.hour && device::co2TurnFansBackOnMinute >= rtcTime.min)
      device::co2DisabledFans = false;
    else if (device::co2DisabledFans)
      return;

    // Check to see if the fan speed is less than or greater than the min or max speed and if so adjust the current fans speed
    if (device::fanOneSpeed < user::targetMinFanOneSpeed) {
      device::fanOneSpeed = user::targetMinFanOneSpeed;
      sendToSlave('Z', device::fanOneSpeed);
    }
    else if (device::fanOneSpeed > user::targetMaxFanOneSpeed) {
      device::fanOneSpeed = user::targetMaxFanOneSpeed;
      sendToSlave('Z', device::fanOneSpeed);
    }
    if (device::fanTwoSpeed < user::targetMinFanTwoSpeed) {
      device::fanTwoSpeed = user::targetMinFanTwoSpeed;
      sendToSlave('X', device::fanTwoSpeed);
    }
    else if (device::fanTwoSpeed > user::targetMaxFanTwoSpeed) {
      device::fanTwoSpeed = user::targetMaxFanTwoSpeed;
      sendToSlave('X', device::fanTwoSpeed);
    }

    // Check to see if the temperature and or humidity is out of range of the user targets
    device::controlOptions tempOption = device::SPEED_IDLE;
    device::controlOptions humOption = device::SPEED_IDLE;
    device::controlOptions fanMode = device::SPEED_IDLE;
    // Check to see if the temperature is out of range
    if (user::convertToFarenheit) {
      if (convertToF(sensor::airTemp) > user::targetMaxAirTempF && !user::fansControlTemperature) {
        tempOutOfRange = abs(user::targetMaxAirTempF - convertToF(sensor::airTemp));
        tempOption = device::SPEED_UP;
        tempPercent = tempOutOfRange / (user::targetMaxAirTempF / 100.0);
      }
      else if (convertToF(sensor::airTemp) < user::targetMinAirTempF && !user::fansControlTemperature) {
        tempOutOfRange = abs(user::targetMinAirTempF - convertToF(sensor::airTemp));
        tempOption = device::SPEED_DOWN;
        tempPercent = tempOutOfRange / (user::targetMinAirTempF / 100.0);
      }
    }
    else {
      if (sensor::airTemp > user::targetMaxAirTemp && !user::fansControlTemperature) {
        tempOutOfRange = abs(user::targetMaxAirTemp - sensor::airTemp);
        tempOption = device::SPEED_UP;
        tempPercent = tempOutOfRange / (user::targetMaxAirTemp / 100.0);
      }
      else if (sensor::airTemp < user::targetMinAirTemp && !user::fansControlTemperature) {
        tempOutOfRange = abs(user::targetMinAirTemp - sensor::airTemp);
        tempOption = device::SPEED_DOWN;
        tempPercent = tempOutOfRange / (user::targetMinAirTemp / 100.0);
      }
    }
    // Check to see if the humidity is out of range
    if (sensor::humidity > user::targetMaxHumidity && !user::fansControlHumidity)  {
      humOutOfRange = abs(user::targetMaxHumidity - sensor::humidity);
      humOption = device::SPEED_UP;
      humPercent = humOutOfRange / (user::targetMaxHumidity / 100.0);
    }
    else if (sensor::humidity < user::targetMinHumidity && !user::fansControlHumidity)  {
      humOutOfRange = abs(user::targetMinHumidity - sensor::humidity);
      humOption = device::SPEED_DOWN;
      humPercent = humOutOfRange / (user::targetMinHumidity / 100.0);
    }
    //Serial.print(F("Temp out by: ")); Serial.print(tempOption == device::SPEED_UP ? tempPercent : -tempPercent); Serial.println(F("%"));
    //Serial.print(F("Hum out by ")); Serial.print(humOption == device::SPEED_UP ? humPercent : -humPercent); Serial.println(F("%"));
    // Temperature and humidity are ok, so send 'just fan' IR command
    if (tempPercent <= 0.01 && humPercent <= 0.01) {
      // Fans should stay at current speed ?
      //Serial.println(F("Temp and Hum ok, fans speed remains the same.."));
    }
    // Temperature or humidity is out of range so work out which command needs to be sent
    else {
      // Check if the tempPercent > humPercent, else control the humidity
      if (tempPercent > humPercent)
        fanMode = tempOption;
      else
        fanMode = humOption;
      // adjust the fan speeds based on the fanMode and max min targets
      if (fanMode == device::SPEED_UP && device::fanOneSpeed < user::targetMaxFanOneSpeed)
        device::fanOneSpeed++;
      else if (fanMode == device::SPEED_DOWN && device::fanOneSpeed > user::targetMinFanOneSpeed)
        device::fanOneSpeed--;
      if (fanMode == device::SPEED_UP && device::fanTwoSpeed < user::targetMaxFanTwoSpeed)
        device::fanTwoSpeed++;
      else if (fanMode == device::SPEED_DOWN && device::fanTwoSpeed > user::targetMinFanTwoSpeed)
        device::fanTwoSpeed--;
      // Send the new fan speeds to the Atmel328P
      if (device::fanOneSpeed != previousFanOneSpeed) {
        sendToSlave('Z', device::fanOneSpeed);
        Serial.print(F("Fan one speed:")); Serial.println(device::fanOneSpeed);
        previousFanOneSpeed = device::fanOneSpeed;
      }
      if (device::fanTwoSpeed != previousFanTwoSpeed) {
        sendToSlave('X', device::fanTwoSpeed);
        Serial.print(F("Fan two speed:")); Serial.println(device::fanTwoSpeed);
        previousFanTwoSpeed = device::fanTwoSpeed;
      }
    }
    previousMillis = millis();
  }
}

// Control the waters EC and or PH and or PMM
void waterEnvriomentControl() {
  static unsigned long previousDoserMillis;
  float ecPercent = 0, phPercent = 0, tdsPercent = 0;
  float targetEc = 0, targetPh = 0, targetTds = 0;
  float sensorPercent = 0, mlsMultipler = 1;
  byte dosingMode = 0;
  static byte phDoseCount = 0, ecDoseCount = 0, tdsDoseCount = 0;
  bool startDoser[6] {false, false, false, false, false, false};
  unsigned long lastTouch = millis();

  // Every hour increment the dosing hour counter
  rtcTime = rtc.getTime();
  if (device::previousDosingHour != rtcTime.hour && rtcTime.min >= device::previousDosingMinute) {
    device::dosingTimerHourCounter++;
    device::previousDosingHour = rtcTime.hour;
    Serial.print(F("Dosing Timer Hour Counter = ")); Serial.println(device::dosingTimerHourCounter);
  }

  // If the dosing hour counter is greather than or equals the dosingInterval then run the dosing logic
  if (device::dosingTimerHourCounter >= user::dosingInterval) {
    device::dosingTimerHourCounter = 0;
    machineLearning::logPhDownResult = false;
    machineLearning::logPhUpResult = false;
    machineLearning::logEcResult = false;
    machineLearning::logTdsResult = false;
    if (!device::currentlyDosing && device::sensorsReady) {

      // Check if the PPM is out of range
      if (sensor::tds == 0) {
        Serial.println(F("PPM reading equals zero, skipping PPM dosing"));
        targetTds = sensor::tds;
      }
      else if (sensor::tds < user::targetMinTds - user::tdsOffset && user::convertToTds) {
        targetTds = user::targetMinTds;
        tdsPercent = percentOutOfRange(targetTds, sensor::tds);
        Serial.print(F("PPM is lower than min by ")); Serial.print(tdsPercent); Serial.println(F("%"));
      }
      else {
        targetTds = sensor::tds;
      }

      // Check if the EC is out of range
      if (sensor::ec == 0) {
        Serial.println(F("EC reading equals zero, skipping EC dosing"));
        targetEc = sensor::ec;
      }
      else if (sensor::ec < user::targetMinEc - user::ecOffset && !user::convertToTds) {
        targetEc = user::targetMinEc;
        ecPercent = percentOutOfRange(targetEc, sensor::ec);
        Serial.print(F("EC is lower than min by ")); Serial.print(ecPercent); Serial.println(F("%"));
      }
      else {
        targetEc = sensor::ec;
      }

      // Check if the PH is out of range
      if (sensor::ph == 0) {
        Serial.println(F("PH reading equals zero, skipping PH dosing"));
        targetPh = sensor::ph;
      }
      else if (sensor::ph < user::targetMinPh - user::phOffset) {
        targetPh = user::targetMinPh;
        device::adjustPhDown = false;
        phPercent = percentOutOfRange(targetPh, sensor::ph);
        Serial.println(F("PH is lower than min"));
      }
      else if (sensor::ph > user::targetMaxPh + user::phOffset) {
        targetPh = user::targetMaxPh;
        device::adjustPhDown = true;
        phPercent = percentOutOfRange(targetPh, sensor::ph);
        Serial.println(F("PH is higher than max"));
      }
      else {
        Serial.println(F("PH is in range"));
        targetPh = sensor::ph;
      }
      if (phPercent > 0)
        Serial.print(F("PH is out of range by ")); Serial.print(phPercent); Serial.println(F("%"));


      // Check if a mode has ran up to the swapInterval limit, if so we reset the cout and set the precent of out range to 0,
      // Thus it will skip the comparison later and adjust the other sensors enviroment.
      if (phPercent != 0 && (ecPercent > 0 || phPercent > 0) && phDoseCount >= user::swapInterval) {
        phPercent = 0;
        phDoseCount = 0;
        Serial.println(F("Reach max trys setting PH % to 0 !"));
      }
      else if (ecPercent != 0 && (phPercent > 0 || tdsPercent > 0) && ecDoseCount >= user::swapInterval) {
        ecPercent = 0;
        ecDoseCount = 0;
        Serial.println(F("Reach max trys setting EC % to 0 !"));
      }
      else if (tdsPercent != 0 && (phPercent > 0 || ecPercent > 0) && tdsDoseCount >= user::swapInterval) {
        tdsPercent = 0;
        tdsDoseCount = 0;
        Serial.println(F("Reach max trys setting PPM % to 0 !"));
      }

      //
      sensorPercent = phPercent;
      if (ecPercent > phPercent) {
        sensorPercent = ecPercent;
        dosingMode = 1;
      }
      else if (tdsPercent > ecPercent) {
        sensorPercent = tdsPercent;
        dosingMode = 2;
      }
      Serial.print(F("sensorPercent")); Serial.println(sensorPercent);
      Serial.print(F("Dosing mode")); Serial.println(dosingMode);

      if (sensorPercent > 0) {
        // Display a notification to cancel or continue with the starting of the dosing
        launchDosingNotification(sensorPercent, dosingMode, lastTouch);

        // Work out dosing amount
        int dosingAmount = 0;
        if (device::continueDosing != device::CANCEL) {
          // ====== PH ======
          if (dosingMode == 0) {
            Serial.println(F("Setting up PH dosing"));
            if (phDoseCount < 255)
              phDoseCount++;
            ecDoseCount = 0;
            tdsDoseCount = 0;
            // Work out the correct amount of PH down solution to dose
            if (device::adjustPhDown) {
              saveLogMessage(8);
              if (user::doserTwoMode == device::DOSER_PH) {
                dosingAmount += user::doserTwoMills;
                startDoser[1] = true;
              }
              if (user::doserFourMode == device::DOSER_PH) {
                dosingAmount += user::doserFourMills;
                startDoser[3] = true;
              }
              if (user::numberOfDosers >= 6 && user::doserSixMode == device::DOSER_PH) {
                dosingAmount += user::doserSixMills;
                startDoser[5] = true;
              }
              loadMachineLearning(sensor::ph, machineLearning::previousPhDownSensor, machineLearning::phDownSensorHistory, user::targetMinPh, machineLearning::logPhDownResult, machineLearning::phDownDosingInc,
                                  machineLearning::phDownArrayBlock, machineLearning::savePhDownResult , dosingAmount, machineLearning::phDownMls, machineLearning::phDownMultipler, machineLearning::previousPhDownMls, true);
              mlsMultipler = machineLearning::phDownMultipler;
            }
            // Else work out the correct amount of PH up solution to dose
            else {
              saveLogMessage(7);
              if (user::doserOneMode == device::DOSER_PH) {
                dosingAmount += user::doserThreeMills;
                startDoser[0] = true;
              }
              if (user::doserThreeMode == device::DOSER_PH) {
                dosingAmount += user::doserThreeMills;
                startDoser[2] = true;
              }
              if (user::numberOfDosers >= 5 && user::doserFiveMode == device::DOSER_PH) {
                dosingAmount += user::doserFiveMills;
                startDoser[4] = true;
              }
              loadMachineLearning(sensor::ph, machineLearning::previousPhUpSensor, machineLearning::phUpSensorHistory, user::targetMaxPh , machineLearning::logPhUpResult, machineLearning::phUpDosingInc,
                                  machineLearning::phUpArrayBlock, machineLearning::savePhUpResult , dosingAmount, machineLearning::phUpMls, machineLearning::phUpMultipler, machineLearning::previousPhUpMls, true);
              mlsMultipler = machineLearning::phUpMultipler;
            }
            if (user::disableMachineLearning)
              mlsMultipler = 1;
          }

          // ====== EC ======
          else if (dosingMode == 1) {
            Serial.println(F("Setting up EC dosing"));
            saveLogMessage(9);
            if (ecDoseCount < 255)
              ecDoseCount++;
            phDoseCount = 0;
            tdsDoseCount = 0;
            // Work out the correct amount of solution to dose
            if (user::doserOneMode == device::DOSER_EC) {
              dosingAmount += user::doserThreeMills;
              startDoser[0] = true;
            }
            if (user::doserTwoMode == device::DOSER_EC) {
              dosingAmount += user::doserThreeMills;
              startDoser[1] = true;
            }
            if (user::doserThreeMode == device::DOSER_EC) {
              dosingAmount += user::doserThreeMills;
              startDoser[2] = true;
            }
            if (user::doserFourMode == device::DOSER_EC) {
              dosingAmount += user::doserFourMills;
              startDoser[3] = true;
            }
            if (user::numberOfDosers >= 5 && user::doserFiveMode == device::DOSER_EC) {
              dosingAmount += user::doserFiveMills;
              startDoser[4] = true;
            }
            if (user::numberOfDosers >= 6 && user::doserSixMode == device::DOSER_EC) {
              dosingAmount += user::doserSixMills;
              startDoser[5] = true;
            }
            loadMachineLearning(sensor::ec, machineLearning::previousEcSensor, machineLearning::ecSensorHistory, user::targetMaxEc , machineLearning::logEcResult, machineLearning::ecDosingInc,
                                machineLearning::ecArrayBlock, machineLearning::saveEcResult, dosingAmount, machineLearning::ecMls, machineLearning::ecMultipler, machineLearning::previousEcMls, true);
            mlsMultipler = machineLearning::ecMultipler;
            if (user::disableMachineLearning)
              mlsMultipler = 1;
          }

          // ====== PPM ======
          else if (dosingMode == 2) {
            Serial.println(F("Setting up PPM dosing"));
            saveLogMessage(10);
            if (tdsDoseCount < 255)
              tdsDoseCount++;
            phDoseCount = 0;
            ecDoseCount = 0;
            // Work out the correct amount of solution to dose
            if (user::doserOneMode == device::DOSER_EC) {
              dosingAmount += user::doserThreeMills;
              startDoser[0] = true;
            }
            if (user::doserTwoMode == device::DOSER_EC) {
              dosingAmount += user::doserThreeMills;
              startDoser[1] = true;
            }
            if (user::doserThreeMode == device::DOSER_EC) {
              dosingAmount += user::doserThreeMills;
              startDoser[2] = true;
            }
            if (user::doserFourMode == device::DOSER_EC) {
              dosingAmount += user::doserFourMills;
              startDoser[3] = true;
            }
            if (user::numberOfDosers >= 5 && user::doserFiveMode == device::DOSER_EC) {
              dosingAmount += user::doserFiveMills;
              startDoser[4] = true;
            }
            if (user::numberOfDosers >= 6 && user::doserSixMode == device::DOSER_EC) {
              dosingAmount += user::doserSixMills;
              startDoser[5] = true;
            }
            loadMachineLearning(sensor::tds, machineLearning::previousTdsSensor, machineLearning::tdsSensorHistory, user::targetMaxTds, machineLearning::logTdsResult, machineLearning::tdsDosingInc,
                                machineLearning::tdsArrayBlock, machineLearning::saveTdsResult, dosingAmount, machineLearning::tdsMls, machineLearning::tdsMultipler, machineLearning::previousTdsMls, true);
            mlsMultipler = machineLearning::tdsMultipler;
            if (user::disableMachineLearning)
              mlsMultipler = 1;
          }
          Serial.println(F("Starting dosing"));
          Serial.print(F("mlsMultipler: ")); Serial.println(mlsMultipler);
          device::currentlyDosing = true;
          previousDoserMillis = millis();
        }
      }
    }
  }
  // Run dosers
  byte previousDoserNum = 0;
  device::currentDoserNum = 0;
  const char* str[3] = {"PH", "EC", "PPM"};
  while (device::currentlyDosing) { // timing is critical we must use a while loop
    // show continue dialog
    if (device::currentDoserNum != previousDoserNum) {
      abortMessage(message::cancelDosing, str[dosingMode], sensorPercent, device::currentDoserNum, 2);
      previousDoserNum = device::currentDoserNum;
    }
    if (startDoser[0])
      startDoser[0] = runDoser(1, pin::doserOne, user::doserOneSpeed, user::doserOneMills * mlsMultipler, previousDoserMillis);
    else if (startDoser[1])
      startDoser[1] = runDoser(2, pin::doserTwo, user::doserTwoSpeed, user::doserTwoMills * mlsMultipler, previousDoserMillis);
    else if (startDoser[2])
      startDoser[2] = runDoser(3, pin::doserThree, user::doserThreeSpeed, user::doserThreeMills * mlsMultipler, previousDoserMillis);
    else if (startDoser[3])
      startDoser[3] = runDoser(4, pin::doserFour, user::doserFourSpeed, user::doserFourMills * mlsMultipler, previousDoserMillis);
    else if (startDoser[4])
      startDoser[4] = runDoser(5, pin::doserFive, user::doserFiveSpeed, user::doserFiveMills * mlsMultipler, previousDoserMillis);
    else if (startDoser[5])
      startDoser[5] = runDoser(6, pin::doserSix, user::doserSixSpeed, user::doserSixMills * mlsMultipler, previousDoserMillis);
    else if (device::currentlyDosing) {
      Serial.println(F("Dosing complete"));
      device::currentlyDosing = false;
      clearPage();
      display::refreshPage = true;
    }
    // touch event - cancel button
    const uint16_t startX = 166, startY = 166;
    if (tft.touched()) {
      tft.touchReadPixel(&display::touch_x, &display::touch_y);
      if (millis() >= lastTouch) {
        if (display::touch_x >= startX + 200 && display::touch_x <= startX + 400 && display::touch_y >= startY + 200 && display::touch_y <= startY + 250) { // Cancel
          Serial.println(F("Dosing aborted"));
          beep();
          device::currentlyDosing = false;
          for (bool& val : startDoser)
            val = false;
          clearPage();
          display::refreshPage = true;
        }
      }
    }
  }
}

// Return the percent out of range
float percentOutOfRange(const float & a_setPoint, const float & a_val) {
  float outOfRange = abs(a_setPoint - a_val);
  float percent = a_setPoint / 100.0;
  return outOfRange / percent;
}

// Run a given doser for a_mls * 1000 on a_doserPin at a_doserSpeed
bool runDoser(const byte & a_doserNum, const byte & a_doserPin, const byte & a_doserSpeed, const unsigned long & a_mls, unsigned long & a_previousDoserMillis) {
  static unsigned long debugPreviousMillis = 0; // JUST FOR TESTING !
  if (millis() - a_previousDoserMillis <= 1000UL * a_mls) {
    if (a_previousDoserMillis != debugPreviousMillis) { // JUST FOR TESTING !  ----->
      Serial.print(F("Starting doser: ")); Serial.print(a_doserNum); Serial.print(F(" pumping ")); Serial.print(a_mls); Serial.println(F("mls"));
      debugPreviousMillis = a_previousDoserMillis;
    } // <---
    analogWrite(a_doserPin, a_doserSpeed * 2.55);
    device::currentDoserNum = a_doserNum;
  }
  else {
    Serial.print(F("Stopping doser: ")); Serial.println(a_doserNum); // JUST FOR TESTING !
    Serial.print(F("Doser run time: ")); Serial.println(millis() - a_previousDoserMillis); // JUST FOR TESTING !
    analogWrite(a_doserPin, 0);
    a_previousDoserMillis = millis();
    return false;
  }
  return true;
}
