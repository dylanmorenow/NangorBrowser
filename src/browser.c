#include "browser.h"
#include "data.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ============================================================
 * browser.c — Implementasi semua fitur browser
 * ============================================================ */

/* --- LCG --- */
unsigned long browserLCGNext(Browser *b) {
    b->lcgSeed = (LCG_MULTIPLIER * b->lcgSeed + LCG_INCREMENT) % LCG_MODULUS;
    return b->lcgSeed;
}

/* --- Init --- */
void browserInit(Browser *b) {
    setInit(&b->db);
    mapInit(&b->cache);
    graphInit(&b->webGraph);
    listInit(&b->tabs);
    queueInit(&b->dlQueue);
    b->lcgSeed = LCG_SEED_INITIAL;
    dataInit(&b->db, &b->webGraph);
}

/* ============================================================
 * Helper: tampilkan konten halaman + linked pages
 * Dipanggil oleh open, back, forward, openlinked
 * ============================================================ */
void browserDisplayPage(Browser *b, const char *url) {
    /* Cek cache dulu */
    const char *content = mapGet(&b->cache, url);

    if (content != NULL) {
        printf("[Status: Cache-Hit] Mengambil data dari cache...\n\n");
    } else {
        /* Cache miss: cari di database */
        printf("[Status: Cache-Miss] Mengambil data dari database...\n\n");
        WebPage *page = setSearch(&b->db, url);
        if (page == NULL) {
            printf("404 Not Found! Halaman tidak ditemukan.\n");
            return;
        }
        /* Simpan ke cache */
        mapPut(&b->cache, url, page->content);
        content = mapGet(&b->cache, url);
    }

    /* Tampilkan konten */
    printf("%s\n\n", content);

    /* Tampilkan linked pages dari web graph */
    WebPage *page = setSearch(&b->db, url);
    if (page != NULL) {
        graphPrintNeighbors(&b->webGraph, page->id);
    }
    printf("\n");
}

/* ============================================================
 * F01 - Discover
 * ============================================================ */
void featureDiscover(Browser *b) {
    if (b->db.count == 0) {
        printf("Database kosong, tidak ada halaman untuk ditampilkan.\n");
        return;
    }

    int total   = b->db.count;
    int tampil  = (total < 5) ? total : 5;

    /* Tandai indeks yang sudah dipilih supaya tidak duplikat */
    int chosen[5];
    int chosenCount = 0;
    int i, j;

    printf("Berikut adalah beberapa halaman yang mungkin menarik untukmu:\n");

    while (chosenCount < tampil) {
        int idx = (int)(browserLCGNext(b) % (unsigned long)total);

        /* Cek duplikat */
        bool duplicate = FALSE;
        for (j = 0; j < chosenCount; j++) {
            if (chosen[j] == idx) { duplicate = TRUE; break; }
        }
        if (duplicate) continue;

        chosen[chosenCount++] = idx;
        printf("  - %s\n", b->db.pages[idx].url);
    }

    /* Kalau total < 5, info-kan */
    if (total < 5) {
        printf("(Hanya %d halaman tersedia di database)\n", total);
    }
    (void)i;
}

/* ============================================================
 * F02 - Search
 * ============================================================ */
void featureSearch(Browser *b, const char *query) {
    WebPage *results[MAX_WEB_PAGES];
    int n = setSearchPrefix(&b->db, query, results, MAX_WEB_PAGES);

    printf("Search result(s) untuk \"%s\":\n", query);
    if (n == 0) {
        printf("  Tidak Ditemukan\n");
        return;
    }
    int i;
    for (i = 0; i < n; i++) {
        printf("  - %s\n", results[i]->url);
    }
}

/* ============================================================
 * F03 - Open Page
 * ============================================================ */
void featureOpenPage(Browser *b, const char *url) {
    /* Cek dulu apakah halaman ada */
    WebPage *page = setSearch(&b->db, url);
    if (page == NULL) {
        const char *cached = mapGet(&b->cache, url);
        if (cached == NULL) {
            printf("404 Not Found! Halaman tidak ditemukan.\n");
            return;
        }
    }

    /* Push ke stack navigasi tab aktif */
    Tab *cur = listGetCurrentTab(&b->tabs);
    stackPush(&cur->navStack, url);

    /* Tampilkan halaman */
    browserDisplayPage(b, url);
}

/* ============================================================
 * F05 - Page Management
 * ============================================================ */
