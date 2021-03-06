#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/* niedozwolone znaki x to te:
   x < unvalid1 || (unvalid2 < x && x < unvalid3) || unvalid4 < x
*/
#define INVALID1 9
#define INVALID2 13
#define INVALID3 32
#define INVALID4 126
// cyfry 0-7 w ascii są pomiędzy
#define MIN8 '0'
#define MAX8 '7'

#define DELIMITERS " \t\n\v\f\r\n"
#define STARTINGSIZE 100

// zmienne zwracane przez funkcje valid
#define VALID 1
#define IGNORED 0
#define INVALID -1

// zmienia litery stringa na lowercase
static inline void tolower_line(char *line){
    while(*line != 0){
        *line = tolower(*line);
        line++;
    }
}

static inline int valid(char *line, int read){
    char *starting;
    starting = line;
    if(*line == '\n' || *line == '#') return IGNORED;
    while(*line != 0 && *line != '\n'){
        if(*line < INVALID1 || (INVALID2 < *line && *line < INVALID3) || *line > INVALID4)
            return INVALID;
        line += 1;
    }
    if(*line == 0 && line - starting < read)
        return INVALID;
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
   zamienia ją na 10tną */
static inline long double octal(char *word, char **end){
    char *word2;
    word2 = word;
    if(*word == MIN8){
        while(*word != 0 && MIN8 <= *word && *word <= MAX8){
            word++;
        }
        if(*word == 0){ // jest to liczba ósemkowa
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

static inline int partition_n(long double n[], int l, int r){
    int pivot = r;
    int i = l - 1;
    for(int j = l; j <= r - 1; j++){
        if(n[j] < n[pivot]){
            i++;
            swap_n(n, i, j);
        }
    }
    swap_n(n, i + 1, r);
    return 1 + i;
}

static inline void sort_n(long double n[], int l, int r){
    if(l < r){
        int new = partition_n(n, l, r);
        sort_n(n, l, new - 1);
        sort_n(n, new + 1, r);
    }
}

static inline void swap_nan(char *nan[], int i, int j){
    char *x;
    x = nan[i];
    nan[i] = nan[j];
    nan[j] = x;
}

static inline int partition_nan(char **n, int l, int r){
    int pivot = r;
    int i = l - 1;
    for(int j = l; j <= r - 1; j++){
        if(strcmp(n[j], n[pivot]) < 0){
            i++;
            swap_nan(n, i, j);
        }
    }
    swap_nan(n, i + 1, r);
    return 1 + i;

}

static inline void sort_nan(char **n, int l, int r){
    if(l < r){
        int new = partition_nan(n, l, r);
        sort_nan(n, l, new - 1);
        sort_nan(n, new + 1, r);
    }
}


// ----------------------------------------------------------------------

static inline int save_number(long double **n, int *n_i, int n_size, long double number){
    if(*n_i == n_size){
        n_size *= 2;
        *n = realloc(*n, n_size * sizeof(long double));
        if(n == NULL) exit(1);
    }
    *(*n + *n_i) = number;
    (*n_i)++;
    return n_size;
}

static inline int save_nan(char ***nan, int *nan_i, int nan_size, char *word){
    if(*nan_i == nan_size){
        nan_size = nan_size * 2;
        *nan = realloc(*nan, nan_size * sizeof(char*));
        if(nan == NULL) exit(1);
    }
    int word_length = 1; 
    char *word2;
    word2 = word;
    while(*word2 != 0){
        word2++;
        word_length++;
    }
    char *word_copy = malloc(word_length * sizeof(char));
    if(word_length != 0 && word == NULL) exit(1);
    strcpy(word_copy, word);
    *(*nan + *nan_i) = word_copy;
    (*nan_i)++;
    return nan_size;
}

static inline void process_word(char* word, long double **n, int *n_i, char ***nan, int *nan_i, int *nan_size, int *n_size) {
    char *end; // wskaznik czy slowo jest liczba (=NULL) czy nieliczbą (=/=NULL)
    long double number = octal(word, &end);
    if(*end != 0) // nie osemkowa wiec jeszcze nie zamieniona na long double
        number = strtold(word, &end); //wiec zamieniam na ld
    if((*word == '-' || *word == '+') && *(word + 1) == '0' && *(word + 2) == 'x') end = word;
    if(*word == '0' && *(word + 1) == 'x' && *(word + 2) == 0){
        *end = 0;
        number = 0;
    }
    if(*word == '0' && *(word + 1) == 'x'){
        int i = 2;
        while(*(word + i) != 0 && *end == 0){
            if(*(word + i) == '.' || *(word + i) == 'p')
                end = word;
            i++;
        }
    }
    if(*end == 0 && isnan(number)) end = word;
    if(*end == 0) *n_size = save_number(n, n_i, *n_size, number);
    else *nan_size = save_nan(nan, nan_i, *nan_size, word);
}


int process_line(char* line, long double **n, int *n_i, char ***nan, int *nan_i, int read){
    int validity = valid(line, read);
    if(validity != VALID) //(validity == IGNORED || validity == INVALID)
        return validity;
    tolower_line(line); 
    *n = malloc(STARTINGSIZE * sizeof(long double));
    if(n == NULL) exit(1);
    *nan = malloc(STARTINGSIZE * sizeof(char*));
    if(*nan == NULL) exit(1);
    int *n_size; int *nan_size;
    n_size = malloc(sizeof(int)); nan_size = malloc(sizeof(int));
    if(n_size == NULL || nan_size == NULL) exit(1);
    *n_size = STARTINGSIZE, *nan_size = STARTINGSIZE;
    *n_i = 0; *nan_i = 0; // iteratory po tablicach z liczbami i nieliczbami
    char *word;
    word = strtok(line, DELIMITERS);
    while(word != NULL){
        process_word(word, n, n_i, nan, nan_i, nan_size, n_size);
        word = strtok(NULL, DELIMITERS);
    }
    free(n_size); free(nan_size); 
    sort_n(*n, 0, *n_i - 1);
    if(*nan_i != 0) sort_nan(*nan, 0, *nan_i - 1);
    return VALID;
}