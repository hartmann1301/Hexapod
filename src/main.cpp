#include <Arduino.h>

#include "Pins.h"
#include "Defines.h"
#include "Body/Servos.h"
#include "Body/Motions.h"
#include "Globals.h"

void setup()
{
  // this is to redirect the serial when bt is used
  Debug::init();

  // create the gaits and sequences
  Motions::init();

  // inits display, sometimes it failed so i wait a bit
  delay(100);  
  display.init();

  // inits the ps2 controller fake container
  input.init();

  // init this button to decide connection
  pinMode(inputButtonPin, INPUT_PULLUP);

  // trys to connect to prior remote, is blocking!!!
  remoteControl.init(); 

  body.init();

  // clears lower part of the screen
  display.clear();

  // draw range to display
  gadgets.init();

  statesMachine.init();

  delay(20);
}

void loop(void)
{
  display.update();

  remoteControl.update();

  input.update();

  gadgets.update();

  statesMachine.update();

  body.update();


  //input.ps2x.printToSerial();

  //Debug::println("input.rx:" + String(input.rx) + " input.ry:" + String(input.ry));
  //display.drawPoint(DisplayPoint(ps2x.Analog(PSS_LX), ps2x.Analog(PSS_LY)));
  //display.drawPoint(DisplayPoint(input.rx + 100, input.ry + 100));
  //display.fillTop();
}
