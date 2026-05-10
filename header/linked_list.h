#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/* ============================================================
 * linked_list.h — ADT Linked List (singly linked)
 *
 * KENAPA Linked List?
 * Dipakai sebagai adjacency list di ADT Graph.
 * Setiap node graph punya linked list yang berisi daftar
 * tetangganya (linked pages).
 *
 * Pakai linked list (bukan array) di sini karena:
 * - Jumlah linked pages per halaman tidak diketahui di awal
 * - Insert O(1) di head (tidak perlu realloc)
 * - Traverse O(n) sudah cukup untuk use-case ini
 *
 * Setiap node menyimpan:
 * - targetId  : ID halaman tujuan (foreign key ke WebPage.id)
 * - targetUrl : URL halaman tujuan (cache lokal supaya tidak
 *               perlu lookup ke Set setiap kali tampil)
 * ============================================================ */

#include "config.h"

/* Node satu elemen di linked list */
typedef struct LLNode {
    int          targetId;
    char         targetUrl[MAX_URL_LENGTH];
    struct LLNode *next;
} LLNode;

/* Linked List container */
typedef struct {
    LLNode *head;
    int     size;
} LinkedList;

/* --- Operasi Linked List --- */

/* Inisialisasi linked list kosong. */
void llInit(LinkedList *ll);

/* Cek apakah linked list kosong. */
bool llIsEmpty(LinkedList *ll);

/* Tambah node baru di depan (O(1)).
 * Duplikat targetId ditolak (set semantics). */
bool llInsertFront(LinkedList *ll, int targetId, const char *targetUrl);

/* Hapus node berdasarkan targetId.
 * Return TRUE jika berhasil, FALSE jika tidak ditemukan. */
bool llDelete(LinkedList *ll, int targetId);

/* Cek apakah targetId ada di linked list. */
bool llContains(LinkedList *ll, int targetId);

/* Ambil URL berdasarkan urutan (0-indexed), untuk openlinked <x>.
 * Return pointer ke targetUrl, NULL jika indeks invalid. */
const char* llGetByIndex(LinkedList *ll, int index);

/* Ambil ID berdasarkan urutan (0-indexed). */
int llGetIdByIndex(LinkedList *ll, int index);

/* Print semua node (untuk tampilkan linked pages). */
void llPrint(LinkedList *ll);

/* Hapus semua node dan bebaskan memori. */
void llClear(LinkedList *ll);

#endif /* LINKED_LIST_H */