void featureAddPage(Browser *b, const char *url) {
    /* Cek duplikat */
    if (setSearch(&b->db, url) != NULL) {
        printf("Sudah terdapat halaman dengan url %s. "
               "Gunakan url lain yang belum terdaftar!\n", url);
        return;
    }

    /* Input konten */
    char content[MAX_CONTENT_LENGTH] = "";
    char line[512];
    printf("Masukkan konten (Akhiri dengan '.' di baris baru):\n");

    while (1) {
        printf(">>> ");
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        /* Hapus newline di akhir */
        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, ".") == 0) break;

        /* Tambah ke content dengan \n pemisah */
        if (strlen(content) > 0)
            strncat(content, "\n", MAX_CONTENT_LENGTH - strlen(content) - 1);
        strncat(content, line, MAX_CONTENT_LENGTH - strlen(content) - 1);
    }

    /* Insert ke database */
    if (!setInsert(&b->db, url, content)) {
        printf("Gagal menambahkan halaman.\n");
        return;
    }

    /* Tambah node ke graph */
    WebPage *newPage = setSearch(&b->db, url);
    if (newPage != NULL) graphAddNode(&b->webGraph, newPage->id);

    /* Input linked pages */
    printf("Masukkan linked pages (Ketik 'DONE' jika sudah selesai):\n");
    while (1) {
        printf(">>> ");
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, "DONE") == 0) break;

        WebPage *target = setSearch(&b->db, line);
        if (target == NULL) {
            printf("URL tidak ditemukan!\n");
        } else {
            graphAddEdge(&b->webGraph, newPage->id, target->id, line);
        }
    }

    printf("Halaman %s berhasil ditambahkan!\n", url);
}

void featureEditPage(Browser *b, const char *url) {
    WebPage *page = setSearch(&b->db, url);
    if (page == NULL) {
        printf("Tidak ada halaman dengan url %s!\n", url);
        return;
    }

    /* Cek cache */
    if (mapGet(&b->cache, url) != NULL) {
        printf("[Status: Cache-Hit] Mengambil data dari cache...\n");
    } else {
        printf("[Status: Cache-Miss] Mengambil data dari database...\n");
    }

    /* Tampilkan konten saat ini */
    printf("Konten saat ini:\n%s\n\n", page->content);

    /* Tampilkan linked pages saat ini */
    printf("Linked pages saat ini:\n");
    graphPrintNeighbors(&b->webGraph, page->id);

    /* Input konten baru */
    char newContent[MAX_CONTENT_LENGTH] = "";
    char line[512];
    printf("\nMasukkan konten baru (akhiri dengan '.' atau ketik '.' saja jika tidak ingin mengubah):\n");

    bool contentChanged = FALSE;
    while (1) {
        printf(">>> ");
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, ".") == 0) break;
        contentChanged = TRUE;

        if (strlen(newContent) > 0)
            strncat(newContent, "\n", MAX_CONTENT_LENGTH - strlen(newContent) - 1);
        strncat(newContent, line, MAX_CONTENT_LENGTH - strlen(newContent) - 1);
    }

    if (contentChanged) {
        setUpdate(&b->db, url, newContent);
        mapRemove(&b->cache, url);  /* Invalidate cache */
        mapPut(&b->cache, url, newContent);
    }

    /* Input linked pages baru */
    printf("Masukkan linked pages baru (Ketik 'DONE' jika selesai, 'SKIP' untuk tidak ubah):\n");
    printf(">>> ");
    if (fgets(line, sizeof(line), stdin) != NULL) {
        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, "SKIP") != 0) {
            /* Reset linked pages lama */
            LinkedList *neighbors = graphGetNeighbors(&b->webGraph, page->id);
            if (neighbors != NULL) llClear(neighbors);

            /* Tambah yang baru */
            while (strcmp(line, "DONE") != 0) {
                WebPage *target = setSearch(&b->db, line);
                if (target == NULL) {
                    printf("URL tidak ditemukan!\n");
                } else {
                    graphAddEdge(&b->webGraph, page->id, target->id, line);
                }
                printf(">>> ");
                if (fgets(line, sizeof(line), stdin) == NULL) break;
                line[strcspn(line, "\n")] = '\0';
            }
        }
    }

    printf("Halaman %s berhasil diperbarui!\n", url);
}

void featureDeletePage(Browser *b, const char *url) {
    WebPage *page = setSearch(&b->db, url);
    if (page == NULL) {
        printf("Tidak ada halaman dengan url %s!\n", url);
        return;
    }

    int pageId = page->id;

    /* Hapus dari cache */
    if (mapRemove(&b->cache, url)) {
        printf("[Status: Cache-Hit] URL ditemukan di cache dan telah dibersihkan.\n");
    }

    /* Hapus dari graph (node + semua edge terkait) */
    printf("Membersihkan relasi linked pages...\n");
    graphRemoveNode(&b->webGraph, pageId);

    /* Hapus dari database */
    setDelete(&b->db, url);

    printf("Halaman %s berhasil dihapus!\n", url);
}

/* ============================================================
 * F06 - Tabs
 * ============================================================ */
