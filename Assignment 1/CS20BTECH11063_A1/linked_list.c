
#include "linked_list.h"

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

//Function to insert song to the Linked List and also works as new
void insert_linked_list_node(struct linked_list_node** lln, char* s)
{
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
        // //If not empty then it adds to head
        new_lln->next = *lln;
        *lln = new_lln;

    }

    //Function returns are finishing
    return;

}

//Function to create new Linked List
struct linked_list_node* linked_list_node_new()
{
    //Memory allocation for new queue
    struct linked_list_node* lln = (struct linked_list_node*)malloc(sizeof(struct linked_list_node));
    lln->next = NULL;
    return lln;
}
