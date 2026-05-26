#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "browser.h"
#include "feature/f01_discover.h"
#include "feature/f02_search.h"
#include "feature/f03_open.h"
#include "feature/f04_caching.h"
#include "feature/f05_page_mgmt.h"
#include "feature/f06_tabs.h"
#include "feature/f07_back_forward.h"
#include "feature/f08_tabs_history.h"
#include "feature/f09_webgraph.h"
#include "feature/f10_download.h"
#include "feature/f11_exit.h"
#include "feature/f_history.h"
#include "feature/f_bookmark.h"

#define INPUT_MAX 512

/* ANSI Colors & Styles */
#define RST   "\033[0m"
#define BLD   "\033[1m"
#define DIM   "\033[2m"
#define UND   "\033[4m"
#define BLK   "\033[30m"
#define RED   "\033[31m"
#define GRN   "\033[32m"
#define YLW   "\033[33m"
#define BLU   "\033[34m"
#define MAG   "\033[35m"
#define CYN   "\033[36m"
#define WHT   "\033[37m"
#define BGBLU "\033[44m"
#define BGCYN "\033[46m"
#define BGGRY "\033[100m"


static void trimNewline(char *s) {
    int len = (int)strlen(s);
    if (len > 0 && s[len - 1] == '\n') s[len - 1] = '\0';
    if (len > 1 && s[len - 2] == '\r') s[len - 2] = '\0';
}

static char* splitCommand(char *input, char *cmd, int cmdSize) {
    int i = 0;
    while (*input == ' ') input++;
    while (*input && *input != ' ' && i < cmdSize - 1) {
        cmd[i++] = *input++;
    }
    cmd[i] = '\0';
    while (*input == ' ') input++;
    return input;
}


static void printBanner(void) {
    printf("\n");
    printf(BLU BLD "  \xe2\x95\x94\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x97\n" RST);
    printf(BLU BLD "  \xe2\x95\x91" RST "                                                  " BLU BLD "\xe2\x95\x91\n" RST);
    printf(BLU BLD "  \xe2\x95\x91" RST CYN BLD "   _   _    _    _   _  ____  ___  ____           " RST BLU BLD "\xe2\x95\x91\n" RST);
    printf(BLU BLD "  \xe2\x95\x91" RST CYN BLD "  | \\ | |  / \\  | \\ | |/ ___|/ _ \\|  _ \\          " RST BLU BLD "\xe2\x95\x91\n" RST);
    printf(BLU BLD "  \xe2\x95\x91" RST CYN BLD "  |  \\| | / _ \\ |  \\| | |  _| | | | |_) |         " RST BLU BLD "\xe2\x95\x91\n" RST);
    printf(BLU BLD "  \xe2\x95\x91" RST CYN BLD "  | |\\  |/ ___ \\| |\\  | |_| | |_| |  _ <          " RST BLU BLD "\xe2\x95\x91\n" RST);
    printf(BLU BLD "  \xe2\x95\x91" RST CYN BLD "  |_| \\_/_/   \\_\\_| \\_|\\____|\\___/|_| \\_\\         " RST BLU BLD "\xe2\x95\x91\n" RST);
    printf(BLU BLD "  \xe2\x95\x91" RST CYN BLD "                    B R O W S E R                 " RST BLU BLD "\xe2\x95\x91\n" RST);
    printf(BLU BLD "  \xe2\x95\x91" RST "                                                  " BLU BLD "\xe2\x95\x91\n" RST);
    printf(BLU BLD "  \xe2\x95\x91" RST DIM "  Terminal Web Browser  \xe2\x94\x82  IF1210 Tubes 2026      " RST BLU BLD "\xe2\x95\x91\n" RST);
    printf(BLU BLD "  \xe2\x95\x91" RST DIM "  Kelompok K04-K        \xe2\x94\x82  STI  \xe2\x94\x82  ITB            " RST BLU BLD "\xe2\x95\x91\n" RST);
    printf(BLU BLD "  \xe2\x95\x91" RST "                                                  " BLU BLD "\xe2\x95\x91\n" RST);
    printf(BLU BLD "  \xe2\x95\x9a\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x9d\n" RST);
    printf("\n");
    printf("  Ketik " YLW BLD "help" RST " untuk melihat daftar perintah.\n\n");
}


