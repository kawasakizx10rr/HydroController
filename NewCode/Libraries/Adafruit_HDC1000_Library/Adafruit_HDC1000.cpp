/*!
 * @file Adafruit_HDC1000.cpp
 *
 * @mainpage Adafruit HDC1000 Library
 *
 * @section intro_sec Introduction
 *
 * This is a library for the HDC1000 Humidity & Temp Sensor
 *
 * Designed specifically to work with the HDC1000 sensor from Adafruit
 * ----> https://www.adafruit.com/products/2635
 *
 * These sensors use I2C to communicate, 2 pins are required to
 * interface
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @section author Author
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 *
 * @section license License
 *
 * BSD license, all text above must be included in any redistribution
 */

#include "Adafruit_HDC1000.h"

Adafruit_HDC1000::Adafruit_HDC1000() {}

/*!
 * @brief Starts I2C connection
 * @param addr I2C address of the HDC1000
 * @param wire The TwoWire master, defaults to &Wire
 * @return Returns true if successful
 */
bool Adafruit_HDC1000::begin(uint8_t addr, TwoWire *wire) {
  if (i2c_dev) {
    delete i2c_dev; // remove old interface
  }

  i2c_dev = new Adafruit_I2CDevice(addr, wire);

  if (!i2c_dev->begin()) {
    return false;
  }

  reset();
  if (read16(HDC1000_MANUFID) != 0x5449)
    return false;
  uint16_t device_id = read16(HDC1000_DEVICEID);
  if ((device_id != 0x1000) && (device_id != 0x1050))
    return false;

  return true;
}

/*!
 * @brief Soft resets the HDC1000 over I2C
 */
void Adafruit_HDC1000::reset(void) {
  // reset, and select 14 bit temp & humidity
  uint16_t config = HDC1000_CONFIG_RST | HDC1000_CONFIG_MODE |
                    HDC1000_CONFIG_TRES_14 | HDC1000_CONFIG_HRES_14;

  writeConfig(config);
}

/*!
 * @brief Gets the temperature over I2C from the HDC1000
 * @return Returns the temperature
 */
float Adafruit_HDC1000::readTemperature(void) {

  float temp = (read32(HDC1000_TEMP, 20) >> 16);
  temp /= 65536;
  temp *= 165;
  temp -= 40;

  return temp;
}

/*!
 * @brief Gets the humidity over I2C from the HDC1000
 * @return Returns the humidity
 */
float Adafruit_HDC1000::readHumidity(void) {

  float hum = (read32(HDC1000_TEMP, 20) & 0xFFFF);

  hum /= 65536;
  hum *= 100;

  return hum;
}

/*!
 * @brief Resets, heats up, selects temperature and humidity,
 * then takes 1000 readings and tosses
 */
void Adafruit_HDC1000::drySensor(void) {
  uint16_t origConfig = read16(0x2);

  // reset, heat up, and select 14 bit temp & humidity
  uint16_t newConfig = HDC1000_CONFIG_RST | HDC1000_CONFIG_HEAT |
                       HDC1000_CONFIG_MODE | HDC1000_CONFIG_TRES_14 |
                       HDC1000_CONFIG_HRES_14;

  writeConfig(newConfig);

  delay(15);

  // take 1000 readings & toss
  for (int i = 0; i < 1000; i++) {
    read32(HDC1000_TEMP, 20);
    delay(1);
  }

  origConfig |= HDC1000_CONFIG_RST;
  writeConfig(origConfig);
  delay(15);
}

/*!
 * @brief Writes config to HDC1000
 * @param config Configuration settings to be written
 */
void Adafruit_HDC1000::writeConfig(uint16_t config) {
  Adafruit_BusIO_Register reg16 =
      Adafruit_BusIO_Register(i2c_dev, 0x02, 2, MSBFIRST);
  reg16.write(config);
  delay(15);
}

/*********************************************************************/

