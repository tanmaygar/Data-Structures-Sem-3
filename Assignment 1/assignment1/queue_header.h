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
struct queue_header* queue_header_new();

//Function to enqueue or adding to the queue
void enq_queue_header(struct queue_header* Q, char *s);

//Function to dequeue or deleting from the queue
void deq_queue_header(struct queue_header* Q, char *s);

//Function to check if queue is empty or not
bool stack_node_isempty(struct queue_header* Q);

#endif // QUEUE_HEADER_H_INCLUDED
