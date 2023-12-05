/*
  Version V4.5.3 Updated on the 5th Dec 2023
*/


// Library imports
#include <RA8875.h>
#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include <DS3231.h>
//#include <dht.h>
#include <hdc1080.h>

// Include font files
//#include "Fonts/Aerial_22.h"
#include "Fonts/Akashi_36.h"
#include "Fonts/Hallfetica_42_Nums.h"
#include "Fonts/MyriadPro_32.h"
#include "Icons.h"

// pins used
namespace pin {
// analog sensors
const byte ldr = A0;
const byte tdsSensor = A1;
const byte phSensor = A3;
const byte etapeSensor = A2;
// display
const byte ra8875_int = 2;
const byte ra8875_rst = 33;
const byte ra8875_cs = 34;
const byte ra8875_sdo = 50;
const byte ra8875_sdi = 51;
const byte ra8875_sck = 52;
// dosers
const byte doserOne = 9;
const byte doserTwo = 4;
const byte doserThree = 5;
const byte doserFour = 6;
const byte doserFive = 7;
const byte doserSix = 8;
// temp / humidity sensors
const byte oneWireBus = 38;
const byte dht22 = 39;
// ultrasonic sensor
const byte hcsrTrigger = 40;
const byte hcsrEcho = 41;
// relays
const byte airHeater = 22;
const byte co2Solenoid = 23;
const byte inletPump = 24;
const byte outletPump = 25;
const byte light = 26;
const byte waterHeater = 27;
const byte unused_relay_7 = 28;
const byte unused_relay_8 = 29;
// transistors
const byte phTransistor = 36;
const byte tdsTransistor = 37;
const byte espTransistor = 42;
}

namespace display {
uint16_t touch_x, touch_y = 0;
uint16_t lastTouchX = 0, lastTouchY = 0;
bool enableAverageTouches = false;
uint16_t maxAverageTouches = 5;
uint16_t averageTouchCnt = 0;
byte dosingDialogTimer = 10;
byte page = 0, previousPage = 0;
byte homePage = 0;
byte graphOption = 1;
byte co2PageScrollPos = 0;
byte waterPageScrollPos = 0;
byte doserPageScrollPos = 0;
byte calDoserPageScrollPos = 0;
byte refillDoserPageScrollPos = 0;
byte settingsPage = 0;
byte settingsPageZeroScrollPos = 0;
byte settingsPageOneScrollPos = 0;
byte settingsPageTwoScrollPos = 0;
byte calPhPageScrollPos = 0;
byte systemLogScrollPos = 0;
bool showSystemLogs = false;
byte maxMinsPage = 0;
byte fansPage = 0;
byte warningsPage = 0;
bool showKeyboard = false;
bool showSaveDialog = false;
bool displayIsOff = false;
bool refreshPage = true;
int cursourPosition = 0;
bool refreshCalander;
unsigned long debounceTime = 250; // in millis
unsigned long currentTouch = 0, lastTouch = 0;
unsigned long previousTouchMillis = 0;
const unsigned int RA8875_SEABLUE      = 0x1C1F;
const unsigned int RA8875_GREY         = 0x8C51;
const unsigned int RA8875_GREY2        = 0xE71C;
const unsigned int RA8875_SMOKE_WHITE  = 0xF79E;
const unsigned int RA8875_LIGHT_BLUE   = 0xA67E;
const unsigned int RA8875_LIGHT_GREEN  = 0x87F0;
const unsigned int RA8875_LIGHT_YELLOW = 0xFFF0;
const unsigned int RA8875_LIGHT_PINK   = 0xFC1F;
}

