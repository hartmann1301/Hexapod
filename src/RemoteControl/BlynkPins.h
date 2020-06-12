#ifndef BlynkPins_h
#define BlynkPins_h

#define BLYNK_PRINT Serial
#define DEBUG_BLYNK

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include "../Globals.h"

BLYNK_WRITE(V1)
{
    input.ps2x.manipulateButtons(PSB_PAD_UP, param.asInt());
}

BLYNK_WRITE(V2)
{
    input.ps2x.manipulateButtons(PSB_PAD_LEFT, param.asInt());
}

BLYNK_WRITE(V3)
{
    input.ps2x.manipulateButtons(PSB_PAD_RIGHT, param.asInt());
}

BLYNK_WRITE(V4)
{
    input.ps2x.manipulateButtons(PSB_PAD_DOWN, param.asInt());
}

BLYNK_WRITE(V5)
{
    input.ps2x.manipulateButtons(PSB_TRIANGLE, param.asInt());
}

BLYNK_WRITE(V6)
{
    input.ps2x.manipulateButtons(PSB_SQUARE, param.asInt());
}

BLYNK_WRITE(V7)
{
    input.ps2x.manipulateButtons(PSB_CIRCLE, param.asInt());
}

BLYNK_WRITE(V8)
{
    input.ps2x.manipulateButtons(PSB_CROSS, param.asInt());
}

BLYNK_WRITE(V9)
{
    input.ps2x.manipulateButtons(PSB_SELECT, param.asInt());
}

BLYNK_WRITE(V10)
{
    input.ps2x.manipulateButtons(PSB_START, param.asInt());
}

BLYNK_WRITE(V11)
{
    input.ps2x.manipulateButtons(PSB_L1, param.asInt());
}

BLYNK_WRITE(V12)
{
    input.ps2x.manipulateButtons(PSB_L2, param.asInt());
}

BLYNK_WRITE(V13)
{
    input.ps2x.manipulateButtons(PSB_R2, param.asInt());
}

BLYNK_WRITE(V14)
{
    input.ps2x.manipulateButtons(PSB_R1, param.asInt());
}

BLYNK_WRITE(V15)
{
    uint8_t x = param[0].asInt();
    uint8_t y = param[1].asInt();

    // put values from blynk in ps2x byte arrary
    input.ps2x.manipulateLeftStick(x, y);

    input.updateLeftStick();
}

BLYNK_WRITE(V16)
{
    uint8_t x = param[0].asInt();
    uint8_t y = param[1].asInt();

    // put values from blynk in ps2x byte arrary
    input.ps2x.manipulateRightStick(x, y);

    input.updateRightStick();
}

BLYNK_WRITE(V18)
{
    uint8_t x = param[0].asInt();
    uint8_t y = param[1].asInt();
    uint8_t z = param[1].asInt();

    input.gyro.x = x;
    input.gyro.y = y;
    input.gyro.z = z;
}

#endif