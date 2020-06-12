#ifndef ServoBase_h
#define ServoBase_h

#include "Arduino.h"
#include <Adafruit_PWMServoDriver.h>

//#define DEGUG_ServoBase

class ServoBase
{
public:
    ServoBase()
    {
        m_driver = NULL;
        m_name = "none";
        m_number = 0xff;

        m_pNext = 0xff;
        m_pCurrent = m_pNext;

        m_degreeMin = 90 - 40;
        m_degreeMax = 90 + 40;
        m_degreeTrimm = 0;

        m_isInverted = false;

        m_isFree = false;
        m_useFree = false;
        m_freeTimeout = 0;
    }

    void setName(String name)
    {
        m_name = name;
    }

    void setDriver(Adafruit_PWMServoDriver *driver)
    {
        m_driver = driver;
    }

    void setNumber(uint8_t number)
    {
        m_number = number;
    }

    void setDegreeBorders(uint8_t min, uint8_t max)
    {
        m_degreeMin = min;
        m_degreeMax = max;
    }

    void setTrimmDegree(int8_t trimm)
    {
        // rotate the servo head instead of using this function
        if(abs(trimm) > 15)
            return;

        m_degreeTrimm = trimm;
    }

    void setInverted(bool inverted)
    {
        m_isInverted = inverted;
    }

    void setFree(bool free)
    {
        m_useFree = free;
    }

    void set(uint8_t pos)
    {
        // skip because the same next position was already set
        if (m_pNext == pos)
            return;

#ifdef DEGUG_ServoBase
        Debug::println("set:" + String(m_name) + " to:" + String(pos) + "°");
#endif

        m_pNext = constrain(pos, m_degreeMin, m_degreeMax);
    }

    bool isInverted()
    {
        return m_isInverted;
    }

    uint8_t compareCurrentPositionTo(uint8_t comp)
    {
        return abs(m_pCurrent - comp);
    }

    uint8_t getPosition()
    {
        return m_pNext;
    }

    void move(uint8_t pos)
    {
        set(pos);
        move();
    }

    void move()
    {
        if (m_pCurrent == m_pNext)
            return;

        // save diff for free command later
        uint8_t diff = compareCurrentPositionTo(m_pNext);

        // after this move they should be equal
        m_pCurrent = m_pNext;

        // this should not be necassary, because it´s already done in set
        uint8_t movePosition = constrain(m_pNext, m_degreeMin, m_degreeMax);

#ifdef DEGUG_ServoBase
        Debug::println("move:" + String(m_name) + " to:" + String(movePosition) + "°");
#endif

        // trimm this is a bit dangerous
        movePosition += m_degreeTrimm;

        constexpr uint16_t SERVOMIN = 150; // this is the 'minimum' pulse length count (out of 4096)
        constexpr uint16_t SERVOMAX = 540; // this is the 'maximum' pulse length count (out of 4096)

        int16_t p = 0;
        if (m_isInverted)
        {
            p = map(movePosition, 180, 0, SERVOMIN, SERVOMAX);
        }
        else
        {
            p = map(movePosition, 0, 180, SERVOMIN, SERVOMAX);
        }

        if (m_useFree)
        {
#ifdef DEGUG_ServoBase
            Debug::println(String(millis()) + " set free " + m_name + " in " + String(m_diff * 20));
#endif
            m_freeTimeout = millis() + diff * 10;

            m_isFree = false;
        }

        m_driver->setPWM(m_number, 0, p);
    }

    void free()
    {
        // do never free knees because they are important for stability
        if (m_useFree == false)
            return;

        // check if already free
        if (m_isFree)
            return;

        if (m_freeTimeout > millis())
            return;

#ifdef DEGUG_ServoBase
        Debug::println(String(millis()) + " free " + m_name);
#endif
        // stop pulses which will quickly detach the servo
        m_driver->setPin(m_number, 0, false);

        m_isFree = true;
    }

private:
    Adafruit_PWMServoDriver *m_driver;
    String m_name;
    uint8_t m_number;
    uint8_t m_degreeMin, m_degreeMax;
    uint8_t m_pCurrent, m_pNext;
    int8_t m_degreeTrimm;
    bool m_isInverted;

    bool m_isFree;
    bool m_useFree;
    uint32_t m_freeTimeout;
};

#endif