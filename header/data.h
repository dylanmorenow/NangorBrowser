#ifndef DATA_H
#define DATA_H

/* ============================================================
 * data.h — Hardcoded database halaman web (STI)
 *
 * Spesifikasi STI: data di-hardcode dalam program, tidak
 * perlu load/save dari file external.
 *
 * Data mengikuti format web_pages.csv dan linked_pages.csv
 * yang ada di folder config/ repo.
 * ============================================================ */

#include "set.h"
#include "graph.h"

/* Inisialisasi semua hardcoded data ke dalam Set dan Graph.
 * Dipanggil sekali saat program mulai. */
void dataInit(Set *db, Graph *webGraph);

#endif /* DATA_H */