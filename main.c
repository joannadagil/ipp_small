#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "list.h"
#include "tree.h"
#include "parser.h"

#define STARTVALUE 100

int main(){

	size_t size = STARTVALUE;
	char *row = NULL;
    int read;
    int line_no = 1;

    Tree data = NULL;
    //data = NULL;
    List_list all_lines;
    all_lines = NULL;
    long double *n;
    char **nan;
    int *n_size; int *nan_size; //gwaizki
    n_size = malloc(sizeof(int)); nan_size = malloc(sizeof(int));

	while ((read = getline(&row, &size, stdin)) != -1){
        if (errno == ENOMEM) exit(1);
        int valid = process_line(row, &n, n_size, &nan, nan_size); //& z sizów
        if(valid > 0)
            data = add_line(n, *n_size, nan, *nan_size, line_no, data, &all_lines);
        if(valid < 0)
            fprintf(stderr, "ERROR %d\n", line_no);
        line_no++;
	}
    free(row);
    delete_tree(data);
    print_delete_list_list(all_lines);
    free(n_size); free(nan_size);

    return 0;
}