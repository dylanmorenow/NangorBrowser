#ifndef F06_TABS_H
#define F06_TABS_H

#include "browser.h"

void featureNewTab(Browser *b);
void featureCloseTab(Browser *b);
void featureCheckTab(Browser *b);
void featurePrevTab(Browser *b, int n);
void featureNextTab(Browser *b, int n);

#endif 
