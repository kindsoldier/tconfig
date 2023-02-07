/*
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 */

#ifndef CLEXER_H_QWERTY
#define CLEXER_H_QWERTY

#include <bstream.h>

typedef struct {
    bstream_t* stream;
    int context;
    char letter;
} clexer_t;

#define MAX_TOK_SIZE 1024

#define TOKEN_NULL      0
#define TOKEN_WORD      1
#define TOKEN_SPACE     2
#define TOKEN_COMM      3
#define TOKEN_OPER      4
#define TOKEN_ENDFL     5
#define TOKEN_NEWLN     7

void clexer_init(clexer_t * clexer, bstream_t * stream);
int clexer_get_token(clexer_t * clexer, char* token, int maxsize);
void clexer_destroy(clexer_t* lexer);

#endif
