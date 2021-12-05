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
        case 't': table_size = atoi(optarg); break;
        case 'f': filter_size = atoi(optarg); break;
        case 's': stats = true; break;
        default: break;
        }
    }

    //Prints help message if prompted or invalid use of arguments
    if (help || optopt != 0) {
        printf("SYNOPSIS\n");
        printf("   Encrypts data using RSA encryption.\n");
        printf("   Encrypted data is decrypted by the decrypt program.\n");

        printf("\nUSAGE\n");
        printf("   ./encrypt [-hv] [-i infile] [-o outfile] -n pubkey\n");

        printf("\nOPTIONS\n");
        printf("   -h              Display program help and usage.\n");
        printf("   -v              Display verbose program output.\n");
        printf("   -i infile       Input file of data to encrypt (default: stdin).\n");
        printf("   -o outfile      Output file for encrypted data (default: stdout).\n");
        printf("   -n pbfile       Public key file (default: rsa.pub).\n");
        exit(0);
    }

    //Initialize Bloom Filter & Hash Table with inputted/default sizes
    BloomFilter *bf = bf_create(filter_size);
    HashTable *ht = ht_create(table_size);

    //Initialize variable to store badspeak words and open badspeak file to read from
    char badspeak[1024];
    FILE *badspeak_file = fopen("badspeak.txt", "r");

    //Read through badspeak file, then place badspeak in the filter & table
    while (fscanf(badspeak_file, "%s\n", badspeak) > 0) {
        bf_insert(bf, badspeak);
        ht_insert(ht, badspeak, NULL);
    }

    //Initialize variable to store oldspeak & newspeak pairs and open newspeak file to read from
    char oldspeak[1024];
    char newspeak[1024];
    FILE *newspeak_file = fopen("newspeak.txt", "r");

    //Read through newspeak file, then place oldspeak in filter and word oldspeak & newspeak pair in table
    while (fscanf(newspeak_file, "%s %s\n", oldspeak, newspeak) > 0) {
        bf_insert(bf, oldspeak);
        ht_insert(ht, oldspeak, newspeak);
    }
    bf_print(bf);
    ht_print(ht);

    //Initialize Regular Expression (word) used to match user's words and checks if not successfully made
    regex_t word_regex;
    if (regcomp(&word_regex, WORD, REG_EXTENDED) != 0) {
        regfree(&word_regex);
        exit(0);
    }

    //Initialize bst trees that will hold user's bad/mixed inputted words
    Node *bad_words = bst_create();
    Node *mixed_words = bst_create();

    //Get user's input (store in var) and parse through each word, then filter through it and add to respective trees
    char *input_word = NULL;
    while ((input_word = next_word(stdin, &word_regex))) {
        //Lowercase word to be case insensitive
        tolower(*input_word);
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
    if (stats) {
        printf("lol");
        exit(0);
    }

    if ((bst_size(bad_words) > 0) && (bst_size(mixed_words) > 0)) {
        printf("%s\n", mixspeak_message);
        bst_print(bad_words);
        bst_print(mixed_words);
    }
    if ((bst_size(bad_words) > 0) && (bst_size(mixed_words) == 0)) {
        printf("%s\n", badspeak_message);
        bst_print(bad_words);
    }
    if ((bst_size(bad_words) == 0) && (bst_size(mixed_words) > 0)) {
        printf("%s", goodspeak_message);
        bst_print(mixed_words);
    }

    //Close Files and Free Memory
    fclose(badspeak_file);
    fclose(newspeak_file);
    bf_delete(&bf);
    ht_delete(&ht);
    regfree(&word_regex);
}
