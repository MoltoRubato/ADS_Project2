/*-------------------------------------------------------------- 
..Project: Assignment 2
  tree.h :  
          = Implementation of module patriciaTree of the project
----------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#include "suburbs.h"
#include "tree.h"

/* Takes the relevant prefix, its length, and record, 
returns a patricia tree node */
PatriciaNode* createPatriciaNode(record_t* record, 
                                    int prefixBits, char* prefix) {
    
    PatriciaNode* node = (PatriciaNode*)malloc(sizeof(PatriciaNode));
    assert(node);

    node->record = *record;
    node->branchA = NULL;
    node->branchB = NULL;
    node->prefixBits = prefixBits;
    node->prefix = prefix;
    return node;
}

/* Takes a root of a patricia tree and a record, inserts the record 
into the trie, returns the root patricia tree node */
PatriciaNode* insertPatricia(PatriciaNode* root, record_t* record) {
    if (root == NULL) {
        // If the tree is empty, create a new root node
        int len = strlen(record->officialNameSuburb) * BITS_PER_BYTE;
        char *stem = createStem(
                        record->officialNameSuburb, 0, 
                        len);

        return createPatriciaNode(record, len, stem);
    }

    PatriciaNode* current = root;
    PatriciaNode* parent = NULL;
    unsigned int bitIndex = 0;
    char* key = record->officialNameSuburb;
    unsigned int keyBitLength = 
                        strlen(record->officialNameSuburb) * BITS_PER_BYTE;

    while (current != NULL) {
        char* currPrefix = current->prefix;
        unsigned int currLength = current->prefixBits;
        unsigned int stemLength = 0;

        // Compare bits from the stem's key with the new key
        while ((stemLength < (currLength)) 
                && (stemLength < (keyBitLength)) 
                && (getBit(key, stemLength+bitIndex) 
                    == getBit(currPrefix, stemLength))) {
            stemLength++; // Keep going until the bits are different
        }

        if (stemLength == currLength) {
            // Current node key matches the new key or has common prefix
            if (current->branchA == NULL && current->branchB == NULL) {
                // Identical Leaf node
                return root;
            } else {
                // Internal node, traverse down
                parent = current;
                if(getBit(key, stemLength+bitIndex) == 0){
                    current = current->branchA;
                }else{
                    current = current->branchB;
                }
                bitIndex += stemLength;
            }
        } else {
            // Mismatch found, split the node
            unsigned int mismatchPos = bitIndex + stemLength;

            char *stem = createStem(currPrefix, 0, stemLength);

            // This is the common stem
            char* oldPrefix = current->prefix;
            // Update it so the current prefixs starts from mismatch
            current->prefix = createStem(currPrefix, stemLength, 
                                            current->prefixBits 
                                                - stemLength);
            current->prefixBits -= stemLength;
            
            // Free old prefix memory
            if (oldPrefix != NULL) {
                free(oldPrefix);
            }
            
            PatriciaNode* newNode = createPatriciaNode(record, 
                                        keyBitLength - stemLength, 
                                        createStem(key, mismatchPos, 
                                            keyBitLength - stemLength));     

            PatriciaNode* newInternalNode = 
                    (PatriciaNode*)malloc(sizeof(PatriciaNode));
            assert(newInternalNode);
            newInternalNode->prefix = stem;
            newInternalNode->prefixBits = stemLength;
            newInternalNode->branchA = NULL;
            newInternalNode->branchB = NULL;

            // Assign children based on the mismatch bit        
            if (getBit(record->officialNameSuburb, mismatchPos) == 0) {
                newInternalNode->branchA = newNode;
                newInternalNode->branchB = current;
            } else {
                newInternalNode->branchA = current;
                newInternalNode->branchB = newNode;
            }

            // If there is no parent, update the root
            if (parent == NULL) {
                return newInternalNode; // The internal node is the root now
            } else {
                 if (getBit(newInternalNode->prefix, 0) == 0) {
                    parent->branchA = newInternalNode;
                } else {
                    parent->branchB = newInternalNode;
                }
                return root;
            }
        }
    }
    return root;
}

