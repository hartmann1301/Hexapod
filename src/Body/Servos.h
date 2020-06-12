#ifndef Servos_h
#define Servos_h

#include <Adafruit_PWMServoDriver.h>

#include "Arduino.h"

#include "../Defines.h"

#include "ServoBase.h"
#include "Position.h"
#include "ServoOrder.h"

class Servos
{
public:
    Servos(Adafruit_PWMServoDriver *driver)
        : m_driver(driver), m_orientation(Orientation::Backward)
    {
        for (uint8_t i = 0; i < SERVOS; i++)
        {
            m_servos[i].setName(servoNames.at(i));
            m_servos[i].setDriver(m_driver);
            m_servos[i].setNumber(i);

            // set all servos free except the knees
            if (isHip(i) || isHead(i))
            {
                m_servos[i].setFree(true);
            }

            const uint8_t kneeMin = 5;   // down unter the body
            const uint8_t kneeMax = 175; // up in the air
            if (isKnee(i))
            {
                m_servos[i].setDegreeBorders(kneeMin, kneeMax);
            }

            const uint8_t hipMin = 40;  // forward to head
            const uint8_t hipMax = 160; // back to display
            if (isHip(i))
            {
                m_servos[i].setDegreeBorders(hipMin, hipMax);
            }

            m_servos[int(HexapodServo::headHorizontal)].setDegreeBorders(0, 180);
            m_servos[int(HexapodServo::headVertical)].setDegreeBorders(40, 135);
        }

        // set basic orientation
        for (uint8_t i = 0; i < SERVOS; i++)
        {
            m_orientationArray[i] = i;
        }

        // invert head
        getServo(HexapodServo::headHorizontal)->setInverted(true);

        // invert knees
        getServo(HexapodServo::kneeLeftFront)->setInverted(true);
        getServo(HexapodServo::kneeLeftMiddle)->setInverted(true);
        getServo(HexapodServo::kneeLeftBack)->setInverted(true);

        // set trimm values for knees to make every feet touch the ground
        getServo(HexapodServo::kneeLeftFront)->setTrimmDegree(5);
        getServo(HexapodServo::kneeLeftMiddle)->setTrimmDegree(1);
        getServo(HexapodServo::kneeLeftBack)->setTrimmDegree(+12);
        getServo(HexapodServo::kneeRightBack)->setTrimmDegree(2);
        getServo(HexapodServo::kneeRightMiddle)->setTrimmDegree(4);
        getServo(HexapodServo::kneeRightFront)->setTrimmDegree(-8);

        // set trimm values for the rear legs because they are to close
        getServo(HexapodServo::hipLeftBack)->setTrimmDegree(-10);
        getServo(HexapodServo::hipRightBack)->setTrimmDegree(-10);
    }

    Orientation getOrientation()
    {
        return m_orientation;
    }

    String getOrientationString()
    {
        switch (m_orientation)
        {
        case Orientation::Forward:
            return "Forward";
        case Orientation::ForwardLeft:
            return "Forward Left";
        case Orientation::BackwardLeft:
            return "Backward Left";
        case Orientation::Backward:
            return "Backward";
        case Orientation::BackwardRight:
            return "Backward Right";
        case Orientation::ForwardRight:
            return "Forward Right";
        default:
            return "Unkown";
        }
    }

    void turnOrientationLeft()
    {
        if (m_orientation == Orientation::ForwardRight) 
        {
            setOrientation(Orientation::Forward);
        }
        else
        {
            setOrientation(Orientation(int(m_orientation) + 1));
        }  
    }

    void turnOrientationRight()
    {
        if (m_orientation == Orientation::Forward) 
        {
            setOrientation(Orientation::ForwardRight);
        }
        else
        {
            setOrientation(Orientation(int(m_orientation) - 1));
        }  
    }

    void setOrientation(Orientation o)
    {
        if (m_orientation == o)
            return;

        m_orientation = o;

        for (uint8_t i = 0; i < GAIT_SERVOS; i++)
        {
            uint8_t oNew = (int(o) + i) % 6;

            if (isKnee(i))
            {
                oNew += 6;
            }

            //Debug::println(String(i) + " -> " + String(oNew));

            m_orientationArray[i] = oNew;
        }

        // invert three hip servos
        for (uint8_t j = 0; j < 6; j++)
        {
            // forward means the left three servos are inverted
            getServo(j)->setInverted(j < 3);
        }
    }

    void init()
    {
        // init servo driver
        m_driver->begin();
        m_driver->setPWMFreq(SERVO_PWM_FREQUENCY);

        // set orientation array and invert hips depending on orientation
        setOrientation(Orientation::Forward);
    }

    void moveSoftToStartPosition()
    {
        for (uint8_t i = 0; i < SERVOS; i++)
        {
            if (isHip(i))
            {
                // if the default value is 90 deg it will not work!!!
                setServoPosition(i, uint8_t(Hip::neutral));
            }
            else if (isKnee(i))
            {
                setServoPosition(i, uint8_t(Knee::stand));
            }
            else if (isHead(i))
            {
                setServoPosition(i, 90);
            }

            move();
            delay(100);
        }
    }

    void move(Position p)
    {
        setPosition(p);
        move();
    }

