#ifndef BTREE_H_INCLUDED
#define BTREE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>
#define NUM_KEYS 3
#define MINIMUM_DEGREE 2
#define COORD 2
#define FAST 1
#define DISK 10
#define MEM_CELL_T 10
#define MEM_ROW_N 3

bool memory_bool_btree[MEM_ROW_N][2 * MEM_CELL_T] = {{false}};

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
    if(i < node->num_keys && input_key == node->key[i])
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
        if(node->coord[0] == node->children[i]->coord[0])
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
    // z->key[0] = y->key[2];
    for(int j = 0; j < MINIMUM_DEGREE-1; j++)
    {
        z->key[j] = y->key[j + MINIMUM_DEGREE];
    }
    for(int j = MINIMUM_DEGREE-1; j < 2*MINIMUM_DEGREE-1; j++)
    {
        z->key[j] = 0;
    }
    if(!(y->leaf))
    {
        for(int j = 0; j < MINIMUM_DEGREE; j++)
        {
            z->children[j] = y->children[j + MINIMUM_DEGREE];
        }
    }
    y->num_keys = MINIMUM_DEGREE - 1;
    for(int j = node->num_keys; j > index; j--)
    {
        node->children[j + 1] = node->children[j];
    }
    node->children[index+1] = z;
    for(int j = node->num_keys; j >= index; j--)//made changes
    {
        node->key[j+1] = node->key[j];
    }
    node->key[index] = y->key[1];
    node->num_keys = node->num_keys + 1;
    for(int j = 1; j < 2*MINIMUM_DEGREE-1; j++)
    {
        y->key[j] = 0;
    }
    for(int j = MINIMUM_DEGREE; j < 2*MINIMUM_DEGREE; j++)
    {
        y->children[j] = NULL;
    }
}

void BTREE_insert_non_full(struct BTREE_node* node, int num)
{
    int i = node->num_keys - 1;
    if(node->leaf)
    {
        while(i >= 0 && num < node->key[i])
        {
            node->key[i+1] = node->key[i];
            i--;
        }
        node->key[i + 1] = num;
        node->num_keys =  node->num_keys + 1;
    }
    else
    {
        while(i >= 0 && num < node->key[i])
        {
            i--;
        }
        i++;
        if(node->children[i]->num_keys == 2*MINIMUM_DEGREE-1)
        {
            BTREE_node_split_child(node, i);
            if(num > node->key[i])
            {
                i++;
            }
        }

        BTREE_insert_non_full(node->children[i], num);
        
    }
}

void insert_node_BTREE(struct BTREE_node** node, int num)
{
    struct BTREE_node *r = (*node);
    if((*node)->num_keys == 2*MINIMUM_DEGREE-1)
    {
        struct BTREE_node *s = new_BTREE_node();
        (*node) = s;
        s->leaf = false;
        s->num_keys = 0;
        s->children[0] = r;
        BTREE_node_split_child(s, 0);
        BTREE_insert_non_full(s, num);

    }
    else
    {
        BTREE_insert_non_full(r, num);
    }
}

int find_predecessor(struct BTREE_node* node)
{
    if(node->children[node->num_keys] == NULL)
    {
        return node->key[node->num_keys-1];
    }
    return find_predecessor(node->children[node->num_keys]);
}

int find_successor(struct BTREE_node* node)
{
    if(node->children[0] == NULL)
    {
        return node->key[0];
    }
    return find_successor(node->children[0]);
}