namespace message {
const char saveProfile []            PROGMEM = "Are you sure you want to\nsave profile $? This will\noverwrite all the data\ncurrently saved to profile $";
const char resetDevice []            PROGMEM = "Are you sure you want to\nreset the device to its default\nstate? This will delete all\nsave profiles and system\nconfiguration settings.";
const char setWaterHeight []         PROGMEM = "Please make sure the tank\nis completely empty and the\nheight sensor is correctly\nmounted before continuing!";
const char setEtapeMinReading []     PROGMEM = "Please remove the Etape\nfrom the water and ensure\nthe Etape is not bent before\ncontinuing the calibration!";
const char setEtapeMaxReading []     PROGMEM = "Please place the Etape\nin water up to 32cm and\nthe Etape is not bent before\ncontinuing the calibration!";
const char calibrateTds []           PROGMEM = "Please place the TDS probe\nin $ mS/cm calibration\nsolution, and do not disturb\nthe sensor then press\ncontinue.";
const char drainingAlert []          PROGMEM = "Alert the system is about\nto start draining and filling\nthe water reservoir.\nStarting in $ seconds\nDo you want to continue?";
const char cancelDraining []         PROGMEM = "The system is currently\ndraining the water reservoir\nto the minimum target $*\nPress cancel to quit draining\nat any time.";
const char refillingAlert []         PROGMEM = "Alert the system is about\nto start refilling the water\nreservoir.\nStarting in $ seconds\nDo you want to continue?";
const char cancelRefilling []        PROGMEM = "The system is currently\nfilling the water reservoir to\nthe maximum target $*\nPress cancel to quit refilling\nat any time.";
const char cancelDosing []           PROGMEM = "Dosing currently in progress\n* out of range by $%\nCurrently running doser #\nPress cancel to quit dosing\nat any time.";
const char calibrateCo2 []           PROGMEM = "Please make sure the Co2\nhas been a constant 400ppm\nfor at least an hour before\nstarting the calibration\nDo you want to continue?";
const char calibratingCo2 []         PROGMEM = "Please wait 10 seconds\nwhile the Co2 sensor is\ncalibrating. And do not\ninterfere with the Co2 while\nthe sensor is calibrating.";
const char co2Alert []               PROGMEM = "Alert the system is about\nto start pumping Co2. Please\nmake sure that no one is\ninside! Starting in $ seconds\nDo you want to continue?";
const char cancelCo2 []              PROGMEM = "Pumping Co2 for $ minutes.\nPlease make sure that no\none is inside! Press cancel to\nquit Co2 pumping at any time.";
// log messages
const char bootNotification[]        PROGMEM = "System started";     // 0
const char drainNotification[]       PROGMEM = "Drained water";      // 1
const char refillNotification[]      PROGMEM = "Refilled water";     // 2
const char co2Notification[]         PROGMEM = "Adjusted Co2";       // 3
const char lightOnNotification[]     PROGMEM = "Light on";           // 4
const char lightOffNotification[]    PROGMEM = "Light off";          // 5
const char lightErrorNotification[]  PROGMEM = "Error light off";    // 6
const char phUpNotification[]        PROGMEM = "Adjusted PH up";     // 7
const char phDownNotification[]      PROGMEM = "Adjusted PH down";   // 8
const char ecNotification[]          PROGMEM = "Adjusted EC";        // 9
const char tdsNotification[]         PROGMEM = "Adjusted TDS";       // 10
const char heaterOnNotification[]    PROGMEM = "Air heater on";      // 11
const char heaterOffNotification[]   PROGMEM = "Air heater off";     // 12
const char waterOnNotification[]     PROGMEM = "Water heater on";    // 13
const char waterOffNotification[]    PROGMEM = "Water heater off";   // 14
const char* const notificationsArray[] PROGMEM = {
  bootNotification,
  drainNotification,
  refillNotification,
  co2Notification,
  lightOnNotification,
  lightOffNotification,
  lightErrorNotification,
  phUpNotification,
  phDownNotification,
  ecNotification,
  tdsNotification,
  heaterOnNotification,
  heaterOffNotification
};
const byte maxLogs = 20;
const byte maxCharsPerLog = 13;
byte logTypeArray[maxLogs];
char timeStrArray[maxLogs][maxCharsPerLog];
byte systemLogPos = 0;
}

