#include "i2c_master_noint.h"
#include "nu32dip.h"

#define TARGET_ADDRESS 0b0100000<<1
#define TARGET_REG 0x09
#define TARGET_WRITE_REG 0x0A
#define TARGET_WRITE_VAL_ON 0b10000000
#define TARGET_WRITE_VAL_OFF 0b00000000
#define GP7_DELAY_COUNT 3000000

void toggle_GP7(void);
unsigned char GP0_read(unsigned char a, unsigned char reg);
void i2c_write(unsigned char a, unsigned char reg, unsigned char val);

int main() {
    unsigned char reading;

    i2c_master_setup();
    NU32DIP_Startup();

    while (1) {
        toggle_GP7();
        reading = GP0_read(TARGET_ADDRESS, TARGET_REG);
        if (reading) {
            i2c_write(TARGET_ADDRESS, TARGET_WRITE_REG, TARGET_WRITE_VAL_ON);
        } else { 
            i2c_write(TARGET_ADDRESS, TARGET_WRITE_REG, TARGET_WRITE_VAL_OFF);
        }
    }
}

void toggle_GP7(void) {
    LATAbits.LATA4 = !LATAbits.LATA4;
    _CP0_SET_COUNT(0);
    while (_CP0_GET_COUNT() < GP7_DELAY_COUNT) {}
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