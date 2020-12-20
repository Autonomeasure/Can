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

class GPS {
	private:
		HardwareSerial 	*gpsSerial;
		TinyGPSPlus			 gps;

	public:
		GPS(HardwareSerial *gpsSerial); // The constructor

		void begin(uint8_t baudrate = 9600); 					// Call the begin function and set the UART baudrate
		void read(); 																	// Read gpsSerial and encode it using gps.encode
		bool get_time(char *hMsm); 										// Get the current time (hMsm (four chars)
		bool get_position(double *lat, double *lon); 	// Get the current location of the Can
		bool get_altitude(double *altitude); 					// Get the current altitude in meters
		bool get_ground_speed(double *speed); 				// Get the current ground speed in m/s
		bool get_air_speed(double *speed);						// Get the current air speed in m/s
};

#endif