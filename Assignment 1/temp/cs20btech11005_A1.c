#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct song
{
    char name[20];
    int key;
    struct song *next;
}song_t;

song_t *head =NULL;
song_t *current = NULL;
song_t *previous = NULL;
song_t *first = NULL;

void insertFirst(int key, char name[])
{
    song_t *link = (song_t*) malloc(sizeof(song_t));
    link->key=key;
    strncpy(link->name,name,sizeof(link->name));

    link->next= head;

    head=link;
    first=head;
}

song_t* deleteFirst()
{
    song_t *tempLink=head;
    head=head->next;

    return tempLink;
}

bool isEmpty()
{
    return head==NULL;
}

int length()
{
    int length=0;
    song_t *current;

    for(current=head; current!=NULL;current=current->next)
    {
        length++;
    }
    return length;
}

song_t* find(char name[])
{
    song_t* current= head;

    if(head==NULL)
    {
        return NULL;
    }

    while(strcmp(current->name,name))
    {
        if(current->next==NULL)
        {
            return NULL;
        }
        else
        {
            current=current->next;
        }
    }
    return current;
}

song_t* delete_key(int key) {

   song_t* current = head;
   song_t* previous = NULL;
	
   if(head == NULL) {
      return NULL;
   }

    while(current->key != key) 
    {
      if(current->next == NULL) 
        {
            return NULL;
        } 
        else 
        {
            previous = current;
            current = current->next;
        }
    }

   if(current == head) 
    {
        head = head->next;
    }
    else
    {
        previous->next = current->next;
    }    
	
   return current;
}


typedef struct stack_tt
{
    int top_number;
    song_t stack[20];
    int MAXSIZE;

}stack_t;

void stack_initialise(stack_t *all_stack)
{
    all_stack->MAXSIZE=20;
    all_stack->top_number=-1;
}
bool isemptystack(stack_t all_stack)
{
    if(all_stack.top_number ==-1)
    {
        return true;
    }
    else
    return false;
}
bool isfull(stack_t all_stack) {
    if(all_stack.top_number == all_stack.MAXSIZE)
        return true;
    else
        return false;
}

song_t top(stack_t all_stack)
{
    return all_stack.stack[all_stack.top_number];
}

song_t pop(stack_t *all_stack)
{
    song_t temp;
    if(!isemptystack(*all_stack))
    {
        temp = all_stack->stack[all_stack->top_number];
        all_stack->top_number--;
        return temp;
    }
    else
    {
        printf("Stack is empty.\n");
        return temp;
    }

}

void push(song_t data,stack_t *all_stack)
{
    if(!isfull(*all_stack))
    {
        all_stack->top_number++;
        all_stack->stack[all_stack->top_number]=data;

    }
    else
    {
        printf("Stack is full.\n");
    }
}

void printstack(stack_t all_stack)
{
    for(int i=0;i<=all_stack.top_number;i++)
    {
        printf("%s  ",all_stack.stack[i].name);
    }
    printf("\n");
}

void play_stack(stack_t *play_stack,stack_t *recent_stack,stack_t *temp_stack)
{
    char command;
    int times = 0;
    char next='n';
    char prev='p';
    char end='e';
    while(play_stack->top_number>-1)
    {
        printf("play next song / previous songs / end?n/p:k/e \n");
        scanf(" %c",&command);
        if(command==next)
        {
            printf("playing %s \n",top(*play_stack).name);
            push(top(*play_stack),recent_stack);
            pop(play_stack);
        }
        else if (command==prev)
        {
            scanf("%c",&command);
            if(command==':')
            {
                scanf("%d",&times);
            }
            else if(command=='\n')
            {
                times=1;
            }
            else
            {
                printf("invalid command\n");
            }

            if(times>(recent_stack->top_number + 1))
            {
                printf("there arent %d previous songs \n",times);
                times=recent_stack->top_number+1;
            }
            for(int j=0;j<times;j++)
            {
                push(top(*recent_stack),temp_stack);
                pop(recent_stack);
            }
            while(temp_stack->top_number>-1)
            {
                printf("playing %s \n",top(*temp_stack).name);
                push(top(*temp_stack),recent_stack);
                pop(temp_stack);
            }
        }
        else if (command==end)
        {
            return ;
        }
    }
    printf("Bye \n");
    
}


int main()
{
    char tempname[20];
    int num_songs=0;
    char check[]="-1";
    stack_t playlist;
    stack_initialise(&playlist);
    stack_t recent_songs;
    stack_initialise(&recent_songs);
    stack_t temp_playlist;
    stack_initialise(&temp_playlist);

    printf("enter the names of songs(as strings), end with -1\n");
    scanf("%s",tempname);
    while (*tempname!=*check)
    {
        insertFirst(++num_songs,tempname);
        scanf("%s",tempname);
    }
    printf("All songs entered\n");

    printf("Create playlist , end with -1\n");
    scanf("%s",tempname);
    while (strcmp(check,tempname))
    {
        if(find(tempname)!=NULL)
        {
            push(*find(tempname),&playlist);
            printf("song found and enqueued\n");        
        }
        else
        {
            printf("song not found\n");
        }
        scanf("%s",tempname);
    }
    printf("playlist ready\n");

    play_stack(&playlist,&recent_songs,&temp_playlist);
    


    return 0;
}