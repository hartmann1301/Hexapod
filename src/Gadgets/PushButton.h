#ifndef PushButton_h
#define PushButton_h

#include "Arduino.h"

#include "Pins.h"

#include "../Globals.h"

class PushButton
{
public:
    PushButton()
        : m_isButtonPressed(false)
    {
    }

    void init()
    {
        drawInputButton();
    }

    void update()
    {
        bool isButtonPressed = !digitalRead(inputButtonPin);

        if (m_isButtonPressed == isButtonPressed)
            return;

        m_isButtonPressed = isButtonPressed;

        Debug::println("Input Button is now " + String(digitalRead(inputButtonPin)));

        drawInputButton();
    }

    void drawInputButton()
    {
        String buttonChar = (m_isButtonPressed) ? "." : " ";

        output.print(15, 0, buttonChar, 1);
    }

private:
    bool m_isButtonPressed;
};

#endif