#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "tree.h"

static inline int height(Tree data){
    if(data == NULL) return 0;
    return data->height;
}

static inline Tree new_node(long double *n, int n_size, char **nan, int nan_size, int line, List_list *all_lines){
    Tree node;
    node = malloc(sizeof(struct Node));
    if(node == NULL) exit(1);
    node->n = n;
    node->n_size = n_size;
    node->nan = nan;
    node->nan_size = nan_size;
    node->lines = new_list(line);
    add_list_list(all_lines, node->lines);
    node->left = NULL; node->right = NULL;
    node->height = 1;
    return node;
}

static inline int min(int x, int y){
    if(x < y) return x;
    return y;
}

void kurczaki(char **nan, int nan_size){
    char kurczak[] = "kurczak";
    char ipp[] = "ipp";
    char peczar[] = "peczar"; 
    int k = 0; int i = 0; int p = 0; int zmiana = 0;
    char *last = NULL;

    for(int j = 0; j < nan_size; j++) {
        if(strcmp(kurczak, *nan) == 0)
            k++;
        if(strcmp(ipp, *nan) == 0)
            i++;
        if(strcmp(peczar, *nan) == 0)
            p++;
        if(last){
            if(strcmp(last, *nan) != 0){
                zmiana++;
            }
        }
        last = *nan;
        nan++;    
    }
    printf("kurczak: %d  ", k);
    printf("ipp:     %d  ", i);
    printf("peczar:  %d\n", p);
    printf("zmiana:  %d\n", zmiana);
}

static inline int compare(long double *n1, int n1_size, char **nan1, int nan1_size, long double *n2, int n2_size, char **nan2, int nan2_size){
    int iterator = 0;
    while(iterator < min(n1_size, n2_size)){
        if(*n1 < *n2) return -1;
        if(*n1 > *n2) return 1;
        n1 += 1;
        n2 += 1;
        iterator++;
    }
    if(n1_size - iterator > 0) return 1;
    if(n2_size - iterator > 0) return -1;

    iterator = 0;
    while(iterator < min(nan1_size, nan2_size)){
        int compared = strcmp(*nan1, *nan2);
        if(compared < 0) return -1;
        if(compared > 0) return 1;
        nan1++; nan2++; iterator++;
    }

    if(nan1_size - iterator > 0) return 1;
    if(nan2_size - iterator > 0) return -1;

    return 0;
}

// zwraca wierzchołek tożsamy z nowym wierszem albo NULL jeśli go nie ma 
static inline Tree search(long double *n_new, int n_new_size, char **nan_new, int nan_new_size, int line, Tree data){
    if(data == NULL)
        return NULL;
    int compared = compare(n_new, n_new_size, nan_new, nan_new_size, data->n, data->n_size, data->nan, data->nan_size);
    kurczaki(nan_new, nan_new_size);
    if(compared == -1)
        return search(n_new, n_new_size, nan_new, nan_new_size, line, data->left);
    if(compared == 1)
        return search(n_new, n_new_size, nan_new, nan_new_size, line, data->right);
    return data;
}

//-----------AVL insertion-------------------------

// zwraca największą wysokość spośród wysokości prawego i lewego syna
static inline int max_height(Tree data){
    if(height(data->left) > height(data->right))
        return height(data->left);
    return height(data->right);
}

static inline Tree rotate_right(Tree data){
    Tree data_l = data->left;
    Tree data_l_r = data_l->right;
    data_l->right = data;
    data->left = data_l_r;
    data->height = max_height(data) + 1;
    data_l->height = max_height(data_l) + 1;
    return data_l;
}

static inline Tree rotate_left(Tree data){
    Tree data_r = data->right;
    Tree data_r_l = data_r->left;
    data_r->left = data;
    data->right = data_r_l;
    data->height = max_height(data) + 1;
    data_r->height = max_height(data_r) + 1;
    return data_r;
}

//różnica między wysokością lewego i prawego syna
static inline int difference_height(Tree data){
    if(data == NULL) return 0;
    return height(data->left) - height(data->right);
}

static inline Tree balance(long double *n_new, int n_new_size, char **nan_new, int nan_new_size, Tree data){
    int difference = difference_height(data);
    if(difference > 1 && compare(n_new, n_new_size, nan_new, nan_new_size, data->left->n, data->left->n_size, data->left->nan, data->left->nan_size) == -1)
        return rotate_right(data);
    if(difference > 1 && compare(n_new, n_new_size, nan_new, nan_new_size, data->left->n, data->left->n_size, data->left->nan, data->left->nan_size) == 1){
        data->left = rotate_left(data->left);
        return rotate_right(data);
    }
    if(difference < -1 && compare(n_new, n_new_size, nan_new, nan_new_size, data->right->n, data->right->n_size, data->right->nan, data->right->nan_size) == 1)
        return rotate_left(data);
    if(difference < -1 && compare(n_new, n_new_size, nan_new, nan_new_size, data->right->n, data->right->n_size, data->right->nan, data->right->nan_size) == -1){
        data->right = rotate_right(data->right);
        return rotate_left(data);
    }
    return data;
}

// dodaje nowa linie do drzewa
// założenie wstępne: wiersz nie może być tozsamy z żadną komórką
static inline Tree insert(long double *n_new, int n_new_size, char **nan_new, int nan_new_size, int line, Tree data, List_list *all_lines){
    if(data == NULL)
        return new_node(n_new, n_new_size, nan_new, nan_new_size, line, all_lines);
    int compared = compare(n_new, n_new_size, nan_new, nan_new_size, data->n, data->n_size, data->nan, data->nan_size);
    if(compared == -1)
        data->left = insert(n_new, n_new_size, nan_new, nan_new_size, line, data->left, all_lines);
    else
        data->right = insert(n_new, n_new_size, nan_new, nan_new_size, line, data->right, all_lines);
    data->height = 1 + max_height(data);
    return balance(n_new, n_new_size, nan_new, nan_new_size, data);
}

/* --------------------------------------------- */

Tree add_line(long double *n_new, int n_new_size, char **nan_new, int nan_new_size, int line, Tree data, List_list *all_lines){
    Tree searched = search(n_new, n_new_size, nan_new, nan_new_size, line, data);
    if(!searched)
        return insert(n_new, n_new_size, nan_new, nan_new_size, line, data, all_lines);
    
    add_list(searched->lines, line);

    free(n_new);
    for(int i = 0; i < nan_new_size; i++){              
        free(*(nan_new + i));
    }
    free(nan_new);

    return data;
}

void delete_tree(Tree data){
    if(data){
        free(data->n);

        for(int i = 0; i < data->nan_size; i++){
            free(*(data->nan + i));
        }

        free(data->nan);

        delete_tree(data->left);
        delete_tree(data->right);
        free(data);
    }
} 