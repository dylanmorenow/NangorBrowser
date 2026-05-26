#ifndef MAP_H
#define MAP_H


#include "config.h"

typedef struct {
    char url[MAX_URL_LENGTH];
    char content[MAX_CONTENT_LENGTH];
    int  insertOrder;   /* Counter urutan masuk, untuk FIFO eviction */
    bool isOccupied;    /* Apakah slot ini terisi? */
} MapEntry;

typedef struct {
    MapEntry entries[CACHE_MAX_AMOUNT];
    int      count;         /* Jumlah entry saat ini */
    int      orderCounter;  /* Counter global, naik setiap ada insert baru */
} Map;

/* --- Operasi Map --- */

/* Inisialisasi map kosong. */
void mapInit(Map *m);

/* Cek apakah map penuh. */
bool mapIsFull(Map *m);

/* Cek apakah map kosong. */
bool mapIsEmpty(Map *m);

/* Cari konten berdasarkan URL (Cache lookup).
 * Return pointer ke string konten jika ditemukan (Cache-Hit),
 * NULL jika tidak ditemukan (Cache-Miss). */
const char* mapGet(Map *m, const char *url);

/* Simpan pasangan URL-konten ke cache.
 * - Jika URL sudah ada, update konten (tidak tambah entry baru).
 * - Jika cache penuh, evict entry FIFO (insertOrder terkecil) dulu.
 * Return TRUE jika berhasil. */
bool mapPut(Map *m, const char *url, const char *content);

/* Hapus entry berdasarkan URL dari cache.
 * Diperlukan saat delete_page (F05) agar tidak ada stale cache.
 * Return TRUE jika berhasil, FALSE jika URL tidak ada di cache. */
bool mapRemove(Map *m, const char *url);

/* Print semua entry di cache (untuk debug / informasi). */
void mapPrint(Map *m);

/* Helper internal: cari indeks entry berdasarkan URL.
 * Return indeks jika ditemukan, -1 jika tidak. */
int mapFindIndex(Map *m, const char *url);

/* Helper internal: cari indeks entry dengan insertOrder terkecil (FIFO victim).
 * Return indeks entry yang akan di-evict. */
int mapFindFIFOVictim(Map *m);

#endif 