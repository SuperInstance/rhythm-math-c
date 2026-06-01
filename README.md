# rhythm-math-c

**The mathematics of rhythm — C99 edition.** Polyrhythms, meters, groove analysis, syncopation metrics, and tempo math for hardware devices, drum machines, sequencers, and embedded audio.

A C99 port of [rhythm-nation-math](https://github.com/SuperInstance/rhythm-nation-math). Zero external dependencies.

## Build

```bash
make          # Build static library (librhythmmath.a)
make test     # Build and run tests
make clean    # Clean build artifacts
```

## Usage

```c
#include "rhythm_api.h"

/* Tempo */
double ms = tempo_bpm_to_ms(120.0);  // 500.0

/* Polyrhythm */
Polyrhythm p = {.rates = {3, 2}, .count = 2};
int cycle = polyrhythm_cycle_length(&p);  // 6

/* Meter */
Meter m = {4, 4};
int beats = meter_beat_count(&m);  // 4

/* Syncopation */
int pattern[] = {0, 1, 0, 1, 0, 1, 0, 1};
double score = syncopation_score(pattern, 8, 4);

/* Groove */
double grid[] = {0.0, 0.5, 1.0, 1.5};
double *swung = swing_apply(grid, 4, 0.5);
```

## Modules

| Module | Header | Description |
|--------|--------|-------------|
| Tempo | `tempo.h` | BPM↔ms conversion, frequency, tempo curves (linear/exponential/logarithmic) |
| Polyrhythm | `polyrhythm.h` | LCM cycle length, binary patterns, density, complexity |
| Groove | `groove.h` | Swing, velocity mapping, microtiming humanization |
| Syncopation | `syncopation.h` | Longuet-Higgins/Lee syncopation, offbeat density, rhythmic entropy |
| Meter | `meter.h` | Time signatures, simple/compound/odd detection, downbeats, subdivisions |
| API | `rhythm_api.h` | Unified include for all modules |

## API

### Tempo

- `tempo_bpm_to_ms(bpm)` — BPM → milliseconds per beat
- `tempo_ms_to_bpm(ms)` — milliseconds per beat → BPM
- `tempo_hz(&t)` — frequency in Hz
- `tempo_period(&t)` — period in seconds
- `tempo_curve_linear(start, end, steps)` — linear tempo ramp (caller frees)
- `tempo_curve_exponential(start, end, steps)` — exponential ramp (caller frees)
- `tempo_curve_logarithmic(start, end, steps)` — logarithmic ramp (caller frees)

### Polyrhythm

- `polyrhythm_cycle_length(&p)` — LCM of all rates
- `polyrhythm_pattern(&p)` — flat int array, count × cycle_length (caller frees)
- `polyrhythm_density(&p)` — total hits / cycle length
- `polyrhythm_complexity(&p)` — cycle length / sum of rates
- `polyrhythm_combined_cycle(rates, count)` — OR of all voices (caller frees)

### Groove

- `groove_average_velocity(&g)` — mean velocity
- `swing_apply(grid, length, amount)` — apply swing to beat positions (caller frees)
- `groove_velocity(pattern, length, accents)` — binary → MIDI velocity (caller frees)
- `microtiming_humanize(offsets, length, amount, seed)` — gaussian humanization (caller frees)

### Syncopation

- `syncopation_score(pattern, length, meter)` — Longuet-Higgins/Lee measure
- `offbeat_density(pattern, length, meter)` — fraction of off-beat hits
- `rhythmic_entropy(pattern, length)` — Shannon entropy of intervals

### Meter

- `meter_is_simple(&m)` / `meter_is_compound(&m)` / `meter_is_odd(&m)`
- `meter_beat_count(&m)` — main beats per measure
- `meter_subdivision(&m)` — subdivisions per beat
- `meter_downbeats(measures, &m)` — downbeat positions (caller frees)
- `meter_subdivisions(&m, level, &count)` — subdivision grid (caller frees)
- `time_signature_changes(meters, count, repeats, &out_count)` — meter sequence (caller frees)

## License

MIT
