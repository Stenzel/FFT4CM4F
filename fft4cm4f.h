// cortex-m4f version of djbfft for float

#ifndef FFT4CM4F_H
#define FFT4CM4F_H


#ifdef __cplusplus
extern "C" {
#endif

typedef struct 
{
	float re;
	float im; 
} complex;

//extern void fftc4_8(complex *);
extern void fftc4_16(complex *);
extern void fftc4_32(complex *);
extern void fftc4_64(complex *);
extern void fftc4_128(complex *);
extern void fftc4_256(complex *);
extern void fftc4_512(complex *);
extern void fftc4_1024(complex *);
extern void fftc4_2048(complex *);
extern void fftc4_4096(complex *);
extern void fftc4_8192(complex *);

//extern void fftc4_un4(complex *);
//extern void fftc4_un8(complex *);
extern void fftc4_un16(complex *);
extern void fftc4_un32(complex *);
extern void fftc4_un64(complex *);
extern void fftc4_un128(complex *);
extern void fftc4_un256(complex *);
extern void fftc4_un512(complex *);
extern void fftc4_un1024(complex *);
extern void fftc4_un2048(complex *);
extern void fftc4_un4096(complex *);
extern void fftc4_un8192(complex *);

//extern void fftr4_8(float *);
extern void fftr4_16(float *);
extern void fftr4_32(float *);
extern void fftr4_64(float *);
extern void fftr4_128(float *);
extern void fftr4_256(float *);
extern void fftr4_512(float *);
extern void fftr4_1024(float *);
extern void fftr4_2048(float *);
extern void fftr4_4096(float *);
extern void fftr4_8192(float *);

//extern void fftr4_un8(float *);
extern void fftr4_un16(float *);
extern void fftr4_un32(float *);
extern void fftr4_un64(float *);
extern void fftr4_un128(float *);
extern void fftr4_un256(float *);
extern void fftr4_un512(float *);
extern void fftr4_un1024(float *);
extern void fftr4_un2048(float *);
extern void fftr4_un4096(float *);
extern void fftr4_un8192(float *);

extern const complex fftc4_roots16[];
extern const complex fftc4_roots32[];
extern const complex fftc4_roots64[];
extern const complex fftc4_roots128[];
extern const complex fftc4_roots256[];
extern const complex fftc4_roots512[];
extern const complex fftc4_roots1024[];
extern const complex fftc4_roots2048[];
extern const complex fftc4_roots4096[];
extern const complex fftc4_roots8192[];

#ifdef __cplusplus
};
#endif
#endif
