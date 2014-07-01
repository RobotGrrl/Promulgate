/*******************
Promulgate
-----------

This library lets other devices trigger actions using a very simple API.
You can also send commands to other devices to trigger actions.

Erin K / RobotGrrl - May 21, 2014
--> http://RobotGrrl.com/blog
--> http://RoboBrrd.com

MIT license, check LICENSE for more information
All text above must be included in any redistribution
*******************/

#include "Promulgate.h"


Promulgate::Promulgate(Stream *in, Stream *out) {
  in_stream = in;
  out_stream = out;
  begin();
}

void Promulgate::begin() {

  LOG_LEVEL = WARN;

  set_debug_stream(out_stream);
  reading_message = false;
  ser_len = 0;

  reset_buffer();

}

void Promulgate::set_debug_stream(Stream *d) {
  debug_stream = d;
}

void Promulgate::set_rx_callback( void (*rxCallback)(char action, char cmd, uint8_t key, uint16_t val, char delim) ) {
  _rxCallback = rxCallback;
}

void Promulgate::set_tx_callback( void (*txCallback)() ) {
  _txCallback = txCallback;
}


// Message parsing

void Promulgate::parse_message(char msg[], uint8_t len) {
  
  if(LOG_LEVEL <= DEBUG) *debug_stream << "parsing message" << endl;
  
  if(len < 3) return; // we are not looking for short messages...
  
  // get the action specifier
  char action = msg[0];
  
  if(action != '0') {
  
    // get the command
    char cmd = msg[1];
    
    // find the , to see if there is a value for the key
    uint8_t comma = 0;
    for(uint8_t i=2; i<len-2; i++) {
      if(msg[i] == ',') {
        comma = i;
        break;
      }
    }
    
    // there is no val
    boolean find_val = true;
    if(comma == 0) {
      comma = len-1;
      find_val = false;
    }
    
    // get the key number
    uint8_t key = 0;
    uint8_t lb = 2; // index of leading digit
    uint8_t ub = comma-1; // index of last digit
    
    for(uint8_t i=lb; i<=ub; i++) {
      key += ( msg[lb + (i-lb)] - '0') * pow(10, ub-i);
    }
    
    // get the val number
    uint16_t val = 0;
    lb = comma+1;
    ub = len-2;
 
    if(find_val) {
  
      for(uint8_t i=lb; i<=ub; i++) {
        val += ( msg[lb + (i-lb)] - '0' ) * pow(10, ub-i);
      }
      
    }
    
    // get the delimeter
    char delim = msg[len-1];
    
    // print it for debugging
    if(LOG_LEVEL >= DEBUG) {
      *debug_stream << "---RECEIVED---" << endl;
      *debug_stream << "Action specifier: " << action << endl;
      *debug_stream << "Command: " << cmd << endl;
      *debug_stream << "Key: " << key << endl;
      *debug_stream << "Value: " << val << endl;
      *debug_stream << "Delim: " << delim << endl;
    }
   

    if(action == '~') {
      core_action(cmd, key, val, delim);
    } else {
      _rxCallback(action, cmd, key, val, delim);
    }

  }

}


void Promulgate::organize_message(char c) {

  if(!reading_message) {
    
    // check for the action specifier
    if(c == '~' || c == '@' || c == '#' || c == '^' || c == '&') {
      reading_message = true;
      ser[ser_len] = c;
      ser_len++;
    }
    
  } else {
   
    ser[ser_len] = c;
    ser_len++;
    
    // check for the delimeter
    if(c == '!' || c == '?' || c == ';') {
      reading_message = false;
      parse_message(ser, ser_len);
      reset_buffer();
    }
    
  }


  if(ser_len >= 19) {
    // something bad has happened if we are here...
    ser_len = 0;
    if(LOG_LEVEL >= WARN) {
      *debug_stream << "Message > 20 chars, resetting and not parsing" << endl;
    }
    reading_message = false;
    reset_buffer();
  }
  
}



// Core Actions

void Promulgate::core_action(char cmd, uint8_t key, uint16_t val, char delim) {
  
  switch(cmd) {
    
    case 'D':
      if(delim == '?') {
        
        uint8_t res = digitalRead(key);
        transmit_action('>', cmd, key, res, delim);
        
      } else if(delim == '!') {
        
        if(val == 0) {
          digitalWrite(key, LOW);
        } else if(val == 1) {
          digitalWrite(key, HIGH);
        } else {
          if(LOG_LEVEL >= WARN) *debug_stream << "Unknown action for command, value: " << cmd << ", " << val << endl;
        }
        
      } else {
        if(LOG_LEVEL >= WARN) *debug_stream << "Unknown delimeter: " << delim << endl;
      }
    break;
    
    case 'A':
      if(delim == '?') {
      
        uint8_t res = analogRead(key);
        transmit_action('>', cmd, key, res, delim);
        
      } else if(delim == '!') {
        
        analogWrite(key, val);
        
      } else {
        if(LOG_LEVEL >= WARN) *debug_stream << "Unknown delimeter: " << delim << endl; 
      }
    break;
    
  }
  
}



// Send Reply

void Promulgate::transmit_action(char action, char cmd, uint8_t key, uint16_t val, char delim) {
  
  *out_stream << action << cmd << key << "," << val << delim;

  if(LOG_LEVEL >= DEBUG) {
    *debug_stream << "---TRANSMITTED---" << endl;
    *debug_stream << "Action specifier: " << action << endl;
    *debug_stream << "Command: " << cmd << endl;
    *debug_stream << "Key: " << key << endl;
    *debug_stream << "Value: " << val << endl;
    *debug_stream << "Delim: " << delim << endl;
  }

  _txCallback();

}



// Init / Reset

void Promulgate::reset_buffer() {
  
 for(uint8_t i=0; i<20; i++) {
    ser[i] = 0;
  }
  
  ser_len = 0; 
}

