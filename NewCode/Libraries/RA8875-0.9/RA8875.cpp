/*
Part of RA8875 library from https://github.com/sumotoy/RA8875
License:GNU General Public License v3.0

    RA8875 fast SPI library for RAiO SPI RA8875 drived TFT
    Copyright (C) 2014  egidio massimo costa sumotoy (a t) gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#if !defined(SPARK)//SPI already included in applications.h
	#include <SPI.h>
#endif
#include "RA8875.h"

#if defined(EEPROM_SUPPORTED)
#include <EEPROM.h>
#endif

#if defined (USE_FT5206_TOUCH)
	#if !defined(SPARK)//wire it's already included in applications.h
		#include <Wire.h>
		#if defined(___DUESTUFF) && defined(USE_DUE_WIRE1_INTERFACE)
			#define Wire Wire1
		#endif
	#endif
	const uint8_t _ctpAdrs = 0x38;
	const uint8_t coordRegStart[5] = {0x03,0x09,0x0F,0x15,0x1B};
	static volatile bool _FT5206_INT = false;
#endif

static volatile uint8_t _RA8875_INTS = 0b00000000;//container for INT states
/*------------------------------
Bit:	Called by:		In use:
--------------------------------
0: 		isr triggered	[*]
1: 		Resistive TS	[*]
2: 		KeyScan			[*]
3: 		DMA				 	
4: 		BTE				 
5: 		FT5206 TS		[*]
6: 		-na-			 
7: 		-na-			 
--------------------------------*/


