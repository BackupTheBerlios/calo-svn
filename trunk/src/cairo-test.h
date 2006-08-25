/*
 * Copyright © 2004 Red Hat, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of
 * Red Hat, Inc. not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior
 * permission. Red Hat, Inc. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * RED HAT, INC. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL RED HAT, INC. BE LIABLE FOR ANY SPECIAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR
 * IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author: Carl D. Worth <cworth@cworth.org>
 */

#ifndef _CAIRO_TEST_H_
#define _CAIRO_TEST_H_

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <math.h>
#include <cairo.h>

CAIRO_BEGIN_DECLS

# include <stdint.h>

#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#define CAIRO_PRINTF_FORMAT(fmt_index, va_index) \
	__attribute__((__format__(__printf__, fmt_index, va_index)))
#else
#define CAIRO_PRINTF_FORMAT(fmt_index, va_index)
#endif

typedef enum cairo_test_status {
    CAIRO_TEST_SUCCESS = 0,
    CAIRO_TEST_FAILURE,
    CAIRO_TEST_UNTESTED,
    CAIRO_TEST_CRASHED
} cairo_test_status_t;

typedef cairo_test_status_t  (cairo_test_draw_function_t) (cairo_t *cr, int width, int height);

typedef struct _cairo_test {
    const char *name;
    const char *description;
    int width;
    int height;
    cairo_test_draw_function_t *draw;
} cairo_test_t;

/* The standard test interface which works by examining result image.
 *
 * cairo_test() accepts a test struct which will be called once for
 * each testable backend. The following checks will be performed for
 * each backend:
 *
 * 1) If draw() does not return CAIRO_TEST_SUCCESS then this backend
 *    fails.
 *
 * 2) Otherwise, if cairo_status(cr) indicates an error then this
 *    backend fails.
 *
 * 3) Otherwise, if the image size is 0, then this backend passes.
 *
 * 4) Otherwise, if every channel of every pixel exactly matches the
 *    reference image then this backend passes. If not, this backend
 *    fails.
 *
 * The overall test result is PASS if and only if there is at least
 * one backend that is tested and if all tested backend pass according
 * to the four criteria above.
 */
cairo_test_status_t
cairo_test (cairo_test_t *test);

/* cairo_test_init() and cairo_test_log() exist to help in writing
 * tests for which cairo_test() is not appropriate for one reason or
 * another. For example, some tests might not be doing any drawing at
 * all, or may need to create their own cairo_t rather than be handed
 * one by cairo_test.
 */

/* Initialize test-specific resources, (log files, etc.) */
void
cairo_test_init (const char *test_name);

/* Print a message to the log file, ala printf. */
void
cairo_test_log (const char *fmt, ...) CAIRO_PRINTF_FORMAT(1, 2);

/* Helper functions that take care of finding source images even when
 * building in a non-srcdir manner, (ie. the tests will be run in a
 * directory that is different from the one where the source image
 * exists). */
cairo_surface_t *
cairo_test_create_surface_from_png (const char *filename);

cairo_pattern_t *
cairo_test_create_pattern_from_png (const char *filename);

cairo_status_t
cairo_test_paint_checkered (cairo_t *cr);

void
xasprintf (char **strp, const char *fmt, ...) CAIRO_PRINTF_FORMAT(2, 3);

CAIRO_END_DECLS

#endif
