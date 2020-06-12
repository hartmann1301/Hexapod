#ifndef ServoOrder_h
#define ServoOrder_h

#include "Arduino.h"

enum class HexapodServo
{
    hipLeftFront = 0,
    hipLeftMiddle,
    hipLeftBack,
    hipRightBack,
    hipRightMiddle,
    hipRightFront,
    kneeLeftFront,
    kneeLeftMiddle,
    kneeLeftBack,
    kneeRightBack,    
    kneeRightMiddle,    
    kneeRightFront,
    headHorizontal,
    headVertical
};

enum class Orientation
{
    Forward = 0,
    ForwardLeft,
    BackwardLeft,
    Backward,
    BackwardRight,
    ForwardRight
};

class ServoNames
{
public:
    const String at(uint8_t i)
    {
        return m_servoNames[i];
    }

private:
    const String m_servoNames[SERVOS] =
        {
            "hipLeftFront",
            "hipLeftMiddle",
            "hipLeftBack",
            "hipRightBack",            
            "hipRightMiddle",
            "hipRightFront",
            "kneeLeftFront",
            "kneeLeftMiddle",
            "kneeLeftBack",
            "kneeRightBack",            
            "kneeRightMiddle",
            "kneeRightFront",
            "headHorizontal",
            "headVertical"};
};

// 
ServoNames servoNames;

#endif