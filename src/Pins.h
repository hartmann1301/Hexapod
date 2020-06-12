#ifndef Pins_h
#define Pins_h

/*                           _____
                     _______|-USB-|_______
                    |       |_____|      |o
                    |    Wemos D1 Mini   \
    Battery Input  -|5V                3V3|-  Output Vcc
    Ground         -|GND               D8 |-  Range Echo
    Display cs     -|D4                D7 |-  Display Mosi
    Display dc     -|D3                D6 |-  Input Button
    Servo sda      -|D2                D5 |-  Display scl
    Servo scl      -|D1                D0 |-  Range Trigger and Laser
    Bluetooth In   -|RX                A0 |-  Battery Measure
    Bluetooth Out  -|TX                RST|-  Display Reset
                     \____________________/
*/

// measure analog voltage
#define vBattMeasurePin A0

// Ultrasonic Sensor and
#define triggerLaserPin D0
#define echoPin D8

// Input button 
#define inputButtonPin D6

// SPI Display, hardware SPI with default mosi and scl
#define csPin D3
#define dcPin D4
//#define sclPin  D5
//#define mosiPin D7

// Servo Shield, hardware I2C with default pins

//#define sclPin  D1
//#define sdaPin  D2

#endif