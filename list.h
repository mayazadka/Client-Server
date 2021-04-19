//includes
#include <stdio.h>
#include <stdlib.h>

//structures
typedef struct Node
{
    int value;
    struct Node *next;
}Node;
typedef struct List
{
    int size;
    Node* head;
    Node* tail; 
}List;

//functions
Node* newNode(int value, Node* next);
List* newList();
List* initialNewList(int size);
void addLast(List* list , int value);
int takeFirst(List* list , int* value);
int contains(List* list,int value);
void freeList(List* list);
void freeNodes(Node* node);