/**************************************************************************/
/*!
	Contructors
	CSp: SPI SS pin
	RSTp: Reset pin
	INTp: INT pin
	//Teensy 3.0 , 3.1 , LC
	mosi_pin
	sclk_pin
	miso_pin
	Note:
	Teensy CS SPI1:[MOSI1(0) MISO1(1) CLK1(20) CS1(6)]
*/
/**************************************************************************/
//------------------------------TEENSY 3/3.1 ---------------------------------------
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
	RA8875::RA8875(const uint8_t CSp,const uint8_t RSTp,const uint8_t mosi_pin,const uint8_t sclk_pin,const uint8_t miso_pin)
	{
		_mosi = mosi_pin;
		_miso = miso_pin;
		_sclk = sclk_pin;
		_cs = CSp;
		_rst = RSTp;
//------------------------------Teensy LC-------------------------------------------
#elif defined(__MKL26Z64__)
	RA8875::RA8875(const uint8_t CSp,const uint8_t RSTp,const uint8_t mosi_pin,const uint8_t sclk_pin,const uint8_t miso_pin)
	{
		_mosi = mosi_pin;
		_miso = miso_pin;
		_sclk = sclk_pin;
		_cs = CSp;
		_rst = RSTp;
		_altSPI = false;
//------------------------------Teensy of the future -------------------------------------------
#elif defined(__MK64FX512__) || defined(__MK66FX1M0__)
	RA8875::RA8875(const uint8_t CSp,const uint8_t RSTp,const uint8_t mosi_pin,const uint8_t sclk_pin,const uint8_t miso_pin)
	{
		_mosi = mosi_pin;
		_miso = miso_pin;
		_sclk = sclk_pin;
		_cs = CSp;
		_rst = RSTp;
//---------------------------------DUE--------------------------------------------
#elif defined(___DUESTUFF)//DUE
	RA8875::RA8875(const uint8_t CSp, const uint8_t RSTp) 
	{
		_cs = CSp;
		_rst = RSTp;
//---------------------------------SPARK----------------------------------------
#elif defined(SPARK)//SPARK
	RA8875::RA8875(const uint8_t CSp, const uint8_t RSTp) 
	{
		_cs = CSp;
		_rst = RSTp;
//------------------------------ENERGIA-------------------------------------------
#elif defined(NEEDS_SET_MODULE)
	RA8875::RA8875(const uint8_t module, const uint8_t RSTp) 
	{
		selectCS(module);
		_rst = RSTp;
		_cs = 255;
//----------------------------8 BIT ARDUINO's---------------------------------------
#else
	RA8875::RA8875(const uint8_t CSp, const uint8_t RSTp) 
	{
		_cs = CSp;
		_rst = RSTp;
#endif
}

/**************************************************************************/
/*!	
	Helper for Energia, it will set CS pin accordly module selected
	module: 0...3
	[private]
*/
/**************************************************************************/
#if defined(NEEDS_SET_MODULE)
void RA8875::selectCS(uint8_t module) 
{
	if (module > 3) module = 3;
	switch(module){
		case 0:
			_cs = PA_3;
		break;
		case 1:
			_cs = PF_3;
		break;
		case 2:
			_cs = PB_5;
		break;
		case 3:
			_cs = PD_1;
		break;
	}
	SPImodule = module;
}
#endif


/**************************************************************************/
/*!
	Initialize library and SPI
	Parameter:
	(display type)
    RA8875_480x272 (4.3" displays)
    RA8875_800x480 (5" and 7" displays)
	Adafruit_480x272 (4.3" Adafruit displays)
	Adafruit_800x480 (5" and 7" Adafruit displays)
	(colors) - The color depth (default 16) 8 or 16 (bit)
	-------------------------------------------------------------
	UPDATE! in Energia IDE some devices needs an extra parameter!
	module: sets the SPI interface (it depends from MCU). Default:0
*/
/**************************************************************************/
void RA8875::begin(const enum RA8875sizes s,uint8_t colors) 
{
	_errorCode = 0;
	_displaySize = s;
	_rotation = 0;
	_portrait = false;
	_inited = false;
	_sleep = false;
	_hasLayerLimits = false;
	_intPin = 255;
	_intNum = 0;
	_useISR = false;
	_enabledInterrups = 0b00000000;
	/* used to understand what causes an INT
	bit
	0:
	1:
	2: Touch (resistive)
	3:
	4:
	5:
	6:
	7:
	8:
	*/
	#if defined(USE_FT5206_TOUCH)
		_intCTSPin = 255;
		_intCTSNum = 0;
	#endif
	_maxLayers = 2;
	_currentLayer = 0;
	_useMultiLayers = false;//starts with one layer only
	_brightness = 255;
	//_FNTrender = false;
	/* set-->  _TXTparameters  <--
	0:_extFontRom = false;
	1:_autoAdvance = true;
	2:_textWrap = user defined
	3:_fontFullAlig = false;
	4:_fontRotation = false;//not used
	5:_alignXToCenter = false;
	6:_alignYToCenter = false;
	7: render         = false;
	*/
	_relativeCenter = false;
	_absoluteCenter = false;
	_arcAngle_max = ARC_ANGLE_MAX;
	_arcAngle_offset = ARC_ANGLE_OFFSET;
	_angle_offset = ANGLE_OFFSET;
	_color_bpp = 16;
	_colorIndex = 0;

	if (colors != 16) {
		_color_bpp = 8;
		_colorIndex = 3;
	}

	switch (_displaySize){
		case RA8875_480x272:
		case Adafruit_480x272:
			_width = 	480;
			_height = 	272;
			_initIndex = 0;
		break;
		case RA8875_800x480:
		case Adafruit_800x480:
		case RA8875_800x480ALT:
			_width = 	800;
			_height = 	480;
			_hasLayerLimits = true;
			_maxLayers = 1;
			if (_color_bpp < 16) _maxLayers = 2;
			_initIndex = 1;
			if (_displaySize == RA8875_800x480ALT) _initIndex = 2;
		break;
		default:
		_errorCode |= (1 << 0);//set
		return;
	}
	RA8875_WIDTH = _width;
	RA8875_HEIGHT = _height;
	_activeWindowXL = 0;
	_activeWindowXR = RA8875_WIDTH;
	_activeWindowYT = 0;
	_activeWindowYB = RA8875_HEIGHT;
	#if !defined(_AVOID_TOUCHSCREEN)//common to all touch
		_clearTInt = false;
		_touchEnabled = false;
		#if defined(USE_RA8875_TOUCH)//resistive touch
			_calibrated = _isCalibrated();//check calibration at startup
			if (!_calibrated) {
				_tsAdcMinX = 0; _tsAdcMinY = 0; _tsAdcMaxX = 1023; _tsAdcMaxY = 1023;
			} else {
				_tsAdcMinX = TOUCSRCAL_XLOW; _tsAdcMinY = TOUCSRCAL_YLOW; _tsAdcMaxX = TOUCSRCAL_XHIGH; _tsAdcMaxY = TOUCSRCAL_YHIGH;
			}
		#endif
	#endif

	#if defined(USE_RA8875_KEYMATRIX)
		_keyMatrixEnabled = false;
	#endif
/* Display Configuration Register	  [0x20]
	  7: (Layer Setting Control) 0:one Layer, 1:two Layers
	  6,5,4: (na)
	  3: (Horizontal Scan Direction) 0: SEG0 to SEG(n-1), 1: SEG(n-1) to SEG0
	  2: (Vertical Scan direction) 0: COM0 to COM(n-1), 1: COM(n-1) to COM0
	  1,0: (na) */
	_DPCR_Reg = 0b00000000;
/*	Memory Write Control Register 0 [0x40]
	7: 0(graphic mode), 1(textx mode)
	6: 0(font-memory cursor not visible), 1(visible)
	5: 0(normal), 1(blinking)
	4: na
	3-2: 00(LR,TB), 01(RL,TB), 10(TB,LR), 11(BT,LR)
	1: 0(Auto Increase in write), 1(no)
	0: 0(Auto Increase in read), 1(no) */
	_MWCR0_Reg = 0b00000000;
	
/*	Font Control Register 0 [0x21]
	7: 0(CGROM font is selected), 1(CGRAM font is selected)
	6: na
	5: 0(Internal CGROM [reg 0x2F to 00]), 1(External CGROM [0x2E reg, bit6,7 to 0)
	4-2: na
	1-0: 00(ISO/IEC 8859-1), 01(ISO/IEC 8859-2), 10(ISO/IEC 8859-3), 11(ISO/IEC 8859-4)*/
	
	_FNCR0_Reg = 0b00000000;
/*	Font Control Register 1 [0x22]
	7: 0(Full Alignment off), 1(Full Alignment on)
	6: 0(no-trasparent), 1(trasparent)
	5: na
	4: 0(normal), 1(90degrees)
	3-2: 00(x1), 01(x2), 10(x3), 11(x3) Horizontal font scale
	1-0: 00(x1), 01(x2), 10(x3), 11(x3) Vertical font scale */
	_FNCR1_Reg = 0b00000000;

	/*	Font Write Type Setting Register [0x2E]
	7-6: 00(16x16,8x16,nx16), 01(24x24,12x24,nx24), 1x(32x32,16x32, nx32)
	5-0: 00...3F (font width off to 63 pixels)*/
	_FWTSET_Reg = 0b00000000;
	
	/*	Serial Font ROM Setting [0x2F]
	GT Serial Font ROM Select
	7-5: 000(GT21L16TW/GT21H16T1W),001(GT30L16U2W),010(GT30L24T3Y/GT30H24T3Y),011(GT30L24M1Z),111(GT30L32S4W/GT30H32S4W)
	FONT ROM Coding Setting
	4-2: 000(GB2312),001(GB12345/GB18030),010(BIG5),011(UNICODE),100(ASCII),101(UNI-Japanese),110(JIS0208),111(Latin/Greek/Cyrillic/Arabic)
	1-0: 00...11 
		 bits	ASCII		Lat/Gr/Cyr		Arabic
		 00		normal		normal			na
		 01		Arial		var Wdth		Pres Forms A
		 10		Roman		na				Pres Forms B
		 11		Bold		na				na */
	_SFRSET_Reg = 0b00000000;
	
	/*	Interrupt Control Register1		  [0xF0]
	7,6,5: (na)
	4: KEYSCAN Interrupt Enable Bit
	3: DMA Interrupt Enable Bit
	2: TOUCH Panel Interrupt Enable Bit
	1: BTE Process Complete Interrupt Enable Bit
	0:
	When MCU-relative BTE operation is selected(*1) and BTE
	Function is Enabled(REG[50h] Bit7 = 1), this bit is used to
		Enable the BTE Interrupt for MCU R/W:
		0 : Disable BTE interrupt for MCU R/W.
		1 : Enable BTE interrupt for MCU R/W.
	When the BTE Function is Disabled, this bit is used to
		Enable the Interrupt of Font Write Function:
		0 : Disable font write interrupt.
		1 : Enable font write interrupt.
	*/
	_INTC1_Reg = 0b00000000;

	//------------------------------- Start SPI initialization ------------------------------------------
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		//always uses SPI transaction
		if ((_mosi == 11 || _mosi == 7) && (_miso == 12 || _miso == 8) && (_sclk == 13 || _sclk == 14)) {//valid SPI pins?
			if (_mosi != 11) SPI.setMOSI(_mosi);
			if (_miso != 12) SPI.setMISO(_miso);
			if (_sclk != 13) SPI.setSCK(_sclk);
		} else {
			_errorCode |= (1 << 1);//set
			return;
		}
		if (!SPI.pinIsChipSelect(_cs)) {
			_errorCode |= (1 << 2);//set
			return;
		}
		pinMode(_cs, OUTPUT);
		SPI.begin();
		digitalWrite(_cs, HIGH);
	#elif defined(__MK64FX512__) || defined(__MK66FX1M0__)	//future teensys
		//always uses SPI transaction
		if ((_mosi == 11 || _mosi == 7) && (_miso == 12 || _miso == 8) && (_sclk == 13 || _sclk == 14)) {//valid SPI pins?
			if (_mosi != 11) SPI.setMOSI(_mosi);
			if (_miso != 12) SPI.setMISO(_miso);
			if (_sclk != 13) SPI.setSCK(_sclk);
		} else {
			_errorCode |= (1 << 1);//set
			return;
		}
		if (!SPI.pinIsChipSelect(_cs)) {
			_errorCode |= (1 << 2);//set
			return;
		}
		pinMode(_cs, OUTPUT);
		SPI.begin();
		digitalWrite(_cs, HIGH);
	#elif defined(__MKL26Z64__)//TeensyLC
		//always uses SPI ransaction
		#if TEENSYDUINO > 121//not supported prior 1.22!
			if ((_mosi == 11 || _mosi == 7 || _mosi == 0 || _mosi == 21) && (_miso == 12 || _miso == 8 || _miso == 1 || _miso == 5) && (_sclk == 13 || _sclk == 14 || _sclk == 20)) {//valid SPI pins?
				if ((_mosi == 0 || _mosi == 21) && (_miso == 1 || _miso == 5) && (_sclk == 20)) {//identify alternate SPI channel 1 (24Mhz)
					_altSPI = true;
					if (_cs != 6){//on SPI1 cs should be only 6!
						_errorCode |= (1 << 2);//set
						return;
					}
					if (_mosi != 11) SPI1.setMOSI(_mosi);
					if (_miso != 12) SPI1.setMISO(_miso);
					if (_sclk != 13) SPI1.setSCK(_sclk);
					pinMode(_cs, OUTPUT);
					SPI1.begin();
				} else {//default SPI channel 0 (12Mhz)
					_altSPI = false;
					if (_mosi != 11) SPI.setMOSI(_mosi);
					if (_miso != 12) SPI.setMISO(_miso);
					if (_sclk != 13) SPI.setSCK(_sclk);
					if (!SPI.pinIsChipSelect(_cs)) {//ERROR
						_errorCode |= (1 << 2);//set
						return;
					}
					pinMode(_cs, OUTPUT);
					SPI.begin();
					digitalWrite(_cs, HIGH);
				}
			} else {
				_errorCode |= (1 << 1);//set
				return;
			}
		#else
			_altSPI = false;
			pinMode(_cs, OUTPUT);
			SPI.begin();
			digitalWrite(_cs, HIGH);
			_errorCode |= (1 << 3);//set
		#endif
	#elif !defined(ENERGIA)//everithing but ENERGIA
		#if defined(___DUESTUFF)// DUE
			#if defined(SPI_DUE_MODE_EXTENDED)
				//DUE SPI mode extended you can use only follow pins
				if (_cs == 4 || _cs == 10 || _cs == 52) {
					SPI.begin(_cs);
				} else {
					_errorCode |= (1 << 2);//error! wrong cs pin
					return;
				}
			#else
				//DUE in normal SPI mode
				pinMode(_cs, OUTPUT);
				SPI.begin();
				#if defined(_FASTSSPORT)
					csport = portOutputRegister(digitalPinToPort(_cs));
					cspinmask = digitalPinToBitMask(_cs);
					*csport |= cspinmask;//hi
				#else
					digitalWrite(_cs, HIGH);
				#endif
			#endif
		#elif defined(ESP8266) || defined(ESP32)
			pinMode(_cs, OUTPUT);
			SPI.begin();
			#if defined(_FASTSSPORT) && !defined(ESP32)
				GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, _pinRegister(_cs));//H
			#else
				digitalWrite(_cs, HIGH);//for now
			#endif
		#elif defined(SPARK)
			pinMode(_cs, OUTPUT);
			SPI.begin();
			pinSetFast(_cs);//for now
		#else
			//UNO,MEGA,Yun,nano,duemilanove and other 8 bit arduino's
			pinMode(_cs, OUTPUT);
			SPI.begin();
			csport = portOutputRegister(digitalPinToPort(_cs));//pinMode(_cs, OUTPUT);
			cspinmask = digitalPinToBitMask(_cs);
			*csport |= cspinmask;//hi
		#endif
	#endif
	if (_rst < 255){//time for hardware reset RA8875
		pinMode(_rst, OUTPUT);
		digitalWrite(_rst, HIGH);
		delay(10);
		digitalWrite(_rst, LOW);
		delay(220);//120
		digitalWrite(_rst, HIGH);
		delay(300);//200
	}
	
	#if defined(ENERGIA) && defined(NEEDS_SET_MODULE)//energia specific
		SPI.setModule(SPImodule);
	#endif

	//set SPI SPEED, starting at low speed, after init will raise up!
	#if defined(SPI_HAS_TRANSACTION)
		_SPImaxSpeed = 4000000UL;//we start in low speed here!
	#else//do not use SPItransactons
		#if defined (__AVR__)//8 bit arduino's
			pinMode(_cs, OUTPUT);
			SPI.begin();
			SPI.setClockDivider(SPI_SPEED_SAFE);
			delay(1);
			SPI.setDataMode(SPI_MODE3);
		#else
			#if defined(___DUESTUFF) && defined(SPI_DUE_MODE_EXTENDED)
				SPI.setClockDivider(_cs,SPI_SPEED_SAFE);
				delay(1);
				SPI.setDataMode(_cs,SPI_MODE3);
			#elif defined (ESP8266)
				SPI.setClockDivider(SPI_SPEED_SAFE);
				delay(1);
				SPI.setDataMode(SPI_MODE0);
			#elif defined(SPARK)
				SPI.setClockDivider(SPI_SPEED_SAFE);
				delay(1);
				SPI.setDataMode(SPI_MODE0);
			#else
				SPI.setClockDivider(SPI_SPEED_SAFE);
				delay(1);
				SPI.setDataMode(SPI_MODE3);
			#endif
		#endif
	#endif
	#if defined(ENERGIA)//dunno why but energia wants this here or not work!
		pinMode(_cs, OUTPUT);
		digitalWrite(_cs, HIGH);
	#endif
	//SPI initialization done
	if (_errorCode != 0) return;//ouch, error/s.Better stop here!
	_initialize();//----->Time to Initialize the RA8875!
	//------- time for capacitive touch stuff -----------------
	#if defined(USE_FT5206_TOUCH)
		Wire.begin();
		#if defined(___DUESTUFF)
			Wire.setClock(400000UL); // Set I2C frequency to 400kHz
			/*
			#if !defined(USE_DUE_WIRE1_INTERFACE)//sorry but I do not own a DUE, have to learn about Wire1
			// Force 400 KHz I2C, rawr! (Uses pins 20, 21 for SDA, SCL)
			TWI1->TWI_CWGR = 0;
			TWI1->TWI_CWGR = ((VARIANT_MCK / (2 * 400000)) - 4) * 0x101;
			#endif
			*/
		#else
			//TODO, Dunno what to do here with SPARK
			#if ARDUINO >= 157
				Wire.setClock(400000UL); // Set I2C frequency to 400kHz
			#else
				TWBR = ((F_CPU / 400000UL) - 16) / 2; // Set I2C frequency to 400kHz
			#endif
		#endif
		delay(10);
		_initializeFT5206();//initialize FT5206 controller
		_maxTouch = 5;
		_gesture = 0;
		_currentTouches = 0;
		_currentTouchState = 0;
		_needISRrearm = false;
		//TO DO
		//Modify RA8875 registers and disconnect internal Touch Screen totally
	#endif
}


/************************* Initialization *********************************/

/**************************************************************************/
/*!
      Hardware initialization of RA8875 and turn on
	  [private]
*/
/**************************************************************************/
void RA8875::_initialize() 
{
	_inited = false;
	if (_rst == 255) {// soft reset time ?
		writeCommand(RA8875_PWRR);
		_writeData(RA8875_PWRR_SOFTRESET);
		delay(20);
		_writeData(RA8875_PWRR_NORMAL);
		delay(200);
	}
	//set the sysClock
	_setSysClock(initStrings[_initIndex][0],initStrings[_initIndex][1],initStrings[_initIndex][2]);
	
	//color space setup
	if (_color_bpp < 16){//256
		_writeRegister(RA8875_SYSR,0x00);//256
		_colorIndex = 3;
	} else {
		_writeRegister(RA8875_SYSR,0x0C);//65K
		_colorIndex = 0;
	}
	
	_writeRegister(RA8875_HDWR,initStrings[_initIndex][3]);		//LCD Horizontal Display Width Register
	_writeRegister(RA8875_HNDFTR,initStrings[_initIndex][4]);	//Horizontal Non-Display Period Fine Tuning Option Register
	_writeRegister(RA8875_HNDR,initStrings[_initIndex][5]);		//LCD Horizontal Non-Display Period Register
	_writeRegister(RA8875_HSTR,initStrings[_initIndex][6]);		//HSYNC Start Position Register
	_writeRegister(RA8875_HPWR,initStrings[_initIndex][7]);		//HSYNC Pulse Width Register
	_writeRegister(RA8875_VDHR0,initStrings[_initIndex][8]);	//LCD Vertical Display Height Register0
	_writeRegister(RA8875_VDHR0+1,initStrings[_initIndex][9]);	//LCD Vertical Display Height Register1
	_writeRegister(RA8875_VNDR0,initStrings[_initIndex][10]);	//LCD Vertical Non-Display Period Register 0
	_writeRegister(RA8875_VNDR0+1,initStrings[_initIndex][11]);	//LCD Vertical Non-Display Period Register 1
	_writeRegister(RA8875_VSTR0,initStrings[_initIndex][12]);	//VSYNC Start Position Register 0
	_writeRegister(RA8875_VSTR0+1,initStrings[_initIndex][13]);	//VSYNC Start Position Register 1
	_writeRegister(RA8875_VPWR,initStrings[_initIndex][14]);	//VSYNC Pulse Width Register
	_updateActiveWindow(true);									//set the whole screen as active
	//clearActiveWindow();
	delay(10); //100
	//setFont(INTFONT);	//set internal font use
	//postburner PLL!
	_setSysClock(sysClockPar[_initIndex][0],sysClockPar[_initIndex][1],initStrings[_initIndex][2]);
	_inited = true;
	//from here we will go at high speed!
	#if defined(_FASTCPU)
		_slowDownSPI(false);
	#else
		#if defined(SPI_HAS_TRANSACTION)
			#if defined(__MKL26Z64__)
				if (_altSPI){
					_SPImaxSpeed = MAXSPISPEED2;
				} else {
					_SPImaxSpeed = MAXSPISPEED;
				}
			#else			
				_SPImaxSpeed = MAXSPISPEED;
			#endif
		#else
			#if defined(___DUESTUFF) && defined(SPI_DUE_MODE_EXTENDED)
				SPI.setClockDivider(_cs,SPI_SPEED_WRITE);
			#else
				SPI.setClockDivider(SPI_SPEED_WRITE);
			#endif
		#endif
	#endif
	delay(1);
	clearMemory();//clearMemory(true);
	delay(1);
	displayOn(true);//turn On Display
	delay(1);
	fillWindow(_RA8875_DEFAULTBACKLIGHT);//set screen black
	backlight(true);
	setRotation(_RA8875_DEFAULTSCRROT);
	setActiveWindow();
}

/**************************************************************************/
/*!
      This function set the sysClock accordly datasheet
	  Parameters:
	  pll1: PLL Control Register 1
	  pll2: PLL Control Register 2
	  pixclk: Pixel Clock Setting Register
	  [private]
*/
/**************************************************************************/
void RA8875::_setSysClock(uint8_t pll1,uint8_t pll2,uint8_t pixclk)
{
	_writeRegister(RA8875_PLLC1,pll1);////PLL Control Register 1
	delay(1);
	_writeRegister(RA8875_PLLC1+1,pll2);////PLL Control Register 2
	delay(1);
	_writeRegister(RA8875_PCSR,pixclk);//Pixel Clock Setting Register
	delay(1);
}

/**************************************************************************/
/*!
      This return a byte with the error code/s:
	  bit--------------------------------------------------------------------
	  0:	The display it's not supported!
	  1:	The MOSI or MISO or SCLK choosed for Teensy it's out permitted range!
	  2:	The CS pin you selected it's out permitted range!
	  3:	You have to upgrade to Teensyduino 1.22 or better to use this feature!
	  4:
	  5:
	  6:
	  7:
	 0b00000000 = no error
*/
/**************************************************************************/
uint8_t RA8875::errorCode(void) 
{
	return _errorCode;
}

/**************************************************************************/
/*!
	return true when register has done the job, otherwise false.
*/
/**************************************************************************/
boolean RA8875::_waitPoll(uint8_t regname, uint8_t waitflag) 
{
	uint8_t temp;
	unsigned long timeout = millis();
	
	while (1) {
		temp = _readRegister(regname);
		if (!(temp & waitflag)) return true;
		if ((millis() - timeout) > 20) return false;//emergency exit!
	}  
	return false;
}

/**************************************************************************/
/*!
	Just another specified wait routine until job it's done
	Parameters:
	res:
	0x80(for most operations),
	0x40(BTE wait), 
	0x01(DMA wait)
*/
/**************************************************************************/
void RA8875::_waitBusy(uint8_t res) 
{
	uint8_t temp; 	
	unsigned long start = millis();//M.Sandercock
	do {
		if (res == 0x01) writeCommand(RA8875_DMACR);//dma
		temp = readStatus();
		if ((millis() - start) > 10) return;
	} while ((temp & res) == res);
}

/**************************************************************************/
/*!		
		Clear memory (different from fillWindow!)
	    Parameters:
		full: true(clear all memory), false(clear active window only)
		When in multilayer it automatically clear L1 & L1 and switch back to current layer
*/
/**************************************************************************/
/*
void RA8875::clearMemory(boolean full)
{
	uint8_t temp = 0b10000000;
	if (!full && !_useMultiLayers) temp |= (1 << 6);//set 6
	_writeRegister(RA8875_MCLR,temp);
	_waitBusy(0x80);
	if (full && _useMultiLayers){
		temp = 0b10000000;
		uint8_t templ = _currentLayer;//remember current layer
		if (templ > 0){//we are in L2
			writeTo(L1);//switch to L1
		} else {
			writeTo(L2);//switch to L2
		}
		_writeRegister(RA8875_MCLR,temp);
		_waitBusy(0x80);
		if (templ > 0){//we was in L2
			writeTo(L2);//switch back to L2
		} else {
			writeTo(L1);//switch back to L1
		}
	}
}
*/

/**************************************************************************/
/*!		
		Clear memory (different from fillWindow!)
	    Parameters:
		stop: stop clear memory operation
*/
/**************************************************************************/
void RA8875::clearMemory(bool stop)
{
	uint8_t temp;
	temp = _readRegister(RA8875_MCLR);
	stop == true ? temp &= ~(1 << 7) : temp |= (1 << 7);
	_writeData(temp); 
	if (!stop) _waitBusy(0x80);
}

/**************************************************************************/
/*!		
		Clear the active window
	    Parameters:
		full: false(clear current window), true clear full window
*/
/**************************************************************************/
void RA8875::clearActiveWindow(bool full)
{
	uint8_t temp;
	temp = _readRegister(RA8875_MCLR);
	full == true ? temp &= ~(1 << 6) : temp |= (1 << 6);
	_writeData(temp);  
	//_waitBusy(0x80);
}

/**************************************************************************/
/*!		
		Clear width BG color
	    Parameters:
		bte: false(clear width BTE BG color), true(clear width font BG color)
*/
/**************************************************************************/
void RA8875::clearWidthColor(bool bte)
{
	uint8_t temp;
	temp = _readRegister(RA8875_MCLR);
	bte == true ? temp &= ~(1 << 0) : temp |= (1 << 0);
	_writeData(temp);  
	//_waitBusy(0x80);
}

/**************************************************************************/
/*!
      turn display on/off
*/
/**************************************************************************/
void RA8875::displayOn(boolean on) 
{
	on == true ? _writeRegister(RA8875_PWRR, RA8875_PWRR_NORMAL | RA8875_PWRR_DISPON) : _writeRegister(RA8875_PWRR, RA8875_PWRR_NORMAL | RA8875_PWRR_DISPOFF);
}

/**************************************************************************/
/*!		
		Set the Active Window
	    Parameters:
		XL: Horizontal Left
		XR: Horizontal Right
		YT: Vertical TOP
		YB: Vertical Bottom
*/
/**************************************************************************/
void RA8875::setActiveWindow(int16_t XL,int16_t XR ,int16_t YT ,int16_t YB)
{
	if (_portrait){ swapvals(XL,YT); swapvals(XR,YB);}
	
	if (XR >= RA8875_WIDTH) XR = RA8875_WIDTH;
	if (YB >= RA8875_HEIGHT) YB = RA8875_HEIGHT;
	
	_activeWindowXL = XL; _activeWindowXR = XR;
	_activeWindowYT = YT; _activeWindowYB = YB;
	_updateActiveWindow(false);
}

/**************************************************************************/
/*!		
		Set the Active Window as FULL SCREEN
*/
/**************************************************************************/
void RA8875::setActiveWindow(void)
{
	_activeWindowXL = 0; _activeWindowXR = RA8875_WIDTH;
	_activeWindowYT = 0; _activeWindowYB = RA8875_HEIGHT;
	if (_portrait){swapvals(_activeWindowXL,_activeWindowYT); swapvals(_activeWindowXR,_activeWindowYB);}
	_updateActiveWindow(true);
}

/**************************************************************************/
/*!		
		Set the Active Window
	    Parameters:
		XL: Horizontal Left
		XR: Horizontal Right
		YT: Vertical TOP
		YB: Vertical Bottom
*/
/**************************************************************************/
void RA8875::getActiveWindow(int16_t &XL,int16_t &XR ,int16_t &YT ,int16_t &YB)//0.69b24
{
	XL = _activeWindowXL; XR = _activeWindowXR;
	YT = _activeWindowYT; YB = _activeWindowYB;
}

/**************************************************************************/
/*!		
		Return the max tft width.
		Parameters: 
		absolute: if true will return the phisical width
*/
/**************************************************************************/
uint16_t RA8875::width(bool absolute) const 
{ 
	if (absolute) return RA8875_WIDTH;
	return _width; 
}

/**************************************************************************/
/*!		
		Return the max tft height.
		Parameters: 
		absolute: if true will return the phisical height
*/
/**************************************************************************/
uint16_t RA8875::height(bool absolute) const 
{ 
	if (absolute) return RA8875_HEIGHT;
	return _height; 
}


/**************************************************************************/
/*!
      Change the beam scan direction on display
	  Parameters:
	  invertH: true(inverted),false(normal) horizontal
	  invertV: true(inverted),false(normal) vertical
*/
/**************************************************************************/
void RA8875::_scanDirection(boolean invertH,boolean invertV)
{
	invertH == true ? _DPCR_Reg |= (1 << 3) : _DPCR_Reg &= ~(1 << 3);
	invertV == true ? _DPCR_Reg |= (1 << 2) : _DPCR_Reg &= ~(1 << 2);
	_writeRegister(RA8875_DPCR,_DPCR_Reg);
}

/**************************************************************************/
/*!
      Change the rotation of the screen
	  Parameters:
	  rotation:
	    0 = default
		1 = 90
		2 = 180
		3 = 270
*/
/**************************************************************************/
void RA8875::setRotation(uint8_t rotation)//0.69b32 - less code
{
	_rotation = rotation % 4; //limit to the range 0-3
	switch (_rotation) {
	case 0:
		//default, connector to bottom
		_portrait = false;
		_scanDirection(0,0);
		#if defined(USE_RA8875_TOUCH)
			if (!_calibrated) {
				_tsAdcMinX = 0;  _tsAdcMinY = 0; _tsAdcMaxX = 1023;  _tsAdcMaxY = 1023;
			} else {
				_tsAdcMinX = TOUCSRCAL_XLOW; _tsAdcMinY = TOUCSRCAL_YLOW; _tsAdcMaxX = TOUCSRCAL_XHIGH; _tsAdcMaxY = TOUCSRCAL_YHIGH;
			}
		#endif
    break;
	case 1:
		//90
		_portrait = true;
		_scanDirection(1,0);
		#if defined(USE_RA8875_TOUCH)
			if (!_calibrated) {
				_tsAdcMinX = 1023; _tsAdcMinY = 0; _tsAdcMaxX = 0; _tsAdcMaxY = 1023;
			} else {
				_tsAdcMinX = TOUCSRCAL_XHIGH; _tsAdcMinY = TOUCSRCAL_YLOW; _tsAdcMaxX = TOUCSRCAL_XLOW; _tsAdcMaxY = TOUCSRCAL_YHIGH;
			}
		#endif
    break;
	case 2:
		//180
		_portrait = false;
		_scanDirection(1,1);
		#if defined(USE_RA8875_TOUCH)
			if (!_calibrated) {
				_tsAdcMinX = 1023; _tsAdcMinY = 1023; _tsAdcMaxX = 0; _tsAdcMaxY = 0;
			} else {
				_tsAdcMinX = TOUCSRCAL_XHIGH; _tsAdcMinY = TOUCSRCAL_YHIGH; _tsAdcMaxX = TOUCSRCAL_XLOW; _tsAdcMaxY = TOUCSRCAL_YLOW;
			}
		#endif
    break;
	case 3:
		//270
		_portrait = true;
		_scanDirection(0,1);
		#if defined(USE_RA8875_TOUCH)
			if (!_calibrated) {
				_tsAdcMinX = 0; _tsAdcMinY = 1023; _tsAdcMaxX = 1023; _tsAdcMaxY = 0;
			} else {
				_tsAdcMinX = TOUCSRCAL_XLOW; _tsAdcMinY = TOUCSRCAL_YHIGH; _tsAdcMaxX = TOUCSRCAL_XHIGH; _tsAdcMaxY = TOUCSRCAL_YLOW;
			}
		#endif
    break;
	}
	if (_portrait){
		_width = RA8875_HEIGHT;
		_height = RA8875_WIDTH;
		_FNCR1_Reg |= (1 << 4);
	} else {
		_width = RA8875_WIDTH;
		_height = RA8875_HEIGHT;
		_FNCR1_Reg &= ~(1 << 4);
	}
	_writeRegister(RA8875_FNCR1,_FNCR1_Reg);//0.69b21
	setActiveWindow();
}

/**************************************************************************/
/*!
      Get rotation setting
*/
/**************************************************************************/
uint8_t RA8875::getRotation()
{
	return _rotation;
}

/**************************************************************************/
/*!
      true if rotation 1 or 3
*/
/**************************************************************************/
boolean RA8875::isPortrait(void)
{
	return _portrait;
}


/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+								COLOR STUFF											 +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

/**************************************************************************/
/*!
      Set the display 'Color Space'
	  Parameters:
	  Bit per Pixel color (colors): 8 or 16 bit
	  NOTE:
	  For display over 272*480 give the ability to use
	  Layers since at 16 bit it's not possible.
*/
/**************************************************************************/
void RA8875::setColorBpp(uint8_t colors)
{
	if (colors != _color_bpp){//only if necessary
		if (colors < 16) {
			_color_bpp = 8;
			_colorIndex = 3;
			_writeRegister(RA8875_SYSR,0x00);
			if (_hasLayerLimits) _maxLayers = 2;
		} else if (colors > 8) {//65K
			_color_bpp = 16;
			_colorIndex = 0;
			_writeRegister(RA8875_SYSR,0x0C);
			if (_hasLayerLimits) _maxLayers = 1;
			_currentLayer = 0;
		}
	}
}

/**************************************************************************/
/*!
      Return current Color Space (8 or 16)
*/
/**************************************************************************/
uint8_t RA8875::getColorBpp(void)
{
	return _color_bpp;
}

/**************************************************************************/
/*!
      Sets set the foreground color using 16bit RGB565 color
	  It handles automatically color conversion when in 8 bit!
	  Parameters:
	  color: 16bit color RGB565
*/
/**************************************************************************/
void RA8875::setForegroundColor(uint16_t color)
{
	_foreColor = color;//keep track
	_writeRegister(RA8875_FGCR0,((color & 0xF800) >> _RA8875colorMask[_colorIndex]));
	_writeRegister(RA8875_FGCR0+1,((color & 0x07E0) >> _RA8875colorMask[_colorIndex+1]));
	_writeRegister(RA8875_FGCR0+2,((color & 0x001F) >> _RA8875colorMask[_colorIndex+2]));
}
/**************************************************************************/
/*!
      Sets set the foreground color using 8bit R,G,B
	  Parameters:
	  R: 8bit RED
	  G: 8bit GREEN
	  B: 8bit BLUE
*/
/**************************************************************************/
void RA8875::setForegroundColor(uint8_t R,uint8_t G,uint8_t B)
{
	_foreColor = Color565(R,G,B);//keep track
	_writeRegister(RA8875_FGCR0,R);
	_writeRegister(RA8875_FGCR0+1,G);
	_writeRegister(RA8875_FGCR0+2,B);
}
/**************************************************************************/
/*!
      Sets set the background color using 16bit RGB565 color
	  It handles automatically color conversion when in 8 bit!
	  Parameters:
	  color: 16bit color RGB565
	  Note: will set background Trasparency OFF
*/
/**************************************************************************/
void RA8875::setBackgroundColor(uint16_t color)
{
	_backColor = color;//keep track
	_writeRegister(RA8875_BGCR0,((color & 0xF800) >> _RA8875colorMask[_colorIndex]));//11
	_writeRegister(RA8875_BGCR0+1,((color & 0x07E0) >> _RA8875colorMask[_colorIndex+1]));//5
	_writeRegister(RA8875_BGCR0+2,((color & 0x001F) >> _RA8875colorMask[_colorIndex+2]));//0
}
/**************************************************************************/
/*!
      Sets set the background color using 8bit R,G,B
	  Parameters:
	  R: 8bit RED
	  G: 8bit GREEN
	  B: 8bit BLUE
	  Note: will set background Trasparency OFF
*/
/**************************************************************************/
void RA8875::setBackgroundColor(uint8_t R,uint8_t G,uint8_t B)
{
	_backColor = Color565(R,G,B);//keep track
	_writeRegister(RA8875_BGCR0,R);
	_writeRegister(RA8875_BGCR0+1,G);
	_writeRegister(RA8875_BGCR0+2,B);
}
/**************************************************************************/
/*!
      Sets set the trasparent background color using 16bit RGB565 color
	  It handles automatically color conversion when in 8 bit!
	  Parameters:
	  color: 16bit color RGB565
	  Note: will set background Trasparency ON
*/
/**************************************************************************/
void RA8875::setTransparentColor(uint16_t color)
{
	_backColor = color;
	_writeRegister(RA8875_BGTR0,((color & 0xF800) >> _RA8875colorMask[_colorIndex]));
	_writeRegister(RA8875_BGTR0+1,((color & 0x07E0) >> _RA8875colorMask[_colorIndex+1]));
	_writeRegister(RA8875_BGTR0+2,((color & 0x001F) >> _RA8875colorMask[_colorIndex+2]));
}
/**************************************************************************/
/*!
      Sets set the Trasparent background color using 8bit R,G,B
	  Parameters:
	  R: 8bit RED
	  G: 8bit GREEN
	  B: 8bit BLUE
	  Note: will set background Trasparency ON
*/
/**************************************************************************/
void RA8875::setTransparentColor(uint8_t R,uint8_t G,uint8_t B)
{
	_backColor = Color565(R,G,B);//keep track
	_writeRegister(RA8875_BGTR0,R);
	_writeRegister(RA8875_BGTR0+1,G);
	_writeRegister(RA8875_BGTR0+2,B);
}

/**************************************************************************/
/*!		
		set foreground,background color (plus transparent background)
		Parameters:
		fColor: 16bit foreground color (text) RGB565
		bColor: 16bit background color RGB565
		backTransp:if true the bColor will be transparent
*/
/**************************************************************************/
void RA8875::setColor(uint16_t fcolor,uint16_t bcolor,bool bcolorTraspFlag)//0.69b30
{
	if (fcolor != _foreColor) setForegroundColor(fcolor);
	if (bcolorTraspFlag){
		setTransparentColor(bcolor);
	} else {
		if (bcolor != _backColor) setBackgroundColor(bcolor);
	}
}

//draw a bit map
void RA8875::drawBMP(const uint16_t& a_x, const uint16_t& a_y, const uint16_t& a_width, const uint16_t& a_height, const uint16_t *a_array, const int8_t& a_scale) {
	writeToBlock(a_x, a_y, a_array, a_width, a_height, a_scale); 
}

void RA8875::drawXBMP(const uint16_t& a_x, const uint16_t& a_y, const uint16_t& a_width, const uint16_t& a_height, const uint8_t* a_array, const uint16_t& a_arraySize, const uint16_t a_color, const uint16_t a_color2, const int8_t& a_scale) {
	writeToBlock(a_x, a_y, a_array, a_arraySize, a_width, a_height, a_color, a_color2, a_scale); 
}

// draw a xbmp using the least amount of rects possible (checks for matching rows)
void RA8875::drawXbmpArray(const uint8_t* a_array, const uint16_t& a_arraySize, const uint16_t& a_x, const uint16_t& a_y,
									 const uint16_t& a_width, const uint16_t& a_height, const uint16_t& a_color, const uint16_t& a_scale) {
	if ((a_width * a_height / 8) > a_arraySize)
		return;
	uint8_t lineBuffer[a_width] {};
	for (uint16_t h = 0; h < a_height; h++) {	
		uint16_t bufferPos = 0, searchPos = 0;
		bool isEmptySpace = true, matchFound = true;
		// load current line into a buffer 
		for (uint16_t w = 0; w < a_width; w++) {
			uint16_t arrayPos = (w + 1 * (h * a_width)) / 8;
			uint8_t bitPos = 7 - (w + 1 * (h * a_width)) % 8;
			uint8_t arrayVal = a_array[arrayPos];
			uint8_t pixelVal = bitRead(arrayVal, bitPos);
			if (pixelVal)
				isEmptySpace = false;
			lineBuffer[bufferPos++] = pixelVal;
		}
		// check if the lines below match the current line, if so increase the rect height x amount
		uint16_t drawY = a_y + (h * a_scale);
		uint16_t h2 = h, rectHeight = 1;
		while (h2 < a_height - 1) {
			for (uint16_t w = 0; w < a_width; w++) {
				uint16_t arrayPos = (w + 1 * ((h2 + 1) * a_width)) / 8;
				uint8_t bitPos = 7 - (w + 1 * ((h2 + 1) * a_width)) % 8;
				uint8_t arrayVal = a_array[arrayPos];
				uint8_t pixelVal = bitRead(arrayVal, bitPos);
				if (pixelVal != lineBuffer[searchPos++]) {
					matchFound = false;
					h2 = a_height;
					break;
				}
			}
			if (matchFound) {
				rectHeight++;
				searchPos = 0;
				h = ++h2;
			}
		}
		// work out the rect's width and height's and draw them
		if (!isEmptySpace) {
			int16_t startX = -1, endX = -1;
			for (uint16_t i = 0; i < a_width; i++) {
				uint8_t px = lineBuffer[i];
				if (px == 1) {
					if (startX == -1)
						endX = startX = i;
					else
						endX = i;
				}
				if (px != 1 || i == a_width - 1) {
					if (startX != -1) {
						drawFilledRectQuick(a_x + (startX * a_scale), drawY, ((endX - startX) + 1) * a_scale, rectHeight * a_scale, a_color);
						startX = -1;
					}
				}
			}
		}
	}
}
/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+								PRINT STUFF											 +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

void RA8875::setFont(const font_returnStruct *a_font) {
  m_currentFont = a_font;
  m_fontArraySize = a_font->arraySize;
  m_fontHeight = a_font->yMargin;
  m_xMargin = a_font->xMargin;
  m_startChar = a_font->startChar;
}

// PRINT CHAR
void RA8875::print(const int16_t& a_fontX, const int16_t& a_fontY, const char a_val) {
	m_fontStartX = m_fontX = a_fontX;
	m_fontY = a_fontY;
	printCharQuick(a_val);
}

void RA8875::println(const int16_t& a_fontX, const int16_t& a_fontY, const char a_val) {
	m_fontStartX = m_fontX = a_fontX;
	m_fontY = a_fontY;
	printCharQuick(a_val);
	m_fontX = m_xMargin;
	m_fontY += m_fontHeight;   
}

void RA8875::print(const char a_val) {
	printCharQuick(a_val);
}

void RA8875::println(const char a_val) {
	printCharQuick(a_val);    
	m_fontX = m_xMargin;
	m_fontY += m_fontHeight;
}

// PRINT CHAR ARRAY
void RA8875::print(const int16_t& a_fontX, const int16_t& a_fontY, const char *a_val = 0) {
	m_fontStartX = m_fontX = a_fontX;
	m_fontY = a_fontY;
	printChar(a_val);
}

void RA8875::println(const int16_t& a_fontX, const int16_t& a_fontY, const char *a_val = 0) {
	m_fontStartX = m_fontX = a_fontX;
	m_fontY = a_fontY;
	printChar(a_val);   
	m_fontX = m_xMargin;
	m_fontY += m_fontHeight;
}

void RA8875::print(const char *a_val = 0) {
	printChar(a_val);
}

void RA8875::println(const char *a_val = 0) {
	printChar(a_val);    
	m_fontX = m_xMargin;
	m_fontY += m_fontHeight;
}

// PRINT STRING
void RA8875::print(const int16_t& a_fontX, const int16_t& a_fontY, const String& a_val) {
	char t_buffer[a_val.length() + 1];
  a_val.toCharArray(t_buffer, a_val.length() + 1);
	print(a_fontX, a_fontY, t_buffer);
}

void RA8875::println(const int16_t& a_fontX, const int16_t& a_fontY, const String& a_val) {
	char t_buffer[a_val.length() + 1];
  a_val.toCharArray(t_buffer, a_val.length() + 1);
	println(a_fontX, a_fontY, t_buffer); 
}

void RA8875::print(const String& a_val) {
	char t_buffer[a_val.length() + 1];
  a_val.toCharArray(t_buffer, a_val.length() + 1);
	print(t_buffer);
}

void RA8875::println(const String& a_val) {
	char t_buffer[a_val.length() + 1];
  a_val.toCharArray(t_buffer, a_val.length() + 1);
	println(t_buffer);    
}

// PRINT FLASH STRING
void RA8875::print(const int16_t& a_fontX, const int16_t& a_fontY, const __FlashStringHelper* a_val) {
	m_fontStartX = m_fontX = a_fontX;
	m_fontY = a_fontY;
	printChar(a_val);
}

void RA8875::println(const int16_t& a_fontX, const int16_t& a_fontY, const __FlashStringHelper* a_val = 0) {
	m_fontStartX = m_fontX = a_fontX;
	m_fontY = a_fontY;
	printChar(a_val);
	m_fontY += m_fontHeight;
	m_fontX = m_xMargin;
}

void RA8875::print(const __FlashStringHelper* a_val = 0) {
	printChar(a_val);
}

void RA8875::println(const __FlashStringHelper* a_val = 0) {
	printChar(a_val);
	m_fontX = m_xMargin;
	m_fontY += m_fontHeight;   
}

// PRINT int64_t
void RA8875::print(const int16_t& a_fontX, const int16_t& a_fontY, const int64_t &a_val) {
	char t_buffer[19];
	ltoa(a_val, t_buffer, 10);
	print(a_fontX, a_fontY, t_buffer);
}

void RA8875::println(const int16_t& a_fontX, const int16_t& a_fontY, const int64_t &a_val) {
	char t_buffer[19];
	ltoa(a_val, t_buffer, 10);
	println(a_fontX, a_fontY, t_buffer);
}

void RA8875::print(const int64_t &a_val) {
	char t_buffer[19];
	ltoa(a_val, t_buffer, 10);
	printChar(t_buffer);
}

void RA8875::println(const int64_t &a_val) {
	char t_buffer[19];
	ltoa(a_val, t_buffer, 10);
	println(t_buffer);
}

// PRINT uint64_t
void RA8875::print(const int16_t& a_fontX, const int16_t& a_fontY, const uint64_t &a_val) {
	char t_buffer[19];
	ltoa(a_val, t_buffer, 10);
	print(a_fontX, a_fontY, t_buffer);
}

void RA8875::println(const int16_t& a_fontX, const int16_t& a_fontY, const uint64_t &a_val) {
	char t_buffer[19];
	ltoa(a_val, t_buffer, 10);
	println(a_fontX, a_fontY, t_buffer);
}

void RA8875::print(const uint64_t &a_val) {
	char t_buffer[19];
	ltoa(a_val, t_buffer, 10);
	printChar(t_buffer);
}

void RA8875::println(const uint64_t &a_val) {
	char t_buffer[19];
	ltoa(a_val, t_buffer, 10);
	println(t_buffer);
}

// PRINT int32_t
void RA8875::print(const int16_t& a_fontX, const int16_t& a_fontY, const int32_t &a_val) {
	char t_buffer[19];
	ltoa(a_val, t_buffer, 10);
	print(a_fontX, a_fontY, t_buffer);
}

void RA8875::println(const int16_t& a_fontX, const int16_t& a_fontY, const int32_t &a_val) {
	char t_buffer[19];
	ltoa(a_val, t_buffer, 10);
	println(a_fontX, a_fontY, t_buffer);
}

void RA8875::print(const int32_t &a_val) {
	char t_buffer[19];
	ltoa(a_val, t_buffer, 10);
	printChar(t_buffer);
}

void RA8875::println(const int32_t &a_val) {
	char t_buffer[19];
	ltoa(a_val, t_buffer, 10);
	println(t_buffer);
}

// PRINT uint32_t
void RA8875::print(const int16_t& a_fontX, const int16_t& a_fontY, const uint32_t &a_val) {
	char t_buffer[19];
	ltoa(a_val, t_buffer, 10);
	print(a_fontX, a_fontY, t_buffer);
}

void RA8875::println(const int16_t& a_fontX, const int16_t& a_fontY, const uint32_t &a_val) {
	char t_buffer[19];
	ltoa(a_val, t_buffer, 10);
	println(a_fontX, a_fontY, t_buffer);
}

void RA8875::print(const uint32_t &a_val) {
	char t_buffer[19];
	ltoa(a_val, t_buffer, 10);
	printChar(t_buffer);
}

void RA8875::println(const uint32_t &a_val) {
	char t_buffer[19];
	ltoa(a_val, t_buffer, 10);
	println(t_buffer);
}

// PRINT uint8_t - int16_t
void RA8875::print(const int16_t& a_fontX, const int16_t& a_fontY, const int16_t &a_val) {
	char t_buffer[19];
	itoa(a_val, t_buffer, 10);
	print(a_fontX, a_fontY, t_buffer);
}

void RA8875::println(const int16_t& a_fontX, const int16_t& a_fontY, const int16_t &a_val) {
	char t_buffer[19];
	itoa(a_val, t_buffer, 10);
	println(a_fontX, a_fontY, t_buffer);
}

void RA8875::print(const int16_t &a_val) {
	char t_buffer[19];
	itoa(a_val, t_buffer, 10);
	printChar(t_buffer);
}

void RA8875::println(const int16_t &a_val) {
	char t_buffer[19];
	itoa(a_val, t_buffer, 10);
	println(t_buffer);
}

// PRINT uint16_t
void RA8875::print(const int16_t& a_fontX, const int16_t& a_fontY, const uint16_t &a_val) {
	char t_buffer[19];
	itoa(a_val, t_buffer, 10);
	print(a_fontX, a_fontY, t_buffer);
}

void RA8875::println(const int16_t& a_fontX, const int16_t& a_fontY, const uint16_t &a_val) {
	char t_buffer[19];
	itoa(a_val, t_buffer, 10);
	println(a_fontX, a_fontY, t_buffer);
}

void RA8875::print(const uint16_t &a_val) {
	char t_buffer[19];
	itoa(a_val, t_buffer, 10);
	printChar(t_buffer);
}

void RA8875::println(const uint16_t &a_val) {
	char t_buffer[19];
	itoa(a_val, t_buffer, 10);
	println(t_buffer);
}

// PRINT FLOAT
void RA8875::print(const int16_t& a_fontX, const int16_t& a_fontY, const double &a_val, const uint8_t& a_precision) {
	char t_buffer[19];
	dtostrf(a_val, 0, a_precision, t_buffer);
	print(a_fontX, a_fontY, t_buffer);
}

void RA8875::println(const int16_t& a_fontX, const int16_t& a_fontY, const double &a_val, const uint8_t& a_precision) {
	char t_buffer[19];
	dtostrf(a_val, 0, a_precision, t_buffer);
	println(a_fontX, a_fontY, t_buffer);
}

void RA8875::print(const double &a_val, const uint8_t& a_precision) {
	char t_buffer[19];
	dtostrf(a_val, 0, a_precision, t_buffer);
	print(t_buffer);
}

void RA8875::println(const double &a_val, const uint8_t& a_precision) {
	char t_buffer[19];
	dtostrf(a_val, 0, a_precision, t_buffer);
	println(t_buffer);
}

void RA8875::printChar(const char *a_val) {
	int16_t t_charPos = 0;
	while (a_val[t_charPos] != '\0') {
		if (a_val[t_charPos] == '\n') {
			m_fontY += m_fontHeight;
			m_fontX = m_xMargin;
		}
		else {
			printCharQuick(a_val[t_charPos]);
		}
		t_charPos++;
	}
}

void RA8875::printChar(const __FlashStringHelper* a_val) {
	for (uint16_t i = 0; i < strlen_P((char *)a_val); i++) {
		char c = pgm_read_byte_near((char *)a_val + i);
		if (c == '\n') {
			m_fontY += m_fontHeight;
			m_fontX = m_xMargin;
		}
		else {
			printCharQuick(c);
		}
	}
}

void RA8875::printCharQuick(const char a_char) {
	if (a_char != 0) {
		if (a_char - m_startChar > 0 || (uint16_t)(a_char - m_startChar) < m_fontArraySize) {
			const uint8_t arrayPosition = a_char - m_startChar;
			const int16_t& t_charWidth = m_currentFont->fontArray[arrayPosition].image->width;
			const int16_t& t_charHeight = m_currentFont->fontArray[arrayPosition].image->height;
			const int16_t& t_arraySize = m_currentFont->fontArray[arrayPosition].image->arraySize;
			const int16_t& t_charYoffset = m_currentFont->fontArray[arrayPosition].image->yOffset;
			const uint8_t* arrayData = m_currentFont->fontArray[arrayPosition].image->data;
			if (a_char != ' ')
				drawFontChar(m_fontX, m_fontY, t_charYoffset, arrayData, t_arraySize, t_charWidth, t_charHeight);
			m_fontX += (t_charWidth * m_scale) + m_xMargin;
		}
		else {
			Serial.print(F("could not find char (shown in DEC) ")); Serial.println(a_char, DEC);
		}
	}
}

void RA8875::drawFontChar(const int16_t& a_fontX, const int16_t& a_fontY, const int16_t& a_charYoffset, const uint8_t* a_array, const int16_t& a_arraySize, const int16_t& a_charWidth, const int16_t& a_charHeight) {
	if (a_fontX + a_charWidth < 0 || a_fontY + a_charHeight + a_charYoffset < 0 || (uint16_t)(a_fontX + a_charWidth) > width() || (uint16_t)(a_fontY + a_charHeight + a_charYoffset) > height()) {
		if (!m_disableFontOverFlow)
			return;
		else {
			m_fontY += m_fontHeight;
			m_fontX = m_fontStartX;
		}
	}    
	drawXBMP(a_fontX, a_fontY + (a_charYoffset * m_scale), a_charWidth, a_charHeight, a_array, a_arraySize, m_fontColor, m_fontBackColor, m_scale);
	//drawXbmpArray(a_array, a_arraySize, a_fontX, a_fontY + (a_charYoffset * m_scale), a_charWidth, a_charHeight, m_fontColor, m_scale);    
}

void RA8875::disableFontOverFlow(const bool& a_option) {
	m_disableFontOverFlow = a_option;
}

// STRING WIDTH
uint16_t RA8875::getStringWidth(const int16_t &a_val) {
	char t_buffer[19];
	itoa(a_val, t_buffer, 10);
	return getStringWidth(t_buffer);
}

uint16_t RA8875::getStringWidth(const uint16_t &a_val) {
	char t_buffer[19];
	itoa(a_val, t_buffer, 10);
	return getStringWidth(t_buffer);
}

uint16_t RA8875::getStringWidth(const int32_t &a_val) {
	char t_buffer[19];
	ltoa(a_val, t_buffer, 10);
	return getStringWidth(t_buffer);
}

uint16_t RA8875::getStringWidth(const uint32_t &a_val) {
	char t_buffer[19];
	ltoa(a_val, t_buffer, 10);
	return getStringWidth(t_buffer);
}

uint16_t RA8875::getStringWidth(const double &a_val, const uint8_t& a_precision) {
	char t_buffer[19];
	dtostrf(a_val, 0, a_precision, t_buffer);
	return getStringWidth(t_buffer);
}

uint16_t RA8875::getStringWidth(const char& a_char) {
	uint16_t t_charWidth = 0;
	if (a_char != 0) {
		const uint8_t arrayPosition = a_char - m_startChar;
		t_charWidth += m_currentFont->fontArray[arrayPosition].image->width;
		t_charWidth += m_xMargin;
	}
	return t_charWidth * m_scale;
}

uint16_t RA8875::getStringWidth(const char *a_chars) {
	uint16_t t_charPos = 0;
	uint16_t t_charWidth = 0;
	while (a_chars[t_charPos] != '\0') {
		if (a_chars[t_charPos] != 0) {
			const uint8_t arrayPosition = a_chars[t_charPos] - m_startChar;
			t_charWidth += m_currentFont->fontArray[arrayPosition].image->width;
			t_charWidth += m_xMargin;
		}
		t_charPos++;
	}
	return t_charWidth * m_scale;
}

void RA8875::setTextColor(const uint16_t& a_fontColor, const uint16_t& a_fontBackColor) {
	m_fontColor = a_fontColor;
	m_fontBackColor = a_fontBackColor;
}

void RA8875::setTextColor(const uint16_t& a_fontColor) {
	m_fontColor = a_fontColor;
}

uint16_t RA8875::getTextColor() {
	return m_fontColor;
}

uint16_t RA8875::getTextBackColor() {
	return m_fontBackColor;
}

int16_t RA8875::getFontX(const bool& a_withXMargin) {
	if (a_withXMargin)
	  return m_fontX;
  else
	  return m_fontX - m_xMargin; 
}

int16_t RA8875::getFontY() {
	return m_fontY;
}

uint8_t RA8875::getLineHeight() {
	return m_fontHeight;
}

void RA8875::setFontScale(const uint16_t& a_scale) {
	m_scale = a_scale;
	if (m_scale < 1)
		m_scale = 1;
}

void RA8875::setCursor(const uint16_t& a_x, const uint16_t& a_y) {
	m_fontStartX = m_fontX = a_x;
	m_fontY = a_y;
}

void RA8875::setLineHeight(const uint8_t& a_lineHeight) {
	m_fontHeight = a_lineHeight;
}
/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+								DRAW STUFF											 +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

/**************************************************************************/
/*!		
		Set the position for Graphic Write
		Parameters:
		x: horizontal position
		y: vertical position
*/
/**************************************************************************/

void RA8875::setXY(int16_t x, int16_t y) 
{
	setX(x);
	setY(y);
}

/**************************************************************************/
/*!		
		Set the x position for Graphic Write
		Parameters:
		x: horizontal position
*/
/**************************************************************************/
void RA8875::setX(int16_t x) 
{
	if (x < 0) x = 0;
	if (_portrait){//fix 0.69b21
		if (x >= RA8875_HEIGHT) x = RA8875_HEIGHT-1;
		_writeRegister(RA8875_CURV0, x & 0xFF);
		_writeRegister(RA8875_CURV0+1, x >> 8);
	} else {
		if (x >= RA8875_WIDTH) x = RA8875_WIDTH-1;
		_writeRegister(RA8875_CURH0, x & 0xFF);
		_writeRegister(RA8875_CURH0+1, (x >> 8)); 
	}
}

/**************************************************************************/
/*!		
		Set the y position for Graphic Write
		Parameters:
		y: vertical position
*/
/**************************************************************************/
void RA8875::setY(int16_t y) 
{
	if (y < 0) y = 0;
	if (_portrait){//fix 0.69b21
		if (y >= RA8875_WIDTH) y = RA8875_WIDTH-1;
		_writeRegister(RA8875_CURH0, y & 0xFF);
		_writeRegister(RA8875_CURH0+1, (y >> 8)); 
	} else {
		if (y >= RA8875_HEIGHT) y = RA8875_HEIGHT-1;
		_writeRegister(RA8875_CURV0, y & 0xFF);
		_writeRegister(RA8875_CURV0+1, y >> 8);
	}
}

 
/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+								SCROLL STUFF											 +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
/**************************************************************************/
/*!     
        Sets the scroll mode. This is controlled by bits 6 and 7 of  
        REG[52h] Layer Transparency Register0 (LTPR0)
		Author: The Experimentalist
*/
/**************************************************************************/
void RA8875::setScrollMode(enum RA8875scrollMode mode)
{
    uint8_t temp = _readRegister(RA8875_LTPR0);
    temp &= 0x3F;            // Clear bits 6 and 7 to zero
    switch(mode){           // bit 7,6 of LTPR0
        case SIMULTANEOUS:  // 00b : Layer 1/2 scroll simultaneously.
            // Do nothing
        break;
        case LAYER1ONLY:        // 01b : Only Layer 1 scroll.
            temp |= 0x40;
        break;
        case LAYER2ONLY:        // 10b : Only Layer 2 scroll.
            temp |= 0x80;
        break;
        case BUFFERED:      	// 11b: Buffer scroll (using Layer 2 as scroll buffer)
            temp |= 0xC0;
        break;
        default:
            return;         	//do nothing
    }
    //TODO: Should this be conditional on multi layer?
    //if (_useMultiLayers) _writeRegister(RA8875_LTPR0,temp);
    //_writeRegister(RA8875_LTPR0,temp);
	_writeData(temp);
}

/**************************************************************************/
/*!		
		Define a window for perform scroll
		Parameters:
		XL: x window start left
		XR: x window end right
		YT: y window start top
		YB: y window end bottom

*/
/**************************************************************************/
void RA8875::setScrollWindow(int16_t XL,int16_t XR ,int16_t YT ,int16_t YB)
{
	if (_portrait){//0.69b22 (fixed)
		swapvals(XL,YT);
		swapvals(XR,YB);
	}
	
	_checkLimits_helper(XL,YT);
	_checkLimits_helper(XR,YB);
	
	_scrollXL = XL; _scrollXR = XR; _scrollYT = YT; _scrollYB = YB;
    _writeRegister(RA8875_HSSW0,(_scrollXL & 0xFF));
    _writeRegister(RA8875_HSSW0+1,(_scrollXL >> 8));
  
    _writeRegister(RA8875_HESW0,(_scrollXR & 0xFF));
    _writeRegister(RA8875_HESW0+1,(_scrollXR >> 8));   
    
    _writeRegister(RA8875_VSSW0,(_scrollYT & 0xFF));
    _writeRegister(RA8875_VSSW0+1,(_scrollYT >> 8));   
 
    _writeRegister(RA8875_VESW0,(_scrollYB & 0xFF));
    _writeRegister(RA8875_VESW0+1,(_scrollYB >> 8));
	delay(1);
}

/**************************************************************************/
/*!
		Perform the scroll

*/
/**************************************************************************/
void RA8875::scroll(int16_t x,int16_t y)
{ 
	if (_portrait) swapvals(x,y);
	//if (y > _scrollYB) y = _scrollYB;//??? mmmm... not sure
	if (_scrollXL == 0 && _scrollXR == 0 && _scrollYT == 0 && _scrollYB == 0){
		//do nothing, scroll window inactive
	} else {
		_writeRegister(RA8875_HOFS0,(x & 0xFF)); 
		_writeRegister(RA8875_HOFS1,(x >> 8));
 
		_writeRegister(RA8875_VOFS0,(y & 0xFF));
		_writeRegister(RA8875_VOFS1,(y >> 8));
	}
}	 

/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+								DMA STUFF											 +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
/**************************************************************************/
/*!

*/
/**************************************************************************/
void RA8875::DMA_blockModeSize(int16_t BWR,int16_t BHR,int16_t SPWR)
{
  	_writeRegister(RA8875_DTNR0,BWR & 0xFF);
  	_writeRegister(RA8875_BWR1,BWR >> 8);

  	_writeRegister(RA8875_DTNR1,BHR & 0xFF);
  	_writeRegister(RA8875_BHR1,BHR >> 8);

  	_writeRegister(RA8875_DTNR2,SPWR & 0xFF);
  	_writeRegister(RA8875_SPWR1,SPWR >> 8); 
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void RA8875::DMA_startAddress(unsigned long adrs)
{ 
  	_writeRegister(RA8875_SSAR0,adrs & 0xFF);
  	_writeRegister(RA8875_SSAR0+1,adrs >> 8);
	_writeRegister(RA8875_SSAR0+2,adrs >> 16);
  	//_writeRegister(0xB3,adrs >> 24);// not more in datasheet!
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void RA8875::DMA_enable(void)
{ 
	uint8_t temp = _readRegister(RA8875_DMACR);
	temp |= 0x01;
	_writeData(temp);
	_waitBusy(0x01);
}
/**************************************************************************/
/*! (STILL IN DEVELOP, please do not complain)
		Display an image stored in Flash RAM
		Note: you should have the optional FLASH Chip connected to RA8875!
		Note: You should store some image in that chip!
		Note: Never tried!!!!!!!

*/
/**************************************************************************/
void RA8875::drawFlashImage(int16_t x,int16_t y,int16_t w,int16_t h,uint8_t picnum)
{  
	if (_portrait){swapvals(x,y); swapvals(w,h);}//0.69b21 -have to check this, not verified
	_writeRegister(RA8875_SFCLR,0x00);
	_writeRegister(RA8875_SROC,0x87);
	_writeRegister(RA8875_DMACR,0x02);
	//setActiveWindow(0,_width-1,0,_height-1); 
	_checkLimits_helper(x,y);
	_checkLimits_helper(w,h);
	_portrait == true ? setXY(y,x) : setXY(x,y);
	
	DMA_startAddress(261120 * (picnum-1));
	DMA_blockModeSize(w,h,w);   
	_writeRegister(RA8875_DMACR,0x03);
	_waitBusy(0x01);
} 


/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+								BTE STUFF											 +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

/**************************************************************************/
/* 
		Block Transfer Move
		Can move a rectangular block from any area of memory (eg. layer 1) to any other (eg layer 2)
		Can move with transparency - note THE TRANSPARENT COLOUR IS THE TEXT FOREGROUND COLOUR
		ReverseDir is for moving overlapping areas - may need to use reverse to prevent it double-copying the overlapping area (this option not available with transparency or monochrome)
		ROP is Raster Operation. Usually use RA8875_ROP_SOURCE but a few others are defined
		Defaults to current layer if not given or layer is zero.
		Monochrome uses the colour-expansion mode: the input is a bit map which is then converted to the current foreground and background colours, transparent background is optional
		Monochrome data is assumed to be linear, originally written to the screen memory in 16-bit chunks with drawPixels().
		Monochrome mode uses the ROP to define the offset of the first image bit within the first byte. This also depends on the width of the block you are trying to display.
		Monochrome skips 16-bit words in the input pattern - see the example for more explanation and a trick to interleave 2 characters in the space of one.

		This function returns immediately but the actual transfer can take some time
		Caller should check the busy status before issuing any more RS8875 commands.

		Basic usage:
		BTE_Move(SourceX, SourceY, Width, Height, DestX, DestY) = copy something visible on the current layer
		BTE_Move(SourceX, SourceY, Width, Height, DestX, DestY, 2) = copy something from layer 2 to the current layer
		BTE_Move(SourceX, SourceY, Width, Height, DestX, DestY, 2, 1, true) = copy from layer 2 to layer 1, with the transparency option
		BTE_Move(SourceX, SourceY, Width, Height, DestX, DestY, 0, 0, true, RA8875_BTEROP_ADD) = copy on the current layer, using transparency and the ADD/brighter operation 
		BTE_Move(SourceX, SourceY, Width, Height, DestX, DestY, 0, 0, false, RA8875_BTEROP_SOURCE, false, true) = copy on the current layer using the reverse direction option for overlapping areas
*/

void  RA8875::BTE_move(int16_t SourceX, int16_t SourceY, int16_t Width, int16_t Height, int16_t DestX, int16_t DestY, uint8_t SourceLayer, uint8_t DestLayer,bool Transparent, uint8_t ROP, bool Monochrome, bool ReverseDir)
{
	
	if (SourceLayer == 0) SourceLayer = _currentLayer;	
	if (DestLayer == 0) DestLayer = _currentLayer;
	if (SourceLayer == 2) SourceY |= 0x8000; //set the high bit of the vertical coordinate to indicate layer 2
	if (DestLayer == 2) DestY |= 0x8000; //set the high bit of the vertical coordinate to indicate layer 2
	ROP &= 0xF0; //Ensure the lower bits of ROP are zero
	if (Transparent) {
		if (Monochrome) {
			ROP |= 0x0A; //colour-expand transparent
		} else {
			ROP |= 0x05; //set the transparency option 
		}
	} else {
		if (Monochrome) {
			ROP |= 0x0B; //colour-expand normal
		} else {
			if (ReverseDir) {
				ROP |= 0x03; //set the reverse option
			} else {
				ROP |= 0x02; //standard block-move operation
			}
		}
	}

	_waitBusy(0x40); //Check that another BTE operation is not still in progress
	BTE_moveFrom(SourceX,SourceY);
	BTE_size(Width,Height);
	BTE_moveTo(DestX,DestY);
	BTE_ropcode(ROP);
	//Execute BTE! (This selects linear addressing mode for the monochrome source data)
	if (Monochrome) _writeRegister(RA8875_BECR0, 0xC0); else _writeRegister(RA8875_BECR0, 0x80);
	_waitBusy(0x40);
	//we are supposed to wait for the thing to become unbusy
	//caller can call _waitBusy(0x40) to check the BTE busy status (except it's private)
}

/**************************************************************************/
/*! TESTING

*/
/**************************************************************************/
void RA8875::BTE_size(int16_t w, int16_t h)
{
	//0.69b21 -have to check this, not verified
	if (_portrait) swapvals(w,h);
    _writeRegister(RA8875_BEWR0,w & 0xFF);//BET area width literacy  
    _writeRegister(RA8875_BEWR0+1,w >> 8);//BET area width literacy	   
    _writeRegister(RA8875_BEHR0,h & 0xFF);//BET area height literacy
    _writeRegister(RA8875_BEHR0+1,h >> 8);//BET area height literacy	   
}	

/**************************************************************************/
/*!

*/
/**************************************************************************/

void RA8875::BTE_moveFrom(int16_t SX,int16_t SY)
{
	if (_portrait) swapvals(SX,SY);
	_writeRegister(RA8875_HSBE0,SX & 0xFF);
	_writeRegister(RA8875_HSBE0+1,SX >> 8);
	_writeRegister(RA8875_VSBE0,SY & 0xFF);
	_writeRegister(RA8875_VSBE0+1,SY >> 8);
}	

/**************************************************************************/
/*!

*/
/**************************************************************************/

void RA8875::BTE_moveTo(int16_t DX,int16_t DY)
{
	if (_portrait) swapvals(DX,DY);
	_writeRegister(RA8875_HDBE0,DX & 0xFF);
	_writeRegister(RA8875_HDBE0+1,DX >> 8);
	_writeRegister(RA8875_VDBE0,DY & 0xFF);
	_writeRegister(RA8875_VDBE0+1,DY >> 8);
}	

/**************************************************************************/
/*! TESTING
	Use a ROP code EFX
*/
/**************************************************************************/
void RA8875::BTE_ropcode(unsigned char setx)
{
    _writeRegister(RA8875_BECR1,setx);//BECR1	   
}

/**************************************************************************/
/*! TESTING
	Enable BTE transfer
*/
/**************************************************************************/
void RA8875::BTE_enable(bool on) 
{	
	uint8_t temp = _readRegister(RA8875_BECR0);
	on == true ? temp &= ~(1 << 7) : temp |= (1 << 7);
	_writeData(temp);
	//_writeRegister(RA8875_BECR0,temp);  
	_waitBusy(0x40);
}


/**************************************************************************/
/*! TESTING
	Select BTE mode (CONT (continuous) or RECT)
*/
/**************************************************************************/
void RA8875::BTE_dataMode(enum RA8875btedatam m) 
{	
	uint8_t temp = _readRegister(RA8875_BECR0);
	m == CONT ? temp &= ~(1 << 6) : temp |= (1 << 6);
	_writeData(temp);
	//_writeRegister(RA8875_BECR0,temp);  
}

/**************************************************************************/
/*! TESTING
	Select the BTE SOURCE or DEST layer (1 or 2)
*/
/**************************************************************************/

void RA8875::BTE_layer(enum RA8875btelayer sd,uint8_t l)
{
	uint8_t temp;
	sd == SOURCE ? temp = _readRegister(RA8875_VSBE0+1) : temp = _readRegister(RA8875_VDBE0+1);
	l == 1 ? temp &= ~(1 << 7) : temp |= (1 << 7);
	_writeData(temp);
	//_writeRegister(RA8875_VSBE1,temp);  
}



/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+								LAYER STUFF											 +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

/**************************************************************************/
/*!
		Instruct the RA8875 chip to use 2 layers
		If resolution bring to restrictions it will switch to 8 bit
		so you can always use layers.
		Parameters:
		on: true (enable multiple layers), false (disable)
      
*/
/**************************************************************************/
void RA8875::useLayers(boolean on) 
{
	if (_useMultiLayers == on) return; //no reason to do change that it's already as desidered.
	//bool clearBuffer = false;
	if (_hasLayerLimits && _color_bpp > 8) { //try to set up 8bit color space
		setColorBpp(8);
		_waitBusy();
		_maxLayers = 2;
	}
	if (on){
		_useMultiLayers = true;
		_DPCR_Reg |= (1 << 7);
		//clearBuffer = true;
		clearActiveWindow(true);
	} else {
		_useMultiLayers = false;
		_DPCR_Reg &= ~(1 << 7);
		clearActiveWindow(false);
		
	}
	
	_writeRegister(RA8875_DPCR,_DPCR_Reg);
	if (!_useMultiLayers && _color_bpp < 16) setColorBpp(16);//bring color back to 16
	/*
	if (clearBuffer) { 
		clearWindow(true);
		//for some reason if you switch to multilayer the layer 2 has garbage better clear
		//writeTo(L2);//switch to layer 2
		//clearMemory(false);//clear memory of layer 2
		//clearWindow(false);
		//writeTo(L1);//switch to layer 1
	}
	*/
}


/**************************************************************************/
/*!

      
*/
/**************************************************************************/
void RA8875::layerEffect(enum RA8875boolean efx)
{
	uint8_t	reg = 0b00000000;
	//reg &= ~(0x07);//clear bit 2,1,0
	if (!_useMultiLayers) useLayers(true);//turn on multiple layers if it's off
	switch(efx){//                       bit 2,1,0 of LTPR0
		case LAYER1: //only layer 1 visible  [000]
			//do nothing
		break;
		case LAYER2: //only layer 2 visible  [001]
			reg |= (1 << 0);
		break;
		case TRANSPARENT: //transparent mode [011]
			reg |= (1 << 0); reg |= (1 << 1);
		break;
		case LIGHTEN: //lighten-overlay mode [010]
			reg |= (1 << 1);
		break;
		case OR: //boolean OR mode           [100]
			reg |= (1 << 2);
		break;
		case AND: //boolean AND mode         [101]
			reg |= (1 << 0); reg |= (1 << 2);
		break;
		case FLOATING: //floating windows    [110]
			reg |= (1 << 1); reg |= (1 << 2);
		break;
		default:
			//do nothing
		break;
	}
	_writeRegister(RA8875_LTPR0,reg);
}

/**************************************************************************/
/*!

      
*/
/**************************************************************************/
void RA8875::layerTransparency(uint8_t layer1,uint8_t layer2)
{
	if (layer1 > 8) layer1 = 8;
	if (layer2 > 8) layer2 = 8;
	if (!_useMultiLayers) useLayers(true);//turn on multiple layers if it's off
	//if (_useMultiLayers) _writeRegister(RA8875_LTPR1, ((layer2 & 0x0F) << 4) | (layer1 & 0x0F));
	//uint8_t res = 0b00000000;//RA8875_LTPR1
	//reg &= ~(0x07);//clear bit 2,1,0
	_writeRegister(RA8875_LTPR1, ((layer2 & 0xF) << 4) | (layer1 & 0xF));
}


/**************************************************************************/
/*! return the current drawing layer. If layers are OFF, return 255

*/
/**************************************************************************/
uint8_t RA8875::getCurrentLayer(void)
{
	if (!_useMultiLayers) return 255;
	return _currentLayer;
}

/**************************************************************************/
/*! select pattern

*/
/**************************************************************************/
void RA8875::setPattern(uint8_t num, enum RA8875pattern p)
{
	uint8_t maxLoc;
	uint8_t temp = 0b00000000;
	if (p != P16X16) {
		maxLoc = 16;//at 8x8 max 16 locations
	} else {
		maxLoc = 4;//at 16x16 max 4 locations
		temp |= (1 << 7);
	}
	if (num > (maxLoc - 1)) num = maxLoc - 1;
	temp = temp | num;
	writeTo(PATTERN);
	_writeRegister(RA8875_PTNO,temp);
}

/**************************************************************************/
/*! write colors to memory block

*/
/**************************************************************************/
/*
void RA8875::writeToBlock(int16_t a_x, int16_t a_y, const uint16_t *a_data, uint16_t a_width, uint16_t a_height)
{
	setActiveWindow(a_x, a_x + a_width-1, a_y, a_y + a_height-1);
	setXY(a_x, a_y);	
	writeCommand(RA8875_MRWC);
	for (int i = 0; i < (a_width * a_height); i++) {
		writeData16(a_data[i]);
	}
	_waitBusy(0x80);
}
*/
void RA8875::writeToBlock(int16_t a_x, int16_t a_y, const uint16_t *a_data, uint16_t a_width, uint16_t a_height, uint16_t a_scale) {
	int xPos = 0, linesDrawn = 0, startI = 0;
	setActiveWindow(a_x, a_x + (a_width*a_scale)-1, a_y, a_y + (a_height*a_scale)-1);
	setXY(a_x, a_y);
    writeCommand(RA8875_MRWC);
    _startSend();
	SPI.transfer(RA8875_DATAWRITE);
	for (int i = 0; i < a_width * a_height; i++) {
		if (a_scale != 1 && xPos == 0 && linesDrawn == 0) {
			 //printf("i:%d\n", i);
			 startI = i;
		}
		for (int n = 0; n < a_scale; n++)
			SPI.transfer16(a_data[i]);
		if (a_scale != 1) {
			// draw line again
			if (xPos == a_width - 1 && linesDrawn != a_scale - 1) {
				linesDrawn++;
				i = startI;
				xPos = 0;
			}
			else if (xPos == a_width - 1) {
				linesDrawn = 0;
				xPos = 0;
			}
			else
				xPos++;
		}	
	}
	_endSend();
	setActiveWindow();
}
/**************************************************************************/
/*! write colors to memory block

*/
/**************************************************************************/
/**************************************************************************/
void RA8875::writeToBlock(int16_t a_x, int16_t a_y, const uint8_t* a_data, const uint16_t a_arraySize, const uint16_t a_width, const uint16_t a_height, const uint16_t a_foreground, const uint16_t a_background, const uint16_t a_scale)
{
	int xPos = 0, linesDrawn = 0, startI = 0, startB = 0, pixelsDrawn = 0;
	setActiveWindow(a_x, a_x + (a_width*a_scale)-1, a_y, a_y + (a_height*a_scale)-1);
	setXY(a_x, a_y);
    writeCommand(RA8875_MRWC);
    _startSend();
	SPI.transfer(RA8875_DATAWRITE);
	for (int i = 0; i < a_arraySize; i++) {
		for (int b = 7 ; b >= 0; b--) {
		    if (a_scale != 1 && xPos == 0 && linesDrawn == 0) {
				//printf("i:%d, b:%d\n", i, b);
				startI = i; 
				startB = b;
			}
			char data = pgm_read_byte_near(a_data+i);
			uint8_t val = bitRead(data, b);
			for (int n = 0; n < a_scale; n++) {
				pixelsDrawn++;
			    SPI.transfer16(val ? a_foreground : a_background);
				if (pixelsDrawn == (a_width*a_scale) * (a_height*a_scale)) {
					_endSend();
					setActiveWindow();
					return;
				}
			}
			if (a_scale != 1) {		
				if (xPos == a_width - 1 && linesDrawn != a_scale - 1) { // draw line again
					linesDrawn++;
					i = startI;
					b = startB + 1;
					xPos = 0;
				}
				else if (xPos == a_width - 1) {
					linesDrawn = 0;
					xPos = 0;
				}
				else
					xPos++;
			}
		}
	}
	_endSend();
	setActiveWindow();
}

void RA8875::writePattern(int16_t x,int16_t y,const uint8_t *data,uint8_t size,bool setAW)
{
	int16_t i;
	int16_t a,b,c,d;
	if (size < 8 || size > 16) return;
	if (setAW) getActiveWindow(a,b,c,d);
	setActiveWindow(x,x+size-1,y,y+size-1);
	setXY(x,y);
	
	writeCommand(RA8875_MRWC);
	for (i=0;i<(size*size);i++) {
		_writeData(data[i*2]);
		_writeData(data[i*2+1]);
		_waitBusy(0x80);
	}
	if (setAW) setActiveWindow(a,b,c,d);//set as it was before
}

/**************************************************************************/
/*! This is the most important function to write on:
	LAYERS
	CGRAM
	PATTERN
	CURSOR
	Parameter:
	d (L1, L2, CGRAM, PATTERN, CURSOR)
	When writing on layers 0 or 1, if the layers are not enable it will enable automatically
	If the display doesn't support layers, it will automatically switch to 8bit color
	Remember that when layers are ON you need to disable manually, once that only Layer 1 will be visible

*/
/**************************************************************************/
void RA8875::writeTo(enum RA8875writes d)
{
	uint8_t temp = _readRegister(RA8875_MWCR1);
	//bool trigMultilayer = false;
	switch(d){
		case L1:
			temp &= ~((1<<3) | (1<<2));// Clear bits 3 and 2
			temp &= ~(1 << 0); //clear bit 0
			_currentLayer = 0;
			//trigMultilayer = true;
			_writeData(temp);  
			if (!_useMultiLayers) useLayers(true);
		break;
		case L2:
			temp &= ~((1<<3) | (1<<2));// Clear bits 3 and 2
			temp |= (1 << 0); //bit set 0
			_currentLayer = 1;
			//trigMultilayer = true;
			_writeData(temp);  
			if (!_useMultiLayers) useLayers(true);
		break;
		case CGRAM: 
			temp &= ~(1 << 3); //clear bit 3
			temp |= (1 << 2); //bit set 2
			if (bitRead(_FNCR0_Reg,7)){//REG[0x21] bit7 must be 0
				_FNCR0_Reg &= ~(1 << 7); //clear bit 7
				_writeRegister(RA8875_FNCR0,_FNCR0_Reg);  
				_writeRegister(RA8875_MWCR1,temp);
			} else {
				_writeData(temp);  
			}
		break;
		case PATTERN:
			temp |= (1 << 3); //bit set 3
			temp |= (1 << 2); //bit set 2
			_writeData(temp);  
		break;
		case CURSOR:
			temp |= (1 << 3); //bit set 3
			temp &= ~(1 << 2); //clear bit 2
			_writeData(temp);  
		break;
		default:
		//break;
		return;
	}
	//if (trigMultilayer && !_useMultiLayers) useLayers(true);//turn on multiple layers if it's off
	//_writeRegister(RA8875_MWCR1,temp);   
}


/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+						GEOMETRIC PRIMITIVE  STUFF									 +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
/**************************************************************************/
/*!

*/
/**************************************************************************/
/* void RA8875::fillRect(void) {
	writeCommand(RA8875_DCR);
	_writeData(RA8875_DCR_LINESQUTRI_STOP | RA8875_DCR_DRAWSQUARE);
	_writeData(RA8875_DCR_LINESQUTRI_START | RA8875_DCR_FILL | RA8875_DCR_DRAWSQUARE);
}
 */
/**************************************************************************/
/*!
      Write a single pixel
	  Parameters:
	  x: horizontal pos
	  y: vertical pos
	  color: RGB565 color
	  NOTE:
	  In 8bit bpp RA8875 needs a 8bit color(332) and NOT a 16bit(565),
	  the routine deal with this...
*/
/**************************************************************************/
void RA8875::drawPixel(int16_t x, int16_t y, uint16_t color)
{
	//setXY(x,y);
	setXY(x,y);
	writeCommand(RA8875_MRWC);
	if (_color_bpp > 8){
		writeData16(color); 
	} else {//TOTEST:layer bug workaround for 8bit color!
		_writeData(_color16To8bpp(color)); 
	}
}

/**************************************************************************/
/*!
      Draw a series of pixels
	  Parameters:
	  p: an array of 16bit colors (pixels)
	  count: how many pixels
	  x: horizontal pos
	  y: vertical pos
	  NOTE:
	  In 8bit bpp RA8875 needs a 8bit color(332) and NOT a 16bit(565),
	  the routine deal with this...
*/
/**************************************************************************/
void RA8875::drawPixels(uint16_t p[], uint16_t count, int16_t x, int16_t y)
{
    //setXY(x,y);
	uint16_t temp = 0;
	uint16_t i;
	setXY(x,y);
    writeCommand(RA8875_MRWC);
    _startSend();
	//set data
	#if (defined(__AVR__) && defined(_FASTSSPORT)) || defined(SPARK)
		_spiwrite(RA8875_DATAWRITE);
	#else
		#if defined(SPI_HAS_TRANSACTION) && defined(__MKL26Z64__)	
			if (_altSPI){
				SPI1.transfer(RA8875_DATAWRITE);
			} else {
				SPI.transfer(RA8875_DATAWRITE);
			}
		#else
			SPI.transfer(RA8875_DATAWRITE);
		#endif
	#endif
	//the loop
	for (i=0;i<count;i++){
		if (_color_bpp < 16) {
			temp = _color16To8bpp(p[i]);//TOTEST:layer bug workaround for 8bit color!
		} else {
			temp = p[i];
		}
	#if !defined(ENERGIA) && !defined(___DUESTUFF) && ((ARDUINO >= 160) || (TEENSYDUINO > 121))
		#if defined(SPI_HAS_TRANSACTION) && defined(__MKL26Z64__)	
			if (_color_bpp > 8){
				if (_altSPI){
					SPI1.transfer16(temp);
				} else {
					SPI.transfer16(temp);
				}
			} else {//TOTEST:layer bug workaround for 8bit color!
				if (_altSPI){
					SPI1.transfer(temp);
				} else {
					SPI.transfer(temp & 0xFF);
				}
			}
		#else
			if (_color_bpp > 8){
				SPI.transfer16(temp);
			} else {//TOTEST:layer bug workaround for 8bit color!
				SPI.transfer(temp & 0xFF);
			}
		#endif
	#else
		#if defined(___DUESTUFF) && defined(SPI_DUE_MODE_EXTENDED)
			if (_color_bpp > 8){
				SPI.transfer(_cs, temp >> 8, SPI_CONTINUE); 
				SPI.transfer(_cs, temp & 0xFF, SPI_LAST);
			} else {//TOTEST:layer bug workaround for 8bit color!
				SPI.transfer(_cs, temp & 0xFF, SPI_LAST);
			}
		#else
			#if (defined(__AVR__) && defined(_FASTSSPORT)) || defined(SPARK)
				if (_color_bpp > 8){
					_spiwrite16(temp);
				} else {//TOTEST:layer bug workaround for 8bit color!
					_spiwrite(temp >> 8);
				}
			#else
				if (_color_bpp > 8){
					SPI.transfer(temp >> 8);
					SPI.transfer(temp & 0xFF);
				} else {//TOTEST:layer bug workaround for 8bit color!
					SPI.transfer(temp & 0xFF);
				}
			#endif
		#endif
	#endif
    }
    _endSend();
}


/**************************************************************************/
/*!
      Get a pixel color from screen
	  Parameters:
	  x: horizontal pos
	  y: vertical pos
*/
/**************************************************************************/
uint16_t RA8875::getPixel(int16_t x, int16_t y)
{
    uint16_t color;
    setXY(x,y);
    writeCommand(RA8875_MRWC);
	#if defined(_FASTCPU)
		_slowDownSPI(true);
	#endif
    _startSend();
	#if (defined(__AVR__) && defined(_FASTSSPORT)) || defined(SPARK)
		_spiwrite(RA8875_DATAREAD);
		_spiwrite(0x00);
	#else
		#if defined(SPI_HAS_TRANSACTION) && defined(__MKL26Z64__)	
			if (_altSPI){
				SPI1.transfer(RA8875_DATAREAD);
				SPI1.transfer(0x00);//first byte it's dummy
			} else {
				SPI.transfer(RA8875_DATAREAD);
				SPI.transfer(0x00);//first byte it's dummy
			}
		#else
			SPI.transfer(RA8875_DATAREAD);
			SPI.transfer(0x00);//first byte it's dummy
		#endif
	#endif
	#if !defined(___DUESTUFF) && ((ARDUINO >= 160) || (TEENSYDUINO > 121))
		#if defined(SPI_HAS_TRANSACTION) && defined(__MKL26Z64__)	
			if (_altSPI){
				color  = SPI1.transfer16(0x0);
			} else {
				color  = SPI.transfer16(0x0);
			}
		#else
			color  = SPI.transfer16(0x0);
		#endif
	#else
		#if defined(___DUESTUFF) && defined(SPI_DUE_MODE_EXTENDED)
			color  = SPI.transfer(_cs, 0x0, SPI_CONTINUE); 
			color |= (SPI.transfer(_cs, 0x0, SPI_LAST) << 8);
		#else
			#if (defined(__AVR__) && defined(_FASTSSPORT)) || defined(SPARK)
				color  = _spiread();
				color |= (_spiread() << 8);
			#else
				color  = SPI.transfer(0x0);
				color |= (SPI.transfer(0x0) << 8);
			#endif
		#endif
	#endif
	#if defined(_FASTCPU)
		_slowDownSPI(false);
	#endif
    _endSend();
    return color;
}

/**************************************************************************/
/*!
      Basic line draw
	  Parameters:
	  x0: horizontal start pos
	  y0: vertical start
	  x1: horizontal end pos
	  y1: vertical end pos
	  color: RGB565 color
	  NOTE:
	  Remember that this write from->to so: drawLine(0,0,2,0,RA8875_RED);
	  result a 3 pixel long! (0..1..2)
*/
/**************************************************************************/
void RA8875::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
	//if ((x0 == x1 && y0 == y1) || ((x1 - x0 == 1) && (y1 - y0 == 1))) {//NEW
	if ((x0 == x1 && y0 == y1)) {//Thanks MrTOM
		drawPixel(x0,y0,color);
		return;
	}
	//if ((x1 - x0 == 1) && (y1 - y0 == 1)) drawPixel(x0,y0,color);
	if (_portrait) { swapvals(x0,y0); swapvals(x1,y1);}

	if (color != _foreColor) setForegroundColor(color);//0.69b30 avoid 3 useless SPI calls
	
	_line_addressing(x0,y0,x1,y1);
	
	_writeRegister(RA8875_DCR,0x80);
	_waitPoll(RA8875_DCR, RA8875_DCR_LINESQUTRI_STATUS);
}

/**************************************************************************/
/*!
      Basic line by using Angle as parameter
	  Parameters:
	  x: horizontal start pos
	  y: vertical start
	  angle: the angle of the line
	  length: lenght of the line
	  color: RGB565 color
*/
/**************************************************************************/
void RA8875::drawLineAngle(int16_t x, int16_t y, int16_t angle, uint16_t length, uint16_t color,int offset)
{
	
	if (length < 2) {
		drawPixel(x,y,color);
	} else {
		length--;//n
		drawLine(
		x,
		y,
		//x + (length * _cosDeg_helper(angle + offset)),//_angle_offset
		x + round((length) * _cosDeg_helper(angle + offset)),//Thanks MrTom
		//y + (length * _sinDeg_helper(angle + offset)), 
		y + round((length) * _sinDeg_helper(angle + offset)),//Thanks MrTom
		color);
	}
}

/**************************************************************************/
/*!
      Basic line by using Angle as parameter
	  Parameters:
	  x: horizontal start pos
	  y: vertical start
	  angle: the angle of the line
	  start: where line start
	  length: lenght of the line
	  color: RGB565 color
*/
/**************************************************************************/
void RA8875::drawLineAngle(int16_t x, int16_t y, int16_t angle, uint16_t start, uint16_t length, uint16_t color,int offset)
{
	if (start - length < 2) {//NEW
		drawPixel(x,y,color);
	} else {
		length--;//n
		drawLine(
		//x + start * _cosDeg_helper(angle + offset),//_angle_offset
		x + round(start * _cosDeg_helper(angle + offset)),//Thanks MrTom
		//y + start * _sinDeg_helper(angle + offset),
		y + round(start * _sinDeg_helper(angle + offset)),//Thanks MrTom
		//x + (start + length) * _cosDeg_helper(angle + offset),
		x + round((start + length) * _cosDeg_helper(angle + offset)),//Thanks MrTom
		//y + (start + length) * _sinDeg_helper(angle + offset), 
		y + round((start + length) * _sinDeg_helper(angle + offset)), //Thanks MrTom
		color);
	}
}

void RA8875::roundGaugeTicker(uint16_t x, uint16_t y, uint16_t r, int from, int to, float dev,uint16_t color) 
{
  float dsec;
  int i;
  for (i = from; i <= to; i += 30) {
    dsec = i * (PI / 180);
    drawLine(
		x + (cos(dsec) * (r / dev)) + 1,
		y + (sin(dsec) * (r / dev)) + 1,
		x + (cos(dsec) * r) + 1,
		y + (sin(dsec) * r) + 1, 
		color);
  }
}

/**************************************************************************/
/*!
      for compatibility with popular Adafruit_GFX
	  draws a single vertical line
	  Parameters:
	  x: horizontal start
	  y: vertical start
	  h: height
	  color: RGB565 color
*/
/**************************************************************************/
void RA8875::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
	if (h < 1) h = 1;
	h < 2 ? drawPixel(x,y,color) : drawLine(x, y, x, (y+h)-1, color);
}

/**************************************************************************/
/*!
      for compatibility with popular Adafruit_GFX
	  draws a single orizontal line
	  Parameters:
	  x: horizontal start
	  y: vertical start
	  w: width
	  color: RGB565 color
*/
/**************************************************************************/
void RA8875::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
	if (w < 1) w = 1;
	w < 2 ? drawPixel(x,y,color) : drawLine(x, y, (w+x)-1, y, color);
}

/**************************************************************************/
/*!
	  draws a rectangle
	  Parameters:
	  x: horizontal start
	  y: vertical start
	  w: width
	  h: height
	  color: RGB565 color
*/
/**************************************************************************/
void RA8875::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	//RA8875 it's not out-of-range tolerant so this is a workaround
	if (w < 1 || h < 1) return;//it cannot be!
	if (w < 2 && h < 2){ //render as pixel
		drawPixel(x,y,color);
	} else {			 //render as rect
		_rect_helper(x,y,(w+x)-1,(h+y)-1,color,false);//thanks the experimentalist
	}
}

/**************************************************************************/
/*!
	  draws a FILLED rectangle
	  Parameters:
	  x: horizontal start
	  y: vertical start
	  w: width
	  h: height
	  color: RGB565 color
*/
/**************************************************************************/
void RA8875::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	//RA8875 it's not out-of-range tolerant so this is a workaround
	if (w < 1 || h < 1) return;//it cannot be!
	if (w < 2 && h < 2){ //render as pixel
		drawPixel(x,y,color);
	} else {			 //render as rect
		_rect_helper(x,y,(x+w)-1,(y+h)-1,color,true);//thanks the experimentalist
	}
}
/**************************************************************************/
/*!
	  calculate a grandient color
	  return a spectrum starting at blue to red (0...127)
*/
/**************************************************************************/
uint16_t RA8875::grandient(uint8_t val)
{
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	uint8_t q = val / 32;
	switch(q){
		case 0:
			r = 0; g = 2 * (val % 32); b = 31;
		break;
		case 1:
			r = 0; g = 63; b = 31 - (val % 32);
		break;
		case 2:
			r = val % 32; g = 63; b = 0;
		break;
		case 3:
			r = 31; g = 63 - 2 * (val % 32); b = 0;
		break;
	}
	return (r << 11) + (g << 5) + b;
}

/**************************************************************************/
/*!
	  interpolate 2 16bit colors
	  return a 16bit mixed color between the two
	  Parameters:
	  color1:
	  color2:
	  pos:0...div (mix percentage) (0:color1, div:color2)
	  div:divisions between color1 and color 2
*/
/**************************************************************************/
uint16_t RA8875::colorInterpolation(uint16_t color1,uint16_t color2,uint16_t pos,uint16_t div)
{
    if (pos == 0) return color1;
    if (pos >= div) return color2;
	uint8_t r1,g1,b1;
	Color565ToRGB(color1,r1,g1,b1);//split in r,g,b
	uint8_t r2,g2,b2;
	Color565ToRGB(color2,r2,g2,b2);//split in r,g,b
	return colorInterpolation(r1,g1,b1,r2,g2,b2,pos,div);
}

/**************************************************************************/
/*!
	  interpolate 2 r,g,b colors
	  return a 16bit mixed color between the two
	  Parameters:
	  r1.
	  g1:
	  b1:
	  r2:
	  g2:
	  b2:
	  pos:0...div (mix percentage) (0:color1, div:color2)
	  div:divisions between color1 and color 2
*/
/**************************************************************************/
uint16_t RA8875::colorInterpolation(uint8_t r1,uint8_t g1,uint8_t b1,uint8_t r2,uint8_t g2,uint8_t b2,uint16_t pos,uint16_t div)
{
    if (pos == 0) return Color565(r1,g1,b1);
    if (pos >= div) return Color565(r2,g2,b2);
	float pos2 = (float)pos/div;
	return Color565(
				(uint8_t)(((1.0 - pos2) * r1) + (pos2 * r2)),
				(uint8_t)((1.0 - pos2) * g1 + (pos2 * g2)),
				(uint8_t)(((1.0 - pos2) * b1) + (pos2 * b2))
	);
}
/**************************************************************************/
/*!
	  draws a dots filled area
	  Parameters:
	  x: horizontal origin
	  y: vertical origin
	  w: width
	  h: height
	  spacing: space between dots in pixels (min 2pix)
	  color: RGB565 color
*/
/**************************************************************************/
void RA8875::drawMesh(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t spacing, uint16_t color)
{
	if (spacing < 2) spacing = 2;
	if (((x + w) - 1) >= _width)  w = _width  - x;
	if (((y + h) - 1) >= _height) h = _height - y;
	
	int16_t n, m;

	if (w < x) {n = w; w = x; x = n;}
	if (h < y) {n = h; h = y; y = n;}
	for (m = y; m <= h; m += spacing) {
		for (n = x; n <= w; n += spacing) {
			drawPixel(n, m, color);
		}
	}
}
/**************************************************************************/
/*!
      Fill the ActiveWindow by using a specified RGB565 color
	  Parameters:
	  color: RGB565 color (default=BLACK)
*/
/**************************************************************************/
void RA8875::fillWindow(uint16_t color)
{  
	_line_addressing(0,0,RA8875_WIDTH-1, RA8875_HEIGHT-1);
	setForegroundColor(color);
	writeCommand(RA8875_DCR);
	_writeData(0xB0);
	_waitPoll(RA8875_DCR, RA8875_DCR_LINESQUTRI_STATUS);
}

/**************************************************************************/
/*!
      clearScreen it's different from fillWindow because it doesn't depends
	  from the active window settings so it will clear all the screen.
	  It should be used only when needed since it's slower than fillWindow.
	  parameter:
	  color: 16bit color (default=BLACK)
*/
/**************************************************************************/
void RA8875::clearScreen(uint16_t color)//0.69b24
{  
	setActiveWindow();
	fillWindow(color);
}

/**************************************************************************/
/*!
      Draw circle
	  Parameters:
      x0: The 0-based x location of the center of the circle
      y0: The 0-based y location of the center of the circle
      r: radius
      color: RGB565 color
*/
/**************************************************************************/
void RA8875::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
	_center_helper(x0,y0);
	if (r < 1) return;
	if (r < 2) {
		drawPixel(x0,y0,color);
		return;
	}
	_circle_helper(x0, y0, r, color, false);
}


/**************************************************************************/
/*!
      Draw filled circle
	  Parameters:
      x0: The 0-based x location of the center of the circle
      y0: The 0-based y location of the center of the circle
      r: radius
      color: RGB565 color
*/
/**************************************************************************/
/*
void RA8875::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
	_center_helper(x0,y0);
	if (r <= 0) return;
	_circle_helper(x0, y0, r, color, true);
}
*/

void RA8875::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
	_center_helper(x0,y0);
	if (r < 1) return;
	if (r == 1) {
		drawPixel(x0,y0,color);
		return;
	}
	_circle_helper(x0, y0, r, color, true);
}

