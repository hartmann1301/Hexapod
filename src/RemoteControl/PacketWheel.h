#ifndef PacketWheel_h
#define PacketWheel_h

#include "../Globals.h"

class PacketWheel
{
public:
    PacketWheel(int16_t x, int16_t y)
        : xPos(x), yPos(y)
    {
        cnt = 0;
        nextPossibleUpdate = 0;
    }

    void update()
    {
        // check if update was called to early
        if (millis() < nextPossibleUpdate)
        {
            return;
        }

        nextPossibleUpdate = millis() + 150;

        // increment ticks
        cnt++;
        if (cnt == ticks)
        {
            cnt = 0;
        }

        // draw 4 ticks in different colors
        for (uint8_t i = 0; i < visibleTicks; i++)
        {
            uint8_t t = cnt + i;
            t %= ticks;

            drawTick(t, fadeColors[i]);
        }
    }

private:
    void drawTick(uint8_t c, uint16_t color)
    {
        for (uint8_t r = 0; r < rects; r++)
        {
            int16_t x = 0;
            int16_t y = 0;
            const uint8_t halfTicks = ticks / 2;

            // skip those rects because all those pixels are already painted
            if (c % 2 == 0 && r == 1) 
            {
                continue;
            }
                
            if (c < halfTicks)
            {
                x = xRect[r][c];
                y = yRect[r][c];
            }
            else
            {
                x = xRect[r][c - halfTicks];
                y = yRect[r][c - halfTicks];
                // invert higher half of coordinates
                x *= (-1);
                y *= (-1);
            }

            screen.fillRect(xPos + 7 + x, yPos + 7 + y, 2, 2, color);
        }
    }

    static const uint8_t ticks = 8;
    static const uint8_t rects = 3;
    static const uint8_t visibleTicks = 4;

    const int8_t xRect[rects][ticks / 2] = {{0, 5, 6, 5}, {0, 4, 5, 4}, {0, 3, 4, 3}};
    const int8_t yRect[rects][ticks / 2] = {{-6, -5, 0, 5}, {-5, -4, 0, 4}, {-4, -3, 0, 3}};

    const uint16_t veryDarkGrey = screen.color565(90, 90, 90);
    const uint16_t fadeColors[visibleTicks] = {0x0000, veryDarkGrey, 0x7BEF, 0xFFFF};

    uint32_t nextPossibleUpdate;

    int16_t xPos, yPos;
    uint8_t cnt;
};

#endif