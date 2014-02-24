

#ifndef SeeedRFID_H
#define SeeedRFID_H

#include <SoftwareSerial.h>
#include "Arduino.h"

struct RFIDdata
{
	int dataLen;
	byte chk;
	boolean valid;
	char raw[5];
};

enum RFIDType
{ 
	RFID_UART,
	RFID_WIEGAND
};

class SeeedRFID
{
private:
	SoftwareSerial *_io;
		// SoftwareSerial * _rfidIO; // software serail
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
	long cardNumber();
};

#endif //__SeeedRFID_H__
