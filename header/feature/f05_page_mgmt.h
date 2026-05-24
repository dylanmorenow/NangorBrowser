#ifndef F05_PAGE_MGMT_H
#define F05_PAGE_MGMT_H

#include "browser.h"

/* F05 - Page Management */
void featureAddPage(Browser *b, const char *url);
void featureEditPage(Browser *b, const char *url);
void featureDeletePage(Browser *b, const char *url);

#endif /* F05_PAGE_MGMT_H */
