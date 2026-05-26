#include "feature/f05_page_mgmt.h"
#include "feature/f04_caching.h"
#include "validator.h"
#include <stdio.h>
#include <string.h>


#define INPUT_LINE_MAX 512

/* Helper: baca konten multi-line, akhiri dengan "." di baris sendiri.
 * Return panjang konten yang ditulis. */
static int readMultilineContent(char *buf, int maxLen) {
    char line[INPUT_LINE_MAX];
    int  len = 0;
    buf[0] = '\0';

    while (1) {
        printf(">>> ");
        fflush(stdout);
        if (fgets(line, sizeof(line), stdin) == NULL) break;

        /* Hapus trailing newline */
        int lineLen = (int)strlen(line);
        if (lineLen > 0 && line[lineLen - 1] == '\n') {
            line[--lineLen] = '\0';
        }

        /* Cek terminator: baris yang isinya hanya "." */
        if (strcmp(line, ".") == 0) break;

        /* Tambahkan ke buffer */
        if (len > 0 && len < maxLen - 1) {
            buf[len++] = '\n';
        }
        int toCopy = lineLen;
        if (len + toCopy > maxLen - 1) {
            toCopy = maxLen - 1 - len;
        }
        if (toCopy > 0) {
            strncpy(buf + len, line, toCopy);
            len += toCopy;
        }
        buf[len] = '\0';
    }

    return len;
}


void featureAddPage(Browser *b, const char *url) {
    /* Validasi URL format */
    if (!validateURL(url)) {
        printf("URL tidak valid! Format URL harus sesuai.\n");
        return;
    }

    /* Cek apakah URL sudah ada */
    if (setSearch(&b->db, url) != NULL) {
        printf("Sudah terdapat halaman dengan url %s. "
               "Gunakan url lain yang belum terdaftar!\n", url);
        return;
    }

    /* Cek kapasitas database */
    if (setIsFull(&b->db)) {
        printf("ERROR: Database penuh, tidak bisa menambah halaman baru.\n");
        return;
    }

    /* Baca konten */
    printf("Masukkan konten (Akhiri dengan titik '.' di baris baru):\n");
    char content[MAX_CONTENT_LENGTH];
    readMultilineContent(content, MAX_CONTENT_LENGTH);

    if (strlen(content) == 0) {
        printf("ERROR: Konten tidak boleh kosong.\n");
        return;
    }

    /* Insert ke database */
    setInsert(&b->db, url, content);
    WebPage *newPage = setSearch(&b->db, url);
    if (newPage == NULL) {
        printf("ERROR: Gagal menambahkan halaman.\n");
        return;
    }

    /* Tambah node ke graph */
    graphAddNode(&b->webGraph, newPage->id);

    /* Baca linked pages */
    printf("Masukkan linked pages (Ketik 'DONE' jika sudah selesai):\n");
    char linkedUrl[MAX_URL_LENGTH];
    while (1) {
        printf(">>> ");
        fflush(stdout);
        if (fgets(linkedUrl, sizeof(linkedUrl), stdin) == NULL) break;

        /* Hapus newline */
        int len = (int)strlen(linkedUrl);
        if (len > 0 && linkedUrl[len - 1] == '\n') linkedUrl[len - 1] = '\0';

        if (strcmp(linkedUrl, "DONE") == 0) break;

        /* Cek apakah target URL ada di database */
        WebPage *target = setSearch(&b->db, linkedUrl);
        if (target == NULL) {
            printf("URL tidak ditemukan!\n");
            continue;
        }

        /* Tambah edge */
        graphAddEdge(&b->webGraph, newPage->id, target->id, linkedUrl);
    }

    printf("Halaman %s berhasil ditambahkan!\n", url);
}


void featureEditPage(Browser *b, const char *url) {
    WebPage *page = setSearch(&b->db, url);
    if (page == NULL) {
        printf("Tidak ada halaman dengan url %s!\n", url);
        return;
    }

    /* Tampilkan konten saat ini via browserDisplayPage */
    browserDisplayPage(b, url);

    /* Baca konten baru */
    printf("Masukkan konten baru (akhiri dengan '.' atau ketik '.' "
           "saja jika tidak ingin mengubah konten):\n");
    char newContent[MAX_CONTENT_LENGTH];
    int contentLen = readMultilineContent(newContent, MAX_CONTENT_LENGTH);

    if (contentLen > 0) {
        setUpdate(&b->db, url, newContent);
        /* Update cache jika ada */
        if (mapGet(&b->cache, url) != NULL) {
            mapPut(&b->cache, url, newContent);
        }
    }

    /* Baca linked pages baru */
    printf("Masukkan linked pages baru (Ketik 'DONE' jika sudah selesai, "
           "atau ketik 'SKIP' jika tidak ingin mengubah linked pages):\n");
    char linkedUrl[MAX_URL_LENGTH];
    int firstInput = 1;

    while (1) {
        printf(">>> ");
        fflush(stdout);
        if (fgets(linkedUrl, sizeof(linkedUrl), stdin) == NULL) break;

        int len = (int)strlen(linkedUrl);
        if (len > 0 && linkedUrl[len - 1] == '\n') linkedUrl[len - 1] = '\0';

        if (strcmp(linkedUrl, "SKIP") == 0) break;
        if (strcmp(linkedUrl, "DONE") == 0) break;

        /* Pada input pertama, hapus semua edge lama */
        if (firstInput) {
            /* Hapus semua edge keluar dari node ini */
            LinkedList *neighbors = graphGetNeighbors(&b->webGraph, page->id);
            if (neighbors != NULL) {
                while (!llIsEmpty(neighbors)) {
                    int targetId = llGetIdByIndex(neighbors, 0);
                    graphRemoveEdge(&b->webGraph, page->id, targetId);
                }
            }
            firstInput = 0;
        }

        /* Cek apakah target ada */
        WebPage *target = setSearch(&b->db, linkedUrl);
        if (target == NULL) {
            printf("URL tidak ditemukan!\n");
            continue;
        }

        graphAddEdge(&b->webGraph, page->id, target->id, linkedUrl);
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

    /* Hapus dari cache jika ada */
    if (mapGet(&b->cache, url) != NULL) {
        mapRemove(&b->cache, url);
        printf("[Status: Cache-Hit] URL ditemukan di cache dan telah dibersihkan.\n");
    }

    /* Hapus dari graph (node + semua edge masuk/keluar) */
    printf("Membersihkan relasi linked pages...\n");
    graphRemoveNode(&b->webGraph, pageId);

    /* Hapus dari database */
    setDelete(&b->db, url);

    printf("Halaman %s berhasil dihapus!\n", url);
}
