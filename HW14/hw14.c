#include "servo.h"
#include "nu32dip.h"

#define ANGLE1 45
#define ANGLE2 135
#define DELAY_S 4

void delay(int delay_s);
void set_angle_and_wait(int angle, int delay_s);

int main() {
    NU32DIP_Startup();
    ServoInit();

    while(1) {
        set_angle_and_wait(ANGLE1, DELAY_S);
        set_angle_and_wait(ANGLE2, DELAY_S);
    }
    return 0;
}

void delay(int delay_s) {
    unsigned int delay_count = _CP0_GET_COUNT() + delay_s * 24000000;
    while(_CP0_GET_COUNT() < delay_count) {}
}

void set_angle_and_wait(int angle, int delay_s) {
    SetAng(angle);
    delay(delay_s);
}