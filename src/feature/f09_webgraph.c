#include "feature/f09_webgraph.h"
#include "feature/f04_caching.h"
#include "feature/f_history.h"
#include <stdio.h>

/* ============================================================
 * f09_webgraph.c — F09 openlinked + B02 History recording
 * ============================================================ */

void featureOpenLinked(Browser *b, int index) {
    Tab *cur = listGetCurrentTab(&b->tabs);

    if (stackIsEmpty(&cur->navStack)) {
        printf("ERROR: COMMAND HANYA DAPAT DIGUNAKAN SAAT HALAMAN WEB TERBUKA!\n");
        return;
    }

    const char *currentUrl = stackCurrent(&cur->navStack);
    WebPage *page = setSearch(&b->db, currentUrl);
    if (page == NULL) {
        printf("ERROR: Halaman tidak ditemukan di database.\n");
        return;
    }

    LinkedList *neighbors = graphGetNeighbors(&b->webGraph, page->id);
    if (neighbors == NULL || llIsEmpty(neighbors)) {
        printf("ERROR: HALAMAN TIDAK MEMILIKI TAUTAN YANG BISA DIBUKA!\n");
        return;
    }

    const char *targetUrl = llGetByIndex(neighbors, index - 1);
    if (targetUrl == NULL) {
        printf("ERROR: Indeks %d tidak valid! (tersedia 1-%d)\n",
               index, neighbors->size);
        return;
    }

    stackPush(&cur->navStack, targetUrl);
    printf("Membuka: %s\n\n", targetUrl);
    browserDisplayPage(b, targetUrl);

    /* B02: Record ke global history */
    historyRecord(&b->history, targetUrl);
}
