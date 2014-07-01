/*
 * Promulgate Processing Boilerplate
 * ---------------------------------
 * 
 * The barebones code to get started
 *
 * June 30, 2014
 * RobotGrrl.com
 *
 */
 
import controlP5.*;
import processing.serial.*;

static final boolean DEBUG = false;

void setup() {
 
  size(400, 400);
  noStroke();
 
  initControlP5();
  
}


void draw() {
  
  background(255);
  
  if(guiVisible) {
    cp5.draw();
    cp5_v2.draw();
  }
  
  readData();
  
  //transmit_action('@', 'U', 1, 100, '!');
  
}



