#include "syncopation.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

double syncopation_score(const int *pattern, int length, int meter_numerator) {
    if (length <= 0 || !pattern) return 0.0;

    /* Compute metric weights */
    double syncopation = 0.0;
    for (int i = 0; i < length; i++) {
        if (pattern[i] != 1) continue;
        int beat = i % meter_numerator;
        double w;
        if (beat == 0)
            w = 4.0;
        else if (beat == meter_numerator / 2)
            w = 3.0;
        else if (beat % 2 == 0)
            w = 2.0;
        else
            w = 1.0;

        int next_i = (i + 1) % length;
        if (pattern[next_i] == 0) {
            int nbeat = next_i % meter_numerator;
            double wn;
            if (nbeat == 0)
                wn = 4.0;
            else if (nbeat == meter_numerator / 2)
                wn = 3.0;
            else if (nbeat % 2 == 0)
                wn = 2.0;
            else
                wn = 1.0;
            if (wn > w) syncopation += wn - w;
        }
    }
    return syncopation;
}

double offbeat_density(const int *pattern, int length, int meter_numerator) {
    if (length <= 0 || !pattern) return 0.0;
    int hits = 0;
    int offbeat_hits = 0;
    for (int i = 0; i < length; i++) {
        if (pattern[i] == 1) {
            hits++;
            int beat = i % meter_numerator;
            if (beat != 0 && beat != meter_numerator / 2) {
                offbeat_hits++;
            }
        }
    }
    if (hits == 0) return 0.0;
    return (double)offbeat_hits / hits;
}

double rhythmic_entropy(const int *pattern, int length) {
    if (!pattern || length <= 0) return 0.0;

    /* Collect hit positions */
    int hit_pos[1024];
    int n_hits = 0;
    for (int i = 0; i < length && n_hits < 1024; i++) {
        if (pattern[i] == 1) hit_pos[n_hits++] = i;
    }
    if (n_hits < 2) return 0.0;

    /* Compute intervals (including wrap-around) */
    int intervals[1024];
    int n_intervals = 0;
    for (int i = 0; i < n_hits - 1; i++) {
        intervals[n_intervals++] = hit_pos[i + 1] - hit_pos[i];
    }
    /* Wrap around */
    intervals[n_intervals++] = length - hit_pos[n_hits - 1] + hit_pos[0];

    /* Count occurrences of each interval value */
    /* Simple counting since intervals are small ints */
    int max_val = 0;
    for (int i = 0; i < n_intervals; i++) {
        if (intervals[i] > max_val) max_val = intervals[i];
    }
    int *counts = (int *)calloc((size_t)(max_val + 1), sizeof(int));
    if (!counts) return 0.0;
    for (int i = 0; i < n_intervals; i++) counts[intervals[i]]++;
    
    double entropy = 0.0;
    double total = (double)n_intervals;
    for (int i = 0; i <= max_val; i++) {
        if (counts[i] > 0) {
            double p = counts[i] / total;
            entropy -= p * log2(p);
        }
    }
    free(counts);
    return entropy;
}
