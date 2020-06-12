#ifndef WlanBars_h
#define WlanBars_h

#include "Arduino.h"

#include "../Globals.h"

class WlanBars
{
public:
    WlanBars(int16_t x, int16_t y)
        : m_x(x), m_y(y)
    {
    }

    void init()
    {
        drawBar(0, TFT_WHITE);
        drawBar(1, TFT_WHITE);
        drawBar(2, TFT_WHITE);
        drawBar(3, TFT_WHITE);
    }

    void update(uint8_t percent)
    {
        
    }

private:
    void drawBar(uint8_t num, uint16_t color)
    {
        if (num > 3)
            return;

        uint8_t barHeight = (num + 1) * m_diff;
        uint8_t barTop = m_height - barHeight;

        screen.fillRect(m_x + num * m_diff, m_y + barTop, 2, barHeight, color);
    }

    int16_t m_x, m_y;
    static const uint8_t m_height = 12;
    static const uint8_t m_diff = 3;
};

#endif