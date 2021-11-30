#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Creates node array consisting of oldspeak and newspeak and NULL pointers to their children
//
//oldspeak: the oldspeak component of the node
//newspeak: the newspeak component of the node
Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = (Node *) malloc(sizeof(Node));
    if (n) {
        n->oldspeak = strdup(oldspeak);
        if (newspeak) {
            n->newspeak = strdup(newspeak);
        } else {
            n->newspeak = NULL;
        }
        n->left = NULL;
        n->right = NULL;
    }
    return n;
}

//Frees oldspeak and newspeak strdup() memory then frees the node and NULLs the pointer
//
//n: the node
void node_delete(Node **n) {
    free((*n)->oldspeak);
    if ((*n)->newspeak) {
        free((*n)->newspeak);
    }
    free(*n);
    *n = NULL;
}

//Prints out oldspeak its newspeak translation of the node if not NULL
//
//n: the node
void node_print(Node *n) {
    if (n->newspeak) {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    } else {
        printf("%s\n", n->oldspeak);
    }
}
