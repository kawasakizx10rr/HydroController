void loadMachineLearning(const float& a_sensor, float& a_previousSensor, const float& a_sensorAdjustment, const float& a_sensorTarget, bool& a_logSensor, unsigned int* a_dosingIncrementArray,
                         byte& a_resultArrayBlock, bool& a_saveResult, const int& a_mls, int& a_newDoserMls, float& a_dosingMultipler, int& a_previousMls, const bool& a_compressData)
{
  if (!a_saveResult) {
    a_previousSensor = a_sensor;
    a_previousMls = a_mls;
    a_logSensor = true;
    Serial.print(F("Current sensor: ")); Serial.println(a_sensor);
    Serial.print(F("Current Dosing Mls: ")); Serial.println(a_previousMls); Serial.println();
  }
  else if (a_sensorAdjustment != 0) {
    // Work out the changes since the last dosing
    const float dosingResult = abs(a_previousSensor - a_sensorAdjustment); // a_sensor
    const float incrementPerMl = dosingResult / a_previousMls;
    const float currentDosingMargin = abs(a_sensor - a_sensorTarget);
    Serial.print(F("Previous sensor: ")); Serial.println(a_previousSensor);
    Serial.print(F("Logged sensor: ")); Serial.println(a_sensorAdjustment);
    Serial.print(F("Dosing Increment: ")); Serial.println(dosingResult);
    Serial.print(F("Increment Per Ml: ")); Serial.println(incrementPerMl);
    Serial.print(F("Current Dosing Margin: ")); Serial.println(currentDosingMargin);
    // Save increment per ml to the given modes array.
    if (a_resultArrayBlock < machineLearning::maxSize) {
      a_dosingIncrementArray[a_resultArrayBlock] = a_compressData ? doubleToInt(incrementPerMl) : incrementPerMl;
      a_resultArrayBlock++;
    }
    else {
      for (byte i = 0; i < machineLearning::maxSize - 1; i++) {
        a_dosingIncrementArray[i] = a_dosingIncrementArray[i + 1];
      }
      a_dosingIncrementArray[0] = a_compressData ? doubleToInt(incrementPerMl) : incrementPerMl;
    }
    // Get the new dosing ml's based on the previous calculations saved history
    a_newDoserMls = currentDosingMargin / averageResults(a_dosingIncrementArray, a_compressData);
    a_dosingMultipler = a_newDoserMls / a_mls;
    if (a_dosingMultipler == 0)
      a_dosingMultipler = 1;
    Serial.print(F("New Doser Mls = ")); Serial.println(a_newDoserMls);
    Serial.print(F("New Doser Multiplier = ")); Serial.println(a_dosingMultipler); Serial.println();
    //
    a_previousSensor = a_sensor;
    a_previousMls = a_newDoserMls;
    a_logSensor = true;
  }
  a_saveResult = true;
}
