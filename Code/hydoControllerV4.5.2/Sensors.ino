void readSensors() {
  static byte swapSenor;
  static unsigned long sensorPreviousMillis = 0;
  if (millis() - sensorPreviousMillis >= 2500UL) {
    // WATER TEMP =========================================================================
    dallasTemperature.requestTemperatures();
    sensor::waterTemp = dallasTemperature.getTempCByIndex(0);
    sensor::waterTempF = dallasTemperature.getTempFByIndex(0);
    //Serial.print(F("water temp in c:")); Serial.println(sensor::waterTemp);
    if (sensor::waterTemp < 0)
      sensor::waterTemp = 0;
    if (sensor::waterTempF < 0)
      sensor::waterTempF = 0;
    // WATER LEVEL ========================================================================
    if (user::useEtapeSensor) {
      float resistance = readResistance(pin::etapeSensor, 560);
      sensor::waterLevel = resistanceToCM(resistance, sensor::etapeZeroVolumeResistance, sensor::etapeMaxVolumeResistance, sensor::etapeCalibrationCm);
      sensor::waterLevel += sensor::etapeOffset;
      if (sensor::waterLevel > sensor::etapeCalibrationCm)
        sensor::waterLevel = sensor::etapeCalibrationCm;
      sensor::waterLevelInches = sensor::waterLevel / 2.5;
    }
    else {
      digitalWrite(pin::hcsrTrigger, LOW);
      delayMicroseconds(5); //5
      digitalWrite(pin::hcsrTrigger, HIGH);
      delayMicroseconds(10);
      digitalWrite(pin::hcsrTrigger, LOW);
      sensor::hcsrDuration = pulseIn(pin::hcsrEcho, HIGH);
      if (sensor::emptyWaterTankDepth > 0)
        sensor::waterLevel = sensor::emptyWaterTankDepth - ((sensor::hcsrDuration / 2) / 29.1); // in cm
      else
        sensor::waterLevel = (sensor::hcsrDuration / 2) / 29.1;
      if (sensor::emptyWaterTankDepth > 0)
        sensor::waterLevelInches = (sensor::emptyWaterTankDepth / 2.5) - ((sensor::hcsrDuration / 2) / 74.0); // in Inches
      else
        sensor::waterLevelInches = (sensor::hcsrDuration / 2) / 74.0; // in Inches
      //Serial.print(F("emptyWaterTankDepth:")); Serial.println(sensor::emptyWaterTankDepth);
      //Serial.print(F("Water height in cm:")); Serial.println(sensor::waterLevel);
    }
    if (sensor::waterLevel < 0)
      sensor::waterLevel = 0;
    if (sensor::waterLevelInches < 0)
      sensor::waterLevelInches = 0;
    // AIR TEMP ==========================================================================
    DHT.read22(pin::dht22);
    sensor::airTemp = DHT.temperature;
    //sensor::airTemp = hdc.getTemp();    
    //Serial.print(F("Air temp in c:")); Serial.println(sensor::airTemp);
    if (sensor::airTemp < 0)
      sensor::airTemp = 0;
    // HUMIDITY ==========================================================================
    sensor::humidity = DHT.humidity;
    //sensor::humidity = hdc.getHumidity();
    //Serial.print(F("Humidity:")); Serial.println(sensor::humidity);
    if (sensor::humidity > 99.9)
      sensor::humidity = 99.9;
    // LDR ===============================================================================
    sensor::ldr = analogRead(pin::ldr);
    //Serial.print(F("LDR: ")); Serial.println(sensor::ldr);
    // CO2 ===============================================================================
    const unsigned long responceWindow = millis() + 50UL;
    Serial2.write(sensor::co2Request, 9);
    byte buffer[8] {0, 0, 0, 0, 0, 0, 0, 0};
    byte bufferPosition = 0;
    while (millis() <= responceWindow && Serial2.available() > 0) {
      char c = Serial2.read();
      if (bufferPosition < 8)
        buffer[bufferPosition++] = c;
    }
    float sensorValue = 0;
    if (buffer[0] == 255 && buffer[5] == 0 && buffer[7] == 0)
      sensorValue = (256 * (int)buffer[2]) + (int)buffer[3];
    if (sensorValue <= 0) {
      Serial.print(F("Error reading Co2 sensor buffer:"));
      for (const byte& val : buffer) {
        Serial.print(val); Serial.print(F(","));
      }
      Serial.println();
      sensor::co2 = 0;
    }
    else if (sensorValue < 300) {
      Serial.print("Co2 sensor currently preheating..."); Serial.print(F("co2:")); Serial.println(sensorValue);
      sensor::co2 = 0;
    }
    else {
      sensor::co2 = sensorValue;
      //Serial.print(F("co2:")); Serial.println(sensor::co2);
    }
    if (sensor::co2 < 0)
      sensor::co2 = 0;
    else if (sensor::co2 > 9999)
      sensor::co2 = 9999;
    // EC & PPM / TDS ===================================================================
    if (swapSenor == 0) {
      if (!device::ecWaitTillNextCall) {
        digitalWrite(pin::tdsTransistor, HIGH);  // turn on ec sensor
        device::ecWaitTillNextCall = true;
      }
      else {
        static float rawTdsArray[10];
        static byte tdsCnt = 0;
        float voltage = analogRead(pin::tdsSensor) * device::aref / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
        float temperatureCoefficient = 1.0 + 0.02 * (sensor::waterTemp - 25.0); // temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
        float uS = (133.42 * pow(voltage, 3) - 255.86 * pow(voltage, 2) + 857.39 * voltage) * sensor::ecCalibration;
        float uS25 = uS / temperatureCoefficient; // temperature and calibration compensation
        sensor::tds = uS25 * 0.5; // convert ec value to tds value
        //Serial.print(F("Raw TDS:")); Serial.println(sensor::tds);
        //
        if (tdsCnt < 10) {
          rawTdsArray[tdsCnt++] = sensor::tds;
        }
        else {
          for (byte i = 0; i < 9; i++) {
            rawTdsArray[i] = rawTdsArray[i + 1];
          }
          rawTdsArray[9] = sensor::tds;
        }
        sensor::tds = averageArray(rawTdsArray, tdsCnt);
        //
        if (device::conversionType == device::US)
          sensor::ec = (sensor::tds / 0.50) / 1000;
        else if (device::conversionType == device::AU)
          sensor::ec = (sensor::tds / 0.70) / 1000;
        else
          sensor::ec = (sensor::tds / 0.64) / 1000;
        if (machineLearning::logEcResult && sensor::ec > machineLearning::previousEcSensor)
          machineLearning::ecSensorHistory = sensor::ec;
        else if (machineLearning::logTdsResult && sensor::tds > machineLearning::previousTdsSensor)
          machineLearning::tdsSensorHistory = sensor::tds;
        //Serial.print(F("TDS Voltage:")); Serial.println(voltage);
        //Serial.print(F("TDS:")); Serial.println(sensor::tds);
        //Serial.print(F("EC:")); Serial.println(sensor::ec);
        if (sensor::tds > 9999)
          sensor::tds = 9999;
        else if (sensor::tds < 0)
          sensor::tds = 0;
        if (sensor::ec > 5)
          sensor::ec = 5;
        else if (sensor::ec < 0)
          sensor::ec = 0;
        swapSenor = 1;
        device::ecWaitTillNextCall = false;
        digitalWrite(pin::tdsTransistor, LOW); // turn off ec sensor
      }
    }
    // PH ==============================================================================
    else if (swapSenor == 1) {
      if (!device::phWaitTillNextCall) {
        digitalWrite(pin::phTransistor, HIGH); // turn on sensor
        device::phWaitTillNextCall = true;
      }
      else {
        //float temperatureCoefficient = 1.0 + 0.02 * (sensor::waterTemp - 25.0);
        //Serial.print(F("waterTemp:")); Serial.println(waterTemp);
        static byte phCnt = 0;
        static float rawPhArray[10];
        sensor::phVoltage = analogRead(pin::phSensor) * device::aref / 1024.0;
        sensor::ph = 7 + ((sensor::ph7Voltage -  sensor::phVoltage) / sensor::phVoltagePerUnit);  // TO DO: PH temperature compensation
        //Serial.print(F("Raw PH:")); Serial.println(sensor::ph);
        if (phCnt < 10) {
          rawPhArray[phCnt++] = sensor::ph;
        }
        else {
          for (byte i = 0; i < 9; i++) {
            rawPhArray[i] = rawPhArray[i + 1];
          }
          rawPhArray[9] = sensor::ph;
        }
        sensor::ph = averageArray(rawPhArray, phCnt);
        //Serial.print(F("PH:")); Serial.println(sensor::ph);
        //
        if (sensor::ph > 14)
          sensor::ph = 14.0;
        else if (sensor::ph < 0)
          sensor::ph = 0;
        if (machineLearning::logPhDownResult && sensor::ph < machineLearning::previousPhDownSensor)
          machineLearning::phDownSensorHistory = sensor::ph;
        else if (machineLearning::logPhUpResult && sensor::ph > machineLearning::previousPhUpSensor)
          machineLearning::phUpSensorHistory = sensor::ph;
        device::sensorsReady = true;
        swapSenor = 0;
        device::phWaitTillNextCall = false;
        digitalWrite(pin::phTransistor, LOW); // turn off sensor
      }
    }
    if (device::sensorsReady) {
      pushBackDataArrays();
      setMaxMins();
      device::newGraphData = true;
    }
    sensorPreviousMillis = millis();
  }
}

