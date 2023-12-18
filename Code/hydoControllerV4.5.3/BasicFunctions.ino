// Send a switchcase and command to the 328P via i2c
void sendToSlave(const char& a_switchcase, const byte& a_value) {
  Wire.beginTransmission(device::slaveAddress);
  Wire.write(a_switchcase);
  Wire.write(a_value);
  Wire.endTransmission();
}

// Adjust any (byte,int,float or long) value by x increment within the range of min and max
float adjustValue(float a_val, float a_increment, const float& a_min, const float& a_max) {
  beep();
  if (display::currentTouch - display::lastTouch > 10000UL)
    a_increment *= 10.0;

  if (a_increment < 0) {
    if (a_val >= (a_min + abs(a_increment)))
        a_val += a_increment;
    if (a_val < a_min)
      a_val = a_min;
  }    
  else if (a_increment > 0) {
    if (a_val <= (a_max - a_increment))
      a_val += a_increment;
    if (a_val > a_max)
      a_val = a_max;
  }

  Serial.print(F("Adjust value = ")); Serial.println(a_val);
  return a_val;
}

// Reset the min max values
void clearMaxMins() {
  using namespace device;
  minPh = 0;
  maxPh = 0;
  minEc = 0;
  maxEc = 0;
  minWaterTemp = 0;
  maxWaterTemp = 0;
  minWaterLevel = 0;
  maxWaterLevel = 0;
  minTds = 0;
  maxTds = 0;
  minCo2 = 0;
  maxCo2 = 0;
  minAirTemp = 0;
  maxAirTemp = 0;
  minHumidity = 0;
  maxHumidity = 0;
  minFanOneSpeed = 0;
  maxFanOneSpeed = 0;
  minFanTwoSpeed = 0;
  maxFanTwoSpeed = 0;
}

// Reset the machine learning values
void resetMachineLearning() {
  using namespace machineLearning;
  for (unsigned int& val : phDownDosingInc)
    val = 0;
  for (unsigned int& val : phUpDosingInc)
    val = 0;
  for (unsigned int& val : ecDosingInc)
    val = 0;
  for (unsigned int& val : tdsDosingInc)
    val = 0;
  phDownArrayBlock = 0;
  phUpArrayBlock = 0;
  ecArrayBlock = 0;
  tdsArrayBlock = 0;
  device::adjustPhDown = false;
  savePhDownResult = false;
  savePhUpResult = false;
  saveEcResult = false;
  saveTdsResult = false;
  logPhDownResult = false;
  logPhUpResult = false;
  logEcResult = false;
  logTdsResult = false;
  phDownMls = 0;
  previousPhDownMls = 0;
  phUpMls = 0;
  previousPhUpMls = 0;
  ecMls = 0;
  previousEcMls = 0;
  tdsMls = 0;
  previousTdsMls = 0;
  previousPhDownSensor = 0;
  phDownSensorHistory = 0;
  previousPhUpSensor = 0;
  phUpSensorHistory = 0;
  previousEcSensor = 0;
  ecSensorHistory = 0;
  previousTdsSensor = 0;
  tdsSensorHistory = 0;
  phDownMultipler = 1;
  phUpMultipler = 1;
  ecMultipler = 1;
  tdsMultipler = 1;
  saveUserEEPROM(device::userProfile);
}

// Send a beep coomand to the 328P
void beep() {
  if (!user::disableBeeper)
    sendToSlave('T', 0);
}

// Set the graph page option and refresh the page
void setGraphOption(const byte& a_option) {
  if (display::graphOption != a_option) {
    beep();
    clearPage();
    display::refreshPage = true;
    display::graphOption = a_option;
  }
}

// this function flashes the user defined date time to the RTC
void adjustRTCtime() {
  Serial.println(F("setting RTC time"));
  rtc.setDOW(user::rtcDayOfWeek); // Set Day-of-Week: monday = 1, sunday = 7
  rtc.setTime(user::rtcHour, user::rtcMinute, user::rtcSecond);
  rtc.setDate(user::rtcDay, user::rtcMonth, user::rtcYear);
  delay(200);
}

