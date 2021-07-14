#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void swap(long double *a, long double *b){
    int x = *a;
    *a = *b;
    *b = x;
}

void sort_n(long double n[], int l, int r){
    int i = l, j = r;
    int pivot = n[(l + r) / 2];
    while(i <= j){
        while(pivot > n[i])
            i++;
        while(pivot < n[j])
            j--;
        if(i <= j){
            printf("swapping %Lf i %Lf\n", n[i], n[j]);
            swap(&(n[i]), &(n[j]));
            i++;
            j--;
        }
    }
    if(j > l) sort_n(n, l, j);
    if(i < r) sort_n(n, i, r);
}

int main(){
    printf("we here\n");
    long double a[5];
    a[0] = 1.;
    a[1] = 2.;
    a[2] = 3.;
    a[3] = 0.;
    a[4] = 1.;
    printf("we here\n");
    sort_n(a, 0, 4);
    printf("we here\n");
    for(int i = 0; i < 5; i++)
        printf("%Lf  \n", a[i]);
    return 0;
}