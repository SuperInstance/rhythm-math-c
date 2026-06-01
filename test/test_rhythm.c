/*
 * test_rhythm.c — Comprehensive test suite for rhythm-math-c
 * Compile: cc -o test_rhythm test/test_rhythm.c src/tempo.c src/polyrhythm.c src/groove.c src/syncopation.c src/meter.c -Iinclude -lm
 * Run: ./test_rhythm
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "rhythm_api.h"

static int passed = 0;
static int failed = 0;

#define ASSERT_FEQ(a, b, eps) do { \
    if (fabs((a) - (b)) > (eps)) { \
        printf("  FAIL %s:%d: expected %.6f got %.6f\n", __FILE__, __LINE__, (double)(b), (double)(a)); \
        failed++; return; \
    } \
} while(0)

#define ASSERT_INT(a, b) do { \
    if ((a) != (b)) { \
        printf("  FAIL %s:%d: expected %d got %d\n", __FILE__, __LINE__, (int)(b), (int)(a)); \
        failed++; return; \
    } \
} while(0)

#define ASSERT_TRUE(cond) do { \
    if (!(cond)) { \
        printf("  FAIL %s:%d: assertion false\n", __FILE__, __LINE__); \
        failed++; return; \
    } \
} while(0)

#define TEST(name) static void test_##name(void)
#define RUN(name) do { printf("  %-50s", #name); test_##name(); if (failed == f) { passed++; printf("OK\n"); } } while(0)

/* ======== TEMPO TESTS ======== */

TEST(tempo_bpm_to_ms_120) {
    ASSERT_FEQ(tempo_bpm_to_ms(120.0), 500.0, 0.01);
}

TEST(tempo_bpm_to_ms_60) {
    ASSERT_FEQ(tempo_bpm_to_ms(60.0), 1000.0, 0.01);
}

TEST(tempo_ms_to_bpm_500) {
    ASSERT_FEQ(tempo_ms_to_bpm(500.0), 120.0, 0.01);
}

TEST(tempo_ms_to_bpm_1000) {
    ASSERT_FEQ(tempo_ms_to_bpm(1000.0), 60.0, 0.01);
}

TEST(tempo_struct_120) {
    Tempo t = {120.0};
    ASSERT_FEQ(tempo_hz(&t), 2.0, 0.01);
    ASSERT_FEQ(tempo_period(&t), 0.5, 0.01);
}

TEST(tempo_struct_60) {
    Tempo t = {60.0};
    ASSERT_FEQ(tempo_bpm_to_ms(t.bpm), 1000.0, 0.01);
    ASSERT_FEQ(tempo_period(&t), 1.0, 0.01);
}

TEST(tempo_curve_linear) {
    double *curve = tempo_curve_linear(100.0, 200.0, 5);
    ASSERT_TRUE(curve != NULL);
    ASSERT_INT(5, 5); /* length known */
    ASSERT_FEQ(curve[0], 100.0, 0.01);
    ASSERT_FEQ(curve[4], 200.0, 0.01);
    free(curve);
}

TEST(tempo_curve_exponential) {
    double *curve = tempo_curve_exponential(100.0, 200.0, 5);
    ASSERT_TRUE(curve != NULL);
    ASSERT_TRUE(curve[0] < curve[4]);
    ASSERT_FEQ(curve[0], 100.0, 0.01);
    free(curve);
}

TEST(tempo_curve_logarithmic) {
    double *curve = tempo_curve_logarithmic(100.0, 200.0, 5);
    ASSERT_TRUE(curve != NULL);
    ASSERT_TRUE(curve[0] < curve[4]);
    free(curve);
}

TEST(tempo_curve_single_step) {
    double *curve = tempo_curve_linear(120.0, 120.0, 1);
    ASSERT_TRUE(curve != NULL);
    ASSERT_FEQ(curve[0], 120.0, 0.01);
    free(curve);
}

/* ======== POLYRHYTHM TESTS ======== */

