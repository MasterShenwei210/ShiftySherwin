#include <ShiftySherwin.h>

//example using 1 shift register.
//see .cpp file for detailed directions

ShiftySherwin ShiftOut(1);
void setup() {
  Serial.begin(9600);
  ShiftOut.shiftSetup(2,3,4); //not using srclr pin

}

void loop() {
  ShiftOut.singleBitWrite(0,HIGH); 
  delay(500);
  ShiftOut.singleBitWrite(0,LOW);
  delay(500); 
   ShiftOut.singleByteWrite(1,0b00000010);
   delay(300);
   ShiftOut.clearRegisters();
   delay(300);
}
