#include "spi.h"
#include "nu32dip.h"
#include <math.h>

#define pi 3.1415

unsigned short create_wave(int channel, float voltage);
float sin_wave(float t);
float triangle_wave(float t);

int main()
{

	// enable startups
	NU32DIP_Startup();
	initSPI();

	// initialize stuff
	float t;
    unsigned short v_sine,v_tri;
    unsigned short buf;
	t = 0.0;
	unsigned char spi_return;
    

	while (1)
	{
        t=(t+1)/1000;
		v_sine = (unsigned short) (3*(1+sin(2*pi*t/3)));
		v_tri =  (unsigned short) (3*abs(t-3));
		// send volt to spi
		buf=0b0111<<12;
        buf=buf|(v_sine<<2);
        LATAbits.LATA0=0;
        spi_io((buf & 0xFF00)>>8);
        spi_io((buf & 0x00FF));
        LATAbits.LATA0=1;       
		_CP0_SET_COUNT(0);
		while (_CP0_GET_COUNT() < 240000)
		{
		}
		
	}
}


// bit created: [a_or_b 1 1 1 (10 bit voltage) 0 0]

