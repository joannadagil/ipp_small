#include<stdlib.h>
#include<stdio.h>
#include"list.h"

List new_list(int x){
    List list = malloc(sizeof(struct List1));
    if(list == NULL) exit(1);
    list->value = x;
    list->next = NULL;
    return list;
}

// dodaje nr linii do listy numerow linii
void add_list(List list, int x){
    while(list->next != NULL){
        list = list->next;
    }
    list->next = new_list(x);
}

void print_delete_list(List list){
    if(list != NULL){
        while(list->next != NULL){
            printf("%d ", list->value);
            List next = list->next;
            free(list);
            list = next;
        }
        printf("%d", list->value);
        free(list);
    }
    printf("\n");
}

// ---------------------------------------

static inline List_list new_list_list(List list){
    List_list list_list = malloc(sizeof(struct List2));
    if(list_list == NULL) exit(1);
    list_list->list = list;
    list_list->next = NULL;
    return list_list;
}

void add_list_list(List_list *list_list, List list){
    if(*list_list == NULL){
        *list_list = new_list_list(list);
        return;
    }    
    List_list list_list2;
    list_list2 = *list_list;
    while(list_list2->next != NULL){
        list_list2 = list_list2->next;
    }
    list_list2->next = new_list_list(list);
}

void print_delete_list_list(List_list list_list){
    while(list_list != NULL){
        print_delete_list(list_list->list);
        List_list next = list_list->next;
        free(list_list);
        list_list = next;
    }
}