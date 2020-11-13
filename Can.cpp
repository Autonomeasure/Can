#include "Can.h"

Can::Can(float seaLevelhPa) {
    bmp = new BMP280(seaLevelhPa);
    mpu = new MPU6050();

    STATE = 0b00000000;
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

// Set the state of the Can::STATE variable
bool Can::setState(uint8_t state) {
    // Check if the new state is lower than the last state (so we're going back??) if so return true
    if (state < STATE) {
        STATE = state;
        return true;
    }
    STATE = state;
    return false;
}

// Get Can::STATE
uint8_t Can::getState() {
    return STATE;
}

// Get the acceleration from the MPU module
void Can::getAcceleration(Vector3 *a) {
    mpu->getGy(a, nullptr, nullptr);
}

// Get the gyroscope data from the MPU module
void Can::getGy(Vector3 *gy) {
    mpu->getGy(nullptr, gy, nullptr);
}

// Get the temperatures from the BMP and MPU module
void Can::getTemperature(float *bmpTemp, float *mpuTemp) {
    bmp->getTemperature(bmpTemp);
    mpu->getGy(nullptr, nullptr, mpuTemp);
}

// Get the altitude from the BMP module
void Can::getAltitude(float *altitude) {
    bmp->getAltitude(altitude);

    // Shift everything to the left
    memcpy(altitude_history, &altitude_history[1], sizeof(altitude_history) / sizeof(float));
    altitude_history[sizeof(altitude_history) / sizeof(float) - 1] = *altitude;
}