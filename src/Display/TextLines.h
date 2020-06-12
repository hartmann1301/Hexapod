#ifndef TextLines_h
#define TextLines_h

#include "Arduino.h"

#include "../Globals.h"

class TextLines
{
public:
    TextLines()
    {
    }

    void init()
    {
    }

    void printConnectedDevice(String deviceName)
    {
        screen.fillRect(31, 2, 6 * 6, 10, TFT_BLACK);
        screen.setCursor(31, 11);
        screen.print(deviceName.substring(0, 6));
    }

    void setCursor(uint8_t x, uint8_t y)
    {
        screen.setCursor(x * xOffsetChar, getBaselineY(y));
    }

    void print(const char *text)
    {
        screen.print(text);
    }

    void print(String text)
    {
        screen.print(text);
    }

    void print(int num)
    {
        screen.print(String(num));
    }

    void clear(uint8_t start, uint8_t line, uint8_t len = 16)
    {
        screen.fillRect(xOffsetChar * start,
                        getBaselineY(line) - yFontHeight,
                        len * xOffsetChar,
                        yFontHeight + 2,
                        TFT_BLACK);
    }

private:
    uint8_t getBaselineY(uint8_t line)
    {
        return yOffsetTop + yOffsetLine * line;
    }

    static const uint8_t xOffsetChar = 6;
    static const uint8_t yOffsetTop = 27;
    static const uint8_t yOffsetLine = 12;
    static const uint8_t yFontHeight = 10;
};

#endif