#include "browser.h"
#include "data.h"
#include "feature/f_history.h"
#include "feature/f_bookmark.h"
#include <stdio.h>
#include <string.h>

/* ============================================================
 * browser.c — Browser core: init, display, LCG
 * ============================================================ */

/* --- RNG untuk discover --- */
unsigned long browserRandNext(Browser *b) {
    b->randSeed = (RNG_A * b->randSeed + RNG_C) % RNG_M;
    return b->randSeed;
}

/* --- Init --- */
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

/* ============================================================
 * Helper: tampilkan konten halaman + linked pages
 * Dipanggil oleh open, back, forward, openlinked
 * ============================================================ */
void browserDisplayPage(Browser *b, const char *url) {
    /* Cek cache dulu */
    const char *content = mapGet(&b->cache, url);

    if (content != NULL) {
        printf("[Status: Cache-Hit] Mengambil data dari cache...\n\n");
    } else {
        /* Cache miss: cari di database */
        printf("[Status: Cache-Miss] Mengambil data dari database...\n\n");
        WebPage *page = setSearch(&b->db, url);
        if (page == NULL) {
            printf("404 Not Found! Halaman tidak ditemukan.\n");
            return;
        }
        /* Simpan ke cache */
        mapPut(&b->cache, url, page->content);
        content = mapGet(&b->cache, url);
    }

    /* Tampilkan konten */
    printf("%s\n\n", content);

    /* Tampilkan linked pages dari web graph */
    WebPage *page = setSearch(&b->db, url);
    if (page != NULL) {
        graphPrintNeighbors(&b->webGraph, page->id);
    }
    printf("\n");
}
