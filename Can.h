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

#include "Error.h"

#define EEPROM_GPS_ALTITUDE_TIME_OFFSET 0 // The offset to use for the EEPROM address to get the GPS altitude and time measurements


class Can {
	private:
		GPS *gps; 						// The GPS object that will be used to gather GPS data

		uint8_t radioSetPin;	// The pin where the set pin on the radio module is connected to, for configuring purposes

	public:
		/*
		 * Sets the radioSerial variable. 
		 * Creates a new instance of GPS and initializes that instance (with the gpsSerial variable). 
		 * Creates a new instance of Adafruit_BMP280. 
		 *  
		 * @param radioSerial [HardwareSerial] A pointer to a hardware serial port that communicates with the radio module (APC220)
		 * @param gpsSerial [HardwareSerial] A pointer to a hardware serial port that communicates with the GPS module
		 * @param radioSetPin [int] The pin where the set pin on the radio module is connected to, for configuring purposes
		 * 
		 * @return void
		 */
		Can(HardwareSerial *radioSerial, HardwareSerial *gpsSerial, int radioSetPin);

		HardwareSerial *radioSerial; 	// A pointer to the hardware serial port that communicates with the radio module (APC220)
		Adafruit_BMP280 *bmp; 				// The Adafruit_BMP280 object to communicate with the BMP280 module
		Adafruit_MPU6050 *mpu; 				// The Adafruit_MPU6050 object to communicate with the MPU6050 module

		unsigned int eeprom_GPS_altitude_time_bytes_written; // How many bytes are saved to	the EEPROM memory

		/*
		 * Begins all the serial ports and modules. 
		 * 
		 * @param errors [Error*] A pointer to an Error object, if any error will occur it will be saved to this object (or any further down the chain)
		 * @param radio_uart_baudrate [uint8_t] The baudrate on which the HardwareSerial port connected to the radio will function [OPTIONAL, DEFAULT IS 9600 BAUD]
		 * @param gps_uart_baudrate [uint8_t] The baudrate on which the HardwareSerial port connected to the GPS module will function [OPTIONAL, DEFAULT IS 9600 BAUD]
		 * 
		 * @return amountOfErrors [uint8_t] This number will tell how many errors occured to access all the elemens of the [Error*] errors array
		*/
		uint8_t begin(Error *errors, uint8_t radio_uart_baudrate = 9600, uint8_t gps_uart_baudrate = 9600);

		/*
		 * Configures the APC220 radio module. 
		 * 
		 * @return void
		*/
		void configureRadio();
		Error checkRadioConfiguration(); // Check if the APC220 is configured correctly

		bool tick(); // This method will gather all the data from modules and save it to the SD card and send it to the radio
};

#endif