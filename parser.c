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
#define MIN8 '0'
#define MAX8 '7'

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
   zamienia ją na 10tną
   (tf?) zwraca liczbę zapisaną ósemkowo jeśli to liczba ósemkowa */
static inline long double octal(char *word, char **end){
    char *word2;
    word2 = word;
    if(*word == MIN8){
        while(*word != 0 && MIN8 <= *word && *word <= MAX8){
            word++;
        }
        if(*word == 0){ // jest to liczba ósemkowa
            *end = word; //ain't it wrong?
            return octal_string_to_ld(word2);
        }
    }
    *end = word; // może coś innego bo może nie działać dla jakichś edge casów chociaż dla 010 dziala
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

int save_number(long double **n, int *n_i, int n_size, long double number){
    if(*n_i == n_size){
        n_size *= 2;
        *n = realloc(*n, n_size * sizeof(long double));
        if(n == NULL) exit(1);
    }
    *(*n + *n_i) = number;
    (*n_i)++;
    return n_size;
}

int save_nan(char ***nan, int *nan_i, int nan_size, char *word){
    if(*nan_i == nan_size){
        nan_size = nan_size * 2;
        *nan = realloc(*nan, nan_size * sizeof(char*));
        if(nan == NULL) exit(1);
    }

    //policznie word_length
    int word_length = 1; // 0? chyba lepiej z 1 tho (valgrind mniej bledow)
    char *word2;
    word2 = word;
    while(*word2 != 0){
        word2++;
        word_length++;
    }

    char *word_copy = malloc(word_length * sizeof(char));
    if(word_length != 0 && word == NULL) exit(1); // wczesniej zamiast word bylo n, idk why ale moze to cos popsuje jeszcze
    strcpy(word_copy, word);
    *(*nan + *nan_i) = word_copy;
    (*nan_i)++;
    return nan_size;
}

void process_word(char* word, long double **n, int *n_i, char ***nan, int *nan_i, int *nan_size, int *n_size) {
    
    char *end; // wskaznik czy slowo jest liczba (=NULL) czy nieliczbą (=/=NULL)
    long double number = octal(word, &end);
    
    if(*end != 0) // nie osemkowa wiec jeszcze nie zamieniona na long double
            number = strtold(word, &end); //wiec zamieniam na ld
        if((*word == '-' || *word == '+') && *(word + 1) == '0' && *(word + 2) == 'x') *end = 0; //jesli +/- przed 16tokową to co xd?
        if(*word == '0' && *(word + 1) == 'x' && *(word + 2) == 0){
            *end = 0;
            number = 0;
        }
        if(*end == 0)
            *n_size = save_number(n, n_i, *n_size, number);
        else
            *nan_size = save_nan(nan, nan_i, *nan_size, word);
}

int process_line(char* line, long double **n, int *n_i, char ***nan, int *nan_i){
    
    int validity = valid(line);
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
    sort_n(*n, 0, *n_i);
    if(*nan_i != 0) sort_nan(*nan, 0, *nan_i);
    return VALID;
}