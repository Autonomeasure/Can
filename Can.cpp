#include "Can.h"

Can::Can(float seaLevelhPa) {
    bmp = new BMP280(seaLevelhPa);
    mpu = new MPU6050();
}

Can::~Can() {

}

// Call all the begin methods and return a boolean to check if something failed
bool Can::begin() {
    bool m = mpu->begin();
    bool b = bmp->begin();
    return b && m;
}

// This method gets called every 100ms (at least)
void Can::tick() {
    // Read all the sensor values...
    // Send a radio message with all the data
    // Check if parachutes need to be deployed
    // Check if parachutes are deployed successful if needed
    // Check if we have landed yet
}