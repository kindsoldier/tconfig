/*
 *
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 *
 */


#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define RES_OK   0
#define RES_ERR -1

typedef struct {
    bstream_t* stream;
    int context;
    char letter;
} lexer_t;

#define MAX_TOK_SIZE 1024

#define TOKEN_NULL      0
#define TOKEN_WORD      1
#define TOKEN_SPACE     2
#define TOKEN_COMM      3
#define TOKEN_OPER      4
#define TOKEN_ENDFL     5
#define TOKEN_NEWLN     7

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

int get_ltype(char letter) {
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

void lexer_init(lexer_t * lexer, bstream_t * stream) {
    lexer->stream = stream;
    lexer->context = LEXCONT_UNDEF;
    lexer->letter = bstream_getc(lexer->stream);
}


int lexer_get_token(lexer_t * lexer, char* token) {
    size_t tpos = 0;

    while (true) {

        int ltype = get_ltype(lexer->letter);

        switch (lexer->context) {
        case LEXCONT_ENDFL:{
            return TOKEN_ENDFL;
            token[tpos] = '\0';
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
                token[tpos++] = '\0';
                lexer->context = newcontext;
                lexer->letter = bstream_getc(lexer->stream);
                return TOKEN_COMM;
            }
            token[tpos++] = lexer->letter;
            break;
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
                token[tpos++] = '\0';
                return TOKEN_WORD;
            }
            token[tpos++] = lexer->letter;
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
                strcpy(token, " ");
                return TOKEN_SPACE;
            }
            token[tpos++] = lexer->letter;
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
            break;
        }

        case LEXCONT_UNDEF:
        default:{
            int newcontext = LEXCONT_UNDEF;

            token[tpos++] = lexer->letter;
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
            break;
        }
        }
        lexer->letter = bstream_getc(lexer->stream);
    }
    return TOKEN_ENDFL;
}

static char* strcopy(char* src) {
    size_t srcsize = strlen(src) + 1;
    char* dst = malloc(srcsize);
    memset(dst, '\0', srcsize);
    strcpy(dst, src);
    return dst;
}

typedef struct {
    lexer_t* lexer;
    int pos;
    int lnum;
} yacc_t;


void yacc_init(yacc_t * yacc, lexer_t * lexer) {
    yacc->lexer = lexer;
    yacc->pos = 0;
    yacc->lnum = 0;
}


#define POS1TYPE TOKEN_WORD
#define POS2TYPE TOKEN_OPER
#define POS3TYPE TOKEN_WORD
#define POS4TYPE TOKEN_COMM


int yacc_parse(yacc_t * yacc) {
    char token[MAX_TOK_SIZE];
    int toktype = -1;
    lexer_t* lexer =  yacc->lexer;
    char* key = NULL;
    char* var = NULL;

    while ((toktype = lexer_get_token(lexer, token)) != TOKEN_ENDFL) {
        if (toktype == TOKEN_SPACE) {
            continue;
        }
        if (toktype == TOKEN_COMM) {
            continue;
        }
        //printf("tok=%d pos=%d line=%d [%s]\n", toktype, yacc->pos, yacc->lnum, token);

        if (toktype == TOKEN_NEWLN) {
            yacc->lnum++;
        }

        switch (yacc->pos) {
            case 0: {
                if (toktype == TOKEN_NEWLN) {
                    yacc->pos = 0;
                    break;
                }
                if (toktype != TOKEN_WORD) {
                    return -1;
                }
                yacc->pos++;
                key = strcopy(token);
                break;
            }
            case 1: {
                if (toktype != TOKEN_OPER) {
                    return -1;
                }
                yacc->pos++;
                break;
            }
            case 2: {
                if (toktype != TOKEN_WORD) {
                    return -1;
                }
                yacc->pos++;
                var = strcopy(token);
                break;
            }
            case 3: {
                if (toktype != TOKEN_NEWLN) {
                    return -1;
                }
                yacc->pos = 0;
                printf("(let %s %s)\n", key, var);
                free(key);
                free(var);
                break;
            }
        }
    }
    return 0;
}

int main(int argc, char** argv) {

    char* src = "# string comment \n# next comment \nkey1 = var1 # comment 1\nkey2 = var2 ; comment 2 and 3\n# comment 3\n";

    bstream_t stream;

    bstream_init(&stream);
    bstream_write(&stream, src, strlen(src));
    bstream_dump(&stream);

    lexer_t lexer;
    lexer_init(&lexer, &stream);

    yacc_t yacc;
    yacc_init(&yacc, &lexer);
    int res = yacc_parse(&yacc);

    if (res < 0) {
        printf("parsing error pos %d line %d\n", yacc.pos, yacc.lnum);
    }

    bstream_destroy(&stream);

    return 0;
}
