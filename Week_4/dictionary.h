// Declares a dictionary's functionality

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>


// Maximum length for a word set as constant 45 letters
#define LENGTH 45 // (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)


// Elements of TRIE data structure
typedef struct node {
    bool is_word; // indicates whether a current letter is a valid word
    struct node *children[27]; // table that contains node pointers that will guide us the next element (letter) of the word
    bool is_last; // extra flag preventing the search inside the last node's children while cleaning the memory
}
node;

node *root; // handle for the TRIE
int dict_size; // extra global item for counting the dictionary size while importing


// Prototypes
bool check(const char *word);
bool load(const char *dictionary);
unsigned int size(void);
bool unload(void);
void free_trie(node *r); // extra item for recursive memory cleaning

#endif // DICTIONARY_H
