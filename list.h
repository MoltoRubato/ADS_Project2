/* Skeleton for W4.6, comp20003 workshop Week 4 modified for use in assignment 1 */

/*-------------------------------------------------------------- 
..Project: Assignment 1
  list.h :  
          = the interface of module linkedList of the project

  NOTES:
        - this module is polymorphic
        - the data component in a list node is of type record_t
----------------------------------------------------------------*/
#ifndef _LIST_H_
#define _LIST_H_

// type definitions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef struct node node_t;
struct node {
	record_t *data;          // points to the data element of the node
	node_t *next;            // points to the next node in the list
};

// a linked list is defined as a couple of pointers
struct list {
	node_t *head;  // points to the first node of the list
	node_t *tail;  // points to the last node of the list 
	size_t n;               // number of element in the list
};
typedef struct list list_t;

/* Number of bits in a single character. */
#define BITS_PER_BYTE 8

// generous functions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// creates & returns an empty linked list
list_t *listCreate();

// free the list, with the help of function "freeData"
void listFree(list_t *list, void (*dataFree)(void *));

// returns 1 if the list is empty, 0 otherwise
int listIsEmpty(list_t *list); 

// returns the number of elements in list
int listCount(list_t *list);

// function for adding nodes to the list >>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// inserts a new node with value "data" to the end of "list" 
void listAppend(list_t *list, record_t *data); 

// functions for processing over the whole list >>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// linear search with suburb name as key, returns list of found records
list_t *listSearch(list_t *list, char *key, char *(*dataGetKey)(void *), search_stats_t *stats);


// Remove all matching records of a suburb from the list
int listRemove(list_t *list, char *key, char *(*dataGetKey)(void *), 
                void (*dataFree)(void *));

#endif


/* =====================================================================
   The original skeleton/program was compiled by the comp20003 teaching team,
   the university of Melbourne on 15/07/2022. 
   The current version has beened modified and compiled by Nick Ho and Kerui
   Huang.
   Last Updated: 04/09/2024
   ================================================================== */

