#ifndef BENCHMARK_H
#define BENCHMARK_H

#ifdef __cplusplus
extern "C" {
#endif

// must be provided by chip specific routine
volatile unsigned int getticks(void);

#define NSIZES  9
#define NRUN    8

//result indices
enum {
CMSIS_RF,F4CM4_RF,
CMSIS_RI,F4CM4_RI,
CMSIS_CF,F4CM4_CF,
CMSIS_CI,F4CM4_CI,
NRESULTS
};

typedef struct { unsigned int cycles[NRESULTS][NSIZES]; } RESULT;

RESULT *benchmark(void);

#ifdef __cplusplus
};
#endif

#endif
