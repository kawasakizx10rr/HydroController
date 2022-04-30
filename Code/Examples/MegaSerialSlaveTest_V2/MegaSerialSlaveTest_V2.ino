bool debugEnabled = true;
char serialData[1400] {};

byte userProfile = 0;
int loadUserProfile = -1;
int saveUserProfile = -1;
byte graphArrayPos = 0;
const byte maxGraphArrayValues = 12;
float phArray[maxGraphArrayValues] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float co2Array[maxGraphArrayValues] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float ecArray[maxGraphArrayValues] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float tdsArray[maxGraphArrayValues] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float waterTempArray[maxGraphArrayValues] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float waterLevelArray[maxGraphArrayValues] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float airTempArray[maxGraphArrayValues] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float humidityArray[maxGraphArrayValues] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float fanOneSpeedArray[maxGraphArrayValues] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float fanTwoSpeedArray[maxGraphArrayValues] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// max mins
float minPh = 0;
float maxPh = 0;
int minCo2 = 0;
int maxCo2 = 0;
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
bool co2DisabledFans = false;
enum doserModes {
  DOSER_OFF,
  DOSER_EC,
  DOSER_PH
};

char profileOneName[11]   = "-";
char profileTwoName[11]   = "-";
char profileThreeName[11] = "-";
char profileFourName[11]  = "-";
char profileFiveName[11]  = "-";
byte lightOnTimeHour = 0;
byte lightOnTimeMin = 0;
byte lightOffTimeHour = 0;
byte lightOffTimeMin = 0;
byte lightMode = 0;
byte targetMinFanOneSpeed = 0;
byte targetMaxFanOneSpeed = 0;
byte targetMinFanTwoSpeed = 0;
byte targetMaxFanTwoSpeed = 0;
float targetMinAirTemp = 0;
float targetMaxAirTemp = 0;
float airTempOffset = 0;
float targetMinHumidity = 0;
float targetMaxHumidity = 0;
float humidityOffset = 0;
bool fansControlTemperature = false;
bool fansControlHumidity = false;
bool fanOneFixedSpeed = false;
bool fanTwoFixedSpeed = false;
byte numberOfDosers = 6;
byte doserOneMode = DOSER_EC;   // OFF, EC, PH +
byte doserTwoMode = DOSER_EC;   // OFF, EC, PH +
byte doserThreeMode = DOSER_EC; // OFF, EC, PH +
byte doserFourMode = DOSER_EC;  // OFF, EC, PH -
byte doserFiveMode = DOSER_PH;  // OFF, EC, PH +
byte doserSixMode = DOSER_EC;   // OFF, EC, PH -
int doserOneMills = 0;
int doserTwoMills = 0;
int doserThreeMills = 0;
int doserFourMills = 0;
int doserFiveMills = 0;
int doserSixMills = 0;
int targetMinTds = 0;
int targetMaxTds = 0;
int tdsOffset = 0;
int targetCo2 = 0;
int co2Offset = 0;
int roomWidth = 50;
int roomLength = 60;
int roomHeight = 70;
float co2Flowrate = 1.23;
byte co2CheckTimeMinute = 0;
byte co2CheckTimeHour = 0;
int disableFansTimer = 0;
bool manualCo2Duration = false;
int manualCo2GasDuration = 0;
bool disableCo2Control = false;
float targetMinPh = 6.51;
float targetMaxPh = 0;
float phOffset = 0;
float targetMinEc = 1.65;
float targetMaxEc = 0;
float ecOffset = 0;
float targetMinWaterTemp = 0;
float targetMaxWaterTemp = 0;
float waterTempOffset = 0;
float targetMinWaterHeight = 0;
float targetMaxWaterHeight = 0;
float waterHeightOffset = 0;
bool wifiEnabled = false;
bool resetWifi = false;
int wifiSendInterval = 0;
byte doserOneSpeed = 0;
byte doserTwoSpeed = 0;
byte doserThreeSpeed = 0;
byte doserFourSpeed = 0;
byte doserFiveSpeed = 0;
byte doserSixSpeed = 0;
byte dosingInterval = 0;
byte swapInterval = 0;
int refillDoserOneMills = 0;
int refillDoserTwoMills = 0;
int refillDoserThreeMills = 0;
int refillDoserFourMills = 0;
int refillDoserFiveMills = 0;
int refillDoserSixMills = 0;
bool autoFillDays[31] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte autoFillHour = 0;
byte autoFillMinute = 0;
byte drainTimeout = 0;
unsigned long afkTime = 0; // in minutes
bool convertToFarenheit  = false;
bool convertToInchs = false;
bool convertToTds = false;
int tdsErrorMargin = 0;
float ecErrorMargin = 0;
float phErrorMargin = 9.87;
float co2ErrorMargin = 0;
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
byte ppmErrorState = 0;
byte ecErrorState = 0;
byte phErrorState = 0;
byte co2ErrorState = 0;
byte waterTempErrorState = 0;
byte waterLevelErrorState = 0;
byte airTempErrorState = 0;
byte humidityErrorState = 0;
bool disableLED = false;
bool disableBeeper = false;
bool disableDrainAndRefill = false;
bool disableMachineLearning = false;
bool clearWifiDetails = false;

float ec = 0;
float ph = 0;
int tds = 0;
int co2 = 0;
int ldr = 0;
float waterTemp = 0;
float airTemp = 0;
float humidity = 0;
float waterLevel = 0;


void setup() {
  Serial.begin(115200); // debugging
  Serial1.begin(115200); // Connection to Mega2560
  Serial.println(F("Mega2560 Serial slave started"));
}

void loop() {
  handleSerial1Data();
}
