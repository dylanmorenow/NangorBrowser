#include "feature/f08_tabs_history.h"
#include "feature/f04_caching.h"
#include <stdio.h>


void featureViewTabHistory(Browser *b) {
    Tab *cur = listGetCurrentTab(&b->tabs);
    printf("Tab History [%s]:\n", cur->name);
    stackPrintHistory(&cur->navStack);

    const char *current = stackCurrent(&cur->navStack);
    if (current != NULL) {
        printf("\n");
        browserDisplayPage(b, current);
    }
}
