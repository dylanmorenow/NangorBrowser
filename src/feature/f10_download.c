#include "feature/f10_download.h"
#include <stdio.h>
#include <string.h>

/* ============================================================
 * f10_download.c — F10 Download Manager
 *
 * S02 (STI): Download selesai -> cukup print pesan.
 * ============================================================ */

void featureDownload(Browser *b, const char *url) {
    /* Cek apakah URL ada di database */
    WebPage *page = setSearch(&b->db, url);
    if (page == NULL) {
        printf("404 Not Found! Halaman tidak ditemukan.\n");
        return;
    }

    /* Cek apakah antrian penuh */
    if (queueIsFull(&b->dlQueue)) {
        printf("Download tidak diterima, antrian sudah penuh.\n");
        return;
    }

    /* Hitung ticks */
    int ticks = queueCalcTicks(url);

    /* Enqueue */
    queueEnqueue(&b->dlQueue, url);

    /* Info ke user */
    if (b->dlQueue.count == 1) {
        printf("Download %s (%d ticks)\n", url, ticks);
    } else {
        /* Hitung total ticks pending dari antrian sebelumnya */
        int pending = 0;
        int i, idx;
        for (i = 0; i < b->dlQueue.count - 1; i++) {
            idx = (b->dlQueue.front + i) % DOWNLOAD_MAX_AMOUNT;
            pending += b->dlQueue.items[idx].ticksRemaining;
        }
        printf("Download %s (%d ticks) -> antrian no %d, "
               "%d ticks masih tertunda dari antrian sebelumnya\n",
               url, ticks, b->dlQueue.count, pending);
    }
}

void featureTick(Browser *b) {
    queueTick(&b->dlQueue);
}
