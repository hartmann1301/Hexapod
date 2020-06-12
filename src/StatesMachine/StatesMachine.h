#ifndef StatesMachine_h
#define StatesMachine_h

#include "Arduino.h"
#include "StatesTimeout.h"

#include "../Globals.h"

enum class States
{
    stand,
    sit,
    kick,
    sequence,
    walk,
    rotateLeft,
    rotateRight,
    trimmer
};

class StatesMachine
{
public:
    StatesMachine()
        : timeoutMovement(2000), timeoutChangeState(500)
    {
        currentState = States::sit;
        nextState = States::stand;
        nextMotion = nullptr;

        m_nextPrintMs = 0;
    }

    void init()
    {
        setState(States::stand);
    }

    void update()
    {
        // do nothing else if the laser is on
        if (checkLaser())
            return;

        switch (currentState)
        {
        case States::stand:
            stateStand();
            break;
        case States::sit:
            stateSit();
            break;
        case States::sequence:
            stateSequence();
            break;
        case States::kick:
            stateKick();
            break;
        case States::walk:
            stateWalk();
            break;
        case States::rotateLeft:
            stateRotate();
            break;
        case States::rotateRight:
            stateRotate();
            break;
        case States::trimmer:
            stateTrimmer();
            break;
        }

        body.servos.move();
        body.servos.free();
    }

    void stateTrimmer()
    {
        // leave trimmer
        if (timeoutChangeState.isActive() && input.ps2x.Button(PSB_CIRCLE) && input.ps2x.Button(PSB_SQUARE))
        {
            setSequence(&toStand, States::stand);
        }

        body.trimmer.update();
    }

    void stateKick()
    {
        // leave kick
        if (timeoutChangeState.isActive() && !input.ps2x.Button(PSB_L1))
        {
            setSequence(&toStand, States::stand);
        }

        body.kick.update();
    }

    bool checkLaser()
    {
        if (input.ps2x.Button(PSB_TRIANGLE))
        {
            body.servos.setServoWithAnalogStick(HexapodServo::headHorizontal, input.leftStick.x);
            body.servos.setServoWithAnalogStick(HexapodServo::headVertical, input.leftStick.y, true);
        }
        else if (input.ps2x.Button(PSB_R1))
        {
            if (millis() > m_nextPrintMs)
            {
                m_nextPrintMs = millis() + 200;

                output.clear(0, 1);
                output.print(0, 1, String("x:") + String(input.gyro.x) + String(" y:") + String(input.gyro.y));
            }

            int16_t yContrained = constrain(input.gyro.y, -50, 50);
            int16_t xContrained = constrain(input.gyro.x, -50, 50);

            body.servos.setServoPosition(HexapodServo::headHorizontal, 90 + yContrained);
            body.servos.setServoPosition(HexapodServo::headVertical, 90 + xContrained);
        }

        if (input.ps2x.Button(PSB_TRIANGLE) || input.ps2x.Button(PSB_R1))
        {
            gadgets.laserDiode.on();

            body.servos.move();
            body.servos.free();

            return true;
        }
        else
        {
            gadgets.laserDiode.off();

            return false;
        }
    }

    void stateStand()
    {
        if (input.leftStick.len > 10)
        {
            setState(States::walk);
        }
        else if (input.rightStick.len > 0 && input.rightStick.x < -10)
        {
            setState(States::rotateLeft);
        }
        else if (input.rightStick.len > 0 && input.rightStick.x > 10)
        {
            setState(States::rotateRight);
        }
        else if (input.ps2x.Button(PSB_CROSS))
        {
            setSequence(&kickSides, &toStand, States::stand);
        }
        else if (input.ps2x.Button(PSB_CIRCLE) && input.ps2x.Button(PSB_SQUARE))
        {
            setState(States::trimmer);
        }
        else if (input.ps2x.Button(PSB_CIRCLE) && !input.ps2x.Button(PSB_SQUARE))
        {
            setSequence(&kickFrontRight, &toStand, States::stand);
        }
        else if (input.ps2x.Button(PSB_SQUARE) && !input.ps2x.Button(PSB_CIRCLE))
        {
            setSequence(&kickFrontLeft, &toStand, States::stand);
        }
        else if (input.ps2x.Button(PSB_L1))
        {
            setState(States::kick);
        }
        else if (input.ps2x.Button(PSB_PAD_LEFT))
        {
            if (timeoutChangeState.isActive())
            {
                body.movements.toggleStepHeight();

                timeoutChangeState.start();
            }
        }
        else if (input.ps2x.Button(PSB_PAD_RIGHT))
        {
            if (timeoutChangeState.isActive())
            {
                body.movements.toggleGait();

                timeoutChangeState.start();
            }
        }
        else if (input.ps2x.Button(PSB_PAD_DOWN))
        {
            setSequence(&toSit, States::sit);
        }
        else if (input.ps2x.Button(PSB_PAD_UP))
        {
            
        }
    }

