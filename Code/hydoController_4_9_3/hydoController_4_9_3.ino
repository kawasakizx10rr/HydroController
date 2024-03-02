/*
  Version V4.9.3 Updated on the 18th Feb 2024
*/
// TO DO: add user setup guide for new device / reset
// add back in esp8266 wifi
// device::settingsAdjusted = true;
// water height as float

//#define USING_HDC1080 // comment this line to use a dht22

// Library imports
#include <SPI.h>
#include <RA8875.h>
#include <Wire.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <uRTCLib.h>
#ifdef USING_HDC1080
#include "Adafruit_HDC1000.h"
#else
#include <dht.h>
#endif
#include "Adafruit_VL53L0X.h"
#include "DFRobot_PH.h"
#include "GravityTDS.h"
// Include font files
#include "Fonts/Aerial_22.h"
#include "Fonts/Akashi_36.h"

#include "Fonts/Hallfetica_42_Nums.h"
#include "Fonts/MyriadPro_32.h"
#include "Icons.h"


// pins used
namespace pin {
// analog sensors
const uint8_t ldr = A0;
const uint8_t tdsSensor = A1;
const uint8_t phSensor = A3;
const uint8_t etapeSensor = A2;
// display
const uint8_t ra8875_int = 2;
const uint8_t ra8875_rst = 33;
const uint8_t ra8875_cs = 34;
const uint8_t ra8875_sdo = 50;
const uint8_t ra8875_sdi = 51;
const uint8_t ra8875_sck = 52;
// dosers
const uint8_t doserOne = 9;
const uint8_t doserTwo = 4;
const uint8_t doserThree = 5;
const uint8_t doserFour = 6;
const uint8_t doserFive = 7;
const uint8_t doserSix = 8;
// temp / humidity sensors
const uint8_t oneWireBus = 38;
const uint8_t dht22 = 39;
// ultrasonic sensor
const uint8_t hcsrTrigger = 40;
const uint8_t hcsrEcho = 41;
// relays
const uint8_t airHeater = 22;
const uint8_t co2Solenoid = 23;
const uint8_t inletPump = 24;
const uint8_t outletPump = 25;
const uint8_t light = 26;
const uint8_t waterHeater = 27;
const uint8_t auxRelayOne = 28;
const uint8_t auxRelayTwo = 29;
// transistors
const uint8_t phTransistor = 36;
const uint8_t tdsTransistor = 37;
const uint8_t espTransistor = 42;
}

enum fanModes {
  FAN_ON,
  FAN_OFF,
  FAN_AUTO
};

