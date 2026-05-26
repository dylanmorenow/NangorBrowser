#include "feature/f04_caching.h"
#include <stdio.h>
#include <string.h>



void browserDisplayPage(Browser *b, const char *url) {
    const char *content = mapGet(&b->cache, url);

    if (content != NULL) {
        printf("[Status: Cache-Hit] Mengambil data dari cache...\n\n");
    } else {
        printf("[Status: Cache-Miss] Mengambil data dari database...\n\n");
        WebPage *page = setSearch(&b->db, url);
        if (page == NULL) {
            printf("404 Not Found! Halaman tidak ditemukan.\n");
            return;
        }
        mapPut(&b->cache, url, page->content);
        content = mapGet(&b->cache, url);
    }

    printf("%s\n\n", content);

    WebPage *page = setSearch(&b->db, url);
    if (page != NULL) {
        graphPrintNeighbors(&b->webGraph, page->id);
    }
    printf("\n");
}
