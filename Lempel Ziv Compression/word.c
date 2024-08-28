#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "word.h"
#include "code.h"
#include <string.h>

//word create function: creates word and allocates memory for it
Word *word_create(uint8_t *syms, uint32_t len) {

    Word *word = (Word *) malloc(sizeof(Word)); //allocate memory for a Word pointer

    //initialize the following variables
    word->syms = syms;
    word->len = len;

    return word;
}

//word append symbol function: appends a symbol to word (
Word *word_append_sym(Word *w, uint8_t sym) {

    Word *new_word = (Word *) malloc(sizeof(Word)); //allocate memoery for a new word pointer

    uint32_t new_len = w->len + 1; //length gets increased by one

    uint8_t *new_syms = (uint8_t *) malloc(new_len * sizeof(uint8_t));
    if (w->syms != NULL) {
        memcpy(new_syms, w->syms, w->len * sizeof(uint8_t));
    }
    new_syms[new_len - 1] = sym;

    new_word->syms = new_syms; //set new syms
    new_word->len = new_len; //set new lenth

    return new_word; //return the new word pointer
}

//word delete function: deletes the word
void word_delete(Word *w) {
    free(w); //free the word
}

//word table create function: creates the word table an allocates memory for it (array of words)
WordTable *wt_create(void) {
    WordTable *wt = (WordTable *) calloc(
        MAX_CODE, sizeof(WordTable)); //calloc with the value of MAX_CODE and the size of WordTable
    if (!wt) { //returns null if the wt pointer is null
        return NULL;
    }
    wt[EMPTY_CODE]
        = word_create(NULL, 0); //initialize wt at index EMPTY_CODE witha string of length of zero
    return wt;
}

//word table reset function: reset the wt pointer to just empty Word. All other words in the table are NULL
void wt_reset(WordTable *wt) {
    if (wt == NULL) { //return if wt pointer is null
        return;
    }

    for (int iters = 0; iters < MAX_CODE; iters++) { //loop through until MAX_CODE is reached
        if (iters == EMPTY_CODE) { //dont need delete and set EMPTY_CODE to NULL
            continue;
        }
        if (wt[iters] != NULL) { //if at the index iters is not equal to NULL
            word_delete(wt[iters]); //deletes the word at the index iteres
            wt[iters] = NULL; //sets the value at index iters to NULL
        }
    }
}

//word table delete function: deletes the words in the word table and setting it to NULL. At the end freeing the wordtable
void wt_delete(WordTable *wt) {
    if (wt == NULL) { //return if wt pointer is NULL
        return;
    }

    for (int iters = 0; iters < MAX_CODE; iters++) { //loop through until MAX_CODE is reached
        if (wt[iters] != NULL) { //if at the index iters is not equal to NULL
            word_delete(wt[iters]); //delete it at index iters
            wt[iters] = NULL; //set it to null at index iteres
        }
    }

    free(wt);
}
