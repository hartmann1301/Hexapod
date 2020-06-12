#ifndef Body_h
#define Body_h

#include "Arduino.h"

#include <Adafruit_PWMServoDriver.h>

#include "../Defines.h"
#include "Servos.h"

#include "Trimmer.h"
#include "Kick.h"
#include "Movements.h"

class Body
{
public:
    Body()
        : servoDriver(SERVO_IIC_ADDR),
          servos(&servoDriver),
          kick(&servos),
          trimmer(&servos),          
          movements(&servos)
    {
    }

    void init()
    {
        servos.init();
        movements.init();
        trimmer.init();

        servos.moveSoftToStartPosition();
    }

    void update()
    {
        servos.free();
    }

    Adafruit_PWMServoDriver servoDriver;
    Servos servos;

    Kick kick;
    Trimmer trimmer;
    Movements movements;
};

#endif