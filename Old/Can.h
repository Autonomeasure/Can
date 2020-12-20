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

#include <SoftwareSerial.h>
#include <EEPROM.h> // This is used to store some values (such as the state)
/*
 * EEPROM addresses:
 *    0: Can::STATE
*/

#define STATE_ADDRESS 0

#include "Vector3.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BMP280.h"
#include "Adafruit_MPU6050.h"
#include "SDCard.h"

class Can {
private:
//    BMP280 *bmp;
//    MPU *mpu;
    Adafruit_BMP280 *bmp;
    Adafruit_MPU6050 *mpu;
    
    
    // The state of the can:
    // 00000000 (0): Unset, check if the EEPROM memory has it, otherwise, set it to 00000001 so we know it the memory is empty
    // 00000001 (1): It is set but nothing happens yet
    // 00000010 (2): Flight in progress, ascending
    // 00000011 (3): Flight in progress, descending
    // 00000100 (4): Flight in progress, parachute should get deployed
    // 00000101 (5): Flight in progress, parachute deployed succesfully
    // 00000110 (6): Landing successful
    uint8_t STATE;
    unsigned int    id;

public:
    Can(float seaLevelhPa, uint8_t radio_rx, uint8_t radio_tx, uint8_t gps_rx, uint8_t gps_tx);

    SoftwareSerial *radio;
    SDCard          *sd;

    float altitude_history[10];

    bool    setState(uint8_t state); // Set the state of the Can::STATE variable and set it in the EEPROM memory
    uint8_t getState(); // Get Can::STATE and get it from EEPROM memory if necessary
    bool    begin(); // Call all the begin methods and return a boolean to check if something failed
    void    configureRadio(); // Configure the APC220 module to the right settings
    void    tick(); // This method gets called every 100ms

    void    getGy(Vector3 *a, Vector3 *gy, float *temp);  // Get the data (acceleration, gyroscope and temperature) data from the MPU6050 module
    void    getBMPPressure(float *pressure);              // Get the pressure data from the BMP280 module
    void    getBMPTemperature(float *temperature);        // Get the temperature data from the MPU6050 module
};
#endif
