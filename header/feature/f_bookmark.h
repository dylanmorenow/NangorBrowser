#ifndef F_BOOKMARK_H
#define F_BOOKMARK_H

/* ============================================================
 * f_bookmark.h — B04 Bookmark Manager
 *
 * Struct BookmarkManager sudah didefinisikan di browser.h.
 * ============================================================ */

#include "browser.h"

/* Inisialisasi bookmark manager */
void bookmarkInit(BookmarkManager *bm);

/* add_bookmark <alias> <url> */
void featureAddBookmark(Browser *b, const char *alias, const char *url);

/* view_bookmark */
void featureViewBookmark(Browser *b);

/* delete_bookmark <alias> */
void featureDeleteBookmark(Browser *b, const char *alias);

/* open_bookmark <alias> */
void featureOpenBookmark(Browser *b, const char *alias);

#endif /* F_BOOKMARK_H */