static void printHelp(void) {
    printf("\n");
    /* top border ─────────────────────────────────────── */
    printf(BLU "  \xe2\x94\x8c"
        "\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
        "\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
        "\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
        "\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
        "\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
        "\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
        "\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
        "\xe2\x94\x90\n" RST);
    /* title */
    printf(BLU "  \xe2\x94\x82" RST BLD "  NANGOR BROWSER " RST DIM "\xe2\x94\x80 Daftar Perintah                      " RST BLU "\xe2\x94\x82\n" RST);
    /* divider ────────────────────────────────────────── */
    printf(BLU "  \xe2\x94\x9c"
        "\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
        "\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
        "\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
        "\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
        "\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
        "\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
        "\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
        "\xe2\x94\xa4\n" RST);

    /* ── Navigasi ─────────────────────────────────────── */
    printf(BLU "  \xe2\x94\x82" RST "                                                        " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST CYN BLD "  \xe2\x97\x86 Navigasi                                            " RST BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "discover" RST "              " "Tampilkan 5 URL acak            " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "search" RST DIM " <query>" RST "        " "Cari URL dan konten             " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "open" RST DIM " <url>" RST "           " "Buka halaman web                 " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "back" RST DIM " [n]" RST "             " "Kembali n halaman                " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "forward" RST DIM " [n]" RST "          " "Maju n halaman                   " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "openlinked" RST DIM " <n>" RST "       " "Buka linked page ke-n            " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "view_tab_history" RST "      " "Lihat history tab aktif         " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "                                                        " BLU "\xe2\x94\x82\n" RST);

    /* ── Tabs ─────────────────────────────────────────── */
    printf(BLU "  \xe2\x94\x82" RST MAG BLD "  \xe2\x97\x86 Tabs                                                " RST BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "newtab" RST "                " "Buat tab baru                   " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "closetab" RST "              " "Tutup tab aktif                 " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "checktab" RST "              " "Lihat daftar tab                " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "prevtab" RST DIM " <n>" RST "          " "Pindah n tab ke kiri             " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "nexttab" RST DIM " <n>" RST "          " "Pindah n tab ke kanan            " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "                                                        " BLU "\xe2\x94\x82\n" RST);

    /* ── Manajemen Halaman ────────────────────────────── */
    printf(BLU "  \xe2\x94\x82" RST GRN BLD "  \xe2\x97\x86 Manajemen Halaman                                   " RST BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "add_page" RST DIM " <url>" RST "        " "Tambah halaman baru             " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "edit_page" RST DIM " <url>" RST "       " "Edit halaman                    " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "delete_page" RST DIM " <url>" RST "     " "Hapus halaman                   " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "                                                        " BLU "\xe2\x94\x82\n" RST);

    /* ── Download ─────────────────────────────────────── */
    printf(BLU "  \xe2\x94\x82" RST RED BLD "  \xe2\x97\x86 Download                                            " RST BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "download" RST DIM " <url>" RST "        " "Tambah ke antrian download      " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "tick" RST "                  " "Majukan waktu 1 langkah         " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "                                                        " BLU "\xe2\x94\x82\n" RST);

    /* ── Bookmark ─────────────────────────────────────── */
    printf(BLU "  \xe2\x94\x82" RST YLW BLD "  \xe2\x97\x86 Bookmark                                            " RST BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "add_bookmark" RST DIM " <a> <u>" RST "  " "Simpan URL dengan alias         " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "view_bookmark" RST "         " "Lihat daftar bookmark           " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "delete_bookmark" RST DIM " <a>" RST "   " "Hapus bookmark                  " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "open_bookmark" RST DIM " <a>" RST "     " "Buka dari bookmark              " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "                                                        " BLU "\xe2\x94\x82\n" RST);

    /* ── Lainnya ──────────────────────────────────────── */
    printf(BLU "  \xe2\x94\x82" RST WHT BLD "  \xe2\x97\x86 Lainnya                                             " RST BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "history" RST "               " "Lihat riwayat global            " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "help" RST "                  " "Tampilkan bantuan ini           " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "  " YLW "exit" RST "                  " "Keluar dari browser             " BLU "\xe2\x94\x82\n" RST);
    printf(BLU "  \xe2\x94\x82" RST "                                                        " BLU "\xe2\x94\x82\n" RST);

    /* bottom border ──────────────────────────────────── */
    printf(BLU "  \xe2\x94\x94"
        "\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
        "\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
        "\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
        "\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
        "\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
        "\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
        "\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80"
        "\xe2\x94\x98\n" RST);
    printf("\n");
}

static void printPrompt(Browser *b) {
    Tab *cur = listGetCurrentTab(&b->tabs);
    printf(BLU "\xe2\x94\x8c\xe2\x94\x80[" RST BLD "%s" RST BLU "]\n" RST, cur->name);
    printf(BLU "\xe2\x94\x94\xe2\x94\x80\xe2\x96\xb6" RST " ");
    fflush(stdout);
}

/* Separator line */
static void printSep(void) {
    printf(DIM "  \xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80" RST "\n");
}

/* ============================================================
 * Command Parser & Dispatcher
 * ============================================================ */
