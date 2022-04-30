namespace device {
bool sensorsReady = true;
// bool currentlyDraining;
bool serialDebug = true;
byte graphArrayPos = 12;
const byte maxGraphArrayValues = 12;
float phArray[maxGraphArrayValues] {6.42, 6.49, 6.51, 5.54, 6.50, 5.36, 4.73, 4.84, 4.87, 4.92, 6.51, 5.54};
float co2Array[maxGraphArrayValues] {3000, 4000, 5000, 4000, 3500, 3000, 2500, 2000, 1000, 2000, 3000, 4000};
float ecArray[maxGraphArrayValues] {0.51, 1.02, 1.53, 2.04, 2.05, 6.06, 7.07, 8.06, 9.07, 1.01, 9.02, 8.03};
float tdsArray[maxGraphArrayValues] {50, 125, 150, 200, 300, 500, 900, 800, 700, 900, 950, 800};
float waterTemperatureArray[maxGraphArrayValues]  {21.62, 21.45, 20.88, 20.24, 20.53, 24.17, 25.51, 27.28, 18.75, 16.57, 14.89, 12.23};
float waterTemperatureArrayF[maxGraphArrayValues] {8.51, 8.44, 8.22, 7.96, 8.08, 9.51, 10.05, 11.23, 10.93, 10.73, 9.82, 9.01};
float waterLevelArray[maxGraphArrayValues] {24.2, 27.5, 23.7, 23.3, 24.3, 22.8, 26.9, 22.4, 29.6, 22.1, 27.3, 21.8};
float waterLevelArrayInInches[maxGraphArrayValues] {10.7, 11.14, 10.2, 9.6, 9.8, 9.3, 8.6, 8.4, 7.7, 7.3, 6.5, 6.3};
float airTemperatureArray[maxGraphArrayValues] {21.45, 20.88, 20.24, 20.53, 24.17, 25.51, 27.28, 18.75, 16.57, 14.89, 12.23, 13.42};
float airTemperatureArrayF[maxGraphArrayValues] {47.6, 49.4, 52.6, 53.7, 53.1, 54.6, 55.7, 55.2, 57.6, 58.1, 57.9, 58.4};
float humidityArray[maxGraphArrayValues] {41.3, 43.5, 40.6, 45.2, 50.7, 54.1, 45.8, 47.9, 48.4, 46.6, 44.2, 42.6};
float fanOneSpeedArray[maxGraphArrayValues] {70, 65, 61, 57, 53, 40, 25, 27, 32, 37, 44, 49};
float fanTwoSpeedArray[maxGraphArrayValues] {40, 35, 31, 27, 23, 30, 35, 37, 42, 47, 44, 49};
byte profileInputNumber = 3;
byte userProfile = 3;
float minPh = 6.23;
float maxPh = 8.24;
int minCo2 = 516;
int maxCo2 = 2078;
float minEc = 0.94;
float maxEc = 2.61;
float minWaterTemp = 17.3;
float maxWaterTemp = 27.6;
float minWaterLevel = 5;
float maxWaterLevel = 32;
int minTds = 602;
int maxTds = 1183;
float minAirTemp = 12.5;
float maxAirTemp = 39.4;
float minHumidity = 35;
float maxHumidity = 99;
byte minFanOneSpeed = 25;
byte maxFanOneSpeed = 100;
byte minFanTwoSpeed = 25;
byte maxFanTwoSpeed = 100;
byte fanOneSpeed = 75;
byte fanTwoSpeed = 98;
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
bool wifiValsAdjusted = false;
bool warningValsAdjusted = false;
enum doserModes {
  DOSER_OFF,
  DOSER_EC,
  DOSER_PH
};
}

