#ifndef QUEUE_HEADER_H_INCLUDED
#define QUEUE_HEADER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "linked_list.h"
#include "stack_node.h"

//Our Struct for Queue using Linked List Implementation
struct queue_header
{
    //Pointer to the front of the queue
    struct linked_list_node* front_queue;

    //Pointer to the end of the queue
    struct linked_list_node* back_queue;
};

//Function to create new Queue
struct queue_header* queue_header_new()
{
    //Memory allocation for new queue
    struct queue_header* Q = (struct queue_header*)malloc(sizeof(struct queue_header));

    //Initializing the back of the queue as NULL
    Q->back_queue = NULL;

    //Initializing the front of the queue as NULL
    Q->front_queue = NULL;

    //Return the queue
    return Q;
}

//Function to enqueue or adding to the queue
void enq_queue_header(struct queue_header* Q, char *s)
{
    //Creating new pointer of for adding element to the queue in the form of a linked list
    struct linked_list_node* p = (struct linked_list_node*)malloc(sizeof(struct linked_list_node));

    //initializing the song name
    strcpy(p->song_name, s);

    //initializing the next pointer as NULL
    p->next = NULL;

    //If back of the queue is NULL then assign the node
    if(Q->back_queue == NULL)
    {
        Q->back_queue = p;
        Q->front_queue = p;
    }
    else
    {
        //else next pointer is assigned
        Q->back_queue->next = p;
        Q->back_queue = p;
    }
}

//Function to dequeue or deleting from the queue
void deq_queue_header(struct queue_header* Q, char *s)
{
    //If front of the queue is NULL then it returns empty
    if(Q->front_queue == NULL)
    {
        char tmp[STRING_SIZE] = "empty";
        strcpy(s, tmp);
    }
    else
    {
        //else it removes the element from the queue
        strcpy(s, Q->front_queue->song_name);
        Q->front_queue = Q->front_queue->next;

        if(Q->front_queue == NULL)
        {
            Q->back_queue = NULL;
        }
    }
    return;
}

//Function to check if queue is empty or not
bool stack_node_isempty(struct queue_header* Q)
{
    if(Q->front_queue == NULL)
    {
        return false;
    }
    return true;
}


#endif // QUEUE_HEADER_H_INCLUDED
