#include "servo.h"
#include "nu32dip.h"

void ServoInit(){
    RPB15Rbits.RPB15R = 0b0101; // B15 as OC1
    T2CONbits.TCKPS = 4;     // Timer2 prescaler N=16 (1:4)
    PR2 = 60000 - 1;              // period = (PR2+1) * N * (1/48000000) = 50Hz
    TMR2 = 0;                // initial TMR2 count is 0
    OC1CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
    OC1CONbits.OCTSEL = 0;   // Use timer2
    OC1RS = .25*(PR2 + 1);   // duty cycle = OC1RS/(PR2+1) = 25% 90 >> 2ms
    OC1R = .25*(PR2 + 1);    // initialize before turning OC1 on; afterward it is read-only
    T2CONbits.ON = 1;        // turn on Timer2
    OC1CONbits.ON = 1;       // turn on OC1
}

void SetAng(int ang){
    int duty = ang * 35 + 100;
    OC1RS = duty;
}
