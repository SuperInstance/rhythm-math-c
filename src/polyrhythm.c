#include "polyrhythm.h"
#include <stdlib.h>
#include <string.h>

int polyrhythm_lcm2(int a, int b) {
    if (a == 0 || b == 0) return 0;
    /* GCD */
    int aa = a < 0 ? -a : a;
    int bb = b < 0 ? -b : b;
    while (bb) { int t = bb; bb = aa % bb; aa = t; }
    int gcd = aa;
    return (a < 0 ? -a : a) / gcd * (b < 0 ? -b : b);
}

int polyrhythm_lcm_array(const int *rates, int count) {
    if (count <= 0) return 0;
    int result = rates[0];
    for (int i = 1; i < count; i++) {
        result = polyrhythm_lcm2(result, rates[i]);
    }
    return result;
}

int polyrhythm_cycle_length(const Polyrhythm *p) {
    return polyrhythm_lcm_array(p->rates, p->count);
}

int *polyrhythm_pattern(const Polyrhythm *p) {
    int cycle = polyrhythm_cycle_length(p);
    if (cycle <= 0) return NULL;
    int *flat = (int *)calloc((size_t)p->count * (size_t)cycle, sizeof(int));
    if (!flat) return NULL;
    for (int v = 0; v < p->count; v++) {
        int step = cycle / p->rates[v];
        for (int i = 0; i < cycle; i++) {
            flat[v * cycle + i] = (i % step == 0) ? 1 : 0;
        }
    }
    return flat;
}

double polyrhythm_density(const Polyrhythm *p) {
    int cycle = polyrhythm_cycle_length(p);
    if (cycle <= 0) return 0.0;
    int total_hits = 0;
    for (int i = 0; i < p->count; i++) total_hits += p->rates[i];
    return (double)total_hits / cycle;
}

double polyrhythm_complexity(const Polyrhythm *p) {
    int cycle = polyrhythm_cycle_length(p);
    if (cycle <= 0) return 0.0;
    int sum = 0;
    for (int i = 0; i < p->count; i++) sum += p->rates[i];
    if (sum == 0) return 0.0;
    return (double)cycle / sum;
}

int *polyrhythm_combined_cycle(const int *rates, int count) {
    int cycle = polyrhythm_lcm_array(rates, count);
    if (cycle <= 0) return NULL;
    int *combined = (int *)calloc((size_t)cycle, sizeof(int));
    if (!combined) return NULL;
    for (int r = 0; r < count; r++) {
        int step = cycle / rates[r];
        for (int i = 0; i < rates[r]; i++) {
            combined[i * step] = 1;
        }
    }
    return combined;
}
