#ifndef Kick_h
#define Kick_h

#include "Arduino.h"

#include "Servos.h"
#include "ServoOrder.h"

#include "../Globals.h"

class Kick
{
public:
    Kick(Servos *servos)
        : m_servos(servos), m_currentHip(SKIP_SERVO), m_printCoordinatesMs(0),
          m_feetHip(0), m_feetKnee(0)
    {
    }

    void init()
    {
    }

    void start()
    {
    }

    void stop()
    {
        // clear second and third line
        output.clear(0, 1);
        output.clear(0, 2);
    }

    void update()
    {
        checkHip();

        moveFeet();

        printFeetCoordinates();
    }

private:
    void setHip(uint8_t f)
    {
        if (m_currentHip == f)
            return;

        // reset old feet to default
        if (m_currentHip != SKIP_SERVO)
        {
            m_servos->setServoPosition(m_currentHip, int(Hip::neutral));
            m_servos->setServoPosition(m_currentHip + 6, int(Knee::stand));
        }

        // set new feet
        if (f == SKIP_SERVO)
        {
            // clear second line
            output.clear(0, 2);
        }
        else
        {
            output.print(0, 2, "hor:");
            output.print(8, 2, "ver:");

            printFeetCoordinates();
        }

        // try to look to the active leg
        moveHead(f);

        // draws to screen
        String newHip = (f == SKIP_SERVO) ? " " : servoNames.at(f);
        output.clear(0, 1);
        output.print(0, 1, newHip);

        m_currentHip = f;
    }

    void moveHead(uint8_t hip)
    {
        m_servos->setServoPosition(HexapodServo::headVertical, 80);

        if (hip == SKIP_SERVO)
        {
            m_servos->setServoPosition(HexapodServo::headHorizontal, 90);
        }
        else
        {
            m_servos->setServoPosition(HexapodServo::headHorizontal, (((5 - hip) + 3) % 6) * 30 + 15);
        }
    }

    void checkHip()
    {
        if (input.leftStick.len < 50)
        {
            setHip(SKIP_SERVO);
            return;
        }

        // zero degree is in front of the robot / or stick up
        int16_t deg = (input.leftStick.degree + 180) % 360;

        // get one of 12 sektors like the clock
        uint8_t sektor = (deg + 15) / 30;

        //Debug::println("deg:" + String(deg) + " sektor:" + String(sektor));

        // same feet because of 15deg hysterese
        if (sektor == m_currentHip * 2 ||
            sektor == m_currentHip * 2 + 1 ||
            sektor == m_currentHip * 2 + 2)
            return;

        // set new hip
        setHip(sektor / 2);
    }

    void moveFeet()
    {
        if (m_currentHip == SKIP_SERVO)
            return;

        uint8_t kickHeight = map(input.rightStick.y, -100, 100, int(Knee::maxUp), int(Knee::maxDown));

        //Debug::print("kickHeight:" + String(kickHeight));
        //Debug::println(" input.rightStick.y:" + String(input.rightStick.y));

        m_servos->setServoWithAnalogStick(m_currentHip, input.rightStick.x, m_currentHip < 3);
        //m_servos->setServoWithAnalogStick(m_currentHip + 6, kickHeight, true);
        m_servos->setServoPosition(m_currentHip + 6, kickHeight);
    }

    void printFeetCoordinates()
    {
        if (m_currentHip == SKIP_SERVO)
            return;

        if (m_printCoordinatesMs > millis())
            return;

        m_printCoordinatesMs = millis() + 200;

        printFeetServo(m_feetHip, m_servos->getServoPosition(m_currentHip), 4);
        printFeetServo(m_feetKnee, m_servos->getServoPosition(m_currentHip + 6), 12);
    }

    void printFeetServo(uint8_t &storedValue, uint8_t newValue, uint8_t xOffset)
    {
        if (storedValue == newValue)
            return;

        storedValue = newValue;

        output.print(xOffset, 2, newValue, 3);
    }

    Servos *m_servos;

    uint8_t m_currentHip;

    uint32_t m_printCoordinatesMs;
    uint8_t m_feetHip, m_feetKnee;
};

#endif