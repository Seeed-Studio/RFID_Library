
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