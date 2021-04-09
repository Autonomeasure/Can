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
//#include <SPI.h>
//#include <SD.h>

#include "Radio.h"

#include "src/Adafruit_BMP280.h"
#include "src/Adafruit_MPU6050.h"

#include "TinyGPS++.h"

#include "Vector3.h"

#define RADIO Serial1 // The HardwareSerial object that connects with the APC220 radio module
#define GPS Serial2 // The HardwareSerial object that connects with the GPS module

#define EEPROM_LAST_SENT_PACKET_ID_OFFSET (0) // The last packet ID that has been sent
#define EEPROM_LAST_SENT_PACKET_TIME_OFFSET (sizeof(unsigned int)) // The last time a packet was sent
#define EEPROM_GPS_TIME_OFFSET (EEPROM_LAST_SENT_PACKET_TIME_OFFSET + (sizeof(char) * 4))
#define EEPROM_GPS_ALTITUDE_OFFSET (EEPROM_GPS_TIME_OFFSET + (sizeof(char) * 4 * 10))


typedef struct {
  char *time;
  float gps_altitude;
  float mpu_altitude;
}GPS_Altitude;

class Can {
	private:
    HardwareSerial   *gpsSerial;
    TinyGPSPlus gps;

//    File log_file;

		uint8_t radioSetPin;	// The pin where the set pin on the radio module is connected to, for configuring purposes

		uint8_t _gps_altitude_time_cursor; // Where in the GPS_Altitude[10] array we are (so what's the newest data)

		unsigned int packet_id;

    // GPS accuracy things
    TinyGPSCustom* vdop;

    // Previous GPS data to check if we are still receiving new data
    double last_lat;
    double last_lon;
    double last_alt;
    uint8_t gps_error_count;

    GPS_Altitude _gps_altitude_history[10];


     /*
      * Get the GPS data from the serial connection and encode the data
      * 
      * @return [void]
      */
      void _encode_gps();

      /*
       * Save the altitude info to the _gps_altitude_history array
       * 
       * @param time [char*] A char array with the hours, minutes, seconds and centiseconds
       * @param gps_altitude [float] The altitude according to the GPS module
       * @param mpu_altitude [float] The altitude calculated using the air pressure
       * 
       * @return [void]
       */
       void _save_altitude(char* time, float gps_altitude, float mpu_altitude);

       /*
        * Calculate the current vertical velocity
        * 
        * @param time [char*] A char array with the hours, minutes, seconds and centiseconds
        * @param gps_altitude [float] The current altitude according to the GPS module
        * @param mpu_altitude [float] The current altitude calculated using the air pressure
        * @param gps_velocity [float*] The location where the vertical velocity based on the GPS data will be stored
        * @param mpu_velocity [float*] The location where the vertical velocity based on the MPU data will be stored
        * 
        * @return [void]
        */
        void _calculate_vertical_velocity(char* time, float gps_altitude, float mpu_altitude, float* gps_velocity, float* mpu_velocity);

       /*
        * Calculate the expected time until the Can hits the ground
        * 
        * @param time [char*] A char array with the hours, minutes, seconds and centiseconds
        * @param gps_altitude [float] The current altitude according to the GPS module
        * @param mpu_altitude [float] The current altitude calculated using the air pressure
        * @param gps_time_until_impact [int*] The location where the expected time until impact according to the GPS data will be stored
        * @param ,[i_time_until_impact [int*] The location where the expected time until impact according to the MPU data will be stored
        * 
        * @return [void]
        */
        void _calculate_expected_time_until_impact(char* time, float gps_altitude, float mpu_altitude, int* gps_time_until_impact, int* mpu_time_until_impact);

	public:
//    static const HardwareSerial RADIO = Serial1;
    float sea_level_hPa; // Used to calculate the altitude based on the current air pressure

    static const int error_led = 4;
 
		/*
		 * Sets the radioSerial variable. 
		 * Creates a new instance of GPS and initializes that instance
		 * Creates a new instance of Adafruit_BMP280. 
		 *  
		 * @param radioSetPin [int] The pin where the set pin on the radio module is connected to, for configuring purposes
     * @param sea_level_hPa [float] The hPa at sea level 
		 * 
		 * @return void
		 */
		Can(int radioSetPin, float sea_level_hPa);
   
    // The Adafruit_BMP280 object to communicate with the BMP280 module
		Adafruit_BMP280 bmp;
    // The Adafruit_MPU6050 object to communicate with the MPU6050 module
		Adafruit_MPU6050 mpu;

		/*
		 * Begins all the serial ports and modules. 
		 * 
		 * @return error [uint8_t] If this number is higher than 0 an error occured, the value of the number will tell what the error was
		*/
		uint8_t begin();

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
