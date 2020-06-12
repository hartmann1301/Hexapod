#ifndef Output_h
#define Output_h

#include "Arduino.h"

#include "../Globals.h"

class Output
{
public:
    Output()
    {
    }

    void print(uint8_t x, uint8_t y, int i, uint8_t clrLen)
    {
        print(x, y, String(i), clrLen);
    }

    void print(uint8_t x, uint8_t y, String s, uint8_t clrLen = 0)
    {
        if (clrLen > 0)
        {
            clear(x, y, clrLen);
        }

        display.textLines.setCursor(x, y);
        display.textLines.print(s);

        switch (remoteControl.getType())
        {
        case RemoteType::Wlan:
        {
            blynkLCD.print(x, y, s);
            break;
        }
        case RemoteType::Bluetooth:
        {
            // 2 int16_t: cursorX, cursorY
            packetProtocol.sendPacketRaw16Bit(PKT_DISPLAY_TEXT_CURSOR, x, y);

            // string to display
            packetProtocol.sendString(PKT_DISPLAY_STRING, s);
            break;
        }
        }
    }

    void clear(uint8_t x, uint8_t y, uint8_t len = 16)
    {
        // display on hexapod
        display.textLines.clear(x, y, len);

        // connected device
        switch (remoteControl.getType())
        {
        case RemoteType::Wlan:
        {
            String empty;
            for (uint8_t i = 0; i < len; i++)
            {
                empty += " ";
            }
            blynkLCD.print(x, y, empty);
            break;
        }
        case RemoteType::Bluetooth:
        {
            // 3 int16_t: x, y, len(of chars)
            packetProtocol.sendPacketRaw16Bit(PKT_DISPLAY_TEXT_CLEAR, x, y, len); 
            // send this twice, no idea why
            delay(1);
            packetProtocol.sendPacketRaw16Bit(PKT_DISPLAY_TEXT_CLEAR, x, y, len);
            break;
        }
        }
    }
private:
};

/*
enum
{
    PKT_NAME_STRING,         // string is name of connected device
    PKT_NAME_REQUEST,        // no data
    PKT_PS2_BUTTONS,         // 6 byte: 2 byte buttons, 4 byte sticks
    PKT_GYROSKOP_DATA,       // 3 int16_t angle float*100: x,y,z
    PKT_SERIAL_STRING,       // string to serial for debug
    PKT_DISPLAY_STRING,      // string to display
    PKT_DISPLAY_TEXT_SIZE,   // 1 uint8_t: size
    PKT_DISPLAY_TEXT_CURSOR, // 2 int16_t: cursorX, cursorY
    PKT_DISPLAY_TEXT_CLEAR,  // 3 int16_t: x, y, len(of chars)
    PKT_DISPLAY_TEXT_COLOR,  // 1 uint16_t: textcolor
    PKT_DISPLAY_DRAW_PIXEL,  // 3 uint16_t: x, y, color
    PKT_DISPLAY_DRAW_LINE,   // 4 uint16_t: x1, y1, x2, y2, color
    PKT_DISPLAY_FILL_RECT,   // 4 uint16_t: x, y, w, h, color
    PKT_PLAY_TONE,           // 2 int16_t: frequency, durration
    PKT_BATTERY_PERCENT      // 1 uint8_t: percent
};
*/

#endif
