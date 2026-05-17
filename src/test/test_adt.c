#include <stdio.h>
#include <string.h>
#include "config.h"
#include "stack.h"
#include "queue.h"
#include "set.h"
#include "map.h"
#include "linked_list.h"
#include "graph.h"
#include "list.h"

/* Warna terminal untuk output test */
#define GREEN "\033[32m"
#define RED   "\033[31m"
#define RESET "\033[0m"

int passed = 0, failed = 0;

void check(const char *testName, int condition) {
    if (condition) {
        printf(GREEN "  [PASS]" RESET " %s\n", testName);
        passed++;
    } else {
        printf(RED   "  [FAIL]" RESET " %s\n", testName);
        failed++;
    }
}

/* ===== TEST STACK ===== */
void testStack() {
    printf("\n--- Stack ---\n");
    Stack s;
    stackInit(&s);

    check("Stack awal kosong", stackIsEmpty(&s));
    check("Back dari stack kosong = NULL", stackBack(&s) == NULL);
    check("Forward dari stack kosong = NULL", stackForward(&s) == NULL);

    stackPush(&s, "a.com");
    stackPush(&s, "b.com");
    stackPush(&s, "c.com");

    check("Current setelah 3x push = c.com",
          strcmp(stackCurrent(&s), "c.com") == 0);
    check("Back available = 2", stackBackAvailable(&s) == 2);
    check("Forward available = 0", stackForwardAvailable(&s) == 0);

    stackBack(&s);  /* Ke b.com */
    check("Setelah back, current = b.com",
          strcmp(stackCurrent(&s), "b.com") == 0);
    check("Forward available = 1 (ada c.com)", stackForwardAvailable(&s) == 1);

    /* Push baru dari tengah → hapus forward history */
    stackPush(&s, "d.com");
    check("Push baru dari tengah, current = d.com",
          strcmp(stackCurrent(&s), "d.com") == 0);
    check("Forward history terhapus", stackForwardAvailable(&s) == 0);
    check("Back masih ada (a.com, b.com)", stackBackAvailable(&s) == 2);

    /* Test back N langkah */
    const char *dest = stackBackN(&s, 2);
    check("BackN(2) dari d.com = a.com", dest && strcmp(dest, "a.com") == 0);
    check("BackN terlalu jauh = NULL", stackBackN(&s, 5) == NULL);
}

/* ===== TEST QUEUE ===== */
void testQueue() {
    printf("\n--- Queue ---\n");
    Queue q;
    queueInit(&q);

    check("Queue awal kosong", queueIsEmpty(&q));

    /* Tick saat kosong */
    printf("  (Output tick kosong): ");
    bool tickResult = queueTick(&q);
    check("Tick kosong return FALSE", !tickResult);

    /* Test rumus ticks: youtube.com = 10 karakter → floor(10/5)+2 = 4 */
    check("Calc ticks youtube.com = 4", queueCalcTicks("youtube.com") == 4);
    /* github.com = 10 karakter → 4 */
    check("Calc ticks github.com = 4", queueCalcTicks("github.com") == 4);
    /* a.co = 4 karakter → floor(4/5)+2 = 2 */
    check("Calc ticks a.co = 2", queueCalcTicks("a.co") == 2);

    queueEnqueue(&q, "test.com");
    check("Setelah enqueue, tidak kosong", !queueIsEmpty(&q));

    /* Isi queue sampai penuh */
    int i;
    for (i = 0; i < DOWNLOAD_MAX_AMOUNT - 1; i++) {
        queueEnqueue(&q, "x.com");
    }
    check("Queue penuh", queueIsFull(&q));
    check("Enqueue ke queue penuh = FALSE", !queueEnqueue(&q, "overflow.com"));
}

