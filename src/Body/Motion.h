#ifndef Motion_h
#define Motion_h

#include <memory>

#include "Arduino.h"

#include "../Debug.h"
#include "../Defines.h"

#include "ServoBase.h"
#include "Position.h"

enum class MotionType
{
    Sequence,
    Gait
};

class Motion
{
public:
    Motion(String name, uint8_t p, MotionType motionType)
        : m_name(name),
          m_phases(p),
          m_type(motionType),
          m_positions{new Position[m_phases]},
          m_motionCnt(0),
          m_isActive(false)
    {
    }

    MotionType getType()
    {
        return m_type;
    }

    void resetCount()
    {
        m_motionCnt = 0;
    }

    void setKnees(HexapodLegs legs, Knee pos)
    {
        setKnees(m_motionCnt, legs, pos);
    }

    void setKnees(uint8_t i, HexapodLegs legs, Knee pos)
    {
        if (i > m_phases - 1)
            return;

        m_positions[i].setKnees(legs, pos);
    }

    void setHips(HexapodLegs legs, Hip pos)
    {
        setHips(m_motionCnt, legs, pos);
    }

    void setHips(uint8_t i, HexapodLegs legs, Hip pos)
    {
        m_positions[i].setHips(legs, pos);
    }

    void next()
    {
        //Debug::print(String(millis()) + " new phase of " + print() + " " + String(m_motionCnt));

        if (m_motionCnt < m_phases - 1)
        {
            m_motionCnt++;
        }
        else
        {
            m_motionCnt = 0;
        }

        Debug::println(" -> " + String(m_motionCnt));
    }

    /*
    void rotatePositionsTo(Orientation o)
    {
        for (uint8_t i = 0; i < m_phases; i++)
        {
            // rotate all the motion phases once
            m_positions[i].rotateTo(o);
        }
    }
    */

    Position getPosition()
    {
        return m_positions[m_motionCnt];
    }

    String print()
    {
        return m_name;
    }

    void start()
    {
        Debug::println(String(millis()) + " start motion " + print());

        m_motionCnt = 0;
        m_isActive = true;
    }

    void done()
    {
        m_isActive = false;
    }

    bool isActive()
    {
        return m_isActive;
    }

    bool isLastStep()
    {
        if (m_type == MotionType::Gait)
            return false;

        if (m_motionCnt == m_phases - 1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

private:
    String m_name;
    const uint8_t m_phases;
    MotionType m_type;
    std::unique_ptr<Position[]> m_positions;
    uint8_t m_motionCnt;

    // sequence
    bool m_isActive;
};

#endif
