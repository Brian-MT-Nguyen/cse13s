#include "node.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *n = (Node *) malloc(sizeof(Node));
    if (n) {
        n->left = NULL;
        n->right = NULL;
        n->symbol = symbol;
        n->frequency = frequency;
    }
    return n;
}

void node_delete(Node **n) {
    free(*n);
    *n = NULL;
}

Node *node_join(Node *left, Node *right) {
    Node *parent_node = node_create('$', left->frequency + right->frequency);
    parent_node->left = left;
    parent_node->right = right;
    return parent_node;
}

void node_print(Node *n) {
    printf("%hhu, ", n->symbol);
}
