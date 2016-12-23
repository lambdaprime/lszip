/* 
 * MemoryAllocator helps to save the efforts of allocating each time 
 * chunks of small capacity. It allocates one huge chunk of memory in heap 
 * in one operation (its size should be specified at the moment of 
 * MemoryAllocator construction through MA_create function) and saves pointer
 * to it in its internal array of pointers. 
 * When you request MemoryAllocator for portion of memory, it allocates it
 * in its internal chunk. If internal chunk become out of space, 
 * MemoryAllocator will allocate another internal chunk and add it to the 
 * array.
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

#ifndef MEMORYALLOCATOR_H
#define MEMODYALLOCATOR_H

struct _MA_allocator;
typedef struct _MA_allocator* MA_allocator;

MA_allocator MA_create(int sizeOfChunks);
void* MA_allocate(MA_allocator alloc, int size);
void MA_destroy(MA_allocator alloc);

#endif
