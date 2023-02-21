/*
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 */

#ifndef TCLEXER_H_QWERTY
#define TCLEXER_H_QWERTY

#include <bstream.h>

typedef struct {
    bstream_t* stream;
    int context;
    char letter;
    int pos;
} tclexer_t;

#define MAX_TOK_SIZE 1024

#define TOKEN_NULL      0
#define TOKEN_WORD      1
#define TOKEN_SPACE     2
#define TOKEN_COMM      3
#define TOKEN_OPER      4
#define TOKEN_ENDFL     5
#define TOKEN_NEWLN     7

tclexer_t* new_tclexer(bstream_t * stream);
void tclexer_init(tclexer_t * tclexer, bstream_t * stream);
int tclexer_get_token(tclexer_t * tclexer, char* token, int maxsize);
void tclexer_destroy(tclexer_t* lexer);
void tclexer_free(tclexer_t* lexer);
#endif
