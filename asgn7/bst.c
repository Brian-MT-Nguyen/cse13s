#include "bst.h"
#include <stdio.h>
#include <string.h>

uint64_t branches = 0;
//
Node *bst_create(void) {
    return NULL;
}

void bst_delete(Node **root) {
    if (*root) {
        bst_delete(&(*root)->left);
        bst_delete(&(*root)->right);
        node_delete(&*root);
    }
}

//Helper function to help compare and find the maximum of 2 values
static int max(int x, int y) {
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

//
uint32_t bst_height(Node *root) {
    if (root) {
        return 1 + max(bst_height(root->left), bst_height(root->right));
    }
    return 0;
}

uint32_t bst_size(Node *root) {
    if (root) {
        return bst_size(root->left) + 1 + bst_size(root->right);
    }
    return 0;
}

Node *bst_find(Node *root, char *oldspeak) {
    if (root) {
        branches += 1;
        if (strcmp(root->oldspeak, oldspeak) > 0) {
            return bst_find(root->left, oldspeak);
        } else if (strcmp(root->oldspeak, oldspeak) < 0) {
            return bst_find(root->right, oldspeak);
        } else if (strcmp(root->oldspeak, oldspeak) == 0) {
            return root;
        }
    }
    return NULL;
}

Node *bst_insert(Node *root, char *oldspeak, char *newspeak) {
    if (root) {
        branches += 1;
        if (strcmp(root->oldspeak, oldspeak) > 0) {
            root->left = bst_insert(root->left, oldspeak, newspeak);
        } else if (strcmp(root->oldspeak, oldspeak) < 0) {
            root->right = bst_insert(root->right, oldspeak, newspeak);
        }
        return root;
    }
    return root = node_create(oldspeak, newspeak);
}

void bst_print(Node *root) {
    if (root) {
        bst_print(root->left);
        node_print(root);
        bst_print(root->right);
    }
}
