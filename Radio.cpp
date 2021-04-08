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
#include "Radio.h"

// Write to the HardwareSerial::write method of the radio connection
static void Radio::write(uint8_t c) {
  // Check if we can write at least one byte
  if (RADIO.availableForWrite() >= 1) {
    RADIO.write(c);
    return;
  }

  // The Arduino was not able to send this message, maybe create an array of transmissions that are waiting to be transmitted?
}

// Convert a float to an integer
static int Radio::float_to_int(float input) {
  // Multiply the input with 100 and convert it to a signed integer
  return int(input * 100);
}

// Convert all the values that need to be converted to the correct data type and transmit all the data using the correct protocol
static uint8_t Radio::transmit(unsigned int transmission_id, float bmp_temperature, float mpu_temperature, float pressure, double latitude, double longitude, float altitude, char* time) {
  // Convert all the values that need to be converted before the transmission can take place
  int i_bmp_temperature = Radio::float_to_int(bmp_temperature); // Convert the bmp_temperature to an integer
  int i_mpu_temperature = Radio::float_to_int(mpu_temperature); // Convert the mpu_temperature to an integer
  int i_altitude = Radio::float_to_int(altitude);               // Convert the altitude to an integer

  // Start the transmission
  Radio::write(SOH);                // Start of header
  Radio::write(transmission_id);    // Transmission identifier
  Radio::write(i_bmp_temperature);  // BMP280 temperature
  Radio::write(i_mpu_temperature);  // MPU6050 temperature
  Radio::write(pressure);           // BMP280 air pressure
  Radio::write(latitude);           // The latitude received from the GPS module
  Radio::write(longitude);          // The longitude received from the GPS module
  Radio::write(i_altitude);         // The altitude received from the GPS module
  Radio::write(time[0]);            // The hour received from the GPS module
  Radio::write(time[1]);            // The minute received from the GPS module
  Radio::write(time[2]);            // The second received from the GPS module
  Radio::write(time[3]);            // The centisecond received from the GPS module
  Radio::write(EOT);                // End of transmission

  return 0;
}
