#include "stack.h"
#include <string.h>
#include <stdio.h>

/* ============================================================
 * stack.c — Implementasi ADT Stack (navigasi tab)
 * ============================================================ */

void stackInit(Stack *s) {
    s->size    = 0;
    s->current = -1;  /* -1 = belum ada halaman sama sekali */
}

bool stackIsEmpty(Stack *s) {
    return (s->current == -1);
}

bool stackPush(Stack *s, const char *url) {
    /* Hapus semua URL di depan current (forward history hilang) */
    /* Contoh: current=1, size=4 → buang indeks 2 dan 3         */
    s->size = s->current + 1;

    /* Cek apakah masih ada ruang */
    if (s->size >= STACK_MAX_SIZE) {
        /* Stack penuh: geser semua ke kiri (buang yang paling lama) */
        int i;
        for (i = 0; i < STACK_MAX_SIZE - 1; i++) {
            strncpy(s->urls[i], s->urls[i + 1], MAX_URL_LENGTH - 1);
            s->urls[i][MAX_URL_LENGTH - 1] = '\0';
        }
        s->size    = STACK_MAX_SIZE - 1;
        s->current = STACK_MAX_SIZE - 2;
    }

    /* Tambah URL baru */
    strncpy(s->urls[s->size], url, MAX_URL_LENGTH - 1);
    s->urls[s->size][MAX_URL_LENGTH - 1] = '\0';
    s->current = s->size;
    s->size++;

    return TRUE;
}

const char* stackCurrent(Stack *s) {
    if (stackIsEmpty(s)) return NULL;
    return s->urls[s->current];
}

const char* stackBack(Stack *s) {
    if (s->current <= 0) return NULL;  /* Tidak ada halaman sebelumnya */
    s->current--;
    return s->urls[s->current];
}

const char* stackForward(Stack *s) {
    if (s->current >= s->size - 1) return NULL;  /* Tidak ada forward */
    s->current++;
    return s->urls[s->current];
}

const char* stackBackN(Stack *s, int n) {
    if (n <= 0) return NULL;
    if (s->current - n < 0) return NULL;  /* N terlalu besar */
    s->current -= n;
    return s->urls[s->current];
}

const char* stackForwardN(Stack *s, int n) {
    if (n <= 0) return NULL;
    if (s->current + n >= s->size) return NULL;  /* N terlalu besar */
    s->current += n;
    return s->urls[s->current];
}

int stackBackAvailable(Stack *s) {
    if (stackIsEmpty(s)) return 0;
    return s->current;  /* Ada current langkah ke belakang */
}

int stackForwardAvailable(Stack *s) {
    if (stackIsEmpty(s)) return 0;
    return s->size - 1 - s->current;
}

void stackPrintHistory(Stack *s) {
    if (stackIsEmpty(s)) {
        printf("  (Riwayat tab kosong)\n");
        return;
    }

    int i;
    for (i = 0; i < s->size; i++) {
        if (i == s->current) {
            printf("  [%d] %s  <- YOU ARE HERE\n", i, s->urls[i]);
        } else {
            printf("  [%d] %s\n", i, s->urls[i]);
        }
    }
}