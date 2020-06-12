#ifndef LaserDiode_h
#define LaserDiode_h

#include "Arduino.h"

#include "Pins.h"

#include "../Globals.h"

class LaserDiode
{
public:
    LaserDiode()
        : isActive(false)
    {
    }

    void init()
    {
        // this is also done in Laserdiode.h, shoudn´t be a problem
        pinMode(triggerLaserPin, OUTPUT);

        digitalWrite(triggerLaserPin, LOW);
    }

    void on()
    {
        if (isActive == true)
            return;

        Debug::println("turn laser on");

        output.print(0, 2, "Laser is Active!");

        isActive = true;
        digitalWrite(triggerLaserPin, HIGH);
    }

    void off()
    {
        if (isActive == false)
            return;

        Debug::println("turn laser off");

        output.clear(0, 2);

        isActive = false;
        digitalWrite(triggerLaserPin, LOW);
    }

    void update()
    {
        /*
        if (input.ps2x.ButtonPressed(PSB_TRIANGLE))
        {
            if (isActive)
            {
                off();
            }
            else
            {
                on();
            }
        }
        */
    }

    bool isActive;
};

#endif