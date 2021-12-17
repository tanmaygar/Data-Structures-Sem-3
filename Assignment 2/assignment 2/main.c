#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define STRING_SIZE 10
#define INPUT_SIZE 100

int preorder_array_index = 0;

//Function corrects the string after taking input from fgets function to remove \n
void string_corrector(char *s)
{
    int len = strlen(s);
    s[len - 1] = '\0';
}

struct BST_node
{
    char song_name[STRING_SIZE];
    struct BST_node *left;
    struct BST_node *right;
};

struct BST_node *new_BST_node(char *s)
{
    struct BST_node *new_node = (struct BST_node *)malloc(sizeof(struct BST_node));
    new_node->left = NULL;
    new_node->right = NULL;
    strcpy(new_node->song_name, s);

    return new_node;
}

int search_song_in_array(char song_list[STRING_SIZE][STRING_SIZE], int begin, int end, char *song_name)
{
    for (int i = begin; i < end; i++)
    {
        if (strcmp(song_name, song_list[i]) == 0)
        {
            return i;
        }
    }
}

struct BST_node *tree_BST_node(char inorder_songs_array[STRING_SIZE][STRING_SIZE], char preorder_songs_array[STRING_SIZE][STRING_SIZE], int begin, int end)
{

    //static int preorder_array_index = 0;
    if (begin > end || preorder_array_index == end)
    {
        return NULL;
    }

    struct BST_node *node = new_BST_node(preorder_songs_array[preorder_array_index++]);

    if (begin == end)
    {
        return node;
    }

    int inorder_array_index = search_song_in_array(inorder_songs_array, begin, end, node->song_name);

    node->left = tree_BST_node(inorder_songs_array, preorder_songs_array, begin, inorder_array_index - 1);
    node->right = tree_BST_node(inorder_songs_array, preorder_songs_array, inorder_array_index + 1, end);

    return node;
}

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

struct BST_node *search_key_BST(struct BST_node *node, char *key)
{
    if (node == NULL)
    {
        return NULL;
    }

    if (strcmp(node->song_name, key) == 0)
    {
        return node;
    }

    if (strcmp(node->song_name, key) < 0)
    {
        return search_key_BST(node->right, key);
    }

    if (strcmp(node->song_name, key) > 0)
    {
        return search_key_BST(node->left, key);
    }
}

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

struct BST_node *delete_node_BST(struct BST_node *node, char *key)
{
    if (node == NULL)
    {
        return NULL;
    }

    if (strcmp(node->song_name, key) < 0)
    {
        node->right = delete_node_BST(node->right, key);
    }
    else if (strcmp(node->song_name, key) > 0)
    {
        node->left = delete_node_BST(node->left, key);
    }
    else
    {
        struct BST_node *tmp;
        if (node->left == NULL)
        {
            tmp = node->right;
            return tmp;
        }
        else if (node->right == NULL)
        {
            tmp = node->left;
            return tmp;
        }
        else
        {
            struct BST_node *new_tmp = find_next_for_deletion_BST(node->right);
            strcpy(node->song_name, new_tmp->song_name);
            node->right = delete_node_BST(node->right, new_tmp->song_name);
        }
    }
    return node;
}

struct AVL_TREE_node
{
    char song_name[STRING_SIZE];
    struct AVL_TREE_node* left;
    struct AVL_TREE_node* right;
};

int main()
{
    char input_file_string[INPUT_SIZE] = "";
    char input_file_inorder[INPUT_SIZE] = "";
    char input_file_preorder[INPUT_SIZE] = "";

    FILE *file_ptr = fopen("songs.txt", "r");
    if (file_ptr == NULL)
    {
        return EXIT_FAILURE;
    }
    fscanf(file_ptr, "%s", input_file_string);

    int i = 0;
    while (input_file_string[i] != ';')
    {
        i++;
    }
    strncpy(input_file_inorder, input_file_string, i);
    strncpy(input_file_preorder, input_file_string + (i + 1) * sizeof(char), strlen(input_file_string));
    char inorder_songs_array[STRING_SIZE][STRING_SIZE];
    char preorder_songs_array[STRING_SIZE][STRING_SIZE];
    for (int i = 0; i < STRING_SIZE; i++)
    {
        for (int j = 0; j < STRING_SIZE; j++)
        {
            inorder_songs_array[i][j] = '\0';
            preorder_songs_array[i][j] = '\0';
        }
    }
    i = 0;
    int j = 0;
    int num_songs = 0;
    int check = 0;
    while (input_file_inorder[i] != '\0')
    {
        while (input_file_inorder[j] != ':')
        {
            if (input_file_inorder[j] == '\0')
            {
                break;
            }
            j++;
        }

        strncpy(inorder_songs_array[num_songs], input_file_inorder + (check) * sizeof(char), j - check);
        check = j + 1;
        num_songs++;
        j++;
        i++;
        if (input_file_inorder[j] == '\0')
        {
            break;
        }
    }
    i = 0;
    j = 0;
    num_songs = 0;
    check = 0;
    while (input_file_preorder[i] != '\0')
    {
        while (input_file_preorder[j] != ':')
        {
            if (input_file_preorder[j] == '\0')
            {
                break;
            }
            j++;
        }

        strncpy(preorder_songs_array[num_songs], input_file_preorder + (check) * sizeof(char), j - check);
        check = j + 1;
        num_songs++;
        j++;
        i++;
        if (input_file_preorder[j] == '\0')
        {
            break;
        }
    }
    struct BST_node *song_BST = tree_BST_node(inorder_songs_array, preorder_songs_array, 0, num_songs);
    print_order(song_BST);
    if (search_key_BST(song_BST, "S3") != NULL)
    {
        printf("found S3\n");
        delete_node_BST(song_BST, "S3");
        delete_node_BST(song_BST, "S4");
    }
    printf("Enter the songs you need to delete and end with -1 \n");
    char input_char[STRING_SIZE], input_termin[STRING_SIZE] = "-1";
    while (true)
    {
        fgets(input_char, STRING_SIZE, stdin);
        string_corrector(input_char);
        if (strcmp(input_char, input_termin) == 0)
        {
            break;
        }
        delete_node_BST(song_BST, input_char);
    }
    printf("Enter the songs you need to add to playlist and end with -1 \n");
    while(true)
    {
        fgets(input_char, STRING_SIZE, stdin);
        string_corrector(input_char);
        if (strcmp(input_char, input_termin) == 0)
        {
            break;
        }
        if(search_key_BST(song_BST, input_char) == 0)
        {
            printf("Song found and added\n");
            //add song to avl tree
        }
        else
        {
            printf("Song not found\n");
        }
    }

    fclose(file_ptr);
    return EXIT_SUCCESS;
}
