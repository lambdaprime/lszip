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
    LL_for_each(list, print);
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

void testQSort_(char* words[], int* sortedPositions, int numOfWords) {
    puts("Testing quicksort function");
    LinkedList list = LL_create();

    for (int i = 0; i < numOfWords; i++) {
        LL_append(list, words[i]);
    }

    LL_qsort(list, comparator, 0, LL_size(list) - 1);

    LL_for_each(list, print);
    
    for (int i = 0; i < numOfWords; i++) {
        printf("current word number is: %d\n", i);
        assert(strcmp(words[sortedPositions[i]], (char *)LL_at(list, i)) == 0);    
    }

    LL_destroy(list);
}

void testQSort() {
    testQSort_(
        (char*[]){
            "asdfd);", "tabloid", "elapid", "venomous", "curtain", "applicable",
            "applicable", "innovative", "nest"
        }, 
        (int[]){5, 6, 0, 4, 2, 7, 8, 1, 3}, 
        9);
    testQSort_(
        (char*[]){"1", "2", "5", "3", "4"}, 
        (int[]){0, 1, 3, 4, 2}, 
        5);
    testQSort_(
        (char*[]){"4", "2", "3", "1"}, 
        (int[]){3, 1, 2, 0}, 
        4);
    testQSort_(
        (char*[]){"1", "2", "1", "1"}, 
        (int[]){0, 2, 3, 1}, 
        4);
    testQSort_(
               (char*[]){"1", "2", "1", "1", "2", "1", "2", "2", "1", "1", "2"}, 
               (int[]){0, 2, 3, 5, 8, 9, 1, 4, 6, 7, 10}, 
        11);
}

int main() {
    testOperations();
    testQSort();
    testEmptyLinkedList();
    testUniq();
    puts("All tests was successfully passed");
}
