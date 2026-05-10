#ifndef BROWSER_H
#define BROWSER_H

/* ============================================================
 * browser.h — State global browser
 *
 * Satu struct Browser menyimpan SEMUA state program:
 * - Database halaman (Set)
 * - Cache (Map)
 * - Web graph (Graph)
 * - Tabs (List)
 * - Download queue (Queue)
 * - LCG seed untuk discover
 *
 * Kenapa satu struct?
 * Supaya semua fungsi fitur cukup terima pointer ke Browser,
 * tidak perlu banyak parameter global yang berantakan.
 * ============================================================ */

#include "config.h"
#include "set.h"
#include "map.h"
#include "graph.h"
#include "list.h"
#include "queue.h"

typedef struct {
    Set    db;          /* Database semua halaman web */
    Map    cache;       /* Cache URL → konten */
    Graph  webGraph;    /* Web graph linked pages */
    List   tabs;        /* Daftar tab */
    Queue  dlQueue;     /* Antrian download */
    unsigned long lcgSeed; /* Seed LCG untuk discover */
} Browser;

/* Inisialisasi semua komponen browser + load hardcoded data. */
void browserInit(Browser *b);

/* ============================================================
 * FUNGSI FITUR — semua terima pointer ke Browser
 * ============================================================ */

/* F01 - Discover: tampilkan 5 URL acak pakai LCG */
void featureDiscover(Browser *b);

/* F02 - Search: prefix match pada URL */
void featureSearch(Browser *b, const char *query);

/* F03 - Open Page: buka halaman, cek cache dulu */
void featureOpenPage(Browser *b, const char *url);

/* F04 - Caching: dihandle otomatis dalam featureOpenPage */

/* F05 - Page Management */
void featureAddPage(Browser *b, const char *url);
void featureEditPage(Browser *b, const char *url);
void featureDeletePage(Browser *b, const char *url);

/* F06 - Tabs */
void featureNewTab(Browser *b);
void featureCloseTab(Browser *b);
void featureCheckTab(Browser *b);
void featurePrevTab(Browser *b, int n);
void featureNextTab(Browser *b, int n);

/* F07/F08 - Back / Forward */
void featureBack(Browser *b, int n);
void featureForward(Browser *b, int n);
void featureViewTabHistory(Browser *b);

/* F09 - openlinked: buka linked page berdasarkan nomor */
void featureOpenLinked(Browser *b, int index);

/* F10 - Download Manager */
void featureDownload(Browser *b, const char *url);
void featureTick(Browser *b);

/* F11 - Exit */
void featureExit(Browser *b);

/* Helper: tampilkan halaman (konten + linked pages).
 * Dipanggil oleh open, back, forward, openlinked. */
void browserDisplayPage(Browser *b, const char *url);

/* Helper: LCG — hasilkan angka acak berikutnya */
unsigned long browserLCGNext(Browser *b);

#endif /* BROWSER_H */