namespace device {
const bool relayOffState = HIGH; // does your relay use normally low or high logic for its off state?
const char versionNumber[] = "4.5.3"; // do not adjust !
const byte slaveAddress = 9;
int remotlyLoadUserProfile = -1;
int remotlySaveUserProfile = -1;
bool sensorsReady = false;
bool currentlyDosing = false;
bool airHeaterIsOn = false;
bool waterHeaterIsOn = false;
byte currentDoserNum = 0;
// bool currentlyDraining;
bool systemReset = false;
bool serialDebug = true;
byte dosingTimerHourCounter = 0;
byte isPriming; // current doser that is priming
byte previousDosingMinute = 0, previousDosingHour = 0;
unsigned long slideShowpreviousMillis = millis();
byte continueDosing = 0; // tri state bool
int scrollTouchCount = 0;
byte phCalStage = 0;
bool newGraphData = false;
byte graphArrayPos = 0;
bool ecWaitTillNextCall = false;
bool phWaitTillNextCall = false;
const byte maxGraphArrayValues = 12;
float phArray[maxGraphArrayValues];
float co2Array[maxGraphArrayValues];
float ecArray[maxGraphArrayValues];
float tdsArray[maxGraphArrayValues];
float waterTemperatureArray[maxGraphArrayValues];
float waterTemperatureArrayF[maxGraphArrayValues];
float waterLevelArray[maxGraphArrayValues];
float waterLevelArrayInInches[maxGraphArrayValues];
float airTemperatureArray[maxGraphArrayValues];
float airTemperatureArrayF[maxGraphArrayValues];
float humidityArray[maxGraphArrayValues];
float fanOneSpeedArray[maxGraphArrayValues];
float fanTwoSpeedArray[maxGraphArrayValues];
byte lightSwitchedOnHour = 0;
byte lightSwitchedOnMin = 0;
bool lightOn = false;
int intputPosition = 0;
bool upper = false;
bool refreshKeys = false;
byte profileInputNumber = 0;
byte userProfile = 0;
int lastIntputPosition = 0;
bool lockSaveButtons = false, updateKeyboardInput = false;
unsigned long keyBoardClosedTime = millis();
float minPh = 0;
float maxPh = 0;
float minCo2 = 0;
float maxCo2 = 0;
float minEc = 0;
float maxEc = 0;
float minWaterTemp = 0;
float maxWaterTemp = 0;
float minWaterLevel = 0;
float maxWaterLevel = 0;
int minTds = 0;
int maxTds = 0;
float minAirTemp = 0;
float maxAirTemp = 0;
float minHumidity = 0;
float maxHumidity = 0;
byte minFanOneSpeed = 0;
byte maxFanOneSpeed = 0;
byte minFanTwoSpeed = 0;
byte maxFanTwoSpeed = 0;
byte fanOneSpeed = 0;
byte fanTwoSpeed = 0;
byte ppmErrorState = 0;
byte ecErrorState = 0;
byte phErrorState = 0;
byte co2ErrorState = 0;
byte waterTempErrorState = 0;
byte waterLevelErrorState = 0;
byte airTempErrorState = 0;
byte humidityErrorState = 0;
byte co2TurnFansBackOnMinute = 0;
byte co2TurnFansBackOnHour = 0;
bool co2DisabledFans = false;
bool usernameAdjusted = false;
bool settingsAdjusted = false;
bool ecValsAdjusted = false;
bool phValsAdjusted = false;
bool co2ValsAdjusted = false;
bool waterValsAdjusted = false;
bool doserValsAdjusted = false;
bool lightValsAdjusted = false;
bool fanValsAdjusted = false;
bool warningValsAdjusted = false;
bool adjustPhDown = false;
const float aref = 5.0;
enum systemStates {
  NO_ADJUSTMENT,
  ADJUSTING_TDS,
  ADJUSTING_EC,
  ADJUSTING_PH,
  ADJUSTING_WATER_HEIGHT,
  ADJUSTING_CO2, 
};
byte systemStatus = NO_ADJUSTMENT; // TO DO, Set system status when, draining or refilling or dosing or Co2 pumping and send to ESP8266 as the webpage will not be able to adjust values
enum dosingDialogOption {
  NOT_SET,
  CONTINUE,
  CANCEL
};
enum etapeDialogOption {
  UNCHANGED,
  PAGE_ONE,
  PAGE_TWO
};
enum warningStates {
  NO_WARNING,
  MINOR_WARNING,
  MAJOR_WARNING
};
enum doserModes {
  DOSER_OFF,
  DOSER_EC,
  DOSER_PH
};
enum conversionTypes {
  EU,
  US,
  AU
}; // EC 0.64, US 0.50, AU 0.7
byte conversionType = EU;
enum controlOptions {
  SPEED_IDLE,
  SPEED_DOWN,
  SPEED_UP
};
}

