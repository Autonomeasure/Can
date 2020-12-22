/*
 * CanSat 2020-2021 Team Autonomeasure - Can
 * Code written by: Joep van Dijk
 * First mission:
 *    Our first mission is to be able to land properly, collect data from the BMP280 sensor and send that data to the groundstation at least one time per second. 
 * 
 * Second mission:
 *    Our second mission is to make a sustainable data collection station. 
 * 
 * Links:
 *    GitHub Autonomeasure:           https://github.com/Autonomeasure
 *    GitHub Can repo:                https://github.com/Autonomeasure/Can
 *    GitHub GroundStation repo:      https://github.com/Autonomeasure/GroundStation
 *    Instagram:                      https://instagram.com/Autonomeasure/
 */
#include "Can.h"

// -============= PUBLIC FUNCTIONS =============-
// The constructor, sets the radio serial and gps serial port variables. It also sets the radio set pin variable for later use.
Can::Can(HardwareSerial *radioSerial, HardwareSerial *gpsSerial, int radioSetPin) {
	// Set all the variables
	this->radioSerial = radioSerial;
	this->radioSetPin = radioSetPin;

	// Create the GPS object
	this->gps = new GPS(gpsSerial);

	// Create the Adafruit_BMP280 object
	this->bmp = new Adafruit_BMP280();

	// Create the new Adafruit_BMP280 objct
	this->mpu = new Adafruit_MPU6050();
}

// Call the begin function of all serial ports and set the mode of the radioSetPin as OUTPUT and set it to HIGH
uint8_t Can::begin(Error *errors, uint8_t radio_uart_baudrate = 9600, uint8_t gps_uart_baudrate = 9600) {
	uint8_t amountOfErrors;
	this->radioSerial->begin(radio_uart_baudrate);
	this->gps->begin(gps_uart_baudrate);

	pinMode(this->radioSetPin, OUTPUT);
	digitalWrite(this->radioSetPin, HIGH); // Set the radio into "production" mode

	if (!this->bmp->begin()) {
		errors[amountOfErrors].errorID = 1;
		amountOfErrors++;
	}
	if (!this->mpu->begin()) {
		errors[amountOfErrors].errorID = 2;
			amountOfErrors++;
	}

	return amountOfErrors;
}

// Configure the APC220 module to the right settings
void Can::configureRadio() {
  digitalWrite(this->radioSetPin, LOW); // Set the APC220 in config mode
  delay(50);

  // Frequency: 434000 kHz
  // Air rate is 2400 bps = 1
  // Output power = 4
  // UART baudrate = 3
  // Byte check parity = 0

  this->radioSerial->print("w 434000 1 4 3 0");
  this->radioSerial->write(0x0D);
  this->radioSerial->write(0x0A);
  delay(10);

  digitalWrite(this->radioSetPin, HIGH); // Set the APC220 in "production" mode
	delay(10); // Stabalize
}

// Check if the APC220 is configured correctly
Error Can::checkRadioConfiguration() {
	digitalWrite(this->radioSetPin, LOW); // Set the APC220 in config mode
	delay(10); // Stabalize
	this->radioSerial->println("RD"); // Request the configuration back from the APC220
	delay(10);

	// Read the incoming data from the APC220 module
	while (this->radioSerial->available()) {
		Serial.write(this->radioSerial->read()); // For testing purposes print it back to the Serial monitor

		// TODO Add code to check if the APC220 is configured properly
	}

	digitalWrite(this->radioSetPin, HIGH); // Set the APC220 in "production" mode
	delay(10); // Stabalize
}

// This method will gather all the data from modules and save it to the SD card and send it to the radio
bool Can::tick() {
	
}