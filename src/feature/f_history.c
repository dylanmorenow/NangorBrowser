#include "feature/f_history.h"
#include <stdio.h>
#include <string.h>

/* ============================================================
 * f_history.c — B02 Global History (Priority Queue)
 *
 * Array-based priority queue, sorted descending by accessTime.
 * ============================================================ */

void historyInit(GlobalHistory *gh) {
    int i;
    for (i = 0; i < HISTORY_MAX_AMOUNT; i++) {
        gh->entries[i].isOccupied = 0;
        gh->entries[i].accessTime = 0;
    }
    gh->count       = 0;
    gh->timeCounter = 0;
}

/* Helper: cari URL di history. Return indeks, -1 jika tidak ada. */
static int findByUrl(GlobalHistory *gh, const char *url) {
    int i;
    for (i = 0; i < gh->count; i++) {
        if (gh->entries[i].isOccupied &&
            strcmp(gh->entries[i].url, url) == 0) {
            return i;
        }
    }
    return -1;
}

/* Helper: sort descending by accessTime (insertion sort) */
static void sortHistory(GlobalHistory *gh) {
    int i, j;
    for (i = 1; i < gh->count; i++) {
        HistoryEntry temp = gh->entries[i];
        j = i - 1;
        while (j >= 0 && gh->entries[j].accessTime < temp.accessTime) {
            gh->entries[j + 1] = gh->entries[j];
            j--;
        }
        gh->entries[j + 1] = temp;
    }
}

void historyRecord(GlobalHistory *gh, const char *url) {
    gh->timeCounter++;

    /* Cek apakah URL sudah ada di history */
    int existing = findByUrl(gh, url);
    if (existing != -1) {
        /* Update waktu akses saja */
        gh->entries[existing].accessTime = gh->timeCounter;
        sortHistory(gh);
        return;
    }

    /* URL belum ada: tambah baru */
    if (gh->count >= HISTORY_MAX_AMOUNT) {
        /* History penuh: hapus yang paling lama (terakhir setelah sort) */
        gh->entries[gh->count - 1].isOccupied = 0;
        gh->count--;
    }

    /* Tambahkan di akhir, lalu sort */
    strncpy(gh->entries[gh->count].url, url, MAX_URL_LENGTH - 1);
    gh->entries[gh->count].url[MAX_URL_LENGTH - 1] = '\0';
    gh->entries[gh->count].accessTime = gh->timeCounter;
    gh->entries[gh->count].isOccupied = 1;
    gh->count++;

    sortHistory(gh);
}

void featureViewHistory(GlobalHistory *gh) {
    if (gh->count == 0) {
        printf("Riwayat kosong, belum ada halaman yang pernah dibuka.\n");
        return;
    }

    printf("Riwayat halaman yang pernah dikunjungi:\n");
    int i;
    for (i = 0; i < gh->count; i++) {
        if (gh->entries[i].isOccupied) {
            printf("  [%d] %s (waktu: %d)\n",
                   i + 1, gh->entries[i].url, gh->entries[i].accessTime);
        }
    }
}
