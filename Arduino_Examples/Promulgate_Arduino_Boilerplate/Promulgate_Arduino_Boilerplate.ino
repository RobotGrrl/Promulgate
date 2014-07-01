/*
 * Promulgate Arduino Boilerplate
 * ------------------------------
 *
 * The barebones to get started
 *
 * June 30, 2014
 * RobotGrrl.com
 *
 */

#include <Streaming.h>
#include <SoftwareSerial.h>
#include "Promulgate.h"

boolean DEBUG = false;

Promulgate promulgate = Promulgate(&Serial, &Serial);

void setup() {
  
  Serial.begin(9600);
  Serial << "Howdy!" << endl;
  
  promulgate.LOG_LEVEL = Promulgate::ERROR_;
  promulgate.set_rx_callback(received_action);
  promulgate.set_tx_callback(transmit_complete);
  
}

void loop() {

  promulgate.transmit_action('#', 'L', 1, 1023, '!');
  promulgate.transmit_action('#', 'R', 1, 15, '!');

  delay(100);
  
}

void serialEvent() {
  while(Serial.available()) {
    char c = Serial.read();
    promulgate.organize_message(c);
  }  
}


void received_action(char action, char cmd, uint8_t key, uint16_t val, char delim) {
  
  if(DEBUG) {
    Serial << "---CALLBACK---" << endl;
    Serial << "action: " << action << endl;
    Serial << "command: " << cmd << endl;
    Serial << "key: " << key << endl;
    Serial << "val: " << val << endl;
    Serial << "delim: " << delim << endl;
  }
  
}

void transmit_complete() {
  
}