/**************************************************************************/
/*!
      Draw a quadrilater by connecting 4 points
	  Parameters:
	  x0:
	  y0:
	  x1:
	  y1:
	  x2:
	  y2:
	  x3:
	  y3:
      color: RGB565 color
*/
/**************************************************************************/
void RA8875::drawQuad(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2,int16_t x3, int16_t y3, uint16_t color) 
{
	drawLine(x0, y0, x1, y1, color);//low 1
	drawLine(x1, y1, x2, y2, color);//high 1
	drawLine(x2, y2, x3, y3, color);//high 2
	drawLine(x3, y3, x0, y0, color);//low 2
}


/**************************************************************************/
/*!
      Draw a filled quadrilater by connecting 4 points
	  Parameters:
	  x0:
	  y0:
	  x1:
	  y1:
	  x2:
	  y2:
	  x3:
	  y3:
      color: RGB565 color
	  triangled: if true a full quad will be generated, false generate a low res quad (faster)
	  *NOTE: a bug in _triangle_helper create some problem, still fixing....
*/
/**************************************************************************/
void RA8875::fillQuad(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color, bool triangled) 
{
	  _triangle_helper(x0, y0, x1, y1, x2, y2, color,true);
	  if (triangled) _triangle_helper(x2, y2, x3, y3, x0, y0, color,true);
      _triangle_helper(x1, y1, x2, y2, x3, y3, color,true);
}

