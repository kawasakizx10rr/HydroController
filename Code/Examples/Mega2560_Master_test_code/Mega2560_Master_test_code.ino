#include <SPI.h>
#include <RA8875.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DS3231.h>
#include <dht.h>

namespace pin {
// analog sensors
const byte ldrSensor = A0;
const byte tdsSensor = A1;
const byte eTapeSensor = A2;
const byte phSensor = A3;
const byte moistureSensorOne = A5;
const byte moistureSensorTwo = A7;
const byte moistureSensorThree = A9;
const byte moistureSensorFour = A11;
const byte moistureSensorFive = A13;
const byte moistureSensorSix = A15;
// flow rate sensor
const byte flowRateInt = 3;
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
const byte oneWiireBus = 38;
const byte dht22 = 39;
// ultrasonic sensor
const byte hcsrTrig = 40;
const byte hcsrEcho = 41;
// relays
const byte airHeater = 22;
const byte co2Solenoid = 23;
const byte inletPump = 24;
const byte outletPump = 25;
const byte light = 26;
const byte waterHeater = 27;
const byte unusedRelaySeven = 28;
const byte unusedRelayEight = 29;
// transistors
const byte phTransistor = 36;
const byte tdsTransistor = 37;
const byte espTransistor = 42;
}

enum ecPpmConversionTypes {EU, US, AU}; // EC 0.64, US 0.50, AU 0.7 https://www.thebudlab.ca/wp-content/uploads/wpforo/attachments/14/100-EC-to-PPM.png
byte ecPpmConversionType = EU; // CHANGE TO YOUR PREFERED CONVERSION METHOD !
float ecCalibration = 1;
int ecCalibrationSolution = 707;
int phBufferArray[10] {};
int phBuffer = 0;
const float aref = 5.0;
const float phOffset = 0;
const byte slaveAddress = 9;
const byte co2Request[] {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
uint16_t tx, ty;
bool startDisplay = false;
uint16_t choosenColor = RA8875_WHITE;
const bool relayOffState = HIGH;
bool espIsTurnedOn = false;
// Calibration values measured from the Etape LiquidSensor.
float etapeZeroVolumeResistance = 1877.79; // 2104.56  resistance value (in ohms) when no liquid is present.
float etapeCalibrationResistance = 271.4; // 288.71 resistance value (in ohms) when liquid is at max line.
const float etapeCalibrationCm = 31.00; // maximum mesurement in centimeters on etap 12" version
const float etapeOffset = 2.5; // Lowest value read on etape in Centimeters, datasheet says 2.5 but i get 2.5
float phVoltagePerUnit = 0.1839;
float ph4Voltage = 3.05;
float ph7Voltage = 2.5;
float ph4BufferValue = 4.01;
float ph7BufferValue = 7.0;
// Doser speeds
byte doserOneSpeed = 100;
byte doserTwoSpeed = 100;
byte doserThreeSpeed = 100;
byte doserFourSpeed = 100;
byte doserFiveSpeed = 100;
byte doserSixSpeed = 100;

bool using_dfRobot_PhSensor = false; // IMPORTANT: CHANGE THIS VALUE IF YOU ARE USING A DF ROBOT PH SENSOR

bool using_pc_interface_tool = true; // IMPORTANT: CHANGE THIS VALUE IF YOU ARE NOT USING THE PC INTERFACE TEST TOOL TO CONNECT THE THE PCB


DS3231 rtc(SDA, SCL);
RA8875 tft = RA8875(pin::ra8875_cs, pin::ra8875_rst);
OneWire oneWire(pin::oneWiireBus);
DallasTemperature dallasTemperature(&oneWire);
Time t;
dht DHT;

void setup() {
  // Start instances
  Serial.begin(115200); // debug
  Serial1.begin(115200); // esp
  Serial2.begin(9600); // co2  
  if (!using_pc_interface_tool)
    Serial.print(F("Starting PCB master test code V3.3..."));
  Wire.begin();
  SPI.begin();
  dallasTemperature.begin();
  rtc.begin();
  //tft.begin(Adafruit_800x480); // Adafruit RA8875 displays
  tft.begin(RA8875_800x480); // BuyDisplay RA8875 displays
  tft.useINT(pin::ra8875_int);
  tft.touchBegin();
  tft.enableISR(true);
  // set outputs
  pinMode(pin::flowRateInt, INPUT_PULLUP);
  pinMode(pin::hcsrEcho, INPUT);
  pinMode(pin::hcsrTrig, OUTPUT);
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
  pinMode(pin::unusedRelaySeven, OUTPUT);
  pinMode(pin::unusedRelayEight, OUTPUT);
  pinMode(pin::tdsTransistor, OUTPUT);
  pinMode(pin::phTransistor, OUTPUT);
  pinMode(pin::espTransistor, OUTPUT);
  // set pin states where required
  digitalWrite(pin::light, relayOffState);
  digitalWrite(pin::waterHeater, relayOffState);
  digitalWrite(pin::inletPump, relayOffState);
  digitalWrite(pin::outletPump, relayOffState);
  digitalWrite(pin::airHeater, relayOffState);
  digitalWrite(pin::co2Solenoid, relayOffState);
  digitalWrite(pin::unusedRelaySeven, relayOffState);
  digitalWrite(pin::unusedRelayEight, relayOffState);
  if (!using_pc_interface_tool)
    Serial.println(F("Loading complete.\n"));
  delay(2000);
  if (!using_pc_interface_tool)
    showSerialCommands();
    digitalWrite(pin::phTransistor, LOW);
    digitalWrite(pin::tdsTransistor, LOW);
  digitalWrite(pin::waterHeater, relayOffState);
}

void loop() {
  serialRead();
  drawGfx();
}