namespace user {
char profileOneName[11]   = "Profile_1";
char profileTwoName[11]   = "Profile_2";
char profileThreeName[11] = "Profile_3";
char profileFourName[11]  = "Profile_4";
char profileFiveName[11]  = "Profile_5";
byte lightOnTimeHour = 23;
byte lightOnTimeMin = 15;
byte lightOffTimeHour = 22;
byte lightOffTimeMin = 30;
byte lightMode = 1;
byte targetMinFanOneSpeed = 25;
byte targetMaxFanOneSpeed = 100;
byte targetMinFanTwoSpeed = 50;
byte targetMaxFanTwoSpeed = 100;
float targetMinAirTemp = 20;
float targetMaxAirTemp = 27.5;
float airTempOffset = 2.0;
float targetMinAirTempF = 68.0; // NEW IN F
float targetMaxAirTempF = 81.5; // NEW IN F
float airTempOffsetF = 1.0; // NEW IN F
float targetMinHumidity = 40;
float targetMaxHumidity = 60;
float humidityOffset = 2.0;
bool fansControlTemperature = true;
bool fansControlHumidity = false;
bool fanOneFixedSpeed = false;
bool fanTwoFixedSpeed = false;
byte numberOfDosers = 6;
byte doserOneMode = device::DOSER_PH;   // OFF, EC, PH +
byte doserTwoMode = device::DOSER_EC;   // OFF, EC, PH +
byte doserThreeMode = device::DOSER_PH; // OFF, EC, PH +
byte doserFourMode = device::DOSER_EC;  // OFF, EC, PH -
byte doserFiveMode = device::DOSER_PH;  // OFF, EC, PH +
byte doserSixMode = device::DOSER_EC;   // OFF, EC, PH -
int doserOneMills = 10;
int doserTwoMills = 10;
int doserThreeMills = 10;
int doserFourMills = 10;
int doserFiveMills = 10;
int doserSixMills = 10;
int targetMinTds = 400;
int targetMaxTds = 600;
int tdsOffset = 20;
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
bool manualCo2Duration = false;
int manualCo2GasDuration = 30;
bool disableCo2Control = true;
float targetMinPh = 6.0;
float targetMaxPh = 6.5;
float phOffset = 0.5;
float targetMinEc = 1.60;
float targetMaxEc = 1.70;
float ecOffset = 0.10;
float targetMinWaterTemp = 20.0;
float targetMaxWaterTemp = 27.0;
float waterTempOffset = 1.0;
float targetMinWaterTempF = 73.4; // NEW IN F
float targetMaxWaterTempF = 77.0; // NEW IN F
float waterTempOffsetF = 1.0; // NEW IN F
float targetMinWaterHeight = 10;
float targetMaxWaterHeight = 32;
float waterHeightOffset = 2;
float targetMinWaterHeightInches = 10.1;
float targetMaxWaterHeightInches = 32.2;
float waterHeightOffsetInches = 2.3;
bool wifiEnabled = true;
bool resetWifi = false;
int wifiSendInterval = 5;
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
bool autoFillDays[31] {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
byte autoFillHour = 12;
byte autoFillMinute = 30;
byte drainTimeout = 1;
unsigned long graphInterval = 1;
unsigned long afkTime = 1; // in minutes
bool setRTCtime = false;
bool convertToFarenheit  = false;
bool convertToInches = false;
bool convertToTds = false;
int tdsErrorMargin = 50;
float ecErrorMargin = 0.5;
float phErrorMargin = 0.5;
float co2ErrorMargin = 100;
float waterTempErrorMargin = 1.0;
float waterTempErrorMarginF = 1.0; // NEW IN F
float waterHeightErrorMargin = 5;
float waterHeightErrorMarginInches = 1;
float airTempErrorMargin = 3.0;
float airTempErrorMarginF = 2.0; // NEW IN F
byte humidityErrorMargin = 5;
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
}

namespace sensor {
float ec = 1.65;
float ph = 6.75;
int tds = 628;
int co2 = 450;
int ldr = 654;
float waterTemp = 25.3;
float waterTempF = 49.3;
float airTemp = 27.4;
float humidity = 44;
float waterLevel = 30;
float waterLevelInches = 12;
}

namespace wifi {
unsigned long wifiSendInterval = 1;
bool resetWifi = false;
bool clearWifiDetails = false;
unsigned long connectionTime = 0;
unsigned long wifiPreviousMillis = 0;
enum connectionStatus {
  UNCONNECTED,
  CONNECTED,
  PORTAL,
  FAILED
};
byte connectionState = UNCONNECTED;
}

void setup() {
  Serial.begin(115200); // debugging
  Serial1.begin(115200); // connection esp
  Serial.println(F("Mega2560 Serial master started"));
}

void loop() {
  espSerial();
}
