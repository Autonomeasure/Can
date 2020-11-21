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
#include "BMP280.h"
#include "MPU6050.h"

class Can {
private:
    BMP280 *bmp;
    MPU *mpu;
    
    // The state of the can:
    // 00000000 (0): Unset, check if the EEPROM memory has it, otherwise, set it to 00000001 so we know it the memory is empty
    // 00000001 (1): It is set but nothing happens yet
    // 00000010 (2): Flight in progress, ascending
    // 00000011 (3): Flight in progress, descending
    // 00000100 (4): Flight in progress, parachute should get deployed
    // 00000101 (5): Flight in progress, parachute deployed succesfully
    // 00000110 (6): Landing successful
    uint8_t STATE;

public:
    Can(float seaLevelhPa, uint8_t radio_rx, uint8_t radio_tx, uint8_t gps_rx, uint8_t gps_tx);
    ~Can();

    SoftwareSerial *radio;

    float altitude_history[10];

    bool    setState(uint8_t state); // Set the state of the Can::STATE variable and set it in the EEPROM memory
    uint8_t getState(); // Get Can::STATE and get it from EEPROM memory if necessary
    bool    begin(); // Call all the begin methods and return a boolean to check if something failed
    void    tick(); // This method gets called every 100ms

    void    getAcceleration(Vector3 *a); // Get the acceleration from the MPU module
    void    getGy(Vector3 *gy); // Get the gyroscope data from the MPU module
    void    getTemperature(float *bmpTemp, float *mpuTemp); // Get the temperatures from the BMP and MPU module
    void    getAltitude(float *altitude); // Get the altitude from the BMP module
};
#endif
