/* 
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

// lszip.cpp : Defines the entry point for the console application.
//

#include <string.h>
#include <memory.h>
#include "lszip.h"
//#include <regex.h>

int numberOfDigits(int a) {
    int c = 1;
    while (a /= 10) {
        c++;
    }
    return c;
}

///* Checks whether file name matches regular expression
// */
//int matches(const char* fileName) {
//    traceIn("%s", fileName);
//    regex_t rexp;
//    int retCode = regcomp(&rexp, LsDirName, REG_EXTENDED);
//    trace("%s", LsDirName);
//    if (retCode != 0) {
//        perror("Could not compile regular expression");
//        return -1;
//    }
//
//    if (regexec(&rexp, fileName, 0, NULL, 0) == 0) {
//        retCode = 1;
//    } else {
//        retCode = 0;
//    }
//
//    regfree(&rexp);
//
//    traceOut();
//    return retCode;
//}

/*
 * Prints listing output 
 */
void print(void *data) {
    traceIn();
    zip_entry_t *zipEntry = (zip_entry_t *)data;
    
    //if (!matches(zipEntry->fileName)) {
    //    return;
    //}

    if (IsLongListing) {
        int len = numberOfDigits(MaxFileSize);
        int formatSize = numberOfDigits(len) + 5;
        char *format = (char *)malloc(formatSize);
        if (format != NULL) {
            snprintf(format, formatSize, "%%%dd  ", numberOfDigits(MaxFileSize));
            printf(format, zipEntry->fileInfo.uncompressed_size);
            printf("%2.2lu-%2.2lu-%2.2lu  %2.2lu:%2.2lu   ", 
                   (uLong)zipEntry->fileInfo.tmu_date.tm_mon + 1,
                   (uLong)zipEntry->fileInfo.tmu_date.tm_mday,
                   (uLong)zipEntry->fileInfo.tmu_date.tm_year % 100,
                   (uLong)zipEntry->fileInfo.tmu_date.tm_hour,
                   (uLong)zipEntry->fileInfo.tmu_date.tm_min);
            free(format);
        }
    }
    printf("%s", zipEntry->fileName);
    printf("\n");
    traceOut();
}

int comparator(const void* cmp1, const void* cmp2) {
    zip_entry_t* zipEntry1 = (zip_entry_t*)cmp1;
    zip_entry_t* zipEntry2 = (zip_entry_t*)cmp2;
    //trace("%s %s", zipEntry1->fileName, zipEntry2->fileName);
    return strcmp(zipEntry1->fileName, zipEntry2->fileName);
}

inline int processZippedFile(char* filePath, const unz_file_info* fileInfo) {
    traceIn("%s", filePath);

    int lsDirNameLen = strlen(LsDirName);
    int lsFileNameLen = strlen(LsFileName);

    // true if we are listing root directory of zip file
    int isLsRootDir = lsDirNameLen == 0;

    if (!isLsRootDir && strncmp(LsDirName, filePath, lsDirNameLen) != 0) {
        traceOut(0);
    }

    char* fileName = filePath + lsDirNameLen;

    if (*fileName == ZIP_FILESEP_CHAR) {
        fileName++;
    }

    if (lsFileNameLen && strcmp(fileName, LsFileName) != 0) {
        traceOut(0);
    }

    int p = strcspn(fileName, ZIP_FILESEP_STR);

    if (fileName[p] == ZIP_FILESEP_CHAR) {
        p++;
    }

    fileName[p] = '\0';

    trace("Listing %s", fileName);
    trace("Previous path %s", PrevZippedFileName);

    // omit "fileName" subdirectories listing
    if (strcmp(PrevZippedFileName, fileName) == 0) {
        traceOut(0);
    }

    int fileNameLen = strlen(fileName);
    zip_entry_t *zipEntry = (zip_entry_t *)MA_allocate(MemAlloc, Sizeof_zip_entry_t);
    if (zipEntry == NULL) {
        perror("Memory allocation error");
        return -1;
    }

    memcpy(&zipEntry->fileInfo, fileInfo, sizeof(unz_file_info));
            
    zipEntry->fileName = (char*)MA_allocate(MemAlloc, fileNameLen + 1);
    if (zipEntry->fileName == NULL) {
        perror("Memory allocation error");
        return -1;
    }

    strcpy(zipEntry->fileName, fileName);

    if (MaxFileSize < fileInfo->uncompressed_size) {
        MaxFileSize = fileInfo->uncompressed_size;
    }

    trace("Adding item %s", fileName);
    if (IsDirsFirst && zipEntry->fileName[fileNameLen - 1] == ZIP_FILESEP_CHAR) {
        trace("to the directory list", fileName);
        LL_append(DirListing2, zipEntry);
    } else {
        LL_append(DirListing, zipEntry);
    }

    strcpy(PrevZippedFileName, fileName);

    traceOut(0);
}

