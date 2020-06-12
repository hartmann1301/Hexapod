#ifndef Wlan_h
#define Wlan_h

#include "Arduino.h"

#include <Timer.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include "Icons.h"
#include "WlanBars.h"

#include "../Globals.h"

WidgetTerminal blynkTerminal(V0);
WidgetLCD blynkLCD(V17);

char auth[] = "f13f7632a14247448d6bdfebf74753e8"; // app with sticks

// Your WiFi credentials.
char ssid[] = "kostenloses WLAN";
char pass[] = "Kaktusfreunde 30";

class Wlan
{
public:
    Wlan()
        : wlanBars(16, 2)
    {
    }

    void init()
    {
        drawIcon();

        screen.setCursor(16, 11);
        screen.print("Wlan");

        // this may take some time
        Blynk.begin(auth, ssid, pass);

        blynkTerminal.print("Start Hexapod");
        blynkTerminal.flush();

        // clear space for bars
        screen.fillRect(16, 2, 54, 11, TFT_BLACK);

        //display.textLines.printConnectedDevice("Blynk");
        // upper line doesn´t work somehow
        screen.setCursor(31, 11);
        screen.print("Blynk");

        wlanBars.init();
    }

    void update()
    {
        Blynk.run();


        if (Blynk.connected())
        {
            wlanBars.update(100);
        }
        else
        {
        }
    }

    void drawIcon()
    {
        screen.drawRGBBitmap(0, 0, wifiSymbol, 15, 15);
    }

private:
    void updateBar()
    {
    }

    Timer taskTimer;
    WlanBars wlanBars;
};

#endif