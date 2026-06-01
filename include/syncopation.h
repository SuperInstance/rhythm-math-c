#ifndef RHYTHM_SYNCOPATION_H
#define RHYTHM_SYNCOPATION_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Longuet-Higgins/Lee syncopation measure */
double syncopation_score(const int *pattern, int length, int meter_numerator);

/* Fraction of hits on off-beats */
double offbeat_density(const int *pattern, int length, int meter_numerator);

/* Shannon entropy of the interval distribution */
double rhythmic_entropy(const int *pattern, int length);

#ifdef __cplusplus
}
#endif

#endif /* RHYTHM_SYNCOPATION_H */
