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
#include "Can.h"


// -============= PRIVATE METHODS =============-

// Saves the time and altitude to the EEPROM
uint8_t Can::save_altitude_time_to_eeprom(char *time, double altitude) {
  uint8_t error;

  // Save the current time in EEPROM
  EEPROM.put(EEPROM_GPS_TIME_OFFSET + ((sizeof(char) * 4) * (gps_altitude_time_cursor + 1)), time);

  // Save the current altitude in EEPROM
  EEPROM.put(EEPROM_GPS_ALTITUDE_OFFSET + (sizeof(double) * (gps_altitude_time_cursor + 1)), altitude);

  // Shift the cursor
  gps_altitude_time_cursor++;
  gps_altitude_time_cursor %= 10;

  return error;
}

// Calculate the air speed (vertical velocity) with the data that has been saved in the EEPROM
uint8_t Can::calculate_air_speed(double *air_speed) {
  uint8_t error;

  char time1[4];
  char time2[4];
  double altitude1;
  double altitude2;

  // Get the last saved time
  EEPROM.get(EEPROM_GPS_TIME_OFFSET + gps_altitude_time_cursor , time1);

  // Get the first saved time
  EEPROM.get(EEPROM_GPS_TIME_OFFSET + ((gps_altitude_time_cursor + 9) % 10), time2);

  // Get the last saved altitude
  EEPROM.get(EEPROM_GPS_TIME_OFFSET + gps_altitude_time_cursor, altitude1);

  // Get the first saved altitutde
  EEPROM.get(EEPROM_GPS_TIME_OFFSET + ((gps_altitude_time_cursor + 9) % 10), altitude2);

  // Calculate the difference in time
  uint8_t deltaCentiSeconds = time2[3] - time1[3];
  uint8_t deltaSeconds = time2[2] - time1[2];
  uint8_t deltaMinutes = time2[1] - time1[1];

  // Calculate how many seconds have passed (hopefully 10)
  float passedSeconds = (deltaCentiSeconds / 100) + (deltaSeconds) + (deltaMinutes * 60);

  // Calculate the difference in altitude
  double deltaAltitude = altitude2 - altitude1;

  // Calculate the vertical velocity
  *air_speed = deltaAltitude / passedSeconds;

  return error;
}

// Calculate the expected amount of time it takes until the Can hits the ground
uint8_t Can::calculate_expected_time_until_impact(double altitude, double air_speed, double *exptected_time_until_impact) {
  uint8_t error;

  return error;
}


// -============= PUBLIC METHODS =============-

// The constructor, sets the radio serial and gps serial port variables. It also sets the radio SET pin variable for later use.
Can::Can(HardwareSerial *radioSerial, HardwareSerial *gpsSerial, int radioSetPin, uint8_t ticksPerSecond, float sea_level_hPa) {
	// Set all the variables
	this->radioSerial = radioSerial;
	this->radioSetPin = radioSetPin;
	this->ticksPerSecond = ticksPerSecond;

	// Create the GPS object
//	this->gps = new GPS(gpsSerial);
    this->gpsSerial = gpsSerial;

	// Create the Adafruit_BMP280 object
	this->bmp = new Adafruit_BMP280();

	// Create the new Adafruit_BMP280 objct
	this->mpu = new Adafruit_MPU6050();

  // Set the sea level hPa variable
  this->sea_level_hPa = sea_level_hPa;
}

// Call the begin function of all serial ports and set the mode of the radioSetPin as OUTPUT and set it to HIGH
uint8_t Can::begin(uint8_t radio_uart_baudrate = 4800, uint8_t gps_update_frequency = 1) {
	uint8_t error;
	this->radioSerial->begin(radio_uart_baudrate);
	this->gpsSerial->begin(9600);

	pinMode(this->radioSetPin, OUTPUT);

  // Set the radio into "production" mode
	digitalWrite(this->radioSetPin, HIGH);

	if (!this->bmp->begin()) {
		error = 1; // BMP280 module failed to initialize
	}
	if (!this->mpu->begin()) {
		error = 2; // MPU6050 module failed to initialize
	}

 // Initialize the SD card
 

	return error;
}

// Configure the APC220 module to the right settings
void Can::configureRadio() {
  // Set the APC220 in config mode
  digitalWrite(this->radioSetPin, LOW);
  delay(50);

  // Frequency is 434000 kHz = 434000. 
  // Air rate is 2400 bps = 1. 
  // Output power = 4. 
  // UART baudrate = 3. 
  // Byte check parity = 0. 

  this->radioSerial->print("w 434000 1 4 3 0");
  this->radioSerial->write(0x0D);
  this->radioSerial->write(0x0A);
  delay(10);

  // Set the APC220 in "production" mode
  digitalWrite(this->radioSetPin, HIGH);

  // Stabalize
	delay(10);
}

