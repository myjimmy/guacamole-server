/*
 * Copyright (C) 2016 Glyptodon LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#ifndef GUAC_TERMINAL_TYPESCRIPT_H
#define GUAC_TERMINAL_TYPESCRIPT_H

#include "config.h"

#include <guacamole/timestamp.h>

/**
 * A NULL-terminated string of raw bytes which should be written at the
 * beginning of any typescript.
 */
#define GUAC_TERMINAL_TYPESCRIPT_HEADER "[BEGIN TYPESCRIPT]\n"

/**
 * A NULL-terminated string of raw bytes which should be written at the
 * end of any typescript.
 */
#define GUAC_TERMINAL_TYPESCRIPT_FOOTER "\n[END TYPESCRIPT]\n"

/**
 * The maximum amount of time to allow for a particular timing entry, in
 * milliseconds. Any timing entries exceeding this value will be written as
 * exactly this value instead.
 */
#define GUAC_TERMINAL_TYPESCRIPT_MAX_DELAY 86400000

/**
 * The maximum numeric value allowed for the .1, .2, .3, etc. suffix appended
 * to the end of the typescript filename if a typescript having the requested
 * name already exists.
 */
#define GUAC_TERMINAL_TYPESCRIPT_MAX_SUFFIX 255

/**
 * The maximum length of the string containing a sequential numeric suffix
 * between 1 and GUAC_TERMINAL_TYPESCRIPT_MAX_SUFFIX inclusive, in bytes,
 * including NULL terminator.
 */
#define GUAC_TERMINAL_TYPESCRIPT_MAX_SUFFIX_LENGTH 4

/**
 * The maximum overall length of the full path to the typescript file,
 * including any additional suffix and NULL terminator, in bytes.
 */
#define GUAC_TERMINAL_TYPESCRIPT_MAX_NAME_LENGTH 2048

/**
 * The suffix which will be appended to the typescript data file's name to
 * produce the name of the timing file.
 */
#define GUAC_TERMINAL_TYPESCRIPT_TIMING_SUFFIX "timing"

/**
 * An active typescript, consisting of a data file (raw terminal output) and
 * timing file (related timestamps and byte counts).
 */
typedef struct guac_terminal_typescript {

    /**
     * Buffer of raw terminal output which has not yet been written to the
     * data file.
     */
    char buffer[4096];

    /**
     * The number of bytes currently stored in the buffer.
     */
    int length;

    /**
     * The file descriptor of the file into which raw terminal output should be
     * written.
     */
    int data_fd;

    /**
     * The file descriptor of the file into which timing information
     * (timestamps and byte counts) related to the raw terminal output in the
     * data file should be written.
     */
    int timing_fd;

    /**
     * The last time that this typescript was flushed. If this typescript was
     * never flushed, this will be the time the typescripe was created.
     */
    guac_timestamp last_flush;

} guac_terminal_typescript;

/**
 * Creates a new pair of typescript files within the given path and using the
 * given base name, returning an abstraction which represents those files.
 * Terminal output will be written to these new files, along with timing
 * information. If the create_path flag is non-zero, the given path will be
 * created if it does not yet exist.
 *
 * @param path
 *     The full absolute path to a directory in which the typescript files
 *     should be created.
 *
 * @param name
 *     The base name to use for the typescript files created within the
 *     specified path.
 *
 * @param create_path
 *     Zero if the specified path MUST exist for typescript files to be
 *     written, or non-zero if the path should be created if it does not yet
 *     exist.
 *
 * @return
 *     A new guac_terminal_typescript representing the typescript files
 *     requested, or NULL if creation of the typescript files failed.
 */
guac_terminal_typescript* guac_terminal_typescript_alloc(const char* path,
        const char* name, int create_path);

/**
 * Writes a single byte of terminal data to the typescript, flushing and
 * writing a new timestamp if necessary.
 *
 * @param typescript
 *     The typescript that the given byte of raw terminal data should be
 *     written to.
 *
 * @param c
 *     The single byte of raw terminal data to write to the typescript.
 */
void guac_terminal_typescript_write(guac_terminal_typescript* typescript,
        char c);

/**
 * Flushes any pending data to the typescript, writing a new timestamp to the
 * timing file if any data was flushed.
 *
 * @param typescript
 *     The typescript which should be flushed.
 */
void guac_terminal_typescript_flush(guac_terminal_typescript* typescript);

/**
 * Frees all resources associated with the given typescript, flushing and
 * closing the data and timing files and freeing all related memory. If the
 * provided typescript is NULL, this function has no effect.
 *
 * @param typescript
 *     The typescript to free.
 */
void guac_terminal_typescript_free(guac_terminal_typescript* typescript);

#endif

