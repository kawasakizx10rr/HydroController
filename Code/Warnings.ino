// This function works out if there is a warning to display
// based on the sensor values compared against the users target
// setpoints +/- offset +/- errorMargin
void displayWarnings() {
  device::globalErrorState = device::NO_WARNING;
  static uint8_t previousErrorState = 0;
  static unsigned long prevWarnMillis = millis();
  if (device::sensorsReady && millis() - prevWarnMillis >= 1000UL) {
    setErrorState(sensor::ec, user::targetMinEc, user::targetMaxEc, user::ecErrorMargin, user::disableEcWarnings, device::ecErrorState);
    setErrorState(sensor::tds, user::targetMinTds, user::targetMaxTds, user::tdsErrorMargin, user::disableEcWarnings, device::ecErrorState);
    setErrorState(sensor::ph, user::targetMinPh, user::targetMaxPh, user::phErrorMargin, user::disablePhWarnings, device::phErrorState);
    if (!user::convertToF)
      setErrorState(sensor::waterTemp, user::targetMinWaterTemp, user::targetMaxWaterTemp, user::waterTempErrorMargin, user::disableWaterTempWarnings, device::waterTempErrorState);
    else
      setErrorState(convertToF(sensor::waterTemp), user::targetMinWaterTempF, user::targetMaxWaterTempF, user::waterTempErrorMarginF, user::disableWaterTempWarnings, device::waterTempErrorState);
    if (!user::convertToInches)
      setErrorState(sensor::waterLevel, user::targetMinWaterHeight, user::targetMaxWaterHeight, user::waterHeightErrorMargin, user::disableWaterHeightWarnings, device::waterLevelErrorState);
    else
      setErrorState(sensor::waterLevelInches, user::targetMinWaterHeightInches, user::targetMaxWaterHeightInches, user::waterHeightErrorMarginInches, user::disableWaterHeightWarnings, device::waterLevelErrorState);
    if (!user::convertToF)
      setErrorState(sensor::airTemp, user::targetMinAirTemp, user::targetMaxAirTemp, user::airTempErrorMargin, user::disableAirTempWarnings, device::airTempErrorState);
    else
      setErrorState(convertToF(sensor::airTemp), user::targetMinAirTempF, user::targetMaxAirTempF, user::airTempErrorMarginF, user::disableAirTempWarnings, device::airTempErrorState);
    setErrorState(sensor::humidity, user::targetMinHumidity, user::targetMaxHumidity, user::humidityErrorMargin, user::disableHumidityWarnings, device::humidityErrorState);
    //
    if (sensor::co2 <= (user::targetCo2 - user::co2Offset - user::co2ErrorMargin)) {
      if (!user::disableCo2Warnings)
        device::globalErrorState = device::MAJOR_WARNING;
      device::co2ErrorState = device::MAJOR_WARNING;
    }
    else if (sensor::co2 < user::targetCo2) {
      if (!user::disableCo2Warnings && device::globalErrorState != device::MAJOR_WARNING)
        device::globalErrorState = device::MINOR_WARNING;
      device::co2ErrorState = device::MINOR_WARNING;
    }
    else {
      device::co2ErrorState = device::NO_WARNING;
    }
    // Send alarm state to 328P
    if (device::globalErrorState != previousErrorState) {
      if (device::globalErrorState == device::NO_WARNING)
        sendToSlave('A', device::NO_WARNING); // no alarm
      else if (device::globalErrorState == device::MINOR_WARNING)
        sendToSlave('A', device::MINOR_WARNING); // minor alarm
      else if (device::globalErrorState == device::MAJOR_WARNING)
        sendToSlave('A', device::MAJOR_WARNING); // major alarm
      previousErrorState = device::globalErrorState;
    }
    prevWarnMillis = millis();
  }
}

void setErrorState(const float& a_sensor, const float& a_targetMin, const float& a_targetMax, const float& a_errorMargin, const float& a_disableWarning, uint8_t& a_userErrorState) {
  if (a_sensor <= (a_targetMin - a_errorMargin) || a_sensor >= (a_targetMax + a_errorMargin)) {
    if (!a_disableWarning)
      device::globalErrorState = device::MAJOR_WARNING;
    a_userErrorState = device::MAJOR_WARNING;
  }
  else if (a_sensor < a_targetMin || a_sensor > a_targetMax) {
    if (!a_disableWarning && device::globalErrorState != device::MAJOR_WARNING)
      device::globalErrorState = device::MINOR_WARNING;
    a_userErrorState = device::MINOR_WARNING;
  }
  else {
    a_userErrorState = device::NO_WARNING;
  }
}