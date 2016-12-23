/* 
 * Tests for MemoryAllocator.
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

#include "MemoryAllocator.h"
#include "MemoryAllocatorImpl.h"
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct {
    char* string1;
    char* string2;
    char* string3;
} test_struct_1;

typedef struct {
    int int1;
    int int2;
    int int3;
} test_struct_2;

void testSimpleAllocation() {
    MA_allocator alloc = MA_create(30);

    char* string = (char*)MA_allocate(alloc, 20);
    memset(string, 'A', 19);
    
    assert(strcmp(string, "AAAAAAAAAAAAAAAAAAA") == 0);
    MA_destroy(alloc);
}

void testTooLongAllocation() {
    MA_allocator alloc = MA_create(10);

    char* string = (char*)MA_allocate(alloc, 20);
    assert(string == NULL);
}

void testChunkAndChunkArrayOverflow() {
    MA_allocator alloc = MA_create(10);
    int c = 350;
    char *strings[350];

    while (c--) {
        char* string = (char*)MA_allocate(alloc, 8);
        memset(string, 'A', 7);
        strings[c] = string;
    }

    for (int i = 0; i < 350; i++) {
        assert(strcmp(strings[i], "AAAAAAA") == 0);
    }
    
    assert(alloc->numberOfUsedChunks == 350);
    MA_destroy(alloc);
}

void testFullChunk() {
    MA_allocator alloc = MA_create(1);
    char* string = (char *)MA_allocate(alloc, 1);
    string[0] = 'A';
    assert(string[0] == 'A');
    MA_destroy(alloc);

    struct sampleStruct {
        int a;
        int b;
    };
    struct sampleStruct *s;
    alloc = MA_create(sizeof(struct sampleStruct));
    s = (struct sampleStruct*)MA_allocate(alloc, sizeof(struct sampleStruct));
    s->a = 31337;
    s->b = 12007;
    assert(s->a == 31337);
    assert(s->b == 12007);
    MA_destroy(alloc);
}

void testSeveralChunks() {
    MA_allocator alloc1 = MA_create(30);
    MA_allocator alloc2 = MA_create(17);

    char* string1 = (char*)MA_allocate(alloc1, 20);
    char* string2 = (char*)MA_allocate(alloc2, 17);
    
    memset(string1, 'A', 19);
    memset(string2, 'B', 16);

    assert(strcmp(string1, "AAAAAAAAAAAAAAAAAAA") == 0);
    assert(strcmp(string2, "BBBBBBBBBBBBBBBB") == 0);
    MA_destroy(alloc1);
    MA_destroy(alloc2);
}

int main() {
    testSimpleAllocation();
    testTooLongAllocation();
    testChunkAndChunkArrayOverflow();
    testFullChunk();
    testSeveralChunks();
    puts("All tests was successfully passed");
    while(1);
    return 0;
}
