void serialRead() {
  static char switchCase = 0;
  static char chars[22] {0};
  static int cnt = 0;
  static float value = 0;
  static char date[22] {0};
  static bool saveValues = false;
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '?') {
      showSerialCommands();
    }
    else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ) {
      switchCase = c;
    }
    else if ((c >= '0' && c <= '9') || c == '-' || c == ':' || c == '.' || c == '/') {
      if (cnt < 22)
        chars[cnt++] = c;
    }
    else if (c == '\n') {
      if (cnt != 0) {
        value = atof(chars);
      }
      else if (switchCase == 'b') {
        for (byte i = 0; i < 22; i++)
          date[i] = chars[i];
      }
      if (switchCase != 0)
        saveValues = true;
      cnt = 0;
      for (char& val : chars) val = 0;
      if (!using_pc_interface_tool) {
        Serial.print(F("switchCase = ")); Serial.println(switchCase);
        Serial.print(F("value = ")); Serial.println(value);
      }
    }
  }
  if (saveValues) {
    // Set the alarm mode
    if (switchCase == 'A') {
      if (!using_pc_interface_tool) {
        Serial.print(F("Alarm mode = ")); Serial.println(value);
      }
      else {
        Serial.print(F("A=")); Serial.println(value,0);
      }
      sendToSlave('A', value);
    }
    // Play touch beep
    else if (switchCase == 'B') {
      if (!using_pc_interface_tool)
        Serial.println(F("Beep"));
      else {
        Serial.print(F("B=")); Serial.println(1);
      }
      sendToSlave('T', 0);
    }
    // Adjust the mcp4151 which in turn adjusts the PH calibration
    else if (switchCase == 'C') {
      if (!using_pc_interface_tool) {
        Serial.print(F("PH calibration step = ")); Serial.println(value);
      }
      else {
        Serial.print(F("C=")); Serial.println(value,0);
      }
      sendToSlave('P', value);
    }
    // Start the RA8875 display demo
    else if (switchCase == 'D') {
      if (!using_pc_interface_tool)
        Serial.println(F("Starting paint demo"));
      else {
        Serial.print(F("D=")); Serial.println(value,0);
      }
      if (value == 1) {
        startDisplay = true;
        displayButtons();
      }
      else {
        tft.fillWindow(RA8875_BLACK);
        startDisplay = false;
      }
    }
    // Run a given doser / pump
    else if (switchCase == 'E') {
      if (value == 1)
        runDoser(1, pin::doserOne, doserOneSpeed);
      else if (value == 2)
        runDoser(2, pin::doserTwo, doserTwoSpeed);
      else if (value == 3)
        runDoser(3, pin::doserThree, doserThreeSpeed);
      else if (value == 4)
        runDoser(4, pin::doserFour, doserFourSpeed);
      else if (value == 5)
        runDoser(5, pin::doserFive, doserFiveSpeed);
      else if (value == 6)
        runDoser(6, pin::doserSix, doserSixSpeed);
      if (using_pc_interface_tool) {
        Serial.print(F("E=")); Serial.println(value,0);
      }
    }
    // Run a given relay for five seconds
    else if (switchCase == 'F') {
      if (value == 1)
        runRelay(1, pin::airHeater);
      else if (value == 2)
        runRelay(2, pin::co2Solenoid);
      else if (value == 3)
        runRelay(3, pin::inletPump);
      else if (value == 4)
        runRelay(4, pin::outletPump);
      else if (value == 5)
        runRelay(5, pin::light);
      else if (value == 6)
        runRelay(6, pin::waterHeater);
      else if (value == 7)
        runRelay(7, pin::unusedRelaySeven);
      else if (value == 8)
        runRelay(8, pin::unusedRelayEight);
      if (using_pc_interface_tool) {
        Serial.print(F("F=")); Serial.println(value,0);
      }
    }
    // Get the RTC time
    else if (switchCase == 'G') {
      t = rtc.getTime();
      if (!using_pc_interface_tool) {
        Serial.print(F("RTC = "));
        Serial.print(rtc.getDateStr(FORMAT_LONG));
        Serial.print(F(" "));
        Serial.print(rtc.getTimeStr());
        Serial.println(".");
      }
      else {
        Serial.print(F("G="));
        Serial.print(rtc.getTimeStr());
        Serial.print("-");
        Serial.print(rtc.getDOWStr(FORMAT_SHORT));
        Serial.print("-");
        Serial.print(t.date);
        Serial.print("/");
        Serial.print(t.mon);
        Serial.print("/");
        Serial.println(t.year);
      }
    }
    // Get the ultra sonic sensors value
    else if (switchCase == 'H') {
      if (!using_pc_interface_tool)
        Serial.print(F("Ultrasonic sensor value = "));
      else
        Serial.print(F("H="));
      Serial.println(getWaterHeight());
    }
    // Get the DHT22 sensors value
    else if (switchCase == 'I') {
      DHT.read22(pin::dht22);
      float airTemp = DHT.temperature;
      float humidity = DHT.humidity;
      if (!using_pc_interface_tool) {
        Serial.print(F("DHT22 humidity = ")); Serial.println(humidity);
        Serial.print(F("DHT22 air temp in c = ")); Serial.println(airTemp);
      }
      else {
        Serial.print(F("I=")); Serial.print(airTemp, 2);
        Serial.print(F(" ")); Serial.println(humidity, 1);
      }
    }
    // Get the ds18b20 sensors value
    else if (switchCase == 'J') {
      dallasTemperature.requestTemperatures();
      float waterTemp = dallasTemperature.getTempCByIndex(0);
      if (!using_pc_interface_tool) {
        Serial.print(F("ds18b20 temp in c = ")); Serial.println(waterTemp);
      }
      else {
        Serial.print(F("J=")); Serial.println(waterTemp, 2);
      }
    }
    // Get the Co2 sensors value
    else if (switchCase == 'K') {
      const unsigned long responceWindow = millis() + 50UL;
      Serial2.write(co2Request, 9);
      byte buffer[8] {0, 0, 0, 0, 0, 0, 0, 0};
      byte bufferPosition = 0;
      while (millis() <= responceWindow && Serial2.available() > 0) {
        char c = Serial2.read();
        if (bufferPosition < 8)
          buffer[bufferPosition++] = c;
      }
      float co2 = 0;
      if (buffer[0] == 255 && buffer[5] == 0 && buffer[7] == 0)
        co2 = (256 * (int)buffer[2]) + (int)buffer[3];
      if (!using_pc_interface_tool) {
        Serial.print(F("MH-Z14A Co2 sensor value = ")); Serial.println(co2);
      }
      else {
        Serial.print(F("K=")); Serial.println(co2,0);
      }
    }
    // Get the PH sensors value
    else if (switchCase == 'L') {
      digitalWrite(pin::phTransistor, HIGH); // turn on sensor
      delay(2000);
      // Get water temperature in celsius
      dallasTemperature.requestTemperatures();
      float waterTemp = dallasTemperature.getTempCByIndex(0);
      //float temperatureCoefficient = 1.0 + 0.02 * (waterTemp - 25.0);
      if (!using_pc_interface_tool) {
        Serial.print(F("ds18b20 temp in c = ")); Serial.println(waterTemp);
      }
      // Get the Ph
      float ph = 0;
      if (using_dfRobot_PhSensor) {
        float volts = analogRead(pin::phSensor) * aref / 1024;
        ph = 3.5 * volts + phOffset; // ) / temperatureCoefficient;
      }
      else {
        float volts = analogRead(pin::phSensor) * aref / 1024.0;
        ph = 7 + ((ph7Voltage - volts) / phVoltagePerUnit); // PH without temperature compensation
      }
      if (!using_pc_interface_tool) {
        Serial.print(F("PH sensor value = ")); Serial.println(ph);
      }
      else {
        Serial.print(F("L=")); Serial.println(ph, 2);
      }
      digitalWrite(pin::phTransistor, LOW); // turn off sensor
    }
    // Get the TDS sensors value
    else if (switchCase == 'M') {
      readTdsQuick();
    }
    // Set the fan 1 speed
    else if (switchCase == 'N') {
      if (!using_pc_interface_tool) {
        Serial.print(F("fan 1 speed = ")); Serial.println(value,0);
      }
      else {
        Serial.print(F("N=")); Serial.println(value,0);
      }
      sendToSlave('Z', value);
    }
    // Set the fan 2 speed
    else if (switchCase == 'O') {
      if (!using_pc_interface_tool) {
        Serial.print(F("fan 2 speed = ")); Serial.println(value,0);
      }
      else {
        Serial.print(F("O=")); Serial.println(value,0);
      }
      sendToSlave('X', value);
    }
    // Send some dummy data to the ESP8266
    else if (switchCase == 'P') {
      if (!using_pc_interface_tool)
        Serial.println(F("Calibrating TDS sensor"));
      calibrateTDS();
    }
    // Send some dummy data to the ESP8266
    else if (switchCase == 'Q') {
      if (!using_pc_interface_tool && value == 1)
        Serial.println(F("Sending dummy wifi data..."));
      else if (!using_pc_interface_tool && value == 2)
        Serial.println(F("Opening wifi portal..."));
      else if (!using_pc_interface_tool && value == 3)
        Serial.println(F("Clearing wifi data and resetting..."));
      else if (!using_pc_interface_tool) {
        Serial.print(F("Q=")); Serial.println(0);
      }
      espSerial(value);
    }
    // Send some dummy data to the ESP8266
    else if (switchCase == 'R') {
      if (!using_pc_interface_tool)
        Serial.println(F("Setting the Etape zero volume resistance"));
      setEtapeZeroVolumeResistance();
    }
    // Send some dummy data to the ESP8266
    else if (switchCase == 'S') {
      if (!using_pc_interface_tool)
        Serial.println(F("Setting the Etape full volume resistance"));
      setEtapeCalibrationResistance();
    }
    // Send some dummy data to the ESP8266
    else if (switchCase == 'T') {
      readEtape();
    }
    // Set doser 1 speed
    else if (switchCase == 'U') {
      doserOneSpeed = value;
      if (!using_pc_interface_tool) {
        Serial.print(F("doser one speed = ")); Serial.println(value);
      }
      else {
        Serial.print(F("U=")); Serial.println(value,0);
      }
    }
    // Set doser 2 speed
    else if (switchCase == 'V') {
      doserTwoSpeed = value;
      if (!using_pc_interface_tool) {
        Serial.print(F("doser two speed = ")); Serial.println(value,0);
      }
      else {
        Serial.print(F("V=")); Serial.println(value);
      }
    }
    // Set doser 3 speed
    else if (switchCase == 'W') {
      doserThreeSpeed = value;
      if (!using_pc_interface_tool) {
        Serial.print(F("doser three speed = ")); Serial.println(value,0);
      }
      else {
        Serial.print(F("W=")); Serial.println(value,0);
      }
    }
    // Set doser 4 speed
    else if (switchCase == 'X') {
      doserFourSpeed = value;
      if (!using_pc_interface_tool) {
        Serial.print(F("doser four speed = ")); Serial.println(value,0);
      }
      else {
        Serial.print(F("X=")); Serial.println(value,0);
      }
    }
    // Set doser 5 speed
    else if (switchCase == 'Y') {
      doserFiveSpeed = value;
      if (!using_pc_interface_tool) {
        Serial.print(F("doser five speed = ")); Serial.println(value,0);
      }
      else {
        Serial.print(F("Y=")); Serial.println(value,0);
      }
    }
    // Set doser 6 speed
    else if (switchCase == 'Z') {
      doserSixSpeed = value;
      if (!using_pc_interface_tool) {
        Serial.print(F("doser six speed = ")); Serial.println(value,0);
      }
      else {
        Serial.print(F("Z=")); Serial.println(value,0);
      }
    }
    // Turn the ESP8266 on or off
    else if (switchCase == 'a') {
      if (value == 0) {
        if (!using_pc_interface_tool)
          Serial.println(F("Turning off ESP8266"));
        digitalWrite(pin::espTransistor, LOW);
        espIsTurnedOn = false;
      }
      else {
        if (!using_pc_interface_tool)
          Serial.println(F("Turning on ESP8266"));
        digitalWrite(pin::espTransistor, HIGH);
        espIsTurnedOn = true;
      }
      if (using_pc_interface_tool) {
        Serial.print(F("a=")); Serial.println(value,0);
      }
    }
    // Set the RTC dateTime
    else if (switchCase == 'b') {
      setDate(date);
    }
    // Get a given soil moisture sensor's value (sensor 1 > 6)
    else if (switchCase == 'c') {
      if (value == 1) {
        if (!using_pc_interface_tool) {
          Serial.print(F("Moisture sensor 1 = ")); Serial.println(analogRead(pin::moistureSensorOne));
        }
        else {
          Serial.print(F("c1=")); Serial.println(analogRead(pin::moistureSensorOne));
        }
      }
      else if (value == 2) {
        if (!using_pc_interface_tool) {
          Serial.print(F("Moisture sensor 2 = ")); Serial.println(analogRead(pin::moistureSensorTwo));
        }
        else {
          Serial.print(F("c2=")); Serial.println(analogRead(pin::moistureSensorTwo));
        }
      }
      else if (value == 3) {
        if (!using_pc_interface_tool) {
          Serial.print(F("Moisture sensor 3 = ")); Serial.println(analogRead(pin::moistureSensorThree));
        }
        else {
          Serial.print(F("c3=")); Serial.println(analogRead(pin::moistureSensorThree));
        }
      }
      else if (value == 4) {
        if (!using_pc_interface_tool) {
          Serial.print(F("Moisture sensor 4 = ")); Serial.println(analogRead(pin::moistureSensorFour));
        }
        else {
          Serial.print(F("c4=")); Serial.println(analogRead(pin::moistureSensorFour));
        }
      }
      else if (value == 5) {
        if (!using_pc_interface_tool) {
          Serial.print(F("Moisture sensor 5 = ")); Serial.println(analogRead(pin::moistureSensorFive));
        }
        else {
          Serial.print(F("c5=")); Serial.println(analogRead(pin::moistureSensorFive));
        }
      }
      else if (value == 6) {
        if (!using_pc_interface_tool) {
          Serial.print(F("Moisture sensor 6 = ")); Serial.println(analogRead(pin::moistureSensorSix));
        }
        else {
          Serial.print(F("c6=")); Serial.println(analogRead(pin::moistureSensorSix));
        }
      }
    }
    // Get the LDR sensor value
    else if (switchCase == 'd') {
      if (!using_pc_interface_tool) {
        Serial.print(F("LDR ssensor = ")); Serial.println(analogRead(pin::ldrSensor));
      }
      else {
        Serial.print(F("d=")); Serial.println(analogRead(pin::ldrSensor));
      }
    }
    // Start recording the water flow rate sensor for 10 seconds
    else if (switchCase == 'e') {
      if (!using_pc_interface_tool)
        Serial.println(F("Reading water flow rate for 10 seconds, please wait until the process is complete before entering any further commands"));
      unsigned long flowRateEndTime = millis() + 10000UL;
      unsigned long flowCounter = 0;
      float currentWaterAmount = 0;
      bool pinState = false;
      while (millis() < flowRateEndTime) {
        if (digitalRead(3) == HIGH && !pinState) {
          flowCounter++;
          pinState = true;
          //Serial.println(flowCounter);
        }
        else if (digitalRead(3) == LOW && pinState) {
          pinState = false;
        }
      }
      currentWaterAmount = flowCounter / 7.5;
      if (!using_pc_interface_tool) {
        Serial.print(F("Total flow rate over 10 seconds = ")); Serial.print(currentWaterAmount,2); Serial.println(F(" Ltrs"));
      }
      else {
        Serial.print(F("e=")); Serial.println(currentWaterAmount,2);
      }
    }
    else if (switchCase == 'f') {
      ph4Voltage = 0;
      readPhVoltage(ph4Voltage);
      if (!using_pc_interface_tool) {
        Serial.print(F("PH4 Voltage = ")); Serial.print(ph4Voltage, 4); Serial.println(F(" volts"));
      }
      else {
        Serial.print(F("f=")); Serial.println(ph4Voltage, 4);
      }
    }
    else if (switchCase == 'g') {
      ph7Voltage = 0;
      readPhVoltage(ph7Voltage);
      if (!using_pc_interface_tool) {
        Serial.print(F("PH7 Voltage = ")); Serial.print(ph7Voltage, 4); Serial.println(F(" volts"));
      }
      else {
        Serial.print(F("g=")); Serial.println(ph7Voltage, 4);
      }
    }
    else if (switchCase == 'h') {
      ph4BufferValue = value;
      if (!using_pc_interface_tool) {
        Serial.print(F("ph 4 buffer value = ")); Serial.println(ph4BufferValue, 2);
      }
      else {
        Serial.print(F("h=")); Serial.println(ph4BufferValue, 2);
      }
    }
    else if (switchCase == 'i') {
      ph7BufferValue = value;
      if (!using_pc_interface_tool) {
        Serial.print(F("ph 7 buffer value = ")); Serial.println(ph7BufferValue, 2);
      }
      else {
        Serial.print(F("i=")); Serial.println(ph7BufferValue, 2);
      }
    }
    else if (switchCase == 'j') {
      if (ph7Voltage != 0 && ph4Voltage != 0)
        phTwoPartCalibration();
      else if (!using_pc_interface_tool)
        Serial.println(F("Error: PH 4 or PH 7 Mv has not been set yet"));
    }
    // Reset values
    saveValues = false;
    switchCase = 0;
    value = 0;
  }
}

