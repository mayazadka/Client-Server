#include "list.h"

list newList()
{
    list list;
    list.head = NULL;
    list.tail = NULL;
    list.size=0;
}
list initialNewList(int** initialList ,int size)
{
    int i;
    list list;
    for(i = 0 ; i < size ; i++){add(&list , initialList[i]);}
}

void add(list* list , int value)
{
    node node;
    node.value = value;
    node.prev = list->tail;
    node.next = NULL;
    list->tail->next = &node;
    list->tail = &node;
    if(list->size == 0){list->head = &node;}
    list->size++;
}
int take(list* list , int* value)
{
    if(list->size == 0) return emptyList;
    *value = list->head->value;
    //delete head
    list->head = list->head->next;
    list->head->prev = NULL;
    return 0;
}
int takeLast(list* list , int* value)
{
    if(list->size == 0)return emptyList;
    *value = list->tail->value;
    //delete head
    list->tail = list->tail->prev;
    list->tail->next = NULL;
}
bool contains(list* list,int value)
{
    node* node = list->head;
    if(node == NULL) return false;
    while((node = node->next) != NULL)
    {
         if (node->value == value) {return true;}
    }
    return false;
}
