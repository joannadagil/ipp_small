#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* niedozwolone znaki x to te:
   x < unvalid1 || (unvalid2 < x && x < unvalid3) || unvalid4 < x
*/
#define INVALID1 9
#define INVALID2 13
#define INVALID3 32
#define INVALID4 126
// cyfry 0-7 w ascii są pomiędzy
#define MIN8 48
#define MAX8 55

#define DELIMITERS " \t\n\v\f\r\n"
#define STARTINGSIZE 100
#define ZERO 0
#define NOTZERO 1

// zmienne zwracane przez funkcje valid
#define VALID 1
#define IGNORED 0
#define INVALID -1

// zmienne zwracane przez funkcję number_type
#define NANUMBER 0
#define INT10 1
#define INT16 2
#define INT8 3
#define FLOAT 4

// zmienia litery stringa na lowercase
static inline void tolower_line(char *line){
    while(*line != 0){
        *line = tolower(*line);
        line++;
    }
}

static inline int valid(char *line){
    if(*line == '\n' || *line == '#') return IGNORED;
    while(*line != 0 && *line != '\n'){ //line -> *line
        if(*line < INVALID1 || (INVALID2 < *line && *line < INVALID3) || *line > INVALID4)
            return INVALID;
        line += 1;
    }
    return VALID;
}

static inline long double octal_string_to_ld(char *word){
    long double result = 0;
    while(*word != 0){
        result = result * 8 + (*word - MIN8);
        word++;
    }
    return result;
}

/* *end = 0 jeśli word to liczba ósemkowa
   zwraca liczbę zapisaną ósemkowo jeśli to liczba ósemkowa */
static inline long double octal(char *word, char **end){
    char *word2;
    word2 = word;
    if(*word == MIN8){
        while(*word != 0 && MIN8 <= *word && *word <= MAX8){
            word++;
        }
        if(*word == 0){
            *end = word;
            return octal_string_to_ld(word2);
        }
    }
    *end = word;
    return 0.;
}

// ---------------------SORTING---------------------------

static inline void swap_n(long double a[], int i, int j){
    long double x = a[i];
    a[i] = a[j];
    a[j] = x;
}

static inline void sort_n(long double n[], int l, int r){
    int i = l, j = r - 1;
    int pivot = (l + r) / 2;
    while(i <= j){
        while(n[pivot] > n[i])
            i++;
        while(n[pivot] < n[j])
            j--;
        if(i <= j){
            swap_n(n, i, j);
            i++;
            j--;
        }
    }
    if(j > l) sort_n(n, l, j);
    if(i < r) sort_n(n, i, r);
}

static inline void swap_nan(char *nan[], int i, int j){
    char *x;
    x = nan[i];
    nan[i] = nan[j];
    nan[j] = x;
}

static inline void sort_nan(char **n, int l, int r){
    int i = l, j = r - 1;
    int pivot = (l + r) / 2;
    while(i <= j){
        while(*n[pivot] > *n[i])
            i++;
        while(*n[pivot] < *n[j])
            j--;
        if(i <= j){
            swap_nan(n, i, j);
            i++;
            j--;
        }
    }
    if(j > l) sort_nan(n, l, j);
    if(i < r) sort_nan(n, i, r);
}

// ----------------------------------------------------------------------

int process_line(char* line, long double **n, int *n_i, char ***nan, int *nan_i){
    int validity = valid(line);
    if(validity == IGNORED)
        return IGNORED;
    if(validity == INVALID)
        return INVALID;
    tolower_line(line); 
    *n = malloc(STARTINGSIZE * sizeof(long double));
    if(n == NULL) exit(1);
    *nan = malloc(STARTINGSIZE * sizeof(char*));
    if(nan == NULL) exit(1);
    int n_size = STARTINGSIZE, nan_size = STARTINGSIZE;
    *n_i = 0;
    *nan_i = 0;
    char *word;
    word = strtok(line, DELIMITERS);
    char end1 = 1;
    char *end;
    end = &end1;
    long double number;
    while(word != NULL){
        number = octal(word, &end);
        if(*end != 0)
            number = strtold(word, &end);
        if((*word == '-' || *word == '+') && *(word + 1) == '0' && *(word + 2) == 'x') *end = 0;
        if(*word == '0' && *(word + 1) == 'x' && *(word + 2) == 0){
            *end = 0;
            number = 0;
        }
        if(*end == 0){
            if(*n_i == n_size){
                n_size = n_size * 2;
                *n = realloc(*n, n_size * sizeof(long double));
                if(n == NULL) exit(1);
            }
            *(*n + *n_i) = number;
            (*n_i)++;
        }else{
            if(*nan_i == nan_size){
                nan_size = nan_size * 2;
                *nan = realloc(*nan, nan_size * sizeof(char*));
                if(nan == NULL) exit(1);
            }
            int word_length = 0;
            char *word2;
            word2 = word;
            while(*word2 !=0){
                word2++;
                word_length++;
            }
            char *word_copy = malloc(word_length * sizeof(char));
            if(word_length != 0 && n == NULL) exit(1);
            strcpy(word_copy, word);
            *(*nan + *nan_i) = word_copy;
            (*nan_i)++;
        }
        word = strtok(NULL, DELIMITERS);
    }
    sort_n(*n, 0, *n_i);
    if(*nan_i != 0) sort_nan(*nan, 0, *nan_i);
    return VALID;
}