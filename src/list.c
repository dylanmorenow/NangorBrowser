#include "list.h"
#include <stdio.h>
#include <string.h>

/* ============================================================
 * list.c — Implementasi ADT List (manajemen Tab)
 * ============================================================ */

/* Helper: inisialisasi satu tab dengan nama dan counter yang diberikan */
static void initTab(Tab *t, int counter) {
    snprintf(t->name, sizeof(t->name), "TAB%d", counter);
    stackInit(&t->navStack);
}

void listInit(List *l) {
    l->count        = 0;
    l->currentIndex = 0;
    l->tabCounter   = 1;

    /* Buat TAB1 sebagai tab default */
    initTab(&l->tabs[0], l->tabCounter++);
    l->count = 1;
}

bool listIsFull(List *l) {
    return (l->count >= TABS_MAX_AMOUNT);
}

bool listAddTab(List *l) {
    if (listIsFull(l)) return FALSE;

    initTab(&l->tabs[l->count], l->tabCounter);
    printf("Tab baru (%s) berhasil dibuat!\n",
           l->tabs[l->count].name);

    l->tabCounter++;
    l->count++;
    return TRUE;
}

bool listCloseCurrentTab(List *l) {
    if (l->count <= 1) return FALSE;  /* Minimal 1 tab */

    int closedIdx = l->currentIndex;

    /* Geser semua tab setelah closedIdx ke kiri */
    int i;
    for (i = closedIdx; i < l->count - 1; i++) {
        l->tabs[i] = l->tabs[i + 1];
    }
    l->count--;

    /* Sesuaikan currentIndex:
     * - Kalau tab yang di-close bukan yang terakhir → current tetap
     *   di indeks yang sama (sekarang menunjuk ke tab berikutnya)
     * - Kalau tab yang di-close adalah yang terakhir → geser ke kiri */
    if (l->currentIndex >= l->count) {
        l->currentIndex = l->count - 1;
    }

    return TRUE;
}

bool listPrevTab(List *l, int n) {
    if (n <= 0) return FALSE;
    int newIdx = l->currentIndex - n;
    if (newIdx < 0) return FALSE;  /* Posisi tidak valid */
    l->currentIndex = newIdx;
    return TRUE;
}

bool listNextTab(List *l, int n) {
    if (n <= 0) return FALSE;
    int newIdx = l->currentIndex + n;
    if (newIdx >= l->count) return FALSE;  /* Posisi tidak valid */
    l->currentIndex = newIdx;
    return TRUE;
}

Tab* listGetCurrentTab(List *l) {
    return &l->tabs[l->currentIndex];
}

Tab* listGetTab(List *l, int index) {
    if (index < 0 || index >= l->count) return NULL;
    return &l->tabs[index];
}

void listPrintTabs(List *l) {
    printf("List of tab(s):\n");
    int i;
    for (i = 0; i < l->count; i++) {
        printf("  [%d] %s", i + 1, l->tabs[i].name);
        if (i == l->currentIndex) printf("  *");
        printf("\n");
    }
    printf("Current tab: %s\n", l->tabs[l->currentIndex].name);
}