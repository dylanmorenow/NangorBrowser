#ifndef CONFIG_H
#define CONFIG_H


/* --- Kapasitas Sistem --- */
#define CACHE_MAX_AMOUNT    10      /* Max URL yang bisa di-cache */
#define TABS_MAX_AMOUNT     10      /* Max tab yang bisa dibuka */
#define DOWNLOAD_MAX_AMOUNT 5       /* Max item di antrian download */
#define MAX_WEB_PAGES       100     /* Max total halaman di database */
#define HISTORY_MAX_AMOUNT  50      /* Max entri di global history */

/* --- Ukuran String --- */
#define MAX_URL_LENGTH      256     /* Panjang max sebuah URL */
#define MAX_CONTENT_LENGTH  4096    /* Panjang max konten halaman */

#define RNG_A       1103515245
#define RNG_C       12345
#define RNG_M       2147483648U   /* 2^31 */
#define RNG_SEED    73939133

typedef int bool;
#define TRUE  1
#define FALSE 0


typedef struct {
    int  id;
    char url[MAX_URL_LENGTH];
    char content[MAX_CONTENT_LENGTH];
} WebPage;

#endif 