void showSerialCommands() {
  Serial.println(F(" ___________________________________________________________________________________________________________________ "));
  Serial.println(F("|                                          Available Serial switch case commands                                    |"));
  Serial.println(F("|___________________________________________________________________________________________________________________|"));
  Serial.println(F("|  Switch (case sensitive) |         Example        |  SwitchCase Description                                       |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|            ?             |           ?            |  Show Serial switch case commands                             |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|         A0 > A2          |          A1            |  Set alarm mode: (no alarm, minor alarm, major alarm)         |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|            B             |           B            |  Play a touch tone                                            |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|        C0 > C255         |          C20           |  Set PH calibration step (0>255)                              |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|         D0 > D1          |          D1            |  Start or stop ra8875 display paint demo                      |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|         E1 > E6          |          E2            |  Start or stop a given doser (doser 1 > 6)                    |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|         F1 > F8          |          F6            |  Start or stop a given relay (relay 1 > 8)                    |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|            G             |           G            |  Get the RTC data and time                                    |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|            H             |           H            |  Get the ultrasonic sensor value                              |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|            I             |           I            |  Get the DHT22 temperature and humidity                       |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|            J             |           J            |  Get the DS18B20 temperature in Celsius                       |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|            K             |           K            |  Get the MH-Z14A Co2 sensor value                             |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|            L             |           L            |  Get the PH sensor value                                      |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|            M             |           M            |  Get the TDS/EC sensor values                                 |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|         N0 > N100        |          N50           |  Set fan 1 speed (0 > 100%)                                   |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|         O0 > O100        |          075           |  Set fan 2 speed (0 > 100%)                                   |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|            P             |           P            |  Calibrate the TDS sensor using 1414Us / 707ppm solution @25c |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|            Q             |           Q            |  Send dummy wifi data to the ESP8266                          |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|            R             |           R            |  Set the Etape zero volume resistance                         |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|            S             |           S            |  Set the Etape full volume resistance                         |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|            T             |           T            |  Get the Etape water height                                   |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|         U0 > U100        |          U50           |  Set doser one speed (0 > 100%)                               |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|         V0 > V100        |          V50           |  Set doser two speed (0 > 100%)                               |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|         W0 > W100        |          W50           |  Set doser three speed (0 > 100%)                             |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|         X0 > X100        |          X50           |  Set doser four speed (0 > 100%)                              |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|         Y0 > Y100        |          Y50           |  Set doser five speed (0 > 100%)                              |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|         Z0 > Z100        |          Z50           |  Set doser six speed (0 > 100%)                               |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|         a0 > a1          |          a1            |  Turn the ESP8266 on or off                                   |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("| bHH:MM:SS-DOW-DD/MM/YYYY | b23:59:59-2-31/12/2019 |  Set RTC dateTime (DOW = 1 > 7) Where 1 > 7 is Mon to Sun     |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|         c1 > c6          |          c1            |  Get a given soil moisture sensor's value (sensor 1 > 6)      |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|            d             |           d            |  Get the LDR sensor value                                     |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|            e             |           e            |  Get the total water flow rate in Ltrs over 10 seconds        |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|            f             |           f            |  Get the Mv reading for the PH 4.01 calibration solution      |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|            g             |           g            |  Get the Mv reading for the PH 7.00 calibration solution      |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|            h             |         h4.01          |  Set the PH 4 buffer solution value                           |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|            i             |         i7.00          |  Set the PH 7 buffer solution value                           |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|"));
  Serial.println(F("|            j             |           j            |  Perform the 2-part PH calibration                            |"));
  Serial.println(F("|__________________________|________________________|_______________________________________________________________|\n"));
}
