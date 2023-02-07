/*
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 */

#ifndef CYACC_H_QWERTY
#define CYACC_H_QWERTY

#include <clexer.h>
#include <mapper.h>

typedef struct {
    clexer_t* lexer;
    mapper_t* mapper;
    int pos;
    int lnum;
} cyacc_t;

cyacc_t * new_cyacc(clexer_t * lexer, mapper_t* mapper);
void cyacc_init(cyacc_t* yacc, clexer_t* lexer, mapper_t* mapper);
int cyacc_parse(cyacc_t* yacc);
void cyacc_destroy(cyacc_t* yacc);
void cyacc_free(cyacc_t* yacc);

#endif
