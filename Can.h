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
#if !defined(CAN_H)
#define CAN_H
#include <Arduino.h>
#include "GPS.h"
#include "Adafruit_BMP280.h"
#include "Adafruit_MPU6050.h"

#define EEPROM_GPS_ALTITUDE_TIME_OFFSET 0 // The offset to use for the EEPROM address to get the GPS altitude and time measurements


class Can {
	private:
		GPS *gps; 						// The GPS object that will be used to gather GPS data

		uint8_t radioSetPin;	// The pin where the set pin on the radio module is connected to, for configuring purposes

	public:
		/**
		 * Sets the radioSerial variable. 
		 * Creates a new instance of GPS and initializes that instance (with the gpsSerial variable). 
		 * Creates a new instance of Adafruit_BMP280. 
		 *  
		 * @param radioSerial A pointer to a hardware serial port that communicates with the radio module (APC220)
		 * @param gpsSerial A pointer to a hardware serial port that communicates with the GPS module
		 * @param radioSetPin The pin where the set pin on the radio module is connected to, for configuring purposes
		 * 
		 * @return null
		 **/
		Can(HardwareSerial *radioSerial, HardwareSerial *gpsSerial, int radioSetPin);

		HardwareSerial *radioSerial; // A pointer to the hardware serial port that communicates with the radio module (APC220)
		Adafruit_BMP280 *bmp; // The Adafruit_BMP280 object to communicate with the BMP280 module
		Adafruit_MPU6050 *mpu; // The Adafruit_MPU6050 object to communicate with the MPU6050 module

		unsigned int eeprom_GPS_altitude_time_bytes_written; // How many bytes are saved to	the EEPROM memory

		bool begin(uint8_t radio_uart_baudrate = 9600, uint8_t gps_uart_baudrate = 9600); // Call the begin function of all serial ports and set the mode of the radioSetPin as OUTPUT and set it to HIGH
		void configureRadio(); // Configure the APC220 module to the right settings
};

#endif