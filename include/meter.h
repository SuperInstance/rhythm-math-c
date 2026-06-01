#ifndef RHYTHM_METER_H
#define RHYTHM_METER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int numerator;
    int denominator;
} Meter;

int meter_is_simple(const Meter *m);
int meter_is_compound(const Meter *m);
int meter_is_odd(const Meter *m);
int meter_beat_count(const Meter *m);
int meter_subdivision(const Meter *m);

/* Downbeat positions for `measures` measures. Caller must free. */
int *meter_downbeats(int measures, const Meter *m);

/* Subdivision grid for one measure at given level. Caller must free. */
int *meter_subdivisions(const Meter *m, int subdivision_level, int *out_count);

/* Generate time signature change sequence.
   `repeats` may be NULL (defaults to 1 each).
   out_count receives the number of meters returned. Caller must free. */
Meter *time_signature_changes(const Meter *meters, int meter_count,
                               const int *repeats, int *out_count);

#ifdef __cplusplus
}
#endif

#endif /* RHYTHM_METER_H */
