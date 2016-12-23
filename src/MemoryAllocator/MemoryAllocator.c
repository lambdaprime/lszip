/* 
 * MemoryAllocator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MemoryAllocator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MemoryAllocator. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "MemoryAllocator.h"
#include "MemoryAllocatorImpl.h"
#include <memory.h>
#include <stdlib.h>

#define INITIAL_NUMBER_OF_CHUNKS 100

static MA_chunk newChunk(MA_allocator alloc);

MA_allocator MA_create(int sizeOfChunks) {
    MA_allocator alloc = (MA_allocator)calloc(1, sizeof(struct _MA_allocator));
    if (alloc == NULL) {
        return NULL;
    }

    alloc->arrayOfChunks = (MA_chunk*)calloc(INITIAL_NUMBER_OF_CHUNKS, sizeof(struct _MA_chunk)); 
    if (alloc->arrayOfChunks == NULL) {
        return NULL;
    }

    alloc->maxNumberOfChunks = INITIAL_NUMBER_OF_CHUNKS ;
    alloc->numberOfUsedChunks = 0;
    alloc->sizeOfChunks = sizeOfChunks;

    if (newChunk(alloc) == NULL) {
        return NULL;
    }

    return alloc;
}

static MA_chunk newChunk(MA_allocator alloc) {
    
    MA_chunk chunk = (MA_chunk)calloc(1, sizeof(struct _MA_chunk));
    if (chunk == NULL) {
        return NULL;
    }
    
    chunk->ptr = malloc(alloc->sizeOfChunks);
    if (chunk->ptr == NULL) {
        return NULL;
    }

    chunk->maxSize = alloc->sizeOfChunks;
    chunk->usedSize = 0;

    memset(chunk->ptr, 0, chunk->maxSize);

    if (alloc->numberOfUsedChunks == alloc->maxNumberOfChunks) {
        int newMaxNumber = alloc->maxNumberOfChunks * 2;
        int newMaxSize = newMaxNumber* sizeof(MA_chunk);
        MA_chunk* tmp = (MA_chunk*)realloc(alloc->arrayOfChunks, newMaxSize);
        if (tmp == NULL) {
            return NULL;
        }
        alloc->arrayOfChunks = tmp;
        alloc->maxNumberOfChunks = newMaxNumber;
    }

    alloc->arrayOfChunks[alloc->numberOfUsedChunks++] = chunk;
    return chunk;

}

void* MA_allocate(MA_allocator alloc, int size) {
    MA_chunk chunk = alloc->arrayOfChunks[alloc->numberOfUsedChunks - 1];
    int oldSize = chunk->usedSize;
    int newSize = chunk->usedSize + size;
    
    if (newSize > chunk->maxSize) {
        chunk = newChunk(alloc);
        if (chunk == NULL || size > chunk->maxSize) {
            return NULL;
        }
        oldSize = 0;
        newSize = size;
    }

    chunk->usedSize = newSize;
    return (char*)chunk->ptr + oldSize;
}

void MA_destroy(MA_allocator alloc) {
    for (int i = 0; i < alloc->numberOfUsedChunks; i++) {
        free(alloc->arrayOfChunks[i]->ptr);
    }
    free(alloc->arrayOfChunks);
    free(alloc);
}
