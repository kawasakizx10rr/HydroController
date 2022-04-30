/*
  All code contained within this software is the property of Edwin James Martin,
  Created on the 28/12/2019, All rights are reserved and none of the software or text
  is to be modified, redistributed, sold or use in other projects, unless prior written
  permission is obtained from the creator of the software being Edwin James Martin.

  I Edwin James Martin take absolutely no responsibility for any damage or harm that
  may incur from the dangers of AC current in regard to the hardware and or software
  used. If you are not a trained electrician please contact a trained electrical engineer to
  install and inspect the AC.

  Version: 0.2.1
  Last modified: 2 May 2020

  Note: This code is designed to run as fast as possible, due to the precise timing
  required to accurately chop up the AC wave form and read i2c commands. The loop
  time must be less than 100uS at any given time, currently 18uS is the max loop
  time with all functions running, as a result port manipulation is used throughout.

  --------------------------------------------------------------------------
  SwitchCases:
  Switch     Value range     Modes                                  Example
  A          0 > 2           NO_ALARM, MINOR_ALARM, MAJOR_ALARM     A1
  B          0 > 1           BUZZER_ENABLED, BUZZER_DISABLED        B0
  L          0 > 1           LED_ENABLED, LED_DISABLED              L0
  P          0 > 255         Digital Pot steps                      P128
  T          0               TOUCH_TONE                             T0
  Z          0 > 100         FAN_ONE_SPEED_IN_PERCENT               Z50
  X          0 > 100         FAN_TWO_SPEED_IN_PERCENT               X75
  --------------------------------------------------------------------------
  PINS:
  AC interruptor  = PORTD 2
  buzzer          = PORTD 3
  fanOne          = PORTD 4
  fanTwo          = PORTD 5
  ledRed          = PORTD 7
  ledGreen        = PORTB 0
  ledBlue         = PORTB 1
  mcp4151_CS      = PORTB 2
*/

#include <SPI.h>
#include <Wire.h>

const byte deviceAddress = 9;
const byte halfWave = 100; // 100 FOR 50Hz VAC 83 FOR 60Hz VAC (!!! MAKE SURE TO ADJUST FOR YOUR POWER FREQUENCY !!!)
volatile byte alarmOption = 0;
volatile unsigned long fanOneSpeed = 0, fanTwoSpeed = 0, toneDuration = 0;
volatile unsigned long fanOneCutOffPoint = 0, fanTwoCutOffPoint = 0;
volatile bool buzzerDisabled = false, ledDisabled = false;
volatile bool finishFanOneCutOff = false, finishFanTwoCutOff = false;
enum modes {
  NO_ALARM, MINOR_ALARM, MAJOR_ALARM
};

void setup() {
  Serial.begin(115200); // Just for debugging
  DDRD = B11111000; // Sets pins 3 to 7 as outputs
  DDRB = B00000111; // Sets pins 8 to 10 as outputs
  PORTD |= _BV(PD7);
  delay(500);
  PORTD &= ~_BV(PD7);
  PORTB |= _BV(PB0);
  delay(500);
  PORTB &= ~_BV(PB0);
  PORTB |= _BV(PB1);
  delay(500);
  PORTB &= ~_BV(PB1);
  PORTD |= _BV(PD3);
  delay(50);
  PORTD &= ~_BV(PD3);
  SPI.begin();
  Wire.begin(deviceAddress); // Start the I2C Bus as Slave on address 9 (0 - 100) are available connections
  Wire.onReceive(i2cReceiveEvent); // Attach a function to trigger when something is received.
  attachInterrupt(digitalPinToInterrupt(2), zeroCrosssEvent, RISING);
}

void loop() {
  //unsigned long loopTime = micros();
  playTone();
  cutOff();
  alarm();
  //Serial.println(micros() - loopTime);
}

