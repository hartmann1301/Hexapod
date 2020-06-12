#ifndef Gadgets_h
#define Gadgets_h

#include "Arduino.h"

#include "Pins.h"

#include "LaserDiode.h"
#include "Ultrasonic.h"
#include "PushButton.h"

class Gadgets
{
public:
    Gadgets()
    {
    }

    void init()
    {
        laserDiode.init();
        ultrasonic.init();
        pushButton.init();
    }

    void update()
    {
        laserDiode.update();
        ultrasonic.update();
        pushButton.update();
    }

    LaserDiode laserDiode;
    Ultrasonic ultrasonic;
    PushButton pushButton;

private:
};

#endif