/**************************************************************************/
/*!
      Draw a polygon from a center
	  Parameters:
	  cx: x center of the polygon
	  cy: y center of the polygon
	  sides: how many sides (min 3)
	  diameter: diameter of the polygon
	  rot: angle rotation of the polygon
      color: RGB565 color
*/
/**************************************************************************/
void RA8875::drawPolygon(int16_t cx, int16_t cy, uint8_t sides, int16_t diameter, float rot, uint16_t color)
{ 
	_center_helper(cx,cy);
	sides = (sides > 2? sides : 3);
	float dtr = (PI / 180.0) + PI;
	float rads = 360.0 / sides;//points spacd equally
	uint8_t i;
	for (i = 0; i < sides; i++) { 
		drawLine(
			cx + (sin((i*rads + rot) * dtr) * diameter),
			cy + (cos((i*rads + rot) * dtr) * diameter),
			cx + (sin(((i+1)*rads + rot) * dtr) * diameter),
			cy + (cos(((i+1)*rads + rot) * dtr) * diameter),
			color);
	}
}


/**************************************************************************/
/*!
      ringMeter 
	  (adapted from Alan Senior (thanks man!))
	  it create a ring meter with a lot of personalizations,
	  it return the width of the gauge so you can use this value
	  for positioning other gauges near the one just created easily
	  Parameters:
	  val:  your value
	  minV: the minimum value possible
	  maxV: the max value possible
	  x:    the position on x axis
	  y:    the position on y axis
	  r:    the radius of the gauge (minimum 50)
	  units: a text that shows the units, if "none" all text will be avoided
	  scheme:0...7 or 16 bit color (not BLACK or WHITE)
	  0:red
	  1:green
	  2:blue
	  3:blue->red
	  4:green->red
	  5:red->green
	  6:red->green->blue
	  7:cyan->green->red
	  8:black->white linear interpolation
	  9:violet->yellow linear interpolation
	  or
      RGB565 color (not BLACK or WHITE)
	  backSegColor: the color of the segments not active (default BLACK)
	  angle:		90 -> 180 (the shape of the meter, 90:halfway, 180:full round, 150:default)
	  inc: 			5...20 (5:solid, 20:sparse divisions, default:10)
*/
/**************************************************************************/
void RA8875::ringMeter(int val, int minV, int maxV, int16_t x, int16_t y, uint16_t r, const char* units, uint16_t colorScheme,uint16_t backSegColor,int16_t angle,uint8_t inc)
{
	if (inc < 5) inc = 5;
	if (inc > 20) inc = 20;
	if (r < 50) r = 50;
	if (angle < 90) angle = 90;
	if (angle > 180) angle = 180;
	int curAngle = map(val, minV, maxV, -angle, angle);
	uint16_t colour;
	x += r;
	y += r;   // Calculate coords of centre of ring
	uint16_t w = r / 4;    // Width of outer ring is 1/4 of radius
	const uint8_t seg = 5; // Segments are 5 degrees wide = 60 segments for 300 degrees
	// Draw colour blocks every inc degrees
	for (int16_t i = -angle; i < angle; i += inc) {
		colour = RA8875_BLACK;
		switch (colorScheme) {
			case 0:
				colour = RA8875_RED;
				break; // Fixed colour
			case 1:
				colour = RA8875_GREEN;
				break; // Fixed colour
			case 2:
				colour = RA8875_BLUE;
				break; // Fixed colour
			case 3:
				colour = grandient(map(i, -angle, angle, 0, 127));
				break; // Full spectrum blue to red
			case 4:
				colour = grandient(map(i, -angle, angle, 63, 127));
				break; // Green to red (high temperature etc)
			case 5:
				colour = grandient(map(i, -angle, angle, 127, 63));
				break; // Red to green (low battery etc)
			case 6:
				colour = grandient(map(i, -angle, angle, 127, 0));
				break; // Red to blue (air cond reverse)
			case 7:
				colour = grandient(map(i, -angle, angle, 35, 127));
				break; // cyan to red 
			case 8:
				colour = colorInterpolation(0,0,0,255,255,255,map(i,-angle,angle,0,w),w);
				break; // black to white
			case 9:
				colour = colorInterpolation(0x80,0,0xC0,0xFF,0xFF,0,map(i,-angle,angle,0,w),w);
				break; // violet to yellow
			default:
				if (colorScheme > 9){
					colour = colorScheme;
				} else {
					colour = RA8875_BLUE;
				}
				break; // Fixed colour
		}
		// Calculate pair of coordinates for segment start
		float xStart = cos((i - 90) * 0.0174532925);
		float yStart = sin((i - 90) * 0.0174532925);
		uint16_t x0 = xStart * (r - w) + x;
		uint16_t y0 = yStart * (r - w) + y;
		uint16_t x1 = xStart * r + x;
		uint16_t y1 = yStart * r + y;

		// Calculate pair of coordinates for segment end
		float xEnd = cos((i + seg - 90) * 0.0174532925);
		float yEnd = sin((i + seg - 90) * 0.0174532925);
		int16_t x2 = xEnd * (r - w) + x;
		int16_t y2 = yEnd * (r - w) + y;
		int16_t x3 = xEnd * r + x;
		int16_t y3 = yEnd * r + y;

		if (i < curAngle) { // Fill in coloured segments with 2 triangles
			fillQuad(x0, y0, x1, y1, x2, y2, x3, y3, colour, false);
		} else {// Fill in blank segments
			fillQuad(x0, y0, x1, y1, x2, y2, x3, y3, backSegColor, false);
		}
	}

	// text
	if (strcmp(units, "none") != 0){
		//erase internal background
		if (angle > 90) {
			fillCircle(x, y, r - w, _backColor); 
		} else {
			//fillCurve(x, y + getFontHeight() / 2, r - w, r - w, 1, _backColor);
			//fillCurve(x, y + getFontHeight() / 2, r - w, r - w, 2, _backColor);
		}
		//prepare for write text
		if (r > 84) {
			//setFontScale(1);
		} else {
			//setFontScale(0);
		}
		if (_portrait){
		//	setCursor(y, x - 15, true);
		} else {
		//	setCursor(x - 15, y, true);
		}
		//print(val);
		//print(" ");
		//print(units);
	}

	// Calculate and return right hand side x coordinate
	//return x + r;
	
}

