#ifndef BST_MEM_H_INCLUDED
#define BST_MEM_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>
#define COORD 2
#define FAST 1
#define DISK 10
#define MEM_CELL_T 10
struct BST_mem_node 
{
    int value;
    int left_coord[COORD];
    int right_coord[COORD];
    struct BST_mem_node *left;
    struct BST_mem_node *right;
};

struct BST_mem_node *new_BST_mem_node(int key)
{
    struct BST_mem_node *new_node = (struct BST_mem_node *)malloc(sizeof(struct BST_mem_node));
    new_node->value = key;
    new_node->left_coord[0] = INT_MIN;
    new_node->left_coord[1] = INT_MIN;
    new_node->right_coord[0] = INT_MIN;
    new_node->right_coord[1] = INT_MIN;
    new_node->left = NULL;
    new_node->right = NULL;
    
    return new_node;
}

struct BST_mem_node *search_BST_mem_node(struct BST_mem_node *node, int key, int coord[COORD], int *time)
{
    if(node->value == key)
    {
        return node;
    }
    else if(node->value > key)
    {
        if(coord[0] == node->left_coord[0])
        {
            (*time) = (*time) + FAST;
            search_BST_mem_node(node->left, key, node->left_coord, time);
        }
        else
        {
            (*time) = (*time) + DISK;
            search_BST_mem_node(node->left, key, node->left_coord, time);
        }
    }
    else if(node->value < key)
    {
        if(coord[0] == node->right_coord[0])
        {
            (*time) = (*time) + FAST;
            search_BST_mem_node(node->right, key, node->right_coord, time);
        }
        else
        {
            (*time) = (*time) + DISK;
            search_BST_mem_node(node->right, key, node->right_coord, time);
        }
    }
    else
    {
        return NULL;
    }
}

struct BST_mem_node *insert_node_BST_mem(struct BST_mem_node **node, int key, int coord_node[COORD], int *secondary_memory, int row, int *time)
{
    if((*node) == NULL)
    {
        (*node) = new_BST_mem_node(key);
        if(secondary_memory[row] < 2 * MEM_CELL_T - 1)
        {
            secondary_memory[row]++;
            (*time) = (*time) + DISK + FAST;
            coord_node[0] = row;
            coord_node[1] = secondary_memory[row];
        }
        else
        {
            secondary_memory[row + 1]++;
            (*time) = (*time) + DISK + FAST;
            coord_node[0] = row + 1;
            coord_node[1] = secondary_memory[row + 1];
        }
    }
    else if(key < (*node)->value)
    {
        if(coord_node[0] == (*node)->left_coord[0])
        {
            (*time) = (*time) + FAST;
            (*node)->left = insert_node_BST_mem(&(*node)->left, key, (*node)->left_coord, secondary_memory, (*node)->left_coord[0], time);
        }
        else
        {
            (*time) = (*time) + DISK;
            (*node)->left = insert_node_BST_mem(&(*node)->left, key, (*node)->left_coord, secondary_memory, (*node)->left_coord[0], time);
        }
    }
    else if(key > (*node)->value)
    {
        if(coord_node[0] == (*node)->right_coord[0])
        {
            (*time) = (*time) + FAST;
            (*node)->right = insert_node_BST_mem(&(*node)->right, key, (*node)->right_coord, secondary_memory, (*node)->right_coord[0], time);
        }
        else
        {
            (*time) = (*time) + DISK;
            (*node)->right = insert_node_BST_mem(&(*node)->right, key, (*node)->right_coord, secondary_memory, (*node)->right_coord[0], time);
        }
    }

    return (*node);

}

#endif // BST_MEM_H_INCLUDED
