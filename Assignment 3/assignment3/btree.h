#ifndef BTREE_H_INCLUDED
#define BTREE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define NUM_KEYS 3
#define MINIMUM_DEGREE 2
#define COORD 2
#define FAST 1
#define DISK 10
#define MEM_CELL_T 10


struct BTREE_node
{
    bool leaf;
    int num_keys;
    int key[NUM_KEYS];
    struct BTREE_node *children[NUM_KEYS + 1];
    int coord[COORD];

};

struct BTREE_pair_return
{
    struct BTREE_node *btree_node;
    int index;
};

//
// write function for making children null
//

//Function to allocate memory for new node of BST
struct BTREE_node *new_BTREE_node()
{
    struct BTREE_node *new_node = (struct BTREE_node *)malloc(sizeof(struct BTREE_node));
    new_node->leaf = true;
    new_node->num_keys = 0;
    for(int i = 0; i < NUM_KEYS + 1; i++)
    {
        new_node->children[i] = NULL;
    }
    for(int i = 0; i < NUM_KEYS; i++)
    {
        new_node->key[i] = 0;
    }
    return new_node;
}

void search_BTREE_node(struct BTREE_node *node, int input_key, struct BTREE_pair_return** return_pair)
{
    int i = 0;
    while(i < node->num_keys && input_key > node->key[i])
    {
        i++;
    }
    if(i <= node->num_keys && input_key == node->key[i])
    {
        (*return_pair)->btree_node = node;
        (*return_pair)->index = i;
        return;
    }
    else if (node->leaf)
    {
        (*return_pair) = NULL;
        return;
    }
    else
    {
        if(node->coord[0] == node->children[i-1]->coord[0])
        {
            //(*time) = (*time) + FAST;
        }
        else
        {
            //(*time) = (*time) + DISK;
        }
        search_BTREE_node(node->children[i], input_key, return_pair);
    }
}



void BTREE_node_split_child(struct BTREE_node *node, int index)
{
    struct BTREE_node *z = new_BTREE_node();
    struct BTREE_node *y = node->children[index];
    z->leaf = y->leaf;
    z->num_keys = MINIMUM_DEGREE - 1;
    z->key[0] = y->key[2];
    if(!(y->leaf))
    {
        for(int j = 0; j < 2; j++)
        {
            z->children[j] = y->children[j + 2];
        }
    }
    y->num_keys = 1;
    for(int j = node->num_keys + 1; j > index + 1; j--)
    {
        node->children[j + 1] = node->children[j];
    }
    node->children[index+1] = z;
    for(int j = node->num_keys; j > index; j--)
    {
        node->key[j+1] = node->key[j];
    }
    node->key[index] = node->key[2];
    node->num_keys = node->num_keys + 1;
}

void BTREE_insert_non_full(struct BTREE_node* node, int num)
{
    
}

void insert_node_BTREE(struct BTREE_node** node, int num)
{
    struct BTREE_node *r = (*node);
    if((*node)->num_keys == 3)
    {
        struct BTREE_node *s = new_BTREE_node();
        (*node) = s;
        s->leaf = false;
        s->num_keys = 0;
        s->children[0] = r;
        BTREE_node_split_child(s, 0);
        //BTREE_insert_non_full(s, num);

    }
    else
    {
        //BTREE_insert_non_full(r, num);
    }
}
#endif // BTREE_H_INCLUDED