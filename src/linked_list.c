#include "linked_list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ============================================================
 * linked_list.c — Implementasi ADT Singly Linked List
 * ============================================================ */

void llInit(LinkedList *ll) {
    ll->head = NULL;
    ll->size = 0;
}

bool llIsEmpty(LinkedList *ll) {
    return (ll->head == NULL);
}

bool llInsertFront(LinkedList *ll, int targetId, const char *targetUrl) {
    /* Tolak duplikat */
    if (llContains(ll, targetId)) return FALSE;

    /* Alokasi node baru */
    LLNode *newNode = (LLNode *)malloc(sizeof(LLNode));
    if (newNode == NULL) return FALSE;  /* Malloc gagal */

    newNode->targetId = targetId;
    strncpy(newNode->targetUrl, targetUrl, MAX_URL_LENGTH - 1);
    newNode->targetUrl[MAX_URL_LENGTH - 1] = '\0';

    /* Sambung ke head */
    newNode->next = ll->head;
    ll->head      = newNode;
    ll->size++;

    return TRUE;
}

bool llDelete(LinkedList *ll, int targetId) {
    if (llIsEmpty(ll)) return FALSE;

    /* Kasus khusus: node yang mau dihapus adalah head */
    if (ll->head->targetId == targetId) {
        LLNode *toDelete = ll->head;
        ll->head = ll->head->next;
        free(toDelete);
        ll->size--;
        return TRUE;
    }

    /* Traverse cari node sebelum yang mau dihapus */
    LLNode *cur = ll->head;
    while (cur->next != NULL && cur->next->targetId != targetId) {
        cur = cur->next;
    }

    if (cur->next == NULL) return FALSE;  /* Tidak ditemukan */

    LLNode *toDelete = cur->next;
    cur->next = toDelete->next;
    free(toDelete);
    ll->size--;
    return TRUE;
}

bool llContains(LinkedList *ll, int targetId) {
    LLNode *cur = ll->head;
    while (cur != NULL) {
        if (cur->targetId == targetId) return TRUE;
        cur = cur->next;
    }
    return FALSE;
}

const char* llGetByIndex(LinkedList *ll, int index) {
    if (index < 0 || index >= ll->size) return NULL;

    LLNode *cur = ll->head;
    int i;
    for (i = 0; i < index; i++) {
        cur = cur->next;
    }
    return cur->targetUrl;
}

int llGetIdByIndex(LinkedList *ll, int index) {
    if (index < 0 || index >= ll->size) return -1;

    LLNode *cur = ll->head;
    int i;
    for (i = 0; i < index; i++) {
        cur = cur->next;
    }
    return cur->targetId;
}

void llPrint(LinkedList *ll) {
    if (llIsEmpty(ll)) {
        printf("  (Tidak ada linked pages)\n");
        return;
    }

    LLNode *cur = ll->head;
    int i = 1;
    while (cur != NULL) {
        printf("  [%d] %s\n", i++, cur->targetUrl);
        cur = cur->next;
    }
}

void llClear(LinkedList *ll) {
    LLNode *cur = ll->head;
    while (cur != NULL) {
        LLNode *next = cur->next;
        free(cur);
        cur = next;
    }
    ll->head = NULL;
    ll->size = 0;
}