namespace display {
uint32_t lastButtonTouch = millis();
uint16_t touch_x, touch_y = 0;
uint16_t lastTouchX = 0, lastTouchY = 0;
int16_t scrollLeftCount = 0, scrollRightCount = 0, lastScrollX = 0;
uint8_t page = 0, previousPage = 0;
uint8_t homePage = 0;
bool showTdsGraph = true;
bool showPhGraph = true;
bool showEcGraph = true;
bool showCo2Graph = true;
bool showWaterTempGraph = true;
bool showWaterLvlGraph = true;
bool showFanOneGraph = true;
bool showFanTwoGraph = true;
bool showAirTempGraph = true;
bool showHumidityGraph = true;
bool showCalErrorMessage = false;
bool showingDialog = false;
bool showWifiSsid = false; 
bool showWifiPassword = false;
bool showSystemLogs = false;
bool showAfkTime = false;
bool showBrightness = false;
bool showGraphInterval = false;
bool showNumberOfDosers = false;
bool showDosingInterval = false;
bool showEcDosingMode = false;
bool showPhDosingMode = false;
bool showEcTdsValue = false;
bool showPhDownUpValue = false;
bool showSr04HeightCalibration = false;
uint8_t showEtapeCalibration = 0;
uint8_t showTouchCalibration = 0; ////////////////
bool showDoserCalibration = false;
bool showTdsCalibration = false;
bool showCo2Calibration = false;
bool showPhCalibration = false;
bool showRTCtime = false;
bool showKeyboard = false;
bool showSaveDialog = false;
bool showInfoDialog = false;
uint8_t co2PageScrollPos = 0;
uint8_t waterPageScrollPos = 0;
uint8_t doserPageScrollPos = 0;
uint8_t calDoserPageScrollPos = 0;
uint8_t calPhPageScrollPos = 0;
uint8_t calTdsPageScrollPos = 0;
uint8_t refillDoserPageScrollPos = 0;
uint8_t timerPagePageScrollPos = 0;
uint8_t settingsPage = 0;
uint8_t settingsPageZeroScrollPos = 0;
uint8_t settingsPageOneScrollPos = 0;
uint8_t settingsPageTwoScrollPos = 0;
uint8_t settingsPageThreeScrollPos= 0;
uint8_t systemLogScrollPos = 0;
uint8_t setRtcTimePos = 0;
uint8_t maxMinsPage = 0;
uint8_t fansPage = 0;
uint8_t warningsPage = 0;
bool displayIsOff = false;
bool refreshPage = true;
int16_t cursourPosition = 0;
bool refreshCalander;
uint16_t debounceTime = 200; // in millis
uint32_t lastTouchMillis = 0;

uint32_t touchStartMillis = 0; // time button has been held downconst char*
//uint32_t lastDialogTouchTime = 0;
uint32_t infoDialogDisplayTime = 0;

const uint16_t RA8875_SEABLUE      = 0x1C1F;
const uint16_t RA8875_DARKGREY     = 0x8C51;
const uint16_t RA8875_LIGHTGREY    = 0xE71C;
const uint16_t RA8875_MIDGREY      = 0x8410;
const uint16_t RA8875_ORANGE       = 0xFD06;
//const uint16_t RA8875_SMOKE_WHITE  = 0xF79E;
const uint16_t RA8875_LIGHT_BLUE   = 0xA67E;
const uint16_t RA8875_LIGHT_GREEN  = 0x87F0;
const uint16_t RA8875_LIGHT_YELLOW = 0xFFF0;
const uint16_t RA8875_LIGHT_PINK   = 0xFC1F;
const uint16_t RA8875_PURPLE       = 0x981F;
}

