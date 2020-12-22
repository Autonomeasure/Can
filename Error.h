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
#if !defined(ERROR_H)
#define ERROR_H
#include <Arduino.h>

/*
 * This structure will be returned by methods that can cause any error such as the begin functions. 
 * If an error is thrown further actions may be required to fix them and proceed with the mission. 
 * 
 * Possible error identifiers and its meanings:
 *    0: No error has occured
 * 		1: BMP280 module failure, Adafruit_BMP280::begin returned false
 * 		2: MPU6050 module failure, Adafruit_MPU6050::begin returned false
 * 		3: Incorrect APC220 radio configuration
 * 
 * 		255: Unknown error
*/
typedef struct {
  uint8_t errorID; // This identifies what type of error it is. 
}Error;

#endif
