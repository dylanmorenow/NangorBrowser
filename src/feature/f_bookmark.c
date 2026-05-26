#include "feature/f_bookmark.h"
#include "feature/f04_caching.h"
#include "feature/f_history.h"
#include <stdio.h>
#include <string.h>


void bookmarkInit(BookmarkManager *bm) {
    int i;
    for (i = 0; i < BOOKMARK_MAX; i++) {
        bm->entries[i].isOccupied = 0;
    }
    bm->count = 0;
}

/* Helper: cari bookmark berdasarkan alias */
static int findByAlias(BookmarkManager *bm, const char *alias) {
    int i;
    for (i = 0; i < BOOKMARK_MAX; i++) {
        if (bm->entries[i].isOccupied &&
            strcmp(bm->entries[i].alias, alias) == 0) {
            return i;
        }
    }
    return -1;
}

/* Helper: cari slot kosong */
static int findEmptySlot(BookmarkManager *bm) {
    int i;
    for (i = 0; i < BOOKMARK_MAX; i++) {
        if (!bm->entries[i].isOccupied) return i;
    }
    return -1;
}

void featureAddBookmark(Browser *b, const char *alias, const char *url) {
    BookmarkManager *bm = &b->bookmarks;

    /* Cek alias sudah dipakai */
    if (findByAlias(bm, alias) != -1) {
        printf("ERROR: Alias '%s' sudah digunakan!\n", alias);
        return;
    }

    /* Cek URL ada di database */
    if (setSearch(&b->db, url) == NULL) {
        printf("ERROR: URL '%s' tidak ditemukan di database!\n", url);
        return;
    }

    /* Cek kapasitas */
    if (bm->count >= BOOKMARK_MAX) {
        printf("ERROR: Bookmark sudah penuh!\n");
        return;
    }

    int slot = findEmptySlot(bm);
    if (slot == -1) return;

    strncpy(bm->entries[slot].alias, alias, 63);
    bm->entries[slot].alias[63] = '\0';
    strncpy(bm->entries[slot].url, url, MAX_URL_LENGTH - 1);
    bm->entries[slot].url[MAX_URL_LENGTH - 1] = '\0';
    bm->entries[slot].isOccupied = 1;
    bm->count++;

    printf("Bookmark '%s' -> %s berhasil ditambahkan!\n", alias, url);
}

void featureViewBookmark(Browser *b) {
    BookmarkManager *bm = &b->bookmarks;

    if (bm->count == 0) {
        printf("Belum ada bookmark yang tersimpan.\n");
        return;
    }

    printf("=== Daftar Bookmark ===\n");
    int i, num = 1;
    for (i = 0; i < BOOKMARK_MAX; i++) {
        if (bm->entries[i].isOccupied) {
            printf("  [%d] %-20s -> %s\n", num++,
                   bm->entries[i].alias, bm->entries[i].url);
        }
    }
    printf("Total: %d bookmark\n", bm->count);
}

void featureDeleteBookmark(Browser *b, const char *alias) {
    BookmarkManager *bm = &b->bookmarks;
    int idx = findByAlias(bm, alias);
    if (idx == -1) {
        printf("ERROR: Bookmark dengan alias '%s' tidak ditemukan!\n", alias);
        return;
    }

    printf("Bookmark '%s' -> %s berhasil dihapus!\n",
           bm->entries[idx].alias, bm->entries[idx].url);
    bm->entries[idx].isOccupied = 0;
    bm->count--;
}

void featureOpenBookmark(Browser *b, const char *alias) {
    BookmarkManager *bm = &b->bookmarks;
    int idx = findByAlias(bm, alias);
    if (idx == -1) {
        printf("ERROR: Bookmark dengan alias '%s' tidak ditemukan!\n", alias);
        return;
    }

    const char *url = bm->entries[idx].url;

    /* Cek URL masih ada di database */
    WebPage *page = setSearch(&b->db, url);
    if (page == NULL) {
        printf("ERROR: URL '%s' sudah tidak ada di database!\n", url);
        return;
    }

    /* Push ke navStack dan tampilkan */
    Tab *cur = listGetCurrentTab(&b->tabs);
    stackPush(&cur->navStack, url);
    printf("Membuka bookmark '%s': %s\n\n", alias, url);
    browserDisplayPage(b, url);

    /* Record ke history (B02) */
    historyRecord(&b->history, url);
}