namespace message {
// dialogs
const char saveProfile[]                PROGMEM = "Are you sure you want to\nsave profile $? This will\noverwrite all the data\ncurrently saved to profile $";
const char resetDevice[]                PROGMEM = "Are you sure you want to\nreset the device to its default\nstate? This will delete all\nsave profiles and system\nconfiguration settings.";
const char setWaterHeight[]             PROGMEM = "Please make sure the tank\nis completely empty and the\nheight sensor is correctly\nmounted before continuing!";
const char setEtapeMinReading[]         PROGMEM = "Please remove the Etape\nfrom the water and ensure\nthe Etape is not bent before\ncontinuing the calibration!";
const char setEtapeMaxReading[]         PROGMEM = "Please place the Etape\nin water up to 32cm and\nthe Etape is not bent before\ncontinuing the calibration!";
const char calibrateError[]             PROGMEM = "Error: the calibrate has\nfailed, please try again\nor cancel the calibration.";
const char calibrateTds[]               PROGMEM = "Please place the TDS probe\nin $ uS/cm calibration\nsolution, and do not disturb\nthe sensor then press\ncontinue.";
const char calibratePh[]                PROGMEM = "Please place the PH probe\nin PH $ calibration\nsolution, and do not disturb\nthe sensor then press\ncontinue.";
const char drainingAlert[]              PROGMEM = "Alert the system is about\nto start draining and filling\nthe water reservoir.\nStarting in $ seconds\nDo you want to continue?";
const char cancelDraining[]             PROGMEM = "The system is currently\ndraining the water reservoir\nto the minimum target $*\nPress cancel to quit draining\nat any time.";
const char refillingAlert[]             PROGMEM = "Alert the system is about\nto start refilling the water\nreservoir.\nStarting in $ seconds\nDo you want to continue?";
const char cancelRefilling[]            PROGMEM = "The system is currently\nfilling the water reservoir to\nthe maximum target $*\nPress cancel to quit refilling\nat any time.";
const char cancelDosing[]               PROGMEM = "Dosing currently in progress\n* out of range by $%\nCurrently running doser #\n@ mls remaing to pump\nPress cancel to quit dosing.";
const char calibrateCo2[]               PROGMEM = "Please make sure the Co2\nhas been a constant 400ppm\nfor at least an hour before\nstarting the calibration\nDo you want to continue?";
const char calibratingCo2[]             PROGMEM = "Please wait 10 seconds\nwhile the Co2 sensor is\ncalibrating. And do not\ninterfere with the Co2 while\nthe sensor is calibrating.";
const char co2Alert[]                   PROGMEM = "Alert the system is about\nto start pumping Co2. Please\nmake sure that no one is\ninside! Starting in $ seconds\nDo you want to continue?";
const char cancelCo2[]                  PROGMEM = "Pumping Co2 for $ minutes.\nPlease make sure that no\none is inside! Press cancel to\nquit Co2 pumping at any time.";
// pages info's
const char ecPageInfo[]                 PROGMEM = "Set the minimum and maximum EC target. The EC will be adjusted when the EC falls below the minimum target.";
const char tdsPageInfo[]                PROGMEM = "Set the minimum and maximum TDS target. The TDS will be adjusted when the TDS falls below the minimum target.";
const char phPageInfo[]                 PROGMEM = "Set the minimum and maximum PH target. The PH will be adjusted when the PH goes outside the minimum or maximum targets.";
const char co2Page1Info[]               PROGMEM = "Set the Co2 target and offset. The Co2 will be adjusted when the Co2 falls below the target minus the offset.";
const char co2Page2Info[]               PROGMEM = "Set the dimensions of the room to control the Co2. The system will use these measurements to assist in calculating the gas duration.";
const char co2Page3Info[]               PROGMEM = "Set the Co2 flow rate, the time to adjust the rooms Co2, and the duration in minutes to disable the extraction fans.";
const char co2Page4Info[]               PROGMEM = "Enable or disable manual Co2 gas duration, when enabled the Co2 duration will be static, and manual Co2 gas duration can be adjusted. The Co2 adjustment can also be disabled";
const char waterPage1Info[]             PROGMEM = "Set the minimum and maximum target water height. The water will be refilled with the water falls below the minimum target.";
const char waterPage2Info[]             PROGMEM = "Set the minimum and maximum target water temperature. The water will be heated when the temperature falls below the minimum target.";
const char waterPage3Info[]             PROGMEM = "Set the dimensions of the water tank. The system will use these measurements to assist in calculating the precise EC, TDS, PH dosing amount.";
const char waterPage4Info[]             PROGMEM = "Set the days and time to refresh the water tank. The system will drain and refill the water and run the doser using the fixed refill tank mls. The drain and refill can also be disabled.";
const char waterPage5Info[]             PROGMEM = "Set the dosing millilitres for each dosing pump. The system will run the dosers using the fixed refill tank mls after the drain and refill.";
const char dosingPageInfo[]             PROGMEM = "Set the dosing millilitres when in incremental mode and dosing mode for each pump. EC# mode is for nutrients that does not effect the EC. You can also prime each pump.";
const char lightPageInfo[]              PROGMEM = "Set the light on and off time, and the timer mode. The modes available are constant on, constant off, and auto. A warning will be raised if the LDR fails to detect the light.";
const char fanPage1Info[]               PROGMEM = "Set the minimum and maximum fans speed targets for the main fan. Note, you should ensure that at the minimum fan speed, the fan still rotates sufficiently.";
const char fanPage2Info[]               PROGMEM = "Set the minimum and maximum fans speed targets for the secondary fan. Note, you should ensure that at the minimum fan speed, the fan still rotates sufficiently.";
const char fanPage3Info[]               PROGMEM = "Set the minimum and maximum air temperature targets. The system will use these values to control the rooms air temperature via the extraction fans."; 
const char fanPage4Info[]               PROGMEM = "Set the minimum and maximum humidity targets. The system will use these values to control the rooms humidity via the extraction fans.";
const char fanPage5Info[]               PROGMEM = "Set the fan modes. The air temperature control, and humidity control can be disabled. And each fan can be set to a fixed speed mode.";
const char warningPageInfo[]            PROGMEM = "Set the error margin for each environmental reading. When the given reading is outside the target +/- the error margin, a critical error is raised.";
const char maxminsPageInfo[]            PROGMEM = "View the environmental readings history. Note the maximum and minimum readings are shared between save profiles.";
const char profilesPageInfo[]           PROGMEM = "Select the user profile to load. You can save and load the current settings to any of the profiles., and rename each of the profile.";
// settings Info[]
const char calDoserPageInfo[]           PROGMEM = "Set the PWM for each of the dosing pumps, the aim is to achive 1 milliliter per second. You can also adjust the pumps voltage on the PCB if required."; 
const char rtcTimePageInfo[]            PROGMEM = "Set the date and time for the clock. Note the time is a 24 hour format and the date is a dd-mm-yyyy format.";
const char displayTimeoutPageInfo[]     PROGMEM = "Set the display timeout interval in minutes, the display will be turned off when it has not been touched for the given interval.";
const char graphIntervalPageInfo[]      PROGMEM = "Set the graph update interval in seconds or minutes. The valid range is 2 to 59 seconds, or 1 to 999 minutes.";
const char dosingIntervalPageInfo[]     PROGMEM = "Set the dosing interval in hours. The system will adjust the waters EC, TDS or PH every dosing interval.";
const char numDosersPageInfo[]          PROGMEM = "Set the number of dosing pumps connected to the hydro controller. The minimum number of dosing pumps is 4.";
const char systemLosPageInfo[]          PROGMEM = "View the log history, this includes a brief description and the date and time of each environmental adjustment.";
const char ecDosingModePageInfo[]       PROGMEM = "Set the EC/TDS dosing mode, when set to precise the system will calculate the dosing mls required to meet the target, and when set to incremental the system will dose the user defined mls.";
const char phDosingModePageInfo[]       PROGMEM = "Set the PH dosing mode, when set to precise the system will calculate the dosing mls required to meet the target, and when set to incremental the system will dose the user defined mls.";
const char ecTdsValuePageInfo[]         PROGMEM = "Set the value of the EC and TDS nutrients used to adjust the waters EC/TDS when EC mode is set to precise. Note this is the value of all the nutrients mixed together.";
const char phValuePageInfo[]            PROGMEM = "Set the value of the PH down and PH up buffer solutions, these values are used to adjust the waters PH when PH mode is set to precise.";
const char wifiSsidPageInfo[]           PROGMEM = "Set the SSID for the WiFi hotspot, note the minimum SSID length is 8 characters and 16 characters maximum.";
const char wifiPassPageInfo[]           PROGMEM = "Set the password for the WiFi hotspot, note the minimum password length is 8 characters and 16 characters maximum.";
const char auxRelayTimerPageInfo[]      PROGMEM = "Set the duration for relays cyclic timer. When set to auto, the relay will turn on and off every duration, else it can be set to be permanently on or off.";
const char selectBrightnessInfo[]       PROGMEM = "Set the display brightness in precent. Please avoid setting the display brighter than required, as it can impact the lifetime of the display.";
const char pleaseWait[]                 PROGMEM = "Please wait a few seconds while the calibration is preformed.";
const char* const infoMessageArray[] PROGMEM = {
  ecPageInfo,                    // 0
  tdsPageInfo,                   // 1
  phPageInfo,                    // 2
  co2Page1Info,                  // 3
  co2Page2Info,                  // 4
  co2Page3Info,                  // 5
  co2Page4Info,                  // 6
  waterPage1Info,                // 7
  waterPage2Info,                // 8
  waterPage3Info,                // 9
  waterPage4Info,                // 10
  waterPage5Info,                // 11
  dosingPageInfo,                // 12
  lightPageInfo,                 // 13
  fanPage1Info,                  // 14
  fanPage2Info,                  // 15
  fanPage3Info,                  // 16
  fanPage4Info,                  // 17
  fanPage5Info,                  // 18
  warningPageInfo,               // 19
  maxminsPageInfo,               // 20 
  profilesPageInfo,              // 21
  calDoserPageInfo,              // 22
  rtcTimePageInfo,               // 23
  displayTimeoutPageInfo,        // 24
  graphIntervalPageInfo,         // 25
  dosingIntervalPageInfo,        // 26
  numDosersPageInfo,             // 27
  systemLosPageInfo,             // 28
  ecDosingModePageInfo,          // 29
  phDosingModePageInfo,          // 30
  ecTdsValuePageInfo,            // 31
  phValuePageInfo,               // 32
  wifiSsidPageInfo,              // 33
  wifiPassPageInfo,              // 34
  auxRelayTimerPageInfo,         // 35
  selectBrightnessInfo,          // 36
  pleaseWait                     // 37
};
// log messages
const char bootNotification[]           PROGMEM = "System started";     // 0
const char drainNotification[]          PROGMEM = "Drained water";      // 1
const char refillNotification[]         PROGMEM = "Refilled water";     // 2
const char co2Notification[]            PROGMEM = "Adjusted Co2";       // 3
const char lightOnNotification[]        PROGMEM = "Light on";           // 4
const char lightOffNotification[]       PROGMEM = "Light off";          // 5
const char lightErrorNotification[]     PROGMEM = "Error light off";    // 6
const char phUpNotification[]           PROGMEM = "Adjusted PH up";     // 7
const char phDownNotification[]         PROGMEM = "Adjusted PH down";   // 8
const char ecNotification[]             PROGMEM = "Adjusted EC";        // 9
const char tdsNotification[]            PROGMEM = "Adjusted TDS";       // 10
const char heaterOnNotification[]       PROGMEM = "Air heater on";      // 11
const char heaterOffNotification[]      PROGMEM = "Air heater off";     // 12
const char waterOnNotification[]        PROGMEM = "Water heater on";    // 13
const char waterOffNotification[]       PROGMEM = "Water heater off";   // 14
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
  heaterOffNotification,
  waterOnNotification,
  waterOffNotification
};
const uint8_t maxLogs = 50;
const uint8_t maxCharsPerLog = 13; // 31 Jan 13:45
uint8_t logTypeArray[maxLogs];
char timeStrArray[maxLogs][maxCharsPerLog];
uint8_t systemLogPos = 0;
uint8_t infoPos = 0;
}

