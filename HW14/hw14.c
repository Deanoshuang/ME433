#include "servo.h"
#include "nu32dip.h"

int main(){
    NU32DIP_Startup();
    ServoInit();
    while(1){
        SetAng(45);
        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT() < 24000000*4){}
        SetAng(135);
        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT() < 24000000*4){}
        }
    return 0;
}
