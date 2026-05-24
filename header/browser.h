#ifndef BROWSER_H
#define BROWSER_H

#include "config.h"
#include "set.h"
#include "map.h"
#include "graph.h"
#include "list.h"
#include "queue.h"

/* B02: Global History */
typedef struct {
    char url[MAX_URL_LENGTH];
    int  accessTime;
    int  isOccupied;
} HistoryEntry;

typedef struct {
    HistoryEntry entries[HISTORY_MAX_AMOUNT];
    int count;
    int timeCounter;
} GlobalHistory;

/* B04: Bookmark Manager */
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

/* Browser - struct utama menyimpan semua state */
typedef struct {
    Set    db;
    Map    cache;
    Graph  webGraph;
    List   tabs;
    Queue  dlQueue;
    unsigned long randSeed;
    GlobalHistory  history;
    BookmarkManager bookmarks;
} Browser;

void browserInit(Browser *b);
unsigned long browserRandNext(Browser *b);

/* browserDisplayPage ada di f04_caching.h */

#endif