namespace device {
uint32_t prevMillis = millis();
bool globalDebug = true; 
float currentDoserMls = 0;
uint16_t profileEEPROMSize = 0;
uint16_t systemEEPROMSize = 0;
const char* versionNumber = "4.9.5";
bool relayOffState = HIGH;
bool disableVL53L0X = false;
const uint8_t unoAddress = 9;
bool sensorsReady = false;
bool currentlyDosing = false;
bool airHeaterIsOn = false;
bool waterHeaterIsOn = false;
uint8_t currentDoserNum = 0;
// bool currentlyDraining;
uint8_t dosingTimerHourCounter = 0;
uint32_t primeTouchTime = 0;
uint8_t doserIsPriming = 0;
//bool doserIsPriming[6] {0,0,0,0,0,0};
uint8_t previousDosingMinute = 0;
uint8_t previousDosingHour = 0;
uint32_t slideShowpreviousMillis = millis();
uint32_t sensorPreviousMillis = 0;
uint8_t continueDosing = 0;
uint8_t phCalStage = 0;
bool newGraphData = false;
bool ecWaitTillNextCall = false;
bool phWaitTillNextCall = false;
int16_t lightDuration = -1;
bool lightOn = false;
//uint8_t auxRelayOneSwitchedOnOffDay = 0;
int16_t auxRelayOneDuration = -1;
bool auxRelayOneOn = false;
//uint8_t auxRelayTwoSwitchedOnOffDay = 0;
int16_t auxRelayTwoDuration = -1;
bool auxRelayTwoOn = false;
int16_t intputPosition = 0;
enum charTypes {LOWER, UPPER, SPECIAL};
charTypes charType = LOWER;
const char specialSymbols[11] PROGMEM = { '!', '@', '#', '&', '$', '%', '?', '/', '-', '~', '*' };
bool refreshKeys = false;
uint8_t profileInputNumber = 0;
uint8_t userProfile = 0;
int16_t lastIntputPosition = 0;
bool lockSaveButtons = false, updateKeyboardInput = false;
uint32_t keyBoardClosedTime = millis();
uint32_t lastCalTouch = millis();
uint16_t calMinX = TOUCSRCAL_XLOW;
uint16_t calMinY = TOUCSRCAL_YLOW;
uint16_t calMaxX = TOUCSRCAL_XHIGH;
uint16_t calMaxY = TOUCSRCAL_YHIGH;
float minPh = 0;
float maxPh = 0;
uint16_t minCo2 = 0;
uint16_t maxCo2 = 0;
float minEc = 0;
float maxEc = 0;
float minWaterTemp = 0;
float maxWaterTemp = 0;
float minWaterLevel = 0;
float maxWaterLevel = 0;
uint16_t minTds = 0;
uint16_t maxTds = 0;
float minAirTemp = 0;
float maxAirTemp = 0;
float minHumidity = 0;
float maxHumidity = 0;
uint8_t minFanOneSpeed = 0;
uint8_t maxFanOneSpeed = 0;
uint8_t minFanTwoSpeed = 0;
uint8_t maxFanTwoSpeed = 0;
float fanOneSpeed = 0;
float fanTwoSpeed = 0;
uint8_t ppmErrorState = 0;
uint8_t ecErrorState = 0;
uint8_t phErrorState = 0;
uint8_t co2ErrorState = 0;
uint8_t waterTempErrorState = 0;
uint8_t waterLevelErrorState = 0;
uint8_t airTempErrorState = 0;
uint8_t humidityErrorState = 0;
uint8_t globalErrorState = 0;
uint8_t co2TurnFansBackOnMinute = 0;
uint8_t co2TurnFansBackOnHour = 0;
bool co2DisabledFans = false;
bool usernameAdjusted = false;
bool settingsAdjusted = false;
const float aref = 5.0;  // TO DO?
enum eepromModes {
  EEPROM_VERIFY = -1, 
  EEPROM_GET = 0, 
  EEPROM_SET = 1
};
enum systemStates {
  NO_ADJUSTMENT,
  ADJUSTING_TDS,
  ADJUSTING_EC,
  ADJUSTING_PH,
  ADJUSTING_WATER_HEIGHT,
  ADJUSTING_CO2, 
};
uint8_t systemStatus = NO_ADJUSTMENT;
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
  DOSER_EC_OP,
  DOSER_PH_DOWN,
  DOSER_PH_UP
};
enum conversionTypes { // to do
  EU,
  US,
  AU
}; // EC 0.64, US 0.50, AU 0.7
uint8_t conversionType = EU;
enum controlOptions {
  SPEED_IDLE,
  SPEED_DOWN_SLOW,
  SPEED_DOWN,
  SPEED_UP,
  SPEED_UP_SLOW,
  SPEED_MIN,
  SPEED_MAX
};
enum timerStatus {
  AUTO_TIMER,
  CONST_ON,
  CONST_OFF
};
}

