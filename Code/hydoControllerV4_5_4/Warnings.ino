// This function works out if there is a warning to display
// based on the sensor values compared against the users target
// setpoints +/- offset +/- errorMargin
void displayWarnings() {
  byte errorState = 0;
  static byte previousErrorState = 0;
  static unsigned long prevWarnMillis = millis();
  if (device::sensorsReady && millis() - prevWarnMillis >= 1000UL) {
    setErrorState(sensor::ec, user::targetMinEc, user::targetMaxEc, user::ecErrorMargin, user::disableEcWarnings, errorState, device::ecErrorState);
    setErrorState(sensor::tds, user::targetMinTds, user::targetMaxTds, user::tdsErrorMargin, user::disableEcWarnings, errorState, device::ecErrorState);
    setErrorState(sensor::ph, user::targetMinPh, user::targetMaxPh, user::phErrorMargin, user::disablePhWarnings, errorState, device::phErrorState);
    if (!user::convertToF)
      setErrorState(sensor::waterTemp, user::targetMinWaterTemp, user::targetMaxWaterTemp, user::waterTempErrorMargin, user::disableWaterTempWarnings, errorState, device::waterTempErrorState);
    else
      setErrorState(convertToF(sensor::waterTemp), user::targetMinWaterTempF, user::targetMaxWaterTempF, user::waterTempErrorMarginF, user::disableWaterTempWarnings, errorState, device::waterTempErrorState);
    if (!user::convertToInches)
      setErrorState(sensor::waterLevel, user::targetMinWaterHeight, user::targetMaxWaterHeight, user::waterHeightErrorMargin, user::disableWaterHeightWarnings, errorState, device::waterLevelErrorState);
    else
      setErrorState(sensor::waterLevelInches, user::targetMinWaterHeightInches, user::targetMaxWaterHeightInches, user::waterHeightErrorMarginInches, user::disableWaterHeightWarnings, errorState, device::waterLevelErrorState);
    if (!user::convertToF)
      setErrorState(sensor::airTemp, user::targetMinAirTemp, user::targetMaxAirTemp, user::airTempErrorMargin, user::disableAirTempWarnings, errorState, device::airTempErrorState);
    else
      setErrorState(convertToF(sensor::airTemp), user::targetMinAirTempF, user::targetMaxAirTempF, user::airTempErrorMarginF, user::disableAirTempWarnings, errorState, device::airTempErrorState);
    setErrorState(sensor::humidity, user::targetMinHumidity, user::targetMaxHumidity, user::humidityErrorMargin, user::disableHumidityWarnings, errorState, device::humidityErrorState);
    //
    if (sensor::co2 <= (user::targetCo2 - user::co2Offset - user::co2ErrorMargin)) {
      if (!user::disableCo2Warnings)
        errorState = device::MAJOR_WARNING;
      device::co2ErrorState = device::MAJOR_WARNING;
    }
    else if (sensor::co2 < user::targetCo2) {
      if (!user::disableCo2Warnings && errorState != device::MAJOR_WARNING)
        errorState = device::MINOR_WARNING;
      device::co2ErrorState = device::MINOR_WARNING;
    }
    else {
      device::co2ErrorState = device::NO_WARNING;
    }
    // Send alarm state to 328P
    if (errorState != previousErrorState) {
      if (errorState == device::NO_WARNING)
        sendToSlave('A', device::NO_WARNING); // no alarm
      else if (errorState == device::MINOR_WARNING)
        sendToSlave('A', device::MINOR_WARNING); // minor alarm
      else if (errorState == device::MAJOR_WARNING)
        sendToSlave('A', device::MAJOR_WARNING); // major alarm
      previousErrorState = errorState;
    }
    prevWarnMillis = millis();
  }
}

void setErrorState(const float& a_sensor, const float& a_targetMin, const float& a_targetMax, const float& a_errorMargin, const float& a_disableWarning, byte& a_globalErrorState, byte& a_userErrorState) {
  if (a_sensor <= (a_targetMin - a_errorMargin) || a_sensor >= (a_targetMax + a_errorMargin)) {
    if (!a_disableWarning)
      a_globalErrorState = device::MAJOR_WARNING;
    a_userErrorState = device::MAJOR_WARNING;
  }
  else if (a_sensor < a_targetMin || a_sensor > a_targetMax) {
    if (!a_disableWarning && a_globalErrorState != device::MAJOR_WARNING)
      a_globalErrorState = device::MINOR_WARNING;
    a_userErrorState = device::MINOR_WARNING;
  }
  else {
    a_userErrorState = device::NO_WARNING;
  }
}
