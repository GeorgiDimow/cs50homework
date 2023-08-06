// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

void printLinkedList(node *head);

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];
int SIZE = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int hash_inedx = hash(word);

    node *current = table[hash_inedx];
    while (current != NULL)
    {
        int result = strcasecmp(current->word, word);
        if (result == 0)
        {
            return true;
        }
        current = current->next;
    }


    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");

    if (file == NULL)
    {
        fclose(file);
        return false;
    }


    int index = 0;
    char word[LENGTH + 1];

    char c;
    while (fread(&c, sizeof(char), 1, file))
    {
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // Append character to word
            word[index] = c;
            index++;
            // Ignore alphabetical strings too long to be words
            if (index > LENGTH)
            {
                // Consume remainder of alphabetical string
                while (fread(&c, sizeof(char), 1, file) && isalpha(c));

                // Prepare for new word
                index = 0;
            }
        }
        else if (index > 0)
        {
            // Terminate current word
            word[index] = '\0';
            SIZE++;

            int hash_inedx = hash(word);

            node *newNode = malloc(sizeof(node));
            if (newNode == NULL)
            {
                printf("Memory allocation failed\n");
                return false;
            }

            strcpy(newNode->word, word);
            newNode->next = table[hash_inedx];
            table[hash_inedx] = newNode;

            // Prepare for next word
            index = 0;
        }
    }
    
    fclose(file);
    // for (int i = 0; i < N; i++)
    // {
    //     printf("table[%d]: ", i);
    //     printLinkedList(table[i]);
    // }

    return true;
}


void printLinkedList(node *head)
{
    node *current = head;
    while (current != NULL)
    {
        printf("%s -> ", current->word);
        current = current->next;
    }
    printf("NULL\n");
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return SIZE;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{

    for (int i = 0; i < N; i++)
    {
        node *current = table[i];
        while (current != NULL)
        {
            node *temp = current;
            current = current->next;
            free(temp);
        }
        table[i] = NULL;
    }
    return true;
}
