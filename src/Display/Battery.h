#ifndef Battery_h
#define Battery_h

#include "Arduino.h"

#include "../Pins.h"

class Battery
{
public:
    Battery()
    {
        nextReadVoltageTime = 0;
    }

    void init()
    {
    }

    void update()
    {
        // check if it is time to read
        if (millis() < nextReadVoltageTime)
            return;

        // read raw value from 0 to 4095
        uint16_t raw = analogRead(vBattMeasurePin);

        // set next timeout
        nextReadVoltageTime = millis() + 100;

        //Debug::println(String(millis(), DEC) + "v raw:" + String(raw));

        // this is the voltage measured the pin
        voltage = raw / 396.9;

        //Debug::println("v adc:" + String(voltage, 2));

        // this is because of the voltage dividor
        voltage *= 3.08;

        //Debug::println("voltage:" + String(voltage, 2));

        const float vInvalid = 2.0;
        const float vEmpty = 5.9;
        const float vFull = 8.0;
        const float vRange = vFull - vEmpty;

        if (voltage < vInvalid)
        {
            percent = 0xff;
        }
        else if (voltage < vEmpty)
        {
            percent = 0;
        }
        else if (voltage > vFull)
        {
            percent = 100;
        }
        else
        {
            percent = ((voltage - vEmpty) * 100) / vRange;
        }

        //Debug::println("percent:" + String(percent));
    }

    float voltage;
    uint8_t percent;

private:
    uint32_t nextReadVoltageTime;
};

#endif