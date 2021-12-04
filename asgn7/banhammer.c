#include "node.h"
#include "bst.h"
#include "ht.h"
#include "bv.h"
#include <stdio.h>

int main(void) {
    char *brianster = "brian";
    char *brainster = "brain";

    char *kelly = "kelly";
    char *kel = "kel";

    char *ninja = "ninja";
    char *nina = "nina";

    char *andy = "andy";

    char *lucas = "lucas";

    char *ricca = "ricca";
    char *grandma = "grandma tanskin";

    char *adrian = "adrian";
    char *aids = "aids";

    HashTable *root = ht_create(3);

    ht_insert(root, brianster, brainster);
    ht_insert(root, brianster, brainster);
    ht_insert(root, kelly, kel);
    ht_insert(root, ninja, nina);
    ht_insert(root, andy, NULL);
    ht_insert(root, adrian, aids);
    ht_insert(root, lucas, NULL);
    ht_insert(root, ricca, grandma);

    ht_print(root);
    printf("Size of HT: %d\n", ht_size(root));
    printf("Count of Non NULL Trees: %d\n", ht_count(root));
    printf("AVG BST Size: %f\n", ht_avg_bst_size(root));
    printf("AVG BST Height: %f\n", ht_avg_bst_height(root));

    if (ht_lookup(root, "lmao") == NULL) {
        printf("Nice (Not in Tree)\n");
    }
    if (ht_lookup(root, brianster) != NULL) {
        printf("Nice (In Tree)\n");
    }
    if (ht_lookup(root, andy) != NULL) {
        printf("Nice (In Tree)\n");
    }
    if (ht_lookup(root, "test") == NULL) {
        printf("Nice (Not in Tree)\n");
    }
    if (ht_lookup(root, "eggroll") == NULL) {
        printf("Nice (Not in Tree)\n");
    }

    BitVector *bv = bv_create(64);
    printf("inserting 6th bit\n");
    bv_set_bit(bv, 6);
    bv_print(bv);
    printf("inserting 43th bit\n");
    bv_set_bit(bv, 43);
    bv_print(bv);
    printf("inserting 56th bit (illegal)\n");
    bv_set_bit(bv, 56);
    bv_print(bv);
    printf("inserting 55th bit\n");
    bv_set_bit(bv, 55);
    bv_print(bv);
    printf("inserting 3th bit\n");
    bv_set_bit(bv, 3);
    bv_print(bv);
    printf("clearing 3th bit\n");
    bv_clr_bit(bv, 3);
    bv_print(bv);
    if (bv_get_bit(bv, 43) == true) {
        printf("1!");
    }
    if(bv_get_bit(bv, 24) == false) {
        printf("0!");
    }
    if(bv_get_bit(bv, 6) == true) {
        printf("1!");
    }
    ht_delete(&root);
}
