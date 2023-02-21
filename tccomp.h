/*
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 */

#ifndef TCCOMP_H_QWERTY
#define TCCOMP_H_QWERTY

#include <tclexer.h>
#include <vmapper.h>

typedef struct {
    tclexer_t* lexer;
    vmapper_t* vmapper;
    int pos;
    int lnum;
} tccomp_t;

tccomp_t * new_tccomp(tclexer_t * lexer, vmapper_t* vmapper);
void tccomp_init(tccomp_t* yacc, tclexer_t* lexer, vmapper_t* vmapper);
int tccomp_parse(tccomp_t* yacc);
void tccomp_destroy(tccomp_t* yacc);
void tccomp_free(tccomp_t* yacc);

#endif