TEST(polyrhythm_lcm_basic) {
    ASSERT_INT(polyrhythm_lcm2(3, 4), 12);
    ASSERT_INT(polyrhythm_lcm2(2, 3), 6);
}

TEST(polyrhythm_lcm_array) {
    int rates[] = {2, 3, 5};
    ASSERT_INT(polyrhythm_lcm_array(rates, 3), 30);
}

TEST(polyrhythm_hemiola_cycle) {
    Polyrhythm p = {.rates = {3, 2}, .count = 2};
    ASSERT_INT(polyrhythm_cycle_length(&p), 6);
}

TEST(polyrhythm_hemiola_density) {
    Polyrhythm p = {.rates = {3, 2}, .count = 2};
    ASSERT_FEQ(polyrhythm_density(&p), 5.0 / 6.0, 0.01);
}

TEST(polyrhythm_hemiola_complexity) {
    Polyrhythm p = {.rates = {3, 2}, .count = 2};
    ASSERT_FEQ(polyrhythm_complexity(&p), 6.0 / 5.0, 0.01);
}

TEST(polyrhythm_pattern) {
    Polyrhythm p = {.rates = {3, 2}, .count = 2};
    int *flat = polyrhythm_pattern(&p);
    ASSERT_TRUE(flat != NULL);
    /* Voice 0 (rate=3): hits at 0, 2, 4 */
    ASSERT_INT(flat[0 * 6 + 0], 1);
    ASSERT_INT(flat[0 * 6 + 2], 1);
    ASSERT_INT(flat[0 * 6 + 4], 1);
    /* Voice 1 (rate=2): hits at 0, 3 */
    ASSERT_INT(flat[1 * 6 + 0], 1);
    ASSERT_INT(flat[1 * 6 + 3], 1);
    free(flat);
}

TEST(polyrhythm_4_3_cycle) {
    Polyrhythm p = {.rates = {4, 3}, .count = 2};
    ASSERT_INT(polyrhythm_cycle_length(&p), 12);
}

TEST(polyrhythm_single_rate) {
    Polyrhythm p = {.rates = {4}, .count = 1};
    ASSERT_INT(polyrhythm_cycle_length(&p), 4);
    ASSERT_FEQ(polyrhythm_complexity(&p), 1.0, 0.01);
}

TEST(polyrhythm_combined_cycle) {
    int rates[] = {3, 2};
    int *combined = polyrhythm_combined_cycle(rates, 2);
    ASSERT_TRUE(combined != NULL);
    int sum = 0;
    for (int i = 0; i < 6; i++) sum += combined[i];
    ASSERT_INT(sum, 4); /* 3 + 2 - 1 overlap at 0 */
    free(combined);
}

/* ======== GROOVE TESTS ======== */

TEST(groove_basic) {
    GroovePattern g = {.velocities = {100, 80, 90, 70}, .offsets = {0, 5, -3, 10}, .length = 4};
    ASSERT_INT(g.length, 4);
    ASSERT_FEQ(groove_average_velocity(&g), 85.0, 0.01);
}

TEST(groove_empty) {
    GroovePattern g = {.length = 0};
    ASSERT_FEQ(groove_average_velocity(&g), 0.0, 0.01);
}

TEST(swing_apply_basic) {
    double grid[] = {0.0, 0.5, 1.0, 1.5};
    double *swung = swing_apply(grid, 4, 0.5);
    ASSERT_TRUE(swung != NULL);
    ASSERT_FEQ(swung[0], 0.0, 0.001); /* On-beats don't move */
    ASSERT_TRUE(swung[1] > 0.5);       /* Off-beats pushed later */
    ASSERT_FEQ(swung[2], 1.0, 0.001);
    ASSERT_TRUE(swung[3] > 1.5);
    free(swung);
}

TEST(swing_zero) {
    double grid[] = {0.0, 0.5, 1.0};
    double *swung = swing_apply(grid, 3, 0.0);
    ASSERT_TRUE(swung != NULL);
    ASSERT_FEQ(swung[1], 0.5, 0.001);
    free(swung);
}

