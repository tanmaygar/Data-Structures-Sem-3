#ifndef MINHEAPS_H_INCLUDED
#define MINHEAPS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#define NUM_PEOPLE 100

enum condition_state
{
    SUS,
    INF,
    REC
};

struct node_person
{
    int id;
    enum condition_state state;
};

struct time_stamp_event
{
    int timestamp;
    struct node_person *person;
};

void swap_person(struct node_person *a, struct node_person *b)
{
    struct node_person *tmp = b;
    b = a;
    a = tmp;
}

void swap_timestamp(struct time_stamp_event *a, struct node_timestamp* b)
{
    struct time_stamp_event *tmp = b;
    b = a;
    a = tmp;
}

void min_heapify(struct time_stamp_event arr[NUM_PEOPLE], int size, int index)
{
    if (size == 1)
    {
        return;
    }
    int largest = index;
    int l = 2 * index + 1;
    int r = 2 * index + 2;
    if (l < size && arr[l].timestamp > arr[largest].timestamp)
        largest = l;
    if (r < size && arr[r].timestamp > arr[largest].timestamp)
        largest = r;
    if (largest != index)
    {
        swap_timestamp(&arr[index], &arr[largest]);
        min_heapify(arr, size, largest);
    }
}

#endif