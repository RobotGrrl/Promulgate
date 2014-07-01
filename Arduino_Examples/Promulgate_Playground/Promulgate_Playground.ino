/*
 * Fun Little Playground
 * ----------------------
 *
 * Just playing around with a few sensors and outputs,
 * connected to a 'game' in Processing
 *
 * June 25, 2014
 * RobotGrrl.com
 *
 */

#include <Streaming.h>
#include <SoftwareSerial.h>
#include "Promulgate.h"

boolean DEBUG = false;

// leds
uint8_t rgb[] = { 9, 10, 11 };
uint8_t led = 13;
long last_ledblink = 0;
boolean rgb_on[] = { true, true, true };
uint8_t rgb_val[] = { 0, 0, 0 };

// slider
uint8_t slider = A0;
uint16_t slider_val = 0;

// ldrs
uint8_t ldr[] = { A1, A2 };
uint16_t ldr_val[] = { 0, 0 };


Promulgate promulgate = Promulgate(&Serial, &Serial);

void setup() {
  
  Serial.begin(9600);
  Serial << "Howdy!" << endl;
  
  promulgate.LOG_LEVEL = Promulgate::ERROR_;
  promulgate.set_rx_callback(received_action);
  promulgate.set_tx_callback(transmit_complete);
  
  pinMode(led, OUTPUT);
  pinMode(rgb[0], OUTPUT);
  pinMode(rgb[1], OUTPUT);
  pinMode(rgb[2], OUTPUT);
  pinMode(slider, INPUT);
  pinMode(ldr[0], INPUT);
  pinMode(ldr[1], INPUT);
  
}

void loop() {

  slider_val = analogRead(slider);
  ldr_val[0] = analogRead(ldr[0]);
  ldr_val[1] = analogRead(ldr[1]);
  
  if(rgb_on[0]) rgb_val[0] = (int)(slider_val/4);
  if(!rgb_on[0]) rgb_val[0] = 0;
  if(rgb_on[1]) rgb_val[1] = (int)(slider_val/4);
  if(!rgb_on[1]) rgb_val[1] = 0;
  if(rgb_on[2]) rgb_val[2] = (int)(slider_val/4);
  if(!rgb_on[2]) rgb_val[2] = 0;
  
  rgb_led(rgb_val[0], rgb_val[1], rgb_val[2]);
  promulgate.transmit_action('@', 'A', 1, (int)(slider_val/4), '!');

  promulgate.transmit_action('#', 'L', 1, ldr_val[0], '!');
  promulgate.transmit_action('#', 'R', 1, ldr_val[1], '!');

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
  
  if(action == '@') {
    
    switch(cmd) {
      case 'r': {
        rgb_on[0] = !rgb_on[0];
      }
      break;
      case 'g': {
        rgb_on[1] = !rgb_on[1];
      }
      break;
      case 'b': {
        rgb_on[2] = !rgb_on[2];
      }
      break;
      case 'L': {
        if(val == 0) {
          digitalWrite(led, LOW);
        } else {
          digitalWrite(led, HIGH);
        }
      }
      break;
    }
    
  }
  
}

void transmit_complete() {
  
}


