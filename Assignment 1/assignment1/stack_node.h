#ifndef STACK_NODE_H_INCLUDED
#define STACK_NODE_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "linked_list.h"

//Our Struct for Stack using Linked List Implementation
struct stack_node
{
    struct stack_node* next;
    char song_name[STRING_SIZE];
};

//Pushing to the stack
void push_stack_node(struct stack_node** sn, char* s);

//Function to pop from the stack
void pop_stack_node(struct stack_node** sn, char *s);

//Function to peek at the top of the stack
void top_stack_node(struct stack_node* sn, char *s);

//Function to count the number of elements in a stack
int num_elements_stack_node(struct stack_node* sn);

//Function to create new Stacked Node
struct stack_node* stack_node_new();

#endif // STACK_NODE_H_INCLUDED