/* ===== TEST SET ===== */
void testSet() {
    printf("\n--- Set (Binary Search) ---\n");
    Set s;
    setInit(&s);

    check("Set awal kosong", s.count == 0);

    /* Insert beberapa halaman (sengaja tidak urut) */
    setInsert(&s, "mysteryshack.com", "Konten Mystery Shack");
    setInsert(&s, "apple.com", "Konten Apple");
    setInsert(&s, "zebra.org", "Konten Zebra");
    setInsert(&s, "banana.id", "Konten Banana");

    check("Count = 4 setelah 4x insert", s.count == 4);
    check("Array sorted: pages[0] = apple.com",
          strcmp(s.pages[0].url, "apple.com") == 0);
    check("Array sorted: pages[3] = zebra.org",
          strcmp(s.pages[3].url, "zebra.org") == 0);

    /* Binary search */
    WebPage *found = setSearch(&s, "mysteryshack.com");
    check("Binary search ketemu mysteryshack.com", found != NULL);
    check("Konten correct", found && strcmp(found->content, "Konten Mystery Shack") == 0);
    check("Binary search tidak ketemu = NULL", setSearch(&s, "notexist.com") == NULL);

    /* Duplikat ditolak */
    check("Insert duplikat = FALSE", !setInsert(&s, "apple.com", "duplikat"));
    check("Count tetap 4 setelah insert duplikat", s.count == 4);

    /* Delete */
    check("Delete banana.id = TRUE", setDelete(&s, "banana.id"));
    check("Count = 3 setelah delete", s.count == 3);
    check("Setelah delete, sorted masih ok: pages[0] = apple.com",
          strcmp(s.pages[0].url, "apple.com") == 0);

    /* Prefix search */
    setInsert(&s, "mystery-shop.com", "Konten Mystery Shop");
    WebPage *results[10];
    int n = setSearchPrefix(&s, "mystery", results, 10);
    check("Prefix search 'mystery' ketemu 2 hasil", n == 2);
}

/* ===== TEST MAP ===== */
void testMap() {
    printf("\n--- Map (Cache FIFO) ---\n");
    Map m;
    mapInit(&m);

    check("Map awal kosong", mapIsEmpty(&m));
    check("Get dari map kosong = NULL", mapGet(&m, "url.com") == NULL);

    mapPut(&m, "a.com", "Konten A");
    mapPut(&m, "b.com", "Konten B");

    const char *val = mapGet(&m, "a.com");
    check("Get a.com = 'Konten A'", val && strcmp(val, "Konten A") == 0);
    check("Get b.com tidak NULL", mapGet(&m, "b.com") != NULL);
    check("Get c.com = NULL (tidak ada)", mapGet(&m, "c.com") == NULL);

    /* Update existing key */
    mapPut(&m, "a.com", "Konten A Updated");
    check("Update a.com berhasil",
          strcmp(mapGet(&m, "a.com"), "Konten A Updated") == 0);
    check("Count tetap 2 setelah update", m.count == 2);

    /* Remove */
    check("Remove b.com = TRUE", mapRemove(&m, "b.com"));
    check("Get b.com setelah remove = NULL", mapGet(&m, "b.com") == NULL);
    check("Remove tidak ada = FALSE", !mapRemove(&m, "notexist.com"));
}

/* ===== TEST LINKED LIST ===== */
void testLinkedList() {
    printf("\n--- Linked List ---\n");
    LinkedList ll;
    llInit(&ll);

    check("LL awal kosong", llIsEmpty(&ll));

    llInsertFront(&ll, 2, "b.com");
    llInsertFront(&ll, 3, "c.com");
    llInsertFront(&ll, 1, "a.com");

    check("Size = 3", ll.size == 3);
    check("Contains 2 = TRUE", llContains(&ll, 2));
    check("Contains 99 = FALSE", !llContains(&ll, 99));

    /* Duplikat ditolak */
    check("Insert duplikat ID=2 = FALSE", !llInsertFront(&ll, 2, "dup.com"));
    check("Size tetap 3", ll.size == 3);

    /* Delete */
    check("Delete ID=2 = TRUE", llDelete(&ll, 2));
    check("Size = 2 setelah delete", ll.size == 2);
    check("Contains 2 = FALSE setelah delete", !llContains(&ll, 2));

    llClear(&ll);
    check("Setelah clear, kosong", llIsEmpty(&ll));
}

