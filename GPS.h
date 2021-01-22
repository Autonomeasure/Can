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
#if !defined(GPS_H)
#define GPS_H
#include "TinyGPS++.h"

#include "Error.h"

typedef struct {
	char *time;
	double altitude;
}GPS_Altitude;

class GPS {
	private:
		HardwareSerial 	*gpsSerial;
		TinyGPSPlus			 gps;

	public:
		/*
		 * Sets the gpsSerial variable
		 * 
		 * @param gpsSerial [HardwareSerial*] A pointer to a hardware serial port that communicates with the GPS module
		 * 
		 * @return void
		*/
		GPS(HardwareSerial *gpsSerial);

		/*
		 * Call the begin function of the gpsSerial object and sets the UART baudrate and set the rate at which we will receive GPS data
		 * 
     * @param rate [uint16_t] The amount of times the GPS module transmits GPS data [OPTIONAL, DEFAULT IS 5 Hz]
		 * 
		 * @return void
		*/
		void begin(uint8_t rate = 5);

		/*
		 * Reads the incoming serial data from the GPS module and encodes it with TinyGPSPlus::encode
		 * 
     * @param void
     * 
		 * @return void
		*/
		void read(void);

    /*
     * Get the current time (four chars, hours, minutes, seconds, centiseconds)
     * 
     * @param errors [Error*] A pointer to an Error[] object, if any error will occur it will be saved to this object
     * @param time [char*] A char[] where the time is saved, the array has four elements: hours, minutes, seconds, centiseconds
     * 
     * @return amountOfErrors [uint8_t] This number will tell how many errors occured to access all the elemens of the [Error*] errors array
     */
		uint8_t get_time(Error *errors, char *time);

    /*
     * Get the current location (latitude and longitude) of the Can
     * 
     * @param errors [Error*] A pointer to an Error[] object, if any error will occur it will be saved to this object
     * @param lat [double*] A pointer to the variable where the latitude is going to be saved
     * @param lon [double*] A pointer to the variable where the longitude is going to be saved
     * 
     * @return amountOfErrors [uint8_t] This number will tell how many errors occured to access all the elemens of the [Error*] errors array
     */
		uint8_t get_position(Error *errors, double *lat, double *lon);

    /*
     * Get the current altitude of the Can in meters
     * 
     * @param errors [Error*] A pointer to an Error[] object, if any error will occur it will be saved to this object
     * @param altitude [double*] A pointer to the variable where the altitude of the Can is going to be saved
     * 
     * @return amountOfErrors [uint8_t] This number will tell how many errors occured to access all the elemens of the [Error*] errors array
     */
		uint8_t get_altitude(Error *errors, double *altitude);

//    /*
//     * Get the current ground speed of the Can in m/s
//     * 
//     * @param errors [Error*] A pointer to an Error[] object, if any error will occur it will be saved to this object
//     * @param speed [double*] A pointer to the variable where the ground speed of the Can is going to be saved
//     * 
//     * @return amountOfErrors [uint8_t] This number will tell how many errors occured to access all the elemens of the [Error*] errors array
//     */
//		uint8_t get_ground_speed(Error *errors, double *speed);
};

#endif
