#include "tempo.h"
#include <stdlib.h>
#include <math.h>

double tempo_bpm_to_ms(double bpm) {
    return 60000.0 / bpm;
}

double tempo_ms_to_bpm(double ms) {
    return 60000.0 / ms;
}

double tempo_hz(const Tempo *t) {
    return t->bpm / 60.0;
}

double tempo_period(const Tempo *t) {
    return 60.0 / t->bpm;
}

double *tempo_curve_linear(double start_bpm, double end_bpm, int steps) {
    if (steps <= 0) return NULL;
    double *curve = (double *)malloc((size_t)steps * sizeof(double));
    if (!curve) return NULL;
    int denom = steps > 1 ? steps - 1 : 1;
    for (int i = 0; i < steps; i++) {
        curve[i] = start_bpm + (end_bpm - start_bpm) * i / denom;
    }
    return curve;
}

double *tempo_curve_exponential(double start_bpm, double end_bpm, int steps) {
    if (steps <= 0) return NULL;
    double *curve = (double *)malloc((size_t)steps * sizeof(double));
    if (!curve) return NULL;
    double safe_start = start_bpm < 0.1 ? 0.1 : start_bpm;
    double ratio = end_bpm / safe_start;
    int denom = steps > 1 ? steps - 1 : 1;
    for (int i = 0; i < steps; i++) {
        curve[i] = safe_start * pow(ratio, (double)i / denom);
    }
    return curve;
}

double *tempo_curve_logarithmic(double start_bpm, double end_bpm, int steps) {
    if (steps <= 0) return NULL;
    double *curve = (double *)malloc((size_t)steps * sizeof(double));
    if (!curve) return NULL;
    double log_start = log(start_bpm < 0.1 ? 0.1 : start_bpm);
    double log_end = log(end_bpm < 0.1 ? 0.1 : end_bpm);
    int denom = steps > 1 ? steps - 1 : 1;
    for (int i = 0; i < steps; i++) {
        curve[i] = exp(log_start + (log_end - log_start) * i / denom);
    }
    return curve;
}
