#ifndef Trimmer_h
#define Trimmer_h

#include "Arduino.h"

#include "Servos.h"
#include "ServoOrder.h"

#include "../Globals.h"

class Trimmer
{
public:
    Trimmer(Servos *servos)
        : m_servos(servos)
    {
        m_currentServo = 0;
        m_inputTimeout = 0;
    }

    void init()
    {
    }

    void start()
    {
        output.print(0, 1, "Servo:");

        drawServoName();
        drawServoDegrees();
    }

    void stop()
    {
        // clear lower two lines
        output.clear(0, 1);
        output.clear(0, 2);  
    }

    void update()
    {

        if (isTimeoutActive())
            return;

        if (input.ps2x.Button(PSB_L2))
        {
            nextServo();
            setTimeout();
        }
        else if (input.ps2x.Button(PSB_L1))
        {
            previousServo();
            setTimeout();
        }
        else if (input.ps2x.Button(PSB_PAD_UP))
        {
            servoPlus();            
            setTimeout();
        }
        else if (input.ps2x.Button(PSB_PAD_DOWN))
        {
            servoMinus();    
            setTimeout();
        }
        else if (input.ps2x.Button(PSB_PAD_RIGHT))
        {
            servoPlus();            
            setTimeout();
        }
        else if (input.ps2x.Button(PSB_PAD_LEFT))
        {
            servoMinus();    
            setTimeout();
        }   
    }

    void setTimeout()
    {
        m_inputTimeout = millis() + 100;
    }

    bool isTimeoutActive()
    {
        return (m_inputTimeout < millis()) ? false : true;
    }

    void servoPlus()
    {
        uint8_t deg = m_servos->getServoPosition(m_currentServo);

        uint8_t step = input.ps2x.Button(PSB_CROSS) ? 1 : 10;

        m_servos->setServoPosition(m_currentServo, deg + step);

        drawServoDegrees();
    }

    void servoMinus()
    {
        uint8_t deg = m_servos->getServoPosition(m_currentServo);

        uint8_t step = input.ps2x.Button(PSB_CROSS) ? 1 : 10;

        m_servos->setServoPosition(m_currentServo, deg - step);

        drawServoDegrees();
    }

    void nextServo()
    {
        if (m_currentServo == SERVOS - 1)
        {
            m_currentServo = 0;
        }
        else
        {
            m_currentServo++;
        }

        drawServoName();
        drawServoDegrees();
    }

    void previousServo()
    {
        if (m_currentServo == 0)
        {
            m_currentServo = SERVOS - 1;
        }
        else
        {
            m_currentServo--;
        }

        drawServoName();
        drawServoDegrees();
    }

    void drawServoName()
    {
        output.print(6, 1, m_currentServo, 2);

        output.clear(0, 2);
        output.print(0, 2, servoNames.at(m_currentServo));
    }

    void drawServoDegrees()
    {
        output.print(9, 1, m_servos->getServoPosition(m_currentServo), 3);
    }

private:
    uint8_t m_currentServo;

    uint32_t m_inputTimeout;

    Servos *m_servos;
};

#endif