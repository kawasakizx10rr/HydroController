/*!
 * @file Adafruit_HDC1000.h
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
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 * BSD license, all text above must be included in any redistribution
 */

#include "Arduino.h"
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>

#define HDC1000_I2CADDR 0x40 //!< HDC1000 I2C Address
#define HDC1000_TEMP 0x00    //!< Temperature register address
#define HDC1000_HUMID 0x01   //!< Humidity register address
#define HDC1000_CONFIG 0x02  //!< Config register address
#define HDC1000_CONFIG_RST                                                     \
  (1 << 15) //!< Software reset bit. 0, normal operation. 1, software reset.
#define HDC1000_CONFIG_HEAT                                                    \
  (1 << 13) //!< Heater. 0, heater disabled. 1, heater enabled
#define HDC1000_CONFIG_MODE                                                    \
  (1 << 12) //!< Mode of acquisition. 0, temperature or humidity is acquired. 1,
            //!< temperature is acquired then humidity
#define HDC1000_CONFIG_BATT                                                    \
  (1 << 11) //!< Battery status. 0, Battery voltage > 2.8v. 1, Battery voltage
            //!< < 2.8v
#define HDC1000_CONFIG_TRES_14                                                 \
  0 //!< Temperature measurement resolution, 0, 14 bit. 1, 11 bit.
#define HDC1000_CONFIG_TRES_11                                                 \
  (1 << 10) //!< Temperature measurement resolution, 0, 14 bit. 1, 11 bit.
#define HDC1000_CONFIG_HRES_14                                                 \
  0 //!< Humidity measurement resolution. 00, 14 bit. 01, 11 bit. 10, 8 bit.
#define HDC1000_CONFIG_HRES_11                                                 \
  (1 << 8) //!< Humidity measurement resolution. 00, 14 bit. 01, 11 bit. 10, 8
           //!< bit.
#define HDC1000_CONFIG_HRES_8                                                  \
  (1 << 9) //!< Humidity measurement resolution. 00, 14 bit. 01, 11 bit. 10, 8
           //!< bit.

#define HDC1000_SERIAL1                                                        \
  0xFB //!< Serial ID register. First 2 bytes of the serial ID of the part
#define HDC1000_SERIAL2                                                        \
  0xFC //!< Serial ID register. Mid 2 bytes of the serial ID of the part
#define HDC1000_SERIAL3                                                        \
  0xFD //!< Serial ID register. Last byte of the serial ID of the part
#define HDC1000_MANUFID                                                        \
  0xFE //!< Manufacturer ID register. ID of Texas Instruments
#define HDC1000_DEVICEID 0xFF //!< Device ID register. ID of HDC1000 device

/*!
 * @brief Main HDC1000 class
 */
class Adafruit_HDC1000 {
public:
  Adafruit_HDC1000();

  bool begin(uint8_t addr = HDC1000_I2CADDR, TwoWire *wire = &Wire);

  float readTemperature(void);
  float readHumidity(void);
  void reset(void);
  void drySensor(void);

  uint16_t read16(uint8_t addr, uint8_t delay = 0);
  uint32_t read32(uint8_t addr, uint8_t delay = 0);
  void writeConfig(uint16_t config);
  float convertCtoF(float);
  float convertFtoC(float);
  float computeHeatIndex(float temperature, float percentHumidity, bool isFahrenheit = true);
  float getDuePoint(bool isFahrenheit);
  
private:
  boolean readData(void);
  float humidity, temp;
  Adafruit_I2CDevice *i2c_dev = NULL; ///< Pointer to I2C bus interface
};