/* ===== TEST GRAPH ===== */
void testGraph() {
    printf("\n--- Graph ---\n");
    Graph g;
    graphInit(&g);

    graphAddNode(&g, 1);
    graphAddNode(&g, 2);
    graphAddNode(&g, 3);
    check("3 node ditambah", g.nodeCount == 3);

    check("AddEdge 1→2 = TRUE", graphAddEdge(&g, 1, 2, "b.com"));
    check("AddEdge 1→3 = TRUE", graphAddEdge(&g, 1, 3, "c.com"));
    check("AddEdge duplikat 1→2 = FALSE", !graphAddEdge(&g, 1, 2, "b.com"));
    check("Edge count = 2", g.edgeCount == 2);

    check("HasEdge 1→2 = TRUE", graphHasEdge(&g, 1, 2));
    check("HasEdge 2→1 = FALSE (directed)", !graphHasEdge(&g, 2, 1));

    LinkedList *nb = graphGetNeighbors(&g, 1);
    check("Neighbors node 1 = 2 elemen", nb && nb->size == 2);

    /* RemoveEdge */
    check("RemoveEdge 1→2 = TRUE", graphRemoveEdge(&g, 1, 2));
    check("Edge count = 1", g.edgeCount == 1);

    /* RemoveAllEdgesTo: hapus semua yang menuju node 3 */
    graphRemoveAllEdgesTo(&g, 3);
    check("Setelah removeAllEdgesTo(3), edge count = 0", g.edgeCount == 0);
}

/* ===== TEST LIST (TABS) ===== */
void testList() {
    printf("\n--- List (Tabs) ---\n");
    List l;
    listInit(&l);

    check("Init: 1 tab (TAB1)", l.count == 1);
    check("Current tab = TAB1",
          strcmp(listGetCurrentTab(&l)->name, "TAB1") == 0);

    /* Tambah tab */
    listAddTab(&l);
    listAddTab(&l);
    check("Count = 3", l.count == 3);
    check("Tab ke-2 = TAB2",
          strcmp(listGetTab(&l, 1)->name, "TAB2") == 0);

    /* Navigasi */
    check("nexttab 2 = TRUE", listNextTab(&l, 2));
    check("Current sekarang = TAB3",
          strcmp(listGetCurrentTab(&l)->name, "TAB3") == 0);
    check("nexttab 1 dari TAB3 = FALSE (tidak ada TAB4)", !listNextTab(&l, 1));

    check("prevtab 1 = TRUE", listPrevTab(&l, 1));
    check("Current sekarang = TAB2",
          strcmp(listGetCurrentTab(&l)->name, "TAB2") == 0);

    /* Close tab */
    check("closetab (TAB2) = TRUE", listCloseCurrentTab(&l));
    check("Count = 2", l.count == 2);
    /* Setelah close TAB2 dari tengah, current geser ke TAB3 */
    check("Current setelah close = TAB3",
          strcmp(listGetCurrentTab(&l)->name, "TAB3") == 0);

    /* Tambah lagi: harus TAB4 (counter tidak reset) */
    listAddTab(&l);
    check("Tab baru = TAB4",
          strcmp(listGetTab(&l, l.count - 1)->name, "TAB4") == 0);

    /* Tidak bisa close kalau hanya 1 tab */
    List l2;
    listInit(&l2);
    check("Close saat 1 tab = FALSE", !listCloseCurrentTab(&l2));
}

/* ===== MAIN ===== */
int main() {
    printf("========================================\n");
    printf("  TEST SUITE — All ADTs\n");
    printf("========================================\n");

    testStack();
    testQueue();
    testSet();
    testMap();
    testLinkedList();
    testGraph();
    testList();

    printf("\n========================================\n");
    printf("  Hasil: %d passed, %d failed\n", passed, failed);
    printf("========================================\n");

    return (failed == 0) ? 0 : 1;
}