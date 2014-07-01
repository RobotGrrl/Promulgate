
// this is where we parse the received info
// from the Arduino!
void received_action(char action, char cmd, int key_msg, int val, char delim) {
  
  if(DEBUG) {
    println("---RECEIVED ACTION!---");
    println("action: " + action);
    println("cmd: " + cmd);
    println("key: " + key_msg);
    println("val: " + val);
    println("delim: " + delim);
  }
  
  if(action == '@' && cmd == 'A' && delim == '!') {
    if(rgb_on[0]) bg_colour[0] = val;
    if(!rgb_on[0]) bg_colour[0] = 0;
    if(rgb_on[1]) bg_colour[1] = val;
    if(!rgb_on[1]) bg_colour[1] = 0;
    if(rgb_on[2]) bg_colour[2] = val;
    if(!rgb_on[2]) bg_colour[2] = 0;
  }
  
  if(action == '@' && cmd == 'B' && delim == '!') {
    switch(key_msg) {
      case 1:
        bg_colour[0] = 256;
        bg_colour[1] = 100;
        bg_colour[2] = 100;
      break;
      case 2:
        bg_colour[0] = 100;
        bg_colour[1] = 256;
        bg_colour[2] = 100;
      break;
      case 3:
        bg_colour[0] = 200;
        bg_colour[1] = 200;
        bg_colour[2] = 50;
      break;
      case 4:
        bg_colour[0] = 100;
        bg_colour[1] = 100;
        bg_colour[2] = 256;
      break;  
      case 5:
        bg_colour[0] = 50;
        bg_colour[1] = 50;
        bg_colour[2] = 50;
      break;
    }
  }
  
  if(action == '#' && delim == '!') {
    
    if(cmd == 'L') {
      ldr_val[0] = val;
    } else if(cmd == 'R') {
      ldr_val[1] = val;
    }
    
  }
  
}


void transmit_complete() {
  if(DEBUG) println("message sent");
}


void transmit_action(char action, char cmd, int key_msg, int val, char delim) {
  
  if(key_msg > 256) {
    println("promulgate error: key has to be <= 256");
    return;
  }
  
  if(val > 1023) {
    println("promulgate error: val has to be <= 1023");
    return;
  }
  
  String s = (action + "" + cmd + "" + key_msg + "," + val + "" + delim);
  println(s);
  if(connected) {
    arduino.write(s);
    transmit_complete();
  }
  
}


