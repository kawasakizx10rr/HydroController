
/*
  The logic for the data communication is as follows

  When a web page is loaded its calls the JS function getData, which uses an XMLHttpRequest to get the required data, the host sends request to the native callback getPageData running on the local server (the ESP8266) with the page number and slide number,
  the getPageData calls the native function receiveSerialData which sends the page number and slide number to the Mega2560 via serial, the mega2560 then calls its local function getValues with the page number and slide number, which then sends the required variables 
  back to the ESP8266 via Serial. The ESP8266 then updates its local copy of the variables and then builds a csv string with the newly updated variables and sends it back to the wepgages JS getData function.

  When the user changes a values in the webpage a similar process occours, a JS function named setVal is called with the name of the varible and the value, the varible and value is then sent to the ESP8266 via a XMLHttpRequest to the native callback setValue,
  running on the local server (the ESP8266). The variable name is then converted to an interger index and the index and value is sent to the Mega2560 via Serial which then calls setValues and updates the given variable using a switch statment using the index.
*/

#include <ESP8266WebServer.h>
#include "index.h"

namespace device {
bool remotlyLoadUserProfile = false;
bool remotlySaveUserProfile = false;
bool globalDebug = false; 
bool lightOn = false;
//uint8_t auxRelayOneSwitchedOnOffDay = 0;
//uint8_t auxRelayTwoSwitchedOnOffDay = 0;
uint8_t userProfile = 0;
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
uint8_t fanOneSpeed = 0;
uint8_t fanTwoSpeed = 0;
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
enum systemStates {
  NO_ADJUSTMENT,
  ADJUSTING_TDS,
  ADJUSTING_EC,
  ADJUSTING_PH,
  ADJUSTING_WATER_HEIGHT,
  ADJUSTING_CO2, 
};
uint8_t systemStatus = NO_ADJUSTMENT;
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

uint8_t lightOnTimeHour = 0;
uint8_t lightOnTimeMin = 0;
uint8_t lightOffTimeHour = 0;
uint8_t lightOffTimeMin = 0;
uint8_t lightState = device::AUTO_TIMER;
uint8_t lightMode = 1; // 0 cyclic, 1 timer
int16_t lightOffDuration = 60;
int16_t lightOnDuration = 60;

uint8_t auxRelayOneOnTimeHour = 0;
uint8_t auxRelayOneOnTimeMin = 0;
uint8_t auxRelayOneOffTimeHour = 0;
uint8_t auxRelayOneOffTimeMin = 0;
uint8_t auxRelayOneState = device::AUTO_TIMER;
uint8_t auxRelayOneMode = 0; // 0 cyclic, 1 timer
int16_t auxRelayOneOnDuration = 0;
int16_t auxRelayOneOffDuration = 0;

uint8_t auxRelayTwoOnTimeHour = 0;
uint8_t auxRelayTwoOnTimeMin = 0;
uint8_t auxRelayTwoOffTimeHour = 0;
uint8_t auxRelayTwoOffTimeMin = 0;
uint8_t auxRelayTwoState = device::AUTO_TIMER;
uint8_t auxRelayTwoMode = 0; // 0 cyclic, 1 timer
int16_t auxRelayTwoOnDuration = 0;
int16_t auxRelayTwoOffDuration = 0;

uint8_t targetMinFanOneSpeed = 0;
uint8_t targetMaxFanOneSpeed = 0;
uint8_t targetMinFanTwoSpeed = 0;
uint8_t targetMaxFanTwoSpeed = 0;
float targetMinAirTemp = 0;
float targetMaxAirTemp = 0;
float targetMinHumidity = 0;
float targetMaxHumidity = 0;
bool fansControlTemperature = false;
bool fansControlHumidity = false;
bool fanOneFixedSpeed = false;
bool fanTwoFixedSpeed = false;
uint8_t numberOfDosers = 4;
uint8_t doserOneMode = device::DOSER_EC;
uint8_t doserTwoMode = device::DOSER_EC;
uint8_t doserThreeMode = device::DOSER_PH_DOWN;
uint8_t doserFourMode = device::DOSER_PH_UP;
uint8_t doserFiveMode = device::DOSER_EC_OP;
uint8_t doserSixMode = device::DOSER_EC_OP;
uint16_t doserOneMills = 0;
uint16_t doserTwoMills = 0;
uint16_t doserThreeMills = 0;
uint16_t doserFourMills = 0;
uint16_t doserFiveMills = 0;
uint16_t doserSixMills = 0;
uint16_t targetMinTds = 0;
uint16_t targetMaxTds = 0;
uint16_t targetCo2 = 0;
uint16_t co2Offset = 0;
uint16_t roomWidth = 0;
uint16_t roomLength = 0;
uint16_t roomHeight = 0;
float co2Flowrate = 0;
uint8_t co2CheckTimeMinute = 0;
uint8_t co2CheckTimeHour = 0;
uint16_t disableFansTimer = 0;
bool enableManualCo2Duration = false;
uint16_t manualCo2GasDuration = 0;
bool disableCo2Control = false;
float targetMinPh = 0;
float targetMaxPh = 0;
float targetMinEc = 0;
float targetMaxEc = 0;
float targetMinWaterTemp = 0;
float targetMaxWaterTemp = 0;
float targetMinWaterHeight = 0;
float targetMaxWaterHeight = 0;
float waterTankLength = 0; 
float waterTankWidth = 0; 
uint8_t doserOneSpeed = 0;
uint8_t doserTwoSpeed = 0;
uint8_t doserThreeSpeed = 0;
uint8_t doserFourSpeed = 0;
uint8_t doserFiveSpeed = 0;
uint8_t doserSixSpeed = 0;
uint8_t dosingInterval = 0;
uint16_t refillDoserOneMills = 0;
uint16_t refillDoserTwoMills = 0;
uint16_t refillDoserThreeMills = 0;
uint16_t refillDoserFourMills = 0;
uint16_t refillDoserFiveMills = 0;
uint16_t refillDoserSixMills = 0;
bool autoFillDays[31];
uint8_t autoFillHour = 0;
uint8_t autoFillMinute = 0;
uint8_t drainTimeout = 0;
uint16_t graphInterval = 0;
uint16_t afkTime = 0;
uint8_t brightness = 0;
bool convertToF  = false;
bool convertToInches = false;
bool convertToTds = false;
uint16_t tdsErrorMargin = 0;
float ecErrorMargin = 0;
float phErrorMargin = 0;
uint16_t co2ErrorMargin = 0;
float waterTempErrorMargin = 0;
float waterHeightErrorMargin = 0;
float airTempErrorMargin = 0;
float humidityErrorMargin = 0;
bool disablePpmWarnings = false;
bool disableEcWarnings = false;
bool disableCo2Warnings = false;
bool disablePhWarnings = false;
bool disableWaterTempWarnings = false;
bool disableWaterHeightWarnings = false;
bool disableAirTempWarnings = false;
bool disableHumidityWarnings = false;
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
float waterTemp = 0;
float airTemp = 0;
float humidity = 0;
float waterLevel = 0;
uint8_t sensorArrayPos = 0;
const uint8_t maxSensorArrayVals = 24;
float phArray[maxSensorArrayVals];
float co2Array[maxSensorArrayVals];
float ecArray[maxSensorArrayVals];
float tdsArray[maxSensorArrayVals];
float waterTemperatureArray[maxSensorArrayVals];
float waterLevelArray[maxSensorArrayVals];
float airTemperatureArray[maxSensorArrayVals];
float humidityArray[maxSensorArrayVals];
float fanOneSpeedArray[maxSensorArrayVals];
float fanTwoSpeedArray[maxSensorArrayVals];
}

namespace wifi {
bool wifiEnabled = false;
unsigned long connectionTime = 0;
unsigned long wifiPreviousMillis = 0;
char ssid[16] = "HydroController";
char password[16] = "Password1!";
bool hiddenNetwork = false;
char buffer[2048] {};
}


ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.flush();
  // HTML call back functions from user interactions
  server.on("/", handleRoot);
  server.on("/getPageData", getPageData);
  server.on("/setValue", setValue);
  if (device::globalDebug)
    Serial.println(F("Loading web server..."));
  // Connect to Wi-Fi network with SSID and password
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(wifi::ssid, wifi::password, wifi::hiddenNetwork);
  if (device::globalDebug) {
    Serial.print(F("Started AP on IP address: ")); Serial.println(WiFi.softAPIP());
    Serial.print(F("AP SSID: ")); Serial.println(wifi::ssid);
    Serial.print(F("AP password: ")); Serial.println(wifi::password);
  }
  // Start the Wifi portal
  server.begin();  //Start web server
  delay(2000);
}

void loop() {
  yield();
  server.handleClient();
}
