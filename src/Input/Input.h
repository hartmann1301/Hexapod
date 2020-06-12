#ifndef Input_h
#define Input_h

#include "Arduino.h"

#include "AnalogStick.h"
#include "PS2X_lib.h"
#include "GyroContainer.h"

class Input
{
public:
    Input()
    {
    }

    void init()
    {
        // this clears all the buttons in ps2x array
        ps2x.init_gamepad();
    }

    void update()
    {
        // this is just for the NewButtonState() function while using wlan
        ps2x.update_gamepad();
    }

    void updateLeftStick()
    {
        leftStick.setCoordinates(ps2x.Analog(PSS_LX), ps2x.Analog(PSS_LY));
    }

    void updateRightStick()
    {
        rightStick.setCoordinates(ps2x.Analog(PSS_RX), ps2x.Analog(PSS_RY));
    }

    PS2X ps2x;
    AnalogStick leftStick;
    AnalogStick rightStick;
    GyroContainer gyro;

private:
};

#endif
