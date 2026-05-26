#include "data.h"
#include <stdio.h>



/* Helper: tambah halaman + node graph sekaligus */
static void addPage(Set *db, Graph *g,
                    const char *url, const char *content) {
    setInsert(db, url, content);
    WebPage *p = setSearch(db, url);
    if (p != NULL) graphAddNode(g, p->id);
}

/* Helper: tambah edge graph berdasarkan URL */
static void addLink(Set *db, Graph *g,
                    const char *srcUrl, const char *dstUrl) {
    WebPage *src = setSearch(db, srcUrl);
    WebPage *dst = setSearch(db, dstUrl);
    if (src != NULL && dst != NULL) {
        graphAddEdge(g, src->id, dst->id, dstUrl);
    }
}

void dataInit(Set *db, Graph *webGraph) {


    addPage(db, webGraph,
        "mysteryshack.com",
        "SELAMAT DATANG DI MYSTERY SHACK!\n"
        "Tempat paling ajaib di Nangor Falls (dan satu-satunya yang punya diskon 0%!).\n"
        "Lihatlah Jackalope asli! Lihatlah batu yang menyerupai wajah Stun!\n"
        "Peringatan: Barang yang sudah dibeli tidak dapat dikembalikan,\n"
        "apalagi kalau Anda baru sadar itu cuma barang rongsokan yang ditempel lem."
    );

    addPage(db, webGraph,
        "gideon-tent.org",
        "BACA MASA DEPANMU DI SINI!\n"
        "\"Lil' Gideon\" tahu segalanya tentangmu.\n"
        "Bahkan rahasia yang kau sembunyikan di bawah tempat tidur.\n"
        "Datanglah ke 'Tent of Telepathy', sekarang juga!"
    );

    addPage(db, webGraph,
        "batu-ajaib-asli.com",
        "BATU AJAIB ASLI - LANGSUNG DARI GUA MISTERIUS!\n"
        "Dijamin 100% ajaib (syarat dan ketentuan berlaku).\n"
        "Batu ini konon bisa membawa keberuntungan, atau setidaknya\n"
        "bisa dipakai sebagai pengganjal pintu."
    );

    addPage(db, webGraph,
        "topi-pinus-original.com",
        "TOPI PINUS IKONIK - SEPERTI YANG DIPAKAI ANAK ITU!\n"
        "Stok terbatas! Dapatkan topi biru-putih dengan logo pohon pinus yang melegenda.\n"
        "Dijamin 100% serat kain asli (mungkin)."
    );

    addPage(db, webGraph,
        "journal-3.nf",
        "JURNAL #3 - DOKUMEN RAHASIA NANGOR FALLS\n"
        "Penulis: Tidak Diketahui\n"
        "Isi jurnal ini mengandung pengetahuan berbahaya tentang makhluk-makhluk\n"
        "misterius di Nangor Falls. Baca dengan risiko sendiri.\n"
        "Halaman 1: Gnome - makhluk kecil yang suka mencuri kaus kaki."
    );

    addPage(db, webGraph,
        "nangor-falls-news.com",
        "NANGOR FALLS NEWS - BERITA TERPERCAYA KOTA PALING ANEH\n"
        "Headline hari ini:\n"
        "- Warga laporkan penampakan Bigfoot di supermarket lokal\n"
        "- Harga batu ajaib naik 200% jelang musim panas\n"
        "- Mystery Shack buka cabang baru, Stun Pinus bantah rumor"
    );

    addPage(db, webGraph,
        "topi-pinus-kw.com",
        "TOPI PINUS KW SUPER - HARGA TERJANGKAU!\n"
        "Tampil keren seperti detektif cilik tanpa menguras kantong.\n"
        "Tersedia dalam 3 ukuran: S, M, L\n"
        "Disclaimer: Ini bukan produk resmi dari Mystery Shack."
    );

    addPage(db, webGraph,
        "cipher.nf",
        "BILL CIPHER - ENTITAS DIMENSI LAIN\n"
        "\"Hai! Mau tau rahasia alam semesta?\"\n"
        "\"Tentu saja kamu mau. Semua orang mau.\"\n"
        "\"Deal?\"\n"
        "Peringatan: Berinteraksi dengan halaman ini SANGAT tidak disarankan."
    );

    addPage(db, webGraph,
        "six.itb.ac.id",
        "SIX ITB - SISTEM INFORMASI AKADEMIK ITB\n"
        "Selamat datang di portal akademik Institut Teknologi Bandung.\n"
        "Layanan: Nilai, KRS, Transkrip, Jadwal Kuliah\n"
        "Status server: Sibuk seperti biasa menjelang deadline."
    );

    addPage(db, webGraph,
        "mystery-shop.com",
        "MYSTERY SHOP - BELANJA BARANG MISTERIUS ONLINE\n"
        "Ribuan produk aneh tersedia 24 jam!\n"
        "Gratis ongkir untuk pembelian di atas Rp 500.000\n"
        "Garansi uang kembali jika barang tidak semisterius yang dijanjikan."
    );

    /* ======================================================
     * LINKED PAGES (edges di web graph)
     * Format: addLink(db, webGraph, "sumber", "tujuan")
     * ====================================================== */

    /* mysteryshack.com → */
    addLink(db, webGraph, "mysteryshack.com", "batu-ajaib-asli.com");
    addLink(db, webGraph, "mysteryshack.com", "topi-pinus-original.com");
    addLink(db, webGraph, "mysteryshack.com", "gideon-tent.org");
    addLink(db, webGraph, "mysteryshack.com", "nangor-falls-news.com");

    /* gideon-tent.org → */
    addLink(db, webGraph, "gideon-tent.org", "mysteryshack.com");
    addLink(db, webGraph, "gideon-tent.org", "cipher.nf");

    /* batu-ajaib-asli.com → */
    addLink(db, webGraph, "batu-ajaib-asli.com", "mysteryshack.com");
    addLink(db, webGraph, "batu-ajaib-asli.com", "mystery-shop.com");

    /* topi-pinus-original.com → */
    addLink(db, webGraph, "topi-pinus-original.com", "topi-pinus-kw.com");
    addLink(db, webGraph, "topi-pinus-original.com", "mysteryshack.com");

    /* journal-3.nf → */
    addLink(db, webGraph, "journal-3.nf", "mysteryshack.com");
    addLink(db, webGraph, "journal-3.nf", "cipher.nf");
    addLink(db, webGraph, "journal-3.nf", "nangor-falls-news.com");

    /* nangor-falls-news.com → */
    addLink(db, webGraph, "nangor-falls-news.com", "mysteryshack.com");
    addLink(db, webGraph, "nangor-falls-news.com", "batu-ajaib-asli.com");

    /* cipher.nf → */
    addLink(db, webGraph, "cipher.nf", "gideon-tent.org");
    addLink(db, webGraph, "cipher.nf", "journal-3.nf");

    /* six.itb.ac.id → tidak ada linked pages */

    /* mystery-shop.com → */
    addLink(db, webGraph, "mystery-shop.com", "mysteryshack.com");
    addLink(db, webGraph, "mystery-shop.com", "topi-pinus-kw.com");
    addLink(db, webGraph, "mystery-shop.com", "batu-ajaib-asli.com");

    printf("[System] Database loaded: %d halaman, %d relasi.\n",
           db->count, webGraph->edgeCount);
}