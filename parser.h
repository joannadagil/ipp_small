#ifndef PARSER_H
#define PARSER_H

extern int process_line(char* line, long double **n, int *n_i, char ***nan, int *nan_i, ssize_t read);

#endif /* PARSER_H */