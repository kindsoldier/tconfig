/*
 *
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 *
 */

#ifndef GCLEXER_H_QWERTY
#define GCLEXER_H_QWERTY

//#include <string.h>
//#include <stdbool.h>
//#include <stdio.h>

#include <galexer.h>

typedef struct {
        char** argv;
        int argc;
        int argn;
        galexer_t alex;
} gclexer_t;

void gclexer_init(gclexer_t* lexer, char** argv, int argc);
int gclexer_gettok(gclexer_t* lexer, char* token);

#endif
