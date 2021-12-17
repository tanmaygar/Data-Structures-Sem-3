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
    int num;
    struct BST_node *left;
    struct BST_node *right;
};

//Function to allocate memory for new node of BST
struct BST_node *new_BST_node(int key)
{
    struct BST_node *new_node = (struct BST_node *)malloc(sizeof(struct BST_node));
    //strcpy(new_node->song_name, s);
    new_node->num = key;
    new_node->left = NULL;
    new_node->right = NULL;
    
    return new_node;
}

//Function to print Inorder Traversal of BST
void print_order(struct BST_node *node)
{
    if (node == NULL)
    {
        return;
    }

    print_order(node->left);
    printf("%d|", node->num);
    print_order(node->right);
}

//Function to search a particular key in a BST and returns the pointer to the node
struct BST_node *search_key_BST(struct BST_node *node, int key)
{
    //If node is NULL it return NULL
    if (node == NULL)
    {
        return NULL;
    }

    //If given key matches with the number of the node it returns pointer to the node
    //if (strcmp(node->song_name, s) == 0)
    if (node->num == key)
    {
        return node;
    }

    //If key lies ahead of the number of the node then it recurses to the right subtree
    //if (strcmp(node->song_name, s) < 0)
    if (node->num > key)
    {
        return search_key_BST(node->right, key);
    }

    //If key lies behind of the number of the node then it recurses to the left subtree
    //if (strcmp(node->song_name, s) > 0)
    if (node->num < key)
    {
        return search_key_BST(node->left, key);
    }
}

//Function to insert a node into a bst tree for a given key
struct BST_node *insert_node_BST(struct BST_node **node, int key)
{
    //If node is NULL it allocates memory and enters the key for the node
    if ((*node) == NULL)
    {
        (*node) = new_BST_node(key);
    }
    //If key lies behind the number of the node it recurses to left subtree
    //else if (strcmp((*node)->song_name, s) > 0)
    else if (key < (*node)->num)
    {
        (*node)->left = insert_node_BST(&(*node)->left, key);
    }
    //If key lies ahead the number of the node it recurses to right subtree
    else
    {
        (*node)->right = insert_node_BST(&(*node)->right, key);
    }
    
    return (*node);
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
struct BST_node *delete_node_BST(struct BST_node *node, int key)
{
    //If node is NULL it returns NULL
    if (node == NULL)
    {
        return NULL;
    }

    //If key lies ahead of number of the node then it recurses to the right subtree
    //if (strcmp(node->song_name, s) < 0)
    if (node->num > key)
    {
        node->right = delete_node_BST(node->right, key);
    }
    //If key lies behind of number of the node then it recurses to the left subtree
    //else if (strcmp(node->song_name, s) > 0)
    else if (node->num < key)
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
            node = NULL;
            return tmp;
        }
        else if (node->right == NULL)
        {
            tmp = node->left;
            node = NULL;
            return tmp;
        }
        else
        {
            //Finding the successor to replace the node and then deleting the node
            struct BST_node *new_tmp = find_next_for_deletion_BST(node->right);
            //strcpy(node->song_name, new_tmp->song_name);
            node->num = new_tmp->num;
            //node->right = delete_node_BST(node->right, new_tmp->song_name);
            node->right = delete_node_BST(node->right, new_tmp->num);
        }
    }
    return node;
}

#endif // BST_H_INCLUDED