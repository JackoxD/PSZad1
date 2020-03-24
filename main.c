#include <csl.h>
#include <csl_timer.h>
#include <dsk6416.h>
#include <dsk6416_led.h>
#include <dsk6416_dip.h>
#include <dsk6416_aic23.h>

#include <math.h>

#define PI (3.14159265358979323846)

DSK6416_AIC23_CodecHandle hCodec;
DSK6416_AIC23_Config codecConfig = { \
	    0x0017,  /* 0 DSK6416_AIC23_LEFTINVOL  Left line input channel volume */ \
	    0x0017,  /* 1 DSK6416_AIC23_RIGHTINVOL Right line input channel volume */\
	    0x00d8,  /* 2 DSK6416_AIC23_LEFTHPVOL  Left channel headphone volume */  \
	    0x00d8,  /* 3 DSK6416_AIC23_RIGHTHPVOL Right channel headphone volume */ \
	    0x0015,  /* 4 DSK6416_AIC23_ANAPATH    Analog audio path control */      \
	    0x0000,  /* 5 DSK6416_AIC23_DIGPATH    Digital audio path control */     \
	    0x0000,  /* 6 DSK6416_AIC23_POWERDOWN  Power down control */             \
	    0x0043,  /* 7 DSK6416_AIC23_DIGIF      Digital audio interface format */ \
	    0x0001,  /* 8 DSK6416_AIC23_SAMPLERATE Sample rate control */            \
	    0x0001   /* 9 DSK6416_AIC23_DIGACT     Digital interface activation */   \
};

int main(void) {
	
	int sample, amp;

	float r, f, a, b;
	float sample_0, sample_1, sample_2;

	CSL_init();

	DSK6416_init();

	DSK6416_DIP_init();

	hCodec = DSK6416_AIC23_openCodec(0, &codecConfig);

	amp=32000;

	r = 1.0;
	f = 1000.0/48000.0;

	a = 2*r*cos(2*PI*f);
	b = -r*r;

	sample_2 = 0;
	sample_1 = sin(2*PI*f);

	while (DSK6416_DIP_get(0)) {

		sample_0 = a*sample_1+b*sample_2;
		sample_2 = sample_1;
		sample_1 = sample_0;

		sample = amp*sample_0;

		while(!DSK6416_AIC23_write(hCodec, sample));
		while(!DSK6416_AIC23_write(hCodec, sample));

	}

	DSK6416_AIC23_closeCodec(hCodec);

	return 0;
}
