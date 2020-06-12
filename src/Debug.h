#ifndef Debug_h
#define Debug_h

#include "Arduino.h"

//#define SERIAL_DEBUG

class Debug
{
public:
    Debug()
    {
    }

    static void init()
    {
#ifdef SERIAL_DEBUG
        Serial.begin(115200);
        println();
        println("Hexapod");
#endif
    }

    static void print(int num, uint8_t format)
    {
#ifdef SERIAL_DEBUG
        Serial.print(num, format);
#endif
    }

    static void print(const char *txt)
    {
#ifdef SERIAL_DEBUG
        Serial.print(txt);
#endif
    }

    static void print(String txt)
    {
#ifdef SERIAL_DEBUG
        Serial.print(txt);
#endif
    }

    static void println()
    {
#ifdef SERIAL_DEBUG
        Serial.println();
#endif
    }

    static void println(int num, uint8_t format)
    {
#ifdef SERIAL_DEBUG
        Serial.println(num, format);
#endif
    }

    static void println(const char *txt)
    {
#ifdef SERIAL_DEBUG
        Serial.println(txt);
#endif
    }

    static void println(String txt)
    {
#ifdef SERIAL_DEBUG
        Serial.println(txt);
#endif
    }
};

Debug debug;

#endif