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

#ifndef LINDEKLIST_H
#define LINDEKLIST_H

typedef struct _LinkedList* LinkedList;

/// Creates linked list
LinkedList LL_create();

/// Destroys linked list
void LL_destroy(LinkedList linkedList);

/// Appends new node to the linked list
void LL_append(LinkedList linkedList, void* node);

/// Prepends new node to the linked list
void LL_prepend(LinkedList linkedList, void* node);

/// Removes node at specified index
void LL_remove(LinkedList linkedList, int index);

/// Returns number of nodes in the linked list
int LL_size(LinkedList linkedList);

/**
 * Runs function for every node in the linked list, 
 * passing node as an argument to it.
 */
void LL_for_each(const LinkedList linkedList, void (*f)(void*));

/// Return node with specified index
void* LL_at(const LinkedList linkedList, int index);

/// Does binary sorting
void LL_qsort(const LinkedList linkedList, 
              int (*comparator)(const void* a, const void* b),
              int startIndex, 
              int endIndex);

/// Removes contiguous equal items
void LL_uniq(LinkedList linkedList, 
             int (*comparator)(const void* a, const void* b));

#endif