void featureNewTab(Browser *b) {
    if (listIsFull(&b->tabs)) {
        printf("ERROR: Jumlah tab tidak bisa melebihi batas maksimum!\n");
        return;
    }
    listAddTab(&b->tabs);
}

void featureCloseTab(Browser *b) {
    if (b->tabs.count <= 1) {
        printf("ERROR: Tidak bisa menutup tab, tab minimal berjumlah 1!\n");
        return;
    }
    Tab *cur = listGetCurrentTab(&b->tabs);
    printf("%s berhasil ditutup.\n", cur->name);
    listCloseCurrentTab(&b->tabs);
}

void featureCheckTab(Browser *b) {
    listPrintTabs(&b->tabs);
}

void featurePrevTab(Browser *b, int n) {
    Tab *before = listGetCurrentTab(&b->tabs);
    if (!listPrevTab(&b->tabs, n)) {
        printf("ERROR: Posisi tab tidak valid!\n");
        return;
    }
    Tab *after = listGetCurrentTab(&b->tabs);
    printf("Tab saat ini berhasil diganti ke %s.\n", after->name);
    (void)before;
}

void featureNextTab(Browser *b, int n) {
    if (!listNextTab(&b->tabs, n)) {
        printf("ERROR: Posisi tab tidak valid!\n");
        return;
    }
    Tab *after = listGetCurrentTab(&b->tabs);
    printf("Tab saat ini berhasil diganti ke %s.\n", after->name);
}

/* ============================================================
 * F07/F08 - Back / Forward
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
        /* Tampilkan halaman saat ini */
        const char *current = stackCurrent(&cur->navStack);
        if (current != NULL) browserDisplayPage(b, current);
        return;
    }

    printf("BACK: KEMBALI KE HALAMAN %s\n\n", dest);
    browserDisplayPage(b, dest);
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
}

void featureViewTabHistory(Browser *b) {
    Tab *cur = listGetCurrentTab(&b->tabs);
    printf("Tab History [%s]:\n", cur->name);
    stackPrintHistory(&cur->navStack);

    /* Tampilkan halaman saat ini */
    const char *current = stackCurrent(&cur->navStack);
    if (current != NULL) {
        printf("\n");
        browserDisplayPage(b, current);
    }
}

/* ============================================================
 * F09 - openlinked
 * ============================================================ */
void featureOpenLinked(Browser *b, int index) {
    Tab *cur = listGetCurrentTab(&b->tabs);

    /* Harus ada halaman yang sedang dibuka */
    if (stackIsEmpty(&cur->navStack)) {
        printf("ERROR: COMMAND HANYA DAPAT DIGUNAKAN SAAT HALAMAN WEB TERBUKA!\n");
        return;
    }

    const char *currentUrl = stackCurrent(&cur->navStack);
    WebPage *page = setSearch(&b->db, currentUrl);
    if (page == NULL) {
        printf("ERROR: Halaman tidak ditemukan di database.\n");
        return;
    }

    LinkedList *neighbors = graphGetNeighbors(&b->webGraph, page->id);
    if (neighbors == NULL || llIsEmpty(neighbors)) {
        printf("ERROR: HALAMAN TIDAK MEMILIKI TAUTAN YANG BISA DIBUKA!\n");
        return;
    }

    /* Index dari user adalah 1-based */
    const char *targetUrl = llGetByIndex(neighbors, index - 1);
    if (targetUrl == NULL) {
        printf("ERROR: Indeks %d tidak valid! (tersedia 1-%d)\n",
               index, neighbors->size);
        return;
    }

    /* Push ke stack dan tampilkan */
    stackPush(&cur->navStack, targetUrl);
    printf("Membuka: %s\n\n", targetUrl);
    browserDisplayPage(b, targetUrl);
}

/* ============================================================
 * F10 - Download Manager
 * ============================================================ */
void featureDownload(Browser *b, const char *url) {
    if (queueIsFull(&b->dlQueue)) {
        printf("Download tidak diterima, antrian sudah penuh.\n");
        return;
    }

    int ticks = queueCalcTicks(url);
    queueEnqueue(&b->dlQueue, url);

    if (b->dlQueue.count == 1) {
        printf("Download %s (%d ticks)\n", url, ticks);
    } else {
        printf("Download %s (%d ticks) -> antrian no %d\n",
               url, ticks, b->dlQueue.count);
    }
}

void featureTick(Browser *b) {
    queueTick(&b->dlQueue);
}

/* ============================================================
 * F11 - Exit
 * ============================================================ */
void featureExit(Browser *b) {
    printf("\nTerima kasih telah menggunakan NangorBrowser!\n");
    printf("Sampai jumpa di Nangor Falls...\n");
    printf("(Semoga kamu tidak ketemu Bill Cipher di jalan)\n");
    (void)b;
}