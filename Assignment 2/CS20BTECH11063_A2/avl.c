#include "avl.h"

//Function which returns the height of AVL node
int find_height_AVL(struct AVL_TREE_node *node)
{
    if (node == NULL)
    {
        return 0;
    }
    return node->height;
}

//Function to allocate memory for new AVL tree node
struct AVL_TREE_node *new_AVL_TREE_node(char *s)
{
    struct AVL_TREE_node *new_node = (struct AVL_TREE_node *)malloc(sizeof(struct AVL_TREE_node));
    strcpy(new_node->song_name, s);
    new_node->height = 1;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

//Function to provide with the right rotation of AVL tree node for balancing purpose
void right_rotation_AVL(struct AVL_TREE_node **node)
{
    struct AVL_TREE_node *tmp = (*node)->right;
    (*node)->right = (*node)->right->left;
    tmp->left = *node;

    (*node)->height = 1 + MAX(find_height_AVL((*node)->left), find_height_AVL((*node)->right));
    tmp->height = 1 + MAX(find_height_AVL(tmp->left), find_height_AVL(tmp->right));

    (*node) = tmp;
}

//Function to provide with the left rotation of AVL tree node for balancing purpose
void left_rotation_AVL(struct AVL_TREE_node **node)
{
    struct AVL_TREE_node *tmp = (*node)->left;
    (*node)->left = (*node)->left->right;
    tmp->right = (*node);

    (*node)->height = 1 + MAX(find_height_AVL((*node)->left), find_height_AVL((*node)->right));
    tmp->height = 1 + MAX(find_height_AVL(tmp->left), find_height_AVL(tmp->right));

    (*node) = tmp;
}

//Function to provide with the double right rotation of AVL tree node for balancing purpose
void double_right_rotation_AVL(struct AVL_TREE_node **node)
{
    left_rotation_AVL(&(*node)->right);
    right_rotation_AVL((node));
}

//Function to provide with the double left rotation of AVL tree node for balancing purpose
void double_left_rotation_AVL(struct AVL_TREE_node **node)
{
    right_rotation_AVL(&(*node)->left);
    left_rotation_AVL((node));
}

//Function to Balance AVL tree node
void balance_AVL_TREE(struct AVL_TREE_node **node)
{
    //If node is NULL it returns NULL
    if ((*node) == NULL)
    {
        return;
    }
    //IF height of left subtree is greater than right subtree by more than MAximum Imbalance permitted then it proceeds with left side rotation
    if (find_height_AVL((*node)->left) > find_height_AVL((*node)->right) + MAX_IMBALANCE_AVL_TREE)
    {
        if (find_height_AVL((*node)->left->left) >= find_height_AVL((*node)->left->right))
        {
            left_rotation_AVL(node);
        }
        else
        {
            double_left_rotation_AVL(node);
        }
    }
    //IF height of right subtree is greater than left subtree by more than MAximum Imbalance permitted then it proceeds with right side rotation
    if (find_height_AVL((*node)->right) > find_height_AVL((*node)->left) + MAX_IMBALANCE_AVL_TREE)
    {
        if (find_height_AVL((*node)->right->right) >= find_height_AVL((*node)->right->left))
        {
            right_rotation_AVL(node);
        }
        else
        {
            double_right_rotation_AVL(node);
        }
    }

    //Updating the height of the current node
    (*node)->height = 1 + MAX(find_height_AVL((*node)->left), find_height_AVL((*node)->right));
}

//Function to insert a node into an AVL tree
struct AVL_TREE_node *insert_node_AVL_TREE_pointer(struct AVL_TREE_node **node, char *s)
{
    //If node is NULL it allocates memory and enters the key for the node
    if ((*node) == NULL)
    {
        (*node) = new_AVL_TREE_node(s);
    }
    //If key lies behind the song_name of the node it recurses to left subtree
    else if (strcmp((*node)->song_name, s) > 0)
    {
        (*node)->left = insert_node_AVL_TREE_pointer(&(*node)->left, s);
    }
    //If key lies ahead the song_name of the node it recurses to right subtree
    else
    {
        (*node)->right = insert_node_AVL_TREE_pointer(&(*node)->right, s);
    }
    //Balances the Tree once the node is added
    balance_AVL_TREE(node);
    return (*node);
}

//Function to search for a key inside the AVL tree
struct AVL_TREE_node *search_key_AVL(struct AVL_TREE_node *node, char *s)
{
    //If node is NULL it returns NULL
    if (node == NULL)
    {
        return NULL;
    }

