#ifndef AnalogStick_h
#define AnalogStick_h

#include "Arduino.h"

class AnalogStick
{
public:
    AnalogStick()
        : x(0), y(0), len(0), degree(0)
    {
    }

    void setCoordinates(uint8_t newX, uint8_t newY)
    {
        x = normValue(newX);
        y = normValue(newY);

        len = calcLength(x, y);

        degree = calcDegrees(x, y);
    }

    uint8_t calcLength(int16_t x, int16_t y)
    {
        // Todo calculate degree and norm len

        return sqrt(x * x + y * y);
    }

    int8_t normValue(uint8_t v)
    {
        const uint8_t halfValue = 128;
        int16_t n = v - halfValue;

        const uint8_t minValue = 25;

        bool isNegative = false;
        if (n < 0)
        {
            isNegative = true;
            n = abs(n);
        }

        if (n < minValue)
        {
            return 0;
        }

        n = constrain(n, minValue, halfValue);
        n = map(n, minValue, halfValue, 0, 100);

        if (isNegative)
        {
            n = -n;
        }

        return n;
    }

    uint16_t calcDegrees(int16_t xPos, int16_t yPos)
    {
        if (xPos == 0 && yPos == 0)
            return 0;

        // Fast XY vector to integer deg algorithm - Jan 2011 www.RomanBlack.com
        // Converts any XY values including 0 to a deg value that should be
        // within +/- 1 deg of the accurate value without needing
        // large slow trig functions like ArcTan() or ArcCos().
        // NOTE! at least one of the X or Y values must be non-zero!
        // This is the full version, for all 4 quadrants and will generate
        // the angle in integer degrees from 0-360.
        // Any values of X and Y are usable including negative values provided
        // they are between -1456 and 1456 so the 16bit multiply does not overflow.

        unsigned char negflag;
        unsigned char tempdegree;
        unsigned char comp;
        unsigned int deg; // this will hold the result
        //signed int xPos;            // these hold the XY vector at the start
        //signed int yPos;            // (and they will be destroyed)
        unsigned int ux;
        unsigned int uy;

        // Save the sign flags then remove signs and get XY as unsigned ints
        negflag = 0;
        if (xPos < 0)
        {
            negflag += 0x01; // xPos flag bit
            xPos = (0 - xPos);     // is now +
        }
        ux = xPos; // copy to unsigned var before multiply
        if (yPos < 0)
        {
            negflag += 0x02; // yPos flag bit
            yPos = (0 - yPos);     // is now +
        }
        uy = yPos; // copy to unsigned var before multiply

        // 1. Calc the scaled "degrees"
        if (ux > uy)
        {
            deg = (uy * 45) / ux; // deg result will be 0-45 range
            negflag += 0x10;         // octant flag bit
        }
        else
        {
            deg = (ux * 45) / uy; // deg result will be 0-45 range
        }

        // 2. Compensate for the 4 deg error curve
        comp = 0;
        tempdegree = deg; // use an unsigned char for speed!
        if (tempdegree > 22) // if top half of range
        {
            if (tempdegree <= 44)
                comp++;
            if (tempdegree <= 41)
                comp++;
            if (tempdegree <= 37)
                comp++;
            if (tempdegree <= 32)
                comp++; // max is 4 degrees compensated
        }
        else // else is lower half of range
        {
            if (tempdegree >= 2)
                comp++;
            if (tempdegree >= 6)
                comp++;
            if (tempdegree >= 10)
                comp++;
            if (tempdegree >= 15)
                comp++; // max is 4 degrees compensated
        }
        deg += comp; // deg is now accurate to +/- 1 deg!

        // Invert deg if it was X>Y octant, makes 0-45 into 90-45
        if (negflag & 0x10)
            deg = (90 - deg);

        // 3. Degree is now 0-90 range for this quadrant,
        // need to invert it for whichever quadrant it was in
        if (negflag & 0x02) // if -Y
        {
            if (negflag & 0x01) // if -Y -X
                deg = (180 + deg);
            else // else is -Y +X
                deg = (180 - deg);
        }
        else // else is +Y
        {
            if (negflag & 0x01) // if +Y -X
                deg = (360 - deg);
        }

        return deg;
    }

    int8_t x, y;
    uint8_t len;
    int16_t degree;
};

#endif