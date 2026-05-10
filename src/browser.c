#include "browser.h"
#include "data.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ============================================================
 * browser.c — Implementasi semua fitur browser
 * ============================================================ */

/* --- LCG --- */
unsigned long browserLCGNext(Browser *b) {
    b->lcgSeed = (LCG_MULTIPLIER * b->lcgSeed + LCG_INCREMENT) % LCG_MODULUS;
    return b->lcgSeed;
}

/* --- Init --- */
void browserInit(Browser *b) {
    setInit(&b->db);
    mapInit(&b->cache);
    graphInit(&b->webGraph);
    listInit(&b->tabs);
    queueInit(&b->dlQueue);
    b->lcgSeed = LCG_SEED_INITIAL;
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

/* ============================================================
 * F01 - Discover
 * ============================================================ */


/* ============================================================
 * F02 - Search
 * ============================================================ */


/* ============================================================
 * F03 - Open Page
 * ============================================================ */

/* ============================================================
 * F05 - Page Management
 * ============================================================ */


/* ============================================================
 * F06 - Tabs
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

/* ============================================================
 * F07/F08 - Back / Forward
 * ============================================================ */
void featureBack(Browser *b, int n) {
    Tab *cur = listGetCurrentTab(&b->tabs);

    if (stackIsEmpty(&cur->navStack)) {
        printf("ERROR: BACK TIDAK BISA DIJALANKAN KARENA TIDAK ADA HALAMAN SEBELUMNYA!\n");
        return;
    }

    const char *dest = (n == 1) ? stackBack(&cur->navStack)
                                : stackBackN(&cur->navStack, n);
    if (dest == NULL) {
        printf("ERROR: BACK TIDAK BISA DIGUNAKAN LAGI KARENA TIDAK ADA HALAMAN SEBELUMNYA!\n");
        /* Tampilkan halaman saat ini */
        const char *current = stackCurrent(&cur->navStack);
        if (current != NULL) browserDisplayPage(b, current);
        return;
    }

    printf("BACK: KEMBALI KE HALAMAN %s\n\n", dest);
    browserDisplayPage(b, dest);
}

void featureForward(Browser *b, int n) {
    Tab *cur = listGetCurrentTab(&b->tabs);

    if (stackIsEmpty(&cur->navStack)) {
        printf("ERROR: FORWARD TIDAK BISA DIJALANKAN KARENA TIDAK ADA HALAMAN SELANJUTNYA!\n");
        return;
    }

    const char *dest = (n == 1) ? stackForward(&cur->navStack)
                                : stackForwardN(&cur->navStack, n);
    if (dest == NULL) {
        printf("ERROR: FORWARD TIDAK BISA DIJALANKAN KARENA TIDAK ADA HALAMAN SELANJUTNYA!\n");
        const char *current = stackCurrent(&cur->navStack);
        if (current != NULL) browserDisplayPage(b, current);
        return;
    }

    printf("FORWARD: KE HALAMAN %s\n\n", dest);
    browserDisplayPage(b, dest);
}

void featureViewTabHistory(Browser *b) {
    Tab *cur = listGetCurrentTab(&b->tabs);
    printf("Tab History [%s]:\n", cur->name);
    stackPrintHistory(&cur->navStack);

    /* Tampilkan halaman saat ini */
    const char *current = stackCurrent(&cur->navStack);
    if (current != NULL) {
        printf("\n");
        browserDisplayPage(b, current);
    }
}

/* ============================================================
 * F09 - openlinked
 * ============================================================ */
void featureOpenLinked(Browser *b, int index) {
    Tab *cur = listGetCurrentTab(&b->tabs);

    /* Harus ada halaman yang sedang dibuka */
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

    /* Index dari user adalah 1-based */
    const char *targetUrl = llGetByIndex(neighbors, index - 1);
    if (targetUrl == NULL) {
        printf("ERROR: Indeks %d tidak valid! (tersedia 1-%d)\n",
               index, neighbors->size);
        return;
    }

    /* Push ke stack dan tampilkan */
    stackPush(&cur->navStack, targetUrl);
    printf("Membuka: %s\n\n", targetUrl);
    browserDisplayPage(b, targetUrl);
}

/* ============================================================
 * F10 - Download Manager
 * ============================================================ */


/* ============================================================
 * F11 - Exit
 * ============================================================ */
