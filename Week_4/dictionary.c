// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "dictionary.h"


// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *ptr = root; // pointer to traverse the TRIE, no need for memory allocation

    for (int idx = 0; idx < strlen(word); idx++)
    {
        int key = (word[idx] == '\'') ? 26 : tolower(word[idx]) - 97; // allocate the apostrophe inside children at location 26

        if (!ptr->children[key]) // as soon as we come across a NULL, we break the search immediately
            return false;
			
        ptr = ptr->children[key]; // else, we go down deeper
    }

    if (ptr->is_word == true) // at this point we know that the path exists, so depending on the flag we return either true or false
        return true;

    return false;
}


// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r"); // now we open the same file for reading
    if (!file)
        return false;

    root = malloc(sizeof(node)); // initialize the root, which will give us a handle to the TRIE
    memset(root, 0, sizeof(node));
    node *cursor = NULL;
    char c; // variable to store char as we are going to scan the text character by character

    while ((c = fgetc(file)) != EOF) // we scan the whole file until we get to the end of last line (EOF in 'stdio' library)
    {
        cursor = root; // each time we start a new word, the pointer to the root has to be reinitialized

        while (true) // this is the inner loop for a single word in each line
        {
            if (c == '\n') // this is the only way to stop the inner loop - with the end of current line
            {
                cursor->is_word = true;
                cursor->is_last = true;
                dict_size ++;
                break;
            }

            int key = (c == '\'') ? 26 : c - 97; // allocate the apostrophe inside children at location 26

            if (cursor->children[key]) // if the given character already exists, then we just go one step down
            {
                cursor = cursor->children[key];
                cursor->is_last = false;
            }
            else {
                node *ptr = malloc(sizeof(node));
                memset(ptr, 0, sizeof(node));

                cursor->is_last = false;
                cursor->children[key] = ptr; // if not, before we go down the node has to be initialized
                cursor = cursor->children[key];
            }

            c = fgetc(file); // moving onto a next character
        }
    }

    fclose(file);

    return true;
}


// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dict_size;
}


// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    free_trie(root);
    return true;
}


void free_trie(node *r)
{
    if (r->is_last) {
        free(r);
        return;
    }

    for (int i = 0; i < 27; i++) {
        if (r->children[i] != NULL)
            free_trie(r->children[i]);
    }

    free(r);
    return;
}