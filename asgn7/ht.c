#include "ht.h"
#include "salts.h"
#include "speck.h"
#include <stdio.h>
#include <stdlib.h>

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **trees;
};

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

void ht_delete(HashTable **ht) {
    if (*ht) {
        if ((*ht)->trees) {
            for (uint32_t i = 0; i < (*ht)->size; i++) {
                bst_delete(&(*ht)->trees[i]);
            }
            free(&(*ht)->trees);
        }
        free(*ht);
        *ht = NULL;
    }
}

uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

Node *ht_lookup(HashTable *ht, char *oldspeak) {
    uint32_t index = hash(ht->salt, oldspeak) % ht->size;
    return bst_find(ht->trees[index], oldspeak);
}

void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    uint32_t index = hash(ht->salt, oldspeak) % ht->size;
    bst_insert(ht->trees[index], oldspeak, newspeak);
}

uint32_t ht_count(HashTable *ht) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->trees[i]) {
            count += 1;
        }
    }
    return count;
}

double ht_avg_bst_size(HashTable *ht) {
    double avg_bst_size = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        avg_bst_size += bst_size(ht->trees[i]);
    }
    return (double) avg_bst_size / ht_count(ht);
}
double ht_avg_bst_height(HashTable *ht) {
    double avg_bst_height = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        avg_bst_height += bst_height(ht->trees[i]);
    }
    return (double) avg_bst_height / ht_count(ht);
}

void ht_print(HashTable *ht) {
    for (uint32_t i = 0; i < ht->size; i++) {
        bst_print(ht->trees[i]);
    }
}
