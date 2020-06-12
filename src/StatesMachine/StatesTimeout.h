#ifndef StatesTimeout_h
#define StatesTimeout_h

#include "Arduino.h"

class StatesTimeout
{
public:
    StatesTimeout(uint16_t msTime)
        : m_duration(msTime = 500), m_timeout(0)
    {
    }

    void start(uint16_t duration)
    {
        reset(duration);
    }

    void start()
    {
        start(m_duration);
    }

    void reset(uint16_t duration)
    {
        m_timeout = millis() + duration;
    }

    void reset()
    {
        m_timeout = millis() + m_duration;
    }

    void setDuration(uint16_t duration)
    {
        m_duration = duration;
    }

    bool isActive()
    {
        if (millis() < m_timeout)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

private:
    uint16_t m_duration;
    uint32_t m_timeout;
};

#endif