#ifndef Position_h
#define Position_h

#include "Arduino.h"

#include "../Defines.h"
#include "../Debug.h"

#include "Servos.h"
#include "ServoOrder.h"

enum class Knee
{
    maxUp = 175,
    up = 150,
    highStep = 115,
    midStep = 95,
    lowStep = 75,
    standAwayMax = 75,  
    standAway = 55,
    stand = 45,
    standBody = 35,   
    standBodyMax = 25,
    tiptoe = 25,
    maxDown = 5
};

enum class Hip
{
    forwardMax = 40,
    forwardTurn = 60,
    forward = 70,
    neutral = 90,
    backward = 110,
    backwardTurn = 120,
    backwardMax = 140
};

enum class HexapodLegs
{
    all = 0b111111,
    left = 0b000111,
    right = 0b111000,
    front = 0b100001,
    center = 0b010010,
    back = 0b001100,
    tripodLeft = 0b010101,  // on the left two feet
    tripodRight = 0b101010, // on the right two feet
    leftOutside = 0b000101,
    rightOutside = 0b101000,
    leftFront = 0b000001,
    leftCenter = 0b000010,
    leftBack = 0b000100,
    rightBack = 0b001000,  
    rightCenter = 0b010000,
    rightFront = 0b100000
};

class Position
{
public:
    Position(String name)
        : m_name(name)
    {
        initServos();
    }

    Position()
        : m_name("")
    {
        initServos();
    }

    void setHips(HexapodLegs legs, Hip pos)
    {
        setServoPosition(legs, uint8_t(pos), true);
    }

    void setKnees(HexapodLegs legs, Knee pos)
    {
        setServoPosition(legs, uint8_t(pos), false);
    }

    String getName()
    {
        return m_name;
    }

    void print(String name)
    {
        printPosition(name);
    }

    void print()
    {
        printPosition(getName());
    }

    /*
    void rotateTo(Orientation o)
    {
        if (m_orientation == o)
            return;

        int8_t rotations = int(m_orientation) - int(o);

        if (rotations < 0)
        {
            rotations += 6;
        }

        for (uint8_t k = 0; k < rotations; k++)
        {
            // rotate all the servos once
            rotate60Degrees();
        }
    }

    void rotate60Degrees()
    {
        const uint8_t len = 6;

        // save first postion in array
        uint8_t tmpKnee = servoDegrees[0];
        uint8_t tmpHip = servoDegrees[0 + len];

        // rotate hips and knees
        for (uint8_t i = 0; i < len - 1; i++)
        {
            servoDegrees[i] = servoDegrees[i + 1];
            servoDegrees[i + len] = servoDegrees[i + 1 + len];
        }

        // put old first to the last place
        servoDegrees[len - 1] = tmpKnee;
        servoDegrees[len + len - 1] = tmpHip;
    }
    */

    uint8_t servoDegrees[GAIT_SERVOS];

private:
    void printPosition(String name)
    {
        Debug::println("Position: " + name);

        for (uint8_t i = 0; i < GAIT_SERVOS; i++)
        {
            uint8_t deg = servoDegrees[i];

            if (deg == SKIP_SERVO)
                continue;

            Debug::println("\t" + servoNames.at(i) + " to: " + String(deg));
        }
    }

    void initServos()
    {
        for (uint8_t i = 0; i < GAIT_SERVOS; i++)
        {
            servoDegrees[i] = SKIP_SERVO;
        }
    }

    void setServoPosition(HexapodLegs legs, uint8_t pos, bool isHip)
    {
        uint16 l = (uint16)legs;
        uint8_t cnt = 0;

        if (isHip == false)
        {
            cnt += 6;
        }

        //Debug::print("legs dez:" + String(l) + " bin:");
        //Debug::println(l, BIN);

        for (uint i = 0; i < 6; i++)
        {

            if (l & 0b1)
            {
                //Debug::println("set Position Servo " + String(cnt + i) + " to:" + pos);

                servoDegrees[cnt + i] = pos;
            }

            l = l >> 1;
        }
    }

    String m_name;
    
    //Orientation m_orientation;
};

#endif