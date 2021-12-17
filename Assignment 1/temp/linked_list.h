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
bool find_linked_list_node(struct linked_list_node** lln, char* s)
{
    //temporary pointer to input pointer of Linked List
    struct linked_list_node* curr = *lln;

    //Loop while reaching the end of list
    while(curr != NULL)
    {
        //If the song exists return true
        if(strcmp(s, curr->song_name) == 0)
        {
            return true;
        }
        //If song does not exist then pointer proceeds format
        curr = curr->next;
    }

    //Returns false when song does not exist
    return false;
}

//Function to insert song to the Linked List
void insert_linked_list_node(struct linked_list_node** lln, char* s)
{
    //temporary pointer to input pointer of Linked List
    struct linked_list_node *curr = *lln;

    //New pointer for adding the new element, allocating memory
    struct linked_list_node* new_lln = (struct linked_list_node*)malloc(sizeof(struct linked_list_node));

    //Initializing the song to the new pointer
    strcpy(new_lln->song_name, s);

    //Initializing the next pointer as NULL
    new_lln->next = NULL;

    //If the Linked List is Empty then it assigns this pointer as the head
    if(*lln == NULL)
    {
        *lln = new_lln;
    }
    else
    {
        //If not empty then it adds to last pointer
        while(curr->next != NULL)
        {
            curr = curr->next;
        }
        curr->next = new_lln;
    }

    //Function returns are finishing
    return;

}



#endif // LINKED_LIST_H_INCLUDED
