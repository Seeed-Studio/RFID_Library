/*    
 * SeeedRFID.cpp
 * A library for RFID moudle.
 *   
 * Copyright (c) 2008-2014 seeed technology inc.  
 * Author      : Ye Xiaobo(yexiaobo@seeedstudio.com)
 * Create Time: 2014/2/20
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**************************************************************************
* Pins
* ====
*
* 1. VCC support 3.3 ~ 5V
* 2. TX, RX connect to Arduino or Seeeduino
* 3. T1, T2 is the Signal port for RFID antenna
* 4. W0, W1 is for wiegand protocol, but this library not support yet.
* 
* ```
* 		+-----------+
* 	----|VCC	  T1|----
* 	----|GND	  T2|----
* 	----|TX		 SER|----
* 	----|RX		 LED|----	
* 	----|W0		BEEP|----
* 	----|W1		 GND|----
* 		+-----------+
* ```
***************************************************************************/

#include <SoftwareSerial.h>
#include "SeeedRFID.h"
#include "Arduino.h"

SeeedRFID::SeeedRFID(int rxPin, int txPin)
{
    _rfidIO = new SoftwareSerial(rxPin, txPin);
    _rfidIO->begin(9600);

	// init RFID data
	_data.dataLen = 0;
	_data.chk = 0;
	_data.valid = false;

	_isAvailable = false;
	_type = RFID_UART;
}

SeeedRFID::~SeeedRFID()
{
}


boolean SeeedRFID::checkBitValidationUART()
{
	if( 5 == _data.dataLen && (_data.raw[4] == _data.raw[0]^_data.raw[1]^_data.raw[2]^_data.raw[3]))
	{
		_data.valid = _isAvailable = true;
		return true;
	} else
	{
		_data.valid = _isAvailable = false;
		return false;
	}
}
/*
boolean SeeedRFID::listen()
{
	_rfidIO->listen();
}
*/
boolean SeeedRFID::read()
{

	_isAvailable = false;

	if (_data.dataLen != 0)
	{
		_data.dataLen = 0;
	}
	
	while (_rfidIO->available())
	{
		_data.raw[_data.dataLen++] = _rfidIO->read();
#ifdef DEBUG
	Serial.println("SeeedRFID:read() function, and the RFID raw data: ");
	for (int i = 0; i < _data.dataLen; ++i)
	{
		Serial.println();
		Serial.print(_data.raw[i], HEX);
		Serial.print('\t');
	}
	Serial.println();
#endif
		delay(10);
	}

	return SeeedRFID::checkBitValidationUART();
}

boolean SeeedRFID::isAvailable()
{
	switch(_type){
		case RFID_UART:
			return SeeedRFID::read();
			break;
		case RFID_WIEGAND:
			return false;
			break;
		default:
			return false;
			break;
	}
}

RFIDdata SeeedRFID::data()
{
	if (_data.valid)
	{
		return _data;
	}else{
		// empty data
		RFIDdata _tag;
		return _tag;
	}
}

unsigned long SeeedRFID::cardNumber()
{
	// unsigned long myZero = 255;

	unsigned long sum = 0;
	if(0 != _data.raw[0]){
		// _data.raw[0] = 	_data.raw[0] & myZero;
		sum = sum + _data.raw[0];
		sum = sum<<24;
	}
	// _data.raw[1] = 	_data.raw[1] & myZero;
	sum = sum + _data.raw[1];
	sum = sum<<16;

	unsigned long sum2 = 0;
	// _data.raw[2] = 	_data.raw[2] & myZero;
	sum2 = sum2  + _data.raw[2];
	sum2 = sum2 <<8;
	// _data.raw[3] = 	_data.raw[3] & myZero;
	sum2 = sum2  + _data.raw[3];

	sum = sum + sum2;

#ifdef DEBUG
	Serial.print("cardNumber(HEX): ");
    Serial.println(sum, HEX);
	Serial.print("cardNumber: ");
    Serial.println(sum);
#endif

    return sum;
}
