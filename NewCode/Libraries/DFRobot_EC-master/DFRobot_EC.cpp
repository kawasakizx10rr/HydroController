/*
 * file DFRobot_EC.cpp
 * @ https://github.com/DFRobot/DFRobot_EC
 *
 * Arduino library for Gravity: Analog Electrical Conductivity Sensor / Meter Kit V2 (K=1), SKU: DFR0300
 *
 * Copyright   [DFRobot](http://www.dfrobot.com), 2018
 * Copyright   GNU Lesser General Public License
 *
 * version  V1.01
 * date  2018-06
 */


#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "DFRobot_EC.h"

#define KVALUEADDR 0x0A    //the start address of the K value stored in the EEPROM
#define RES2 820.0
#define ECREF 200.0

DFRobot_EC::DFRobot_EC() {
    this->m_kvalue                 = 1.0;
    this->m_kvalueLow              = 1.0;
    this->m_kvalueHigh             = 1.0;
} 

DFRobot_EC::~DFRobot_EC()
{
 // Nothing going on here yet...
}

void DFRobot_EC::begin(const float a_kvalueLow, const float a_kvalueHigh) {
	this->m_kvalueLow =  a_kvalueLow; 
	this->m_kvalueHigh = a_kvalueHigh; 
    this->m_kvalue =  a_kvalueLow;                // set default K value: K = kvalueLow
}

float DFRobot_EC::readEC(float voltage, float temperature) {
    float value = 0,valueTemp = 0;
    this->m_rawEC = 1000*voltage/RES2/ECREF;
    valueTemp = this->m_rawEC * this->m_kvalue;
    //automatic shift process
    //First Range:(0,2); Second Range:(2,20)
    if(valueTemp > 2.5)
        this->m_kvalue = this->m_kvalueHigh;
	else if(valueTemp < 2.0)
        this->m_kvalue = this->m_kvalueLow;

    value = this->m_rawEC * this->m_kvalue;             //calculate the EC value after automatic shift
    value = value / (1.0+0.0185*(temperature-25.0));  //temperature compensation
    return value;
}

bool DFRobot_EC::calibration(const float a_voltage, const float a_temperature) {
    float compECsolution = 0;
    float KValueTemp = 0;
	Serial.println(F("EC Calibration Mode"));
	Serial.println(F("Please put the probe into the 1413us/cm or 12.88ms/cm buffer solution"));
	readEC(a_voltage, a_temperature);
	if (this->m_rawEC > 0.9 && this->m_rawEC<1.9){                   //recognize 1.413us/cm buffer solution
		Serial.println(F("Detected 1.413us/cm buffer solution"));
		compECsolution = 1.413*(1.0+0.0185*(a_temperature-25.0));  //temperature compensation
	}
	else if (this->m_rawEC > 9 && this->m_rawEC < 16.8){             //recognize 12.88ms/cm buffer solution
		Serial.println(F("Detected 12.88ms/cm buffer solution"));
		compECsolution = 12.88*(1.0+0.0185*(a_temperature-25.0));  //temperature compensation
	}
	else {
		Serial.print(F("Calibration failed try again"));             
		return false;
	}
	KValueTemp = RES2*ECREF*compECsolution/1000.0/a_voltage;       //calibrate the k value
	if((KValueTemp>0.5) && (KValueTemp<1.5)){
		Serial.println(F("EC calibration successful"));
		this->m_kvalue =  KValueTemp;
		if (this->m_rawEC > 0.9 && this->m_rawEC < 1.9) {
			Serial.print(F("1.413us/cm kvalueLow = ")); Serial.println(KValueTemp); 
			this->m_kvalueLow =  KValueTemp;			
		}
		else if (this->m_rawEC > 9 && this->m_rawEC < 16.8) {
			Serial.print(F("12.88ms/cm kvalueHigh = ")); Serial.println(KValueTemp); 
			this->m_kvalueHigh =  KValueTemp;
		}
		return true;
	}
	else{
		Serial.print(F("EC calibration error try again"));   
		return false;
	}
}

float DFRobot_EC::getKvalueLow() {
	return this->m_kvalueLow;
}

float DFRobot_EC::getKvalueHigh() {
	return this->m_kvalueHigh;
}