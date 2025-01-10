/*-------------------------------------------------------------- 
..Project: Assignment 2
  tree.h :  
          = the interface of module patriciaTree of the project
----------------------------------------------------------------*/

#ifndef _TREE_H_
#define _TREE_H_

// type definitions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef struct PatriciaNode {
    int prefixBits;
    char* prefix;

    struct PatriciaNode* branchA;
    struct PatriciaNode* branchB;
    record_t record;
} PatriciaNode;

typedef struct SearchResult {
    char* bestMatch;
    int bestDistance;
    record_t record;
} SearchResult;

/* Number of bits in a single character. */
#define BITS_PER_BYTE 8


// char* stringToBinary(const char* str);

/* Implement insertion for Patricia Tree */
PatriciaNode* createPatriciaNode(record_t* record, int prefixBits, 
                                    char* prefix);
PatriciaNode* insertPatricia(PatriciaNode* root, record_t* record);
SearchResult* createSearchResult(char* match, int distance, 
                                    record_t record);
void collectAllDescendants(PatriciaNode* node, char** results, 
                                record_t* records, int* count);
SearchResult* searchPatricia(PatriciaNode* root, char* query, 
                                search_stats_t* stats);

void freePatriciaNode(PatriciaNode* node, void (*dataFree)(void *));
void freeSearchResult(SearchResult* result);

int getBit(char *s, unsigned int bitIndex);
char *createStem(char *oldKey, unsigned int startBit, unsigned int numBits);

int editDistance(char *str1, char *str2, int n, int m);
int min(int a, int b, int c);

#endif


/* =====================================================================
   This skeleton/program is compiled by Kerui Huang.
   Last Updated: 06/09/2024
   ================================================================== */
