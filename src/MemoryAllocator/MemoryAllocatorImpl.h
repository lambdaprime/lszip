/* 
 * This source file is a part of MemoryAllocator routine.
 *
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

#ifndef MEMORYALLOCATORIMPL_H
#define MEMORYALLOCATORIMPL_H

#include "MemoryAllocator.h"

struct _MA_chunk {
    void* ptr;
    int usedSize;
    int maxSize;
};

typedef struct _MA_chunk* MA_chunk;

struct _MA_allocator {
    MA_chunk* arrayOfChunks;
    int numberOfUsedChunks;
    int maxNumberOfChunks;
    int sizeOfChunks;
};

#endif
