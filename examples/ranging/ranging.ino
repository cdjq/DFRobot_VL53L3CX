
#include "DFRobot_VL53L3CX.h"




DFRobot_VL53L3CX vl53l3cx;



void setup(void)
{
Serial.begin(9600);
vl53l3cx.begin();
Serial.print("vl53l3cx id: ");
Serial.println(vl53l3cx.readID());
}

void loop(void){

vl53l3cx.ranging();

}