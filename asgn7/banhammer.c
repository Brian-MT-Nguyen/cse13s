#include "node.h"
#include "bst.h"
#include "ht.h"
#include "bv.h"
#include "bf.h"
#include "parser.h"
#include "messages.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>

#define OPTIONS "ht:f:s"
#define WORD    "[a-zA-Z0-9_]+(('|-)[a-zA-Z0-9_]+)*"

int main(int argc, char **argv) {
    //Initialize variables used to parse through command line
    int opt = 0;
    bool help = false;
    bool stats = false;
    uint32_t table_size = 1 << 16; //1*(2^16)
    uint32_t filter_size = 1 << 20; //1*(2^20)

    //Parses through command line input options
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': help = true; break;
        case 't':
            table_size = atoi(optarg);
            if (table_size < 0) {
                fprintf(stderr, "Invalid hash table size.\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'f':
            filter_size = atoi(optarg);
            if (filter_size < 0) {
                fprintf(stderr, "Invalid Bloom filter size.\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 's': stats = true; break;
        default: break;
        }
    }

    //Prints help message if prompted or invalid use of arguments
    if (help || optopt != 0) {
        printf("SYNOPSIS\n");
        printf("  A word filtering program for the GPRSC.\n");
        printf("  Filters out and reports bad words parsed from stdin.\n");

        printf("\nUSAGE\n");
        printf("  ./banhammer [-hs] [-t size] [-f size]\n");

        printf("\nOPTIONS\n");
        printf("  -h           Program help and usage.\n");
        printf("  -s           Print program statistics.\n");
        printf("  -t size      Specify hash table size (default: 2^16).\n");
        printf("  -f size      Specify Bloom filter size (default: 2^20).\n");
        exit(EXIT_SUCCESS);
    }

    //Initialize Bloom Filter & Hash Table with inputted/default sizes
    BloomFilter *bf = bf_create(filter_size);
    HashTable *ht = ht_create(table_size);

    //Print errors and exit program if unable to create filter/table
    if (bf == NULL) {
        fprintf(stderr, "Failed to create Bloom filter.\n");
        exit(EXIT_FAILURE);
    }
    if (ht == NULL) {
        fprintf(stderr, "Failed to create hash table.\n");
        exit(EXIT_FAILURE);
    }

    //Initialize variable to store badspeak words and open badspeak file to read from
    char badspeak[1024];
    FILE *badspeak_file = fopen("badspeak.txt", "r");

    //Print error if unable to open badspeak file
    if (badspeak_file == NULL) {
        fprintf(stderr, "Failed to open badspeak.txt");
        exit(EXIT_FAILURE);
    }

    //Read through badspeak file, then place badspeak in the filter & table
    while (fscanf(badspeak_file, "%s\n", badspeak) > 0) {
        bf_insert(bf, badspeak);
        ht_insert(ht, badspeak, NULL);
    }

    //Initialize variable to store oldspeak & newspeak pairs and open newspeak file to read from
    char oldspeak[1024];
    char newspeak[1024];
    FILE *newspeak_file = fopen("newspeak.txt", "r");

    //Print error if unable to open newspeak file
    if (newspeak_file == NULL) {
        fprintf(stderr, "Failed to open newspeak.txt");
        exit(EXIT_FAILURE);
    }
    //Read through newspeak file, then place oldspeak in filter and word oldspeak & newspeak pair in table
    while (fscanf(newspeak_file, "%s %s\n", oldspeak, newspeak) > 0) {
        bf_insert(bf, oldspeak);
        ht_insert(ht, oldspeak, newspeak);
    }

    //Initialize Regular Expression (word) used to match user's words and checks if not successfully made
    regex_t word_regex;
    if (regcomp(&word_regex, WORD, REG_EXTENDED)) {
        return 1;
    }

    //Initialize bst trees that will hold user's bad/mixed inputted words
    Node *bad_words = bst_create();
    Node *mixed_words = bst_create();

    //Get user's input (store in var) and parse through each word, then filter through it and add to respective trees
    char *input_word = NULL;
    while ((input_word = next_word(stdin, &word_regex)) != NULL) {
        //Lowercase each letter in the string (makes word lowercase)
        for (uint32_t i = 0; i < strlen(input_word); i++) {
            input_word[i] = tolower(input_word[i]);
        }
        //Checks if word detected by Bloom Filter
        if (bf_probe(bf, input_word)) {
            Node *word_checker = ht_lookup(ht, input_word);
            //Checks if word in Hash Table and badspeak word
            if ((word_checker != NULL) && (word_checker->newspeak != NULL)) {
                mixed_words
                    = bst_insert(mixed_words, word_checker->oldspeak, word_checker->newspeak);
            }
            //Checks if word in Hash Table and oldspeak & newspeak pair
            else if ((word_checker != NULL) && (word_checker->newspeak == NULL)) {
                bad_words = bst_insert(bad_words, word_checker->oldspeak, NULL);
            }
        }
    }
    //Print Stats if prompted
    if (stats) {
        printf("Average BST size: %0.6f\n", ht_avg_bst_size(ht));
        printf("Average BST height: %0.6f\n", ht_avg_bst_height(ht));
        printf("Average branches traversed: %0.6f\n", (double) branches / lookups);
        printf("Hash table load: %0.6f%%\n", 100.0 * ((double) ht_count(ht) / ht_size(ht)));
        printf("Bloom filter load: %0.6f%%\n", 100.0 * ((double) bf_count(bf) / bf_size(bf)));
    } else {
        //Print message out only if stats not prompted
        if ((bst_size(bad_words) > 0) && (bst_size(mixed_words) > 0)) {
            printf("%s", mixspeak_message);
            bst_print(bad_words);
            bst_print(mixed_words);
        }
        if ((bst_size(bad_words) > 0) && (bst_size(mixed_words) == 0)) {
            printf("%s", badspeak_message);
            bst_print(bad_words);
        }
        if ((bst_size(bad_words) == 0) && (bst_size(mixed_words) > 0)) {
            printf("%s", goodspeak_message);
            bst_print(mixed_words);
        }
    }
    //Close Files and Free Memory
    fclose(badspeak_file);
    fclose(newspeak_file);
    bf_delete(&bf);
    ht_delete(&ht);
    bst_delete(&bad_words);
    bst_delete(&mixed_words);
    clear_words();
    regfree(&word_regex);
    exit(EXIT_SUCCESS);
}
