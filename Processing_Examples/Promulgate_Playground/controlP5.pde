// -- GUI -- //
ControlP5 cp5;
ControlP5 cp5_v2;
DropdownList d1;
boolean guiVisible = true;


void initControlP5() {
  cp5 = new ControlP5(this);
  cp5_v2 = new ControlP5(this);
  cp5.setAutoDraw(false);
  cp5_v2.setAutoDraw(false);
  cp5.setMoveable(false);
  cp5_v2.setMoveable(false);
  
  loadGui();
  
  for (int i=0; i<Serial.list().length; i++) {
    if(DEBUG) println(Serial.list()[i]);
    d1.addItem("" + Serial.list()[i], i);
  }
  
  frame.setBackground(new java.awt.Color(0,0,0));
  
}


void loadGui() {
 
   d1 = cp5_v2.addDropdownList("serialports")
          .setPosition(45, 55)
          .setSize(width-200,150)
          .setBarHeight(30)
          .setCaptionLabel("Serial Ports")
          ;
          
  customize(d1);
  
  cp5.addButton("connect")
     .setValue(0)
     .setPosition(width-150, 55-30)
     .setSize(100,30)
     .setCaptionLabel("Connect!")
     .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER);
     ;
  
}


void customize(DropdownList ddl) {
  // a convenience function to customize a DropdownList
  ddl.setBackgroundColor(color(190));
  ddl.setItemHeight(30);
  //ddl.setBarHeight(15);
  ddl.getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER);
  //ddl.captionLabel().style().marginTop = 3;
  //ddl.captionLabel().style().marginLeft = 3;
  ddl.valueLabel().style().marginTop = 3;
}


void controlEvent(ControlEvent theEvent) {
  // DropdownList is of type ControlGroup.
  // A controlEvent will be triggered from inside the ControlGroup class.
  // therefore you need to check the originator of the Event with
  // if (theEvent.isGroup())
  // to avoid an error message thrown by controlP5.

  if(theEvent.isGroup()) {
    // check if the Event was triggered from a ControlGroup
    //println("event from group : "+theEvent.getGroup().getValue()+" from "+theEvent.getGroup());
    
    if(theEvent.getGroup().getName().equals("serialports")) {
      port = (int)theEvent.getGroup().getValue();
      println("selected port: " + port);
    }
    
  } else if (theEvent.isController()) {
    //println("event from controller : "+theEvent.getController().getValue()+" from "+theEvent.getController());
  }
  
}


void connect(int theValue) {
  
  if(!connected) {
  
    if(port != 99) {
      println("connected");
      connected = true;
      arduino = new Serial(this, Serial.list()[port], 9600);
      cp5.getController("connect").setCaptionLabel("Disconnect");
    } else {
      println("need to select a port!"); 
    }
  
  } else {
    
    arduino.clear();
    arduino.stop();
    
    cp5.getController("connect").setCaptionLabel("Connect!");
    //port = 99;
    connected = false;
    
  }
  
}

