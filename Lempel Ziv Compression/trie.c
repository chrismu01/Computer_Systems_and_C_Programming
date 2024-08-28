#include <stdint.h>
#include <stdlib.h>
#include "trie.h"
#include "code.h"

//TrieNode create fuction: creates TrieNode and allocates memory for it
TrieNode *trie_node_create(uint16_t index) {
    TrieNode *n = (TrieNode *) malloc(sizeof(TrieNode)); // malloc the sizeof TrieNode
    for (int iters = 0; iters < ALPHABET;
         iters++) { //loop until the value of ALPHABET is reached (256)
        n->children[iters] = NULL; //set children at intex iters to NULL
    }
    n->code = index; //node's code set to code
    return n; //return the pointer
}

//trie node delete function: deletes the children of the trie
void trie_node_delete(TrieNode *n) {
    for (int iters = 0; iters < ALPHABET;
         iters++) { //loop until the value of ALPHABET is reached (256)
        free(n->children[iters]); //deallocate memory for children at index iters
    }

    free(n); //free the pointer
}

//trie create function: creates a trie
TrieNode *trie_create(void) {
    struct TrieNode *root
        = (struct TrieNode *) malloc(sizeof(struct TrieNode)); //root pointer create: malloc
    if (root) { //if root exists
        root->code = EMPTY_CODE; //set the code to the value of EMPTY_CODE
        for (int iters = 0; iters < ALPHABET;
             iters++) { //loop through until ALPHABET is reached (256)
            root->children[iters] = NULL; //set the children at index iters to NULL
        }
        return root; //return the root
    } else {
        return NULL; //returns NULL if not sucessful
    }
}

//trie reset function: resets the trie to root TrieNode
void trie_reset(TrieNode *root) {

    if (root == NULL) { //if the root is NULL do nothing
        return;
    }
    for (int iters = 0; iters < ALPHABET;
         iters++) { //loop until the value of ALPHABET is reached (256)
        if (root->children[iters] != NULL) { //if the children at index iters is not NULL
            trie_delete(root->children[iters]); //delete the children at index iters
            root->children[iters] = NULL; //set the children at index iters to NULL
        }
    }

    root->code = EMPTY_CODE; // set code to EMPTY_CODE
    for (int iters = 0; iters < ALPHABET; iters++) {
        root->children[iters] = NULL; // set the children at index iters to NULL
    }
}

//trie delete function: deletes the sub-trie
void trie_delete(TrieNode *n) {
    if (n == NULL) { //if n is NULL do nothing
        return;
    }

    for (int iters = 0; iters < ALPHABET; iters++) { //loop through until ALPHABET is reached (256)
        if (n->children[iters]
            != NULL) { // if the children at index iters is not equal to null. (n points to children)
            trie_delete(n->children[iters]); //delete it
            n->children[iters] = NULL; //set it to NULL
        }
    }

    free(n); //free the pointer
}

//trie step function: returns a pointer to the child node representing the symbol sym
TrieNode *trie_step(TrieNode *n, uint8_t sym) {
    if (n == NULL) { //if the pointer n is NULL return NULL
        return NULL;
    }
    TrieNode *check_sym = n->children[sym];

    if (check_sym == NULL) { //return null if symbol doesnt exist
        return NULL;
    }

    return check_sym; //return the symbol
}
