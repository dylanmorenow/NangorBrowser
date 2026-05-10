#include "graph.h"
#include <stdio.h>

/* ============================================================
 * graph.c — Implementasi ADT Graph (adjacency list)
 * ============================================================ */

void graphInit(Graph *g) {
    int i;
    for (i = 0; i < GRAPH_MAX_NODES; i++) {
        llInit(&g->adjList[i]);
        g->nodeExists[i] = FALSE;
    }
    g->nodeCount = 0;
    g->edgeCount = 0;
}

bool graphAddNode(Graph *g, int pageId) {
    if (pageId <= 0 || pageId >= GRAPH_MAX_NODES) return FALSE;
    if (g->nodeExists[pageId]) return FALSE;  /* Sudah ada */

    /* llInit sudah dilakukan di graphInit, tinggal tandai exist */
    g->nodeExists[pageId] = TRUE;
    g->nodeCount++;
    return TRUE;
}

bool graphRemoveNode(Graph *g, int pageId) {
    if (pageId <= 0 || pageId >= GRAPH_MAX_NODES) return FALSE;
    if (!g->nodeExists[pageId]) return FALSE;

    /* Hapus semua edge keluar dari node ini */
    int edgesRemoved = g->adjList[pageId].size;
    llClear(&g->adjList[pageId]);
    g->edgeCount -= edgesRemoved;

    /* Hapus semua edge yang masuk ke node ini */
    graphRemoveAllEdgesTo(g, pageId);

    g->nodeExists[pageId] = FALSE;
    g->nodeCount--;
    return TRUE;
}

bool graphAddEdge(Graph *g, int sourceId, int targetId, const char *targetUrl) {
    /* Validasi kedua node harus exist */
    if (sourceId <= 0 || sourceId >= GRAPH_MAX_NODES) return FALSE;
    if (targetId <= 0 || targetId >= GRAPH_MAX_NODES) return FALSE;
    if (!g->nodeExists[sourceId]) return FALSE;
    if (!g->nodeExists[targetId]) return FALSE;
    if (graphHasEdge(g, sourceId, targetId)) return FALSE;  /* Duplikat */

    if (llInsertFront(&g->adjList[sourceId], targetId, targetUrl)) {
        g->edgeCount++;
        return TRUE;
    }
    return FALSE;
}

bool graphRemoveEdge(Graph *g, int sourceId, int targetId) {
    if (sourceId <= 0 || sourceId >= GRAPH_MAX_NODES) return FALSE;
    if (!g->nodeExists[sourceId]) return FALSE;

    if (llDelete(&g->adjList[sourceId], targetId)) {
        g->edgeCount--;
        return TRUE;
    }
    return FALSE;
}

LinkedList* graphGetNeighbors(Graph *g, int pageId) {
    if (pageId <= 0 || pageId >= GRAPH_MAX_NODES) return NULL;
    if (!g->nodeExists[pageId]) return NULL;
    return &g->adjList[pageId];
}

bool graphHasEdge(Graph *g, int sourceId, int targetId) {
    if (sourceId <= 0 || sourceId >= GRAPH_MAX_NODES) return FALSE;
    if (!g->nodeExists[sourceId]) return FALSE;
    return llContains(&g->adjList[sourceId], targetId);
}

void graphPrintNeighbors(Graph *g, int pageId) {
    if (pageId <= 0 || pageId >= GRAPH_MAX_NODES ||
        !g->nodeExists[pageId]) {
        printf("Linked pages:\n  (Tidak ada)\n");
        return;
    }

    LinkedList *neighbors = &g->adjList[pageId];
    if (llIsEmpty(neighbors)) {
        printf("Linked pages:\n  (Tidak ada)\n");
        return;
    }

    printf("Linked pages:\n");
    /* Print dalam urutan maju (linked list head = yang terakhir insert).
     * Untuk tampilan konsisten, kita traverse dan print dengan nomor. */
    LLNode *cur = neighbors->head;
    int i = 1;
    while (cur != NULL) {
        printf("  [%d] %s\n", i++, cur->targetUrl);
        cur = cur->next;
    }
}

void graphPrintAll(Graph *g) {
    printf("=== Web Graph (%d nodes, %d edges) ===\n",
           g->nodeCount, g->edgeCount);
    int i;
    for (i = 1; i < GRAPH_MAX_NODES; i++) {
        if (g->nodeExists[i]) {
            printf("  Node [%d]: ", i);
            if (llIsEmpty(&g->adjList[i])) {
                printf("(no outgoing links)\n");
            } else {
                LLNode *cur = g->adjList[i].head;
                while (cur != NULL) {
                    printf("→ %s ", cur->targetUrl);
                    cur = cur->next;
                }
                printf("\n");
            }
        }
    }
}

void graphRemoveAllEdgesTo(Graph *g, int targetId) {
    int i;
    for (i = 1; i < GRAPH_MAX_NODES; i++) {
        if (g->nodeExists[i] && llContains(&g->adjList[i], targetId)) {
            if (llDelete(&g->adjList[i], targetId)) {
                g->edgeCount--;
            }
        }
    }
}