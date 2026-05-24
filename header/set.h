#ifndef SET_H
#define SET_H

/* ============================================================
 * set.h — ADT Set untuk database halaman web (F03)
 *
 * KENAPA Set?
 * Database halaman web adalah kumpulan WebPage yang unik
 * (tidak boleh URL duplikat). Set adalah abstraksi yang tepat:
 * - Setiap elemen unik (diidentifikasi oleh URL)
 * - Operasi: contains, insert, delete
 *
 * IMPLEMENTASI: Sorted Array + Binary Search
 * Array di-maintain selalu terurut berdasarkan URL (strcmp order).
 * Ini memungkinkan binary search O(log n) untuk lookup.
 *
 * KENAPA binary search wajib?
 * Spesifikasi STI mewajibkan binary search di Array Search/Sort/Filter.
 * Lookup halaman (F03) adalah operasi paling sering → harus efisien.
 *
 * TRADEOFF:
 * - Insert: O(n) karena harus geser elemen → tapi insert jarang
 * - Search: O(log n) → ini yang sering dipanggil, jadi worthit
 * ============================================================ */

#include "config.h"

typedef struct {
    WebPage pages[MAX_WEB_PAGES];
    int     count;       /* Jumlah halaman saat ini */
    int     nextId;      /* ID berikutnya (auto-increment) */
} Set;

/* --- Operasi Set --- */

/* Inisialisasi set kosong. */
void setInit(Set *s);

/* Cek apakah set penuh. */
bool setIsFull(Set *s);

/* Cari WebPage berdasarkan URL menggunakan BINARY SEARCH.
 * Return pointer ke WebPage jika ditemukan, NULL jika tidak.
 * CATATAN: Array harus sudah sorted (selalu dijaga oleh setInsert). */
WebPage* setSearch(Set *s, const char *url);

/* Insert halaman baru ke set.
 * - Validasi: URL belum ada di set
 * - Insert di posisi yang benar (maintain sorted order)
 * - Auto-assign ID
 * Return TRUE jika berhasil, FALSE jika URL sudah ada atau set penuh. */
bool setInsert(Set *s, const char *url, const char *content);

/* Delete halaman berdasarkan URL.
 * Return TRUE jika berhasil, FALSE jika URL tidak ditemukan. */
bool setDelete(Set *s, const char *url);

/* Update konten halaman (untuk edit_page).
 * Return TRUE jika berhasil, FALSE jika URL tidak ditemukan. */
bool setUpdate(Set *s, const char *url, const char *newContent);

/* Prefix search: cari semua URL yang dimulai dengan prefix tertentu (F02).
 * Hasil disimpan ke results[], jumlah hasil di-return.
 * Pakai binary search untuk cari titik awal, lalu scan linear. */
int setSearchPrefix(Set *s, const char *prefix,
                    WebPage *results[], int maxResults);

/* Print semua halaman (untuk debug). */
void setPrintAll(Set *s);

/* Fungsi helper internal: binary search murni.
 * Return indeks jika ditemukan, -1 jika tidak. */
int setBinarySearch(Set *s, const char *url);

#endif /* SET_H */