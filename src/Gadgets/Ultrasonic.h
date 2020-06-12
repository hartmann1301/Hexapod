#ifndef Ultrasonic_h
#define Ultrasonic_h

#include "Arduino.h"

#include "Pins.h"

#include "../Globals.h"

#include "LaserDiode.h"

class Ultrasonic
{
public:
    Ultrasonic()
        : m_nextUpdateRange(0), m_range(0)
    {
    }

    void init()
    {
        pinMode(triggerLaserPin, OUTPUT);
        pinMode(echoPin, INPUT);

        reprint();
    }

    void reprint()
    {
        output.print(0, 3, "Range:");
        output.print(10, 3, "cm");            
    }

    void update()
    {
        if (millis() < m_nextUpdateRange)
            return;

        m_nextUpdateRange = millis() + 200;

        int16_t range = getRange();

        if (m_range == range)
            return;

        m_range = range;

        if (range < 300)
            output.print(7, 3, String(m_range), 3);
    }

    int16_t getRange()
    {
        // remember laser
        bool isLaserActive = digitalRead(triggerLaserPin);

        if (isLaserActive)
        {
            // Clear the triggerLaserPin by setting it LOW:
            digitalWrite(triggerLaserPin, LOW);
            delayMicroseconds(5);
        }

        // Trigger the sensor by setting the triggerLaserPin high for 10 microseconds:
        digitalWrite(triggerLaserPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(triggerLaserPin, LOW);
        // Read the echoPin, pulseIn() returns the duration (length of the pulse) in microseconds:
        uint32_t duration = pulseIn(echoPin, HIGH);
        // Calculate the distance:
        int16_t distance = duration * 0.034 / 2;
        // Print the distance on the Serial Monitor (Ctrl+Shift+M):

        /*
        Debug::print("Distance = ");
        Debug::print(distance, DEC);
        Debug::println(" cm");
        */

        // set laser on if it was active before 
        if (isLaserActive)
        {
            digitalWrite(triggerLaserPin, HIGH);
        }

        return distance;
    }

private:
    uint32_t m_nextUpdateRange;
    int16_t m_range;
};

#endif