void delete_node_BTREE(struct BTREE_node** node, int num)
{
    //when it is leaf node case 1
    if((*node)->leaf)
    {
        int j = 0;
        while(j < (*node)->num_keys && num > (*node)->key[j])
        {
            j++;
        }
        for(int i = j; i < (*node)->num_keys - 1; i++)
        {
            (*node)->key[i] = (*node)->key[i+1];
        }
        (*node)->num_keys--;
        for(int i = (*node)->num_keys; i < 2*MINIMUM_DEGREE-1; i++)
        {
            (*node)->key[i] = 0;
        }
    }
    else
    {
        int j = 0;
        while(j < (*node)->num_keys && num > (*node)->key[j])
        {
            j++;
        }
        //case 2
        if((*node)->key[j] == num)
        {
            //case 2a
            if((*node)->children[j]->num_keys >= MINIMUM_DEGREE)
            {
                //Find predecessor and recusviley delete
                int pred = find_predecessor((*node)->children[j]);
                (*node)->key[j] = pred;
                delete_node_BTREE(&(*node)->children[j], pred);
            }
            //case 2b
            else if ((*node)->children[j+1]->num_keys >= MINIMUM_DEGREE)
            {
                //Find successor and recusviley delete
                int succ = find_successor((*node)->children[j + 1]);
                (*node)->key[j] = succ;
                delete_node_BTREE(&(*node)->children[j + 1], succ);
            }
            //case 2c
            else
            {
                struct BTREE_node* y = (*node)->children[j];
                y->key[y->num_keys] = (*node)->key[j];
                for(int i = y->num_keys+1; i < 2 * MINIMUM_DEGREE -1; i++)
                {
                    y->key[i] = (*node)->children[j + 1]->key[i - y->num_keys-1];
                }
                y->num_keys = 2 * MINIMUM_DEGREE - 1;
                for(int i = y->num_keys-1; i < 2 * MINIMUM_DEGREE; i++)
                {
                    y->children[i] = (*node)->children[j+1]->children[i- MINIMUM_DEGREE];
                }
                (*node)->children[j+1] = NULL;
                for(int i = j; i < 2*MINIMUM_DEGREE-2; i++)
                {
                    (*node)->key[i] = (*node)->key[i+1];
                }
                (*node)->num_keys--;
                for(int i = (*node)->num_keys; i < 2*MINIMUM_DEGREE-1; i++)
                {
                    (*node)->key[i] = 0;
                }
                
                // (*node)->children[j+1] = NULL;
                // for(int i = j; i < 2* MINIMUM_DEGREE -2; i++)
                // {
                //     (*node)->key[i] = (*node)->key[i+1];
                // }
                // for(int i = j + 1; i < 2* MINIMUM_DEGREE - 1; i++)
                // {
                //     (*node)->children[i] = (*node)->children[i+1];
                // }
                // (*node)->children[2*MINIMUM_DEGREE-1] = NULL;
                delete_node_BTREE(&(*node)->children[j], num);

            }
            
        }
        //case 3
        else
        {
            if((*node)->children[j]->num_keys >= MINIMUM_DEGREE)
            {
                delete_node_BTREE(&(*node)->children[j], num);
            }
            else if(j == 0)
            {
                if((*node)->children[1]->num_keys >= MINIMUM_DEGREE)
                {
                    (*node)->children[0]->key[1] = (*node)->key[0];
                    (*node)->children[0]->num_keys++;
                    (*node)->children[0]->children[2] = (*node)->children[1]->children[0];

                    (*node)->key[0] = (*node)->children[1]->key[0];
                    for(int i = 0; i < (*node)->children[1]->num_keys-1; i++)
                    {
                        (*node)->children[1]->key[i] = (*node)->children[1]->key[i+1];
                    }

                    for(int i = 0; i < (*node)->children[1]->num_keys; i++)
                    {
                        (*node)->children[1]->children[i] = (*node)->children[1]->children[i+1];
                    }
                    (*node)->children[1]->num_keys--;
                    for(int i = (*node)->children[1]->num_keys; i < 2*MINIMUM_DEGREE-1; i++)
                    {
                        (*node)->children[1]->key[i] = 0;
                    }
                    for(int i = (*node)->children[1]->num_keys + 1; i < 2 * MINIMUM_DEGREE - 1; i++)
                    {
                        (*node)->children[1]->children[i] = NULL;
                    }

                    delete_node_BTREE(&(*node)->children[0], num);
                }
                else
                {
                    //case 3b

                    (*node)->children[0]->key[1] = (*node)->key[0];
                    (*node)->children[0]->key[2] = (*node)->children[1]->key[0];

                    for(int i = (*node)->num_keys+1; i < 2*MINIMUM_DEGREE -1; i++)
                    {
                        (*node)->children[0]->key[i] = (*node)->children[1]->key[i - ((*node)->num_keys+1)];
                    }

                    for(int i = (*node)->num_keys + 1; i < 2 * MINIMUM_DEGREE; i++)
                    {
                        (*node)->children[0]->children[i] = (*node)->children[1]->children[i - ((*node)->num_keys+1)];
                    }

                    (*node)->children[0]->num_keys = 2*MINIMUM_DEGREE-1;

                    for(int i = 0; i < (*node)->num_keys -1; i++)
                    {
                        (*node)->key[i] = (*node)->key[i+1];
                    }
                    for(int i = (*node)->num_keys -1; i < 2*MINIMUM_DEGREE-1; i++)
                    {
                        (*node)->key[i] = 0;
                    }
                    for(int i = 1; i < (*node)->num_keys; i++)
                    {
                        (*node)->children[i] = (*node)->children[i+1];
                    }
                    (*node)->num_keys--;
                    for(int i = (*node)->num_keys; i < 2*MINIMUM_DEGREE; i++)
                    {
                        (*node)->children[i] = NULL;
                    }

                    delete_node_BTREE(&(*node)->children[0], num);
                }
            }
            else if(j == (*node)->num_keys)
            {
                if((*node)->children[j-1]->num_keys >= MINIMUM_DEGREE)
                {
                    (*node)->children[j]->key[1] = (*node)->children[j]->key[0];
                    (*node)->children[j]->key[0] = (*node)->key[j-1];
                    (*node)->key[j-1] = (*node)->children[j-1]->key[(*node)->children[j-1]->num_keys-1];

                    (*node)->children[j]->children[2] = (*node)->children[j]->children[1];
                    (*node)->children[j]->children[1] = (*node)->children[j]->children[0];
                    (*node)->children[j]->children[0] = (*node)->children[j-1]->children[(*node)->children[j-1]->num_keys];
                    (*node)->children[j]->num_keys++;

                    (*node)->children[j-1]->num_keys--;
                    for(int i = (*node)->children[j-1]->num_keys; i < 2*MINIMUM_DEGREE-1; i++)
                    {
                        (*node)->children[j-1]->key[i] = 0;
                    }
                    for(int i = (*node)->children[j-1]->num_keys + 1; i < 2*MINIMUM_DEGREE; i++)
                    {
                        (*node)->children[j-1]->children[i] = NULL;
                    }

                    delete_node_BTREE(&(*node)->children[j], num);
                }
                else
                {
                    //case 3b

                    (*node)->children[(*node)->num_keys-1]->key[1] = (*node)->key[(*node)->num_keys-1];
                    for(int i = MINIMUM_DEGREE; i < 2*MINIMUM_DEGREE-1; i++)
                    {
                        (*node)->children[(*node)->num_keys-1]->key[i] = (*node)->children[(*node)->num_keys]->key[i- MINIMUM_DEGREE];
                    }
                    for(int i = MINIMUM_DEGREE; i < 2*MINIMUM_DEGREE; i++)
                    {
                        (*node)->children[(*node)->num_keys-1]->children[i] = (*node)->children[(*node)->num_keys]->children[i- MINIMUM_DEGREE];
                    }

                    (*node)->num_keys--;
                    for(int i = (*node)->num_keys; i < 2*MINIMUM_DEGREE-1; i++)
                    {
                        (*node)->key[i] = 0;
                    }
                    for(int i = (*node)->num_keys + 1; i < 2*MINIMUM_DEGREE; i++)
                    {
                        (*node)->children[i] = 0;
                    }

                    delete_node_BTREE(&(*node)->children[(*node)->num_keys], num);
                    
                }
            }
            else if((*node)->children[j-1]->num_keys == MINIMUM_DEGREE-1 && (*node)->children[j+1]->num_keys == MINIMUM_DEGREE-1)
            {
                //case 3b

                (*node)->children[j-1]->key[1] = (*node)->key[j-1];
                for(int i = MINIMUM_DEGREE; i < 2*MINIMUM_DEGREE-1; i++)
                {
                    (*node)->children[j-1]->key[i] = (*node)->children[j]->key[i-MINIMUM_DEGREE];
                }
                for(int i = MINIMUM_DEGREE; i < 2*MINIMUM_DEGREE; i++)
                {
                    (*node)->children[j-1]->children[i] = (*node)->children[j]->children[i-MINIMUM_DEGREE];
                }
                for(int i = j-1; i < 2*MINIMUM_DEGREE-2; i++)
                {
                    (*node)->key[i] = (*node)->key[i+1];
                }
                (*node)->num_keys--;
                for(int i = (*node)->num_keys; i < 2*MINIMUM_DEGREE-1; i++)
                {
                    (*node)->key[i] = 0;
                }
                for(int i = j+1; i < 2*MINIMUM_DEGREE-1; i++)
                {
                    (*node)->children[i] = (*node)->children[i+1];
                }
                for(int i = (*node)->num_keys+1; i < 2* MINIMUM_DEGREE; i++)
                {
                    (*node)->children[i] = NULL;
                }

                delete_node_BTREE(&(*node)->children[j-1], num);

            }
            else if((*node)->children[j-1]->num_keys >= MINIMUM_DEGREE)
            {
                (*node)->children[j]->key[1] = (*node)->children[j]->key[0];
                (*node)->children[j]->children[2] = (*node)->children[j]->children[1];
                (*node)->children[j]->children[1] = (*node)->children[j]->children[0];
                (*node)->children[j]->children[0] = (*node)->children[j-1]->children[(*node)->children[j-1]->num_keys];

                (*node)->children[j]->key[0] = (*node)->key[j-1];
                (*node)->key[j-1] = (*node)->children[j-1]->key[(*node)->children[j-1]->num_keys-1];

                (*node)->children[j-1]->num_keys--;
                (*node)->children[j]->num_keys++;

                for(int i = (*node)->children[j-1]->num_keys; i < 2*MINIMUM_DEGREE-1; i++)
                {
                    (*node)->children[j-1]->key[i] = 0;
                }
                for(int i = (*node)->children[j-1]->num_keys + 1; i < 2*MINIMUM_DEGREE; i++)
                {
                    (*node)->children[j-1]->children[i] = NULL;
                }

                delete_node_BTREE(&(*node)->children[j], num);
            }
            else
            {
                (*node)->children[j]->key[1] = (*node)->key[j];
                (*node)->key[j] = (*node)->children[j+1]->key[0];
                (*node)->children[j]->children[2] = (*node)->children[j+1]->children[0];
                (*node)->children[j]->num_keys++;

                for(int i = 0; i < (*node)->children[j+1]->num_keys-1; i++)
                {
                    (*node)->children[j+1]->key[i] = (*node)->children[j+1]->key[i+1];
                }
                for(int i = 0; i < (*node)->children[j+1]->num_keys; i++)
                {
                    (*node)->children[j+1]->children[i] = (*node)->children[j+1]->children[i+1];
                }

                (*node)->children[j+1]->num_keys--;
                for(int i = (*node)->children[j+1]->num_keys; i < 2*MINIMUM_DEGREE-1; i++)
                {
                    (*node)->children[j+1]->key[i] = 0;
                }
                for(int i = (*node)->children[j+1]->num_keys + 1; i < 2*MINIMUM_DEGREE; i++)
                {
                    (*node)->children[j+1]->children[i] = NULL;
                }
                // (*node)->children[j+1]->children[(*node)->children[j+1]->num_keys] = NULL;
                // (*node)->children[j+1]->key[(*node)->children[j+1]->num_keys-1] = 0;
                // (*node)->children[j+1]->num_keys--;

                delete_node_BTREE(&(*node)->children[j], num);
            }

            
        }
    }
}
#endif // BTREE_H_INCLUDED