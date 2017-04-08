// Speed comparison of CMSIS FFT vs FFT4CM4F 
// Each FFT variant is run several times and the minimum cycle count is used to
// eliminate side effects like interrupts or cache misses.
// ---
// to run, all you need is this:
// * a development board for any Cortex-M4 MCU with FPU
// * arm-none-eabi-gcc compiler suite
// * CMSIS Version 5
// * a routine called getticks() that returns elapsed clock cycles
// add this file and the accompanying .h file to an empty project, 
// stir, compile, link with CMSIS/Lib/GCC/libarm_cortexM4lf_math.a,
// then run! Don't forget to call benchmark()
// ---
// (c) 2017 Stefan Stenzel 

#define ARM_MATH_CM4
#define __FPU_PRESENT 1U 

#include <arm_math.h>
#include <arm_const_structs.h>

#include "fft4cm4f.h"
#include "benchmark.h"

// CMSIS constants 
arm_cfft_instance_f32 const * const ARMCI[NSIZES] = 
{
    &arm_cfft_sR_f32_len16,
    &arm_cfft_sR_f32_len32,
    &arm_cfft_sR_f32_len64,
    &arm_cfft_sR_f32_len128,
    &arm_cfft_sR_f32_len256,
    &arm_cfft_sR_f32_len512,
    &arm_cfft_sR_f32_len1024, 
    &arm_cfft_sR_f32_len2048,
    &arm_cfft_sR_f32_len4096
};

// FFT4CM4F 
typedef void (* FFTC)(complex *);
typedef void (* FFTR)(float *);

const FFTC CF_FFT4CM4F[] =      // complex forward
{fftc4_16,fftc4_32,fftc4_64,fftc4_128,fftc4_256,fftc4_512,fftc4_1024,fftc4_2048,fftc4_4096};

const FFTC CI_FFT4CM4F[] =      // complex inverse
{fftc4_un16,fftc4_un32,fftc4_un64,fftc4_un128,fftc4_un256,fftc4_un512,fftc4_un1024,fftc4_un2048,fftc4_un4096};

const FFTR RF_FFT4CM4F[] =      // real forward
{fftr4_16,fftr4_32,fftr4_64,fftr4_128,fftr4_256,fftr4_512,fftr4_1024,fftr4_2048,fftr4_4096};

const FFTR RI_FFT4CM4F[] =    // real inverse
{fftr4_un16,fftr4_un32,fftr4_un64,fftr4_un128,fftr4_un256,fftr4_un512,fftr4_un1024,fftr4_un2048,fftr4_un4096};

// Results come here.
RESULT Result;

// memory for tests
float InOut[8192];

// fill buffer with random data, log2 of size given  
void filldata(int lg2n)
{
    int i,x=0,n;
    
    float *dst = InOut;
    n = 1<<lg2n;
    
    for(i=0; i<n; i++,x+=3) *dst++ = (((i * x) & 0x0FF)-128) * 0x1p-7f;
}

// test for FFT4CM4F routines
void testf4cm4(void)
{
    FFTC fftc; 
    FFTR fftr;

    unsigned int sz,r,ticks,min;
    
    for(sz = 0; sz<NSIZES; sz++)      // loop over sizes
    {
		fftc = CF_FFT4CM4F[sz];			// complex forward
		min = 0x7FFFFFFF;
		for(r=0; r<NRUN; r++)
		{
			filldata(sz+5);				// log2 of size for complex 
			ticks = getticks();
			fftc((complex *)InOut);
			ticks = getticks() - ticks;
			if(ticks < min) min = ticks;
		}
		
		Result.cycles[F4CM4_CF][sz] = min;
		
		fftc = CI_FFT4CM4F[sz];			// complex inverse
		min = 0x7FFFFFFF;
		for(r=0; r<NRUN; r++)
		{
			filldata(sz+5);
			ticks = getticks();
			fftc((complex *)InOut);
			ticks = getticks() - ticks;
			if(ticks < min) min = ticks;
		}
		Result.cycles[F4CM4_CI][sz] = min;
		
		fftr = RF_FFT4CM4F[sz];			// real forward
		min = 0x7FFFFFFF;
		for(r=0; r<NRUN; r++)
		{
			filldata(sz+4);
			ticks = getticks();
			fftr(InOut);
			ticks = getticks() - ticks;
			if(ticks < min) min = ticks;
		}
		Result.cycles[F4CM4_RF][sz] = min;
		
		fftr = RI_FFT4CM4F[sz];			// real inverse
		min = 0x7FFFFFFF;
		for(r=0; r<NRUN; r++)
		{
			filldata(sz+4);
			ticks = getticks();
			fftr(InOut);
			ticks = getticks() - ticks;
			if(ticks < min) min = ticks;
		}
		Result.cycles[F4CM4_RI][sz] = min;
	}
}

// test for CMSIS V5 FFT routines
void testcmsis(void) 
{
	int i,sz;
	unsigned int min,t;
	arm_rfft_fast_instance_f32 af;
	
	for(sz = 0; sz<NSIZES; sz++)				// loop over sizes
	{
		min = 0x7FFFFFFF;
		for(i=0; i<NRUN; i++)
		{
			filldata(sz+5);
			t = getticks();
			arm_cfft_f32(ARMCI[sz],InOut, 0, 0);
			t = getticks() - t;
			if(t < min) min = t;
		}
		Result.cycles[CMSIS_CF][sz] = min;
		
		min = 0x7FFFFFFF;
		for(i=0; i<NRUN; i++)
		{
			filldata(sz+5);
			t = getticks();
			arm_cfft_f32(ARMCI[sz],InOut, 0, 1);
			t = getticks() - t;
			if(t < min) min = t;
		}
		Result.cycles[CMSIS_CI][sz] = min;

		arm_rfft_fast_init_f32 (&af,16<<sz);
		min = 0x7FFFFFFF;
		for(i=0; i<NRUN; i++)
		{
			filldata(sz+4);
			t = getticks();
			arm_rfft_fast_f32(&af,InOut,InOut+4096,0);
			t = getticks() - t;
			if(t < min) min = t;
		}
		Result.cycles[CMSIS_RF][sz] = min;

		min = 0x7FFFFFFF;
		for(i=0; i<NRUN; i++)
		{
			filldata(sz+4);
			t = getticks();
			arm_rfft_fast_f32(&af,InOut,InOut+4096,1);
			t = getticks() - t;
			if(t < min) min = t;
		}
		Result.cycles[CMSIS_RI][sz] = min;
	}
};

RESULT *benchmark(void)
{
	testcmsis(); 
	testf4cm4();
	return &Result;
};

