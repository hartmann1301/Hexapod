#ifndef BatteryIcon_h
#define BatteryIcon_h

#include "Arduino.h"

#include "../Globals.h"
#include "ColorDefinitions.h"

class BatteryIcon
{
public:
    BatteryIcon(int16_t xPos, int16_t yPos)
        : m_x(xPos), m_y(yPos), m_currentBars(0xff), m_lastBars(0xff)
    {
    }

    void init()
    {
        drawFrame();
        clearInside();
    }

    void update(uint8_t chargedPercent)
    {
        // calculate and save last bars
        calculateBars(chargedPercent);

        if (m_lastBars != m_currentBars)
        {
            drawBars();
        }
    }

private:
    void calculateBars(uint8_t chargedPercent)
    {
        m_lastBars = m_currentBars;

        m_currentBars = chargedPercent / 15;
    }

    void drawFrame()
    {
        screen.drawRect(m_x, m_y, m_width, m_height, m_colorFrame);
        screen.fillRect(m_x + m_width, m_y + 3, 1, 8, m_colorFrame);
        screen.fillRect(m_x + m_width + 1, m_y + 4, 1, 6, m_colorFrame);
    }

    void clearInside()
    {
        screen.fillRect(m_x + 1, m_y + 1, m_width - 2, m_height - 2, TFT_BLACK);
    }

    void drawBar(uint8_t pos, uint16_t color)
    {
        // to start drawing from right to left
        pos = 6 - pos;

        screen.fillRect(m_x + 2 + pos * 3, m_y + 2, 2, 10, color);
    }

    void drawBars()
    {
        const uint16_t colorDarkGrey = screen.color565(90, 90, 90);
        const uint16_t colorCharged = getColor(m_currentBars);

        for (uint8_t i = 0; i < 7; i++)
        {
            uint16_t color = (i <= m_currentBars) ? colorCharged : colorDarkGrey;

            drawBar(i, color);
        }
    }

    uint16_t getColor(uint8_t bars)
    {

        switch (bars)
        {
        case 17:    // 0xff / 15 = 17
            return TFT_CYAN;
        case 0:
            return TFT_RED;
        case 1:
            return TFT_ORANGE;
        case 2:
            return TFT_GREENYELLOW;
        default:
            return TFT_GREEN;
        }
    }

    const int16_t m_x;
    const int16_t m_y;
    uint8_t m_currentBars, m_lastBars;

    const int16_t m_width = 24;
    const int16_t m_height = 14;
    const uint16_t m_colorFrame = TFT_LIGHTGREY;
};

#endif