# rhythm-math-c

> Rhythm mathematics in C — polyrhythms, syncopation, groove, meter, and tempo.

## What This Does

`rhythm-math-c` provides pure rhythm math in C. Model time signatures, polyrhythms, syncopation (Longuet-Higgins/Lee), groove patterns, and tempo curves. No audio — just the math. Use it for embedded audio systems, game audio engines, or high-performance music analysis.

## The Cultural Root

See `rhythm-nation-math` (PyPI) for the full cultural background. Polyrhythms from West African drumming — math as LCM problems.

## Install

```bash
git clone https://github.com/SuperInstance/rhythm-math-c.git
cd rhythm-math-c
make
```

## Quick Start

```c
#include "rhythm_api.h"

int main() {
    // Tempo
    Tempo t = {.bpm = 120.0};
    printf("ms/beat: %.1f\n", tempo_bpm_to_ms(120.0));  // 500.0
    printf("Hz: %.2f\n", tempo_hz(&t));                  // 2.0

    // Tempo curve
    double curve[8];
    tempo_curve_linear(120.0, 60.0, 8, curve);

    // Polyrhythm
    Polyrhythm pr = {.rates = {3, 2}, .count = 2};
    int cycle_len = polyrhythm_cycle_length(&pr);  // 6
    int *pattern = polyrhythm_pattern(&pr);
    double density = polyrhythm_density(&pr);

    // Syncopation
    int rhythm[] = {1, 0, 0, 1, 0, 0, 1, 0};
    double sync = syncopation_score(rhythm, 8, 4);
    double offbeat = offbeat_density(rhythm, 8, 4);
    double entropy = rhythmic_entropy(rhythm, 8);

    // Meter
    Meter m = {.numerator = 7, .denominator = 8};
    printf("Odd: %d\n", meter_is_odd(&m));  // 1

    // Groove
    GroovePattern g = {.velocities = {100, 80, 90, 70}, .count = 4};
    double avg_vel = groove_average_velocity(&g);

    return 0;
}
```

## API Reference

### Tempo (`tempo.h`)
- `double tempo_bpm_to_ms(double bpm)`
- `double tempo_ms_to_bpm(double ms)`
- `double tempo_hz(const Tempo *t)`
- `double tempo_period(const Tempo *t)`
- `double *tempo_curve_linear(double start, double end, int steps, double *out)`
- `double *tempo_curve_exponential(double start, double end, int steps, double *out)`
- `double *tempo_curve_logarithmic(double start, double end, int steps, double *out)`

### Polyrhythm (`polyrhythm.h`)
- `int polyrhythm_cycle_length(const Polyrhythm *p)`
- `int *polyrhythm_pattern(const Polyrhythm *p)`
- `double polyrhythm_density(const Polyrhythm *p)`
- `double polyrhythm_complexity(const Polyrhythm *p)`
- `int polyrhythm_lcm2(int a, int b)`

### Syncopation (`syncopation.h`)
- `double syncopation_score(const int *pattern, int length, int meter_numerator)`
- `double offbeat_density(const int *pattern, int length, int meter_numerator)`
- `double rhythmic_entropy(const int *pattern, int length)`

### Meter (`meter.h`)
- `int meter_is_simple(const Meter *m)`
- `int meter_is_compound(const Meter *m)`
- `int meter_is_odd(const Meter *m)`
- `int meter_beat_count(const Meter *m)`
- `int *meter_downbeats(int measures, const Meter *m)`

### Groove (`groove.h`)
- `double groove_average_velocity(const GroovePattern *g)`
- `double *swing_apply(const double *grid, int length, double amount)`
- `int *groove_velocity(const int *pattern, int length, const int *accent_pattern)`
- `double *microtiming_humanize(const double *offsets, int length, double amount, unsigned int seed)`

## License

MIT
