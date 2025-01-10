/* Skeleton for W4.6, comp20003 workshop Week 4 modified for use in assignment 1 */

/*-------------------------------------------------------------- 
..Project: Assignment 1
  list.h :  
          = the inmplementation of module linkedList of the project

  NOTES:
        - this module is polymorphic
        - the data component in a list node is of type void*
----------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

#include "suburbs.h"
#include "list.h"

// generous functions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// creates & returns an empty linked list
list_t *listCreate() {
	list_t *list = malloc(sizeof(*list));
	assert(list);
	list->head = list->tail = NULL;
	list->n = 0;
	return list;
}

// free the list
/* free all memory used by a list, including used by data component
    requires argument dataFree which is a function
    the parameter can be declared as function pointer, as broken down belows:
           (*dataFree)           : dataSize is a pointer to
           (*dataFree)(void *)   :  a function that has "void *" as argument
      void (*dataFree)(void *)   :  and that returns void
    (the above break-down is application of the right-left rule, 
      see https://cseweb.ucsd.edu/~gbournou/CSE131/rt_lt.rule.html)
*/
void listFree(list_t *list, void (*dataFree)(void *)){
	assert(list != NULL);
	node_t *curr = list->head; // curr points to the first node
    
    while (curr) {             // while curr not reaching the end of list
		node_t *tmp = curr;
		curr = curr->next;     // advance curr to the next next node 
		dataFree(tmp->data);       // frees the previous node including
		free(tmp);             //    freeing the space used by data
	}

	free(list);                // free the list itself
}

// returns 1 if the list is empty, 0 otherwise
int listIsEmpty(list_t *list) {
	assert(list);
	return list->head == NULL;
}

// returns the number of elements in list
int listCount(list_t *list){
	assert(list);
	return list->n;
}

// function for adding nodes to the list >>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// inserts a new node with value "data" to the end of "list" 
void listAppend(list_t *list, record_t *data) {
	assert(list);

	// creates a new node and set data 
	node_t *new = malloc(sizeof(*new));
	assert(new);
	new->data = data;
	new->next = NULL;

	// check if the list is empty
	if (list->head == NULL) {
		// if so, make the new node the head
		list->head = new;
		list->tail = new;
	} else {
		// traverse to the end of the list to append
		list->tail->next = new;
		list->tail = new;
	}
	// updates the number of elements in the list
	(list->n)++;
}

// functions for processing over the whole list >>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// linear search with suburb name as key, returns list of found records
list_t *listSearch(list_t *list, char *key, char *(*dataGetKey)(void *), search_stats_t *stats){
    list_t *resultList = listCreate();

    assert(list);
    if (list->head == NULL) return resultList;

    for (node_t *p = list->head; p; p = p->next) {    
        stats->nodeAccesses++;  // Increment node accesses
        stats->stringComparisons++; // Increment string comparisons
        
        char *recordKey = dataGetKey(p->data);
        int bitComparisonsForThisKey = 0; // For the null byte
        
        // Compare each character and calculate bit comparisons
        for (int i = 0; i <= strlen(key); i++) {
            bitComparisonsForThisKey += BITS_PER_BYTE; // Each character comparison adds 8 bits
            
            if (key[i] != recordKey[i]) {
                break; // Stop comparing if characters mismatch
            }
        }
        stats->bitComparisons += bitComparisonsForThisKey; 
        
        int cmpResult = strcmp(recordKey, key);
        if (cmpResult == 0) {
            record_t *recordCopy = copyRecord(p->data);
            if (recordCopy) {
                // Add the copy to resultList instead of the original
                listAppend(resultList, recordCopy); 
            }
        }  
    }
    return resultList;
}


// Remove all matching records of a suburb from the list
int listRemove(list_t *list, char *key, char *(*dataGetKey)(void *),
                void (*dataFree)(void *)) {
    assert(list);
    
	int removedCount = 0;
    
	node_t *curr = list->head;
    node_t *prev = NULL;

    while (curr != NULL) {
        // Use linear search, then delete all the matches
        if (strcmp(dataGetKey(curr->data), key) == 0) { 
            node_t *tmp = curr;

			removedCount++;

            // If the record is the first one in the list, update head
            if (prev == NULL) {   
                list->head = curr->next;
            } else {              // Otherwise, skip the record
                prev->next = curr->next; 
            }

            // Check if we are at the end of the list
            if (curr->next == NULL) { 
                list->tail = prev;
            }

            // Finally, we skip the current record
            curr = curr->next; 
            dataFree(tmp->data);
            free(tmp);
            list->n--;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
	return removedCount;
}


/* =====================================================================
   The original skeleton/program was compiled by the comp20003 teaching team,
   the university of Melbourne on 15/07/2022. 
   The current version was modified and compiled by Nick Ho and Kerui
   Huang.
   Last Updated: 04/09/2024
   ================================================================== */
