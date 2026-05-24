#ifndef VALIDATOR_H
#define VALIDATOR_H

/* ============================================================
 * validator.h — Validasi input
 *
 * E01: Validasi format URL — manual tanpa regex library
 *      Regex target: ^(?!-)[A-Za-z0-9-]{1,63}(?<!-)(\.[A-Za-z]{2,})+$
 *
 * E02: Validasi bilangan bulat positif 1-999999
 * ============================================================ */

#include "config.h"

/* E01: validasi format URL
 * Return TRUE jika URL valid, FALSE jika tidak. */
bool validateURL(const char *url);

/* E02: validasi bilangan bulat positif 1-999999, max 6 digit
 * Jika valid, simpan nilai ke *result.
 * Return TRUE jika valid, FALSE jika tidak. */
bool validatePositiveInt(const char *str, int *result);

#endif /* VALIDATOR_H */
