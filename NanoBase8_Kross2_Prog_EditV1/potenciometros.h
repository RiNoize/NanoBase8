#ifndef potenciometros_h
#define potenciometros_h
#include <Arduino.h>

class Pot
{
public:
    Pot(byte pin, byte command, byte control, byte channel);
    void newValue(byte command, byte value, byte channel);
    byte getValue();
    int pitchvalue();
    bool pitchbend(int valr);
    int  getValueBits(byte _bits);
    byte defineSySex(byte sysex_number);
//    bool potec(bool parte, byte modificar);
    bool potec3(bool parte, byte modificar, int min, int max);
    bool potec2(bool parte, byte modificar, int min, int max);
    bool potec4(bool parte, byte modificar);
    bool potec5(bool parte, byte modificar);
    bool potec6(bool parte, byte modificar);
    bool potec7(bool parte, byte modificar, int min, int max);    
    byte Potec[18] = {0xF0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x10, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF7};
    byte Potec1[18] = {0xF0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x10, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF7}; // reservado SysEx negativo y positivo
    byte Potec2[18] = {0xF0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x10, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF7}; // reservado pote 2 bytes
    byte Pcommand;
    byte Pcontrol;
    byte Pchannel;

private:
    byte _pin;
    byte _control;
    int tmp;
    unsigned int _value;
    unsigned int _oldValue;
    unsigned int _oldbend;
    int _retvalue;
};

#endif
