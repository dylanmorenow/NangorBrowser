#ifndef GRAPH_H
#define GRAPH_H


#include "config.h"
#include "linked_list.h"

#define GRAPH_MAX_NODES (MAX_WEB_PAGES + 1)

typedef struct {
    LinkedList adjList[GRAPH_MAX_NODES]; /* adjList[i] = tetangga node i */
    bool       nodeExists[GRAPH_MAX_NODES]; /* Apakah node i valid? */
    int        nodeCount;
    int        edgeCount;
} Graph;

/* Inisialisasi graph kosong. */
void graphInit(Graph *g);

/* Tambah node baru ke graph.
 * Dipanggil saat halaman baru ditambahkan ke database.
 * Return TRUE jika berhasil, FALSE jika ID sudah ada atau out of range. */
bool graphAddNode(Graph *g, int pageId);

/* Hapus node dari graph beserta semua edge yang terhubung.
 * Dipanggil saat delete_page. */
bool graphRemoveNode(Graph *g, int pageId);

/* Tambah edge berarah dari sourceId ke targetId.
 * Butuh targetUrl untuk disimpan di adjacency list (supaya bisa tampil).
 * Return TRUE jika berhasil. */
bool graphAddEdge(Graph *g, int sourceId, int targetId, const char *targetUrl);

/* Hapus edge dari sourceId ke targetId.
 * Return TRUE jika berhasil. */
bool graphRemoveEdge(Graph *g, int sourceId, int targetId);

/* Ambil adjacency list (linked list tetangga) dari node tertentu.
 * Return pointer ke LinkedList, NULL jika node tidak ada. */
LinkedList* graphGetNeighbors(Graph *g, int pageId);

/* Cek apakah ada edge dari sourceId ke targetId. */
bool graphHasEdge(Graph *g, int sourceId, int targetId);

/* Print semua tetangga dari sebuah node (untuk tampilkan linked pages).
 * Format: [1] url1  [2] url2  ... */
void graphPrintNeighbors(Graph *g, int pageId);

/* Print seluruh graph (untuk debug). */
void graphPrintAll(Graph *g);

/* Hapus semua edge yang MENUJU ke targetId di seluruh graph.
 * Diperlukan saat delete_page: bersihkan semua referensi ke halaman tersebut. */
void graphRemoveAllEdgesTo(Graph *g, int targetId);

#endif 