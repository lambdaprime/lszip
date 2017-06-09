/* 
 * This source file provides LinkedList implementation.
 *
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
#include <memory.h>
#include <stdlib.h>

//struct _ListItem;
typedef struct _ListItem* ListItem;

struct _ListItem {
    void *data;
    ListItem next;
    ListItem prev;
};

struct _LinkedList {
    ListItem begin;
    ListItem end;
    int size;
};

const int sizeof_ListItem = sizeof(struct _ListItem);
const int sizeof_LinkedList = sizeof(struct _LinkedList);

static ListItem at(const LinkedList linkedList, int index);
static void swap(ListItem li1, ListItem li2);
static void remove(LinkedList linkedList, ListItem li);

LinkedList LL_create() {
    LinkedList linkedList = (LinkedList)calloc(1, sizeof_LinkedList);
    if (linkedList == NULL) {
        return NULL;
    }
    memset(linkedList, 0, sizeof_LinkedList);
    return linkedList;
}

void LL_destroy(LinkedList linkedList) {
    if (linkedList->begin != NULL) {
        for (ListItem li = linkedList->begin->next; 
            li != NULL;
            li = li->next) {
            free(li->prev);
        }
        if (linkedList->end != NULL) {
            free(linkedList->end);
        }
    }
    free(linkedList);
}

int LL_size(LinkedList linkedList) {
    return linkedList->size;
}

void LL_append(LinkedList linkedList, void* node) {
    ListItem li = (ListItem)calloc(1, sizeof_ListItem);
    if (li == NULL) {
        return;
    }

    li->data = node;
    if (linkedList->begin == NULL) {
        linkedList->begin = li;
        linkedList->end = li;
        li->prev = NULL;
        li->next = NULL;
    } else {
        li->prev = linkedList->end;
        li->next = NULL;
        linkedList->end->next = li;
        linkedList->end = li; 
    }

    linkedList->size++;
}

void LL_prepend(LinkedList linkedList, void* node) {
    ListItem li = (ListItem)calloc(1, sizeof_ListItem);
    if (li == NULL) {
        return;
    }

    li->data = node;
    if (linkedList->begin == NULL) {
        linkedList->begin = li;
        linkedList->end = li;
        li->prev = NULL;
        li->next = NULL;
    } else {
        li->prev = NULL;
        li->next = linkedList->begin;
        linkedList->begin->prev = li;
        linkedList->begin = li; 
    }

    linkedList->size++;
}

void LL_for_each(const LinkedList linkedList, void (*f)(void*)) {

    for (ListItem li = linkedList->begin; 
         li != NULL;
         li = li->next) {
        f(li->data);
    }
}

void* LL_at(const LinkedList linkedList, int index) {
    return at(linkedList, index)->data;
}

static ListItem at(const LinkedList linkedList, int index) {
    ListItem li;

    if (index >= linkedList->size) {
        return NULL;
    }

    li = linkedList->begin;
    while (index-- > 0) {
         li = li->next;
    }
    return li;
}

void swap(ListItem li1, ListItem li2) {
    void* tmp = li1->data;
    li1->data = li2->data;
    li2->data = tmp;
}

int partition(const LinkedList linkedList, int (*comparator)(const void* a, const void* b), 
              int lo, int hi)
{
    int i = lo;
    int j = hi + 1;
    int pivot = lo;

    while (1) {
        while (comparator(LL_at(linkedList, ++i), 
                           LL_at(linkedList, pivot)) < 0)
            if (i == hi) break;
        while (comparator(LL_at(linkedList, --j),
                           LL_at(linkedList, pivot)) > 0)
            if (j == lo) break;
        if (i >= j) break;
        swap(at(linkedList, i), at(linkedList, j));
    }

    swap(at(linkedList, j), at(linkedList, lo));
    
    return j;
}

void LL_qsort(const LinkedList linkedList, 
              int (*comparator)(const void* a, const void* b),
              int startIndex, int endIndex) 
{

    if (endIndex <= startIndex) {
        return;
    }
    
    int pivot = (startIndex + endIndex) / 2;
    int newPivot = partition(linkedList, comparator, startIndex, endIndex);

    LL_qsort(linkedList, comparator, startIndex, newPivot - 1);
    LL_qsort(linkedList, comparator, newPivot + 1, endIndex);

}

void LL_remove(LinkedList linkedList, int index) {
    remove(linkedList, at(linkedList, index));
}

static void remove(LinkedList linkedList, ListItem li) {
    if (li->prev == NULL) {
        linkedList->begin = li->next;

    } else if (li->next == NULL) {
        linkedList->end = li->prev;
        linkedList->end->next = NULL;
    } else {
        li->prev->next = li->next;
        li->next->prev = li->prev;
    }
    free(li);
    linkedList->size--;
}

void LL_uniq(LinkedList linkedList, int (*comparator)(const void* a, const void *b)) {
    if (linkedList->size < 1) {
        return ;
    }

    ListItem li = linkedList->begin;
    while (li->next != NULL) {
        if (comparator(li->data, li->next->data) == 0) {
            remove(linkedList, li->next);
        } else {
            li = li->next;
        }
    }
}