// prime a doser at a given speed in PWM 0-255
void prime(const byte& a_doser, const byte& a_pin, const byte& a_speed) {
  static byte previousDoser = 0;
  static int currentDosingAmount = 0;
  static unsigned long primeRunTime = 0;
  if (a_speed != 0) {
    if (a_doser != previousDoser) {
      Serial.print(F("Priming doser ")); Serial.println(a_doser);
      analogWrite(a_pin, a_speed * 2.55);
      device::isPriming = a_doser;
      currentDosingAmount = 0;
      primeRunTime = millis();
      previousDoser = a_doser;
    }
    if (millis() - primeRunTime >= 1000UL) {
      primeRunTime = millis();
      Serial.print(F("Primed ")); Serial.print(++currentDosingAmount); Serial.println(F(" mls"));
      beep();
    }
  }
  else { // If not priming turn all off dosers, just to be safe
    Serial.println(F("Stopping dosers"));
    analogWrite(pin::doserOne, 0);
    analogWrite(pin::doserTwo, 0);
    analogWrite(pin::doserThree, 0);
    analogWrite(pin::doserFour, 0);
    analogWrite(pin::doserFive, 0);
    analogWrite(pin::doserSix, 0);
    previousDoser = 0;
    currentDosingAmount = 0;
    primeRunTime = 0;
  }
}

// Save a log message to the array and push back the array when full
void saveLogMessage(const byte& a_logType) {
  rtcTime = rtc.getTime();
  const char* t_mon = rtc.getMonthStr(FORMAT_SHORT);
  const char* t_time = rtc.getTimeStr(FORMAT_SHORT);
  char t_date[8];
  itoa(rtcTime.date, t_date, 10);
  // Push back log array if full
  if (pushBackLogs) {
    for (byte i = 0; i < message::maxLogs - 1; i++) {
      // push back log type
      message::logTypeArray[i] = message::logTypeArray[i + 1];
      // copy log i + 1 to log i
      for (byte n = 0; n < message::maxCharsPerLog; n++) {
        message::timeStrArray[i][n] = message::timeStrArray[i + 1][n];
      }
    }
  }
  // Add the log message type
  message::logTypeArray[message::systemLogPos] = a_logType;
  // Clear the last block
  memset(message::timeStrArray[message::systemLogPos], 0, message::maxCharsPerLog);
  // Add the day
  if (rtc.day() < 10)
    strcpy(message::timeStrArray[message::systemLogPos], "0");
  strcpy(message::timeStrArray[message::systemLogPos], t_day);
  strcat(message::timeStrArray[message::systemLogPos], " ");
  /// add month
  strcat(message::timeStrArray[message::systemLogPos], t_mon);
  strcat(message::timeStrArray[message::systemLogPos], " ");
  // Add the time
  strcat(message::timeStrArray[message::systemLogPos], t_time);
  Serial.print(F("Added new log type [")); Serial.print(a_logType); Serial.print(F("] "));
  const char* logPretext = PROGMEM_read(&message::notificationsArray[a_logType]);
  Serial.print((const __FlashStringHelper *)logPretext);
  Serial.print(F(" ")); Serial.println(message::timeStrArray[message::systemLogPos]);
  if (message::systemLogPos < message::maxLogs - 1) {
    message::systemLogPos++;
    pushBackLogs = false;
  }
  else {
    pushBackLogs = true;
  }
}

// Convert a float to int, with a precison of 2 decimal places
int fltToInt(const float& a_value) {
  int val = a_value * 100;
  return val;
}

// Convert a float to int, with a precison of 3 decimal places
int doubleToInt(const float& a_value) {
  int val = a_value * 1000;
  return val;
}

// convert a celsius value to ferinheight
float convertToF(const float& a_value) {
  return (a_value * 1.8) + 32;
}

// convert a centimeter value to inches
float convertToInch(const float& a_value) {
  return a_value / 2.54;
}