void readTdsQuick() {
  // turn the ph sensor off
  digitalWrite(pin::phTransistor, LOW);
  // Get water temperature in celsius
  dallasTemperature.requestTemperatures();
  sensor::waterTemp = dallasTemperature.getTempCByIndex(0);
  // Turn on the TDS sensor
  digitalWrite(pin::tdsTransistor, HIGH);
  // allow sensor to power up
  delay(500);
  // Read the TDS / EC
  float voltage = analogRead(pin::tdsSensor) * device::aref / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
  float temperatureCoefficient = 1.0 + 0.02 * (sensor::waterTemp - 25.0); // temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
  float uS = (133.42 * pow(voltage, 3) - 255.86 * pow(voltage, 2) + 857.39 * voltage) * sensor::ecCalibration;
  float uS25 = uS / temperatureCoefficient; // temperature and calibration compensation
  sensor::tds = uS25 * 0.5; // convert ec value to tds value
  if (device::conversionType == device::US)
    sensor::ec = (sensor::tds / 0.50) / 1000;
  else if (device::conversionType == device::AU)
    sensor::ec = (sensor::tds / 0.70) / 1000;
  else
    sensor::ec = (sensor::tds / 0.64) / 1000;
  delay(100);
  // Print the results
  //Serial.print(F("ec = ")); Serial.println(sensor::ec);
  //Serial.print(F("tds = ")); Serial.println(sensor::tds);
  // Turn off the TDS sensor
  digitalWrite(pin::tdsTransistor, LOW);
  // Check if the values are out of range
  if (sensor::tds > 9999)
    sensor::tds = 9999;
  else if (sensor::tds < 0)
    sensor::tds = 0;
  if (sensor::ec > 5)
    sensor::ec = 5.0;
  else if (sensor::ec < 0)
    sensor::ec = 0;
  if (sensor::waterTemp < 0)
    sensor::waterTemp = 0;
  if (sensor::waterTempF < 0)
    sensor::waterTempF = 0;
  digitalWrite(pin::tdsTransistor, LOW); // turn off ec sensor
}