TEST(groove_velocity_basic) {
    int pattern[] = {1, 0, 1, 0};
    int *vel = groove_velocity(pattern, 4, NULL);
    ASSERT_TRUE(vel != NULL);
    ASSERT_TRUE(vel[0] > 0);
    ASSERT_INT(vel[1], 0);
    ASSERT_TRUE(vel[2] > 0);
    ASSERT_INT(vel[3], 0);
    free(vel);
}

TEST(groove_velocity_accent) {
    int pattern[] = {1, 1, 1, 1};
    int accent[] = {2, 1, 2, 1};
    int *vel = groove_velocity(pattern, 4, accent);
    ASSERT_TRUE(vel != NULL);
    ASSERT_TRUE(vel[0] > vel[1]); /* accented > unaccented */
    free(vel);
}

TEST(microtiming_humanize_basic) {
    double offsets[] = {0.0, 5.0, -3.0, 10.0};
    double *humanized = microtiming_humanize(offsets, 4, 1.0, 42);
    ASSERT_TRUE(humanized != NULL);
    /* Values should differ from originals (with high probability) */
    /* Just check they're valid doubles (not NaN/Inf) */
    for (int i = 0; i < 4; i++) {
        ASSERT_TRUE(humanized[i] == humanized[i]); /* NaN check */
    }
    free(humanized);
}

TEST(microtiming_zero_amount) {
    double offsets[] = {1.0, 2.0};
    double *humanized = microtiming_humanize(offsets, 2, 0.0, 42);
    ASSERT_TRUE(humanized != NULL);
    ASSERT_FEQ(humanized[0], 1.0, 0.001);
    ASSERT_FEQ(humanized[1], 2.0, 0.001);
    free(humanized);
}

/* ======== SYNCOPATION TESTS ======== */

TEST(syncopation_all_downbeats) {
    int pattern[] = {1, 0, 1, 0, 1, 0, 1, 0};
    ASSERT_FEQ(syncopation_score(pattern, 8, 4), 0.0, 0.01);
}

TEST(syncopation_all_offbeats) {
    int pattern[] = {0, 1, 0, 1, 0, 1, 0, 1};
    double score = syncopation_score(pattern, 8, 4);
    ASSERT_TRUE(score > 0.0);
}

TEST(syncopation_empty) {
    ASSERT_FEQ(syncopation_score(NULL, 0, 4), 0.0, 0.01);
}

TEST(offbeat_density_all_on) {
    int pattern[] = {1, 0, 1, 0};
    ASSERT_FEQ(offbeat_density(pattern, 4, 4), 0.0, 0.01);
}

TEST(offbeat_density_mixed) {
    int pattern[] = {1, 1, 1, 1};
    double density = offbeat_density(pattern, 4, 4);
    ASSERT_TRUE(density > 0.0);
}

TEST(offbeat_empty) {
    ASSERT_FEQ(offbeat_density(NULL, 0, 4), 0.0, 0.01);
}

TEST(entropy_solid) {
    int pattern[] = {1, 1, 1, 1};
    ASSERT_FEQ(rhythmic_entropy(pattern, 4), 0.0, 0.01);
}

TEST(entropy_varied) {
    int pattern[] = {1, 0, 1, 0, 0, 1, 0, 1};
    double entropy = rhythmic_entropy(pattern, 8);
    ASSERT_TRUE(entropy > 0.0);
}

TEST(entropy_single) {
    int pattern[] = {1};
    ASSERT_FEQ(rhythmic_entropy(pattern, 1), 0.0, 0.01);
}

TEST(entropy_two) {
    int pattern[] = {1, 1};
    ASSERT_FEQ(rhythmic_entropy(pattern, 2), 0.0, 0.01);
}

/* ======== METER TESTS ======== */

