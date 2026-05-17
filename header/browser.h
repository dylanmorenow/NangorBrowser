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
 * - Seed RNG untuk discover
 * - Global History (B02)
 * - Bookmark Manager (B04)
 * ============================================================ */

#include "config.h"
#include "set.h"
#include "map.h"
#include "graph.h"
#include "list.h"
#include "queue.h"

/* ---- B02: Global History (priority queue) ---- */
typedef struct {
    char url[MAX_URL_LENGTH];
    int  accessTime;    /* Counter waktu akses (semakin besar = lebih baru) */
    int  isOccupied;
} HistoryEntry;

typedef struct {
    HistoryEntry entries[HISTORY_MAX_AMOUNT];
    int count;
    int timeCounter;
} GlobalHistory;

/* ---- B04: Bookmark Manager ---- */
#define BOOKMARK_MAX 50

typedef struct {
    char alias[64];
    char url[MAX_URL_LENGTH];
    int  isOccupied;
} BookmarkEntry;

typedef struct {
    BookmarkEntry entries[BOOKMARK_MAX];
    int count;
} BookmarkManager;

/* ---- Browser struct utama ---- */
typedef struct {
    Set    db;          /* Database semua halaman web */
    Map    cache;       /* Cache URL -> konten */
    Graph  webGraph;    /* Web graph linked pages */
    List   tabs;        /* Daftar tab */
    Queue  dlQueue;     /* Antrian download */
    unsigned long randSeed;    /* Seed RNG untuk discover */
    GlobalHistory  history;   /* B02: Global History */
    BookmarkManager bookmarks; /* B04: Bookmark Manager */
} Browser;

/* Inisialisasi semua komponen browser + load hardcoded data. */
void browserInit(Browser *b);

/* Helper: tampilkan halaman (konten + linked pages).
 * Dipanggil oleh open, back, forward, openlinked. */
void browserDisplayPage(Browser *b, const char *url);

/* Hasilkan angka acak berikutnya (untuk discover) */
unsigned long browserRandNext(Browser *b);

#endif /* BROWSER_H */
