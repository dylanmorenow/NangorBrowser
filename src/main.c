#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "browser.h"

/* ============================================================
 * main.c — Program utama: main loop + command parser
 *
 * Alur:
 * 1. Init browser (load data, inisialisasi semua ADT)
 * 2. Tampilkan banner
 * 3. Loop: baca input → parse → dispatch ke fungsi fitur
 * ============================================================ */

#define INPUT_MAX 512

/* Warna ANSI (bonus kreativitas) */
#define CYAN    "\033[36m"
#define YELLOW  "\033[33m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define BOLD    "\033[1m"
#define RESET   "\033[0m"

/* ============================================================
 * Helpers parsing
 * ============================================================ */

/* Trim newline di akhir string */
static void trimNewline(char *s) {
    int len = (int)strlen(s);
    if (len > 0 && s[len - 1] == '\n') s[len - 1] = '\0';
    if (len > 1 && s[len - 2] == '\r') s[len - 2] = '\0';
}

/* Ambil token pertama dari input (command), return pointer ke sisa (args) */
static char* splitCommand(char *input, char *cmd, int cmdSize) {
    int i = 0;
    /* Skip leading spaces */
    while (*input == ' ') input++;

    /* Ambil kata pertama sebagai command */
    while (*input && *input != ' ' && i < cmdSize - 1) {
        cmd[i++] = *input++;
    }
    cmd[i] = '\0';

    /* Skip spasi antara command dan args */
    while (*input == ' ') input++;

    return input;  /* Pointer ke args (sisa string) */
}

/* ============================================================
 * Banner
 * ============================================================ */
static void printBanner() {
    printf(CYAN BOLD);
    printf("╔═══════════════════════════════════════════════════╗\n");
    printf("║          NANGOR BROWSER v1.0                      ║\n");
    printf("║    Terminal Web Browser - IF1210 Tubes 2026       ║\n");
    printf("║         Kelompok K04-K  |  STI  |  ITB            ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n");
    printf(RESET);
    printf("Ketik " YELLOW "'help'" RESET " untuk melihat daftar perintah.\n\n");
}

static void printHelp() {
    printf(BOLD "=== DAFTAR PERINTAH ===" RESET "\n");
    printf(CYAN "  Navigasi:" RESET "\n");
    printf("    discover              - Tampilkan 5 URL acak\n");
    printf("    search <query>        - Cari URL berdasarkan prefix\n");
    printf("    open <url>            - Buka halaman web\n");
    printf("    back [n]              - Kembali ke halaman sebelumnya\n");
    printf("    forward [n]           - Maju ke halaman berikutnya\n");
    printf("    openlinked <n>        - Buka linked page ke-n\n");
    printf("    view_tab_history      - Lihat history tab aktif\n");
    printf(CYAN "  Tabs:" RESET "\n");
    printf("    newtab                - Buat tab baru\n");
    printf("    closetab              - Tutup tab aktif\n");
    printf("    checktab              - Lihat daftar tab\n");
    printf("    prevtab <n>           - Pindah n tab ke kiri\n");
    printf("    nexttab <n>           - Pindah n tab ke kanan\n");
    printf(CYAN "  Manajemen Halaman:" RESET "\n");
    printf("    add_page <url>        - Tambah halaman baru\n");
    printf("    edit_page <url>       - Edit halaman\n");
    printf("    delete_page <url>     - Hapus halaman\n");
    printf(CYAN "  Download:" RESET "\n");
    printf("    download <url>        - Tambah ke antrian download\n");
    printf("    tick                  - Majukan waktu 1 langkah\n");
    printf(CYAN "  Lain-lain:" RESET "\n");
    printf("    help                  - Tampilkan bantuan ini\n");
    printf("    exit                  - Keluar dari browser\n");
    printf("\n");
}

/* ============================================================
 * Prompt: tampilkan [TABx] >>> 
 * ============================================================ */
static void printPrompt(Browser *b) {
    Tab *cur = listGetCurrentTab(&b->tabs);
    printf(GREEN "[%s]" RESET " >>> ", cur->name);
    fflush(stdout);
}

