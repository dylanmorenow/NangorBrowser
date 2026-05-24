#include "feature/f06_tabs.h"
#include <stdio.h>

/* ============================================================
 * f06_tabs.c — F06 Tabs
 * ============================================================ */

void featureNewTab(Browser *b) {
    if (listIsFull(&b->tabs)) {
        printf("ERROR: Jumlah tab tidak bisa melebihi batas maksimum!\n");
        return;
    }
    listAddTab(&b->tabs);
}

void featureCloseTab(Browser *b) {
    if (b->tabs.count <= 1) {
        printf("ERROR: Tidak bisa menutup tab, tab minimal berjumlah 1!\n");
        return;
    }
    Tab *cur = listGetCurrentTab(&b->tabs);
    printf("%s berhasil ditutup.\n", cur->name);
    listCloseCurrentTab(&b->tabs);
}

void featureCheckTab(Browser *b) {
    listPrintTabs(&b->tabs);
}

void featurePrevTab(Browser *b, int n) {
    Tab *before = listGetCurrentTab(&b->tabs);
    if (!listPrevTab(&b->tabs, n)) {
        printf("ERROR: Posisi tab tidak valid!\n");
        return;
    }
    Tab *after = listGetCurrentTab(&b->tabs);
    printf("Tab saat ini berhasil diganti ke %s.\n", after->name);
    (void)before;
}

void featureNextTab(Browser *b, int n) {
    if (!listNextTab(&b->tabs, n)) {
        printf("ERROR: Posisi tab tidak valid!\n");
        return;
    }
    Tab *after = listGetCurrentTab(&b->tabs);
    printf("Tab saat ini berhasil diganti ke %s.\n", after->name);
}