void setAveragePhVoltage(float& a_voltage) {
  float minPhVoltage = 0, maxPhVoltage = 0;
  if (device::ecWaitTillNextCall) {
    digitalWrite(pin::tdsTransistor, LOW);
    delay(100);
  }
  digitalWrite(pin::phTransistor, HIGH);
  delay(2500);
  Serial.print(F("PH voltage reading: "));
  for (byte i = 0; i < 30; i++) {
    float voltage = analogRead(pin::phSensor) * 5.0 / 1024.0;
    if (voltage < minPhVoltage || minPhVoltage == 0)
      minPhVoltage = voltage;
    if (voltage > maxPhVoltage || maxPhVoltage == 0)
      maxPhVoltage = voltage;
    Serial.print(i); Serial.print(F(" of 30, "));
    delay(2000);
  }
  Serial.println(F("30 of 30"));
  digitalWrite(pin::phTransistor, LOW);
  if (device::ecWaitTillNextCall) {
    delay(100);
    digitalWrite(pin::tdsTransistor, HIGH);
  }
  a_voltage = (minPhVoltage + maxPhVoltage) / 2.0;
}

void setPh7Voltage() {
  sensor::ph7Voltage = 0;
  sensor::phPotStep = 0;
  sendToSlave('P', sensor::phPotStep);
  while (sensor::ph7Voltage < 2.5 && sensor::phPotStep < 254) {
    setAveragePhVoltage(sensor::ph7Voltage);
    if (sensor::ph7Voltage < 2.5) {
      sendToSlave('P', sensor::phPotStep++);
      Serial.print(F("The PH 7 voltage is less than 2.5 volts, incrementing MCP4151 step to: ")); Serial.print(sensor::phPotStep); Serial.println(F(" of 120"));
    }
  }
  Serial.print(F("Reading complete, MCP4151 step = ")); Serial.print(sensor::phPotStep); Serial.print(F(", PH 7 voltage = ")); Serial.println(sensor::ph7Voltage, 4);
}

