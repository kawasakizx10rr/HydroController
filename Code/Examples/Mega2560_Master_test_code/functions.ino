void drawGfx() {
  if (tft.touched() && startDisplay) {
    tft.touchReadPixel(&tx, &ty);
    if (tx >= 10 && tx <= 50 && ty >= 10 && ty <= 55) {
      choosenColor = RA8875_WHITE;
      displayButtons();
      tft.fillRect(10, 10, 40, 40, RA8875_BLACK);
    }
    else if (tx >= 60 && tx <= 100 && ty >= 10 && ty <= 55) {
      choosenColor = RA8875_BLUE;
      displayButtons();
      tft.fillRect(60, 10, 40, 40, RA8875_BLACK);
    }
    else if (tx > 110 && tx < 150 && ty >= 10 && ty <= 55) {
      choosenColor = RA8875_RED;
      displayButtons();
      tft.fillRect(110, 10, 40, 40, RA8875_BLACK);
    }
    else if (tx > 160 && tx < 200 && ty >= 10 && ty <= 55) {
      choosenColor = RA8875_GREEN;
      displayButtons();
      tft.fillRect(160, 10, 40, 40, RA8875_BLACK);
    }
    else if (tx > 210 && tx < 250 && ty >= 10 && ty <= 55) {
      choosenColor = RA8875_CYAN;
      displayButtons();
      tft.fillRect(210, 10, 40, 40, RA8875_BLACK);
    }
    else if (tx > 260 && tx < 300 && ty >= 10 && ty <= 55) {
      choosenColor = RA8875_MAGENTA;
      displayButtons();
      tft.fillRect(260, 10, 40, 40, RA8875_BLACK);
    }
    else if (tx > 310 && tx < 350 && ty >= 10 && ty <= 55) {
      choosenColor = RA8875_YELLOW;
      displayButtons();
      tft.fillRect(310, 10, 40, 40, RA8875_BLACK);
    }
    tft.drawPixel(tx, ty, choosenColor);
  }
}

void displayButtons() {
  tft.fillRect(10, 10, 40, 40, RA8875_WHITE);
  tft.fillRect(60, 10, 40, 40, RA8875_BLUE);
  tft.fillRect(110, 10, 40, 40, RA8875_RED);
  tft.fillRect(160, 10, 40, 40, RA8875_GREEN);
  tft.fillRect(210, 10, 40, 40, RA8875_CYAN);
  tft.fillRect(260, 10, 40, 40, RA8875_MAGENTA);
  tft.fillRect(310, 10, 40, 40, RA8875_YELLOW);
}

void runDoser(const byte& a_doser, const byte& a_pin, const byte& a_speed) {
  static bool pumpOn[6] {false, false, false, false, false, false};
  if (!pumpOn[a_doser - 1] && a_speed > 0) {
    analogWrite(a_pin, a_speed * 2.55);
    pumpOn[a_doser - 1] = true;
    if (!using_pc_interface_tool) {
      Serial.print(F("Running pump ")); Serial.println(a_doser);
    }
  }
  else {
    analogWrite(a_pin, LOW);
    pumpOn[a_doser - 1] = false;
    if (!using_pc_interface_tool) {
      Serial.print(F("Turning off pump ")); Serial.println(a_doser);
    }
  }
}

void runRelay(const byte& a_relay, const byte& a_pin) {
  static bool relayOn[8] {false, false, false, false, false, false, false, false};
  if (!relayOn[a_relay - 1]) {
    digitalWrite(a_pin, !relayOffState);
    relayOn[a_relay - 1] = true;
    if (!using_pc_interface_tool) {
      Serial.print(F("Running relay ")); Serial.println(a_relay);
    }
  }
  else {
    digitalWrite(a_pin, relayOffState);
    relayOn[a_relay - 1] = false;
    if (!using_pc_interface_tool) {
      Serial.print(F("Turning off relay ")); Serial.println(a_relay);
    }
  }
}

void sendToSlave(const char& a_switchcase, const byte & a_value) {
  Wire.beginTransmission(slaveAddress);
  Wire.write(a_switchcase);
  Wire.write(a_value);
  Wire.endTransmission();
}

int getWaterHeight() {
  digitalWrite(pin::hcsrTrig, LOW);
  delayMicroseconds(5); //5
  digitalWrite(pin::hcsrTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin::hcsrTrig, LOW);
  float hcsrDuration = pulseIn(pin::hcsrEcho, HIGH);
  int waterLevel = (hcsrDuration / 2) / 29.1; // in cm
  return waterLevel;
}