void i2cReceiveEvent(const int cnt) {
  static char switchCase = 0;
  static byte value = 0;
  if (cnt == 2) {
    switchCase = Wire.read();
    value = Wire.read();
    Serial.print(F("\nReceived switchcase ")); Serial.print(switchCase);  Serial.print(F(" with the value "));  Serial.println(value);
  }
  if (switchCase == 'A')
    alarmOption = value;
  else if (switchCase == 'B')
    buzzerDisabled = value;
  else if (switchCase == 'L')
    ledDisabled = value;
  else if (switchCase == 'P')
    digitalPotWrite(value);
  else if (switchCase == 'T')
    toneDuration = millis() + 50UL;
  else if (switchCase == 'Z') {
    if (value == 0) {
      PORTD &= ~_BV(PD4); // pin 4 low
      fanOneSpeed = 0;
    }
    else if (value == 100) {
      PORTD |= _BV(PD4); // pin 4 high
      fanOneSpeed = 0;
    }
    else
      fanOneSpeed = (100 - value) * halfWave; // invert 100-0% to 0-100% and map to 0-10000
  }
  else if (switchCase == 'X') {
    if (value == 0) {
      PORTD &= ~_BV(PD5); // pin 5 low
      fanTwoSpeed = 0;
    }
    else if (value == 100) {
      PORTD |= _BV(PD5); // pin 5 high
      fanTwoSpeed = 0;
    }
    else
      fanTwoSpeed = (100 - value) * halfWave; // invert 100-0% to 0-100% and map to 0-10000
  }
}

void digitalPotWrite(const int& a_value) {
  digitalWrite(10, LOW);
  SPI.transfer(0x00);
  SPI.transfer(a_value);
  digitalWrite(10, HIGH);
}

void playTone() {
  if (!buzzerDisabled && toneDuration != 0 && millis() <= toneDuration) {
    PORTD |= _BV(PD3); // pin 3 high
  }
  else if ((toneDuration != 0 && millis() > toneDuration) || buzzerDisabled) {
    PORTD &= ~_BV(PD3); // pin 3 low
    toneDuration = 0;
  }
}

void zeroCrosssEvent() {
  if (fanOneSpeed != 0) {
    fanOneCutOffPoint = micros() + fanOneSpeed;
    PORTD &= ~_BV(PD4); // pin 4 low
    finishFanOneCutOff = true;
  }
  if (fanTwoSpeed != 0) {
    fanTwoCutOffPoint = micros() + fanTwoSpeed;
    PORTD &= ~_BV(PD5); // pin 5 low
    finishFanTwoCutOff = true;
  }
}

void cutOff() {
  if (finishFanOneCutOff && micros() >= fanOneCutOffPoint) {
    PORTD |= _BV(PD4); // pin 4 high
    finishFanOneCutOff = false;
  }
  if (finishFanTwoCutOff && micros() >= fanTwoCutOffPoint) {
    PORTD |= _BV(PD5); // pin 5 high
    finishFanTwoCutOff = false;
  }
}

void alarm() {
  static unsigned long alarmPreviousMillis = 0;
  static byte count = 0;
  if (ledDisabled) { // Turn off the RGB LED
    PORTD &= ~_BV(PD7); // pin 7 low
    PORTB &= ~_BV(PB0); // pin 8 low
    PORTB &= ~_BV(PB1); // pin 9 low
  }
  else if (alarmOption == NO_ALARM) // No alarm set the LED to blue
    PORTB |= _BV(PB1); // pins 9 high
  else if (millis() - alarmPreviousMillis > 1000UL) { // Timed tone and LED blink
    count++;
    if (alarmOption == MINOR_ALARM) {  // yellow 1 sec on 3 secs off sequence.
      if (count == 1) {
        PORTD |= _BV(PD7);
        PORTB |= _BV(PB0);
        toneDuration = millis() + 1000UL;
      }
      else if (count == 2) {
        PORTD &= ~_BV(PD7); // pin 7 low
        PORTB &= ~_BV(PB0); // pin 8 low
        PORTB &= ~_BV(PB1); // pin 9 low
      }
      else if (count >= 4) {
        count = 0;
      }
    }
    else if (alarmOption == MAJOR_ALARM) {  // red 1 sec on 1 sec off sequence.
      if (count == 1) {
        PORTD |= _BV(PD7); // pin 9 high
        toneDuration = millis() + 1000UL;
      }
      else if (count >= 2) {
        PORTD &= ~_BV(PD7); // pin 7 low
        PORTB &= ~_BV(PB0); // pin 8 low
        PORTB &= ~_BV(PB1); // pin 9 low
        count = 0;
      }
    }
    alarmPreviousMillis = millis();
  }
}
