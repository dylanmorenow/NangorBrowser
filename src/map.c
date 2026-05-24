#include "map.h"
#include <string.h>
#include <stdio.h>

/* ============================================================
 * map.c — Implementasi ADT Map (cache FIFO)
 * ============================================================ */

void mapInit(Map *m) {
    int i;
    for (i = 0; i < CACHE_MAX_AMOUNT; i++) {
        m->entries[i].isOccupied = FALSE;
        m->entries[i].insertOrder = 0;
    }
    m->count        = 0;
    m->orderCounter = 0;
}

bool mapIsFull(Map *m) {
    return (m->count >= CACHE_MAX_AMOUNT);
}

bool mapIsEmpty(Map *m) {
    return (m->count == 0);
}

int mapFindIndex(Map *m, const char *url) {
    int i;
    for (i = 0; i < CACHE_MAX_AMOUNT; i++) {
        if (m->entries[i].isOccupied &&
            strcmp(m->entries[i].url, url) == 0) {
            return i;
        }
    }
    return -1;
}

int mapFindFIFOVictim(Map *m) {
    int minOrder = -1;
    int minIdx   = -1;
    int i;

    for (i = 0; i < CACHE_MAX_AMOUNT; i++) {
        if (m->entries[i].isOccupied) {
            if (minIdx == -1 || m->entries[i].insertOrder < minOrder) {
                minOrder = m->entries[i].insertOrder;
                minIdx   = i;
            }
        }
    }
    return minIdx;
}

/* Cari slot kosong pertama di array */
static int mapFindEmptySlot(Map *m) {
    int i;
    for (i = 0; i < CACHE_MAX_AMOUNT; i++) {
        if (!m->entries[i].isOccupied) return i;
    }
    return -1;
}

const char* mapGet(Map *m, const char *url) {
    int idx = mapFindIndex(m, url);
    if (idx == -1) return NULL;  /* Cache-Miss */
    return m->entries[idx].content;
}

bool mapPut(Map *m, const char *url, const char *content) {
    /* Cek dulu apakah URL sudah ada di cache → update saja */
    int existing = mapFindIndex(m, url);
    if (existing != -1) {
        strncpy(m->entries[existing].content, content, MAX_CONTENT_LENGTH - 1);
        m->entries[existing].content[MAX_CONTENT_LENGTH - 1] = '\0';
        /* Update order supaya entry ini jadi "paling baru" */
        m->entries[existing].insertOrder = ++m->orderCounter;
        return TRUE;
    }

    /* Cache penuh → evict entry FIFO */
    if (mapIsFull(m)) {
        int victim = mapFindFIFOVictim(m);
        printf("  [Cache] Evicting '%s' (FIFO)\n", m->entries[victim].url);
        m->entries[victim].isOccupied = FALSE;
        m->count--;
    }

    /* Masukkan ke slot kosong */
    int slot = mapFindEmptySlot(m);
    if (slot == -1) return FALSE;  /* Seharusnya tidak terjadi */

    strncpy(m->entries[slot].url, url, MAX_URL_LENGTH - 1);
    m->entries[slot].url[MAX_URL_LENGTH - 1] = '\0';
    strncpy(m->entries[slot].content, content, MAX_CONTENT_LENGTH - 1);
    m->entries[slot].content[MAX_CONTENT_LENGTH - 1] = '\0';
    m->entries[slot].insertOrder = ++m->orderCounter;
    m->entries[slot].isOccupied  = TRUE;
    m->count++;

    return TRUE;
}

bool mapRemove(Map *m, const char *url) {
    int idx = mapFindIndex(m, url);
    if (idx == -1) return FALSE;

    m->entries[idx].isOccupied = FALSE;
    m->count--;
    return TRUE;
}

void mapPrint(Map *m) {
    if (mapIsEmpty(m)) {
        printf("  (Cache kosong)\n");
        return;
    }

    /* Print dalam urutan insert (FIFO order) agar informatif */
    int i;
    printf("  Cache (%d/%d):\n", m->count, CACHE_MAX_AMOUNT);
    for (i = 0; i < CACHE_MAX_AMOUNT; i++) {
        if (m->entries[i].isOccupied) {
            printf("    [order=%d] %s\n",
                   m->entries[i].insertOrder,
                   m->entries[i].url);
        }
    }
}