#ifndef Movements_h
#define Movements_h

#include <Adafruit_PWMServoDriver.h>

#include "Arduino.h"
#include "../Defines.h"

#include "ServoBase.h"
#include "Position.h"
#include "Motion.h"
#include "Motions.h"
#include "Servos.h"

#define MILLIS_PRO_DEGREE 2
#define MILLIS_STEP_INTERVAL 20

class Movements
{
public:
    Movements(Servos *servos)
        : m_servos(servos), m_throttle(0)
    {
        m_timePhaseStart = 0;
        m_timePhaseEnd = 0;
        m_timePhaseLastCall = 0;
        m_timePhaseBreak = 0;

        m_skipPhaseIntersteps = false;
        m_endSequenceNext = false;

        m_stepHeight = 0;
        m_gaitType = 1; // should be tripod2
    }

    void init()
    {
    }

    void toggleStepHeight()
    {
        if (m_stepHeight == 2)
        {
            m_stepHeight = 0;
        }
        else
        {
            m_stepHeight++;
        }

        String h = "";

        switch (m_stepHeight)
        {
        case 0:
            Motions::setStepHeight(Knee::lowStep);
            h = "Low";
            break;
        case 1:
            Motions::setStepHeight(Knee::midStep);
            h = "Mid";
            break;
        case 2:
            Motions::setStepHeight(Knee::highStep);
            h = "High";
            break;
        }

        output.clear(0, 1);
        output.print(0, 1, String("set ") + h + String(" Steps"));
    }

    void toggleGait()
    {
        if (m_gaitType == 2)
        {
            m_gaitType = 0;
        }
        else
        {
            m_gaitType++;
        }

        setGaitMotion();

        output.clear(0, 1);
        output.print(0, 1, String("set Gait:") + m_currentMotion->print());
    }

    void setGaitMotion()
    {
        switch (m_gaitType)
        {
        case 0:
            setMotion(&gaitTripod1);
            break;
        case 1:
            setMotion(&gaitTripod2);
            break;
        case 2:
            setMotion(&gaitSwing);
            break;
        }
    }

    void setThrottle(int8_t throttle)
    {
        if (m_throttle == throttle)
            return;

        //Debug::println("setThrottle " + String(m_throttle) + " -> " + String(throttle));

        // check for gait break, to return phase after throttle went to zero
        if (m_currentMotion->getType() == MotionType::Gait)
        {
            if (m_throttle == 0 && throttle != 0 && m_timePhaseBreak != 0)
            {
                // end break
                uint32_t millisSinceBreak = millis() - m_timePhaseBreak;

                m_timePhaseStart += millisSinceBreak;
                m_timePhaseEnd += millisSinceBreak;
            }
            else if (m_throttle != 0 && throttle == 0)
            {
                // start break
                m_timePhaseBreak = millis();
            }
        }

        // set the new throttle
        m_throttle = throttle;

        // phase intersteps are not necassary if it is very fast
        if (abs(m_throttle) > 90)
        {
            m_skipPhaseIntersteps = true;
        }
        else
        {
            m_skipPhaseIntersteps = false;
        }

        //Debug::println("setThrottle to:" + String(m_throttle));

        // maybe check disabling intersteps
    }

    void update(int8_t input)
    {
        if (m_currentMotion->isActive() == false)
            return;

        setThrottle(input);

        if (m_currentMotion == &gaitTripod1 ||
            m_currentMotion == &gaitTripod2 ||
            m_currentMotion == &gaitSwing)
        {
            checkOrientation();
        }

        updatePhase();

        m_servos->move();
    }

    void setMotion(Motion *motion)
    {
        m_timePhaseEnd = millis();
        m_endSequenceNext = false;

        m_currentMotion = motion;
        m_currentMotion->start();
    }

