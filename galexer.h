/*
 *
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 *
 */

#ifndef GALEXER_H_QWERTY
#define GALEXER_H_QWERTY

#define TOKEN_NULL      1
#define TOKEN_WORD      2
#define TOKEN_DELIM     3
#define TOKEN_PREF      4
#define TOKEN_ENDF      5
#define TOKEN_SPACE     6


typedef struct {
    char* arg;
    int currcontext;
    int argn;
    int rpos;
    int tpos;
    char newletter;
} galexer_t;

void galexer_init(galexer_t* lexer, char* arg);
int galexer_gettok(galexer_t* lexer, char* token);

#endif