int walkThruZip(const char* zipFileName) {
    traceIn();

    unzFile zipFile = unzOpen(zipFileName);
    unz_file_info fileInfo;
    int retCode;
    char filePath[MAX_PATH_NAME];

    if (zipFile == NULL) {
        fprintf(stderr, "File '%s' doesn't exist or is not valid.\n", zipFileName);
        traceOut(-1);
    }

    memset(filePath, 0, MAX_PATH_NAME);

    for (retCode = unzGoToFirstFile(zipFile);
         retCode == UNZ_OK;
         retCode = unzGoToNextFile(zipFile))
    {

        retCode = unzGetCurrentFileInfo(zipFile, &fileInfo, filePath, MAX_PATH_NAME, NULL, 0, NULL, 0);
        if (retCode != UNZ_OK) {
            fprintf(stderr, "Error %d with zip file in unzGetCurrentFileInfo\n", retCode);
            traceOut(-1);
        }

        retCode = processZippedFile(filePath, &fileInfo);
        if (retCode != 0) {
            fprintf(stderr, "Unable to process zipped file %s\n", filePath);
            traceOut(-1);
        }

    }

    // if unzGoToFirstFile() failed
    if (retCode != UNZ_OK && retCode != UNZ_END_OF_LIST_OF_FILE) {
        printf("Error %d with zip file in unzGoToFirstFile\n", retCode);
        traceOut(-1);
    }

    unzClose(zipFile);
    traceOut(0);

}

void showUsageBanner() {
    puts("Usage: lszip [OPTION] <ZIP_FILE> [NAME]");
    puts("List information about files or directories in ZIP_FILE zip archive.");
    puts("The NAME which ends with / resolved to the directory name (/some/path/), everything else to the file name (/some/path).");
    puts("By default NAME is equal to root / directory.");
    puts("Entities are sorted alphabetically.");
    puts("");
    puts("Options: ");
    puts(" -l use a long listing format");
    puts(" -h display this help and exit");
    puts(" -v output version information and exit");
    puts(" -2 list directories first");
}

void showVersionBanner()
{
    puts("lszip 1.0 Copyright (C) 2010 ID <id.blackmesa@gmail.com>");
    puts("");
    puts("This program comes with ABSOLUTELY NO WARRANTY.");
    puts("This is free software, and you are welcome to redistribute it");
    puts("under the terms of the GNU General Public License:");
    puts("<http://www.gnu.org/licenses/gpl.html>");
    puts("");
}

void initialize() {
    traceIn();

    MemAlloc = MA_create(65536);
    if (MemAlloc == NULL) {
        perror("Memory allocation error");
        exit(1);
    }

    DirListing = LL_create();
    if (DirListing == NULL) {
        perror("Failed to create LinkedList");
        exit(1);
    }

    if (IsDirsFirst) {
        DirListing2 = LL_create();
        if (DirListing2 == NULL) {
            perror("Failed to create LinkedList");
            exit(1);
        }
    }

    memset(PrevZippedFileName, 0, MAX_FILE_NAME);

    traceOut();
}

void deinitialize() {
    traceIn();
    MA_destroy(MemAlloc);
    LL_destroy(DirListing);
    if (IsDirsFirst) {
        LL_destroy(DirListing2);
    }
    traceOut();
}

int doListing(const char* zipFileName) {
    traceIn();

    int retCode = walkThruZip(zipFileName);

    if (retCode != 0) {
        traceOut(retCode);
    }
        
    trace("Sorting (size %d)", LL_size(DirListing));
    LL_qsort(DirListing, comparator, 0, LL_size(DirListing) - 1);
    if (IsDirsFirst) {
        trace("Sorting #2 (size %d)", LL_size(DirListing2));
        LL_qsort(DirListing2, comparator, 0, LL_size(DirListing2) - 1);
    }
        
    trace("Removing duplicates (size %d)", LL_size(DirListing));
    LL_uniq(DirListing, comparator);
    if (IsDirsFirst) {
        trace("Removing duplicates #2 (size %d)", LL_size(DirListing2));
        LL_uniq(DirListing2, comparator);
    }
        
    // with option 'dirs first' we need to show directories first
    if (IsDirsFirst) {
        trace("Producing listing output #2 (size %d)", LL_size(DirListing2));
        LL_for_each(DirListing2, print);
    }
    trace("Producing listing output (size %d)", LL_size(DirListing));
    LL_for_each(DirListing, print);

    traceOut(retCode);
}

int main(int argc, char* argv[])
{
    const char* zipFileName = NULL;
    int retCode = 0;

    // Zip file directory name to list. Empty string means root directory.
    LsDirName = "";

    LsFileName = "";

    if (argc <= 1) {
        showUsageBanner();
        exit(0);
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
            case 'v':
                showVersionBanner();
                exit(0);
            case 'l':
                IsLongListing = 1;
                break;
            case '2':
                trace("dirs first");
                IsDirsFirst = 1;
                break;
            case 'h':
            default:
                showUsageBanner();
                exit(0);
            }
        } else {
            if (zipFileName == NULL) {
                zipFileName = argv[i];
            } else {
                if (argv[i][0] == ZIP_FILESEP_CHAR) {
                    argv[i]++;
                }
                char* p = strrchr(argv[i], ZIP_FILESEP_CHAR);
                if (p == NULL) {
                    LsFileName = argv[i];
                } else {
                    *p = '\0';
                    LsDirName = argv[i];
                    LsFileName = p + 1;
                }
            }
        }
    }
    
    trace("zip file: %s", zipFileName);
    trace("zip file directory to list: %s", LsDirName);
    trace("zip file to list: %s", LsFileName);

    initialize();
    retCode = doListing(zipFileName);
    deinitialize();

    exit(retCode);
}