void phTwoPartCalibration() {
  sensor::phVoltagePerUnit = abs((sensor::ph7Voltage - sensor::ph4Voltage) / (sensor::ph7BufferValue - sensor::ph4BufferValue));
  Serial.print(F("PH phVoltagePerUnit = ")); Serial.println(sensor::phVoltagePerUnit, 4);
}

// calibrate the co2 sensor, by pulling the cal pin low for 10 seconds then back to high
void calibrateCo2() {
  if (display::refreshPage)
    continueMessage(message::calibratingCo2, -1, 0, false, false, false);
  const byte calibrateCo2[] {0XFF, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78};
  Serial2.write(calibrateCo2, 9);
  display::refreshPage = true;
  clearPage();
}

int getWaterHeight() {
  static unsigned long sensorPreviousMillis = 0;
  int waterLevel = 0;
  if (millis() - sensorPreviousMillis >= 1000UL) {
    if (user::useEtapeSensor) {
      float resistance = readResistance(pin::etapeSensor, 560);
      waterLevel = resistanceToCM(resistance, sensor::etapeZeroVolumeResistance, sensor::etapeMaxVolumeResistance, sensor::etapeCalibrationCm);
      waterLevel += sensor::etapeOffset;
      if (waterLevel > sensor::etapeCalibrationCm)
        waterLevel = sensor::etapeCalibrationCm;
    }
    else {
      digitalWrite(pin::hcsrTrigger, LOW);
      delayMicroseconds(5); //5
      digitalWrite(pin::hcsrTrigger, HIGH);
      delayMicroseconds(10);
      digitalWrite(pin::hcsrTrigger, LOW);
      float hcsrDuration = pulseIn(pin::hcsrEcho, HIGH);
      waterLevel = sensor::emptyWaterTankDepth - ((hcsrDuration / 2) / 29.1); // in cm
    }
    sensorPreviousMillis = millis();
    return waterLevel;
  }
  return -1;
}

// Get the water tanks empty depth in cm
void setTankDepth() {
  digitalWrite(pin::hcsrTrigger, LOW);
  delayMicroseconds(5);
  digitalWrite(pin::hcsrTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin::hcsrTrigger, LOW);
  sensor::hcsrDuration = pulseIn(pin::hcsrEcho, HIGH);
  float distance = (sensor::hcsrDuration / 2) / 29.1;   // Divide by 29.1 or multiply by 0.0343
  if (distance > 0 && distance < 999) {
    sensor::emptyWaterTankDepth = distance;
    Serial.print(F("EmptyWaterTankDepth = ")); Serial.println(sensor::emptyWaterTankDepth);
  }
  else {
    sensor::emptyWaterTankDepth = 0;
    Serial.println(F("Error: tank depth <= 0 or >= 999"));
  }
  saveSystemEEPROM();
}

// This is a generic Co2 formula tried and tested.
void generateCo2GasTime(void) {
  float roomHeight = 0.0328084 * user::roomHeightCm; // convert cm to feet
  float roomWidth = 0.0328084 * user::roomWidthCm; // convert cm to feet
  float roomLenght = 0.0328084 * user::roomLengthCm; // convert cm to feet
  float flowrate = user::co2FlowrateLtrs / 28.3168; // convert ltr to feet cubed
  if (user::convertToInches) {
    roomHeight = 0.0833333 * user::roomHeightInches; // convert inches to feet
    roomWidth = 0.0833333 * user::roomWidthInches; // convert inches to feet
    roomLenght = 0.0833333 * user::roomLengthInches; // convert inches to feet
    flowrate = user::co2FlowrateFeet3; // already in feet cubed
  }
  
  float roomsize = roomLenght * roomWidth * roomHeight;
  float co2PpmMargin = user::targetCo2 - sensor::co2;
  co2PpmMargin *= 0.000001;
  //flowrate = flowrate / 60.0; // If in hours
  float gasTime = (roomsize * co2PpmMargin) / flowrate;
  Serial.print(F("Gas time in minues = ")); Serial.println(gasTime);
  if (gasTime >= 999)
    gasTime = 999;
  sensor::co2GasTime = gasTime * 60000UL;
  Serial.print(F("Co2 gas time in millis = ")); Serial.println(sensor::co2GasTime);
}

