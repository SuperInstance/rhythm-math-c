#ifndef RHYTHM_POLYRHYTHM_H
#define RHYTHM_POLYRHYTHM_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define POLYRHYTHM_MAX_RATES 16

typedef struct {
    int rates[POLYRHYTHM_MAX_RATES];
    int count;
} Polyrhythm;

/* LCM of two integers */
int polyrhythm_lcm2(int a, int b);

/* LCM of all rates in the polyrhythm */
int polyrhythm_cycle_length(const Polyrhythm *p);

/* Generate pattern for each voice. Caller must free each row and the array itself.
   Returns a flat array of (count * cycle_length) ints. */
int *polyrhythm_pattern(const Polyrhythm *p);

/* Density: total hits / cycle length */
double polyrhythm_density(const Polyrhythm *p);

/* Complexity: cycle_length / sum(rates) */
double polyrhythm_complexity(const Polyrhythm *p);

/* LCM of an arbitrary array of rates */
int polyrhythm_lcm_array(const int *rates, int count);

/* Combined rhythm pattern (any voice hits). Caller must free. */
int *polyrhythm_combined_cycle(const int *rates, int count);

#ifdef __cplusplus
}
#endif

#endif /* RHYTHM_POLYRHYTHM_H */
