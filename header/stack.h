#ifndef STACK_H
#define STACK_H

/* ============================================================
 * stack.h — ADT Stack untuk navigasi back/forward per tab
 *
 * KENAPA Stack?
 * Navigasi browser adalah use-case klasik stack: buka halaman
 * = push, back = pop. Tapi F08 perlu "lihat semua history"
 * dan "lompat N langkah", jadi kita pakai array + current index
 * (bukan stack murni yang hanya punya top).
 *
 * STRUKTUR:
 * urls[]       = array semua URL yang pernah dibuka di tab ini
 * size         = total URL tersimpan (termasuk yang "di depan"
 *                setelah back)
 * current      = indeks halaman yang sedang aktif
 *
 * Contoh: buka A, B, C, lalu back 1x
 *   urls    = ["A", "B", "C"]
 *   size    = 3
 *   current = 1   (sedang di B)
 *   → forward tersedia ke C, back tersedia ke A
 *
 * Kalau dari posisi current buka halaman baru D:
 *   urls    = ["A", "B", "D"]   (C dihapus/ditimpa)
 *   size    = 3
 *   current = 2
 * ============================================================ */

#include "config.h"

/* Ukuran max history satu tab (pakai HISTORY_MAX_AMOUNT dari config) */
#define STACK_MAX_SIZE HISTORY_MAX_AMOUNT

typedef struct {
    char urls[STACK_MAX_SIZE][MAX_URL_LENGTH];
    int  size;      /* jumlah URL valid di array */
    int  current;   /* indeks posisi sekarang (-1 = belum ada halaman) */
} Stack;

/* --- Operasi Stack --- */

/* Inisialisasi stack kosong. Wajib dipanggil sebelum pakai. */
void stackInit(Stack *s);

/* Cek apakah stack kosong (belum ada halaman sama sekali) */
bool stackIsEmpty(Stack *s);

/* Push URL baru ke stack.
 * - Potong semua URL di depan current (hapus forward history)
 * - Tambah URL baru sebagai current
 * - Return TRUE jika berhasil, FALSE jika stack penuh */
bool stackPush(Stack *s, const char *url);

/* Ambil URL di posisi current (halaman yang sedang dibuka).
 * Return NULL kalau stack kosong. */
const char* stackCurrent(Stack *s);

/* Back: geser current mundur 1.
 * Return URL tujuan kalau berhasil, NULL kalau sudah di paling awal. */
const char* stackBack(Stack *s);

/* Forward: geser current maju 1.
 * Return URL tujuan kalau berhasil, NULL kalau tidak ada forward history. */
const char* stackForward(Stack *s);

/* Back N langkah sekaligus (untuk F08: back <x>).
 * Return URL tujuan kalau berhasil, NULL kalau N terlalu besar. */
const char* stackBackN(Stack *s, int n);

/* Forward N langkah sekaligus (untuk F08: forward <x>).
 * Return URL tujuan kalau berhasil, NULL kalau N terlalu besar. */
const char* stackForwardN(Stack *s, int n);

/* Berapa langkah back yang tersedia dari posisi current? */
int stackBackAvailable(Stack *s);

/* Berapa langkah forward yang tersedia dari posisi current? */
int stackForwardAvailable(Stack *s);

/* Print semua URL di stack (untuk view_tab_history).
 * Menandai posisi current dengan "<- YOU ARE HERE". */
void stackPrintHistory(Stack *s);

#endif /* STACK_H */