void tdsCalibration() {
  Serial.println(F("Starting TDS sensor calibration"));
  // Turn the PH sensor off
  digitalWrite(pin::phTransistor, LOW);
  // Turn on the TDS sensor
  digitalWrite(pin::tdsTransistor, HIGH);
  // allow TDS sensor to power up
  delay(500);
  // Get the water temperature in celsius
  dallasTemperature.requestTemperatures();
  sensor::waterTemp = dallasTemperature.getTempCByIndex(0);
  // Print the current calibration solution value
  Serial.print(F("Raw EC calibration solution  = ")); Serial.print(sensor::ecCalibrationSolution); Serial.println(F("ppm"));
  // Calibration solution temperature compensation
  float temperatureCoefficient = 1.0 + 0.02 * (sensor::waterTemp - 25.0);
  float calSolution = sensor::ecCalibrationSolution / temperatureCoefficient;
  // Print the current calibration solution value
  Serial.print(F("Raw EC calibration solution with temperature correction = ")); Serial.print(calSolution); Serial.println(F("Us"));
  // Get the raw analog reading voltage
  float voltage = analogRead(pin::tdsSensor) * device::aref / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
  float ppm = ((133.42 * pow(voltage, 3) - 255.86 * pow(voltage, 2) + 857.39 * voltage) * 0.5) / temperatureCoefficient; //convert ec value to tds value
  Serial.print(F("PPM reading without calibration = ")); Serial.println(ppm);
  float KValueTemp = calSolution / ppm;
  // Check if the calibration values are in the expected ranges
  if (calSolution > 0 && calSolution < 2000 && KValueTemp > 0 && KValueTemp < 5) {
    Serial.println();
    Serial.println(F("Calibration Successful."));
    Serial.print(F("KValueTemp = ")); Serial.println(KValueTemp);
    sensor::ecCalibration = KValueTemp;
    saveSystemEEPROM();
  }
  // Else if the calibration values are NOT in the expected ranges, print the reason why
  else {
    Serial.println();
    Serial.println(F("Calibration Failed, Please try Again"));
    if (calSolution < 0 || calSolution > 2000) {
      Serial.print(F("(EC calibration solution * 2) is less than 0 or grater than 2000, value = ")); Serial.println(calSolution);
    }
    if (KValueTemp < 0 || KValueTemp > 5) {
      Serial.print(F("KValueTemp is less than 0 or greather than 4, value = ")); Serial.println(KValueTemp);
    }
    Serial.println();
  }
  // Turn off the TDS sensor
  digitalWrite(pin::tdsTransistor, LOW);
}

void setEtapeZeroVolumeResistance() {
  sensor::temp_etapeZeroVolumeResistance = readResistance(pin::etapeSensor, 560);
  Serial.print(F("Etape zero volume resistance = ")); Serial.print(sensor::etapeZeroVolumeResistance, 2); Serial.println(F(" Ohms"));
}

void setEtapeMaxVolumeResistance() {
  sensor::etapeZeroVolumeResistance = sensor::temp_etapeZeroVolumeResistance;
  sensor::temp_etapeZeroVolumeResistance = 0;
  sensor::etapeMaxVolumeResistance = readResistance(pin::etapeSensor, 560);
  Serial.print(F("Etape max reading resistance = ")); Serial.print(sensor::etapeMaxVolumeResistance, 2); Serial.println(F(" Ohms"));
}

float readResistance(const int& a_pin, const int& a_seriesResistance) {
  // Get ADC value.
  float resistance = analogRead(a_pin);
  // Convert ADC reading to resistance.
  resistance = (1023.0 / resistance) - 1.0;
  resistance = a_seriesResistance / resistance;
  return resistance;
}

float resistanceToCM(const float & a_resistance, const float & a_zeroResistance, const float & a_calResistance, const float & a_calCM) {
  if (a_resistance > a_zeroResistance || (a_zeroResistance - a_calResistance) == 0.0) {
    // Stop if the value is above the zero threshold, or no max resistance is set (would be divide by zero).
    return 0.0;
  }
  // Compute scale factor by mapping resistance to 0...1.0+ range relative to maxResistance value.
  float scale = (a_zeroResistance - a_resistance) / (a_zeroResistance - a_calResistance);
  // Scale maxVolume based on computed scale factor.
  return a_calCM * scale;
}