/*!
 * @brief Reads 16 bits
 * @param addr I2C register address
 * @param delayms Delay after write and before read
 * @return Returns what was read
 */
uint16_t Adafruit_HDC1000::read16(uint8_t addr, uint8_t delayms) {
  uint8_t buff[2];
  uint16_t val = 0;

  buff[0] = addr;
  i2c_dev->write(buff, 1);
  delay(delayms);
  i2c_dev->read(buff, 2);
  val = buff[0];
  val <<= 8;
  val |= buff[1];

  return val;
}

/*!
 * @brief Reads 32 bits
 * @param addr I2C register address
 * @param delayms Delay after write and before read
 * @return Returns what was read
 */
uint32_t Adafruit_HDC1000::read32(uint8_t addr, uint8_t delayms) {
  uint8_t buff[4];
  uint32_t val = 0;

  buff[0] = addr;
  i2c_dev->write(buff, 1);
  delay(delayms);
  i2c_dev->read(buff, 4);
  val = buff[0];
  val <<= 8;
  val |= buff[1];
  val <<= 8;
  val |= buff[2];
  val <<= 8;
  val |= buff[3];

  return val;
}

/*!
 *  @brief  Converts Celcius to Fahrenheit
 *  @param  c
 *					value in Celcius
 *	@return float value in Fahrenheit
 */
float Adafruit_HDC1000::convertCtoF(float c) { return c * 1.8 + 32; }

/*!
 *  @brief  Converts Fahrenheit to Celcius
 *  @param  f
 *					value in Fahrenheit
 *	@return float value in Celcius
 */
float Adafruit_HDC1000::convertFtoC(float f) { return (f - 32) * 0.55555; }

/*!
 *  @brief  Compute Heat Index
 *  				Using both Rothfusz and Steadman's equations
 *					(http://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml)
 *  @param  temperature
 *          temperature in selected scale
 *  @param  percentHumidity
 *          humidity in percent
 *  @param  isFahrenheit
 * 					true if fahrenheit, false if celcius
 *	@return float heat index
 */
float Adafruit_HDC1000::computeHeatIndex(float temperature, float percentHumidity, bool isFahrenheit) {
  if (!isFahrenheit)
    temperature = convertCtoF(temperature);

  float hi = 0.5 * (temperature + 61.0 + ((temperature - 68.0) * 1.2) + (percentHumidity * 0.094));

  if (hi > 79) {
    hi = -42.379 + 2.04901523 * temperature + 10.14333127 * percentHumidity +
         -0.22475541 * temperature * percentHumidity +
         -0.00683783 * pow(temperature, 2) +
         -0.05481717 * pow(percentHumidity, 2) +
         0.00122874 * pow(temperature, 2) * percentHumidity +
         0.00085282 * temperature * pow(percentHumidity, 2) +
         -0.00000199 * pow(temperature, 2) * pow(percentHumidity, 2);

    if ((percentHumidity < 13) && (temperature >= 80.0) &&
        (temperature <= 112.0))
      hi -= ((13.0 - percentHumidity) * 0.25) *
            sqrt((17.0 - abs(temperature - 95.0)) * 0.05882);

    else if ((percentHumidity > 85.0) && (temperature >= 80.0) &&
             (temperature <= 87.0))
      hi += ((percentHumidity - 85.0) * 0.1) * ((87.0 - temperature) * 0.2);
  }

  return isFahrenheit ? hi : convertFtoC(hi);
}

/*!
 *  @brief  Get the current humidity due point
 *  @param  isFahrenheit
 *			the return type in Fahrenheit or Celcius 
 *	@return float due point
 */
float Adafruit_HDC1000::getDuePoint(bool isFahrenheit) {
  float tempC = readTemperature();
  float hum = readHumidity();
  float dp  = (tempC - (100.0 - hum) / 5.0);
  if (isFahrenheit)
	dp = (dp * 9.0 / 5.0) + 32.0;
  return dp;
}