#include "feature/f02_search.h"
#include <stdio.h>
#include <string.h>

/* ============================================================
 * f02_search.c — F02 Search + B03 Advanced Search
 *
 * F02 dasar: prefix match pada URL
 * B03 bonus: case-insensitive substring match pada URL DAN konten
 *
 * Implementasi: B03 menggantikan F02 (superset).
 * ============================================================ */

/* Helper: konversi karakter ke lowercase */
static char toLower(char c) {
    if (c >= 'A' && c <= 'Z') return (char)(c + ('a' - 'A'));
    return c;
}

/* Helper: case-insensitive substring search (string matching).
 * Return 1 jika needle ditemukan di dalam haystack. */
static int containsSubstringCI(const char *haystack, const char *needle) {
    int hLen = (int)strlen(haystack);
    int nLen = (int)strlen(needle);

    if (nLen == 0) return 1;
    if (nLen > hLen) return 0;

    int i, j;
    for (i = 0; i <= hLen - nLen; i++) {
        int match = 1;
        for (j = 0; j < nLen; j++) {
            if (toLower(haystack[i + j]) != toLower(needle[j])) {
                match = 0;
                break;
            }
        }
        if (match) return 1;
    }
    return 0;
}

void featureSearch(Browser *b, const char *query) {
    printf("Search result(s) for \"%s\":\n", query);

    int found = 0;
    int i;
    for (i = 0; i < b->db.count; i++) {
        /* Cek case-insensitive substring di URL */
        if (containsSubstringCI(b->db.pages[i].url, query)) {
            printf("  - %s\n", b->db.pages[i].url);
            found++;
            continue;
        }
        /* Cek case-insensitive substring di konten (B03) */
        if (containsSubstringCI(b->db.pages[i].content, query)) {
            printf("  - %s\n", b->db.pages[i].url);
            found++;
        }
    }

    if (found == 0) {
        printf("  Tidak Ditemukan\n");
    }
}