    //If key matches with song name node it returns that node
    if (strcmp(node->song_name, s) == 0)
    {
        return node;
    }

    //If key lies ahead of song_name of the node then it recurses to right subtree
    if (strcmp(node->song_name, s) < 0)
    {
        return search_key_AVL(node->right, s);
    }

    //If key lies behind of song_name of the node then it recurses to left subtree
    if (strcmp(node->song_name, s) > 0)
    {
        return search_key_AVL(node->left, s);
    }
}

//Function to find the next element to fill the gaps when a partiuclar node needs to be deleted
struct AVL_TREE_node *find_next_for_deletion_AVL(struct AVL_TREE_node *node)
{
    if (node == NULL)
    {
        return NULL;
    }

    struct AVL_TREE_node *tmp = node;

    while (tmp->left != NULL)
    {
        tmp = tmp->left;
    }
    return tmp;
}

//Function to delete a node from an AVL tree
struct AVL_TREE_node *delete_node_AVL(struct AVL_TREE_node **node, char *s)
{
    if ((*node) == NULL)
    {
        return NULL;
    }
    //if string s lies before name of song of the node then it goes to left subtree
    if (strcmp((*node)->song_name, s) > 0)
    {
        (*node)->left = delete_node_AVL(&(*node)->left, s);
    }
    //if string s lies ahead of name of song of the node then it goes to right subtree
    else if (strcmp((*node)->song_name, s) < 0)
    {
        (*node)->right = delete_node_AVL(&(*node)->right, s);
    }
    //if atmost one child then it proceeds to delete the node
    else if ((*node)->left == NULL || (*node)->right == NULL)
    {
        struct AVL_TREE_node *tmp;
        if ((*node)->left == NULL)
        {
            tmp = (*node)->right;
        }
        else
        {
            tmp = (*node)->left;
        }

        if (tmp == NULL)
        {
            tmp = (*node);
            (*node) = NULL;
        }
        else
        {
            (*node) = tmp;
        }
        //free(tmp);
        tmp = NULL;
    }
    //if node has both children then it find next element and replaces it with that the proceeds to delete that node
    else
    {
        struct AVL_TREE_node *tmp = find_next_for_deletion_AVL((*node)->right);
        strcpy((*node)->song_name, tmp->song_name);
        delete_node_AVL(&(*node)->right, tmp->song_name);
    }

    //after deleting the node is balanced while recurssively going up the tree
    balance_AVL_TREE(node);

    return (*node);
}

//Calculates the number of songs in the AVL tree
void num_songs_playlist_AVL(struct AVL_TREE_node* node, int* num_song_playlist)
{
    if(node == NULL)
    {
        return;
    }
    num_songs_playlist_AVL(node->left, num_song_playlist);
    (*num_song_playlist)++;
    num_songs_playlist_AVL(node->right, num_song_playlist);

}

// void insert_node_AVL_TREE(struct AVL_TREE_node *node, char *s)
// {
//     if (node == NULL)
//     {
//         //node = new_AVL_TREE_node(s);
//         node = (struct AVL_TREE_node *)malloc(sizeof(struct AVL_TREE_node));
//         node->left = NULL;
//         node->right = NULL;
//         node->height = 0;
//         strcpy(node->song_name, s);
//     }
//     else if (strcmp(node->song_name, s) < 0)
//     {
//         insert_node_AVL_TREE(node->left, s);
//     }
//     else
//     {
//         insert_node_AVL_TREE(node->right, s);
//     }

//     //balance_AVL_TREE(node);
// }

