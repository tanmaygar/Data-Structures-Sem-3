
#include "stack_node.h"

void push_stack_node(struct stack_node** sn, char* s)
{
    //Creating new pointer to the node
    struct stack_node* new_lln = (struct stack_node*)malloc(sizeof(struct stack_node));

    //Initializing the node with song name
    strcpy(new_lln->song_name, s);

    //Initializing the pointer to the next as NULL
    new_lln->next = NULL;

    //Since we are pushing we make the next pointer the root
    new_lln->next = *sn;

    //Changing the root node to the newly added node
    *sn = new_lln;

    //printf("pushed song %s to stack\n", s);
    return;

}

//Function to pop from the stack
void pop_stack_node(struct stack_node** sn, char *s)
{
    //If stack is empty it returns empty
    if(*sn == NULL)
    {
        char tmp[STRING_SIZE] = "empty";
        strcpy(s, tmp);
    }
    else
    {
        //else it advances the pointer to the next node
        strcpy(s, (*sn)->song_name);
        *sn = (*sn)->next;
    }
    return;
}

//Function to peek at the top of the stack
void top_stack_node(struct stack_node* sn, char *s)
{
    if(sn == NULL)
    {
        //if stack is empty it returns empty
        char tmp[STRING_SIZE] = "empty";
        strcpy(s, tmp);
    }
    else
    {
        //else it returns the name of the song
        strcpy(s, sn->song_name);
    }
}

int num_elements_stack_node(struct stack_node* sn)
{
    //Creating a temporary stack to count the number of elements
    struct stack_node* new_lln = (struct stack_node*)malloc(sizeof(struct stack_node));

    //Counter to count the number of elements
    int counter = 0;
    char tmp_str[STRING_SIZE];

    //Popping from original stack and then pushing to temporary stack 
    while(sn != NULL)
    {
        pop_stack_node(&sn, tmp_str);
        push_stack_node(&new_lln, tmp_str);
        counter++;
    }
    // if (counter != 0)
    // {
    //     while (new_lln != NULL)
    //     {
    //         pop_stack_node(&new_lln, tmp_str);
    //         push_stack_node(&sn, tmp_str);
    //     }
    // }

    //Popping from temporary stack and pushing back to original stack
    for (int i = 0; i < counter; i++)
    {
        pop_stack_node(&new_lln, tmp_str);
        push_stack_node(&sn, tmp_str);
    }

    //Return counter
    return counter;
}

//Function to create new Stacked Node
struct stack_node* stack_node_new()
{
    //Memory allocation for new queue
    struct stack_node* sn = (struct stack_node*)malloc(sizeof(struct stack_node));
    sn->next = NULL;
    return sn;
}