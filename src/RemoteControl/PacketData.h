#ifndef BluetoothPacketData_h
#define BluetoothPacketData_h

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

// TODO:
//  PKT_SET_CASELEDS         // uint8_t LED mask + uint16_t Color

#endif