TEST(meter_4_4) {
    Meter m = {4, 4};
    ASSERT_TRUE(meter_is_simple(&m));
    ASSERT_TRUE(!meter_is_compound(&m));
    ASSERT_TRUE(!meter_is_odd(&m));
    ASSERT_INT(meter_beat_count(&m), 4);
    ASSERT_INT(meter_subdivision(&m), 2);
}

TEST(meter_6_8_compound) {
    Meter m = {6, 8};
    ASSERT_TRUE(meter_is_compound(&m));
    ASSERT_INT(meter_beat_count(&m), 2);
    ASSERT_INT(meter_subdivision(&m), 3);
}

TEST(meter_7_8_odd) {
    Meter m = {7, 8};
    ASSERT_TRUE(meter_is_odd(&m));
    ASSERT_INT(meter_beat_count(&m), 7);
}

TEST(meter_3_4_simple) {
    Meter m = {3, 4};
    ASSERT_TRUE(meter_is_simple(&m));
    ASSERT_INT(meter_beat_count(&m), 3);
}

TEST(meter_downbeats) {
    Meter m = {4, 4};
    int *beats = meter_downbeats(3, &m);
    ASSERT_TRUE(beats != NULL);
    ASSERT_INT(beats[0], 0);
    ASSERT_INT(beats[1], 4);
    ASSERT_INT(beats[2], 8);
    free(beats);
}

TEST(meter_subdivisions) {
    Meter m = {4, 4};
    int count = 0;
    int *subs = meter_subdivisions(&m, 2, &count);
    ASSERT_TRUE(subs != NULL);
    ASSERT_INT(count, 8);
    free(subs);
}

TEST(time_signature_changes) {
    Meter meters[] = {{4, 4}, {3, 4}};
    int repeats[] = {2, 1};
    int out_count = 0;
    Meter *changes = time_signature_changes(meters, 2, repeats, &out_count);
    ASSERT_TRUE(changes != NULL);
    ASSERT_INT(out_count, 3);
    ASSERT_INT(changes[0].numerator, 4);
    ASSERT_INT(changes[2].numerator, 3);
    free(changes);
}

TEST(time_signature_changes_no_repeats) {
    Meter meters[] = {{4, 4}, {3, 4}};
    int out_count = 0;
    Meter *changes = time_signature_changes(meters, 2, NULL, &out_count);
    ASSERT_TRUE(changes != NULL);
    ASSERT_INT(out_count, 2);
    free(changes);
}

/* ======== INTEGRATION / CROSS-MODULE ======== */

TEST(tempo_polyrhythm_integration) {
    /* At 120bpm, a 3:2 polyrhythm cycle (6 subdivisions) should take
       6 * (500ms / subdivision_per_beat) depending on subdivision */
    Tempo t = {120.0};
    Polyrhythm p = {.rates = {3, 2}, .count = 2};
    double ms_per_beat = tempo_bpm_to_ms(t.bpm);
    ASSERT_FEQ(ms_per_beat, 500.0, 0.01);
    ASSERT_INT(polyrhythm_cycle_length(&p), 6);
}

TEST(groove_syncopation_integration) {
    /* A groove with all downbeat hits should have low syncopation */
    int pattern[] = {1, 0, 1, 0, 1, 0, 1, 0};
    int *vel = groove_velocity(pattern, 8, NULL);
    ASSERT_TRUE(vel != NULL);
    double score = syncopation_score(pattern, 8, 4);
    ASSERT_FEQ(score, 0.0, 0.01);
    free(vel);
}

TEST(meter_polyrhythm_odd_meter) {
    /* 7/8 meter with a 3:2 polyrhythm */
    Meter m = {7, 8};
    ASSERT_TRUE(meter_is_odd(&m));
    Polyrhythm p = {.rates = {3, 2}, .count = 2};
    ASSERT_INT(polyrhythm_cycle_length(&p), 6);
}

