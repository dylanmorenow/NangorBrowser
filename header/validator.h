#ifndef VALIDATOR_H
#define VALIDATOR_H


#include "config.h"

/* E01: validasi format URL
 * Return TRUE jika URL valid, FALSE jika tidak. */
bool validateURL(const char *url);

/* E02: validasi bilangan bulat positif 1-999999, max 6 digit
 * Jika valid, simpan nilai ke *result.
 * Return TRUE jika valid, FALSE jika tidak. */
bool validatePositiveInt(const char *str, int *result);

#endif 
