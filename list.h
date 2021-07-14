#ifndef LIST_H
#define LIST_H

struct List1;

typedef struct List1* List;

struct List1 {
  int value;
  List next;
};

extern List new_list(int x);

extern void add_list(List list, int x);

extern void print_delete_list(List list);

struct List2;
typedef struct List2* List_list;

struct List2{
  List list;
  List_list next;
};

extern void add_list_list(List_list *list_list, List list);

extern void print_delete_list_list(List_list list);

#endif /* LIST_H */