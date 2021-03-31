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
 * 
 * 
 * ERROR CODES:
 *  0: No error occured
 *  1: BMP280 module failed to initialize
 *  2: MPU6050 module failed to initialize
 *  
 *  10: Invalid BMP280 data
 *  11: Invalid MPU6050 data
 *  
 *  30: The GPS time is invalid
 *  31: The GPS location is invalid
 *  32: The GPS altitude is invalid
 * 
 * TODO
 * 		1) Figure out what should be done if an error occurs
 * 			1) Just ignore it?
 * 			2) Try to fix it by recalling Can::tick / rereading the sensor / etc
 * 		2) Write the Can::tick logic
 * 		3) Write the Can::checkRadioConfiguration check so it checks if the radio is configured correctly
 *    4) finish Can::calculate_expected_time_until_impact
 *    
 *    
 * This project falls under the GNU GPL-3.0 license, see LICENSE or https://www.gnu.org/licenses/gpl-3.0.txt.
 */
#if !defined(CAN_H)
#define CAN_H
#include <Arduino.h>
#include <EEPROM.h>

#include "src/Adafruit_BMP280.h"
#include "src/Adafruit_MPU6050.h"

#include "TinyGPS++.h"
//#include <TinyGPS++.h>

#include "Vector3.h"

//#include "Error.h"

#define EEPROM_LAST_SENT_PACKET_ID_OFFSET (0) // The last packet ID that has been sent
#define EEPROM_LAST_SENT_PACKET_TIME_OFFSET (sizeof(unsigned int)) // The last time a packet was sent
#define EEPROM_GPS_TIME_OFFSET (EEPROM_LAST_SENT_PACKET_TIME_OFFSET + (sizeof(char) * 4))
#define EEPROM_GPS_ALTITUDE_OFFSET (EEPROM_GPS_TIME_OFFSET + (sizeof(char) * 4 * 10))


typedef struct {
  char *time;
  double altitude;
}GPS_Altitude;

class Can {
	private:
//		GPS *gps; 						// The GPS object that will be used to gather GPS data
      HardwareSerial   *gpsSerial;
      TinyGPSPlus gps;

		uint8_t radioSetPin;	// The pin where the set pin on the radio module is connected to, for configuring purposes

		uint8_t gps_altitude_time_cursor; // Where in the GPS_Altitude[10] array we are (so what's the newest data)

		unsigned int lastPacketID;
		unsigned int ticksPerSecond;

    /*
     * Saves the time and altitude to the EEPROM
     * 
     * @param time [char*] A char[] that has four elements: hour, minutes, seconds and centiseconds
     * @param altitude [double] The altitude the GPS module shows
     * 
     * @return error [uint8_t] If this number is higher than 0 an error occured, the value of the number will tell what the error was
     */
    uint8_t save_altitude_time_to_eeprom(char *time, double altitude);

    /*
     * Calculate the air speed (vertical velocity) with the data that has been saved in the EEPROM
     * 
     * @param errors [Error*] A pointer to an Error[] object, if any error will occur it will be saved to this object
     * @param air_speed [double*] A pointer to the variable where the air speed should be saved
     * 
     * @return error [uint8_t] If this number is higher than 0 an error occured, the value of the number will tell what the error was
     */
    uint8_t calculate_air_speed(double *air_speed);
          
    /*
     * Calculate the expected amount of time it takes until the Can hits the ground
     * 
     * @param errors [Error*] A pointer to an Error[] object, if any error will occur it will be saved to this object
     * @param altitude [double] The current altitude of the Can in meters
     * @param air_speed [double] The current air speed (vertical velocity) in m/s
     * @param exptected_time_until_impact [double*] A pointer to the variable where the expected time until impact should be stored in seconds
     * 
     * @return error [uint8_t] If this number is higher than 0 an error occured, the value of the number will tell what the error was
     */
    uint8_t calculate_expected_time_until_impact(double altitude, double air_speed, double *exptected_time_until_impact);

    /*
     * Save the radio transmission to the SD card
     * 
     * @param errors [Error*] A pointer to an Error[] object, if any error will occur it will be saved to this object
     * 
     * @return error [uint8_t] If this number is higher than 0 an error occured, the value of the number will tell what the error was
     */
    uint8_t save_radio_transmission_to_sd();

	public:
		/*
		 * Sets the radioSerial variable. 
		 * Creates a new instance of GPS and initializes that instance (with the gpsSerial variable). 
		 * Creates a new instance of Adafruit_BMP280. 
		 *  
		 * @param radioSerial [HardwareSerial*] A pointer to a hardware serial port that communicates with the radio module (APC220)
		 * @param gpsSerial [HardwareSerial*] A pointer to a hardware serial port that communicates with the GPS module
		 * @param radioSetPin [int] The pin where the set pin on the radio module is connected to, for configuring purposes
		 * @param tickPerSecond [uint8_t] How many times per second Can::tick is being called
		 * 
		 * @return void
		 */
		Can(HardwareSerial *radioSerial, HardwareSerial *gpsSerial, int radioSetPin, uint8_t ticksPerSecond);

    // A pointer to the hardware serial port that communicates with the radio module (APC220)
		HardwareSerial *radioSerial;
    // The Adafruit_BMP280 object to communicate with the BMP280 module
		Adafruit_BMP280 *bmp;
    // The Adafruit_MPU6050 object to communicate with the MPU6050 module
		Adafruit_MPU6050 *mpu;

		/*
		 * Begins all the serial ports and modules. 
		 * 
		 * @param radio_uart_baudrate [uint8_t] The baudrate on which the HardwareSerial port connected to the radio will function [OPTIONAL, DEFAULT IS 4800 BAUD]
		 * @param gps_update_frequency [uint8_t] The rate at which the GPS module will transmit GPS data [OPTIONAL, DEFAULT IS 1 Hz]
		 * 
		 * @return error [uint8_t] If this number is higher than 0 an error occured, the value of the number will tell what the error was
		*/
		uint8_t begin(uint8_t radio_uart_baudrate = 4800, uint8_t gps_update_frequency = 1);

		/*
		 * Configures the APC220 radio module. 
		 * 
		 * The settings are: 
		 * 		Frequency is 434000 kHz = 434000. 
  	 *		Air rate is 2400 bps = 1. 
  	 *		Output power = 4. 
  	 *		UART baudrate = 3. 
  	 *		Byte check parity = 0. 
		 * 
		 * @return void
		*/
		void configureRadio();

		/*
		 * Checks if the APC220 radio module is configured correctly
		 * 
		 * @return error [uint8_t] If this number is higher than 0 an error occured, the value of the number will tell what the error was
		*/
		uint8_t checkRadioConfiguration();

		/*
		 * This method gathers all the data from the BMP280, MPU6050 and GPS module. 
		 * The data will be saved to the SD card and transmitted over the radio. 
		 * 
		 * @return error [uint8_t*] This array of numbers indicates if an error has occured
		*/
		uint8_t* tick();


    // STATIC METHOD
    static uint8_t check_error(uint8_t error);
};

#endif
