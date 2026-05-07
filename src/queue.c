#include "queue.h"
#include <string.h>
#include <stdio.h>

/* ============================================================
 * queue.c — Implementasi ADT Queue (circular array)
 * ============================================================ */

void queueInit(Queue *q) {
    q->front = 0;
    q->rear  = 0;
    q->count = 0;
}

bool queueIsEmpty(Queue *q) {
    return (q->count == 0);
}

bool queueIsFull(Queue *q) {
    return (q->count == DOWNLOAD_MAX_AMOUNT);
}

int queueCalcTicks(const char *url) {
    int len = (int)strlen(url);
    return (len / 5) + 2;
}

bool queueEnqueue(Queue *q, const char *url) {
    if (queueIsFull(q)) return FALSE;

    int ticks = queueCalcTicks(url);

    strncpy(q->items[q->rear].url, url, MAX_URL_LENGTH - 1);
    q->items[q->rear].url[MAX_URL_LENGTH - 1] = '\0';
    q->items[q->rear].ticksRemaining = ticks;
    q->items[q->rear].ticksTotal     = ticks;

    /* Circular: wrap around kalau sudah ujung array */
    q->rear = (q->rear + 1) % DOWNLOAD_MAX_AMOUNT;
    q->count++;

    return TRUE;
}

void queueDequeue(Queue *q) {
    if (queueIsEmpty(q)) return;
    q->front = (q->front + 1) % DOWNLOAD_MAX_AMOUNT;
    q->count--;
}

DownloadItem* queuePeek(Queue *q) {
    if (queueIsEmpty(q)) return NULL;
    return &q->items[q->front];
}

bool queueTick(Queue *q) {
    if (queueIsEmpty(q)) {
        printf("Antrian download saat ini kosong.\n");
        return FALSE;
    }

    DownloadItem *active = queuePeek(q);
    active->ticksRemaining--;

    if (active->ticksRemaining <= 0) {
        /* Download selesai! (S02: cukup print pesan) */
        printf("%s selesai terdownload!\n", active->url);
        queueDequeue(q);

        /* Kalau masih ada antrian berikutnya, info-kan */
        if (!queueIsEmpty(q)) {
            DownloadItem *next = queuePeek(q);
            printf("Lanjut downloading %s... (%d ticks tersisa)\n",
                   next->url, next->ticksRemaining);
        }
    } else {
        /* Masih dalam proses */
        printf("Downloading %s... (%d ticks tersisa)\n",
               active->url, active->ticksRemaining);
    }

    return TRUE;
}

void queuePrint(Queue *q) {
    if (queueIsEmpty(q)) {
        printf("  (Antrian download kosong)\n");
        return;
    }

    int i, idx;
    for (i = 0; i < q->count; i++) {
        idx = (q->front + i) % DOWNLOAD_MAX_AMOUNT;
        if (i == 0) {
            printf("  [AKTIF] %s (%d/%d ticks)\n",
                   q->items[idx].url,
                   q->items[idx].ticksRemaining,
                   q->items[idx].ticksTotal);
        } else {
            printf("  [%d]     %s (%d ticks)\n",
                   i, q->items[idx].url, q->items[idx].ticksTotal);
        }
    }
}