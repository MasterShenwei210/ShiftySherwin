#ifndef SHIFTYSHERWIN_H
#define SHIFTYSHERWIN_H

#include "Arduino.h"


class ShiftySherwin
{
    public:
        ShiftySherwin(int num_of_reg);

        void shiftSetup(int ser, int srclk, int rclk, int srclr =0);
        void singleBitWrite(int pin, bool state);
        void singleByteWrite(int regNum, uint8_t value);
        void clearRegisters();
    private:
        void writeRegisters();
};

#endif // SHIFTYSHERWIN_H
