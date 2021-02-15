#include "list.h"

Node* newNode(int value, Node* next)
{
    Node* node = (Node*)malloc(sizeof(Node));
    if(node == NULL) {return NULL;}

    node->next = next;
    node->value = value;

    return node;
}

List* newList()
{
    List* list = (List*)malloc(sizeof(List));
    if(list == NULL) {return NULL;}

    list->head = NULL;
    list->tail = NULL;
    list->size=0;

    return list;
}

List* initialNewList(int size)
{
    List* list = newList();

    int i;
    for(i = 0 ; i < size ; i++)
    {
        addLast(list ,i);
    }

    return list;
}

void addLast(List* list , int value)
{
    if(list->size == 0)
    {
        list->head = newNode(value, NULL);
        list->tail = list->head;
    }
    else
    {
        Node* node = newNode(value, NULL);
        list->tail->next = node;
        list->tail = node;
    }
    
    list->size++;
}

int takeFirst(List* list , int* value)
{
    if(list->size == 0) {return 0;}

    *value = list->head->value;
    
    Node* node =list->head;
    list->head = list->head->next;
    free(node);

    if(list->size == 0)
    {
        list->head = NULL;
        list->tail = NULL;
    }

    return 1;
}

int contains(List* list,int value)
{
    Node* node = list->head;
    while(node!= NULL)
    {
        if(node->value == value) {return 1;}
        node = node->next;
    }
    return 0;
}

void freeList(List* list)
{
    freeNodes(list->head);
    free(list);
}

void freeNodes(Node* node)
{
    if(node->next == NULL)
    {
        free(node);
    }
    else
    {
        freeNodes(node->next);
        free(node);
    }
}