namespace user {
char profileOneName[11]   = "Profile_1";
char profileTwoName[11]   = "Profile_2";
char profileThreeName[11] = "Profile_3";
char profileFourName[11]  = "Profile_4";
char profileFiveName[11]  = "Profile_5";
byte lightOnTimeHour = 23;
byte lightOnTimeMin = 0;
byte lightOffTimeHour = 22;
byte lightOffTimeMin = 30;
byte lightMode = 1;
byte targetMinFanOneSpeed = 25;
byte targetMaxFanOneSpeed = 100;
byte targetMinFanTwoSpeed = 50;
byte targetMaxFanTwoSpeed = 100;
float targetMinAirTemp = 20;
float targetMaxAirTemp = 27.5;
float targetMinAirTempF = 68.0; // NEW IN F
float targetMaxAirTempF = 81.5; // NEW IN F
float targetMinHumidity = 40.1;
float targetMaxHumidity = 60.2;
bool fansControlTemperature = true;
bool fansControlHumidity = false;
bool fanOneFixedSpeed = false;
bool fanTwoFixedSpeed = false;
byte numberOfDosers = 6;
byte doserOneMode = device::DOSER_EC;   // OFF, EC, PH +
byte doserTwoMode = device::DOSER_PH;   // OFF, EC, PH +
byte doserThreeMode = device::DOSER_EC; // OFF, EC, PH +
byte doserFourMode = device::DOSER_PH;  // OFF, EC, PH -
byte doserFiveMode = device::DOSER_EC;  // OFF, EC, PH +
byte doserSixMode = device::DOSER_EC;   // OFF, EC, PH -
int doserOneMills = 10;
int doserTwoMills = 10;
int doserThreeMills = 10;
int doserFourMills = 10;
int doserFiveMills = 10;
int doserSixMills = 10;
int targetMinTds = 400;
int targetMaxTds = 600;
int targetCo2 = 400;
int co2Offset = 99;
int roomWidthCm = 120;
int roomLengthCm = 120;
int roomHeightCm = 200;
float co2FlowrateLtrs = 6;
int roomWidthInches = 120;
int roomLengthInches = 120;
int roomHeightInches = 200;
float co2FlowrateFeet3 = 6;
byte co2CheckTimeMinute = 0;
byte co2CheckTimeHour = 22;
int disableFansTimer = 30;
bool enableManualCo2Duration = false;
int manualCo2GasDuration = 30;
bool disableCo2Control = true;
float targetMinPh = 6.0;
float targetMaxPh = 6.5;
float targetMinEc = 1.60;
float targetMaxEc = 1.70;
float targetMinWaterTemp = 20.0;
float targetMaxWaterTemp = 27.0;
float targetMinWaterTempF = 73.4;
float targetMaxWaterTempF = 77.0;
float targetMinWaterHeight = 10.1;
float targetMaxWaterHeight = 32.2;
float targetMinWaterHeightInches = 10.1;
float targetMaxWaterHeightInches = 32.2;
byte doserOneSpeed = 100;
byte doserTwoSpeed = 100;
byte doserThreeSpeed = 100;
byte doserFourSpeed = 100;
byte doserFiveSpeed = 100;
byte doserSixSpeed = 100;
byte dosingInterval = 6;
byte swapInterval = 2;
int refillDoserOneMills = 10;
int refillDoserTwoMills = 10;
int refillDoserThreeMills = 10;
int refillDoserFourMills = 10;
int refillDoserFiveMills = 10;
int refillDoserSixMills = 10;
bool autoFillDays[31];
byte autoFillHour = 12;
byte autoFillMinute = 0;
byte drainTimeout = 1;
unsigned int backgroundColor = display::RA8875_LIGHT_BLUE;
unsigned long graphInterval = 1;
unsigned long afkTime = 1; // in minutes
bool setRTCtime = false;
bool convertToF  = false;
bool convertToInches = false;
bool convertToTds = false;
int tdsErrorMargin = 50;
float ecErrorMargin = 0.5;
float phErrorMargin = 0.5;
float co2ErrorMargin = 100;
float waterTempErrorMargin = 1.0;
float waterTempErrorMarginF = 1.0;
float waterHeightErrorMargin = 5;
float waterHeightErrorMarginInches = 1;
float airTempErrorMargin = 3.0;
float airTempErrorMarginF = 2.0;
float humidityErrorMargin = 5;
bool disablePpmWarnings = true;
bool disableEcWarnings = true;
bool disableCo2Warnings = true;
bool disablePhWarnings = true;
bool disableWaterTempWarnings = true;
bool disableWaterHeightWarnings = true;
bool disableAirTempWarnings = true;
bool disableHumidityWarnings = true;
//bool disableAllWarnings = false;
bool disableLED = false;
bool disableBeeper = false;
bool disableDrainAndRefill = true;
bool disableMachineLearning = true;
bool clearWifiDetails = false;
bool resetSettings = false;
bool showSr04HeightCalibration = false;
byte showEtapeCalibration = device::UNCHANGED;
bool showDoserCalibration = false;
bool showTdsCalibration = false;
bool showCo2Calibration = false;
bool showPhCalibration = false;
byte rtcDayOfWeek = 0;
byte rtcSecond = 0;
byte rtcMinute = 0;
byte rtcHour = 0;
byte rtcDay = 0;
byte rtcMonth = 0;
int rtcYear = 0;
bool useEtapeSensor = true;
}

