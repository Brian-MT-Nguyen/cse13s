#include "bst.h"
#include <stdio.h>
#include <string.h>

uint64_t branches = 0;

//Creates a NULL bst tree (just for semantics)
Node *bst_create(void) {
    return NULL;
}

//Recursively frees all the memory from the nodes of the tree
//
//root: the root node of the bst
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

//Gets the height of the tree by adding 1 per traversal and comparing the max height (left vs right) via recursion
//Returns the int height of the tree
//
//root: the root node of the bst
uint32_t bst_height(Node *root) {
    if (root) {
        return 1 + max(bst_height(root->left), bst_height(root->right));
    }
    return 0;
}

//Gets the size of the tree by adding 1 per traversal via recursion
//Returns the int height of the tree
//
//root: the root node of the bst
uint32_t bst_size(Node *root) {
    if (root) {
        return bst_size(root->left) + 1 + bst_size(root->right);
    }
    return 0;
}

//Finds whether the oldspeak word is in the bst via recursion
//Returns the pointer to the root node if in the tree
//Returns NULL if not in the tree
//
//root: the root node of the bst
//oldspeak: the olspeak word
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

//Inserts the oldspeak & newspeak pair into an empty tree node if it is not already in the tree via recursion
//Returns the pointer to the existing root if duplicate
//Returns the pointer to the creation of the new node otherwise
//
//root: the root node of the bst
//oldspeak: the oldspeak word
//newspeak: the newspeak word
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

//Prints all the nodes of the tree by in order traversal
//
//root: the root node of the bst
void bst_print(Node *root) {
    if (root) {
        bst_print(root->left);
        node_print(root);
        bst_print(root->right);
    }
}
