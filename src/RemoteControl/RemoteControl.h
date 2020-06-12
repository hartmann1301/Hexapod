#ifndef RemoteControl_h
#define RemoteControl_h

#include "Arduino.h"

#include "Wlan.h"
#include "Bluetooth.h"
#include "BlynkPins.h"

#include "../Globals.h"

#define CHANGE_CONNECTION_TIMEOUT 10000

enum class RemoteType
{
    Wlan,
    Bluetooth
};

enum class RemoteStatus
{
    notConnected,
    Connected,
};

class RemoteControl
{
public:
    RemoteControl()
        : m_type(RemoteType::Wlan), m_status(RemoteStatus::notConnected)
    {
        m_changeConnection = 0;
    }

    void init()
    {
        // read push button and decide remote type
        m_type = (digitalRead(inputButtonPin)) ? RemoteType::Bluetooth : RemoteType::Wlan;

        // for testing only!!!!!!!!!!!!!!
        //m_type = RemoteType::Bluetooth;

        switch (m_type)
        {
        case RemoteType::Wlan:
        {
            screen.drawRGBBitmap(16, 18, pictureSmartphone, 65, 42);

            wlan.init();
            break;
        }
        case RemoteType::Bluetooth:
        {
            m_changeConnection = millis() + CHANGE_CONNECTION_TIMEOUT;

            screen.drawRGBBitmap(23, 22, picturePS2controller, 51, 35);

            bluetooth.init();

            // wait for first bluetooth packet
            while (bluetooth.isConnected() == false && m_type == RemoteType::Bluetooth)
            {
                bluetooth.update();

                delay(5);

                checkConnectionChange();
            }

            if (bluetooth.isConnected())
                bluetooth.start();

            break;
        }
        }
    }

    void update()
    {
        switch (m_type)
        {
        case RemoteType::Wlan:
        {
            wlan.update();
            break;
        }

        case RemoteType::Bluetooth:

            bluetooth.update();
            break;
        }
    }

    void checkConnectionChange()
    {
        if (bluetooth.isConnected())
            return;

        if (millis() > m_changeConnection)
        {
            // clear screen connection image
            screen.fillRect(0, 16, 96, 48, TFT_BLACK);

            // clear statusbar space
            screen.fillRect(0, 0, 64, 15, TFT_BLACK);

            // stop cyclic messages
            bluetooth.stop();

            // is debug is enabled it sets correct
            Debug::init();

            // set new type and init again
            m_type = RemoteType::Wlan;

            screen.drawRGBBitmap(16, 18, pictureSmartphone, 65, 42);

            wlan.init();
        }
        else
        {
            // draw timeout
            uint32_t waitingTimeout = CHANGE_CONNECTION_TIMEOUT - (m_changeConnection - millis());

            uint16_t waitingPercent = (waitingTimeout * 100) / CHANGE_CONNECTION_TIMEOUT;

            uint16_t waitingLine = (waitingPercent * screen.width()) / 100;

            //Debug::println("percent:" + String(waitingPercent) + " line:" + String(waitingLine));

            screen.fillRect(0, 63, waitingLine, 1, TFT_WHITE);
        }
    }

    void setStatus(RemoteStatus status)
    {
        if (m_status == status)
            return;

        m_status = status;
    }

    void setConnection(RemoteType type)
    {
        if (m_type == type)
            return;

        m_type = type;
    }

    /*
    RemoteStatus getStatus()
    {
        return m_status;
    }
    */

    RemoteType getType()
    {
        return m_type;
    }

    Wlan wlan;
    Bluetooth bluetooth;

private:
    RemoteType m_type;
    RemoteStatus m_status;

    uint32_t m_changeConnection;
};

#endif