void readTdsQuick() {
  if (!using_pc_interface_tool)
    Serial.println(F("Reading TDS values"));
  // Get water temperature in celsius
  dallasTemperature.requestTemperatures();
  float waterTemp = dallasTemperature.getTempCByIndex(0);
  if (!using_pc_interface_tool && waterTemp != -127) {
    Serial.print(F("ds18b20 temp in c = ")); Serial.println(waterTemp);
  }
  else if (!using_pc_interface_tool && waterTemp == -127) {
    Serial.println(F("ds18b20 not detected!! using static temp of 25c."));
    waterTemp = 25;
  }
  // Turn on the TDS sensor
  digitalWrite(pin::tdsTransistor, HIGH);
  // allow sensor to power up
  delay(2000);
  // Read the TDS / EC
  float voltage = analogRead(pin::tdsSensor) * aref / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
  float temperatureCoefficient = 1.0 + 0.02 * (waterTemp - 25.0); // temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
  float uS = (133.42 * pow(voltage, 3) - 255.86 * pow(voltage, 2) + 857.39 * voltage) * ecCalibration;
  float uS25 = uS / temperatureCoefficient; // temperature and calibration compensation
  float tds = uS25 * 0.5; // convert ec value to tds value
  float ec = 0;
  if (!using_pc_interface_tool) {
    if (ecPpmConversionType == US)
      ec = (tds / 0.50) / 1000;
    else if (ecPpmConversionType == AU)
      ec = (tds / 0.70) / 1000;
    else
      ec = (tds / 0.64) / 1000;
  }
  // Print the results
  if (!using_pc_interface_tool) {
    Serial.print(F("EC = ")); Serial.println(ec, 2);
    Serial.print(F("PPM = ")); Serial.println(tds, 0);
    Serial.print(F("Us = ")); Serial.println(uS25, 2);
  }
  else {
    Serial.print(F("M=")); Serial.print(ec, 2);
    Serial.print(F(" ")); Serial.println(tds, 0);
  }
  // Turn off the TDS sensor
  digitalWrite(pin::tdsTransistor, LOW);
}

void espSerial(const byte& a_mode) {
  if (espIsTurnedOn) {
    if (!using_pc_interface_tool)
      Serial.println(F("The Wifi"));
    // TO DO
  }
}

bool isValidNumber(const char* a_array, const byte& a_size) {
  for (byte i = 0; i < a_size; i++) {
    if (a_array[i] == '\0')
      break;
    if (!(a_array[i] >= '0' && a_array[i] <= '9'))
      return false;
  }
  return true;
}

void setDate(const char* a_array) {
  if (isValidDate(a_array)) {
    char buf[6];
    // hour
    buf[0] = a_array[0];
    buf[1] =  a_array[1];
    byte hour = atoi(buf);
    for (char& val : buf) val = 0;
    if (!using_pc_interface_tool) {
      Serial.print(F("hour = ")); Serial.println(hour);
    }
    // minute
    buf[0] = a_array[3];
    buf[1] = a_array[4];
    byte minute = atoi(buf);
    for (char& val : buf) val = 0;
    if (!using_pc_interface_tool) {
      Serial.print(F("minute = ")); Serial.println(minute);
    }
    // second
    buf[0] = a_array[6];
    buf[1] = a_array[7];
    byte second = atoi(buf);
    for (char& val : buf) val = 0;
    if (!using_pc_interface_tool) {
      Serial.print(F("seconds = ")); Serial.println(second);
    }
    //Day of week
    byte dayOfWeek = a_array[9] - '0';
    if (!using_pc_interface_tool) {
      Serial.print(F("dayOfWeek = ")); Serial.println(dayOfWeek);
    }
    // day
    buf[0] = a_array[11];
    buf[1] = a_array[12];
    byte day = atoi(buf);
    for (char& val : buf) val = 0;
    if (!using_pc_interface_tool) {
      Serial.print(F("day = ")); Serial.println(day);
    }
    // month
    buf[0] = a_array[14];
    buf[1] = a_array[15];
    byte month = atoi(buf);
    for (char& val : buf) val = 0;
    if (!using_pc_interface_tool) {
      Serial.print(F("month = ")); Serial.println(month);
    }
    // year
    buf[0] = a_array[17];
    buf[1] = a_array[18];
    buf[2] = a_array[19];
    buf[3] = a_array[20];
    int year = atoi(buf);
    for (char& val : buf) val = 0;
    if (!using_pc_interface_tool) {
      Serial.print(F("year = ")); Serial.println(year);
    }
    if (using_pc_interface_tool) {
      Serial.print(F("b=1")); Serial.println(a_array);
    }
    adjustRTCtime(dayOfWeek, hour, minute, second, day, month, year);
  }
  else if (!using_pc_interface_tool) {
    Serial.print(F("Error reading time string: ")); Serial.println(a_array);
  }
  else {
    Serial.print(F("b=")); Serial.println(0);
  }
}

