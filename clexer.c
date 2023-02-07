/*
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 */

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <clexer.h>

#define RES_OK   0
#define RES_ERR -1

#define LEXCONT_UNDEF   0
#define LEXCONT_WORD    1
#define LEXCONT_SPACE   2
#define LEXCONT_COMM    3
#define LEXCONT_OPER    4
#define LEXCONT_ENDFL   5
#define LEXCONT_NEWLN   7

#define LTYPE_SPACE     1
#define LTYPE_LETTER    2
#define LTYPE_OPER      3
#define LTYPE_COMMB     4
#define LTYPE_NEWLN     5
#define LTYPE_ENDFL     7


static int get_ltype(char letter) {
    switch (letter) {
        case '\n':
            return LTYPE_NEWLN;
        case ' ':
            return LTYPE_SPACE;
        case '#':
        case ';':
            return LTYPE_COMMB;
        case '=':
            return LTYPE_OPER;
        case EOF:
            return LTYPE_ENDFL;
    }
    return LTYPE_LETTER;
}

clexer_t* new_clexer(bstream_t * stream) {
    clexer_t* lexer = malloc(sizeof(clexer_t));
    if (lexer == NULL) return NULL;
    lexer->stream = stream;
    lexer->context = LEXCONT_UNDEF;
    //lexer->letter = bstream_getc(lexer->stream);

    return lexer;
}


void clexer_init(clexer_t * lexer, bstream_t * stream) {
    lexer->stream = stream;
    lexer->context = LEXCONT_UNDEF;
    lexer->pos = 0;
}

int clexer_get_token(clexer_t * lexer, char* token, int maxsize) {
    lexer->pos = 0;

    if (lexer->context == LEXCONT_UNDEF) {
        lexer->letter = bstream_getc(lexer->stream);
    }

    while (true) {
        int ltype = get_ltype(lexer->letter);

        switch (lexer->context) {
            case LEXCONT_ENDFL:{
                return TOKEN_ENDFL;
            }
            case LEXCONT_WORD:{
                int newcontext = LEXCONT_WORD;

                switch (ltype) {
                    case LTYPE_SPACE:{
                        newcontext = LEXCONT_SPACE;
                        break;
                    }
                    case LTYPE_NEWLN:{
                        newcontext = LEXCONT_NEWLN;
                        break;
                    }
                    case LTYPE_COMMB:{
                        newcontext = LEXCONT_COMM;
                        break;
                    }
                    case LTYPE_OPER:{
                        newcontext = LEXCONT_OPER;
                        break;
                    }
                    case LTYPE_ENDFL:{
                        newcontext = LEXCONT_ENDFL;
                        break;
                    }
                }
                if (newcontext != lexer->context) {
                    lexer->context = newcontext;
                    token[lexer->pos++] = '\0';
                    return TOKEN_WORD;
                }
                token[lexer->pos++] = lexer->letter;
                break;
            }
            case LEXCONT_COMM:{
                int newcontext = LEXCONT_COMM;

                switch (ltype) {
                    case LTYPE_NEWLN:{
                        newcontext = LEXCONT_NEWLN;
                        break;
                    }
                    case LTYPE_ENDFL:{
                        newcontext = LEXCONT_ENDFL;
                        break;
                    }
                }
                if (newcontext != lexer->context) {
                    token[lexer->pos++] = '\0';
                    lexer->context = newcontext;
                    return TOKEN_COMM;
                }
                token[lexer->pos++] = lexer->letter;
                break;
            }
            case LEXCONT_SPACE:{
                int newcontext = LEXCONT_SPACE;

                switch (ltype) {
                    case LTYPE_OPER:{
                        newcontext = LEXCONT_OPER;
                        break;
                    }
                    case LTYPE_COMMB:{
                        newcontext = LEXCONT_COMM;
                        break;
                    }
                    case LTYPE_LETTER:{
                        newcontext = LEXCONT_WORD;
                        break;
                    }
                    case LTYPE_NEWLN:{
                        newcontext = LEXCONT_NEWLN;
                        break;
                    }
                    case LTYPE_ENDFL:{
                        newcontext = LEXCONT_ENDFL;
                        break;
                    }
                }
                if (newcontext != lexer->context) {
                    lexer->context = newcontext;
                    strcpy(token, "SPACE");
                    return TOKEN_SPACE;
                }
                token[lexer->pos++] = lexer->letter;
                break;
            }
            case LEXCONT_OPER:{
                int newcontext = LEXCONT_OPER;

                switch (ltype) {
                    case LTYPE_SPACE:{
                        newcontext = LEXCONT_SPACE;
                        break;
                    }
                    case LTYPE_NEWLN:{
                        newcontext = LEXCONT_NEWLN;
                        break;
                    }
                    case LTYPE_COMMB:{
                        newcontext = LEXCONT_COMM;
                        break;
                    }
                    case LTYPE_LETTER:{
                        newcontext = LEXCONT_WORD;
                        break;
                    }
                    case LTYPE_ENDFL:{
                        newcontext = LEXCONT_ENDFL;
                        break;
                    }
                }
                if (newcontext != lexer->context) {
                    lexer->context = newcontext;
                    strcpy(token, "=");
                    return TOKEN_OPER;
                }
                token[lexer->pos++] = lexer->letter;
                break;
            }
            case LEXCONT_NEWLN:{
                int newcontext = LEXCONT_NEWLN;

                switch (ltype) {
                    case LTYPE_SPACE:{
                        newcontext = LEXCONT_SPACE;
                        break;
                    }
                    case LTYPE_COMMB:{
                        newcontext = LEXCONT_COMM;
                        break;
                    }
                    case LTYPE_LETTER:{
                        newcontext = LEXCONT_WORD;
                        break;
                    }
                    case LTYPE_ENDFL:{
                        newcontext = LEXCONT_ENDFL;
                        break;
                    }
                }
                if (newcontext != lexer->context) {
                    lexer->context = newcontext;
                    strcpy(token, "NL");
                    return TOKEN_NEWLN;
                }
                token[lexer->pos++] = lexer->letter;
                break;
            }

            case LEXCONT_UNDEF:
            default:{
                int newcontext = LEXCONT_UNDEF;
                switch (ltype) {
                    case LTYPE_SPACE:{
                        newcontext = LEXCONT_SPACE;
                        break;
                    }
                    case LTYPE_NEWLN:{
                        newcontext = LEXCONT_NEWLN;
                        break;
                    }
                    case LTYPE_COMMB:{
                        newcontext = LEXCONT_COMM;
                        break;
                    }
                    case LTYPE_LETTER:{
                        newcontext = LEXCONT_WORD;
                        break;
                    }
                    case LTYPE_OPER:{
                        newcontext = LEXCONT_OPER;
                        break;
                    }
                    case LTYPE_ENDFL:{
                        newcontext = LEXCONT_ENDFL;
                        break;
                    }
                }
                lexer->context = newcontext;
                token[lexer->pos++] = lexer->letter;
                break;
            }
        }
        lexer->letter = bstream_getc(lexer->stream);
    }
    return TOKEN_ENDFL;
}

void clexer_destroy(clexer_t* lexer) {
}

void clexer_free(clexer_t* lexer) {
    free(lexer);
}
