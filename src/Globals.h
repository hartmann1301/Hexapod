#ifndef Globals_h
#define Globals_h

#include "Pins.h"
#include "Defines.h"

#include "Debug.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <Fonts/Costum_w10_h5.h>
#include <SPI.h>
Adafruit_SSD1331 screen = Adafruit_SSD1331(csPin, dcPin, 0);

// all methods and variables for the adafruit screen
#include "Display/Display.h"
Display display;

#include "Input/Input.h"
Input input;

#include "RemoteControl/RemoteControl.h"
RemoteControl remoteControl;

#include "Output/output.h"
Output output;

#include "Gadgets/Gadgets.h"
Gadgets gadgets;

#include "Body/Body.h"
Body body;

#include "StatesMachine/StatesMachine.h"
StatesMachine statesMachine;

#endif