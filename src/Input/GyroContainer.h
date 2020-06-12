#ifndef GyroContainer_h
#define GyroContainer_h

#include "Arduino.h"

class GyroContainer
{
public:
    GyroContainer()
        : x(0), y(0), z(0)
    {
    }
    
    int16_t x, y, z;
};

#endif