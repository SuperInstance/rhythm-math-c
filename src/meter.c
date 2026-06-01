#include "meter.h"
#include <stdlib.h>
#include <string.h>

int meter_is_simple(const Meter *m) {
    return m->numerator == 2 || m->numerator == 3 || m->numerator == 4;
}

int meter_is_compound(const Meter *m) {
    return m->numerator % 3 == 0 && m->numerator > 3;
}

int meter_is_odd(const Meter *m) {
    int n = m->numerator;
    return n != 2 && n != 3 && n != 4 && n != 6 && n != 9 && n != 12;
}

int meter_beat_count(const Meter *m) {
    if (meter_is_compound(m)) return m->numerator / 3;
    return m->numerator;
}

int meter_subdivision(const Meter *m) {
    if (meter_is_compound(m)) return 3;
    return 2;
}

int *meter_downbeats(int measures, const Meter *m) {
    if (measures <= 0) return NULL;
    int *result = (int *)malloc((size_t)measures * sizeof(int));
    if (!result) return NULL;
    for (int i = 0; i < measures; i++) {
        result[i] = i * m->numerator;
    }
    return result;
}

int *meter_subdivisions(const Meter *m, int subdivision_level, int *out_count) {
    int total = m->numerator * subdivision_level;
    if (out_count) *out_count = total;
    int *result = (int *)malloc((size_t)total * sizeof(int));
    if (!result) return NULL;
    for (int i = 0; i < total; i++) result[i] = i;
    return result;
}

Meter *time_signature_changes(const Meter *meters, int meter_count,
                               const int *repeats, int *out_count) {
    int total = 0;
    for (int i = 0; i < meter_count; i++) {
        total += repeats ? repeats[i] : 1;
    }
    if (out_count) *out_count = total;
    Meter *result = (Meter *)malloc((size_t)total * sizeof(Meter));
    if (!result) return NULL;
    int idx = 0;
    for (int i = 0; i < meter_count; i++) {
        int rep = repeats ? repeats[i] : 1;
        for (int j = 0; j < rep; j++) {
            result[idx++] = meters[i];
        }
    }
    return result;
}
