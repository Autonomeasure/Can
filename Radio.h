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
 * This project falls under the GNU GPL-3.0 license, see LICENSE or https://www.gnu.org/licenses/gpl-3.0.txt.
 */

#if !defined(CAN_H)
#define CAN_H
#include <Arduino.h>

#define SOH 0x01 // Start of heading (SOH), the start of a radio transmission
#define EOT 0x04 // End of transmission (EOT), the end of a radio transmission

class Radio {
  public:
  /*
   * Write to the HardwareSerial::write method of the radio connection
   * 
   * @param c [uint8_t] The byte that needs to be transmissed
   * 
   * @return [void]
   */
  static void write(uint8_t c);
  
  /*
   * Convert a float to an integer
   * Multiply the input float with 100 and convert it to an integer, return the integer
   * 
   * @param input [float] The input value
   * 
   * @return output [int] The output value
   */
  static int float_to_int(float input);

  /*
   * Convert all the values that need to be converted to the correct data type and transmit all the data using the correct protocol
   * 
   * @param transmission_id [unsigned int] The transmission identifier, it starts at 0 and will increment every time a message is transmitted
   * @param bmp_temperature [float] The temperature received from the BMP280 module, it still needs to be converted to an integer using the Radio::float_to_int method
   * @param mpu_temperature [float] The temperatre received from the MPU6050 module, it still needs to be converted to an integer using the Radio::float_to_int method
   * @param pressure [float] The air pressure received from the BMP280 module
   * @param latitude [double] The latitude received from the GPS module
   * @param longitude [double] The longitude received from the GPS module
   * @param altitude [float] The altitude received from the GPS module
   * @param time [char[4]] The time received from the GPS module converted to 4 bytes
   * 
   * @return error [uint8_t] If this number is higher than 0 an error occured, the value of the number will tell what the error was
   */
  static uint8_t transmit(unsigned int transmission_id, float bmp_temperature, float mpu_temperature, float pressure, double latitude, double longitude, float altitude, char time[4]);
  
};

#endif
