/*
 * Fun Little Playground
 * -----------------------
 * 
 * Just a little fun game thingy that connects
 * to an Arduino with sensors and outputs
 *
 * June 25, 2014
 * RobotGrrl.com
 *
 */
 
import controlP5.*;
import processing.serial.*;

static final boolean DEBUG = false;

int bg_colour[] = {256, 256, 256};
boolean rgb_on[] = { true, true, true };
int prev_mouse_button = 0;
int ldr_val[] = { 0, 0 }; // left, right

PImage lefty, righty;

void setup() {
 
  size(400, 600);
  noStroke();
 
  lefty = loadImage("doodle-robobrrd2.png");
  righty = loadImage("doodle-robobrrd.png");
 
  initControlP5();
  
}


void draw() {
  
  background(bg_colour[0], bg_colour[1], bg_colour[2]);
  
  int lefty_height = (int)map(ldr_val[0], 0, 1023, height-200, 80);
  int righty_height = (int)map(ldr_val[1], 0, 1023, height-200, 80);
  
  image(lefty, 20, lefty_height);
  image(righty, width-150-20, righty_height);
  
  if(guiVisible) {
    cp5.draw();
    cp5_v2.draw();
  }
  
  readData();
  
}


void keyPressed() {
  
  if(key == CODED) {
    
    switch(keyCode) {
      
      case UP:
        transmit_action('@', 'U', 1, 100, '!');
      break;
      case DOWN:
        transmit_action('@', 'D', 1, 100, '!');
      break;
      case LEFT:
        transmit_action('@', 'O', 1, 100, '!');
      break;
      case RIGHT:
        transmit_action('@', 'C', 1, 100, '!');
      break;
    }
    
  } else {
    
    switch(key) {
      case 'r':
        transmit_action('@', 'r', 1, 1, '!');
        rgb_on[0] = !rgb_on[0];
        break;
       case 'g':
         transmit_action('@', 'g', 1, 1, '!');
         rgb_on[1] = !rgb_on[1];
       break;
       case 'b':
         transmit_action('@', 'b', 1, 1, '!');
         rgb_on[2] = !rgb_on[2];
       break;
    } 
    
  }
  
}


void mousePressed() {
 
 if(mouseButton == LEFT) {
   transmit_action('@', 'L', 1, 1, '!');
 }
  
}

void mouseReleased() {
  
  transmit_action('@', 'L', 1, 0, '!');
  
}