static int handleCommand(Browser *b, char *input) {
    char cmd[64] = "";
    char *args = splitCommand(input, cmd, sizeof(cmd));

    if (cmd[0] == '\0') return 0;

    if (strcmp(cmd, "discover") == 0) {
        featureDiscover(b);

    } else if (strcmp(cmd, "search") == 0) {
        if (args[0] == '\0') {
            printf("  " RED "\xe2\x9c\x97" RST " Penggunaan: " YLW "search <query>\n" RST);
        } else {
            featureSearch(b, args);
        }

    } else if (strcmp(cmd, "open") == 0) {
        if (args[0] == '\0') {
            printf("  " RED "\xe2\x9c\x97" RST " Penggunaan: " YLW "open <url>\n" RST);
        } else {
            featureOpenPage(b, args);
        }

    } else if (strcmp(cmd, "back") == 0) {
        int n = 1;
        if (args[0] != '\0') n = atoi(args);
        if (n <= 0) n = 1;
        featureBack(b, n);

    } else if (strcmp(cmd, "forward") == 0) {
        int n = 1;
        if (args[0] != '\0') n = atoi(args);
        if (n <= 0) n = 1;
        featureForward(b, n);

    } else if (strcmp(cmd, "openlinked") == 0) {
        if (args[0] == '\0') {
            printf("  " RED "\xe2\x9c\x97" RST " Penggunaan: " YLW "openlinked <nomor>\n" RST);
        } else {
            int idx = atoi(args);
            if (idx <= 0) {
                printf("  " RED "\xe2\x9c\x97" RST " Nomor indeks harus lebih dari 0.\n");
            } else {
                featureOpenLinked(b, idx);
            }
        }

    } else if (strcmp(cmd, "view_tab_history") == 0) {
        featureViewTabHistory(b);

    } else if (strcmp(cmd, "home") == 0) {
        printf("  " GRN "\xe2\x9c\x93" RST " Kamu sudah berada di menu utama.\n");

    } else if (strcmp(cmd, "newtab") == 0) {
        featureNewTab(b);

    } else if (strcmp(cmd, "closetab") == 0) {
        featureCloseTab(b);

    } else if (strcmp(cmd, "checktab") == 0) {
        featureCheckTab(b);

    } else if (strcmp(cmd, "prevtab") == 0) {
        if (args[0] == '\0') {
            printf("  " RED "\xe2\x9c\x97" RST " Penggunaan: " YLW "prevtab <n>\n" RST);
        } else {
            featurePrevTab(b, atoi(args));
        }

    } else if (strcmp(cmd, "nexttab") == 0) {
        if (args[0] == '\0') {
            printf("  " RED "\xe2\x9c\x97" RST " Penggunaan: " YLW "nexttab <n>\n" RST);
        } else {
            featureNextTab(b, atoi(args));
        }

    } else if (strcmp(cmd, "add_page") == 0) {
        if (args[0] == '\0') {
            printf("  " RED "\xe2\x9c\x97" RST " Penggunaan: " YLW "add_page <url>\n" RST);
        } else {
            featureAddPage(b, args);
        }

    } else if (strcmp(cmd, "edit_page") == 0) {
        if (args[0] == '\0') {
            printf("  " RED "\xe2\x9c\x97" RST " Penggunaan: " YLW "edit_page <url>\n" RST);
        } else {
            featureEditPage(b, args);
        }

    } else if (strcmp(cmd, "delete_page") == 0) {
        if (args[0] == '\0') {
            printf("  " RED "\xe2\x9c\x97" RST " Penggunaan: " YLW "delete_page <url>\n" RST);
        } else {
            featureDeletePage(b, args);
        }

    } else if (strcmp(cmd, "download") == 0) {
        if (args[0] == '\0') {
            printf("  " RED "\xe2\x9c\x97" RST " Penggunaan: " YLW "download <url>\n" RST);
        } else {
            featureDownload(b, args);
        }

    } else if (strcmp(cmd, "tick") == 0) {
        featureTick(b);

    } else if (strcmp(cmd, "add_bookmark") == 0) {
        char alias[64] = "";
        char *urlPart = splitCommand(args, alias, sizeof(alias));
        if (alias[0] == '\0' || urlPart[0] == '\0') {
            printf("  " RED "\xe2\x9c\x97" RST " Penggunaan: " YLW "add_bookmark <alias> <url>\n" RST);
        } else {
            featureAddBookmark(b, alias, urlPart);
        }

    } else if (strcmp(cmd, "view_bookmark") == 0) {
        featureViewBookmark(b);

    } else if (strcmp(cmd, "delete_bookmark") == 0) {
        if (args[0] == '\0') {
            printf("  " RED "\xe2\x9c\x97" RST " Penggunaan: " YLW "delete_bookmark <alias>\n" RST);
        } else {
            featureDeleteBookmark(b, args);
        }

    } else if (strcmp(cmd, "open_bookmark") == 0) {
        if (args[0] == '\0') {
            printf("  " RED "\xe2\x9c\x97" RST " Penggunaan: " YLW "open_bookmark <alias>\n" RST);
        } else {
            featureOpenBookmark(b, args);
        }

    } else if (strcmp(cmd, "history") == 0) {
        featureViewHistory(&b->history);

    } else if (strcmp(cmd, "help") == 0) {
        printHelp();

    } else if (strcmp(cmd, "exit") == 0) {
        featureExit(b);
        return 1;

    } else {
        printf("  " RED "\xe2\x9c\x97" RST " Perintah " RED "'%s'" RST " tidak dikenal. Ketik " YLW "help" RST " untuk bantuan.\n", cmd);
    }

    return 0;
}

int main(void) {
    Browser browser;
    char input[INPUT_MAX];

    browserInit(&browser);
    printBanner();

    while (1) {
        printPrompt(&browser);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            featureExit(&browser);
            break;
        }

        trimNewline(input);

        if (handleCommand(&browser, input) == 1) {
            break;
        }

        printSep();
    }

    return 0;
}
