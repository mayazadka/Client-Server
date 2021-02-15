#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct list
{
    int size;
    node* head;
    node* tail; 
}list;

typedef struct node
{
    int value;
    node* next;
    node* prev;
}node;

//
#define emptyList 1
//
list newList();
list initialNewList(int** initialList ,int size);
void add(list* list , int value);
int take(list* list , int* value);
int takeLast(list* list , int* value);
bool contains(list* list,int value);