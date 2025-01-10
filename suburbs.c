/*-------------------------------------------------------------- 
..Project: Assignment1
  suburbs.c :  
         = the implementation of module suburbData of the project
----------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h> 
#include <string.h>
#include "suburbs.h"

// the details of "struct record" is visible only inside "suburb.c"
// and cannot be employed in any other modules

// copy a record so the original pointer is preserved
record_t* copyRecord(record_t *original) {
    record_t *copy = malloc(sizeof(record_t));
    if (copy) {
        *copy = *original;
      
        // copy the data
        copy->officialNameSuburb = strdup(original->officialNameSuburb);
        copy->officialCodeState = strdup(original->officialCodeState);
        copy->officialNameState = strdup(original->officialNameState);
        copy->officialCodeLGA = strdup(original->officialCodeLGA);
        copy->officialNameLGA = strdup(original->officialNameLGA);
    }
    return copy;
}
 
// free the memory allocated to a record ... "void *" for polymorphic use
void recordFree(void *r) {
    record_t *record = (record_t *)r;

    if (record) {
        // dont need to free ints or doubles
        free(record->officialNameSuburb);
        record->officialNameSuburb = NULL;

        free(record->officialCodeState);
        record->officialCodeState = NULL;

        free(record->officialNameState);
        record->officialNameState = NULL;

        free(record->officialCodeLGA);
        record->officialCodeLGA = NULL;

        free(record->officialNameLGA);
        record->officialNameLGA = NULL;

        free(record);
        record = NULL;
    }
}

// getkey function for linear search
char *dataGetKey(void *data) {
    record_t *record = (record_t *)data;
    return record->officialNameSuburb;
}

// dynamically allocates memory for a string and copies that string to another
char *dynamicStringAllocate(char *srcString) {
    // find exactly how much memory we need
    int len = strlen(srcString);
    // allocate exact amount of memory for the new string
    char *newString = malloc((len + 1)*sizeof(char)); // +1 for null byte
    assert(newString);
    strcpy(newString, srcString);
    return newString;
}

/*  make a function to read in the data from csv file in here
    then we can export to dict1. 
    reads each CSV line and create new data for its respective record */
record_t* readRecords(char *line) {
    record_t *newRecord = malloc(sizeof(record_t));
    assert(newRecord);
    
    // temporary buffers for fields containing only strings
    char officialNameSuburb[STRING_BUFFER_SIZE];
    char officialCodeState[STRING_BUFFER_SIZE];
    char officialNameState[STRING_BUFFER_SIZE];
    char officialCodeLGA[STRING_BUFFER_SIZE];
    char officialNameLGA[STRING_BUFFER_SIZE];

    /* read data from the CSV file, taking into consideration 
     the potential quote delimiters */
    sscanf(line, "%d,%d,%[^,],%d,%[^,],%[^,],%[^,],%[^,],%lf,%lf",
           &newRecord->compCode, &newRecord->officialCodeSuburb, 
           officialNameSuburb, &newRecord->year, officialCodeState, 
           officialNameState, officialCodeLGA, officialNameLGA, 
           &newRecord->latitude, &newRecord->longitude);

    sscanf(line, "%d,%d,%[^,],%d,%[^,],%[^,],\"%[^\"]\",\"%[^\"]\",%lf,%lf",
           &newRecord->compCode, &newRecord->officialCodeSuburb, 
           officialNameSuburb, &newRecord->year, officialCodeState, 
           officialNameState, officialCodeLGA, officialNameLGA, 
           &newRecord->latitude, &newRecord->longitude);


    sscanf(line, "%d,%d,%[^,],%d,\"%[^\"]\",\"%[^\"]\","
            "\"%[^\"]\",\"%[^\"]\",%lf,%lf",
           &newRecord->compCode, &newRecord->officialCodeSuburb, 
           officialNameSuburb, &newRecord->year, officialCodeState, 
           officialNameState, officialCodeLGA, officialNameLGA, 
           &newRecord->latitude, &newRecord->longitude);


    /* dynamically allocate the necessary amount of memory for each string.
     remove any quotation marks around strings that were delimited by quotes */
    newRecord->officialNameSuburb = dynamicStringAllocate(officialNameSuburb);
    newRecord->officialCodeState = dynamicStringAllocate(officialCodeState);
    newRecord->officialNameState = dynamicStringAllocate(officialNameState);
    newRecord->officialCodeLGA = dynamicStringAllocate(officialCodeLGA);
    newRecord->officialNameLGA = dynamicStringAllocate(officialNameLGA);

    return newRecord;
}

void printRecord(record_t *rec, FILE *file) {
    if (rec) {
        fprintf(file, "COMP20003 Code: %d, Official Code Suburb: %d, "
        "Official Name Suburb: %s, Year: %d, Official Code State: %s, "
        "Official Name State: %s, Official Code Local Government Area: %s, "
        "Official Name Local Government Area: %s, Latitude: %.7lf, "
        "Longitude: %.7lf\n",
            rec->compCode, rec->officialCodeSuburb, rec->officialNameSuburb, 
            rec->year, rec->officialCodeState, rec->officialNameState, 
            rec->officialCodeLGA, rec->officialNameLGA, rec->latitude, 
            rec->longitude);
    }
}
// prints CSV after deletion of records
void printCSV(record_t *rec, FILE *file) {

    /* for any string fields that contain commas separating more information,
      print the appropriate delimiting quotes around the string field */
    if (rec) {
        fprintf(file, "%d,%d,", rec->compCode, rec->officialCodeSuburb);
        printQuotedField(rec->officialNameSuburb, file);
        fprintf(file, "%d,", rec->year);

        printQuotedField(rec->officialCodeState, file);
        printQuotedField(rec->officialNameState, file);
        printQuotedField(rec->officialCodeLGA, file);
        printQuotedField(rec->officialNameLGA, file);

        fprintf(file, "%.7lf,%.7lf\n", rec->latitude, rec->longitude);
    }
}

/* Check if the field contains any commas 
 prints delimiting quotes if commas exist */
void printQuotedField(char *field, FILE *file) {
    if (strchr(field, ',')) {
        fprintf(file, "\"%s\",", field);
    } else {
        fprintf(file, "%s,", field);
    }
}

/* =====================================================================
   The original program was compiled by the Nick Ho and Kerui Huang.
   Last Updated: 11/08/2024
   ================================================================== */