/**************************************************************************/
/*!
      Draw Triangle
	  Parameters:
      x0: The 0-based x location of the point 0 of the triangle bottom LEFT
      y0: The 0-based y location of the point 0 of the triangle bottom LEFT
      x1: The 0-based x location of the point 1 of the triangle middle TOP
      y1: The 0-based y location of the point 1 of the triangle middle TOP
      x2: The 0-based x location of the point 2 of the triangle bottom RIGHT
      y2: The 0-based y location of the point 2 of the triangle bottom RIGHT
      color: RGB565 color
*/
/**************************************************************************/
void RA8875::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
	_triangle_helper(x0, y0, x1, y1, x2, y2, color, false);
}

/**************************************************************************/
/*!
      Draw filled Triangle
	  Parameters:
      x0: The 0-based x location of the point 0 of the triangle
      y0: The 0-based y location of the point 0 of the triangle
      x1: The 0-based x location of the point 1 of the triangle
      y1: The 0-based y location of the point 1 of the triangle
      x2: The 0-based x location of the point 2 of the triangle
      y2: The 0-based y location of the point 2 of the triangle
      color: RGB565 color
*/
/**************************************************************************/
void RA8875::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
	_triangle_helper(x0, y0, x1, y1, x2, y2, color, true);
}

/**************************************************************************/
/*!
      Draw an ellipse
	  Parameters:
      xCenter:   x location of the center of the ellipse
      yCenter:   y location of the center of the ellipse
      longAxis:  Size in pixels of the long axis
      shortAxis: Size in pixels of the short axis
      color: RGB565 color
*/
/**************************************************************************/
void RA8875::drawEllipse(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint16_t color)
{
	_ellipseCurve_helper(xCenter, yCenter, longAxis, shortAxis, 255, color, false);
}

