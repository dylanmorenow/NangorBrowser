#ifndef QUEUE_H
#define QUEUE_H

/* ============================================================
 * queue.h — ADT Queue untuk Download Manager (F10/S02)
 *
 * KENAPA Queue?
 * Download diproses secara FIFO: yang masuk duluan selesai duluan.
 * Queue adalah struktur data yang paling natural untuk ini.
 *
 * IMPLEMENTASI: Circular Array
 * Lebih efisien daripada linked list untuk ukuran fixed ini karena:
 * - Enqueue dan dequeue O(1)
 * - Tidak perlu alokasi heap (malloc)
 * - Ukuran max sudah diketahui (DOWNLOAD_MAX_AMOUNT)
 *
 * Rumus tick: N = floor(len(URL) / 5) + 2
 * ============================================================ */

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

#endif /* QUEUE_H */