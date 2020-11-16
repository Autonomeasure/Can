#if !defined(RADIO_H)
#define RADIO_H
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Vector3.h"

struct Message {
	String raw;
	float 	*bmpTemp;
	float 	*mpuTemp;
	float 	*pressure;
	float 	*altitude;
	Vector3 *a;
	Vector3 *gy;
	uint8_t *state;
};

class Radio {
private:
	SoftwareSerial *radio;

	void prepare(Message *msg); // Prepare the raw property of the Message object

public:
	Radio(uint8_t rx, uint8_t tx);
	~Radio();

	void send(Message *msg); // Send the message to the ground station
};
#endif
