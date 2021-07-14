#ifndef TREE_H
#define TREE_H

#include "list.h" //???

struct Node;

typedef struct Node* Tree;

struct Node {
    long double *n; // wskaznik na tablicę liczb wiersza
    int n_size; // ilość liczb w wierszu
    char **nan; // wskaźnik na tablicę wskaźników do nieliczb wiersza
    List lines; // lista wierszy, które są tej postaci
    Tree left, right;
    int height;
};

extern Tree add_line(long double *n_new, int n_new_size, char **nan_new, int line, Tree data, List_list *all_lines);

extern void delete_tree(Tree data);


#endif /* TREE_H */