/**************************************************************************/
/*!
      Draw a filled ellipse
	  Parameters:
      xCenter:   x location of the center of the ellipse
      yCenter:   y location of the center of the ellipse
      longAxis:  Size in pixels of the long axis
      shortAxis: Size in pixels of the short axis
      color: RGB565 color
*/
/**************************************************************************/
void RA8875::fillEllipse(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint16_t color)
{
	_ellipseCurve_helper(xCenter, yCenter, longAxis, shortAxis, 255, color, true);
}

/**************************************************************************/
/*!
      Draw a curve
      Parameters:
      xCenter:]   x location of the ellipse center
      yCenter:   y location of the ellipse center
      longAxis:  Size in pixels of the long axis
      shortAxis: Size in pixels of the short axis
      curvePart: Curve to draw in clock-wise dir: 0[180-270°],1[270-0°],2[0-90°],3[90-180°]
      color: RGB565 color
*/
/**************************************************************************/
void RA8875::drawCurve(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint8_t curvePart, uint16_t color)
{
	curvePart = curvePart % 4; //limit to the range 0-3
	if (_portrait) {//fix a problem with rotation
		if (curvePart == 0) {
			curvePart = 2;
		} else if (curvePart == 2) {
			curvePart = 0;
		}
	}
	_ellipseCurve_helper(xCenter, yCenter, longAxis, shortAxis, curvePart, color, false);
}

/**************************************************************************/
/*!
      Draw a filled curve
      Parameters:
      xCenter:]   x location of the ellipse center
      yCenter:   y location of the ellipse center
      longAxis:  Size in pixels of the long axis
      shortAxis: Size in pixels of the short axis
      curvePart: Curve to draw in clock-wise dir: 0[180-270°],1[270-0°],2[0-90°],3[90-180°]
      color: RGB565 color
*/
/**************************************************************************/
void RA8875::fillCurve(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint8_t curvePart, uint16_t color)
{
	curvePart = curvePart % 4; //limit to the range 0-3
	if (_portrait) {//fix a problem with rotation
		if (curvePart == 0) {
			curvePart = 2;
		} else if (curvePart == 2) {
			curvePart = 0;
		}
	}
	_ellipseCurve_helper(xCenter, yCenter, longAxis, shortAxis, curvePart, color, true);
}

/**************************************************************************/
/*!
      Draw a rounded rectangle
	  Parameters:
      x:   x location of the rectangle
      y:   y location of the rectangle
      w:  the width in pix
      h:  the height in pix
	  r:  the radius of the rounded corner
      color: RGB565 color
	  _roundRect_helper it's not tolerant to improper values
	  so there's some value check here
*/
/**************************************************************************/
void RA8875::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
	if (r == 0) drawRect(x,y,w,h,color);
	if (w < 1 || h < 1) return;//it cannot be!
	if (w < 2 && h < 2){ //render as pixel
		drawPixel(x,y,color);
	} else {			 //render as rect
		if (w < h && (r * 2) >= w) r = (w / 2) - 1;
		if (w > h && (r * 2) >= h) r = (h / 2) - 1;
		if (r == w || r == h) drawRect(x,y,w,h,color);
		_roundRect_helper(x, y, (x + w) - 1, (y + h) - 1, r, color, false);
	}
}


/**************************************************************************/
/*!
      Draw a filled rounded rectangle
	  Parameters:
      x:   x location of the rectangle
      y:   y location of the rectangle
      w:  the width in pix
      h:  the height in pix
	  r:  the radius of the rounded corner
      color: RGB565 color
*/
/**************************************************************************/
void RA8875::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
	if (r == 0) fillRect(x,y,w,h,color);
	if (w < 1 || h < 1) return;//it cannot be!
	if (w < 2 && h < 2){ //render as pixel
		drawPixel(x,y,color);
	} else {			 //render as rect
		if (w < h && (r * 2) >= w) r = (w / 2) - 1;
		if (w > h && (r  *2) >= h) r = (h / 2) - 1;
		if (r == w || r == h) drawRect(x,y,w,h,color);
		_roundRect_helper(x, y, (x + w) - 1, (y + h) - 1, r, color, true);
	}
}

/**************************************************************************/
/*!
      check area of a triangle
	  [private]
	  Thanks MrTom
*/
/**************************************************************************/
float RA8875::_check_area(int16_t Ax, int16_t Ay, int16_t Bx, int16_t By, int16_t Cx, int16_t Cy) {
	float area = abs(Ax * (By - Cy) + Bx * (Cy - Ay) + Cx * (Ay - By));     // Calc area
	float mag1 = sqrt((Bx - Ax) * (Bx - Ax) + (By - Ay) * (By - Ay));       // Calc side lengths
	float mag2 = sqrt((Cx - Ax) * (Cx - Ax) + (Cy - Ay) * (Cy - Ay));
	float mag3 = sqrt((Cx - Bx) * (Cx - Bx) + (Cy - By) * (Cy - By));
	float magmax = (mag1>mag2?mag1:mag2)>mag3?(mag1>mag2?mag1:mag2):mag3;   // Find largest length
	return area/magmax;                                                     // Return area
}

/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+					GEOMETRIC PRIMITIVE HELPERS STUFF								 +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
/**************************************************************************/
/*!
      helper function for circles
	  [private]
*/
/**************************************************************************/
void RA8875::_circle_helper(int16_t x0, int16_t y0, int16_t r, uint16_t color, bool filled)//0.69b32 fixed an undocumented hardware limit
{
	if (_portrait) swapvals(x0,y0);//0.69b21

	if (r < 1) r = 1;
	if (r < 2) {//NEW
		drawPixel(x0,y0,color);
		return;
	}
	if (r > RA8875_HEIGHT / 2) r = (RA8875_HEIGHT / 2) - 1;//this is the (undocumented) hardware limit of RA8875
	

	if (color != _foreColor) setForegroundColor(color);//0.69b30 avoid several SPI calls
	
	_writeRegister(RA8875_DCHR0,    x0 & 0xFF);
	_writeRegister(RA8875_DCHR0 + 1,x0 >> 8);

	_writeRegister(RA8875_DCVR0,    y0 & 0xFF);
	_writeRegister(RA8875_DCVR0 + 1,y0 >> 8);	   
	_writeRegister(RA8875_DCRR,r); 

	writeCommand(RA8875_DCR);
	#if defined(_FASTCPU)
		_slowDownSPI(true);
	#endif
	filled == true ? _writeData(RA8875_DCR_CIRCLE_START | RA8875_DCR_FILL) : _writeData(RA8875_DCR_CIRCLE_START | RA8875_DCR_NOFILL);
	_waitPoll(RA8875_DCR, RA8875_DCR_CIRCLE_STATUS);//ZzZzz
	#if defined(_FASTCPU)
		_slowDownSPI(false);
	#endif
}


/**************************************************************************/
/*!
		helper function for rects (filled or not)
		[private]
*/
/**************************************************************************/
/*
void RA8875::_rect_helper(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, bool filled)
{
	if (w < 0 || h < 0) return;//why draw invisible rects?(MrTOM temp fix)
	if (w >= _width) return;
	if (h >= _height) return;
	
	if (_portrait) {swapvals(x,y); swapvals(w,h);}

	_checkLimits_helper(x,y);


	if (color != _foreColor) setForegroundColor(color);
	
	_line_addressing(x,y,w,h);

	writeCommand(RA8875_DCR);
	filled == true ? _writeData(0xB0) : _writeData(0x90);
	_waitPoll(RA8875_DCR, RA8875_DCR_LINESQUTRI_STATUS);
}
*/

void RA8875::_rect_helper(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, bool filled)
{
	if (_portrait) {swapvals(x1,y1); swapvals(x2,y2);}
	if ((x1 < 0 && x2 < 0) || (x1 >= RA8875_WIDTH && x2 >= RA8875_WIDTH) ||
	    (y1 < 0 && y2 < 0) || (y1 >= RA8875_HEIGHT && y2 >= RA8875_HEIGHT))
		return;	// All points are out of bounds, don't draw anything

	_checkLimits_helper(x1,y1);	// Truncate rectangle that is off screen, still draw remaining rectangle
	_checkLimits_helper(x2,y2);

	if (color != _foreColor) setForegroundColor(color);
	
	if (x1==x2 && y1==y2)		// Width & height can still be 1 pixel, so render as a pixel
		drawPixel(x1,y1,color);
	else {
		_line_addressing(x1,y1,x2,y2);

		writeCommand(RA8875_DCR);
		filled == true ? _writeData(0xB0) : _writeData(0x90);
		_waitPoll(RA8875_DCR, RA8875_DCR_LINESQUTRI_STATUS);
	}
}


/**************************************************************************/
/*!
    Draw a filled rect quickly by skipping any out of bounds and size checks,
		assumes that all the variables passed in are within the correct ranges...
	  [public]
*/
/**************************************************************************/
void RA8875::drawFilledRectQuick(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
	if (color != _foreColor) 
		setForegroundColor(color);
	if (w==1 && h==1)		// Width & height can still be 1 pixel, so render as a pixel
		drawPixel(x, y, color);
	else {
		_line_addressing(x, y, (x+w)-1, (y+h)-1);
		writeCommand(RA8875_DCR);
		_writeData(0xB0);
		_waitPoll(RA8875_DCR, RA8875_DCR_LINESQUTRI_STATUS);
	}
}

/**************************************************************************/
/*!
      helper function for triangles
	  [private]
*/
/**************************************************************************/
void RA8875::_triangle_helper(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, bool filled)
{
	if (x0 >= _width || x1 >= _width || x2 >= _width) return;
	if (y0 >= _height || y1 >= _height || y2 >= _height) return;
	
	if (_portrait) {swapvals(x0,y0); swapvals(x1,y1); swapvals(x2,y2);}
	/*
	if (x0 == x1 && y0 == y1){
		drawLine(x0, y0, x2, y2,color);
		return;
	} else if (x0 == x2 && y0 == y2){
		drawLine(x0, y0, x1, y1,color);
		return;
	} else if (x0 == x1 && y0 == y1 && x0 == x2 && y0 == y2) {//new
        drawPixel(x0, y0, color);
		return;
	}
	*/
	/*
	if (y0 > y1) {swapvals(y0, y1); swapvals(x0, x1);}			// Sort points from Y < to >
	if (y1 > y2) {swapvals(y2, y1); swapvals(x2, x1);}
	if (y0 > y1) {swapvals(y0, y1); swapvals(x0, x1);}
	*/
/*	
	if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
		int16_t a, b;
        a = b = x0;
		if (x1 < a) {     
			a = x1;
		} else if (x1 > b) {
			b = x1;
		}
        if (x2 < a) { 
			a = x2;
		} else if (x2 > b) {
			b = x2;
		}
        drawFastHLine(a, y0, b-a+1, color);
        return;
    }
*/	

	// Avoid drawing lines here due to hardware bug in certain circumstances when a
	// specific shape triangle is drawn after a line. This bug can still happen, but
	// at least the user has control over fixing it.
	// Not drawing a line here is slower, but drawing a non-filled "triangle" is
	// slightly faster than a filled "triangle".
	//
	// bug example: tft.drawLine(799,479, 750,50, RA8875_BLUE)
	//              tft.fillTriangle(480,152, 456,212, 215,410, RA8875_GREEN)
	// MrTom
	//
	if (x0 == x1 && y0 == y1 && x0 == x2 && y0 == y2) {			// All points are same
		drawPixel(x0,y0, color);
		return;
	} else if ((x0 == x1 && y0 == y1) || (x0 == x2 && y0 == y2) || (x1 == x2 && y1 == y2)){
		filled = false;									// Two points are same
	} else if (x0 == x1 && x0 == x2){
		filled = false;									// Vertical line
	} else if (y0 == y1 && y0 == y2){
		filled = false;									// Horizontal line
	}
	if (filled){
		if (_check_area(x0,y0, x1,y1, x2,y2) < 0.9) {
			filled = false;			// Draw non-filled triangle to avoid filled triangle bug when two vertices are close together.
		}
	}

	if (color != _foreColor) setForegroundColor(color);//0.69b30 avoid several SPI calls
	
	//_checkLimits_helper(x0,y0);
	//_checkLimits_helper(x1,y1);
	
	_line_addressing(x0,y0,x1,y1);
	//p2

	_writeRegister(RA8875_DTPH0,    x2 & 0xFF);
	_writeRegister(RA8875_DTPH0 + 1,x2 >> 8);
	_writeRegister(RA8875_DTPV0,    y2 & 0xFF);
	_writeRegister(RA8875_DTPV0 + 1,y2 >> 8);
	
	writeCommand(RA8875_DCR);
	filled == true ? _writeData(0xA1) : _writeData(0x81);
	
	_waitPoll(RA8875_DCR, RA8875_DCR_LINESQUTRI_STATUS);
}

/**************************************************************************/
/*!
      helper function for ellipse and curve
	  [private]
*/
/**************************************************************************/
void RA8875::_ellipseCurve_helper(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint8_t curvePart,uint16_t color, bool filled)
{
	_center_helper(xCenter,yCenter);//use CENTER?
	
	if (_portrait) {
		swapvals(xCenter,yCenter);
		swapvals(longAxis,shortAxis);
		if (longAxis > _height/2) longAxis = (_height / 2) - 1;
		if (shortAxis > _width/2) shortAxis = (_width / 2) - 1;
	} else {
		if (longAxis > _width/2) longAxis = (_width / 2) - 1;
		if (shortAxis > _height/2) shortAxis = (_height / 2) - 1;
	}
	if (longAxis == 1 && shortAxis == 1) {
		drawPixel(xCenter,yCenter,color);
		return;
	}
	_checkLimits_helper(xCenter,yCenter);
	
	if (color != _foreColor) setForegroundColor(color);
	
	_curve_addressing(xCenter,yCenter,longAxis,shortAxis);
	writeCommand(RA8875_ELLIPSE);
	
	if (curvePart != 255){
		curvePart = curvePart % 4; //limit to the range 0-3
		filled == true ? _writeData(0xD0 | (curvePart & 0x03)) : _writeData(0x90 | (curvePart & 0x03));
	} else {
		filled == true ? _writeData(0xC0) : _writeData(0x80);
	}
	_waitPoll(RA8875_ELLIPSE, RA8875_ELLIPSE_STATUS);
}



/**************************************************************************/
/*!
	  helper function for rounded Rects
	  PARAMETERS
	  x:
	  y:
	  w:
	  h:
	  r:
	  color:
	  filled:
	  [private]
*/
/**************************************************************************/
void RA8875::_roundRect_helper(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color, bool filled)
{
	if (_portrait) {swapvals(x,y); swapvals(w,h);}

	if (color != _foreColor) setForegroundColor(color);//0.69b30 avoid several SPI calls
	
	_line_addressing(x,y,w,h);

	_writeRegister(RA8875_ELL_A0,    r & 0xFF);
	_writeRegister(RA8875_ELL_A0 + 1,r >> 8);
	_writeRegister(RA8875_ELL_B0,    r & 0xFF);
	_writeRegister(RA8875_ELL_B0 + 1,r >> 8);

	writeCommand(RA8875_ELLIPSE);
	filled == true ? _writeData(0xE0) : _writeData(0xA0);
	_waitPoll(RA8875_ELLIPSE, RA8875_DCR_LINESQUTRI_STATUS);
}

/**************************************************************************/
/*!
	  helper function for draw arcs in degrees
	  DrawArc function thanks to Jnmattern and his Arc_2.0 (https://github.com/Jnmattern)
	  Adapted for DUE by Marek Buriak https://github.com/marekburiak/ILI9341_Due
	  Re-Adapted for this library by sumotoy
	  PARAMETERS
	  cx: center x
	  cy: center y
	  radius: the radius of the arc
	  thickness:
	  start: where arc start in degrees
	  end:	 where arc end in degrees
	  color:
	  [private]
*/
/**************************************************************************/
void RA8875::_drawArc_helper(uint16_t cx, uint16_t cy, uint16_t radius, uint16_t thickness, float start, float end, uint16_t color) {
	
	//_center_helper(cx,cy);//use CENTER?
	int16_t xmin = 65535, xmax = -32767, ymin = 32767, ymax = -32767;
	float cosStart, sinStart, cosEnd, sinEnd;
	float r, t;
	float startAngle, endAngle;

	startAngle = (start / _arcAngle_max) * 360;	// 252
	endAngle = (end / _arcAngle_max) * 360;		// 807

	while (startAngle < 0)   startAngle += 360;
	while (endAngle < 0)     endAngle += 360;
	while (startAngle > 360) startAngle -= 360;
	while (endAngle > 360)   endAngle -= 360;


	if (startAngle > endAngle) {
		_drawArc_helper(cx, cy, radius, thickness, ((startAngle) / (float)360) * _arcAngle_max, _arcAngle_max, color);
		_drawArc_helper(cx, cy, radius, thickness, 0, ((endAngle) / (float)360) * _arcAngle_max, color);
	} else {
		//if (_textMode) _setTextMode(false);//we are in text mode?
		// Calculate bounding box for the arc to be drawn
		cosStart = _cosDeg_helper(startAngle);
		sinStart = _sinDeg_helper(startAngle);
		cosEnd = _cosDeg_helper(endAngle);
		sinEnd = _sinDeg_helper(endAngle);

		r = radius;
		// Point 1: radius & startAngle
		t = r * cosStart;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinStart;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;

		// Point 2: radius & endAngle
		t = r * cosEnd;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinEnd;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;

		r = radius - thickness;
		// Point 3: radius-thickness & startAngle
		t = r * cosStart;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinStart;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;

		// Point 4: radius-thickness & endAngle
		t = r * cosEnd;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinEnd;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;
		// Corrections if arc crosses X or Y axis
		if ((startAngle < 90) && (endAngle > 90)) ymax = radius;
		if ((startAngle < 180) && (endAngle > 180)) xmin = -radius;
		if ((startAngle < 270) && (endAngle > 270)) ymin = -radius;

		// Slopes for the two sides of the arc
		float sslope = (float)cosStart / (float)sinStart;
		float eslope = (float)cosEnd / (float)sinEnd;
		if (endAngle == 360) eslope = -1000000;
		int ir2 = (radius - thickness) * (radius - thickness);
		int or2 = radius * radius;
		for (int x = xmin; x <= xmax; x++) {
			bool y1StartFound = false, y2StartFound = false;
			bool y1EndFound = false, y2EndSearching = false;
			int y1s = 0, y1e = 0, y2s = 0;//, y2e = 0;
			for (int y = ymin; y <= ymax; y++) {
				int x2 = x * x;
				int y2 = y * y;

				if (
					(x2 + y2 < or2 && x2 + y2 >= ir2) && (
					(y > 0 && startAngle < 180 && x <= y * sslope) ||
					(y < 0 && startAngle > 180 && x >= y * sslope) ||
					(y < 0 && startAngle <= 180) ||
					(y == 0 && startAngle <= 180 && x < 0) ||
					(y == 0 && startAngle == 0 && x > 0)
					) && (
					(y > 0 && endAngle < 180 && x >= y * eslope) ||
					(y < 0 && endAngle > 180 && x <= y * eslope) ||
					(y > 0 && endAngle >= 180) ||
					(y == 0 && endAngle >= 180 && x < 0) ||
					(y == 0 && startAngle == 0 && x > 0)))
				{
					if (!y1StartFound) {	//start of the higher line found
						y1StartFound = true;
						y1s = y;
					} else if (y1EndFound && !y2StartFound) {//start of the lower line found
						y2StartFound = true;
						y2s = y;
						y += y1e - y1s - 1;	// calculate the most probable end of the lower line (in most cases the length of lower line is equal to length of upper line), in the next loop we will validate if the end of line is really there
						if (y > ymax - 1) {// the most probable end of line 2 is beyond ymax so line 2 must be shorter, thus continue with pixel by pixel search
							y = y2s;	// reset y and continue with pixel by pixel search
							y2EndSearching = true;
						}
					} else if (y2StartFound && !y2EndSearching) {
						// we validated that the probable end of the lower line has a pixel, continue with pixel by pixel search, in most cases next loop with confirm the end of lower line as it will not find a valid pixel
						y2EndSearching = true;
					}
				} else {
					if (y1StartFound && !y1EndFound) {//higher line end found
						y1EndFound = true;
						y1e = y - 1;
						drawFastVLine(cx + x, cy + y1s, y - y1s, color);
						if (y < 0) {
							y = abs(y); // skip the empty middle
						}
						else
							break;
					} else if (y2StartFound) {
						if (y2EndSearching) {
							// we found the end of the lower line after pixel by pixel search
							drawFastVLine(cx + x, cy + y2s, y - y2s, color);
							y2EndSearching = false;
							break;
						} else {
							// the expected end of the lower line is not there so the lower line must be shorter
							y = y2s;	// put the y back to the lower line start and go pixel by pixel to find the end
							y2EndSearching = true;
						}
					}
				}
			}
			if (y1StartFound && !y1EndFound){
				y1e = ymax;
				drawFastVLine(cx + x, cy + y1s, y1e - y1s + 1, color);
			} else if (y2StartFound && y2EndSearching)	{// we found start of lower line but we are still searching for the end
														// which we haven't found in the loop so the last pixel in a column must be the end
				drawFastVLine(cx + x, cy + y2s, ymax - y2s + 1, color);
			}
		}
	}
}


