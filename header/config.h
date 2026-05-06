#ifndef CONFIG_H
#define CONFIG_H

/* ============================================================
 * config.h — Konstanta global & tipe dasar
 *
 * KENAPA file ini ada?
 * Semua ADT dan fitur butuh konstanta yang sama (ukuran cache,
 * ukuran tab, dll). Daripada hardcode di mana-mana, kita
 * pusatkan di sini. Kalau mau ubah kapasitas, cukup ubah satu
 * tempat.
 * ============================================================ */

/* --- Kapasitas Sistem --- */
#define CACHE_MAX_AMOUNT    10      /* Max URL yang bisa di-cache */
#define TABS_MAX_AMOUNT     10      /* Max tab yang bisa dibuka */
#define DOWNLOAD_MAX_AMOUNT 5       /* Max item di antrian download */
#define MAX_WEB_PAGES       100     /* Max total halaman di database */
#define HISTORY_MAX_AMOUNT  50      /* Max entri di global history */

/* --- Ukuran String --- */
#define MAX_URL_LENGTH      256     /* Panjang max sebuah URL */
#define MAX_CONTENT_LENGTH  4096    /* Panjang max konten halaman */

/* --- LCG (Linear Congruential Generator) untuk F01-Discover ---
 * Parameter standar dari spesifikasi tugas.
 * LCG: X_(n+1) = (a * X_n + c) mod m
 */
#define LCG_MULTIPLIER      1103515245
#define LCG_INCREMENT       12345
#define LCG_MODULUS         2147483648U   /* 2^31, pakai unsigned */
#define LCG_SEED_INITIAL    73939133

/* --- Boolean sederhana (C99 punya _Bool, tapi ini lebih eksplisit) --- */
typedef int bool;
#define TRUE  1
#define FALSE 0

/* --- Tipe dasar: WebPage ---
 * Ini struct utama yang akan dipakai di hampir semua ADT.
 * Disimpan di sini supaya semua header bisa pakai tanpa circular include.
 */
typedef struct {
    int  id;
    char url[MAX_URL_LENGTH];
    char content[MAX_CONTENT_LENGTH];
} WebPage;

#endif /* CONFIG_H */