#include "nu32dip.h" // constants, functions for startup and UART
#include "i2c_master_noint.h"
#include "mpu6050.h"
#include <stdio.h>

#define EXPECTED_WHOAMI 0x68
#define IMU_SAMPLING_RATE 100 // in Hz
#define IMU_SAMPLE_COUNT 300
#define MAX_INPUT_SIZE 100
#define MAX_OUTPUT_SIZE 200

void blink(int, int);

int main() {
    NU32DIP_Startup();
    init_mpu6050();
    char data[IMU_ARRAY_LEN];
    float ax[IMU_SAMPLE_COUNT], ay[IMU_SAMPLE_COUNT], az[IMU_SAMPLE_COUNT], 
          gx[IMU_SAMPLE_COUNT], gy[IMU_SAMPLE_COUNT], gz[IMU_SAMPLE_COUNT], 
          temp[IMU_SAMPLE_COUNT], pitch[IMU_SAMPLE_COUNT], 
          roll[IMU_SAMPLE_COUNT], yaw[IMU_SAMPLE_COUNT];

    if(whoami() != EXPECTED_WHOAMI){
        while(1){
            LATAbits.LATA4 = 1; // Infinite loop with LED on indicating error
        }
    }

    char inputBuffer[MAX_INPUT_SIZE];
    char outputBuffer[MAX_OUTPUT_SIZE];

    while (1) {
        blink(1,5); 
        NU32DIP_ReadUART1(inputBuffer, MAX_INPUT_SIZE);

        for (int i=0; i<IMU_SAMPLE_COUNT; i++){
            _CP0_SET_COUNT(0);
            burst_read_mpu6050(data);
            ax[i] = conv_xXL(data); 
            ay[i] = conv_yXL(data);
            az[i] = conv_zXL(data);
            gx[i] = conv_xG(data);
            gy[i] = conv_yG(data);
            gz[i] = conv_zG(data);
            temp[i] = conv_temp(data);
            while (_CP0_GET_COUNT() < 48000000 / 2 / IMU_SAMPLING_RATE) {} 
        }
        
        for (int i=0; i<IMU_SAMPLE_COUNT; i++){
            sprintf(outputBuffer,"%d %f %f %f %f %f %f %f %f %f\r\n", IMU_SAMPLE_COUNT - i, ax[i], ay[i], az[i], gx[i], gy[i], gz[i], temp[i], pitch[i], roll[i]);
            NU32DIP_WriteUART1(outputBuffer);
        }
    }
}

void blink(int iterations, int time_ms) {
    for (int i = 0; i < iterations; i++) {
        NU32DIP_GREEN = 0; 
        NU32DIP_YELLOW = 1; 
        unsigned int timerCount = _CP0_GET_COUNT(); 
        while (_CP0_GET_COUNT() < timerCount + 12000 * time_ms) {}

        NU32DIP_GREEN = 1; 
        NU32DIP_YELLOW = 0; 
        timerCount = _CP0_GET_COUNT();
        while (_CP0_GET_COUNT() < timerCount + 12000 * time_ms) {}
    }
}
