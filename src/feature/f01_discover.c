#include "feature/f01_discover.h"
#include <stdio.h>


void featureDiscover(Browser *b) {
    int total = b->db.count;

    if (total == 0) {
        printf("Database kosong, tidak ada halaman untuk ditampilkan.\n");
        return;
    }

    int toShow = (total < 5) ? total : 5;

    printf("Berikut adalah beberapa halaman yang mungkin menarik untukmu:\n");

    int i;
    for (i = 0; i < toShow; i++) {
        unsigned long rng = browserRandNext(b);
        int idx = (int)(rng % (unsigned long)total);
        printf("  - %s\n", b->db.pages[idx].url);
    }
}
