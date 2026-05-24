#include "set.h"
#include <string.h>
#include <stdio.h>

/* ============================================================
 * set.c — Implementasi ADT Set (sorted array + binary search)
 * ============================================================ */

void setInit(Set *s) {
    s->count  = 0;
    s->nextId = 1;  /* ID mulai dari 1, sesuai format CSV */
}

bool setIsFull(Set *s) {
    return (s->count >= MAX_WEB_PAGES);
}

/* --- BINARY SEARCH ---
 * Cari URL di sorted array. Return indeks jika ketemu, -1 jika tidak.
 * Ini adalah implementasi binary search standar. */
int setBinarySearch(Set *s, const char *url) {
    int low  = 0;
    int high = s->count - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;  /* Hindari overflow */
        int cmp = strcmp(s->pages[mid].url, url);

        if (cmp == 0) {
            return mid;   /* Ketemu! */
        } else if (cmp < 0) {
            low = mid + 1;    /* URL target lebih besar, cari ke kanan */
        } else {
            high = mid - 1;   /* URL target lebih kecil, cari ke kiri */
        }
    }

    return -1;  /* Tidak ditemukan */
}

WebPage* setSearch(Set *s, const char *url) {
    int idx = setBinarySearch(s, url);
    if (idx == -1) return NULL;
    return &s->pages[idx];
}

bool setInsert(Set *s, const char *url, const char *content) {
    if (setIsFull(s)) return FALSE;
    if (setSearch(s, url) != NULL) return FALSE;  /* URL sudah ada */

    /* Cari posisi insert yang benar (maintain sorted order).
     * Scan dari belakang dan geser ke kanan sampai ketemu posisi yang pas. */
    int i = s->count - 1;
    while (i >= 0 && strcmp(s->pages[i].url, url) > 0) {
        s->pages[i + 1] = s->pages[i];  /* Geser kanan */
        i--;
    }

    /* Isi slot yang baru */
    s->pages[i + 1].id = s->nextId++;
    strncpy(s->pages[i + 1].url, url, MAX_URL_LENGTH - 1);
    s->pages[i + 1].url[MAX_URL_LENGTH - 1] = '\0';
    strncpy(s->pages[i + 1].content, content, MAX_CONTENT_LENGTH - 1);
    s->pages[i + 1].content[MAX_CONTENT_LENGTH - 1] = '\0';

    s->count++;
    return TRUE;
}

bool setDelete(Set *s, const char *url) {
    int idx = setBinarySearch(s, url);
    if (idx == -1) return FALSE;

    /* Geser semua elemen ke kiri mulai dari idx+1 */
    int i;
    for (i = idx; i < s->count - 1; i++) {
        s->pages[i] = s->pages[i + 1];
    }
    s->count--;
    return TRUE;
}

bool setUpdate(Set *s, const char *url, const char *newContent) {
    int idx = setBinarySearch(s, url);
    if (idx == -1) return FALSE;

    strncpy(s->pages[idx].content, newContent, MAX_CONTENT_LENGTH - 1);
    s->pages[idx].content[MAX_CONTENT_LENGTH - 1] = '\0';
    return TRUE;
}

/* Prefix search: pakai binary search cari titik awal, lalu scan linear.
 * "Titik awal" = indeks pertama di mana url[i] >= prefix secara leksikografis. */
int setSearchPrefix(Set *s, const char *prefix,
                    WebPage *results[], int maxResults) {
    int prefixLen = (int)strlen(prefix);
    int found     = 0;

    /* Binary search untuk cari lower bound (posisi awal yang mungkin match) */
    int low = 0, high = s->count - 1, start = s->count;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        /* Bandingkan hanya sepanjang prefix */
        if (strncmp(s->pages[mid].url, prefix, prefixLen) < 0) {
            low = mid + 1;
        } else {
            start = mid;
            high  = mid - 1;
        }
    }

    /* Scan linear dari start selama prefix masih cocok */
    int i;
    for (i = start; i < s->count && found < maxResults; i++) {
        if (strncmp(s->pages[i].url, prefix, prefixLen) == 0) {
            results[found++] = &s->pages[i];
        } else {
            break;  /* Sudah melewati range prefix (array sorted) */
        }
    }

    return found;
}

void setPrintAll(Set *s) {
    if (s->count == 0) {
        printf("  (Database kosong)\n");
        return;
    }
    int i;
    for (i = 0; i < s->count; i++) {
        printf("  [%d] %s\n", s->pages[i].id, s->pages[i].url);
    }
}