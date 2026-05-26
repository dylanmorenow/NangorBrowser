#ifndef QUEUE_H
#define QUEUE_H

#include "config.h"

typedef struct {
    char url[MAX_URL_LENGTH];
    int  ticksRemaining;   /* Sisa tick sampai selesai */
    int  ticksTotal;       /* Total tick saat pertama kali masuk (untuk info) */
} DownloadItem;

typedef struct {
    DownloadItem items[DOWNLOAD_MAX_AMOUNT];
    int front;   /* Indeks item terdepan (yang sedang diproses) */
    int rear;    /* Indeks slot kosong berikutnya */
    int count;   /* Jumlah item saat ini */
} Queue;

/* --- Operasi Queue --- */

/* Inisialisasi queue kosong. */
void queueInit(Queue *q);

/* Cek apakah queue kosong. */
bool queueIsEmpty(Queue *q);

/* Cek apakah queue penuh. */
bool queueIsFull(Queue *q);

/* Hitung ticks yang dibutuhkan untuk URL tertentu.
 * Rumus: floor(strlen(url) / 5) + 2 */
int queueCalcTicks(const char *url);

/* Tambah download baru ke belakang antrian.
 * Return TRUE jika berhasil, FALSE jika antrian penuh. */
bool queueEnqueue(Queue *q, const char *url);

/* Proses 1 tick.
 * - Kurangi ticks item terdepan.
 * - Jika ticks habis, item dianggap selesai dan dikeluarkan (dequeue otomatis).
 * - Output: print status download.
 * Return TRUE jika ada item yang diproses, FALSE jika antrian kosong. */
bool queueTick(Queue *q);

/* Lihat item terdepan tanpa menghapus (peek). */
DownloadItem* queuePeek(Queue *q);

/* Hapus item terdepan (dipanggil otomatis saat download selesai). */
void queueDequeue(Queue *q);

/* Print semua item di antrian (untuk informasi). */
void queuePrint(Queue *q);

#endif