/* Take in the results compared keys, edit distance and the record data. 
Returns a struct for the result and the distance */
SearchResult* createSearchResult(char* match, int distance, record_t record) {
    SearchResult* result = (SearchResult*)malloc(sizeof(SearchResult));
    result->bestMatch = match;
    result->bestDistance = distance;
    result->record = record;
    return result;
}

/* Helper function to recursively collect all descendant data from the node */
void collectAllDescendants(PatriciaNode* node, char** results, 
                            record_t* records, int* count) {
    if (node == NULL) {
        return;
    }

    if (node->branchA == NULL && node->branchB == NULL) {
        // Leaf node, add the record's key and the record itself
        results[*count] = node->record.officialNameSuburb;
        records[*count] = node->record;  // Store the record
        (*count)++;
    } else {
        collectAllDescendants(node->branchA, results, records, count);
        collectAllDescendants(node->branchB, results, records, count);
    }
}

/* Searches the patricia tree for a given query. Count the 
operations and comparisons made. Return the search result 
containing the searched record or the closest matching key's record */
SearchResult* searchPatricia(PatriciaNode* root, char* query, 
                                search_stats_t* stats) {
    if (root == NULL) {
        record_t emptyRecord = {0};

        return createSearchResult(NULL, -1, emptyRecord);
    }

    PatriciaNode* current = root;
    unsigned int bitIndex = 0;
    unsigned int queryBitLength = strlen(query) * BITS_PER_BYTE;
    unsigned int mismatchPos = 0;

    // Traverse the tree
    while (current != NULL) {
        stats->nodeAccesses++;  // Track node accesses

        char* currPrefix = current->prefix;
        unsigned int currLength = current->prefixBits;
        unsigned int stemLength = 0;

        // Compare bits from the prefix with the query
        while (stemLength < currLength && 
                stemLength + bitIndex < queryBitLength &&
                getBit(query, stemLength + bitIndex) == 
                    getBit(currPrefix, stemLength)) {
            stats->bitComparisons++;
            stemLength++;
        }

        // Check if a mismatch occurred
        if (stemLength != currLength) {
            mismatchPos = bitIndex + stemLength;
            break;
        }

        // Move to the next node
        if (current->branchA == NULL && current->branchB == NULL) {
            // We've reached a leaf node
            break;
        } else {
            if(getBit(query, stemLength + bitIndex) == 0){
                current = current->branchA; 
            }else{
                current = current->branchB;
            }
            bitIndex += stemLength;
        }
    }

    // Collect all descendants at the mismatch 
    char* allResults[1000]; // Assume max 1000 descendants
    record_t allRecords[1000]; // Array to hold the records
    int count = 0;
    collectAllDescendants(current, allResults, allRecords, &count);

    // Find the closest match based on edit distance
    SearchResult* closestMatch = createSearchResult(NULL, INT_MAX, 
                                                    current->record);
    stats->stringComparisons++;

    // Using edit distance for spellchecker
    for (int i = 0; i < count; i++) {
        int dist = editDistance(query, allResults[i], 
                                strlen(query), strlen(allResults[i]));

        if (dist < closestMatch->bestDistance || 
           (dist == closestMatch->bestDistance && 
            strcmp(allResults[i], closestMatch->bestMatch) < 0)) {
            closestMatch->bestMatch = allResults[i];
            closestMatch->bestDistance = dist;
            closestMatch->record = allRecords[i];  
            // Store the record of the closest match
        }
    }

    if (closestMatch->bestDistance == 0 && count <= 1) {
        // To account for null byte at the end of leaf node
        stats->bitComparisons += BITS_PER_BYTE;  
    } else{
        // to account for the mismatched bit
        stats->bitComparisons++;    
    }

    return closestMatch;
}

// Frees each node of the patricia tree
void freePatriciaNode(PatriciaNode* node, void (*dataFree)(void *)) {
    if (node == NULL) {
        return;
    }

    // Recursively free child nodes
    freePatriciaNode(node->branchA, *dataFree);
    freePatriciaNode(node->branchB, *dataFree);

    // Free the node's dynamically allocated prefix
    if (node->prefix != NULL) {
        free(node->prefix);
    }

    // // Free the node's records
    // if (node->record.officialNameSuburb != NULL) {
    //     dataFree(&(node->record));
    // }


    // Free the node itself
    free(node);
}

