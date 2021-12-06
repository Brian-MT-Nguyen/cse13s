#include "ht.h"
#include "salts.h"
#include "speck.h"
#include <stdio.h>
#include <stdlib.h>

uint64_t lookups = 0;

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **trees;
};

//Dynamically allocates an array of bst trees based the size + sets its size and salts for hashing
//Returns the hash table
//
//size: the size of the array of bsts
HashTable *ht_create(uint32_t size) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht) {
        ht->salt[0] = SALT_HASHTABLE_LO;
        ht->salt[1] = SALT_HASHTABLE_HI;
        ht->size = size;
        ht->trees = (Node **) calloc(size, sizeof(Node *));
    }
    return ht;
}

//Frees the memory from all the bst trees and then the hash table
//
//ht: the hash table
void ht_delete(HashTable **ht) {
    if (*ht) {
        if ((*ht)->trees) {
            for (uint32_t i = 0; i < (*ht)->size; i++) {
                bst_delete(&(*ht)->trees[i]);
            }
            free((*ht)->trees);
        }
        free(*ht);
        *ht = NULL;
    }
}

//Returns the size of the hash table
//
//ht: the hash table
uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

//Finds whether the oldspeak word is in the hash table
//Returns the pointer to the node if in any tree
//Returns NULL if not in any tree
//
//ht: the hash table
//oldspeak: the oldspeak word
Node *ht_lookup(HashTable *ht, char *oldspeak) {
    lookups += 1;
    uint32_t index = hash(ht->salt, oldspeak) % ht->size;
    return bst_find(ht->trees[index], oldspeak);
}

//Inserts the node into the hash table if not a duplicate
//Return;
//
//ht: the hash table
//oldspeak: the oldspeak word
//newspeak: the newspeak word
void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    lookups += 1;
    uint32_t index = hash(ht->salt, oldspeak) % ht->size;
    ht->trees[index] = bst_insert(ht->trees[index], oldspeak, newspeak);
}

//Finds the amount of non null bsts in the hash table
//Returns the number of non null bsts
//
//ht: the hash table
uint32_t ht_count(HashTable *ht) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->trees[i]) {
            count += 1;
        }
    }
    return count;
}

//Gets the total size of all bst trees and divides by the amount of non null bsts to get avg bst size
//Returns the avg bst size in the hash table
//
//ht: the hash table
double ht_avg_bst_size(HashTable *ht) {
    double avg_bst_size = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        avg_bst_size += bst_size(ht->trees[i]);
    }
    return (double) avg_bst_size / ht_count(ht);
}

//Gets the total height of all bst trees and divides by the amount of non null bsts to get avg bst height
//Returns the avg bst height in the hash table
//
//ht: the hash table
double ht_avg_bst_height(HashTable *ht) {
    double avg_bst_height = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        avg_bst_height += bst_height(ht->trees[i]);
    }
    return (double) avg_bst_height / ht_count(ht);
}

//Prints all the bsts in the hash table in order
//
//ht: the hash table
void ht_print(HashTable *ht) {
    for (uint32_t i = 0; i < ht->size; i++) {
        printf("#%d:\n", i);
        bst_print(ht->trees[i]);
    }
}
