#ifndef RHYTHM_TEMPO_H
#define RHYTHM_TEMPO_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    double bpm;
} Tempo;

/* Core conversions */
double tempo_bpm_to_ms(double bpm);
double tempo_ms_to_bpm(double ms);
double tempo_hz(const Tempo *t);
double tempo_period(const Tempo *t);

/* Tempo curves: caller must free the returned array.
   Returns NULL on allocation failure. */
typedef enum {
    TEMPO_CURVE_LINEAR,
    TEMPO_CURVE_EXPONENTIAL,
    TEMPO_CURVE_LOGARITHMIC
} TempoCurveType;

double *tempo_curve_linear(double start_bpm, double end_bpm, int steps);
double *tempo_curve_exponential(double start_bpm, double end_bpm, int steps);
double *tempo_curve_logarithmic(double start_bpm, double end_bpm, int steps);

#ifdef __cplusplus
}
#endif

#endif /* RHYTHM_TEMPO_H */
