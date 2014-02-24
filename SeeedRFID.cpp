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

#include "SeeedRFID.h"

SeeedRFID::SeeedRFID(int rxPin, int txPin)
{
	_io = new SoftwareSerail(rxPin, txPin);
	_io->begin(9600);

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

boolean checkBitValidationUART()
{
	if( 5 == _data.dataLen && (_data.raw[4] == _data.raw[0]^_data.raw[1]^_data.raw[2]^_data.raw[3]))
	{
		_tag.valid = _isAvailable = true;
		return true;
	} else
	{
		_tag.valid = _isAvailable = false;
		return false;
	}
}

boolean SeeedRFID::read()
{
	_isAvailable = false;

	if (_data.dataLen != 0)
	{
		_data.dataLen = 0;
	}

	if (_io->available() > 0)
	{
		_data.raw[_data.dataLen++] = _io->read();
	}

	return this.checkBitValidationUART();
}

boolean SeeedRFID::isAvailable()
{
	switch(_type){
		case RFID_UART:
			return this.read();
#ifdef DEBUG
	Serial.println("All data from read()");
	Serial.println(_data.raw, HEX);
	Serial.print("RFID check bit: ");
	Serial.println(_data.chk, HEX);
	Serial.print("RFID data validation: ");
	Serial.println(_data.valid);
#endif
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
		return 1; // empty data	
	}
}

long SeeedRFID::cardNumber()
{
#ifdef DEBUG
	unsigned long debugNumber = 0x00916f0b;
	Serial.print("debugNumber(0x00916f0b): ");
	Serial.println(debugNumber);
#endif

	unsigned long sum = 0;
	if(0 != data[0]){
		sum = sum + data[0];
		sum = sum<<24;
	}
	sum = sum + data[1];
	sum = sum<<16;

	unsigned long sum2 = 0;
	sum2 = sum2  + data[2];
	sum2 = sum2 <<8;
	sum2 = sum2  + data[3];

	sum = sum + sum2;

#ifdef DEBUG
	Serial.print("cardNumber(HEX): ");
    Serial.println(sum, HEX);
	Serial.print("cardNumber: ");
    Serial.println(sum);
#endif
}