// Frees the search result of the patricia tree
void freeSearchResult(SearchResult* result) {
    if (result == NULL) {
        return;
    }

    // If `bestMatch` is dynamically allocated, free it
    if (result->bestMatch != NULL) {
        free(result->bestMatch);
    }

    // Free the result structure itself
    free(result);
}


/* Helper function. Gets the bit at bitIndex from the string s. */
int getBit(char *s, unsigned int bitIndex){
    assert(s && bitIndex >= 0);
    unsigned int byte = bitIndex / BITS_PER_BYTE;
    unsigned int indexFrombranchA = bitIndex % BITS_PER_BYTE;
    /* 
        Since we split from the highest order bit first, 
        the bit we are interested will be the highest order 
        bit, rather than a bit that occurs at the end of the
        number. 
    */
    unsigned int offset = 
        (BITS_PER_BYTE - (indexFrombranchA) - 1) % BITS_PER_BYTE;
    unsigned char byteOfInterest = s[byte];
    unsigned int offsetMask = (1 << offset);
    unsigned int maskedByte = (byteOfInterest & offsetMask);
    /*
        The masked byte will still have the bit in its original 
        position, to return either 0 or 1, we need to move the 
        bit to the lowest order bit in the number.
    */
    unsigned int bitOnly = maskedByte >> offset;
    return bitOnly;
}

/* Allocates new memory to hold the numBits specified and fills the allocated
    memory with the numBits specified starting from the startBit of the oldKey
    array of bytes. */
char *createStem(char *oldKey, unsigned int startBit, unsigned int numBits){
    assert(numBits > 0 && startBit >= 0 && oldKey);
    int extraBytes = 0;
    if((numBits % BITS_PER_BYTE) > 0){
        extraBytes = 1;
    }
    int totalBytes = (numBits / BITS_PER_BYTE) + extraBytes;
    char *newStem = malloc(sizeof(char) * totalBytes);
    assert(newStem);
    for(unsigned int i = 0; i < totalBytes; i++){
        newStem[i] = 0;
    }
    for(unsigned int i = 0; i < numBits; i++){
        unsigned int indexFrombranchA = i % BITS_PER_BYTE;
        unsigned int offset = 
            (BITS_PER_BYTE - indexFrombranchA - 1) % BITS_PER_BYTE;
        unsigned int bitMaskForPosition = 1 << offset;
        unsigned int bitValueAtPosition = getBit(oldKey, startBit + i);
        unsigned int byteInNewStem = i / BITS_PER_BYTE;
        newStem[byteInNewStem] |= bitMaskForPosition * bitValueAtPosition;
    }

    return newStem;
}

/* Returns min of 3 integers 
    reference: https://www.geeksforgeeks.org/edit-distance-in-c/ */
int min(int a, int b, int c) {
    if (a < b) {
        if(a < c) {
            return a;
        } else {
            return c;
        }
    } else {
        if(b < c) {
            return b;
        } else {
            return c;
        }
    }
}

/* Returns the edit distance of two strings
    reference: https://www.geeksforgeeks.org/edit-distance-in-c/ */
int editDistance(char *str1, char *str2, int n, int m){
    assert(m >= 0 && n >= 0 && (str1 || m == 0) && (str2 || n == 0));
    // Declare a 2D array to store the dynamic programming
    // table
    int dp[n + 1][m + 1];

    // Initialize the dp table
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            // If the first string is empty, the only option
            // is to insert all characters of the second
            // string
            if (i == 0) {
                dp[i][j] = j;
            }
            // If the second string is empty, the only
            // option is to remove all characters of the
            // first string
            else if (j == 0) {
                dp[i][j] = i;
            }
            // If the last characters are the same, no
            // modification is necessary to the string.
            else if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = min(1 + dp[i - 1][j], 1 + dp[i][j - 1],
                    dp[i - 1][j - 1]);
            }
            // If the last characters are different,
            // consider all three operations and find the
            // minimum
            else {
                dp[i][j] = 1 + min(dp[i - 1][j], dp[i][j - 1],
                    dp[i - 1][j - 1]);
            }
        }
    }

    // Return the result from the dynamic programming table
    return dp[n][m];
}


/* =====================================================================
   This skeleton/program is compiled by Kerui Huang.
   Last Updated: 06/09/2024
   ================================================================== */
