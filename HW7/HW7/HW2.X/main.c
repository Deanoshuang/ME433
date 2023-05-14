#include "nu32dip.h" // constants, functions for startup and UART
#include "i2c_master_noint.h"
#include "mpu6050.h"
#include <stdio.h>

void blink(int, int); // blink the LEDs function

int main() {
    NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
    init_mpu6050();
    // char array for the raw data
    char d[IMU_ARRAY_LEN];
	// floats to store the data
    float ax[300], ay[300], az[300], gx[300], gy[300], gz[300], temp[300], pitch[300], roll[300], yaw[300];
    
	// read whoami
    char who;
    who = whoami(); 
	// print whoami

    // if whoami is not 0x68, stuck in loop with LEDs on
    if(who!= 0x68){
      while(1){
        LATAbits.LATA4 = 1;
      }
    }
    int i;
    
    float dt = 0.005; 
    float a = 0.5;
    char m_in[100];
    char m_out[200];
    while (1) {
        //ue core timer for exactly 100Hz loop
        //_CP0_SET_COUNT(0);
        blink(1,5); 
        // wait to print until you get a newline
        NU32DIP_ReadUART1(m_in,100);
        // read imu to store data
        for (i=0; i<300; i++){
            _CP0_SET_COUNT(0);
            //read IMU
            burst_read_mpu6050(d);
            ax[i] = conv_xXL(d); //accelerations
            ay[i] = conv_yXL(d);
            az[i] = conv_zXL(d);
            gx[i] = conv_xG(d);
            gy[i] = conv_yG(d);
            gz[i] = conv_zG(d);
            temp[i] = conv_temp(d);
            while (_CP0_GET_COUNT() < 48000000 / 2 / 100) {} //delay
        }
        
        // display
        for (i=0; i<300; i++){
            sprintf(m_out,"%d %f %f %f %f %f %f %f %f %f\r\n",300-i,ax[i],ay[i],az[i],gx[i],gy[i],gz[i],temp[i],pitch[i],roll[i]);//,
            NU32DIP_WriteUART1(m_out);
        }
        
    }
}
// blink the LEDs
void blink(int iterations, int time_ms) {
    int i;
    unsigned int t;
    for (i = 0; i < iterations; i++) {
        NU32DIP_GREEN = 0; // on
        NU32DIP_YELLOW = 1; // off
        t = _CP0_GET_COUNT(); // should really check for overflow here
        // the core timer ticks at half the SYSCLK, so 24000000 times per second
        // so each millisecond is 24000 ticks
        // wait half in each delay
        while (_CP0_GET_COUNT() < t + 12000 * time_ms) {
        }

        NU32DIP_GREEN = 1; // off
        NU32DIP_YELLOW = 0; // on
        t = _CP0_GET_COUNT(); // should really check for overflow here
        while (_CP0_GET_COUNT() < t + 12000 * time_ms) {
        }
    }
}