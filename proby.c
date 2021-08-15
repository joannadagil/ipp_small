#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


int main(){

    printf("mama\n");

    size_t size = 100;
	char *row = NULL;
    //Tree data;
    //data = NULL;
    //List_list all_lines;
    //all_lines = NULL;
	int read;
    int line_no = 1;
    long double *n;
    char **nan;
    int n_size, nan_size;

    while ((read = getline(&row, &size, stdin)) != -1){
        line_no++;
	}

    return 0;
    //    joannadagil
}