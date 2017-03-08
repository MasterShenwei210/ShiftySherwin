#include "ShiftySherwin.h"
#include "Arduino.h"

/**
Pin Connections:
Vcc to pin 16 & 10 on register

NOTE: if fast clear function for
shift register will be used, then
pin 10 to another digital pin on
Arduino.

Gnd to pin 8 & 13 on register

serial = pin 14 on register
srclk = pin 11 on register
rclk = pin 12 on register
srclr = pin 10 on register
QH = pin 9 on register

Shift register writing order:
pins 7-> 6-> 5-> 4-> 3-> 2-> 1-> 15
so pin 0 is pin 7, 1 is 6, and so on...

NOTE:
Daisy chain multiple shift registers
by connecting QH (pin 9) to serial of
the next.

Make sure to tie together srclk and
rclk of all shift registers.
 **/

int serPin, srclkPin, rclkPin, srclrPin;
int numRegPins;
bool using_srclr;
bool regPin[800];
int i;
int n;
ShiftySherwin::ShiftySherwin(int num_of_reg)
{
    numRegPins = num_of_reg * 8;

}
/**
initialSetUp()
ENTER:
    1) the Arduino pins connected to the
    serial, srclk, rclk, and srclr (if used) of shift register.
**/
void ShiftySherwin::shiftSetup(int ser, int srclk, int rclk, int srclr){

    serPin = ser;
    srclkPin = srclk;
    rclkPin = rclk;

    if(srclr != 0){
       srclrPin = srclr;
       pinMode(srclrPin,OUTPUT);
       digitalWrite(srclrPin,HIGH);
       using_srclr = true;
    }

    pinMode(serPin,OUTPUT);
    pinMode(srclkPin,OUTPUT);
    pinMode(rclkPin,OUTPUT);
    using_srclr = false;
}

/**
Writes vector ("regPin") data to all shift registers.
**/
void ShiftySherwin::writeRegisters(){
    digitalWrite(rclkPin,LOW);

    for(i = 0; i < numRegPins; i++){
        digitalWrite(srclkPin,LOW);
        digitalWrite(serPin,regPin[i]);
        digitalWrite(srclkPin,HIGH);
    }
    digitalWrite(rclkPin,HIGH);
    delayMicroseconds(1);
}
/**
Write one bit at a time to registers.
ENTER:
    1) single register output pin
    2) bit to turn on or off. (HIGH or LOW);
**/
void ShiftySherwin::singleBitWrite(int pin, bool state){
    regPin[pin] = state;
    writeRegisters();
}

/**
Write all bits of a register
at once (much faster than one bit writing)

ENTER:
    1) which register to write to, starting
    from 1 up to however many connected.
    2) single byte (ex: 0b00000001)
**/
void ShiftySherwin::singleByteWrite(int regNum, uint8_t value){
        bool bit[numRegPins];
        int startingNum;

    if(regNum <= numRegPins){
        for( i = 0; i < 8; i++){
            bit[i] = (value >> i) & 0x01;
        }

        startingNum = (regNum - 1) * 8;

        for(i = startingNum, n = 0; i < startingNum + 8, n < 8; i++, n++){
            regPin[i] = bit[n];
        }
            writeRegisters();
    }
}

/**
Instantly clears all registers. Will either use
SRCLR pin or manually write everything LOW if
SRLCR is tied high.
**/
void ShiftySherwin::clearRegisters(){
    if(using_srclr){
        digitalWrite(srclrPin,LOW);
        delayMicroseconds(1);
        digitalWrite(srclrPin,HIGH);
    }
    else{
        for(i = 0; i < numRegPins; i++){
            regPin[i] = LOW;
        }
        writeRegisters();
    }
}
