#ifndef onPacketReceived_h
#define onPacketReceived_h

#include "PS2X_lib.h"
#include "../Globals.h"

//#define DEBUG_RECEIVED_PACKETS

void onPacketReceived(const uint8_t *buffer, size_t size)
{
    // read packet type and search correct callback function
    packetProtocol.readPacket(buffer, size);

    // draw little animation in statusbar
    packetWheel.update();

    // if this is not called every few ms the connection seems to be lost
    connectionTracker.receivedPacket();
}

// on specific packed received:
void onNameStringReceived(const uint8_t *buffer, size_t size)
{
#ifdef DEBUG_RECEIVED_PACKETS
    packetProtocol.printPacketToSerial("Name String", buffer, size);
#endif

    bluetoothDeviceData.name = (char *)buffer;

    if (bluetoothDeviceData.hasName == true)
    {
        return;
    }

    bluetoothDeviceData.hasName = true;

    // replace the appName with connected device, let space for battery
    display.textLines.printConnectedDevice(bluetoothDeviceData.name);
}

void onSerialStringReceived(const uint8_t *buffer, size_t size)
{
    // do nothing because Serial Pins are conneted to BT pins
}

void onNameRequestReceived(const uint8_t *buffer, size_t size)
{
#ifdef DEBUG_RECEIVED_PACKETS
    packetProtocol.printPacketToSerial("Name Request", buffer, size);
#endif

    packetProtocol.sendString(PKT_NAME_STRING, "Hexapod");
}

void onPS2ButtonsReceived(const uint8_t *buffer, size_t size)
{
#ifdef DEBUG_RECEIVED_PACKETS
    packetProtocol.printPacketToSerial("PS2 Buttons", buffer, size);
#endif

    // check is the size is correct
    if (size != input.ps2x.buttonsDataPayload)
        return;

    input.ps2x.setButtonsData(buffer);

    input.updateLeftStick();
    input.updateRightStick();
}

void onGyroskopDataReceived(const uint8_t *buffer, size_t size)
{
#ifdef DEBUG_RECEIVED_PACKETS
    packetProtocol.printPacketToSerial("Gyroskop", buffer, size);
#endif

    uint8_t counter = 0;
    input.gyro.x = packetProtocol.get16Bit(buffer, counter);
    input.gyro.y = packetProtocol.get16Bit(buffer, counter);
    input.gyro.z = packetProtocol.get16Bit(buffer, counter);
}

void onUnknownPackedReceived(const uint8_t *buffer, size_t size)
{
    packetProtocol.printPacketToSerial("Unknown", buffer, size);
}

void setOnPacketCallbacks()
{
    // register all know packet types
    packetProtocol.addPacketType(PKT_NAME_STRING, &onNameStringReceived);
    packetProtocol.addPacketType(PKT_NAME_REQUEST, &onNameRequestReceived);
    packetProtocol.addPacketType(PKT_PS2_BUTTONS, &onPS2ButtonsReceived);
    packetProtocol.addPacketType(PKT_GYROSKOP_DATA, &onGyroskopDataReceived);
    packetProtocol.addPacketType(PKT_SERIAL_STRING, &onSerialStringReceived);
    /*
    packetProtocol.addPacketType(PKT_DISPLAY_STRING, &onDisplayStringReceived);
    packetProtocol.addPacketType(PKT_DISPLAY_TEXT_SIZE, &onDisplayTextSizeReceived);    
    packetProtocol.addPacketType(PKT_DISPLAY_TEXT_CURSOR, &onDisplayTextCursorReceived);
    packetProtocol.addPacketType(PKT_DISPLAY_TEXT_CLEAR, &onDisplayTextClearReceived);
    packetProtocol.addPacketType(PKT_DISPLAY_TEXT_COLOR, &onDisplayTextColorReceived);
    packetProtocol.addPacketType(PKT_DISPLAY_DRAW_PIXEL, &onDisplayDrawPixelReceived);    
    packetProtocol.addPacketType(PKT_DISPLAY_DRAW_LINE, &onDisplayDrawLineReceived);
    packetProtocol.addPacketType(PKT_DISPLAY_FILL_RECT, &onDisplayFillRectReceived);
    packetProtocol.addPacketType(PKT_PLAY_TONE, &onPlayToneReceived);
    packetProtocol.addPacketType(PKT_BATTERY_PERCENT, &onBatteryPercentReceived);
   */
    packetProtocol.setUnknownPacketHandler(&onUnknownPackedReceived);
}

#endif