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
#include "Radio.h"


// -============= PRIVATE METHODS =============-
// Get the GPS data from the serial connection and encode the data
void Can::_encode_gps() {
  // Gather the GPS data
  unsigned long start = millis();
  do {
    if (GPS.available() > 1) {
      gps.encode(GPS.read());
//      Serial.println("GPSSSS");
    }
  } while(millis() - start < 250);
}

//  Save the altitude info to the _gps_altitude_history array
void Can::_save_altitude(char* time, float gps_altitude, float mpu_altitude) {
  GPS_Altitude altitude;
  altitude.time = time;
  altitude.gps_altitude = gps_altitude;
  altitude.mpu_altitude = mpu_altitude;

  _gps_altitude_history[_gps_altitude_time_cursor] = altitude;
  _gps_altitude_time_cursor++;
}

// Calculate the current vertical velocity
void Can::_calculate_vertical_velocity(char* time, float gps_altitude, float mpu_altitude, float* gps_velocity, float* mpu_velocity) {
  // Get the altitude data from ten measurements ago
  GPS_Altitude old = _gps_altitude_history[_gps_altitude_time_cursor + 1];

  // First convert everything to seconds
  int _t_secs;
  _t_secs += time[0] * 3600; // Hours
  _t_secs += time[1] * 60; // Minutes
  _t_secs += time[2]; // Seconds

  int _old_t_secs;
  _old_t_secs += old.time[0] * 3600;
  _old_t_secs += old.time[1] * 60;
  _old_t_secs += old.time[2];

  // Calculate the difference in time
  int _d_time = _old_t_secs - _t_secs;

  // Calculate the difference in altitude
  float _d_gps_alt = old.gps_altitude - gps_altitude;
  float _d_mpu_alt = old.mpu_altitude - mpu_altitude;
  
  // Divide by the difference in time to get the velocity
  *gps_velocity = _d_gps_alt / _d_time;
  *mpu_velocity = _d_mpu_alt / _d_time;
}

// Calculate the expected time until the Can hits the ground
void Can::_calculate_expected_time_until_impact(char* time, float gps_altitude, float mpu_altitude, int* gps_time_until_impact, int* mpu_time_until_impact) {
  float gps_velocity, mpu_velocity;
  _calculate_vertical_velocity(time, gps_altitude, mpu_altitude, &gps_velocity, &mpu_velocity);

  // Calculate the time it takes until we hit the ground
  *gps_time_until_impact = gps_altitude / gps_velocity;
  *gps_time_until_impact = mpu_altitude / mpu_velocity;
}

// -============= PUBLIC METHODS =============-
// The constructor, sets the radio SET pin variable for later use.
Can::Can(int radioSetPin, float sea_level_hPa) {
	// Set all the variables
	this->radioSetPin = radioSetPin;

  // Set the sea level hPa variable
  this->sea_level_hPa = sea_level_hPa;

  vdop = new TinyGPSCustom(gps, "GPGSA", 17);

  pinMode(2, OUTPUT);
  pinMode(error_led, OUTPUT);
}

// Call the begin function of all serial ports and set the mode of the radioSetPin as OUTPUT and set it to HIGH
uint8_t Can::begin() {
  Serial.println("Can::begin");
	uint8_t error;
  RADIO.begin(9600);
  Serial1.begin(9600);
  GPS.begin(9600);

	pinMode(this->radioSetPin, OUTPUT);

  // Set the radio into "production" mode
	digitalWrite(this->radioSetPin, HIGH);

	if (!bmp.begin()) {
		error = 1; // BMP280 module failed to initialize
	}
	if (!mpu.begin()) {
		error = 2; // MPU6050 module failed to initialize
	}

 // Initialize the SD card
 
  Serial.println("End Can::begin");
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

  RADIO.print("w 434000 1 4 3 0");
  RADIO.write(0x0D);
  RADIO.write(0x0A);
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
	RADIO.println("RD"); 
	delay(10);

	// Read the incoming data from the APC220 module
	while (RADIO.available()) {
    // For testing purposes print it back to the Serial monitor
		Serial.write(RADIO.read());

		// TODO Add code to check if the APC220 is configured properly
	}

  // Set the APC220 in "production" mode
	digitalWrite(this->radioSetPin, HIGH);

  // Stabalize
	delay(10);
}

// This method will gather all the data from modules and save it to the SD card and send it to the radio
uint8_t* Can::tick() {
  // Gather GPS data
	_encode_gps();
  float gps_altitude = gps.altitude.meters(); // The GPS altitude in meters
  double lat = gps.location.lat(); // The GPS latitude
  double lon = gps.location.lng(); // The GPS longitude
  char t[] = {gps.time.hour() + 1, gps.time.minute() + 1, gps.time.second() + 1, gps.time.centisecond() + 1}; // The GPS time

  // Check if the GPS data gets updated or that we lost connection
  if (lat == last_lat && lon == last_lon && gps_altitude == last_alt) {
    gps_error_count++;
  } else {
    gps_error_count = 0;
    last_lat = lat;
    last_lon = lon;
    last_alt = gps_altitude;
  }

  // Check if the gps error count is above 5, if that is true set the error_led pin HIGH
  if (gps_error_count > 5) {
    digitalWrite(error_led, HIGH);
  }

  // Gather the BMP data
  float bmp_temperature = bmp.readTemperature();
  float pressure = bmp.readPressure();
  float bmp_altitude = bmp.readAltitude(sea_level_hPa);

  // Gather the MPU data
  float mpu_temperature;
  sensors_event_t a, g, temp;
  bool mpu_data = mpu.getEvent(&a, &g, &temp);

  Vector3 acceleration;
  Vector3 gyroscope;
  acceleration.x = a.acceleration.x;
  acceleration.y = a.acceleration.y;
  acceleration.z = a.acceleration.z;

  gyroscope.x = g.gyro.x;
  gyroscope.y = g.gyro.y;
  gyroscope.z = g.gyro.z;

  mpu_temperature = temp.temperature;

  digitalWrite(2, HIGH);
  Radio::transmit(packet_id, bmp_temperature, mpu_temperature, pressure, lat, lon, gps_altitude, t);
  digitalWrite(2, LOW);

//  Serial.print("VDOP: ");
//  Serial.println(vdop->value());

  
	
	// Check if the time and altitude should be saved
	if (packet_id % 5 == 0) {
		// Yep, the time and altitude should be saved
    _save_altitude(t, gps_altitude, bmp_altitude);
	}

  packet_id++;

  digitalWrite(error_led, LOW);
  return 0;
}
