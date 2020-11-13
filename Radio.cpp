#include "Radio.h"

Radio::Radio(uint8_t rx, uint8_t tx) {
	radio = new SoftwareSerial(rx, tx);
	radio->begin(9600);
}

// Prepare the raw property of the Message object
void Radio::prepare(Message *msg) {
	msg->raw += "start;millis=";
	msg->raw += millis();
	msg->raw += ";tb=";
	msg->raw += *msg->bmpTemp;
	msg->raw += ";tm=";
	msg->raw += *msg->mpuTemp;
	msg->raw += ";a=";
	msg->raw += *msg->altitude;
	msg->raw += ";p=";
	msg->raw += *msg->pressure;
	msg->raw += ";ax=";
	msg->raw += *msg->a->x;
	msg->raw += ";ay=";
	msg->raw += *msg->a->y;
	msg->raw += ";az=";
	msg->raw += *msg->a->z;
	msg->raw += ";gx=";
	msg->raw += *msg->gy->x;
	msg->raw += ";gy=";
	msg->raw += *msg->gy->y;
	msg->raw += ";gz=";
	msg->raw += *msg->gy->z;
	msg->raw += ";s=";
	msg->raw += *msg->state;
	msg->raw += ";end;";
}

// Send the message to the ground station
void Radio::send(Message *msg) {
	radio->write(&msg->raw);
}