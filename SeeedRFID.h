/*
 * SeeedRFID.h
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

#ifndef SeeedRFID_H
#define SeeedRFID_H

#include <SoftwareSerial.h>
#include "Arduino.h"

struct RFIDdata
{
	int dataLen;
	byte chk;
	boolean valid;
	unsigned char raw[5];
};

enum RFIDType
{
	RFID_UART,
	RFID_WIEGAND
};

class SeeedRFID
{
private:
	SoftwareSerial * _rfidIO; // software serail
	RFIDdata _data;
	boolean _isAvailable;
	RFIDType _type;
	boolean checkBitValidationUART();
	boolean read();
public:
	SeeedRFID(int rxPin, int txPin);
	~SeeedRFID();

	boolean isAvailable();
	RFIDdata data();
	//boolean listen();
	unsigned long cardNumber();
    //void listen();
};

#endif //__SeeedRFID_H__
