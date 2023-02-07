/*
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 */

#ifndef CYACC_H_QWERTY
#define CYACC_H_QWERTY

#include <clexer.h>

typedef struct {
    clexer_t* lexer;
    int pos;
    int lnum;
} cyacc_t;

void cyacc_init(cyacc_t* yacc, clexer_t* lexer);
int cyacc_parse(cyacc_t* yacc);
void cyacc_destroy(cyacc_t* yacc);

#endif
