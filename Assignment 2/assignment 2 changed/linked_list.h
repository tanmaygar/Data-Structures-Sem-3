#ifndef LINKED_LIST_H_INCLUDED
#define LINKED_LIST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define STRING_SIZE 10

//Our struct for Linked List
struct linked_list_node
{
    //Pointer to the next node in the Linked List
    struct linked_list_node* next;

    //Name of the song in string format
    char song_name[STRING_SIZE];
};

//Boolean function to find if the song exists or not in Linked List
bool find_linked_list_node(struct linked_list_node** lln, char* s);

//Function to insert song to the Linked List and also works as new
void insert_linked_list_node(struct linked_list_node** lln, char* s);

//Function to create new Linked List
struct linked_list_node* linked_list_node_new();

#endif // LINKED_LIST_H_INCLUDED
