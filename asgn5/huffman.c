#include "huffman.h"
#include "pq.h"
#include "stack.h"
#include "io.h"
#include <stdio.h>

Node *build_tree(uint64_t hist[static ALPHABET]) {
    PriorityQueue *q = pq_create(ALPHABET);
    Node *n;
    Node *left;
    Node *right;
    Node *parent;
    Node *root;
    for (uint32_t i = 0; i < ALPHABET; i++) {
        if (hist[i] > 0) {
            n = node_create(i, hist[i]);
            enqueue(q, n);
        }
    }
    while (pq_size(q) > 1) {
        dequeue(q, &left);
        dequeue(q, &right);
        parent = node_join(left, right);
        enqueue(q, parent);
    }
    dequeue(q, &root);
    pq_delete(&q);
    return root;
}

void build_codes(Node *root, Code table[static ALPHABET]) {
    Code c = code_init();
    uint8_t dump_bit;
    if (root != NULL) {
        if (!root->left && !root->right) {
            table[root->symbol] = c;
        } else {
            code_push_bit(&c, 0x0);
            build_codes(root->left, table);
            code_pop_bit(&c, &dump_bit);

            code_push_bit(&c, 0x1);
            build_codes(root->right, table);
            code_pop_bit(&c, &dump_bit);
        }
    }
}

void dump_tree(int outfile, Node *root) {
    if (root) {
        dump_tree(outfile, root->left);
        dump_tree(outfile, root->right);
        uint8_t buf[3];
        if (!root->left && !root->right) {
            buf[0] = 'L';
            buf[1] = root->symbol;
            write_bytes(outfile, &buf[0], 1);
            write_bytes(outfile, &buf[1], 1);
        } else {
            buf[2] = 'I';
            write_bytes(outfile, &buf[2], 1);
        }
    }
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
    Node *n;
    Node *left;
    Node *right;
    Node *parent;
    Node *root;
    Stack *s = stack_create(ALPHABET);
    for (uint16_t i = 0; i < nbytes; i++) {
        if (tree[i] == 'L') {
            n = node_create(tree[i + 1], 0);
            stack_push(s, n);
            i++;
        }
        if (tree[i] == 'I') {
            stack_pop(s, &right);
            stack_pop(s, &left);
            parent = node_join(left, right);
            stack_push(s, parent);
        }
    }
    stack_pop(s, &root);
    stack_delete(&s);
    return root;
}

void delete_tree(Node **root) {
    if (*root) {
        delete_tree(&(*root)->right);
        delete_tree(&(*root)->left);
        node_delete(root);
    }
}
