#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// function signatures all go up here to help the compiler

int open_file(const char *filename, int mode,char **outPointers, int *outCount, char *paragraph);
void print_data(int mode, char **pointers, int count, const char *label, int isOriginal);
void insertion_sort(char **pointers, int count);
void do_binary_search(const char *wordsFileName, char **pointers, int count);
    
int main(int argc, char** argv) {
    char *fname_par = NULL;
    char *fname_words = NULL;
    int mode = 0; // 0 - normal, 1 - verbose

    
    if (argc < 4 ) {
        printf("Not enough arguments. Need mode and 2 filenames, e.g.: 0 small.txt words1.txt\n");
        return 1;
    } else {
        fname_par = argv[2];
        fname_words = argv[3];
        mode = atoi(argv[1]);
    }
    
    
    printf("mode: %d | paragraph: %s | words: %s\n", mode, fname_par, fname_words);

    //buffer
    static char paragraph[10001]; 

    static char *pointers[2000];

    int count = 0; 

    if (open_file(fname_par, mode, pointers, &count, paragraph) != 0) {
        return 1;
    }

    // Print original data
    print_data(mode, pointers, count, "-- Original data --", 1); //called first to print original data for the unsorted and unmodified data.

 
    insertion_sort(pointers, count);

    // Print sorted data
    print_data(mode, pointers, count, "-- Clean and sorted data --", 0); //prints now sorted data


    do_binary_search(fname_words, pointers, count);

    return 0;
}

int open_file(const char *filename, int mode, char **outPointers, int *outCount, char *paragraph)
{

    //file error message testing
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("error\n");
        return 1;
    }

    paragraph[0] = '\0'; 

    //buffer
    char buffer[600];

    //reads into buffer
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        strcat(paragraph, buffer);
    }
    fclose(fp);

    // Convert the entire paragraph to lowercase
    for (int i = 0; paragraph[i] != '\0'; i++) {
        paragraph[i] = (char)tolower((unsigned char) paragraph[i]);
    }
    
    char **pointersRef = outPointers;
    int count = 0;

    char *tok = strtok(paragraph, " \n\t\r");
    while (tok != NULL) {
        // Strips punctuations
        char *start = tok;
        char *end = tok + strlen(tok) - 1;
        
        // leading punctuation
        while (*start && ispunct((unsigned char)*start)) {
            start++;
        }

        //  trailng punctuation
        while (end > start && ispunct((unsigned char)*end)) {
            *end = '\0';
            end--;
        }

        // Store the cleaned word as long as not empty
        if (*start != '\0') {
            pointersRef[count] = start;
            count++;
        }

        // next token
        tok = strtok(NULL, " \n\t\r");
    }

    *outCount = count;

    return 0; 
}

//insertion sort
void insertion_sort(char **pointers, int count)
{
    for (int i = 1; i < count; i++) {
        char *key = pointers[i];
        int j = i - 1;

        while (j >= 0 && strcmp(pointers[j], key) > 0) {
            pointers[j + 1] = pointers[j];
            j--;
        }
        pointers[j + 1] = key;
    }
}

//data print
void print_data(int mode, char **pointers, int count, const char *label, int isOriginal)
{
    printf("%s\n", label);

    if (mode == 0) {
        // Normal mode
        for (int i = 0; i < count; i++) {
            printf("%d %s\n", i, pointers[i]);
        }
    } else if (mode == 1) {
        // Verbose mode
        printf("i   | pointers[i]      | word\n");
        printf("----|------------------|----------------\n");
        for (int i = 0; i < count; i++) {
            printf("%-3d | %-16p | %s\n", i, (void*)pointers[i], pointers[i]);
        }
    }
}


//binary search
void do_binary_search(const char *wordsFileName, char **pointers, int count)
{
    FILE *wordsFile = fopen(wordsFileName, "r");
    if (!wordsFile) {
        printf("Error try again: %s\n", wordsFileName);
        return;
    }

    char searching[100];
    while (fgets(searching, sizeof(searching), wordsFile) != NULL) {
        
        searching[strcspn(searching, "\n")] = '\0';

     
        for (int i = 0; searching[i] != '\0'; i++) {
            searching[i] = (char)tolower((unsigned char)searching[i]);
        }


      
        printf("%s\n", searching);

        
        int low = 0;
        int high = count - 1;
        int found = 0; 
        int iterationCount = 0;

        

        int firstPrint = 1; 
        
        //prints for mnid
        while (low <= high) {
            iterationCount++;
            int mid = (low + high) / 2;

            //prints for mid
            if (!firstPrint) {
                printf(", ");
            }
            printf("%d", mid);
            firstPrint = 0;


            //logic and binary search core
            int comp = strcmp(searching, pointers[mid]);
            if (comp == 0) {
                found = 1;
                break;
            } else if (comp < 0) {
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }

        //cprints onfirmation and binary search results 
        printf(", (%d iterations) ", iterationCount);
        if (found) {
            printf("found\n");
        } else {
            printf("not found\n");
        }
    }

    fclose(wordsFile);
}
