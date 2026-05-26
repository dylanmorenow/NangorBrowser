#include "feature/f03_open.h"
#include "feature/f04_caching.h"
#include "feature/f_history.h"
#include <stdio.h>
#include <string.h>



void featureOpenPage(Browser *b, const char *url) {
    /* Cek apakah halaman ada di database */
    WebPage *page = setSearch(&b->db, url);
    if (page == NULL) {
        printf("404 Not Found! Halaman tidak ditemukan.\n");
        return;
    }

    /* Push ke navigation stack tab aktif */
    Tab *cur = listGetCurrentTab(&b->tabs);
    stackPush(&cur->navStack, url);

    /* Tampilkan halaman */
    browserDisplayPage(b, url);

    /* B02: Record ke global history */
    historyRecord(&b->history, url);
}