// when the graph arrays are full push back the array values by 1
void pushBackDataArrays() {
  static bool graphFirstSave = true;
  static unsigned long graphPreviousMillis = 0;
  if ((millis() - graphPreviousMillis >= user::graphInterval * 60000UL) || graphFirstSave) {
    if (device::graphArrayPos < device::maxGraphArrayValues) {
      updateGraphArrays(device::graphArrayPos);
      device::graphArrayPos++;
    }
    else {
      for (byte i = 0; i < device::maxGraphArrayValues - 1; i++) {
        device::phArray[i] = device::phArray[i + 1];
        device::ecArray[i] = device::ecArray[i + 1];
        device::tdsArray[i] = device::tdsArray[i + 1];
        device::co2Array[i] = device::co2Array[i + 1];
        device::waterTemperatureArray[i] = device::waterTemperatureArray[i + 1];
        device::waterTemperatureArrayF[i] = device::waterTemperatureArrayF[i + 1];
        device::waterLevelArray[i] = device::waterLevelArray[i + 1];
        device::waterLevelArrayInInches[i] = device::waterLevelArrayInInches[i + 1];
        device::airTemperatureArray[i] = device::airTemperatureArray[i + 1];
        device::airTemperatureArrayF[i] = device::airTemperatureArrayF[i + 1];
        device::humidityArray[i] = device::humidityArray[i + 1];
        device::fanOneSpeedArray[i] = device::fanOneSpeedArray[i + 1];
        device::fanTwoSpeedArray[i] = device::fanTwoSpeedArray[i + 1];
      }
      updateGraphArrays(device::maxGraphArrayValues - 1);
    }
    graphFirstSave = false;
    graphPreviousMillis = millis();
  }
}

// Average all the data in a machine learning array and convert to float if required
float averageResults(const unsigned int* a_array, const bool& a_convertToFloat) {
  float averagedResult = 0;
  byte count = 0;
  for (byte i = 0; i < machineLearning::maxSize; i++) {
    if (a_array[i] != 0) {
      averagedResult += a_convertToFloat ? a_array[i] / 1000.0 : a_array[i];
      count++;
    }
  }
  if (count != 0)
    averagedResult /= count;
  return averagedResult;
}

// update the graph values at x position
void updateGraphArrays(const byte& a_arrayPosition) {
  device::phArray[a_arrayPosition] = sensor::ph <= 0 ? 0.001 : sensor::ph;
  device::ecArray[a_arrayPosition] = sensor::ec <= 0 ? 0.001 : sensor::ec;
  device::tdsArray[a_arrayPosition] = sensor::tds <= 0 ? 0.01 : sensor::tds;
  device::co2Array[a_arrayPosition] = sensor::co2 <= 0 ? 0.01 : sensor::co2;
  device::waterTemperatureArray[a_arrayPosition] = sensor::waterTemp <= 0 ? 0.001 : sensor::waterTemp;
  device::waterTemperatureArrayF[a_arrayPosition] = sensor::waterTempF <= 0 ? 0.001 : sensor::waterTempF;
  device::waterLevelArray[a_arrayPosition] = sensor::waterLevel <= 0 ? 0.01 : sensor::waterLevel;
  device::waterLevelArrayInInches[a_arrayPosition] = sensor::waterLevelInches <= 0 ? 0.001 : sensor::waterLevelInches;
  device::airTemperatureArray[a_arrayPosition] = sensor::airTemp <= 0 ? 0.001 : sensor::airTemp;
  device::airTemperatureArrayF[a_arrayPosition] = convertToF(sensor::airTemp) <= 0 ? 0.001 : convertToF(sensor::airTemp);
  device::humidityArray[a_arrayPosition] = sensor::humidity <= 0 ? 0.01 : sensor::humidity;
  device::fanOneSpeedArray[a_arrayPosition] = device::fanOneSpeed <= 0 ? 0.01 : device::fanOneSpeed;
  device::fanTwoSpeedArray[a_arrayPosition] = device::fanTwoSpeed <= 0 ? 0.01 : device::fanTwoSpeed;
}