/* ============================================================
 * Command Parser & Dispatcher
 * Return 0 untuk lanjut, 1 untuk exit
 * ============================================================ */
static int handleCommand(Browser *b, char *input) {
    char cmd[64] = "";
    char *args = splitCommand(input, cmd, sizeof(cmd));

    /* Kosong / hanya enter */
    if (cmd[0] == '\0') return 0;

    /* ---- NAVIGASI ---- */
    if (strcmp(cmd, "discover") == 0) {
        featureDiscover(b);

    } else if (strcmp(cmd, "search") == 0) {
        if (args[0] == '\0') {
            printf("Penggunaan: search <query>\n");
        } else {
            featureSearch(b, args);
        }

    } else if (strcmp(cmd, "open") == 0) {
        if (args[0] == '\0') {
            printf("Penggunaan: open <url>\n");
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
            printf("Penggunaan: openlinked <nomor>\n");
        } else {
            int idx = atoi(args);
            if (idx <= 0) {
                printf("ERROR: Nomor indeks harus lebih dari 0.\n");
            } else {
                featureOpenLinked(b, idx);
            }
        }

    } else if (strcmp(cmd, "view_tab_history") == 0) {
        featureViewTabHistory(b);

    } else if (strcmp(cmd, "home") == 0) {
        /* Kembali ke menu utama — di sini sudah di menu utama */
        printf("Kamu sudah berada di menu utama.\n");

    /* ---- TABS ---- */
    } else if (strcmp(cmd, "newtab") == 0) {
        featureNewTab(b);

    } else if (strcmp(cmd, "closetab") == 0) {
        featureCloseTab(b);

    } else if (strcmp(cmd, "checktab") == 0) {
        featureCheckTab(b);

    } else if (strcmp(cmd, "prevtab") == 0) {
        if (args[0] == '\0') {
            printf("Penggunaan: prevtab <n>\n");
        } else {
            int n = atoi(args);
            featurePrevTab(b, n);
        }

    } else if (strcmp(cmd, "nexttab") == 0) {
        if (args[0] == '\0') {
            printf("Penggunaan: nexttab <n>\n");
        } else {
            int n = atoi(args);
            featureNextTab(b, n);
        }

    /* ---- PAGE MANAGEMENT ---- */
    } else if (strcmp(cmd, "add_page") == 0) {
        if (args[0] == '\0') {
            printf("Penggunaan: add_page <url>\n");
        } else {
            featureAddPage(b, args);
        }

    } else if (strcmp(cmd, "edit_page") == 0) {
        if (args[0] == '\0') {
            printf("Penggunaan: edit_page <url>\n");
        } else {
            featureEditPage(b, args);
        }

    } else if (strcmp(cmd, "delete_page") == 0) {
        if (args[0] == '\0') {
            printf("Penggunaan: delete_page <url>\n");
        } else {
            featureDeletePage(b, args);
        }

    /* ---- DOWNLOAD ---- */
    } else if (strcmp(cmd, "download") == 0) {
        if (args[0] == '\0') {
            printf("Penggunaan: download <url>\n");
        } else {
            featureDownload(b, args);
        }

    } else if (strcmp(cmd, "tick") == 0) {
        featureTick(b);

    /* ---- LAIN-LAIN ---- */
    } else if (strcmp(cmd, "help") == 0) {
        printHelp();

    } else if (strcmp(cmd, "exit") == 0) {
        featureExit(b);
        return 1;  /* Signal untuk keluar dari main loop */

    } else {
        printf(RED "Perintah '%s' tidak dikenal." RESET
               " Ketik 'help' untuk bantuan.\n", cmd);
    }

    return 0;
}

/* ============================================================
 * Main
 * ============================================================ */
int main(void) {
    Browser browser;
    char input[INPUT_MAX];

    /* Init semua komponen */
    browserInit(&browser);

    /* Tampilkan banner */
    printBanner();

    /* Main loop */
    while (1) {
        printPrompt(&browser);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            /* EOF (Ctrl+D) → exit */
            printf("\n");
            featureExit(&browser);
            break;
        }

        trimNewline(input);

        if (handleCommand(&browser, input) == 1) {
            break;
        }

        printf("\n");
    }

    return 0;
}