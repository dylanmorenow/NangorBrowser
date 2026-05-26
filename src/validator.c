#include "validator.h"
#include <string.h>


/* Helper: cek apakah karakter adalah huruf */
static int isAlpha(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

/* Helper: cek apakah karakter adalah digit */
static int isDigit(char c) {
    return (c >= '0' && c <= '9');
}

/* Helper: cek apakah karakter valid untuk domain label */
static int isDomainChar(char c) {
    return isAlpha(c) || isDigit(c) || c == '-';
}

bool validateURL(const char *url) {
    if (url == NULL || url[0] == '\0') return FALSE;

    int len = (int)strlen(url);
    if (len > MAX_URL_LENGTH - 1) return FALSE;

    /* Harus ada minimal satu titik */
    const char *firstDot = NULL;
    int i;
    for (i = 0; i < len; i++) {
        if (url[i] == '.') {
            firstDot = &url[i];
            break;
        }
    }
    if (firstDot == NULL) return FALSE;

    /* Parse domain label (bagian sebelum titik pertama) */
    int labelStart = 0;
    int labelEnd = (int)(firstDot - url) - 1;
    int labelLen = labelEnd - labelStart + 1;

    /* Domain label: 1-63 karakter */
    if (labelLen < 1 || labelLen > 63) return FALSE;

    /* Tidak boleh diawali dengan '-' */
    if (url[labelStart] == '-') return FALSE;

    /* Tidak boleh diakhiri dengan '-' */
    if (url[labelEnd] == '-') return FALSE;

    /* Semua karakter harus [A-Za-z0-9-] */
    for (i = labelStart; i <= labelEnd; i++) {
        if (!isDomainChar(url[i])) return FALSE;
    }

    /* Parse extension(s): satu atau lebih ".xxx" */
    int pos = labelEnd + 1;  /* Posisi titik pertama */
    int extensionCount = 0;

    while (pos < len) {
        /* Harus dimulai dengan '.' */
        if (url[pos] != '.') return FALSE;
        pos++;  /* Skip titik */

        /* Baca extension label */
        int extStart = pos;
        while (pos < len && url[pos] != '.') {
            pos++;
        }
        int extLen = pos - extStart;

        /* Extension minimal 2 karakter, semua harus huruf */
        if (extLen < 2) return FALSE;

        int j;
        for (j = extStart; j < pos; j++) {
            if (!isAlpha(url[j])) return FALSE;
        }

        extensionCount++;
    }

    /* Harus ada minimal 1 extension */
    if (extensionCount < 1) return FALSE;

    return TRUE;
}

bool validatePositiveInt(const char *str, int *result) {
    if (str == NULL || str[0] == '\0') return FALSE;

    int len = (int)strlen(str);

    /* Max 6 digit */
    if (len > 6) return FALSE;

    /* Semua harus digit */
    int i;
    for (i = 0; i < len; i++) {
        if (!isDigit(str[i])) return FALSE;
    }

    /* Hitung nilai */
    int val = 0;
    for (i = 0; i < len; i++) {
        val = val * 10 + (str[i] - '0');
    }

    /* Harus 1-999999 */
    if (val < 1 || val > 999999) return FALSE;

    *result = val;
    return TRUE;
}
