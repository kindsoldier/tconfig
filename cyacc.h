/*
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 */

#ifndef CYACC_H_QWERTY
#define CYACC_H_QWERTY

#include <clexer.h>
#include <vmapper.h>

typedef struct {
    clexer_t* lexer;
    vmapper_t* vmapper;
    int pos;
    int lnum;
} cyacc_t;

cyacc_t * new_cyacc(clexer_t * lexer, vmapper_t* vmapper);
void cyacc_init(cyacc_t* yacc, clexer_t* lexer, vmapper_t* vmapper);
int cyacc_parse(cyacc_t* yacc);
void cyacc_destroy(cyacc_t* yacc);
void cyacc_free(cyacc_t* yacc);

#endif
