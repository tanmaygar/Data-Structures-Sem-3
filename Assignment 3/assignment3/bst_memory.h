#ifndef BST_MEMORY_H_INCLUDED
#define BST_MEMORY_H_INCLUDED

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
    struct BST_mem_node *left;
    struct BST_mem_node *right;
    int coord[COORD];
};

struct BST_mem_node *new_BST_mem_node(int key)
{
    struct BST_mem_node *new_node = (struct BST_mem_node *)malloc(sizeof(struct BST_mem_node));
    new_node->value = key;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->coord[0] = 0;
    new_node->coord[1] = 0;
    return new_node;
}

int find_next_empty_cell(int *secondary_memory)
{
    int i = 0;
    while(secondary_memory[i] >= 2 * MEM_CELL_T - 1)
    {
        i++;
    }
    return i;
}

struct BST_mem_node *search_mem_node(struct BST_mem_node *node, int key, int *time)
{
    if(key == node->value)
    {
        return node;
    }
    else if(key < node->value)
    {
        if(node->coord[0] == node->left->coord[0])
        {
            (*time) = (*time) + FAST;
            return search_mem_node(node->left, key, time);
        }
        else
        {
            (*time) = (*time) + DISK + FAST;
            return search_mem_node(node->left, key, time);
        }
    }
    else if(key > node->value)
    {
        if(node->coord[0] == node->right->coord[0])
        {
            (*time) = (*time) + FAST;
            return search_mem_node(node->right, key, time);
        }
        else
        {
            (*time) = (*time) + DISK + FAST;
            return search_mem_node(node->right, key, time);
        }
    }
    else
    {
        return NULL;
    }
}

struct BST_mem_node* insert_node_BST_mem(struct BST_mem_node **node, int key, int *time, int parent_coord[COORD], int *secondary_memory)
{
    if((*node) == NULL)
    {
        (*node) = new_BST_mem_node(key);
        if(secondary_memory[parent_coord[0]] < 2 * MEM_CELL_T - 1)
        {
            (*node)->coord[0] = parent_coord[0];
            (*node)->coord[1] = ++secondary_memory[parent_coord[0]];
            (*time) = (*time) + FAST + DISK;
        }
        else
        {
            (*node)->coord[0] = find_next_empty_cell(secondary_memory);
            (*node)->coord[1] = ++secondary_memory[(*node)->coord[0]];
            (*time) = (*time) + FAST + DISK + DISK;
        }
    }
    else if(key < (*node)->value)
    {
        if((*node)->left == NULL)
        {
            (*node)->left = insert_node_BST_mem(&(*node)->left, key, time, (*node)->coord, secondary_memory);
        }
        else if((*node)->coord[0] == (*node)->left->coord[0])
        {
            (*time) = (*time) + FAST;
            (*node)->left = insert_node_BST_mem(&(*node)->left, key, time, (*node)->coord, secondary_memory);
        }
        else
        {
            (*time) = (*time) + DISK;
            (*node)->left = insert_node_BST_mem(&(*node)->left, key, time, (*node)->coord, secondary_memory);
        }
    }
    else
    {
        if((*node)->right == NULL)
        {
            (*node)->right = insert_node_BST_mem(&(*node)->right, key, time, (*node)->coord, secondary_memory);
        }
        else if((*node)->coord[0] == (*node)->right->coord[0])
        {
            (*time) = (*time) + FAST;
            (*node)->right = insert_node_BST_mem(&(*node)->right, key, time, (*node)->coord, secondary_memory);
        }
        else
        {
            (*time) = (*time) + DISK;
            (*node)->right = insert_node_BST_mem(&(*node)->right, key, time, (*node)->coord, secondary_memory);
        }
    }
    return (*node);
}

struct BST_mem_node *find_next_for_deletion_BST_mem(struct BST_mem_node *node)
{
    if (node == NULL)
    {
        return NULL;
    }

    struct BST_mem_node *tmp = node;

    while (tmp->left != NULL)
    {
        tmp = tmp->left;
    }
    return tmp;
}

struct BST_mem_node *find_next_deletion_BST_mem(struct BST_mem_node *node, int *time, int parent_coord[COORD])
{
    if(node == NULL)
    {
        return NULL;
    }
    struct BST_mem_node *tmp = node;
    int tmp_coord[COORD] = {parent_coord[0], parent_coord[1]};
    while(tmp->left != NULL)
    {
        if(tmp->coord[0] == tmp_coord[0])
        {
            (*time) = (*time) + FAST;
        }
        else
        {
            (*time) = (*time) + DISK;
        }
        tmp_coord[0] = tmp->coord[0];
        tmp_coord[1] = tmp_coord[1];
        tmp = tmp->left;
    }
    return tmp;
}

struct BST_mem_node* delete_node_BST_mem(struct BST_mem_node *node, int key, int *time, int parent_coord[COORD], int *secondary_memory)
{
    if(node == NULL)
    {
        return NULL;
    }
    if(key < node->value)
    {
        if(parent_coord[0] == node->coord[0])
        {
            (*time) = (*time) + FAST;
            node->left = delete_node_BST_mem(node->left, key, time, node->coord, secondary_memory);
        }
        else
        {
            (*time) = (*time) + DISK;
            node->left = delete_node_BST_mem(node->left, key, time, node->coord, secondary_memory);
        }
    }
    else if(key > node->value)
    {
        if(parent_coord[0] == node->coord[0])
        {
            (*time) = (*time) + FAST;
            node->right = delete_node_BST_mem(node->right, key, time, node->coord, secondary_memory);
        }
        else
        {
            (*time) = (*time) + DISK;
            node->right = delete_node_BST_mem(node->right, key, time, node->coord, secondary_memory);
        }
    }
    else
    {
        struct BST_mem_node* tmp;
        if(node->left == NULL && node->right == NULL)
        {
            tmp = NULL;
            node = NULL;
            (*time) = (*time) + DISK;
            return tmp;
        }
        else if(node->left == NULL)
        {
            if(parent_coord[0] == node->right->coord[0])
            {
                (*time) = (*time) + FAST;
            }
            else
            {
                (*time) = (*time) + DISK;
            }
            tmp = node->right;
            node = NULL;
            (*time) = (*time) + DISK;
            return tmp;
        }
        else if(node->right == NULL)
        {
            if(parent_coord[0] == node->left->coord[0])
            {
                (*time) = (*time) + FAST;
            }
            else
            {
                (*time) = (*time) + DISK;
            }
            tmp = node->left;
            node = NULL;
            (*time) = (*time) + DISK;
            return tmp;
        }
        else
        {
            //struct BST_mem_node *new_tmp = find_next_for_deletion_BST_mem(node->right);
            struct BST_mem_node *new_tmp = find_next_deletion_BST_mem(node->right, time, node->coord);
            node->value = new_tmp->value;
            node->right = delete_node_BST_mem(node->right, new_tmp->value, time, node->coord, secondary_memory);
        }
    }
}

#endif // BST_MEM_H_INCLUDED