namespace sensor {
float ec = 0;
float ph = 0;
int tds = 0;
int co2 = 0;
int ldr = 0;
float waterTemp = 0;
float waterTempF = 0;
float airTemp = 0;
float humidity = 0;
float waterLevel = 0;
float waterLevelInches = 0;
float emptyWaterTankDepth = 0;
long hcsrDuration = 0;
float ecCalibration = 1;
const float ecCalibrationSolution = 707; // TDS in ppm
int phPotStep = 0;
float phVoltage = 0;
float ph4Voltage = 2.969;
float ph7Voltage = 2.690;
const float ph7BufferValue = 7.0;
const float ph4BufferValue = 4.01;
float phVoltagePerUnit = 0.0936;
unsigned long co2GasTime = 0;
float temp_etapeZeroVolumeResistance = 0;
float etapeZeroVolumeResistance = 1877.79; // 2104.56  resistance value (in ohms) when no liquid is present.
float etapeMaxVolumeResistance = 271.4; // 288.71 resistance value (in ohms) when liquid is at max line.
const float etapeCalibrationCm = 31.00; // maximum mesurement in centimeters on etap 12" version
const float etapeOffset = 2.5; // Lowest value read on etape in Centimeters, datasheet says 2.5 but i get 2.5
const byte co2Request[] {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
}

namespace wifi {
bool wifiEnabled = false;
unsigned long connectionTime = 0;
unsigned long wifiPreviousMillis = 0;
enum connectionStatus {
  UNCONNECTED,
  CONNECTED,
  TIMEOUT,
  FAILED
};
byte connectionState = UNCONNECTED;
}

namespace machineLearning {
const byte maxSize = 20;
unsigned int phDownDosingInc[maxSize] {0};
unsigned int phUpDosingInc[maxSize]   {0};
unsigned int ecDosingInc[maxSize]     {0};
unsigned int tdsDosingInc[maxSize]    {0};
byte phDownArrayBlock = 0;
byte phUpArrayBlock = 0;
byte ecArrayBlock = 0;
byte tdsArrayBlock = 0;
bool savePhDownResult = false;
bool savePhUpResult = false;
bool saveEcResult = false;
bool saveTdsResult = false;
bool logPhDownResult = false;
bool logPhUpResult = false;
bool logEcResult = false;
bool logTdsResult = false;
int phDownMls = 0, previousPhDownMls = 0;
int phUpMls = 0, previousPhUpMls = 0;
int ecMls = 0, previousEcMls = 0;
int tdsMls = 0, previousTdsMls = 0;
float previousPhDownSensor = 0, phDownSensorHistory = 0;
float previousPhUpSensor = 0, phUpSensorHistory = 0;
float previousEcSensor = 0, ecSensorHistory = 0;
float previousTdsSensor = 0, tdsSensorHistory = 0;
float phDownMultipler = 1;
float phUpMultipler = 1;
float ecMultipler = 1;
float tdsMultipler = 1;
enum modes {
  PH_DOWN,
  PH_UP,
  EC,
  TDS
};
}

// Class instances
RA8875 tft = RA8875(pin::ra8875_cs, pin::ra8875_rst);
OneWire oneWire(pin::oneWireBus);
DallasTemperature dallasTemperature(&oneWire);
DS3231 rtc(SDA, SCL);
Time rtcTime;
//dht DHT;
HDC1080 hdc;

void(*reset)(void) = 0; // Reset function

void setup() {
  Serial.begin(115200); // debugging
  Serial1.begin(115200); // esp8266-01
  Serial2.begin(9600); // co2 sensor
  pinMode(pin::hcsrEcho, INPUT);
  pinMode(pin::hcsrTrigger, OUTPUT);
  pinMode(pin::doserOne, OUTPUT);
  pinMode(pin::doserTwo, OUTPUT);
  pinMode(pin::doserThree, OUTPUT);
  pinMode(pin::doserFour, OUTPUT);
  pinMode(pin::doserFive, OUTPUT);
  pinMode(pin::doserSix, OUTPUT);
  pinMode(pin::light, OUTPUT);
  pinMode(pin::waterHeater, OUTPUT);
  pinMode(pin::inletPump, OUTPUT);
  pinMode(pin::outletPump, OUTPUT);
  pinMode(pin::airHeater, OUTPUT);
  pinMode(pin::co2Solenoid, OUTPUT);
  pinMode(pin::unused_relay_7, OUTPUT);
  pinMode(pin::unused_relay_8, OUTPUT);
  pinMode(pin::tdsTransistor, OUTPUT);
  pinMode(pin::phTransistor, OUTPUT);
  pinMode(pin::espTransistor, OUTPUT);
  // set pin states where required
  digitalWrite(pin::light, device::relayOffState);
  digitalWrite(pin::waterHeater, device::relayOffState);
  digitalWrite(pin::inletPump, device::relayOffState);
  digitalWrite(pin::outletPump, device::relayOffState);
  digitalWrite(pin::airHeater, device::relayOffState);
  digitalWrite(pin::co2Solenoid, device::relayOffState);
  digitalWrite(pin::unused_relay_7, device::relayOffState);
  digitalWrite(pin::unused_relay_8, device::relayOffState);
  //analogReference(EXTERNAL);
  Wire.begin();
  SPI.begin();
  tft.begin(RA8875_800x480); // Uncomment for BuyDisplay and comment out the line below
  //tft.begin(Adafruit_800x480); // Uncomment for Adafruit Display and comment out the line above
  tft.useINT(pin::ra8875_int);
  tft.touchBegin();
  tft.enableISR(true);
  initializeDevice();
}

void loop() {
  touchEvent();
  readSensors();
  drawPages();
  envriomentalControl();
  displayWarnings();
  espSerial();
  screenSaver();
}

void initializeDevice() {
  Serial.print(F("Hydro Master v")); Serial.print(device::versionNumber); Serial.println(F(" started..."));
  tft.fillWindow(RA8875_BLACK);
  tft.setFont(&akashi_36px_Regular);
  tft.setFontScale(2);
  tft.setTextColor(RA8875_WHITE);
  tft.setCursor(120, 200);
  tft.print(F("Hydro master"));
  tft.setFontScale(1);
  tft.setCursor(120, 276);
  tft.print(F("version ")); tft.print(device::versionNumber);
  int startX = 305;
  for (byte i = 0; i < 6; i++) {
    tft.fillCircle(startX, 340, 14, RA8875_WHITE);
    startX += 38;
    delay(500);
  }
  initializeEEPROM();
  dallasTemperature.begin();
  rtc.begin();
  rtcTime = rtc.getTime();
  //DHT.read22(pin::dht22);
  hdc.begin();
  sendToSlave('P', sensor::phPotStep);
  tft.fillWindow(user::backgroundColor);
  frame();
  OuterMenuIcons();
  //display::lastTouchX = tft.width();
  // If light is set to auto mode check timer
  if (user::lightMode == 0 && restartLightingTimer()) {
    digitalWrite(pin::light, !device::relayOffState);
    device::lightOn = true;
    device::lightSwitchedOnHour = rtcTime.hour;
    device::lightSwitchedOnMin = rtcTime.min;
    Serial.println(F("Auto started light on boot"));
  }
  // If the LED is disabled send cmd to Slave
  if (user::disableLED)
    sendToSlave('L', 1);
  // If the sound is disabled send cmd to Slave
  if (user::disableBeeper)
    sendToSlave('B', 1);
  // If the wifi is enabled turn on the ESP
  if (wifi::wifiEnabled)
    digitalWrite(pin::espTransistor, HIGH);
  else
    digitalWrite(pin::espTransistor, LOW);
  // Load the Co2 sensor
  Serial2.write(sensor::co2Request, 9);
  delay(50);
  Serial2.flush();
  // Set system variables to their defaults
  device::fanOneSpeed = user::targetMinFanOneSpeed;
  device::fanTwoSpeed = user::targetMinFanTwoSpeed;
  if (device::graphArrayPos == 0) {
    device::phArray[0] = 0.001; // Dont adjust, set out of range of viewed precison, thus 0
    device::ecArray[0] = 0.001;
    device::tdsArray[0] = 0.01;
    device::co2Array[0] = 0.01;
    device::waterTemperatureArray[0] = 0.001;
    device::waterTemperatureArrayF[0] = 0.001;
    device::waterLevelArray[0] = 0.01;
    device::waterLevelArrayInInches[0] = 0.001;
    device::airTemperatureArray[0] = 0.001;
    device::airTemperatureArrayF[0] = 0.001;
    device::humidityArray[0] = 0.01;
    device::fanOneSpeedArray[0] = 0.01;
    device::fanTwoSpeedArray[0] = 0.01;
    device::graphArrayPos = 1;
  }
  rtcTime = rtc.getTime();
  device::previousDosingMinute = rtcTime.min;
  device::previousDosingHour = rtcTime.hour;
  saveLogMessage(0); // save log message, system started
}