namespace user {
char profileOneName[16]   = "Profile_1";
char profileTwoName[16]   = "Profile_2";
char profileThreeName[16] = "Profile_3";
char profileFourName[16]  = "Profile_4";
char profileFiveName[16]  = "Profile_5";

uint8_t lightOnTimeHour = 23;
uint8_t lightOnTimeMin = 0;
uint8_t lightOffTimeHour = 22;
uint8_t lightOffTimeMin = 30;
uint8_t lightState = device::AUTO_TIMER;
uint8_t lightMode = 1; // 0 cyclic, 1 timer
int16_t lightOffDuration = 60;
int16_t lightOnDuration = 60;

uint8_t auxRelayOneOnTimeHour = 23;
uint8_t auxRelayOneOnTimeMin = 0;
uint8_t auxRelayOneOffTimeHour = 22;
uint8_t auxRelayOneOffTimeMin = 30;
uint8_t auxRelayOneState = device::AUTO_TIMER;
uint8_t auxRelayOneMode = 0; // 0 cyclic, 1 timer
int16_t auxRelayOneOnDuration = 60;
int16_t auxRelayOneOffDuration = 60;

uint8_t auxRelayTwoOnTimeHour = 23;
uint8_t auxRelayTwoOnTimeMin = 0;
uint8_t auxRelayTwoOffTimeHour = 22;
uint8_t auxRelayTwoOffTimeMin = 30;
uint8_t auxRelayTwoState = device::AUTO_TIMER;
uint8_t auxRelayTwoMode = 0; // 0 cyclic, 1 timer
int16_t auxRelayTwoOnDuration = 60;
int16_t auxRelayTwoOffDuration = 60;

uint8_t targetMinFanOneSpeed = 25;
uint8_t targetMaxFanOneSpeed = 100;
uint8_t targetMinFanTwoSpeed = 50;
uint8_t targetMaxFanTwoSpeed = 100;
float targetMinAirTemp = 20;
float targetMaxAirTemp = 27.5;
float targetMinAirTempF = 68.0;
float targetMaxAirTempF = 81.5;
float targetMinHumidity = 40.1;
float targetMaxHumidity = 60.2;
bool fansControlTemperature = true;
bool fansControlHumidity = false;
bool fanOneFixedSpeed = false;
bool fanTwoFixedSpeed = false;
uint8_t numberOfDosers = 6;
uint8_t doserOneMode = device::DOSER_EC;
uint8_t doserTwoMode = device::DOSER_EC;
uint8_t doserThreeMode = device::DOSER_PH_DOWN;
uint8_t doserFourMode = device::DOSER_PH_UP;
uint8_t doserFiveMode = device::DOSER_EC_OP;
uint8_t doserSixMode = device::DOSER_EC_OP;
uint16_t doserOneMills = 10;
uint16_t doserTwoMills = 10;
uint16_t doserThreeMills = 10;
uint16_t doserFourMills = 10;
uint16_t doserFiveMills = 10;
uint16_t doserSixMills = 10;
uint16_t targetMinTds = 400;
uint16_t targetMaxTds = 600;
uint16_t targetCo2 = 400;
uint16_t co2Offset = 99;
uint16_t roomWidthCm = 120;
uint16_t roomLengthCm = 120;
uint16_t roomHeightCm = 200;
float co2FlowrateLtrs = 6;
uint16_t roomWidthInches = 120;
uint16_t roomLengthInches = 120;
uint16_t roomHeightInches = 200;
float co2FlowrateFeet3 = 6;
uint8_t co2CheckTimeMinute = 0;
uint8_t co2CheckTimeHour = 22;
uint16_t disableFansTimer = 30;
bool enableManualCo2Duration = false;
uint16_t manualCo2GasDuration = 30;
bool disableCo2Control = true;
float targetMinPh = 6.0;
float targetMaxPh = 6.5;
float targetMinEc = 1.50;
float targetMaxEc = 1.70;
float targetMinWaterTemp = 20.0;
float targetMaxWaterTemp = 27.0;
float targetMinWaterTempF = 73.4;
float targetMaxWaterTempF = 77.0;
uint16_t targetMinWaterHeight = 4;
uint16_t targetMaxWaterHeight = 12;
uint16_t targetMinWaterHeightInches = 10;
uint16_t targetMaxWaterHeightInches = 32;
uint16_t waterTankLength = 50; 
uint16_t waterTankWidth = 50; 
uint16_t waterTankLengthInches = 20;
uint16_t waterTankWidthInches = 20;
uint8_t doserOneSpeed = 255;
uint8_t doserTwoSpeed = 255;
uint8_t doserThreeSpeed = 255;
uint8_t doserFourSpeed = 255;
uint8_t doserFiveSpeed = 255;
uint8_t doserSixSpeed = 255;
uint8_t dosingInterval = 6;
uint16_t refillDoserOneMills = 10;
uint16_t refillDoserTwoMills = 10;
uint16_t refillDoserThreeMills = 10;
uint16_t refillDoserFourMills = 10;
uint16_t refillDoserFiveMills = 10;
uint16_t refillDoserSixMills = 10;
uint32_t autoFillDays = 0;
uint8_t autoFillHour = 12;
uint8_t autoFillMinute = 0;
uint8_t drainTimeout = 1;
uint16_t backgroundColor = display::RA8875_LIGHT_BLUE;
uint16_t graphInterval = 2;
uint16_t afkTime = 1;
uint8_t brightness = 80;
bool convertToF  = false;
bool convertToInches = false;
bool convertToTds = false;
uint16_t tdsErrorMargin = 50;
float ecErrorMargin = 0.5;
float phErrorMargin = 0.5;
uint16_t co2ErrorMargin = 100;
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
bool disableDrainAndRefill = false; ///////////////
bool clearWifiDetails = false;
bool resetSettings = false;
uint8_t rtcDayOfWeek = 0;
uint8_t rtcSecond = 0;
uint8_t rtcMinute = 0;
uint8_t rtcHour = 0;
uint8_t rtcDay = 0;
uint8_t rtcMonth = 0;
uint8_t rtcYear = 0;
enum heightSensors {
  SR04,
  ETAPE,
  VL53L0X
};
heightSensors heightSensor = ETAPE;
enum dosingModes {
  PRECISE,
  INCREMENTAL
};
dosingModes ecDosingMode = PRECISE;
dosingModes phDosingMode = PRECISE;
}