bool isValidDate(const char* a_array) {
  // check hours is a valid 24 hour format
  if (!(a_array[0] >= '0' && a_array[0] <= '2') && !(a_array[1] >= '0' && a_array[1] <= '9'))
    return false;
  // check for hour delimiter
  if (a_array[2] != ':')
    return false;
  // check minutes is a valid 00 > 60 format
  if (!(a_array[3] >= '0' && a_array[3] <= '6') && !(a_array[4] >= '0' && a_array[4] <= '9'))
    return false;
  // check for minute delimiter
  if (a_array[5] != ':')
    return false;
  // check seconds is a valid 00 > 60 format
  if (!(a_array[6] >= '0' && a_array[6] <= '6') && !(a_array[7] >= '0' && a_array[7] <= '9'))
    return false;
  // check for day of the week delimiter
  if (a_array[8] != '-')
    return false;
  // check day of the week is between 1 to 7
  if (!(a_array[9] >= '1' && a_array[9] <= '7'))
    return false;
  // check for day of the week delimiter
  if (a_array[10] != '-')
    return false;
  // check day of month is a valid 01 > 31 format
  if (!(a_array[11] >= '0' && a_array[11] <= '3') && !(a_array[12] >= '0' && a_array[12] <= '9'))
    return false;
  // check for month delimiter
  if (a_array[13] != '/')
    return false;
  // check month is a valid 01 > 12 format
  if (!(a_array[14] >= '0' && a_array[14] <= '1') && !(a_array[15] >= '0' && a_array[15] <= '9'))
    return false;
  // check for month delimiter
  if (a_array[16] != '/')
    return false;
  // check year is a valid format
  if (!(a_array[17] >= '2' && a_array[17] <= '9') && !(a_array[18] >= '0' && a_array[18] <= '9') && !(a_array[19] >= '0' && a_array[19] <= '9') && !(a_array[20] >= '0' && a_array[20] <= '9'))
    return false;

  return true;
}

void adjustRTCtime(const byte & a_dayOfWeek, const byte & a_hour, const byte & a_minute, const byte & a_second,
                   const byte & a_day, const byte & a_month, const unsigned int& a_year) {
  if (!using_pc_interface_tool)
    Serial.println(F("setting RTC time"));
  rtc.setDOW(a_dayOfWeek); // Set Day-of-Week: monday = 1, sunday = 7
  rtc.setTime(a_hour, a_minute, a_second);
  rtc.setDate(a_day, a_month, a_year);
  delay(200);
}

void readEtape() {
  if (etapeZeroVolumeResistance != 0 && etapeCalibrationResistance != 0) {
    float resistance = readResistance(pin::eTapeSensor, 560);
    float cm = resistanceToCM(resistance, etapeZeroVolumeResistance, etapeCalibrationResistance, etapeCalibrationCm);
    cm += etapeOffset;
    float inch = (cm / 2.5);
    if (!using_pc_interface_tool) {
      Serial.print(F("Etape resistance = ")); Serial.print(resistance); Serial.println(F(" Ohms"));
    }
    if (cm <= etapeOffset && !using_pc_interface_tool) {
      Serial.print(F("Current Etape reading is below the minimum range!"));
    }
    else {
      if (!using_pc_interface_tool) {
        Serial.print(F("Etape reading = ")); Serial.print(cm); Serial.println(" Centimeters");
        Serial.print(F("Etape reading = ")); Serial.print(inch); Serial.println(" Inchs\n");
      }
      else {
        Serial.print(F("T=")); Serial.println(cm);
      }
    }
  }
  else if (!using_pc_interface_tool) {
    Serial.print(F("Error, please calibrate the Etape first"));
  }
}

void setEtapeZeroVolumeResistance() {
  etapeZeroVolumeResistance = readResistance(pin::eTapeSensor, 560);
  if (!using_pc_interface_tool) {
    Serial.print(F("Etape zero volume resistance = ")); Serial.print(etapeZeroVolumeResistance, 2); Serial.println(F(" Ohms"));
  }
  else {
    Serial.print(F("R=")); Serial.println(etapeZeroVolumeResistance, 2);
  }
}

void setEtapeCalibrationResistance() {
  etapeCalibrationResistance = readResistance(pin::eTapeSensor, 560);
  if (!using_pc_interface_tool) {
    Serial.print(F("Etape calibration resistance = ")); Serial.println(etapeCalibrationResistance, 2); Serial.println(F(" Ohms"));
  }
  else {
    Serial.print(F("S=")); Serial.println(etapeCalibrationResistance, 2);
  }
}