/**************************************************************************/
/*!
		this update the RA8875 Active Window registers
		[private]
*/
/**************************************************************************/
void RA8875::_updateActiveWindow(bool full)
{
	if (full){
		// X
		_writeRegister(RA8875_HSAW0,    0x00);
		_writeRegister(RA8875_HSAW0 + 1,0x00);   
		_writeRegister(RA8875_HEAW0,    (RA8875_WIDTH) & 0xFF);
		_writeRegister(RA8875_HEAW0 + 1,(RA8875_WIDTH) >> 8);
		// Y 
		_writeRegister(RA8875_VSAW0,    0x00);
		_writeRegister(RA8875_VSAW0 + 1,0x00); 
		_writeRegister(RA8875_VEAW0,    (RA8875_HEIGHT) & 0xFF); 
		_writeRegister(RA8875_VEAW0 + 1,(RA8875_HEIGHT) >> 8);
	} else {
		// X
		_writeRegister(RA8875_HSAW0,    _activeWindowXL & 0xFF);
		_writeRegister(RA8875_HSAW0 + 1,_activeWindowXL >> 8);   
		_writeRegister(RA8875_HEAW0,    _activeWindowXR & 0xFF);
		_writeRegister(RA8875_HEAW0 + 1,_activeWindowXR >> 8);
		// Y 
		_writeRegister(RA8875_VSAW0,     _activeWindowYT & 0xFF);
		_writeRegister(RA8875_VSAW0 + 1,_activeWindowYT >> 8); 
		_writeRegister(RA8875_VEAW0,    _activeWindowYB & 0xFF); 
		_writeRegister(RA8875_VEAW0 + 1,_activeWindowYB >> 8);
	}
}

/**************************************************************************/
/*!
		Graphic line addressing helper
		[private]
*/
/**************************************************************************/
void RA8875::_line_addressing(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
	//X0
	_writeRegister(RA8875_DLHSR0,    x0 & 0xFF);
	_writeRegister(RA8875_DLHSR0 + 1,x0 >> 8);
	//Y0
	_writeRegister(RA8875_DLVSR0,    y0 & 0xFF);
	_writeRegister(RA8875_DLVSR0 + 1,y0 >> 8);
	//X1
	_writeRegister(RA8875_DLHER0,    x1 & 0xFF);
	_writeRegister(RA8875_DLHER0 + 1,x1 >> 8);
	//Y1
	_writeRegister(RA8875_DLVER0,    y1 & 0xFF);
	_writeRegister(RA8875_DLVER0 + 1,y1 >> 8);
}

/**************************************************************************/
/*!	
		curve addressing helper
		[private]
*/
/**************************************************************************/
void RA8875::_curve_addressing(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
	//center
	_writeRegister(RA8875_DEHR0,    x0 & 0xFF);
	_writeRegister(RA8875_DEHR0 + 1,x0 >> 8);
	_writeRegister(RA8875_DEVR0,    y0 & 0xFF);
	_writeRegister(RA8875_DEVR0 + 1,y0 >> 8);
	//long,short ax
	_writeRegister(RA8875_ELL_A0,    x1 & 0xFF);
	_writeRegister(RA8875_ELL_A0 + 1,x1 >> 8);
	_writeRegister(RA8875_ELL_B0,    y1 & 0xFF);
	_writeRegister(RA8875_ELL_B0 + 1,y1 >> 8);
}


/**************************************************************************/
/*!	
		sin e cos helpers
		[private]
*/
/**************************************************************************/
float RA8875::_cosDeg_helper(float angle)
{
	float radians = angle / (float)360 * 2 * PI;
	return cos(radians);
}

float RA8875::_sinDeg_helper(float angle)
{
	float radians = angle / (float)360 * 2 * PI;
	return sin(radians);
}

/**************************************************************************/
/*!	
		change the arc default parameters
*/
/**************************************************************************/
void RA8875::setArcParams(float arcAngleMax, int arcAngleOffset)
{
	_arcAngle_max = arcAngleMax;
	_arcAngle_offset = arcAngleOffset;
}

/**************************************************************************/
/*!	
		change the angle offset parameter from default one
*/
/**************************************************************************/
void RA8875::setAngleOffset(int16_t angleOffset)
{
	_angle_offset = ANGLE_OFFSET + angleOffset;
}


/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+								PWM STUFF											 +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

/**************************************************************************/
/*!
		on/off GPIO (basic for Adafruit module
*/
/**************************************************************************/
void RA8875::GPIOX(boolean on) 
{
    _writeRegister(RA8875_GPIOX, on);
}

/**************************************************************************/
/*!
		PWM out
		Parameters:
		pw: pwm selection (1,2)
		p: 0...255 rate
		
*/
/**************************************************************************/
void RA8875::PWMout(uint8_t pw,uint8_t p)
{
	uint8_t reg;
	pw > 1 ? reg = RA8875_P2DCR : reg = RA8875_P1DCR;
	_writeRegister(reg, p);
}

/**************************************************************************/
/*!
		Set the brightness of the backlight (if connected to pwm)
		(basic controls pwm 1)
		Parameters:
		val: 0...255
*/
/**************************************************************************/
void RA8875::brightness(uint8_t val) 
{
	_brightness = val;
	PWMout(1,_brightness);
}

/**************************************************************************/
/*!
		controls the backligh by using PWM engine.
		It handles adafruit board separately
		Parameters:
		on: true(backlight on), false(backlight off)
*/
/**************************************************************************/
void RA8875::backlight(boolean on) //0.69b31 (fixed an issue with adafruit backlight)
{
	if (_displaySize == Adafruit_480x272 || _displaySize == Adafruit_800x480) GPIOX(on);
	if (on == true){
		PWMsetup(1,true, RA8875_PWM_CLK_DIV1024);//setup PWM ch 1 for backlight
		PWMout(1,_brightness);//turn on PWM1
	} else {
		PWMsetup(1,false, RA8875_PWM_CLK_DIV1024);//setup PWM ch 1 for backlight
	}
}

/**************************************************************************/
/*!
		Setup PWM engine
		Parameters:
		pw: pwm selection (1,2)
		on: turn on/off
		clock: the clock setting
		[private]
*/
/**************************************************************************/
void RA8875::PWMsetup(uint8_t pw,boolean on, uint8_t clock) 
{
	uint8_t reg;
	uint8_t set;
	if (pw > 1){
		reg = RA8875_P2CR;
		on == true ? set = RA8875_PxCR_ENABLE : set = RA8875_PxCR_DISABLE;
	} else {
		reg = RA8875_P1CR;
		on == true ? set = RA8875_PxCR_ENABLE : set = RA8875_PxCR_DISABLE;
	}
	_writeRegister(reg,(set | (clock & 0xF)));
}

/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+								         ISR										 +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

void RA8875::setInternalInt(enum RA8875intlist b)
{
	_enabledInterrups |= (1 << b);//set
}

void RA8875::clearInternalInt(enum RA8875intlist b)
{
	_enabledInterrups &= ~(1 << b);//clear
}

/**************************************************************************/
/*!
	RA8875 can use the INT pin for many purposes.
	This function just tell the CPU to use a pin for listen to RA8875 pin,
	no matter if we will use ISR or DigitalRead
------------------------------
Bit:	Called by:		In use:
--------------------------------
0: 		isr triggered	[*]
1: 		Resistive TS	[*]
2: 		KeyScan			[*]
3: 		DMA				 	
4: 		BTE				 
5: 		-na-		
6: 		-na-			 
7: 		-na-			 
--------------------------------
	Parameters:
	INTpin: it's the pin where we listen to ISR
	INTnum: it's the INT related to pin. On some processor it's not needed
	This last parameter it's used only when decide to use an ISR.
*/
/**************************************************************************/

void RA8875::useINT(const uint8_t INTpin,const uint8_t INTnum) 
{
	_intPin = INTpin;
	_intNum = INTnum;
	#if defined(___TEENSYES)//all of them (32 bit only)
		pinMode(_intPin ,INPUT_PULLUP);
	#else
		pinMode(_intPin ,INPUT);
	#endif
}

/**************************************************************************/
/*!
		the generic ISR routine, will set to 1 bit 0 of _RA8875_INTS
		[private]
*/
/**************************************************************************/
void RA8875::_isr(void)
{
	_RA8875_INTS |= (1 << 0);//set
}

/**************************************************************************/
/*!
		Enable the ISR, after this any falling edge on
		_intPin pin will trigger ISR
		Parameters:
		force: if true will force attach interrupt
		NOTE:
		if parameter _needISRrearm = true will rearm interrupt
*/
/**************************************************************************/
void RA8875::enableISR(bool force) 
{
	if (force || _needISRrearm){
		_needISRrearm = false;
		#ifdef digitalPinToInterrupt
			attachInterrupt(digitalPinToInterrupt(_intPin),_isr,FALLING);
		#else
			attachInterrupt(_intNum,_isr,FALLING);
		#endif
		_RA8875_INTS = 0b00000000;//reset all INT bits flags
		_useISR = true;
	}
	#if defined(USE_RA8875_TOUCH)
		if (_touchEnabled) _checkInterrupt(2);//clear internal RA int to engage
	#endif
}

/**************************************************************************/
/*!
		Disable ISR
		Works only if previously enabled or do nothing.
*/
/**************************************************************************/
void RA8875::_disableISR(void) 
{
	if (_useISR){
		#if defined(USE_RA8875_TOUCH)
			if (_touchEnabled) _checkInterrupt(2);//clear internal RA int to engage
		#endif
		#ifdef digitalPinToInterrupt
			detachInterrupt(digitalPinToInterrupt(_intPin));
		#else
			detachInterrupt(_intNum);
		#endif
		_RA8875_INTS = 0b00000000;//reset all bits
		_useISR = false;
	}
}

/**************************************************************************/
/*!
		Check the [interrupt register] for an interrupt,
		if found it will reset it.
		bit
		0: complicated....
		1: BTE INT
		2: TOUCH INT
		3: DMA INT
		4: Keyscan INT
*/
/**************************************************************************/
bool RA8875::_checkInterrupt(uint8_t _bit,bool _clear) 
{
	if (_bit > 4) _bit = 4;
	uint8_t temp = _readRegister(RA8875_INTC2);
	if (bitRead(temp,_bit) == 1){
		if (_clear){
			temp |= (1 << _bit);//bitSet(temp,_bit);
			//if (bitRead(temp,0)) bitSet(temp,0);//Mmmmm...
			_writeRegister(RA8875_INTC2, temp);//clear int
		}
		return true;
	}
	return false;
}

#if defined(USE_FT5206_TOUCH)
/**************************************************************************/
/*!
	The FT5206 Capacitive Touch driver uses a different INT pin than RA8875
	and it's not controlled by RA chip of course, so we need a separate code
	for that purpose, no matter we decide to use an ISR or digitalRead.
	no matter if we will use ISR or DigitalRead
	Parameters:
	INTpin: it's the pin where we listen to ISR
	INTnum: it's the INT related to pin. On some processor it's not needed
	This last parameter it's used only when decide to use an ISR.
*/
/**************************************************************************/
void RA8875::useCapINT(const uint8_t INTpin,const uint8_t INTnum) 
{
	_intCTSPin = INTpin;
	_intCTSNum = INTnum;
	#if defined(___TEENSYES)//all of them (32 bit only)
		pinMode(_intCTSPin ,INPUT_PULLUP);
	#else
		pinMode(_intCTSPin ,INPUT);
	#endif
}

/**************************************************************************/
/*!
		Since FT5206 uses a different INT pin, we need a separate isr routine. 
		[private]
*/
/**************************************************************************/
void RA8875::cts_isr(void)
{
	_FT5206_INT = true;
}

/**************************************************************************/
/*!
		Enable the ISR, after this any falling edge on
		_intCTSPin pin will trigger ISR
		Parameters:
		force: if true will force attach interrup
		NOTE:
		if parameter _needCTS_ISRrearm = true will rearm interrupt
*/
/**************************************************************************/
void RA8875::enableCapISR(bool force) 
{
	if (force || _needCTS_ISRrearm){
		_needCTS_ISRrearm = false;
		#ifdef digitalPinToInterrupt
			attachInterrupt(digitalPinToInterrupt(_intCTSPin),cts_isr,FALLING);
		#else
			attachInterrupt(_intCTSNum,cts_isr,FALLING);
		#endif
		_FT5206_INT = false;
		_useISR = true;
	}
}

/**************************************************************************/
/*!
		Disable ISR [FT5206 version]
		Works only if previously enabled or do nothing.
*/
/**************************************************************************/
void RA8875::_disableCapISR(void) 
{
	if (_useISR){
		#ifdef digitalPinToInterrupt
			detachInterrupt(digitalPinToInterrupt(_intCTSPin));
		#else
			detachInterrupt(_intCTSNum);
		#endif
		_FT5206_INT = false;
		_useISR = false;
	}
}
#endif

/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+							     TOUCH SCREEN COMMONS						         +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
#if !defined(_AVOID_TOUCHSCREEN)//Common touch screend methods (for capacitive and resistive)

/**************************************************************************/
/*!
      Checks if a touch event has occured

      @return  True is a touch event has occured (reading it via
               touchRead() will clear the interrupt in memory)
*/
/**************************************************************************/
boolean RA8875::touched(void)
{
  if (_readRegister(RA8875_INTC2) & RA8875_INTCx_TP) return true;
  return false;
}