namespace sensor {
float ec = 0;
float ph = 0;
uint16_t tds = 0;
uint16_t co2 = 0;
uint16_t ldr = 0;
float waterTemp = 0;
float airTemp = 0;
float humidity = 0;
float waterLevel = 0;
float waterVolumeLtrs = 0;
float emptyWaterTankDepth = 0;
float tdsKvalueLow = 1.21;
float tdsKvalueHigh = 1.05;
float phDownSolution = 4.0;
float phUpSolution = 11.0;
float ecSolution = 1.5;
uint16_t tdsSolution = 960;
const float phCalSolutionPart1 = 7.0;
const float phCalSolutionPart2 = 4.0;
const float tdsCalSolutionPart1 = 1413; // uS
const float tdsCalSolutionPart2 = 2760; // uS
float phNeutralVoltage = 1605.47;
float phAcidicVoltage =  2001.46;
uint32_t co2GasTime = 0;
float temp_etapeZeroVolumeResistance = 0;
float etapeZeroVolumeResistance = 1877.79; // 2104.56  resistance value (in ohms) when no liquid is present.
float etapeMaxVolumeResistance = 271.4; // 288.71 resistance value (in ohms) when liquid is at max line.
const uint8_t etapeCalibrationCm = 31; // maximum mesurement in centimeters on etap 12" version
const float etapeOffset = 2.5; // Lowest value read on etape in Centimeters, datasheet says 2.5 but i get 2.5
const uint8_t co2Request[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
uint8_t sensorArrayPos = 0;
const uint8_t maxSensorArrayVals = 24;
uint16_t phArray[maxSensorArrayVals]; // compressed floats * 100
uint16_t co2Array[maxSensorArrayVals];
uint16_t ecArray[maxSensorArrayVals]; // compressed floats * 100
uint16_t tdsArray[maxSensorArrayVals];
uint16_t waterTemperatureArray[maxSensorArrayVals]; // compressed floats * 100
uint16_t waterTemperatureArrayF[maxSensorArrayVals]; // compressed floats * 100
uint16_t waterLevelArray[maxSensorArrayVals]; // compressed floats * 100
uint16_t waterLevelArrayInInches[maxSensorArrayVals]; // compressed floats * 100
uint16_t airTemperatureArray[maxSensorArrayVals]; // compressed floats * 100
uint16_t airTemperatureArrayF[maxSensorArrayVals]; // compressed floats * 100
uint16_t humidityArray[maxSensorArrayVals]; // compressed floats * 100
uint16_t fanOneSpeedArray[maxSensorArrayVals];
uint16_t fanTwoSpeedArray[maxSensorArrayVals];
uint8_t fanArrayPos = 0;
const uint8_t maxFanArrayVals = 60;
uint16_t fanTemperatureArray[maxFanArrayVals]; // compressed floats * 100
uint16_t fanHumidityArray[maxFanArrayVals]; // compressed floats * 100
enum airStates{ IS_FALLING, IS_RISING, IS_SAME};
}

namespace wifi {
bool wifiEnabled = false;
uint32_t connectionTime = 0;
bool remotlyLoadUserProfile = false;
bool remotlySaveUserProfile = false;
char ssid[16] = "HydroController";
char password[16] = "Password1!";
bool hiddenNetwork = false;
}

// Class instances
RA8875 tft = RA8875(pin::ra8875_cs, pin::ra8875_rst);
OneWire oneWire(pin::oneWireBus);
DallasTemperature dallasTemperature(&oneWire);
uRTCLib rtc(0x68); // The RTC uses hardware I2C (SCL/SDA) //DS3231 rtc(SDA, SCL);
#ifdef USING_HDC1080
Adafruit_HDC1000 hdc = Adafruit_HDC1000();
#else
dht DHT(pin::dht22);
#endif
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
DFRobot_PH ph;
GravityTDS gravityTds;

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
  pinMode(pin::auxRelayOne, OUTPUT);
  pinMode(pin::auxRelayTwo, OUTPUT);
  pinMode(pin::tdsTransistor, OUTPUT);
  pinMode(pin::phTransistor, OUTPUT);
  pinMode(pin::espTransistor, OUTPUT);
  //
  digitalWrite(pin::light, device::relayOffState);
  digitalWrite(pin::waterHeater, device::relayOffState);
  digitalWrite(pin::inletPump, device::relayOffState);
  digitalWrite(pin::outletPump, device::relayOffState);
  digitalWrite(pin::airHeater, device::relayOffState);
  digitalWrite(pin::co2Solenoid, device::relayOffState);
  digitalWrite(pin::auxRelayOne, device::relayOffState);
  digitalWrite(pin::auxRelayTwo, device::relayOffState);
  //
  initializeDevice();
}

void loop() {
  touchEvent();
  readSensors();
  drawPages();
  envriomentalControl();
  displayWarnings();
  esp8266DataHandler();
}