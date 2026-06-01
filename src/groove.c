#include "groove.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

double groove_average_velocity(const GroovePattern *g) {
    if (g->length <= 0) return 0.0;
    double sum = 0.0;
    for (int i = 0; i < g->length; i++) sum += g->velocities[i];
    return sum / g->length;
}

double *swing_apply(const double *grid, int length, double amount) {
    if (length <= 0) return NULL;
    double *result = (double *)malloc((size_t)length * sizeof(double));
    if (!result) return NULL;
    memcpy(result, grid, (size_t)length * sizeof(double));
    for (int i = 1; i < length; i += 2) {
        double diff = result[i] - result[i - 1];
        result[i] += amount * diff * 0.33;
    }
    return result;
}

int *groove_velocity(const int *pattern, int length, const int *accent_pattern) {
    if (length <= 0) return NULL;
    int *vel = (int *)malloc((size_t)length * sizeof(int));
    if (!vel) return NULL;
    for (int i = 0; i < length; i++) {
        int a = accent_pattern ? accent_pattern[i] : 1;
        int v = pattern[i] * 80 * a;
        vel[i] = v > 127 ? 127 : v;
    }
    return vel;
}

/* Simple xorshift PRNG */
static unsigned int xorshift32(unsigned int *state) {
    unsigned int x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}

/* Box-Muller for gaussian noise */
static double gauss_rand(unsigned int *state) {
    double u1 = ((double)(xorshift32(state) & 0x7FFFFFFF)) / 0x7FFFFFFF;
    double u2 = ((double)(xorshift32(state) & 0x7FFFFFFF)) / 0x7FFFFFFF;
    if (u1 < 1e-10) u1 = 1e-10;
    return sqrt(-2.0 * log(u1)) * cos(2.0 * 3.14159265358979323846 * u2);
}

double *microtiming_humanize(const double *offsets, int length, double amount, unsigned int seed) {
    if (length <= 0) return NULL;
    double *result = (double *)malloc((size_t)length * sizeof(double));
    if (!result) return NULL;
    unsigned int state = seed;
    for (int i = 0; i < length; i++) {
        result[i] = offsets[i] + gauss_rand(&state) * amount;
    }
    return result;
}
