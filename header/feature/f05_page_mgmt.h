#ifndef F05_PAGE_MGMT_H
#define F05_PAGE_MGMT_H

#include "browser.h"

void featureAddPage(Browser *b, const char *url);
void featureEditPage(Browser *b, const char *url);
void featureDeletePage(Browser *b, const char *url);

#endif 