// Set the min max's, note both min and max could be higher than current max and lower than min at boot, so uses if if
void setMaxMins() {
  // ph max mins
  if (sensor::ph < device::minPh || device::minPh == 0)
    device::minPh = sensor::ph;
  if (sensor::ph > device::maxPh || device::maxPh == 0)
    device::maxPh = sensor::ph;
  // ec max mins
  if (sensor::ec < device::minEc || device::minEc == 0)
    device::minEc = sensor::ec;
  if (sensor::ec > device::maxEc || device::maxEc == 0)
    device::maxEc = sensor::ec;
  // water temp max mins
  if (sensor::waterTemp < device::minWaterTemp || device::minWaterTemp == 0)
    device::minWaterTemp = sensor::waterTemp;
  if (sensor::waterTemp > device::maxWaterTemp || device::maxWaterTemp == 0)
    device::maxWaterTemp = sensor::waterTemp;
  // water level max mins
  if (sensor::waterLevel < device::minWaterLevel || device::minWaterLevel == 0)
    device::minWaterLevel = sensor::waterLevel;
  if (sensor::waterLevel > device::maxWaterLevel || device::maxWaterLevel == 0)
    device::maxWaterLevel = sensor::waterLevel;
  // ppm / tds max mins
  if (sensor::tds < device::minTds || device::minTds == 0)
    device::minTds = sensor::tds;
  if (sensor::tds > device::maxTds || device::maxTds == 0)
    device::maxTds = sensor::tds;
  // co2 max mins
  if (sensor::co2 < device::minCo2 || device::minCo2 == 0)
    device::minCo2 = sensor::co2;
  if (sensor::co2 > device::maxCo2 || device::maxCo2 == 0)
    device::maxCo2 = sensor::co2;
  // air temp max mins
  if (sensor::airTemp < device::minAirTemp || device::minAirTemp == 0)
    device::minAirTemp = sensor::airTemp;
  if (sensor::airTemp > device::maxAirTemp || device::maxAirTemp == 0)
    device::maxAirTemp = sensor::airTemp;
  // humidity max mins
  if (sensor::humidity < device::minHumidity || device::minHumidity == 0)
    device::minHumidity = sensor::humidity;
  if (sensor::humidity > device::maxHumidity || device::maxHumidity  == 0)
    device::maxHumidity = sensor::humidity;
  // fan one max mins
  if (device::fanOneSpeed < device::minFanOneSpeed || device::minFanOneSpeed == 0)
    device::minFanOneSpeed = device::fanOneSpeed;
  if (device::fanOneSpeed > device::maxFanOneSpeed || device::maxFanOneSpeed == 0)
    device::maxFanOneSpeed = device::fanOneSpeed;
  // fan two max mins
  if (device::fanTwoSpeed < device::minFanTwoSpeed || device::minFanTwoSpeed == 0)
    device::minFanTwoSpeed = device::fanTwoSpeed;
  if (device::fanTwoSpeed > device::maxFanTwoSpeed || device::maxFanTwoSpeed  == 0)
    device::maxFanTwoSpeed = device::fanTwoSpeed;
}

// Average any float array
float averageArray(const float* a_array, const byte& a_size) {
  float average = 0;
  for (byte i = 0; i < a_size; i++) {
    average += a_array[i];
  }
  return average / (float)a_size;
}

// Clear all system logs
void clearSystemLogs() {
  message::systemLogPos = 0;
  for (byte i = 0; i < message::maxLogs; i++) {
    message::logTypeArray[i] = 0;
    for (byte n = 0; n < message::maxCharsPerLog; n++) {
      message::timeStrArray[i][n] = 0;
    }
  }
}

void getAverageTouch(const uint16_t a_touchX, const uint16_t a_touchY) {
  static uint16_t averageTouchX, averageYouchY;
  if (display::averageTouchCnt < display::maxAverageTouches) {
    averageTouchX += a_touchX;
    averageYouchY += a_touchY;
    display::averageTouchCnt++;
  }
  else {
    display::touch_x = averageTouchX / display::maxAverageTouches;
    display::touch_y = averageYouchY / display::maxAverageTouches;
    display::averageTouchCnt = 0;
    averageTouchX = 0; averageYouchY = 0;
  }
}