TEST(polyrhythm_density_range) {
    /* Density should be between 0 and 1 */
    Polyrhythm p = {.rates = {3, 2}, .count = 2};
    double d = polyrhythm_density(&p);
    ASSERT_TRUE(d > 0.0 && d <= 1.0);
}

TEST(groove_velocity_clamp) {
    /* Accent > 1 should clamp to 127 */
    int pattern[] = {1, 1};
    int accent[] = {3, 3};
    int *vel = groove_velocity(pattern, 2, accent);
    ASSERT_TRUE(vel != NULL);
    ASSERT_INT(vel[0], 127);
    free(vel);
}

/* ======== MAIN ======== */

int main(void) {
    int f;

    printf("=== rhythm-math-c test suite ===\n\n");

    /* Tempo */
    printf("[Tempo]\n");
    f = failed; RUN(tempo_bpm_to_ms_120);
    f = failed; RUN(tempo_bpm_to_ms_60);
    f = failed; RUN(tempo_ms_to_bpm_500);
    f = failed; RUN(tempo_ms_to_bpm_1000);
    f = failed; RUN(tempo_struct_120);
    f = failed; RUN(tempo_struct_60);
    f = failed; RUN(tempo_curve_linear);
    f = failed; RUN(tempo_curve_exponential);
    f = failed; RUN(tempo_curve_logarithmic);
    f = failed; RUN(tempo_curve_single_step);

    /* Polyrhythm */
    printf("\n[Polyrhythm]\n");
    f = failed; RUN(polyrhythm_lcm_basic);
    f = failed; RUN(polyrhythm_lcm_array);
    f = failed; RUN(polyrhythm_hemiola_cycle);
    f = failed; RUN(polyrhythm_hemiola_density);
    f = failed; RUN(polyrhythm_hemiola_complexity);
    f = failed; RUN(polyrhythm_pattern);
    f = failed; RUN(polyrhythm_4_3_cycle);
    f = failed; RUN(polyrhythm_single_rate);
    f = failed; RUN(polyrhythm_combined_cycle);

    /* Groove */
    printf("\n[Groove]\n");
    f = failed; RUN(groove_basic);
    f = failed; RUN(groove_empty);
    f = failed; RUN(swing_apply_basic);
    f = failed; RUN(swing_zero);
    f = failed; RUN(groove_velocity_basic);
    f = failed; RUN(groove_velocity_accent);
    f = failed; RUN(microtiming_humanize_basic);
    f = failed; RUN(microtiming_zero_amount);

    /* Syncopation */
    printf("\n[Syncopation]\n");
    f = failed; RUN(syncopation_all_downbeats);
    f = failed; RUN(syncopation_all_offbeats);
    f = failed; RUN(syncopation_empty);
    f = failed; RUN(offbeat_density_all_on);
    f = failed; RUN(offbeat_density_mixed);
    f = failed; RUN(offbeat_empty);
    f = failed; RUN(entropy_solid);
    f = failed; RUN(entropy_varied);
    f = failed; RUN(entropy_single);
    f = failed; RUN(entropy_two);

    /* Meter */
    printf("\n[Meter]\n");
    f = failed; RUN(meter_4_4);
    f = failed; RUN(meter_6_8_compound);
    f = failed; RUN(meter_7_8_odd);
    f = failed; RUN(meter_3_4_simple);
    f = failed; RUN(meter_downbeats);
    f = failed; RUN(meter_subdivisions);
    f = failed; RUN(time_signature_changes);
    f = failed; RUN(time_signature_changes_no_repeats);

    /* Integration */
    printf("\n[Integration]\n");
    f = failed; RUN(tempo_polyrhythm_integration);
    f = failed; RUN(groove_syncopation_integration);
    f = failed; RUN(meter_polyrhythm_odd_meter);
    f = failed; RUN(polyrhythm_density_range);
    f = failed; RUN(groove_velocity_clamp);

    printf("\n=== Results: %d passed, %d failed ===\n", passed, failed);
    return failed > 0 ? 1 : 0;
}
