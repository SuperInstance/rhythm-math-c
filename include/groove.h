#ifndef RHYTHM_GROOVE_H
#define RHYTHM_GROOVE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GROOVE_MAX_STEPS 64

typedef struct {
    int velocities[GROOVE_MAX_STEPS];
    double offsets[GROOVE_MAX_STEPS];
    int length;
} GroovePattern;

/* Average velocity of a groove pattern */
double groove_average_velocity(const GroovePattern *g);

/* Apply swing to a grid. Caller must free. */
double *swing_apply(const double *grid, int length, double amount);

/* Convert binary pattern to velocity values with optional accents.
   accent_pattern may be NULL (defaults to all 1s).
   Caller must free. */
int *groove_velocity(const int *pattern, int length, const int *accent_pattern);

/* Humanize microtiming offsets. Adds gaussian noise.
   Uses a deterministic PRNG seeded with seed for reproducibility.
   Caller must free. */
double *microtiming_humanize(const double *offsets, int length, double amount, unsigned int seed);

#ifdef __cplusplus
}
#endif

#endif /* RHYTHM_GROOVE_H */
