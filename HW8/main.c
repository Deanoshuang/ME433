#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro

#include <stdio.h>

#include "mpu6050.h"
#include "i2c_master_noint.h"
#include "ssd1306.h"
#include "font.h"
#include "nu32dip.h"

void generic_i2c_write(unsigned char address, unsigned char registr, unsigned char value);
unsigned char read_GP0(unsigned char address, unsigned char registr);
void blink(int iterations, int time_ms);

int main() {

    NU32DIP_Startup();
    i2c_master_setup();
    ssd1306_setup();
    init_mpu6050();
    // char array for the raw data
    char d[IMU_ARRAY_LEN];
	// floats to store the data
    float az;
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
   
    
    float dt = 0.005; 
    float a = 0.5;
    char m_in[100];
    char m_out[200];
    
    char message[50];
    int i = 0;
    sprintf(message, "Hello world!");
    drawString(message, 0, 0);
    sprintf(message, "I AM TOMMY");
    drawString(message, 0, 8);
    sprintf(message, " counter = ");
    drawString(message, 0, 16);
    sprintf(message, "X-acceleration = ");
    drawString(message, 0, 24);

    while (1) {
        blink(1,5);
        sprintf(message, "%d", i);
        drawString(message, 70, 16);
        _CP0_SET_COUNT(0);
        ssd1306_update();
        i++;
        burst_read_mpu6050(d);
          
        az = conv_xXL(d);
        while (_CP0_GET_COUNT() < 48000000 / 2 *1.4 ) {}
        sprintf(message,"%f\r\n",az);
        drawString(message, 85, 24);
        
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

//read from GP0
unsigned char read_GP0(unsigned char address, unsigned char register1) {
    //send start bit
    i2c_master_start();
    //send addy w write bit
    i2c_master_send(address);
    //send reg u want to read from
    i2c_master_send(register1);
    //restart
    i2c_master_restart();
    //send addy w read bit
    i2c_master_send(address | 1);
    unsigned char r = (i2c_master_recv() & 0b00000001);
    //send ACK bit
    i2c_master_ack(1);
    //send stop bit
    i2c_master_stop();

    //just last bit
    return r;
}

//generic_i2c_write
void generic_i2c_write(unsigned char address, unsigned char register1, unsigned char value) {
    //start, send, addy/reg/value, stop
    i2c_master_start();
    i2c_master_send(address);
    i2c_master_send(register1);
    i2c_master_send(value);
    i2c_master_stop();
}