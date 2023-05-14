#include "i2c_master_noint.h"
#include "nu32dip.h"

void turn_GP7(void);
unsigned char GP0_read(unsigned char a, unsigned char reg);
void i2c_write(unsigned char a, unsigned char reg, unsigned char val);
int main() {
    unsigned char sending=0b0100000<<1;
    unsigned char reading;
    i2c_master_setup();
    NU32DIP_Startup();
    while (1) {
        turn_GP7();
        reading = GP0_read(sending, 0x09);
        if (reading) {i2c_write(sending, 0x0A, 0b10000000);}
        else { i2c_write(sending, 0x0A, 0b00000000);}
    }
}
void turnOn_GP7(void) {
    LATAbits.LATA4 = !LATAbits.LATA4;
    _CP0_SET_COUNT(0);
    while (_CP0_GET_COUNT() < 3000000) {}
}
unsigned char GP0_read(unsigned char a, unsigned char reg) {
    i2c_master_start();
    i2c_master_send(a);
    i2c_master_send(reg);
    i2c_master_restart();
    i2c_master_send(a | 1);
    unsigned char r = (i2c_master_recv() & 0b00000001);
    i2c_master_ack(1);
    i2c_master_stop();
    return r;
}
void i2c_write(unsigned char a, unsigned char reg, unsigned char val) {
    i2c_master_start();
    i2c_master_send(a);
    i2c_master_send(reg);
    i2c_master_send(val);
    i2c_master_stop();
}