#ifndef BlueTooth_h
#define BlueTooth_h

#include "Arduino.h"

// librarys
#include <Timer.h>
#include <PacketProtocol.h>
#include <ConnectionTracker.h>

// files
#include "PacketData.h"
#include "PacketWheel.h"
#include "Icons.h"

#include "../Globals.h"

PacketProtocol packetProtocol;
ConnectionTracker connectionTracker;

PacketWheel packetWheel(12, 0);

// store those values seperately
struct BluetoothDeviceData
{
    int16_t textColor = 0xffff;
    ;
    int16_t textSize = 1;
    int16_t xCursor = 0;
    int16_t yCursor = 0;

    bool hasBattery = false;
    bool hasName = false;

    String name;
};

BluetoothDeviceData bluetoothDeviceData;

#include "onPacketReceived.h"

void sendPacketRequestName()
{
    packetProtocol.sendPacket(PKT_NAME_REQUEST);
}

void sendPacketBattery()
{
    const uint8_t bufLen = 1;
    uint8_t buf[bufLen];

    buf[0] = display.battery.percent;

    packetProtocol.sendPacket(PKT_BATTERY_PERCENT, buf, bufLen);
}

void sendPackets()
{
    sendPacketBattery();

    if (bluetoothDeviceData.hasName == false)
    {
        sendPacketRequestName();
    }
}

void onBTDeviceDisconnected()
{
    // set no battery, screen is clear while writing appName
    bluetoothDeviceData.hasBattery = false;
    bluetoothDeviceData.hasName = false;

    // replace the connected device with appName again
    display.textLines.printConnectedDevice(" none ");
}

void onBTDeviceConnected()
{
    packetProtocol.sendPacketRaw16Bit(PKT_DISPLAY_TEXT_COLOR, TFT_WHITE);

    // textsize 0 means the connected device sets size depending on screen pixels
    packetProtocol.sendPacketRaw8Bit(PKT_DISPLAY_TEXT_SIZE, 0);
}

class Bluetooth
{
public:
    Bluetooth()
    {
    }

    void init()
    {
        packetProtocol.setStream(&Serial);
        packetProtocol.setPacketHandler(&onPacketReceived);

        connectionTracker.setTimeout(1000);
        connectionTracker.setOnDisconnected(onBTDeviceDisconnected);
        connectionTracker.setOnConnected(onBTDeviceConnected);

        setOnPacketCallbacks();

        // begin with bluetooth communication
        Serial.begin(38400, SERIAL_8N1);
        delay(1);

        drawIcon();

        screen.setCursor(14, 11);
        screen.print("Bluetooth");

        // clear space for packetWheel
        screen.fillRect(14, 2, 56, 12, TFT_BLACK);
    }

    void start()
    {
        taskTimer.every(200, sendPackets);

        bluetoothDeviceData.hasBattery = false;
        bluetoothDeviceData.hasName = false;
    }

    void stop()
    {
        taskTimer.stop(0);
    }

    bool isConnected()
    {
        return connectionTracker.isConnected();
    }

    void update()
    {
        packetProtocol.update();

        taskTimer.update();

        // check connection and calls callback functions
        connectionTracker.check();
    }

    void drawIcon()
    {
        screen.drawRGBBitmap(0, 0, bluetoothSymbol, 11, 15);
    }

private:
    Timer taskTimer;
};

#endif