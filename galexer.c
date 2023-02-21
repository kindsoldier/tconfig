/*
 *
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 *
 */

#include <string.h>
#include <stdio.h>

#include <galexer.h>

#define LTYPE_LETTER    1
#define LTYPE_PREFIX    2
#define LTYPE_DELIM     3
#define LTYPE_EOF       4

#define LCONTEXT_START  1
#define LCONTEXT_WORD   2
#define LCONTEXT_DELIM  3
#define LCONTEXT_PREFIX 4
#define LCONTEXT_ENDFL  5
#define LCONTEXT_UNDEF  7

static int get_ltype(char newletter) {
    switch (newletter) {
        case '-':
        case '+':
            return LTYPE_PREFIX;
        case '=':
            return LTYPE_DELIM;
        case EOF:
            return LTYPE_EOF;
    }
    return LTYPE_LETTER;
}

void galexer_init(galexer_t* lexer, char* arg) {
        lexer->arg = arg;
        lexer->rpos = 0;
        lexer->tpos = 0;
        lexer->currcontext = LCONTEXT_START;
        lexer->newletter = '\0';
}

static void galexer_getletter(galexer_t* lexer, char* arg, int size) {
        lexer->newletter = EOF;
        if (lexer->rpos < size) {
            lexer->newletter = arg[lexer->rpos++];
        }
}

int galexer_gettok(galexer_t* lexer, char* token) {

    char* arg = lexer->arg;
    size_t size = strlen(arg);

    if (lexer->currcontext == LCONTEXT_START) {
        lexer->newletter = arg[lexer->rpos++];
        int ltype = get_ltype(lexer->newletter);
        int newcontext = LCONTEXT_ENDFL;
        switch (ltype) {
            case LTYPE_PREFIX: {
                    newcontext = LCONTEXT_PREFIX;
                    break;
            }
            case LTYPE_DELIM: {
                    newcontext = LCONTEXT_DELIM;
                    break;
            }
            case LTYPE_LETTER: {
                    newcontext = LCONTEXT_WORD;
                    break;
            }
            case LTYPE_EOF: {
                    newcontext = LCONTEXT_ENDFL;
                    break;
            }
        }
        lexer->currcontext = newcontext;
    }

    while (lexer->currcontext != LCONTEXT_ENDFL) {

        int ltype = get_ltype(lexer->newletter);

        switch (lexer->currcontext) {
            case LCONTEXT_ENDFL: {
                lexer->currcontext = LCONTEXT_ENDFL;
                break;
            }
            case LCONTEXT_DELIM: {
                int newcontext = lexer->currcontext;
                switch (ltype) {
                    case LTYPE_PREFIX:
                    case LTYPE_LETTER: {
                            newcontext = LCONTEXT_WORD;
                            break;
                    }
                    case LTYPE_EOF: {
                            newcontext = LCONTEXT_ENDFL;
                            break;
                    }
                }
                if (newcontext != lexer->currcontext) {
                    lexer->currcontext = newcontext;
                    token[lexer->tpos] = '\0';
                    lexer->tpos = 0;
                    return TOKEN_DELIM;
                }
                break;
            }
            case LCONTEXT_PREFIX: {
                int newcontext = lexer->currcontext;
                switch (ltype) {
                    case LTYPE_DELIM: {
                            newcontext = LCONTEXT_DELIM;
                            break;
                    }
                    case LTYPE_LETTER: {
                            newcontext = LCONTEXT_WORD;
                            break;
                    }
                    case LTYPE_EOF: {
                            newcontext = LCONTEXT_ENDFL;
                            break;
                    }
                }
                if (newcontext != lexer->currcontext) {
                    lexer->currcontext = newcontext;
                    token[lexer->tpos++] = '\0';
                    lexer->tpos = 0;
                    return TOKEN_PREF;
                }
                break;
            }
            case LCONTEXT_WORD: {
                int newcontext = lexer->currcontext;
                switch (ltype) {
                    case LTYPE_DELIM: {
                            newcontext = LCONTEXT_DELIM;
                            break;
                    }
                    case LTYPE_EOF: {
                            newcontext = LCONTEXT_ENDFL;
                            break;
                    }
                }
                if (newcontext != lexer->currcontext) {
                    lexer->currcontext = newcontext;
                    token[lexer->tpos] = '\0';
                    lexer->tpos = 0;
                    return TOKEN_WORD;
                }
                break;
            }
        }
        token[lexer->tpos++] = lexer->newletter;
        galexer_getletter(lexer, arg, size);
    }

    lexer->currcontext = LCONTEXT_ENDFL;
    lexer->tpos = 0;
    token[lexer->tpos] = '\0';
    strcpy(token, "EOF");
    return TOKEN_ENDF;
}
