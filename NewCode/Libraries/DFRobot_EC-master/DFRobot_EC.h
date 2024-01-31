/*
 * file DFRobot_EC.h * @ https://github.com/DFRobot/DFRobot_EC
 *
 * Arduino library for Gravity: Analog Electrical Conductivity Sensor / Meter Kit V2 (K=1), SKU: DFR0300
 *
 * Copyright   [DFRobot](http://www.dfrobot.com), 2018
 * Copyright   GNU Lesser General Public License
 *
 * version  V1.01
 * date  2018-06
 */

#ifndef _DFROBOT_EC_H_
#define _DFROBOT_EC_H_

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class DFRobot_EC {
public:
    DFRobot_EC();
    ~DFRobot_EC();
    bool calibration(const float voltage, const float temperature);                  //calibration by Serial CMD
    float readEC(const float voltage, const float temperature);                       // voltage to EC value, with temperature compensation
    void  begin(const float a_kvalueLow = 1, const float a_kvalueHigh = 1);                                                        //initialization
	float getKvalueLow();
	float getKvalueHigh();
	
private:
	float m_rawEC;
    float m_kvalue;
    float m_kvalueLow;
    float m_kvalueHigh;
};

#endif
