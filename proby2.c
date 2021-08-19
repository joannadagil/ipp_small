#include <stdio.h>

void IncSumPolys(int *index_poly) {
    (*index_poly)++;
}

int main(){
    int index_poly = 1;
    IncSumPolys(&index_poly);
    printf("%d\n", index_poly);
    return 0;
}