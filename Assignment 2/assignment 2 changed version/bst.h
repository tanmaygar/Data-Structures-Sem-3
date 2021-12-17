#ifndef BST_H_INCLUDED
#define BST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define STRING_SIZE 10
#define INPUT_SIZE 100

//Structure of Binary Search Tree node
struct BST_node
{
    char song_name[STRING_SIZE];
    struct BST_node *left;
    struct BST_node *right;
};

//Function to allocate memory for new node of BST
struct BST_node *new_BST_node(char *s)
{
    struct BST_node *new_node = (struct BST_node *)malloc(sizeof(struct BST_node));
    strcpy(new_node->song_name, s);
    new_node->left = NULL;
    new_node->right = NULL;
    
    return new_node;
}

//Function to create a BST from a given preorder traversal and also given that it is in lexicographic order
struct BST_node* make_bst_tree(char song_list_preorder[STRING_SIZE][STRING_SIZE], int *preorder_position_number, int begin, int end, int size)
{
    if(*preorder_position_number >= size || begin > end)
    {
        return NULL;
    }

    //Allocate memory for a new node which needs to be added
    struct BST_node* new_node = new_BST_node(song_list_preorder[*preorder_position_number]);
    (*preorder_position_number)++;

    //Returns the node when there is only 1 element present in the array
    if(begin == end)
    {
        return new_node;
    }

    int index;
    for(index = begin; index <= end; index++)
    {
        if(strcmp(new_node->song_name, song_list_preorder[index]) < 0)
        {
            break;
        }
    }

    new_node->left = make_bst_tree(song_list_preorder, preorder_position_number, *preorder_position_number, index - 1, size);
    new_node->right = make_bst_tree(song_list_preorder, preorder_position_number, index, end, size);
}

//Function to print Inorder Traversal of BST
void print_order(struct BST_node *node)
{
    if (node == NULL)
    {
        return;
    }

    print_order(node->left);
    printf("%s|", node->song_name);
    print_order(node->right);
}

//Function to search a particular key in a BST and returns the pointer to the node
struct BST_node *search_key_BST(struct BST_node *node, char *key)
{
    //If node is NULL it return NULL
    if (node == NULL)
    {
        return NULL;
    }

    //If given key matches with the song name of the node it returns pointer to the node
    if (strcmp(node->song_name, key) == 0)
    {
        return node;
    }

    //If key lies ahead of the song_name of the node then it recurses to the right subtree
    if (strcmp(node->song_name, key) < 0)
    {
        return search_key_BST(node->right, key);
    }

    //If key lies behind of the song_name of the node then it recurses to the left subtree
    if (strcmp(node->song_name, key) > 0)
    {
        return search_key_BST(node->left, key);
    }
}

//Function to find the next element to fill the gaps when a partiuclar node needs to be deleted
struct BST_node *find_next_for_deletion_BST(struct BST_node *node)
{
    if (node == NULL)
    {
        return NULL;
    }

    struct BST_node *tmp = node;

    while (tmp->left != NULL)
    {
        tmp = tmp->left;
    }
    return tmp;
}

//Function to delete a node from BST
struct BST_node *delete_node_BST(struct BST_node *node, char *key)
{
    //If node is NULL it returns NULL
    if (node == NULL)
    {
        return NULL;
    }

    //If key lies ahead of song_name of the node then it recurses to the right subtree
    if (strcmp(node->song_name, key) < 0)
    {
        node->right = delete_node_BST(node->right, key);
    }
    //If key lies behind of song_name of the node then it recurses to the left subtree
    else if (strcmp(node->song_name, key) > 0)
    {
        node->left = delete_node_BST(node->left, key);
    }
    //If key matches with the song_name of the node then it proceeds to delete it
    else
    {
        //temporary pointer
        struct BST_node *tmp;
        if (node->left == NULL)
        {
            tmp = node->right;
            //free(node);
            return tmp;
        }
        else if (node->right == NULL)
        {
            tmp = node->left;
            return tmp;
        }
        else
        {
            //Finding the successor to replace the node
            struct BST_node *new_tmp = find_next_for_deletion_BST(node->right);
            strcpy(node->song_name, new_tmp->song_name);
            node->right = delete_node_BST(node->right, new_tmp->song_name);
        }
    }
    return node;
}

#endif // BST_H_INCLUDED