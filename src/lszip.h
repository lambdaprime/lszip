/* 
 * This source file is a part of lszip command-line program.
 * Description for this  project/command/program can be found in ReadMe.txt.
 *
 * lszip is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * lszip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with lszip. If not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef LSZIP_H
#define LSZIP_H

#include "LinkedList.h"
#include "MemoryAllocator.h"
#include "unzip.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#define MAX_FILE_NAME 124
#define MAX_PATH_NAME 1024
#define ZIP_FILESEP_CHAR '/'
#define ZIP_FILESEP_STR "/"

//#define TRACING

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>


//#include <tchar.h>

#ifdef TRACING
#define TRACE_PREFIX "[TRACE] "
inline void _trace() {}
inline void _trace(const char *format, ...) {
    va_list(arglist);
    va_start(arglist, format);
    vfprintf(stderr, format, arglist);
}
#define trace(...) \
    _trace("%s %s (%d): ", TRACE_PREFIX, __FILE__, __LINE__); \
    _trace(__VA_ARGS__); \
    fprintf(stderr, "\n")
#define traceIn(...) \
    _trace("%s %s (%d): ", TRACE_PREFIX, __FILE__, __LINE__); \
    _trace("> entry %s (", __FUNCTION__); \
    _trace(__VA_ARGS__); \
    fprintf(stderr, ")\n");
#define traceOut(X) \
    _trace("%s %s (%d): ", TRACE_PREFIX, __FILE__, __LINE__); \
    _trace("< exit %s ", __FUNCTION__); \
    fprintf(stderr, "\n"); \
    return X;
#else
#define trace(...)
#define traceIn(...)
#define traceOut(X) \
    return X;
#endif

typedef struct {
    char * fileName; // name of zipped file
    unz_file_info fileInfo; 
} zip_entry_t;

// Contains 1 if option "-l" was specified, 0 otherwise
int IsLongListing = 0;

// Place directories at the begining of the listing
int IsDirsFirst = 0;

// We save maximum file size to format "size" field during 
// listing output
unsigned int MaxFileSize = 0;

const int Sizeof_zip_entry_t = sizeof(zip_entry_t);

// Contains name of the zipped directory, which content will be shown
const char* LsDirName;

const char* LsFileName;

// Used to decrease number of duplicates in `DirListing' linked list
char PrevZippedFileName[MAX_FILE_NAME];

// Stores `LsDirName' content, in format of zip entries `zip_entry_t'
LinkedList DirListing;

// Same as DirListing, but keeps directories only (for option -2)
LinkedList DirListing2;

// Memory allocator used for `DirListing' list
MA_allocator MemAlloc;

#endif