float readResistance(const int& pin, const int& seriesResistance) {
  // Get ADC value.
  float resistance = analogRead(pin);
  // Convert ADC reading to resistance.
  resistance = (1023.0 / resistance) - 1.0;
  resistance = seriesResistance / resistance;
  return resistance;
}

float resistanceToCM(const float & resistance, const float & zeroResistance, const float & calResistance, const float & calCM) {
  if (resistance > zeroResistance || (zeroResistance - calResistance) == 0.0) {
    // Stop if the value is above the zero threshold, or no max resistance is set (would be divide by zero).
    return 0.0;
  }
  // Compute scale factor by mapping resistance to 0...1.0+ range relative to maxResistance value.
  float scale = (zeroResistance - resistance) / (zeroResistance - calResistance);
  // Scale maxVolume based on computed scale factor.
  return calCM * scale;
}

void calibrateTDS() {
  if (!using_pc_interface_tool)
    Serial.println(F("Starting TDS sensor calibration"));
  // Turn the PH sensor off
  digitalWrite(pin::phTransistor, LOW);
  // Turn on the TDS sensor
  digitalWrite(pin::tdsTransistor, HIGH);
  // allow TDS sensor to power up
  delay(500);
  // Get the water temperature in celsius
  dallasTemperature.requestTemperatures();
  float waterTemp = dallasTemperature.getTempCByIndex(0);
  if (!using_pc_interface_tool) {
    Serial.print(F("ds18b20 temp in c = ")); Serial.println(waterTemp);
  }
  // Print the current calibration solution value
  if (!using_pc_interface_tool) {
    Serial.print(F("Raw EC calibration solution  = ")); Serial.print(ecCalibrationSolution); Serial.println(F("ppm"));
  }
  // Calibration solution temperature compensation
  float temperatureCoefficient = 1.0 + 0.02 * (waterTemp - 25.0);
  float calSolution = ecCalibrationSolution / temperatureCoefficient;
  // Print the current calibration solution value
  if (!using_pc_interface_tool) {
    Serial.print(F("Raw EC calibration solution with temperature correction = ")); Serial.print(calSolution); Serial.println(F("Us"));
  }
  // Get the raw analog reading voltage
  float voltage = analogRead(pin::tdsSensor) * 5.0 / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
  float ppm = ((133.42 * pow(voltage, 3) - 255.86 * pow(voltage, 2) + 857.39 * voltage) * 0.5) / temperatureCoefficient; //convert ec value to tds value
  if (!using_pc_interface_tool)
    Serial.print(F("PPM reading without calibration = ")); Serial.println(ppm);
  float KValueTemp = calSolution / ppm;
  // Check if the calibration values are in the expected ranges
  if (calSolution > 0 && calSolution < 2000 && KValueTemp > 0 && KValueTemp < 5) {
    if (!using_pc_interface_tool) {
      Serial.println();
      Serial.println(F("Calibration Successful."));
      Serial.print(F("KValueTemp = ")); Serial.println(KValueTemp);
    }
    else
      Serial.println(F("P=1"));
    ecCalibration = KValueTemp;
  }
  // Else if the calibration values are NOT in the expected ranges, print the reason why
  else if (!using_pc_interface_tool) {
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
  else {
    Serial.println(F("P=0"));
  }
  // Turn off the TDS sensor
  digitalWrite(pin::tdsTransistor, LOW);
}

void readPhVoltage(float& a_voltage) {
  float phVoltage = 0;
  digitalWrite(pin::tdsTransistor, LOW);
  delay(100);
  digitalWrite(pin::phTransistor, HIGH);
  delay(500);
  for (byte i = 0; i < 4; i++) {
    phVoltage += analogRead(pin::phSensor) * aref / 1024.0;
    delay(250);
  }
  digitalWrite(pin::phTransistor, LOW);
  a_voltage = phVoltage / 4.0;
}

void phTwoPartCalibration() {
  float phStep = abs((ph7Voltage - ph4Voltage) / (ph7BufferValue - ph4BufferValue));
  if (phStep != 0) {
    phVoltagePerUnit = phStep;
    if (!using_pc_interface_tool) {
      Serial.print(F("PH cal success, phVoltagePerUnit = ")); Serial.println(phStep, 4);
    }
    else {
      Serial.print(F("j=")); Serial.println(phStep, 4);
    }
  }
  else if (!using_pc_interface_tool) {
    Serial.println(F("PH cal failed, phVoltagePerUnit = 0"));
  }
  else
    Serial.println(F("j=0"));
}
