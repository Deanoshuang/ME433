#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro

#include <stdio.h>

#include "mpu6050.h"
#include "i2c_master_noint.h"
#include "ssd1306.h"
#include "font.h"
#include "nu32dip.h"

#define WHO_AM_I_VALUE 0x68
#define DELAY_SCALE 1.4
#define CORE_TICKS_PER_SECOND 48000000 / 2

void generic_i2c_write(unsigned char address, unsigned char registr, unsigned char value);
unsigned char read_GP0(unsigned char address, unsigned char registr);
void blink(int iterations, int time_ms);
void check_who_am_i();
void display_messages(int count, float az);

int main() {
    NU32DIP_Startup();
    i2c_master_setup();
    ssd1306_setup();
    init_mpu6050();
    check_who_am_i();

    char data[IMU_ARRAY_LEN];
    float az;
    int count = 0;

    while (1) {
        blink(1,5);
        _CP0_SET_COUNT(0);
        ssd1306_update();
        count++;
        burst_read_mpu6050(data);
        az = conv_xXL(data);
        while (_CP0_GET_COUNT() < CORE_TICKS_PER_SECOND * DELAY_SCALE) {}
        display_messages(count, az);
    }
}

void check_who_am_i() {
    char who = whoami();
    if(who != WHO_AM_I_VALUE){
      while(1){
        LATAbits.LATA4 = 1;  // Turn on LED to signal error state
      }
    }
}

void display_messages(int count, float az) {
    char message[50];

    sprintf(message, "Hello world!");
    drawString(message, 0, 0);
    sprintf(message, "I am Dean");
    drawString(message, 0, 8);
    sprintf(message, " counter = %d", count);
    drawString(message, 0, 16);
    sprintf(message, "X acc= %f\r\n", az);
    drawString(message, 85, 24);
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