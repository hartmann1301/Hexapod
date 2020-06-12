#ifndef Display_h
#define Display_h

#include "Arduino.h"

#include "Pins.h"
#include "ColorDefinitions.h"
#include "Battery.h"
#include "BatteryIcon.h"
#include "TextLines.h"

#include "../RemoteControl/Wlan.h"
#include "../RemoteControl/PacketWheel.h"

struct DisplayPoint
{
    DisplayPoint()
        : x(0), y(0)
    {
    }

    DisplayPoint(uint8_t xPos, uint8_t yPos)
        : x(xPos), y(yPos)
    {
    }

    uint8_t x, y;
};

class Display
{
public:
    Display()
        : batteryIcon(70, 0)
    {
    }

    void init()
    {
        screen.begin();
        screen.fillScreen(TFT_BLACK);
        screen.setTextColor(TFT_WHITE);
        screen.setTextSize(1);
        screen.setFont(&Costum_w10_h5);

        batteryIcon.init();
        updateBattery();

        textLines.init();

        // draw line under status line
        screen.drawFastHLine(0, 15, screen.width(), TFT_LIGHTGREY);
    }

    void update()
    {
        updateBattery();
    }

    void fillTop()
    {
        uint16_t c = screen.color565(rand(), rand(), rand());

        screen.fillRect(64, 0, 32, 10, c);
    }

    void clear()
    {
        screen.fillRect(0, 16, screen.width(), 64 - 16, TFT_BLACK);
    }

    void drawPoint(DisplayPoint p)
    {
        p.x /= 4;
        p.y /= 4;

        screen.drawPixel(lastPoint.x, lastPoint.y, TFT_BLACK);

        lastPoint = p;

        screen.drawPixel(p.x, p.y, TFT_WHITE);
    }

    TextLines textLines;
    Battery battery;

private:
    void updateBattery()
    {
        battery.update();

        batteryIcon.update(battery.percent);
    }

    DisplayPoint lastPoint;
    BatteryIcon batteryIcon;
};

#endif