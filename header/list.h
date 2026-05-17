#ifndef LIST_H
#define LIST_H

/* ============================================================
 * list.h — ADT List untuk manajemen Tab (F06)
 *
 * KENAPA List?
 * Tab adalah kumpulan yang terurut by posisi (bukan by nilai).
 * Pengguna bisa prevtab/nexttab berdasarkan posisi relatif.
 * List (ordered sequence) adalah abstraksi yang tepat.
 *
 * IMPLEMENTASI: Array of Tab
 * - Ukuran max: TABS_MAX_AMOUNT (dari config.h)
 * - Insert: selalu di akhir (newtab)
 * - Delete: tab aktif saja (closetab), elemen setelahnya digeser kiri
 * - Access: by index (prevtab/nexttab)
 *
 * Setiap Tab punya:
 * - name          : "TAB1", "TAB2", dst (format auto-increment)
 * - navStack      : Stack navigasi back/forward milik tab ini
 *
 * COUNTER tab (tabCounter) terus naik meskipun tab di-close,
 * sehingga tab baru selalu punya nama unik (TAB4 setelah TAB3
 * di-close, bukan TAB3 lagi).
 * ============================================================ */

#include "config.h"
#include "stack.h"

/* Struct satu Tab */
typedef struct {
    char  name[16];         /* "TAB1", "TAB2", dst */
    Stack navStack;         /* History navigasi tab ini */
} Tab;

/* List of Tabs */
typedef struct {
    Tab tabs[TABS_MAX_AMOUNT];
    int count;          /* Jumlah tab yang aktif */
    int currentIndex;   /* Indeks tab yang sedang aktif (0-based) */
    int tabCounter;     /* Counter untuk nama tab, terus naik */
} List;

/* --- Operasi List (Tabs) --- */

/* Inisialisasi list: buat 1 tab default (TAB1). */
void listInit(List *l);

/* Cek apakah list penuh. */
bool listIsFull(List *l);

/* Tambah tab baru di akhir.
 * Return TRUE jika berhasil, FALSE jika sudah penuh. */
bool listAddTab(List *l);

/* Tutup tab pada currentIndex.
 * Geser elemen setelahnya ke kiri.
 * Sesuaikan currentIndex (pindah ke tab sebelah).
 * Return FALSE jika hanya tersisa 1 tab (tidak boleh close). */
bool listCloseCurrentTab(List *l);

/* Pindah currentIndex ke kiri sebanyak n posisi (prevtab <n>).
 * Return TRUE jika berhasil, FALSE jika posisi tidak valid. */
bool listPrevTab(List *l, int n);

/* Pindah currentIndex ke kanan sebanyak n posisi (nexttab <n>).
 * Return TRUE jika berhasil, FALSE jika posisi tidak valid. */
bool listNextTab(List *l, int n);

/* Ambil pointer ke tab yang sedang aktif. */
Tab* listGetCurrentTab(List *l);

/* Ambil pointer ke tab pada indeks tertentu (0-based).
 * Return NULL jika indeks invalid. */
Tab* listGetTab(List *l, int index);

/* Print daftar semua tab + tandai current (untuk checktab). */
void listPrintTabs(List *l);

#endif /* LIST_H */