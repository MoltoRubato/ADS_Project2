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
#include "tree.h"

int main(int argc, char** argv) {
    
    // open the csv file to read data from
    FILE *inputFile = fopen(argv[2], "r");
    assert(inputFile);

    // Use a Patricia Tree to store relevant data from records
    PatriciaNode* root = NULL;
    char *line = NULL;
    size_t len = 0;

    // Skip the header line in order to read data correctly
    getline(&line, &len, inputFile);

    /* parse through every line in the csv file 
     and store records in the Patricia Tree */
    while (getline(&line, &len, inputFile) != -1) {
        record_t *currentRec = readRecords(line);

        /* Insert record into the Patricia Tree 
        using the official suburb name as the key */
        root = insertPatricia(root, currentRec);
    }
    
    // Free line and close the data.csv file
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

        // Renew comparison counts
        search_stats_t stats = {0, 0, 0};

        // Search for any records that match the suburb from stdin
        SearchResult* searchResult = searchPatricia(root, suburb, &stats);

        // Print results to the output file
        fprintf(outputFile, "%s -->\n", suburb);

        if (searchResult->bestMatch == NULL) {
            fprintf(outputFile, "NOTFOUND\n");
            printf("%s --> NOTFOUND\n", suburb);
        } else {
            printRecord(&searchResult->record, outputFile);
            printf("%s --> %d records - comparisons: b%d n%d s%d\n",
                   suburb, 1, stats.bitComparisons, 
                   stats.nodeAccesses, stats.stringComparisons);
        }
    }
    // free data
    freePatriciaNode(root, *recordFree);
    
    
    fclose(outputFile);

    return 0;
}



/* =====================================================================
   This program was compiled by Kerui Huang.
   Last Updated: 06/09/2022
   ================================================================== */