/**************************************************************************/
/*!
		Checks an interrupt has occurred. return true if yes.
		Designed to run in loop.
		It works with ISR or DigitalRead methods
		Parameters:
		safe: true  (detach interrupt routine, has to be re-engaged manually!)
			  false (
*/
/**************************************************************************/
// does not work for resistive touch need much more investigation to understand and get it to work
bool RA8875::touched(bool safe)
{
	if (_useISR){//using interrupts
		#if defined(USE_FT5206_TOUCH)
			_needCTS_ISRrearm = safe;
			if (_FT5206_INT)			{
		#elif defined(USE_RA8875_TOUCH)
			_needISRrearm = safe;
			if (bitRead(_RA8875_INTS,0)){
		#endif
			//there was an interrupt
				#if defined(USE_FT5206_TOUCH)
					if (_needCTS_ISRrearm){//safe was true, detach int
						_disableCapISR();
					#else
					if (_needISRrearm){//safe was true, detach int
						_disableISR();
					#endif
					} else {//safe was false, do not detatch int and clear INT flag
						#if defined(USE_FT5206_TOUCH)
							_FT5206_INT = false;
						#elif defined(USE_RA8875_TOUCH)
							_RA8875_INTS &= ~(1 << 0);//clear
							_checkInterrupt(2);//clear internal RA int to re-engage
						#endif
					}
					return true;
			}
			return false;
	} else {//not use ISR, digitalRead method
			#if defined(USE_FT5206_TOUCH)
			//TODO
			
			#elif defined(USE_RA8875_TOUCH)
				if (_touchEnabled){
					#if defined(___TEENSYES)
						if (!digitalReadFast(_intPin)) {
					#else
						if (!digitalRead(_intPin)) {
					#endif
							_clearTInt = true;
							if (_checkInterrupt(2)){
								return true;
							} else {
								return false;
							}
						}//digitalRead
					
						if (_clearTInt){
							_clearTInt = false;
							_checkInterrupt(2);//clear internal RA int
							delay(1);
						}
					
						return false;
					
				} else {//_touchEnabled
					return false;
				}
			#endif
			return false;
	}
}

void RA8875::setTouchLimit(uint8_t limit)
{
	#if defined(USE_FT5206_TOUCH)
	if (limit > 5) limit = 5;//max 5 allowed
	#else
		limit = 1;//always 1
	#endif
	_maxTouch = limit;
}

uint8_t RA8875::getTouchLimit(void)
{
	return _maxTouch;
}
/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+				CAPACITIVE TOUCH SCREEN CONTROLLER	FT5206						     +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
#if defined(USE_FT5206_TOUCH)


/**************************************************************************/
/*!
		Initialization sequence of the FT5206
*/
/**************************************************************************/
void RA8875::_initializeFT5206(void)
{
	uint8_t i;
	for (i=0x80;i<=0x89;i++){
		_sendRegFT5206(i,_FT5206REgisters[i-0x80]);
	}
	_sendRegFT5206(0x00,0x00);//Device Mode
}

/**************************************************************************/
/*!
		Communicate with FT5206
*/
/**************************************************************************/
void RA8875::_sendRegFT5206(uint8_t reg,const uint8_t val)
{
    // save I2C bitrate
	#if !defined(___DUESTUFF)
		uint8_t twbrbackup = TWBR;
		TWBR = 12; // upgrade to 400KHz!
	#endif
	Wire.beginTransmission(_ctpAdrs);
	Wire.write(reg);
	Wire.write(val);
	Wire.endTransmission(_ctpAdrs);
	#if !defined(___DUESTUFF)
		TWBR = twbrbackup;
	#endif
}


/**************************************************************************/
/*!
		This is the function that update the current state of the Touch Screen
		It's developed for use in loop
*/
/**************************************************************************/
void RA8875::updateTS(void)
{
    Wire.requestFrom((uint8_t)_ctpAdrs,(uint8_t)28); //get 28 registers
    uint8_t index = 0;
    while(Wire.available()) {
      _cptRegisters[index++] = Wire.read();//fill registers
    }
	_currentTouches = _cptRegisters[0x02] & 0xF;
	if (_currentTouches > _maxTouch) _currentTouches = _maxTouch;
	_gesture = _cptRegisters[0x01];
	if (_maxTouch < 2) _gesture = 0;
	uint8_t temp = _cptRegisters[0x03];
	_currentTouchState = 0;
	if (!bitRead(temp,7) && bitRead(temp,6)) _currentTouchState = 1;//finger up
	if (bitRead(temp,7) && !bitRead(temp,6)) _currentTouchState = 2;//finger down
}

/**************************************************************************/
/*!
		It gets coordinates out from data collected by updateTS function
		Actually it will not communicate with FT5206, just reorder collected data
		so it MUST be used after updateTS!
*/
/**************************************************************************/
uint8_t RA8875::getTScoordinates(uint16_t (*touch_coordinates)[2])
{
	uint8_t i;
	if (_currentTouches < 1) return 0;
 	for (i=1;i<=_currentTouches;i++){
		switch(_rotation){
			case 0://ok
				//touch_coordinates[i-1][0] = _width - (((_cptRegisters[coordRegStart[i-1]] & 0x0f) << 8) | _cptRegisters[coordRegStart[i-1] + 1]) / (4096/_width);
				//touch_coordinates[i-1][1] = (((_cptRegisters[coordRegStart[i-1]] & 0x0f) << 8) | _cptRegisters[coordRegStart[i-1] + 1]) / (4096/_height);
				touch_coordinates[i-1][0] = ((_cptRegisters[coordRegStart[i-1]] & 0x0f) << 8) | _cptRegisters[coordRegStart[i-1] + 1];
				touch_coordinates[i-1][1] = ((_cptRegisters[coordRegStart[i-1] + 2] & 0x0f) << 8) | _cptRegisters[coordRegStart[i-1] + 3];
			break;
			case 1://ok
				touch_coordinates[i-1][0] = (((_cptRegisters[coordRegStart[i-1] + 2] & 0x0f) << 8) | _cptRegisters[coordRegStart[i-1] + 3]);
				touch_coordinates[i-1][1] = (RA8875_WIDTH - 1) - (((_cptRegisters[coordRegStart[i-1]] & 0x0f) << 8) | _cptRegisters[coordRegStart[i-1] + 1]);
			break;
			case 2://ok
				touch_coordinates[i-1][0] = (RA8875_WIDTH - 1) - (((_cptRegisters[coordRegStart[i-1]] & 0x0f) << 8) | _cptRegisters[coordRegStart[i-1] + 1]);
				touch_coordinates[i-1][1] = (RA8875_HEIGHT - 1) -(((_cptRegisters[coordRegStart[i-1] + 2] & 0x0f) << 8) | _cptRegisters[coordRegStart[i-1] + 3]);
			break;
			case 3://ok
				touch_coordinates[i-1][0] = (RA8875_HEIGHT - 1) - (((_cptRegisters[coordRegStart[i-1] + 2] & 0x0f) << 8) | _cptRegisters[coordRegStart[i-1] + 3]);
				touch_coordinates[i-1][1] = (((_cptRegisters[coordRegStart[i-1]] & 0x0f) << 8) | _cptRegisters[coordRegStart[i-1] + 1]);
			break;
		}
		if (i == _maxTouch) return i;
	} 
    return _currentTouches;
}

/**************************************************************************/
/*!
		Gets the current Touch State, must be used AFTER updateTS!
*/
/**************************************************************************/
uint8_t RA8875::getTouchState(void)
{
	return _currentTouchState;
}

/**************************************************************************/
/*!
		Gets the number of touches, must be used AFTER updateTS!
		Return 0..5
		0: no touch
*/
/**************************************************************************/
uint8_t RA8875::getTouches(void)
{
	return _currentTouches;
}

/**************************************************************************/
/*!
		Gets the gesture, if identified, must be used AFTER updateTS!
*/
/**************************************************************************/
uint8_t RA8875::getGesture(void)
{
	return _gesture;
}

#elif defined(USE_RA8875_TOUCH)
/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+						RESISTIVE TOUCH STUFF										 +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/


/**************************************************************************/
/*!   
	  Initialize support for on-chip resistive Touch Screen controller
	  It also enable the Touch Screen
	  NOTE:
	  You need to use useINT(pin) [before] to define an INT pin!
	  You need to use enableISR() [after] to enable ISR on MCU and RA8875!
*/
/**************************************************************************/
void RA8875::touchBegin(void) 
{
	if (_intPin < 255){
/*		Touch Panel Control Register 0  TPCR0  [0x70]
		7: 0(disable, 1:(enable)
		6,5,4:TP Sample Time Adjusting (000...111)
		3:Touch Panel Wakeup Enable 0(disable),1(enable)
		2,1,0:ADC Clock Setting (000...111) set fixed to 010: (System CLK) / 4, 10Mhz Max! */
		#if defined(___TEENSYES) ||  defined(___DUESTUFF)//fast 32 bit processors
			_writeRegister(RA8875_TPCR0, TP_ENABLE | TP_ADC_SAMPLE_16384_CLKS | TP_ADC_CLKDIV_32);
		#else
			_writeRegister(RA8875_TPCR0, TP_ENABLE | TP_ADC_SAMPLE_4096_CLKS | TP_ADC_CLKDIV_16);
		#endif
		_writeRegister(RA8875_TPCR1, TP_MODE_AUTO | TP_DEBOUNCE_ON);
		setInternalInt(TOUCH);
		_INTC1_Reg |= (1 << 2);//set
		_writeRegister(RA8875_INTC1, _INTC1_Reg);
		_touchEnabled = true;
	} else {
		_touchEnabled = false;
	}
}

/**************************************************************************/
/*!   Initialize support for on-chip resistive Touch Screen controller
	  It also enable the Touch Screen
	  Parameters:
	  intPin:pin connected to RA8875 INT
*/
/**************************************************************************/
void RA8875::touchBegin(uint8_t intPin) {
	_intPin = intPin;
	pinMode(_intPin, INPUT);
	digitalWrite(_intPin, HIGH);
	//auto mode + debounce on
	_writeRegister(RA8875_TPCR1, RA8875_TPCR1_AUTO | RA8875_TPCR1_DEBOUNCE);
	touchEnable(true);
}


/**************************************************************************/
/*!
      Enables or disables the on-chip touch screen controller
 
      @param on Whether to turn touch sensing on or not
*/
/**************************************************************************/
void RA8875::touchEnable(boolean on)
{
  uint8_t   adcClk = (uint8_t) RA8875_TPCR0_ADCCLK_DIV4;

  if ( _displaySize == RA8875_800x480 ) //match up touch size with LCD size
    adcClk = (uint8_t) RA8875_TPCR0_ADCCLK_DIV16;

  if (on)
  {
    /* Enable Touch Panel (Reg 0x70) */
	_touchEnabled = true;
    _writeRegister(RA8875_TPCR0, RA8875_TPCR0_ENABLE        |
                           RA8875_TPCR0_WAIT_4096CLK  |
                           RA8875_TPCR0_WAKEENABLE   |
                           adcClk); // 10mhz max!
    /* Set Auto Mode      (Reg 0x71) */
    _writeRegister(RA8875_TPCR1, RA8875_TPCR1_AUTO    |
                           // RA8875_TPCR1_VREFEXT |
                           RA8875_TPCR1_DEBOUNCE);
    /* Enable TP INT */
    _writeRegister(RA8875_INTC1, _readRegister(RA8875_INTC1) | RA8875_INTCx_TP);
  }
  else
  {
    /* Disable TP INT */
	_touchEnabled = true;
    _writeRegister(RA8875_INTC1, _readRegister(RA8875_INTC1) & ~RA8875_INTCx_TP);
    /* Disable Touch Panel (Reg 0x70) */
    _writeRegister(RA8875_TPCR0, RA8875_TPCR0_DISABLE);
  }
}


/**************************************************************************/
/*! 
      Enables or disables the on-chip touch screen controller
	  You must use touchBegin at least once to instruct the RA8875
	  Parameters:
	  enabled: true(enable),false(disable)
*/
/**************************************************************************/
/* disabled, seem not to work
void RA8875::touchEnable(boolean enabled) {
	if (_intPin < 255){
		//another grrrr bug of the RA8875!
		// if we are in text mode the RA chip cannot get back the INT mode!
		if (_textMode) _setTextMode(false);
		if (!_touchEnabled && enabled) {//Enable
			//enableISR(true);
			// bitSet(_INTC1_Reg,2);
			// _writeRegister(RA8875_INTC1, _INTC1_Reg);
			_touchEnabled = true;
			_checkInterrupt(2);
		} else if (_touchEnabled && !enabled) {//disable
			//_disableISR();
			// bitClear(_INTC1_Reg,2);
			// _writeRegister(RA8875_INTC1, _INTC1_Reg);
			_checkInterrupt(2);
			_touchEnabled = false;
		}
	} else {
		_touchEnabled = false;
	}
}
*/

/**************************************************************************/
/*!   Detect a touch and return true, otherwise false.
	  It also correctly resets INT register to avoid false detections.
	  Will not work at all (return false) if touchBegin/touchEnable not set.
	  Using autoclear=true: 
	  This is useful to detect any press without get coordinates!
	  Note that if you are not using autoclear you will need a readTouchADC or readTouchPixel
	  just after touchDetect or the INT register will not clear and you will get a loopback!
	  In contrast, using autoclear and readTouchADC/readTouchPixel will result in wrong readings.
	  Parameters:
	  Autoclear:(true/false) set true when you want to use this function standalone and
	  not followed by a coordinate reading with readTouchADC/readTouchPixel
*/
/**************************************************************************/
boolean RA8875::touchDetect(boolean autoclear) {
	if (_touchEnabled){
		if (!digitalRead(_intPin)) {
			_clearTInt = true;
			if (touched()){
				if (autoclear) clearTouchInt();
				return true;
			} else {
				return false;
			}
		}
		if (_clearTInt){
			_clearTInt = false;
			clearTouchInt();
			delay(1);
		}
		return false;
	} else {
		return false;
	}
}

/**************************************************************************/
/*!   
	  Read 10bit internal ADC of RA8875 registers and perform corrections
	  It will return always RAW data
	  Parameters:
	  x: out 0...1023
	  Y: out 0...1023

*/
/**************************************************************************/
void RA8875::readTouchADC(uint16_t *x, uint16_t *y) 
{
	#if defined(_FASTCPU)
		_slowDownSPI(true);
	#endif
	uint16_t tx =  _readRegister(RA8875_TPXH);
	uint16_t ty =  _readRegister(RA8875_TPYH);
	uint8_t remain = _readRegister(RA8875_TPXYL);
	#if defined(_FASTCPU)
		_slowDownSPI(false);
	#endif
	tx <<= 2;
	ty <<= 2;
	tx |= remain & 0x03;        // get the bottom x bits
	ty |= (remain >> 2) & 0x03; // get the bottom y bits
	  if (_portrait){
		*x = ty;
		*y = tx;
	  } else {
		tx = 1024 - tx;
		ty = 1024 - ty;
		*x = tx;
		*y = ty;
	  }
}

/**************************************************************************/
/*!   
	  Returns 10bit x,y data with TRUE scale (0...1023)
	  Parameters:
	  x: out 0...1023
	  Y: out 0...1023
*/
/**************************************************************************/
void RA8875::touchReadAdc(uint16_t *x, uint16_t *y) 
{
	uint16_t tx,ty;
	readTouchADC(&tx,&ty);
	#if (defined(TOUCSRCAL_XLOW) && (TOUCSRCAL_XLOW != 0)) || (defined(TOUCSRCAL_XHIGH) && (TOUCSRCAL_XHIGH != 0))
		*x = map(tx,_tsAdcMinX,_tsAdcMaxX,0,1024);
	#else
		*x = tx;
	#endif
	#if (defined(TOUCSRCAL_YLOW) && (TOUCSRCAL_YLOW != 0)) || (defined(TOUCSRCAL_YHIGH) && (TOUCSRCAL_YHIGH != 0))
		*y = map(ty,_tsAdcMinY,_tsAdcMaxY,0,1024);
	#else
		*y = ty;
	#endif
	_checkInterrupt(2);
}

/**************************************************************************/
/*!   
	  Returns pixel x,y data with SCREEN scale (screen width, screen Height)
	  Parameters:
	  x: out 0...screen width  (pixels)
	  Y: out 0...screen Height (pixels)
	  Check for out-of-bounds here as touches near the edge of the screen
	  can be safely mapped to the nearest point of the screen.
	  If the screen is rotated, then the min and max will be modified elsewhere
	  so that this always corresponds to screen pixel coordinates.
	  /M.SANDERSCROCK added constrain
*/
/**************************************************************************/
void RA8875::touchReadPixel(uint16_t *x, uint16_t *y) 
{
	uint16_t tx,ty;
	readTouchADC(&tx,&ty);
	//*x = map(tx,_tsAdcMinX,_tsAdcMaxX,0,_width-1);
	*x = constrain(map(tx,_tsAdcMinX,_tsAdcMaxX,0,_width-1),0,_width-1);
	//*y = map(ty,_tsAdcMinY,_tsAdcMaxY,0,_height-1);
	*y = constrain(map(ty,_tsAdcMinY,_tsAdcMaxY,0,_height-1),0,_height-1);
	_checkInterrupt(2);
}

boolean RA8875::touchCalibrated(void) 
{
	return _calibrated;
}

/**************************************************************************/
/*!   A service utility that detects if system has been calibrated in the past
	  Return true if an old calibration exists
	  [private]
*/
/**************************************************************************/
boolean RA8875::_isCalibrated(void) 
{
	uint8_t uncaltetection = 4;
	#if defined(TOUCSRCAL_XLOW) && (TOUCSRCAL_XLOW != 0)
		uncaltetection--;
	#endif
	#if defined(TOUCSRCAL_YLOW) && (TOUCSRCAL_YLOW != 0)
		uncaltetection--;
	#endif
	#if defined(TOUCSRCAL_XHIGH) && (TOUCSRCAL_XHIGH != 0)
		uncaltetection--;
	#endif
	#if defined(TOUCSRCAL_YHIGH) && (TOUCSRCAL_YHIGH != 0)
		uncaltetection--;
	#endif
	if (uncaltetection < 4) return true;
	return false;
}

void RA8875::clearTouchInt(void) {
	_writeRegister(RA8875_INTC2, RA8875_INTCx_TP);
}

#endif
#endif

/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+								  SLEEP STUFF										 +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

/**************************************************************************/
/*!
    Sleep mode on/off (complete sequence)
	The sleep on/off sequence it's quite tricky on RA8875 when in SPI mode!
*/
/**************************************************************************/
void RA8875::sleep(boolean sleep) 
{
	if (_sleep != sleep){//only if it's needed
		_sleep = sleep;
		if (_sleep == true){
			//1)turn off backlight
			if (_displaySize == Adafruit_480x272 || _displaySize == Adafruit_800x480/* || _displaySize == Adafruit_640x480*/) GPIOX(false);
			//2)decelerate SPI clock
			#if defined(_FASTCPU)
				_slowDownSPI(true);
			#else
				#if defined(SPI_HAS_TRANSACTION)
					_SPImaxSpeed = 4000000UL;
				#else
					#if defined(___DUESTUFF) && defined(SPI_DUE_MODE_EXTENDED)
						SPI.setClockDivider(_cs,SPI_SPEED_READ);
					#else
						SPI.setClockDivider(SPI_SPEED_READ);
					#endif
				#endif
			#endif
			//3)set PLL to default
			_setSysClock(0x07,0x03,0x02);
			//4)display off & sleep
			_writeRegister(RA8875_PWRR, RA8875_PWRR_DISPOFF | RA8875_PWRR_SLEEP);
			delay(100);
		} else {
			//1)wake up with display off(100ms)
			_writeRegister(RA8875_PWRR, RA8875_PWRR_DISPOFF);
			delay(100);
			//2)bring back the pll
			_setSysClock(initStrings[_initIndex][0],initStrings[_initIndex][1],initStrings[_initIndex][2]);
			//_writeRegister(RA8875_PCSR,initStrings[_initIndex][2]);//Pixel Clock Setting Register
			delay(20);
			_writeRegister(RA8875_PWRR, RA8875_PWRR_NORMAL | RA8875_PWRR_DISPON);//disp on
			delay(20);
			//4)resume SPI speed
			#if defined(_FASTCPU)
				_slowDownSPI(false);
			#else
				#if defined(SPI_HAS_TRANSACTION)
					#if defined(__MKL26Z64__)
						if (_altSPI){
							_SPImaxSpeed = MAXSPISPEED2;
						} else {
							_SPImaxSpeed = MAXSPISPEED;
						}
					#else			
						_SPImaxSpeed = MAXSPISPEED;
					#endif
				#else
					#if defined(___DUESTUFF) && defined(SPI_DUE_MODE_EXTENDED)
						SPI.setClockDivider(_cs,SPI_SPEED_WRITE);
					#else
						SPI.setClockDivider(SPI_SPEED_WRITE);
					#endif
				#endif
			#endif
			//5)PLL afterburn!
			_setSysClock(sysClockPar[_initIndex][0],sysClockPar[_initIndex][1],initStrings[_initIndex][2]);
			//5)turn on backlight
			if (_displaySize == Adafruit_480x272 || _displaySize == Adafruit_800x480/* || _displaySize == Adafruit_640x480*/) GPIOX(true);
			//_writeRegister(RA8875_PWRR, RA8875_PWRR_NORMAL);
		}
	}
}



/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+							SPI & LOW LEVEL STUFF									 +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

/**************************************************************************/
/*! PRIVATE
		Write in a register
		Parameters:
		reg: the register
		val: the data
*/
/**************************************************************************/
void RA8875::_writeRegister(const uint8_t reg, uint8_t val) 
{
	writeCommand(reg);
	_writeData(val);
}

/**************************************************************************/
/*! PRIVATE
		Returns the value inside register
		Parameters:
		reg: the register
*/
/**************************************************************************/
uint8_t RA8875::_readRegister(const uint8_t reg) 
{
	writeCommand(reg);
	return _readData(false);
}

/**************************************************************************/
/*!
		Write data
		Parameters:
		d: the data
*/
/**************************************************************************/
void RA8875::_writeData(uint8_t data) 
{
	_startSend();
	#if defined(___DUESTUFF) && defined(SPI_DUE_MODE_EXTENDED)
		SPI.transfer(_cs, RA8875_DATAWRITE, SPI_CONTINUE); 
		SPI.transfer(_cs, data, SPI_LAST);
	#else
		#if (defined(__AVR__) && defined(_FASTSSPORT)) || defined(SPARK)
			_spiwrite16((RA8875_DATAWRITE<<8) | data);
		#else
			#if defined(__MKL26Z64__)	
				if (_altSPI){
					SPI1.transfer16((RA8875_DATAWRITE<<8) | data); 
				} else {
					SPI.transfer16((RA8875_DATAWRITE<<8) | data); 
				}
			#else
				SPI.transfer16((RA8875_DATAWRITE<<8) | data); 
			#endif
		#endif
	#endif
	_endSend();
}

/**************************************************************************/
/*! 
		Write 16 bit data
		Parameters:
		d: the data (16 bit)
*/
/**************************************************************************/
void  RA8875::writeData16(uint16_t data) 
{
	_startSend();
#if (defined(__AVR__) && defined(_FASTSSPORT)) || defined(SPARK)
	_spiwrite(RA8875_DATAWRITE);
#else
#if defined(__MKL26Z64__)	
	if (_altSPI)
		SPI1.transfer(RA8875_DATAWRITE);
	else 
		SPI.transfer(RA8875_DATAWRITE);
#else
	SPI.transfer(RA8875_DATAWRITE);
#endif
#endif
	
	#if !defined(ENERGIA) && !defined(___DUESTUFF) && ((ARDUINO >= 160) || (TEENSYDUINO > 121))
		#if defined(__MKL26Z64__)	
			if (_altSPI){
				SPI1.transfer16(data);
			} else {
				SPI.transfer16(data);
			}
		#else
			SPI.transfer16(data);
		#endif
	#else
		#if defined(___DUESTUFF) && defined(SPI_DUE_MODE_EXTENDED)
			SPI.transfer(_cs, highByte(data), SPI_CONTINUE); 
			SPI.transfer(_cs, lowByte(data), SPI_LAST);
		#else
			#if (defined(__AVR__) && defined(_FASTSSPORT)) || defined(SPARK)
				_spiwrite16(data);
			#else
				SPI.transfer16(data);
			#endif
		#endif
	#endif
	_endSend();
}

/**************************************************************************/
/*!	PRIVATE

*/
/**************************************************************************/
uint8_t RA8875::_readData(bool stat) 
{
	#if defined(SPI_HAS_TRANSACTION)
		if (_inited) _SPImaxSpeed = _SPImaxSpeed/2;
	#else
		#if defined(___DUESTUFF) && defined(SPI_DUE_MODE_EXTENDED)
			if (_inited) SPI.setClockDivider(_cs,SPI_SPEED_READ);
		#else
			if (_inited) SPI.setClockDivider(SPI_SPEED_READ);
		#endif
	#endif
	_startSend();
	#if defined(___DUESTUFF) && defined(SPI_DUE_MODE_EXTENDED)
		stat == true ? SPI.transfer(_cs, RA8875_CMDREAD, SPI_CONTINUE) : SPI.transfer(_cs, RA8875_DATAREAD, SPI_CONTINUE);
		uint8_t x = SPI.transfer(_cs, 0x0, SPI_LAST);
	#else
		#if (defined(__AVR__) && defined(_FASTSSPORT)) || defined(SPARK)
			stat == true ? _spiwrite(RA8875_CMDREAD) : _spiwrite(RA8875_DATAREAD);
			uint8_t x = _spiread();
		#else
			#if defined(__MKL26Z64__)	
				uint8_t x;
				if (_altSPI){
					stat == true ? SPI1.transfer(RA8875_CMDREAD) : SPI1.transfer(RA8875_DATAREAD);
					x = SPI1.transfer(0x0);
				} else {
					stat == true ? SPI.transfer(RA8875_CMDREAD) : SPI.transfer(RA8875_DATAREAD);
					x = SPI.transfer(0x0);
				}
			#else
				stat == true ? SPI.transfer(RA8875_CMDREAD) : SPI.transfer(RA8875_DATAREAD);
				uint8_t x = SPI.transfer(0x0);
			#endif
		#endif
	#endif
	_endSend();
	#if defined(SPI_HAS_TRANSACTION)
		if (_inited) _SPImaxSpeed = _SPImaxSpeed*2;
	#else
		#if defined(___DUESTUFF) && defined(SPI_DUE_MODE_EXTENDED)
			if (_inited) SPI.setClockDivider(_cs,SPI_SPEED_WRITE);
		#else
			if (_inited) SPI.setClockDivider(SPI_SPEED_WRITE);
		#endif
	#endif
	return x;

}

/**************************************************************************/
/*!

*/
/**************************************************************************/
uint8_t	RA8875::readStatus(void) 
{
	return _readData(true);
}

/**************************************************************************/
/*! PRIVATE
		Write a command
		Parameters:
		d: the command
*/
/**************************************************************************/
void RA8875::writeCommand(const uint8_t d) 
{
	_startSend();
	#if defined(___DUESTUFF) && defined(SPI_DUE_MODE_EXTENDED)
		SPI.transfer(_cs, RA8875_CMDWRITE, SPI_CONTINUE); 
		SPI.transfer(_cs, d, SPI_LAST);
	#else
		#if (defined(__AVR__) && defined(_FASTSSPORT)) || defined(SPARK)
			_spiwrite16((RA8875_CMDWRITE<<8)|d);
		#else
			#if defined(SPI_HAS_TRANSACTION) && defined(__MKL26Z64__)	
				if (_altSPI){
					SPI1.transfer16((RA8875_CMDWRITE<<8)|d);
				} else {
					SPI.transfer16((RA8875_CMDWRITE<<8)|d);;
				}
			#else
				SPI.transfer16((RA8875_CMDWRITE<<8)|d);
			#endif
		#endif
	#endif
	_endSend();
}


/*
 void RA8875::debugData(uint16_t data,uint8_t len)
{
	int i;
	for (i=len-1; i>=0; i--){
		if (bitRead(data,i)==1){
			Serial.print("1");
		} else {
			Serial.print("0");
		}
	}
	Serial.print(" -> 0x");
	Serial.print(data,HEX);
	Serial.print("\n");
} 
*/

/*
 void RA8875::showLineBuffer(uint8_t data[],int len)
{
	int i;
	for (i=0; i<len; i++){
		if (data[i] == 1){
			Serial.print("1");
		} else {
			Serial.print("0");
		}
	}
	Serial.print("\n");
} 
*/

#if defined(EEPROM_SUPPORTED)
/**************************************************************************/
/*!
    Read from the EEPROM location
 
    @param location The location of the EEPROM to read
 
    @return The value
*/
/**************************************************************************/

uint32_t RA8875::eepromReadS32(int location)
{
    uint32_t value = ((uint32_t)EEPROM.read(location))<<24;
    value = value | ((uint32_t)EEPROM.read(location+1))<<16;
    value = value | ((uint32_t)EEPROM.read(location+2))<<8;
    value = value | ((uint32_t)EEPROM.read(location+3));
    return value;
}

/**************************************************************************/
/*!
    Write to the EEPROM location
 
    @param location The location of the EEPROM to write to
    @param value The value to write
 */
/**************************************************************************/
void RA8875::eepromWriteS32(int location, int32_t value)
{
    EEPROM.write(location,   (value >> 24)&0xff);
    EEPROM.write(location+1, (value >> 16)&0xff);
    EEPROM.write(location+2, (value >> 8)&0xff);
    EEPROM.write(location+3, (value )&0xff);
}

/**************************************************************************/
/*!
     Read Calibration Data from the EEPROM location

     @param location The location of the EEPROM to read from
     @param matrixPtr The pointer to the Matrix Variable
 
     @return success
 */
/**************************************************************************/
bool RA8875::readCalibration(int location, tsMatrix_t * matrixPtr)
{
    if (location+sizeof(tsMatrix_t) > EEPROMSIZE){
        return false; //readCalibration::Calibration location outside of EEPROM memory bound
    }
    if (EEPROM.read(location+CFG_EEPROM_TOUCHSCREEN_CALIBRATED) == 1){
        matrixPtr->An = eepromReadS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_AN);
        matrixPtr->Bn = eepromReadS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_BN);
        matrixPtr->Cn = eepromReadS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_CN);
        matrixPtr->Dn = eepromReadS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_DN);
        matrixPtr->En = eepromReadS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_EN);
        matrixPtr->Fn = eepromReadS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_FN);
        matrixPtr->Divider = eepromReadS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_DIVIDER);
        return true;
    }
    return false;
}

/**************************************************************************/
/*!
     Write Calibration Data to the EEPROM location
 
     @param location The location of the EEPROM to write to
     @param matrixPtr The pointer to the Matrix Variable
 */
/**************************************************************************/
void RA8875::writeCalibration(int location, tsMatrix_t * matrixPtr)
{
    if (location+sizeof(tsMatrix_t) < EEPROMSIZE){    // Check to see it calibration location outside of EEPROM memory bound
        eepromWriteS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_AN, matrixPtr->An);
        eepromWriteS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_BN, matrixPtr->Bn);
        eepromWriteS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_CN, matrixPtr->Cn);
        eepromWriteS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_DN, matrixPtr->Dn);
        eepromWriteS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_EN, matrixPtr->En);
        eepromWriteS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_FN, matrixPtr->Fn);
        eepromWriteS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_DIVIDER, matrixPtr->Divider);
        EEPROM.write(location+CFG_EEPROM_TOUCHSCREEN_CALIBRATED, 1);
    }
}
#endif
