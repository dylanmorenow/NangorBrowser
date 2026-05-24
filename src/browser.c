#include "browser.h"
#include "data.h"
#include "feature/f_history.h"
#include "feature/f_bookmark.h"
#include <stdio.h>
#include <string.h>

/* ============================================================
 * browser.c - Browser core: init + RNG
 * ============================================================ */

unsigned long browserRandNext(Browser *b) {
    b->randSeed = (RNG_A * b->randSeed + RNG_C) % RNG_M;
    return b->randSeed;
}

void browserInit(Browser *b) {
    setInit(&b->db);
    mapInit(&b->cache);
    graphInit(&b->webGraph);
    listInit(&b->tabs);
    queueInit(&b->dlQueue);
    b->randSeed = RNG_SEED;
    historyInit(&b->history);
    bookmarkInit(&b->bookmarks);
    dataInit(&b->db, &b->webGraph);
}
