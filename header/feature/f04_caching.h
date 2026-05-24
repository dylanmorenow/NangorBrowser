#ifndef F04_CACHING_H
#define F04_CACHING_H

#include "browser.h"

/* F04 - Caching: cek cache sebelum akses database, FIFO eviction */
void browserDisplayPage(Browser *b, const char *url);

#endif