// Check if the APC220 is configured correctly
uint8_t Can::checkRadioConfiguration() {
  // Set the APC220 in config mode
	digitalWrite(this->radioSetPin, LOW);

  // Stabalize
	delay(10);
  
  // Request the configuration back from the APC220
	this->radioSerial->println("RD"); 
	delay(10);

	// Read the incoming data from the APC220 module
	while (this->radioSerial->available()) {
    // For testing purposes print it back to the Serial monitor
		Serial.write(this->radioSerial->read());

		// TODO Add code to check if the APC220 is configured properly
	}

  // Set the APC220 in "production" mode
	digitalWrite(this->radioSetPin, HIGH);

  // Stabalize
	delay(10);
}

// This method will gather all the data from modules and save it to the SD card and send it to the radio
uint8_t* Can::tick() {
	uint8_t error;

	// Gather the GPS data
  unsigned long start = millis();
  do {
    while (gpsSerial->available()) {
      gps.encode(gpsSerial->read());
    }
  } while(millis() - start < 300);

  float gps_altitude;

  uint8_t errorGPSTime;
  uint8_t errorGPSAltitude;
  uint8_t errorGPSPosition;

  // Get the time
  if (!gps.time.isValid()) {
    errorGPSTime = 30; // Invalid GPS time
  }

    TinyGPSTime *t = &gps.time;
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t->hour(), t->minute(), t->second());
//    Serial.print(sz);

  // Get the altitude
  if (!gps.altitude.isValid()) {
    errorGPSAltitude = 32; // Invalid GPS altitude
  }
  gps_altitude = gps.altitude.meters();

  double lat, lon;
  if (!gps.location.isValid()) {
    errorGPSPosition = 31; // Invalid GPS location
  }
  lat = gps.location.lat();
  lon = gps.location.lng();

//  if (errorGPSTime > 0) {
//    Serial.print("Error: GPS time is invalid, error code:   ");
//    Serial.println(errorGPSTime);
//  }
//  if (errorGPSPosition > 0) {
//    Serial.print("Error: GPS position is invalid, error code: ");
//    Serial.println(errorGPSPosition);
//  }
//  if (errorGPSAltitude > 0) {
//    Serial.print("Error: GPS altitude is invalid, error code: ");
//    Serial.println(errorGPSAltitude);
//  }
  
 
  float bmp_temperature = this->bmp->readTemperature();
  float pressure = this->bmp->readPressure();
  float bmp_altitude = this->bmp->readAltitude(sea_level_hPa);
  
  Vector3 acceleration;
  Vector3 gyroscope;
  float mpu_temperature;
  sensors_event_t a, g, temp;
  bool mpu_data = this->mpu->getEvent(&a, &g, &temp);

  if (!mpu_data) {
    error = 11; // Invalid MPU6050 data
    return error;
  }

  acceleration.x = a.acceleration.x;
  acceleration.y = a.acceleration.y;
  acceleration.z = a.acceleration.z;

  gyroscope.x = a.gyro.x;
  gyroscope.y = a.gyro.y;
  gyroscope.z = a.gyro.z;

  mpu_temperature = temp.temperature;

  
  Serial.print("Time: ");
  Serial.println(sz);
  Serial.print("GPS altitude: ");
  Serial.println(gps_altitude);
  Serial.print("Lat: ");
  Serial.println(lat, 6);
  Serial.print("Lon: ");
  Serial.println(lon, 6);

  Serial.print("BMP280 temperature: ");
  Serial.println(bmp_temperature);
  Serial.print("BMP280 altitude: ");
  Serial.println(bmp_altitude);
  Serial.print("MPU6050 temperature: ");
  Serial.println(mpu_temperature);
  Serial.print("Pressure: ");
  Serial.println(pressure/100);

  Serial.print("Acceleration x: ");
  Serial.println(acceleration.x);
  Serial.print("Acceleration y: ");
  Serial.println(acceleration.y);
  Serial.print("Acceleration z: ");
  Serial.println(acceleration.z);

  
  Serial.print("Gyro x: ");
  Serial.println(gyroscope.x);
  Serial.print("Gyro y: ");
  Serial.println(gyroscope.y);
  Serial.print("Gyro z: ");
  Serial.println(gyroscope.z);
  

  // Transmit all the data to the groundstation
  Serial1.println("Test123");
	
	// Check if the time and altitude should be saved
	if (this->lastPacketID % this->ticksPerSecond == 0) {
		// Yep, the time and altitude should be saved
//		/*amountOfErrors +=*/ save_altitude_time_to_eeprom(alt.time, alt.altitude);
	}

 return error;
}
