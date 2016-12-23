/* 
 * LinkedList is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LinkedList is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LinkedList. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "LinkedList.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

int comparator(const void *txt1, const void *txt2) {
    return strcmp((char *)txt1, (char *)txt2);
}

void print(void *txt) {
    puts((char *)txt);
}

void testEmptyLinkedList() {
    puts("Testing empty list ");
    LinkedList list = LL_create();
    assert(LL_size(list) == 0);
    LL_destroy(list);
}

void printList(void* str) {
    printf("%s\n", str);
}

void testOperations() {
    puts("Testing simple list operations");
    LinkedList list = LL_create();
    LL_append(list, "one");
    LL_append(list, "two");
    LL_append(list, "three");
    LL_prepend(list, "one");
    LL_prepend(list, "two");
    LL_prepend(list, "three");
    assert(strcmp("three", (char *)LL_at(list, 0)) == 0);
    assert(strcmp("two", (char *)LL_at(list, 1)) == 0);
    assert(strcmp("one", (char *)LL_at(list, 2)) == 0);
    assert(strcmp("one", (char *)LL_at(list, 3)) == 0);
    assert(strcmp("two", (char *)LL_at(list, 4)) == 0);
    assert(strcmp("three", (char *)LL_at(list, 5)) == 0);
    LL_remove(list, 1);
    assert(strcmp("one", (char *)LL_at(list, 1)) == 0);
    assert(LL_size(list) == 5);
    LL_for_each(list, printList);
    LL_destroy(list);
}

void testUniq() {
    puts("Testing uniq function");
    LinkedList list = LL_create();
    int numberOfWords = 29;
    char *words[29] = { 
        "reveal",       "redundant",    " selfish",
        "  bump",       "selfish",      "bump",
        "paraphrase",   "headline",     "premature",
        "premature",    "conduct",      "premature",
        "equally",      " selfish",     "paraphrase",
        "paraphrase",   "bump ",        "conduct",
        "premature",    "treat",        "conduct",
        "emphases",     "premature",    "negoatiate",
        "conduct",      "Projects/",    "Projects/",
        "Projects/",    "Projects/"
    };

    for (int i = 0; i < numberOfWords; i++) {
        LL_append(list, words[i]);
    }

    assert(LL_size(list) == numberOfWords);

    LL_qsort(list, comparator, 0, LL_size(list) - 1);
    LL_uniq(list, comparator);

    assert(LL_size(list) == numberOfWords - 13);

    LL_destroy(list);
}

void testQSort() {
    puts("Testing quicksort function");
    LinkedList list = LL_create();
    const int numberOfWords = 9;
    char *words[9] = {
        "asdfd);", "tabloid", "elapid", "venomous", "curtain", "applicable",
        "applicable", "innovative", "nest"
    };
    int sortedPositions[] = {5, 6, 0, 4, 2, 7, 8, 1, 3};

    for (int i = 0; i < numberOfWords; i++) {
        LL_append(list, words[i]);
    }

    LL_qsort(list, comparator, 0, LL_size(list) - 1);
    
    for (int i = 0; i < numberOfWords; i++) {
        printf("current word number is: %d\n", i);
        assert(strcmp(words[sortedPositions[i]], (char *)LL_at(list, i)) == 0);    
    }

    LL_destroy(list);
}

int main() {
    testOperations();
    testQSort();
    testEmptyLinkedList();
    testUniq();
    puts("All tests was successfully passed");
    while(1);
}