    bool isSequenceActive()
    {
        if (m_currentMotion->getType() == MotionType::Gait)
            return false;

        if (m_currentMotion->isActive())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void printOrientation()
    {
        output.print(0, 1, m_servos->getOrientationString(), 14);
    }

private:
    void checkOrientation()
    {
        if (input.leftStick.len < 30)
            return;

        // zero degree is in front of the robot / or stick up
        int16_t deg = (input.leftStick.degree + 180) % 360;

        // get one of 12 sektors like the clock
        int8_t sektor = (deg + 30) / 30;

        //Debug::println("deg:" + String(deg) + " sektor:" + String(sektor));

        int8_t o = int(m_servos->getOrientation()) * 2;

        // same feet because of 30deg hysterese
        if (sektor == ((o - 1) + 12) % 12 ||
            sektor == o ||
            sektor == o + 1 ||
            sektor == o + 2)
            return;

        // set new hip
        setOrientation(Orientation(sektor / 2));
    }

    void setOrientation(Orientation o)
    {
        if (m_servos->getOrientation() == o)
            return;

        m_servos->setOrientation(o);

        printOrientation();

        m_positionPhaseStart = m_servos->getPosition();
        m_positionPhaseEnd = stand;

        calculatePhaseDuration();
    }

    void calculatePhaseDuration()
    {
        // calculte max diff from current to m_positionPhaseEnd
        uint8_t degreesToMove = m_servos->getMaximumDegreesToMove(m_positionPhaseEnd);

        // throttle is 100% the phaseDuration is: minDuration + (minDuration * tFaktor * 0)
        //              50%                   is: minDuration + (minDuration * tFaktor * 0,5)
        //               0%                   is: minDuration + (minDuration * tFaktor * 1)
        uint16_t minDuration = degreesToMove * MILLIS_PRO_DEGREE;
        uint16_t phaseDuration = minDuration;
        const float tFaktor = 4.0;
        phaseDuration += minDuration * (tFaktor - float(abs(m_throttle)) * (tFaktor / 100));

        /*
        Debug::print("m_throttle:" + String(m_throttle));
        Debug::print(" degreesToMove:" + String(degreesToMove));
        Debug::println(" phaseDuration:" + String(phaseDuration));
        */

        // set start and end time
        m_timePhaseLastCall = millis();
        m_timePhaseStart = millis();
        m_timePhaseEnd = m_timePhaseStart + phaseDuration;

        //Debug::println("phase will end in " + String(phaseDuration) + " at " + String(m_timePhaseEnd));
    }

    void startNewPhase()
    {
        /* report delay of the this phase
        if (millis() - m_timePhaseEnd > 5)
        {
            Debug::println(" took extra " + String(millis() - m_timePhaseEnd) + "ms");
        }
        */

        // the first time this will be critical
        m_servos->move(m_positionPhaseEnd);

        // this is the step after the last one in a sequence
        if (m_endSequenceNext)
        {
            m_currentMotion->done();
            return;
        }

        //Debug::println("startNewPhase()");

        // get start Position from current body
        m_positionPhaseStart = m_servos->getPosition();

        // get end Position from gait
        m_positionPhaseEnd = m_currentMotion->getPosition();

        // just for debug
        //m_positionPhaseEnd.print();

        calculatePhaseDuration();

        // get directly to next position
        if (m_skipPhaseIntersteps)
        {
            m_servos->move(m_positionPhaseEnd);
        }

        // check when the sequence is done to stop it in next call
        if (m_currentMotion->isLastStep())
        {
            m_endSequenceNext = true;
        }

        // now increment the current phase for the next call
        m_currentMotion->next();
    }

    void inPhaseStep()
    {
        if (m_skipPhaseIntersteps)
            return;

        // go back if not enouth time past since last call
        if (millis() < m_timePhaseLastCall + MILLIS_STEP_INTERVAL)
            return;

        // now this is between two phases
        m_timePhaseLastCall = millis();

        // calculate the time in the current phase
        float m_timePhaseLapsed = millis() - m_timePhaseStart;

        // calculate the progress in current phase, 0 is yet started and 1 means done
        float m_timePhaseProgress = m_timePhaseLapsed / (m_timePhaseEnd - m_timePhaseStart);

        // check for something strange
        if (m_timePhaseProgress < 0.0 || m_timePhaseProgress > 1.0)
            return;

        //Debug::println(String(millis()) + " inPhaseStep progress:" + String(m_timePhaseProgress, 2));

        // create temporary interphase position
        Position phasePosition;

        // move the servos in a phase
        for (int i = 0; i < GAIT_SERVOS; i++)
        {
            uint8_t pEnd = m_positionPhaseEnd.servoDegrees[i];

            if (pEnd == SKIP_SERVO)
                continue;

            // get the start and end postions
            uint8_t pStart = m_positionPhaseStart.servoDegrees[i];
            int16_t pDiff = pEnd - pStart;

            // calculate the movement linear
            uint8_t pNew = float(pDiff) * m_timePhaseProgress;

            // add the current progress to the start postion
            pNew += pStart;

            /*
            Debug::println("i:" + String(i) + " pStart:" + String(pStart) + " pEnd:" + String(pEnd));    
            Debug::println("pDiff:" + String(pDiff) + " pNew:" + String(pNew));  
            */

            phasePosition.servoDegrees[i] = pNew;
        }

        // move interphase
        m_servos->move(phasePosition);
    }

    void updatePhase()
    {
        // check if phase if done
        if (millis() > m_timePhaseEnd)
        {
            startNewPhase();
        }
        else
        {
            inPhaseStep();
        }
    }

    Servos *m_servos;

    int8_t m_throttle;

    Motion *m_currentMotion;

    uint32_t m_timePhaseStart, m_timePhaseEnd, m_timePhaseLastCall;
    uint32_t m_timePhaseBreak;

    Position m_positionPhaseStart;
    Position m_positionPhaseEnd;

    bool m_skipPhaseIntersteps;
    bool m_endSequenceNext;

    uint8_t m_stepHeight;
    uint8_t m_gaitType;
};

#endif