    void stateSit()
    {
        // leave trimmer
        if (timeoutChangeState.isActive() && input.ps2x.Button(PSB_PAD_UP))
        {
            setSequence(&toStand, States::stand);
        }
        else if (input.ps2x.Button(PSB_PAD_LEFT))
        {
            body.servos.turnOrientationLeft();

            setSequence(&toStand, &toSit, States::sit);
        }
        else if (input.ps2x.Button(PSB_PAD_RIGHT))
        {
            body.servos.turnOrientationRight();

            setSequence(&toStand, &toSit, States::sit);
        }
        else if (input.ps2x.Button(PSB_CIRCLE))
        {
            setSequence(&kickFrontRight, &toSit, States::sit);
        }
        else if (input.ps2x.Button(PSB_SQUARE))
        {
            setSequence(&kickFrontLeft, &toSit, States::sit);
        }
        else if (input.ps2x.Button(PSB_CROSS))
        {
            setSequence(&kickFront, &toSit, States::sit);
        }
        else if (input.ps2x.Button(PSB_PAD_LEFT))
        {
            body.servos.turnOrientationLeft();

            setSequence(&toStand, &toSit, States::sit);
        }
        else if (input.ps2x.Button(PSB_PAD_RIGHT))
        {
            body.servos.turnOrientationRight();

            setSequence(&toStand, &toSit, States::sit);
        }

        // set feet
        body.servos.setServoWithAnalogStick(HexapodServo::hipLeftFront, input.leftStick.x, true);
        body.servos.setServoWithAnalogStick(HexapodServo::kneeLeftFront, input.leftStick.y, true);
        body.servos.setServoWithAnalogStick(HexapodServo::hipRightFront, input.rightStick.x);
        body.servos.setServoWithAnalogStick(HexapodServo::kneeRightFront, input.rightStick.y, true);

        // set head s
        int16_t headHorizontal = (input.leftStick.x + input.rightStick.x) / 2;
        body.servos.setServoWithAnalogStick(HexapodServo::headHorizontal, headHorizontal);
        int16_t headVertical = (input.leftStick.y + input.rightStick.y) / 2;
        body.servos.setServoWithAnalogStick(HexapodServo::headVertical, headVertical, true);

        body.servos.move();
    }

    void stateSequence()
    {
        // do the sequence
        body.movements.update(100);

        if (body.movements.isSequenceActive())
            return;

        if (nextMotion == nullptr)
        {
            // both sequences are done, go to next state
            setState(nextState);
        }
        else
        {
            body.movements.setMotion(nextMotion);

            nextMotion = nullptr;
        }
    }

    void setSequence(Motion *firstMotion, Motion *secondMotion, States next)
    {
        nextMotion = secondMotion;

        setSequence(firstMotion, next);
    }

    void setSequence(Motion *motion, States next)
    {
        setState(States::sequence);

        body.movements.setMotion(motion);

        nextState = next;
    }

    bool isTimeoutMovementActive(uint8_t input)
    {
        if (input != 0)
        {
            timeoutMovement.reset();
        }

        if (timeoutMovement.isActive())
        {
            setSequence(&toStand, States::stand);

            return true;
        }
        else
        {
            return false;
        }
    }

    void stateWalk()
    {
        if (isTimeoutMovementActive(input.leftStick.len))
            return;

        body.movements.update(input.leftStick.len);
    }

    void stateRotate()
    {
        if (isTimeoutMovementActive(input.rightStick.len))
            return;

        body.movements.update(input.rightStick.x);
    }

    void setState(States s)
    {
        if (s == currentState)
            return;

        Debug::print(String(millis()) + " change State: " + getStateName(currentState));
        Debug::println(" -> " + getStateName(s));

        endState(s);

        currentState = s;

        output.print(6, 0, getStateName(currentState), 10);

        // a sequence can be very short, every other state is debounced
        if (currentState != States::sequence)
        {
            timeoutChangeState.start();
        }

        switch (currentState)
        {
        case States::stand:
        {
            // draw mode everytime it goes to stand because than is time
            output.print(0, 0, "Mode:");
            output.clear(0, 1);

            gadgets.ultrasonic.reprint();

            body.servos.setOrientation(Orientation::Forward);
            break;
        }
        case States::sit:
        {
            body.movements.printOrientation();

            break;
        }
        case States::sequence:
        {
            break;
        }
        case States::walk:
        {
            timeoutMovement.start();

            body.movements.printOrientation();

            body.movements.setGaitMotion();
            break;
        }
        case States::rotateLeft:
        {
            timeoutMovement.start();

            body.movements.setMotion(&rotateLeft);
            break;
        }
        case States::rotateRight:
        {
            timeoutMovement.start();

            body.movements.setMotion(&rotateRight);
            break;
        }
        case States::trimmer:
        {
            body.trimmer.start();
            break;
        }
        case States::kick:
        {
            body.kick.start();
            break;
        }
        }
    }

    void endState(States s)
    {
        switch (currentState)
        {
        case States::trimmer:
            body.trimmer.stop();
            break;
        case States::kick:
            body.kick.stop();
            break;
        default:
            break;
        }
    }

    String getStateName(States s)
    {
        switch (s)
        {
        case States::stand:
            return "Stand";
        case States::sit:
            return "Sit";
        case States::sequence:
            return "Sequence";
        case States::walk:
            return "Walk";
        case States::rotateLeft:
            return "Turn Left";
        case States::rotateRight:
            return "Turn Right";
        case States::trimmer:
            return "Trimmer";
        case States::kick:
            return "Kick";
        default:
            return "Unknown";
        }
    }

private:
    StatesTimeout timeoutMovement;
    StatesTimeout timeoutChangeState;

    States currentState;

    States nextState;
    Motion *nextMotion;

    uint32_t m_nextPrintMs;
};

#endif