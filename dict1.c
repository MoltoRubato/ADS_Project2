
/*-------------------------------------------------------------- 
..Project: Assignment1
  dict1.c : 
             = the first part of the main program of the project 
----------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "suburbs.h"
#include "list.h"

int main(int argc, char** argv) {

    // open the csv file to read data from
    FILE *inputFile = fopen(argv[2], "r");
    assert(inputFile);


    // create a linked list to store relevant data from records
    list_t *dictionary = listCreate();
    char *line = NULL;
    size_t len = 0;

    // skip the header line in order to read data correctly
    getline(&line, &len, inputFile);

    /* parse through every line in the csv file 
     and store records in the dictionary */
    while (getline(&line, &len, inputFile) != -1) {
        record_t *currentRec = readRecords(line);

        // append to the end of the linked list
        listAppend(dictionary, currentRec);
    }

    // remember to free line and close the data.csv file
    free(line);
    fclose(inputFile);

    // Create an ouput file to store data of records found
    FILE *outputFile = fopen(argv[3], "w");
    assert(outputFile);

    char *suburb = NULL;
    size_t suburbLength = 0;
    while (getline(&suburb, &suburbLength, stdin) != -1) {
        
        // remove the newline character from suburb name
        suburb[strcspn(suburb, "\n")] = '\0';

        // search for any records that match the suburb from stdin
        list_t *results = listSearch(dictionary, suburb, dataGetKey);

        int numRecords = listCount(results);

        fprintf(outputFile, "%s -->\n", suburb);

        if (listIsEmpty(results)) {
            printf("%s --> NOTFOUND\n", suburb);
        } else {
            node_t *current = results->head;
            while (current) {
                printRecord(current->data, outputFile);
                current = current->next;
            }
        }

        if (numRecords) {
            printf("%s --> %d records found\n", suburb, listCount(results));
        }

        listFree(results, *recordFree);
    }

    free(suburb);
    // free the dictionary containing all the data
    listFree(dictionary, *recordFree);

    fclose(outputFile);

    return 0;
}



/* =====================================================================
   This program was compiled by Nick Ho and Kerui Huang.
   Last Updated: 07/08/2022
   ================================================================== */