    void checkSideCollision(HexapodServo num, HexapodServo nextNum)
    {
        const uint8_t saveMin = 120; // to 150 is dangerous
        const uint8_t saveMax = 70;  // to 40 is dangerous

        uint8_t position = getServo(num)->getPosition();

        if (position < saveMin || position == SKIP_SERVO)
            return;

        uint8_t nextPosition = getServo(nextNum)->getPosition();

        if (nextPosition > saveMax)
            return;

        uint8_t saveDegree = 30 - (saveMax - nextPosition);

        if (position <= saveMin + saveDegree)
            return;

        Debug::println("Side Crash! change:" + String(position) + " to:" + String(saveMin + saveDegree));

        getServo(num)->set(saveMin + saveDegree);
    }

    void checkBackCollision()
    {
        const uint8_t saveMin = 80; // to 160 is dangerous

        uint8_t pLeft = getServo(HexapodServo::hipLeftBack)->getPosition();
        uint8_t pRight = getServo(HexapodServo::hipRightBack)->getPosition();

        if (pLeft < saveMin || pRight < saveMin || pLeft == SKIP_SERVO)
            return;

        uint8_t saveDegree = 80 - (pRight - saveMin);

        if (pLeft <= saveMin + saveDegree)
            return;

        Debug::println("Back Crash! change:" + String(pLeft) + " to:" + String(saveMin + saveDegree));

        getServo(HexapodServo::hipLeftBack)->set(saveMin + saveDegree);
    }

    void collisionCheck()
    {
        // maybe check for orientation

        // moves the first leg forward if collision is detected
        checkSideCollision(HexapodServo::hipLeftFront, HexapodServo::hipLeftMiddle);
        checkSideCollision(HexapodServo::hipLeftMiddle, HexapodServo::hipLeftBack);
        checkSideCollision(HexapodServo::hipRightFront, HexapodServo::hipRightMiddle);
        checkSideCollision(HexapodServo::hipRightMiddle, HexapodServo::hipRightBack);

        // moves left leg if collision is detected
        checkBackCollision();
    }

    void move()
    {
        // collision check and correction
        collisionCheck();

        for (uint8_t i = 0; i < SERVOS; i++)
        {
            m_servos[i].move();
        }
    }

    void free()
    {
        for (uint8_t i = 0; i < SERVOS; i++)
        {
            m_servos[i].free();
        }
    }

    void setPosition(Position p)
    {
        //Debug::println("set Position " + p.print());

        for (uint8_t i = 0; i < GAIT_SERVOS; i++)
        {
            uint8_t deg = p.servoDegrees[i];

            if (deg == SKIP_SERVO)
                continue;

            getServo(i)->set(deg);
        }
    }

    uint8_t getMaximumDegreesToMove(Position p)
    {
        uint8_t maxDegree = 0;
        for (uint8_t i = 0; i < GAIT_SERVOS; i++)
        {
            uint8_t deg = p.servoDegrees[i];

            if (deg == SKIP_SERVO)
                continue;

            uint8_t tmpDiff = getServo(i)->compareCurrentPositionTo(deg);

            maxDegree = max(tmpDiff, maxDegree);
        }
        //Debug::println("to Position " + p.print() + " it is maximum " + String(maxDegree));

        return maxDegree;
    }

    Position getPosition()
    {
        Position currentPosition;

        for (uint8_t i = 0; i < GAIT_SERVOS; i++)
        {
            currentPosition.servoDegrees[i] = getServo(i)->getPosition();
        }

        return currentPosition;
    }

    ServoBase *getServo(HexapodServo s)
    {
        return getServo(uint8_t(s));
    }

    ServoBase *getServo(uint8_t s)
    {
        if (s >= SERVOS)
            return nullptr;

        uint8_t sRotated = m_orientationArray[s];

        return &m_servos[sRotated];
    }

    void setServoPosition(HexapodServo s, uint8_t deg)
    {
        setServoPosition(uint8_t(s), deg);
    }

    void setServoPosition(uint8_t s, uint8_t deg)
    {
        if (s >= SERVOS)
            return;

        uint8_t sRotated = m_orientationArray[s];

        m_servos[sRotated].set(deg);
    }

    uint8_t getServoPosition(HexapodServo s)
    {
        return getServoPosition(uint8_t(s));
    }

    uint8_t getServoPosition(uint8_t s)
    {
        if (s >= SERVOS)
            return SKIP_SERVO;

        uint8_t sRotated = m_orientationArray[s];

        return m_servos[sRotated].getPosition();
    }

    void setServoWithAnalogStick(uint8_t i, int8_t stickValue, bool invert = false)
    {
        setServoWithAnalogStick(HexapodServo(i), stickValue, invert);
    }

    void setServoWithAnalogStick(HexapodServo s, int8_t stickValue, bool invert = false)
    {
        int8_t valueHigh = (invert) ? -100 : 100;
        int8_t valueLow = (invert) ? 100 : -100;

        uint8_t deg = map(stickValue, valueLow, valueHigh, 0, 180);

        //Debug::println("setDegree" + String(deg));
        setServoPosition(s, deg);
    }

    // depending on plug position in driver
    inline bool isHip(uint8_t s) { return (s < 6); }
    inline bool isKnee(uint8_t s) { return (s >= 6 && s < 12); }
    inline bool isHead(uint8_t s) { return (s == 12 || s == 13); }
    inline bool isLeft(uint8_t s) { return (0 || 1 || 2 || 7 || 8 || 9); }
    inline bool isRight(uint8_t s) { return (4 || 5 || 6 || 10 || 11 || 12); }
    // is front

private:
    Adafruit_PWMServoDriver *m_driver;

    Orientation m_orientation;

    uint8_t m_orientationArray[SERVOS];

    // gait servos
    ServoBase m_servos[SERVOS];
};

#endif