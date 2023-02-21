/*
 *
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 *
 */

#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include <galexer.h>
#include <gclexer.h>

void gclexer_init(gclexer_t* lexer, char** argv, int argc) {
        lexer->argv = argv;
        lexer->argc = argc;
        lexer->argn = 1;
        if (lexer->argc > lexer->argn) {
            galexer_init(&(lexer->alex), lexer->argv[lexer->argn]);
        }
}

int gclexer_gettok(gclexer_t* lexer, char* token) {

    if (lexer->argn >= lexer->argc) {
        strcpy(token, "EOF");
        return TOKEN_ENDF;
    }

    int toktype = galexer_gettok(&(lexer->alex), token);
    if (toktype == TOKEN_ENDF && lexer->argn != lexer->argc) {
        lexer->argn++;
        galexer_init(&(lexer->alex), lexer->argv[lexer->argn]);
        strcpy(token, "space");
        return TOKEN_SPACE;
    }
    return toktype;
}
