#include "feature/f_history.h"
#include <stdio.h>
#include <string.h>
#include <time.h>



void historyInit(GlobalHistory *gh) {
    int i;
    for (i = 0; i < HISTORY_MAX_AMOUNT; i++) {
        gh->entries[i].isOccupied = 0;
        gh->entries[i].accessTime = 0;
    }
    gh->count = 0;
}

/* Helper: cari URL di history */
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
    long now = (long)time(NULL);

    /* Cek apakah URL sudah ada di history */
    int existing = findByUrl(gh, url);
    if (existing != -1) {
        /* Update waktu akses saja */
        gh->entries[existing].accessTime = now;
        sortHistory(gh);
        return;
    }

    /* URL belum ada: tambah baru */
    if (gh->count >= HISTORY_MAX_AMOUNT) {
        /* History penuh: hapus yang paling lama (terakhir) */
        gh->entries[gh->count - 1].isOccupied = 0;
        gh->count--;
    }

    /* Tambahkan di akhir, lalu sort */
    strncpy(gh->entries[gh->count].url, url, MAX_URL_LENGTH - 1);
    gh->entries[gh->count].url[MAX_URL_LENGTH - 1] = '\0';
    gh->entries[gh->count].accessTime = now;
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
            /* Format waktu sebagai HH:MM */
            time_t t = (time_t)gh->entries[i].accessTime;
            struct tm *tm_info = localtime(&t);
            printf("  [%d] %s (%02d:%02d)\n",
                   i + 1,
                   gh->entries[i].url,
                   tm_info->tm_hour,
                   tm_info->tm_min);
        }
    }
}
