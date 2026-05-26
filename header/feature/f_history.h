#ifndef F_HISTORY_H
#define F_HISTORY_H



#include "browser.h"

/* Inisialisasi global history */
void historyInit(GlobalHistory *gh);

/* Catat akses halaman (dipanggil setiap open/openlinked/back/forward) */
void historyRecord(GlobalHistory *gh, const char *url);

/* Tampilkan history (command: history) */
void featureViewHistory(GlobalHistory *gh);

#endif /* F_HISTORY_H */
