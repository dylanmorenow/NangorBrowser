#include "feature/f07_back_forward.h"
#include "feature/f04_caching.h"
#include "feature/f_history.h"
#include <stdio.h>

/* ============================================================
 * f07_back_forward.c - F07 Back / Forward
 * ============================================================ */

void featureBack(Browser *b, int n) {
    Tab *cur = listGetCurrentTab(&b->tabs);

    if (stackIsEmpty(&cur->navStack)) {
        printf("ERROR: BACK TIDAK BISA DIJALANKAN KARENA TIDAK ADA HALAMAN SEBELUMNYA!\n");
        return;
    }

    const char *dest = (n == 1) ? stackBack(&cur->navStack)
                                : stackBackN(&cur->navStack, n);
    if (dest == NULL) {
        printf("ERROR: BACK TIDAK BISA DIGUNAKAN LAGI KARENA TIDAK ADA HALAMAN SEBELUMNYA!\n");
        const char *current = stackCurrent(&cur->navStack);
        if (current != NULL) browserDisplayPage(b, current);
        return;
    }

    printf("BACK: KEMBALI KE HALAMAN %s\n\n", dest);
    browserDisplayPage(b, dest);
    historyRecord(&b->history, dest);
}

void featureForward(Browser *b, int n) {
    Tab *cur = listGetCurrentTab(&b->tabs);

    if (stackIsEmpty(&cur->navStack)) {
        printf("ERROR: FORWARD TIDAK BISA DIJALANKAN KARENA TIDAK ADA HALAMAN SELANJUTNYA!\n");
        return;
    }

    const char *dest = (n == 1) ? stackForward(&cur->navStack)
                                : stackForwardN(&cur->navStack, n);
    if (dest == NULL) {
        printf("ERROR: FORWARD TIDAK BISA DIJALANKAN KARENA TIDAK ADA HALAMAN SELANJUTNYA!\n");
        const char *current = stackCurrent(&cur->navStack);
        if (current != NULL) browserDisplayPage(b, current);
        return;
    }

    printf("FORWARD: KE HALAMAN %s\n\n", dest);
    browserDisplayPage(b, dest);
    historyRecord(&b->history, dest);
}
