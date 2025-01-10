/*-------------------------------------------------------------- 
..Project: Assignment1
  suburb.h :  
         = the interface of module suburbData of the project
----------------------------------------------------------------*/
#ifndef _SUBURBS_H_
#define _SUBURBS_H_

// type definitions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
struct record {
    int compCode;
    int officialCodeSuburb;
    char* officialNameSuburb;
    int year;
    char* officialCodeState;
    char* officialNameState;
    char* officialCodeLGA;
    char* officialNameLGA;
    double latitude;
    double longitude;
};

typedef struct {
    int bitComparisons;    // Count of bit comparisons
    int stringComparisons; // Count of string comparisons
    int nodeAccesses;      // Count of node accesses
} search_stats_t;

typedef struct record record_t;
// add function prototypes and constants in here
#define STRING_BUFFER_SIZE 128

// copy a record so the original pointers are preserved
record_t* copyRecord(record_t *original);

// free the memory allocated to a record ... "void *" for polymorphic use
void recordFree(void *r);

// get-key function for linear search
char *dataGetKey(void *data);

// dynamically allocates memory for a string and copies that string to another
char *dynamicStringAllocate(char *srcString);

// reads each CSV line and create new data for its respective record
record_t* readRecords(char *line);

// SHOULD PRINT TO AN OUTPUT FILE (MAKE SURE TO CHANGE)
void printRecord(record_t *rec, FILE *file); 

void printCSV(record_t *rec, FILE *file);

void printQuotedField(char *field, FILE *file);

#endif


/* =====================================================================
   This skeleton/program is compiled by the Nick Ho and Kerui Huang.
   Last Updated: 06/08/2024
   ================================================================== */
