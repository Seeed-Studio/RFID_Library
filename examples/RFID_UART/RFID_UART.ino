// RFID_UART.ino

#include <SoftwareSerial.h>
#include <SeeedRFID.h>

#define RFID_RX_PIN 10
#define RFID_TX_PIN 11

#ifdef 1
	int DEBUG = 1;
#endif
SeeedRFID RFID(RFID_RX_PIN, RFID_TX_PIN);

void setup() {
	Serial.begin(9600);
	Serial.println("Hello, 2bk!");
}

void loop() {
	if(RFID.isAvailable()){
	    RFID.cardNumber();
	}
}

