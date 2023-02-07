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

#include <clexer.h>
#include <cyacc.h>

#define RES_OK   0
#define RES_ERR -1

#define POS1TYPE TOKEN_WORD
#define POS2TYPE TOKEN_OPER
#define POS3TYPE TOKEN_WORD
#define POS4TYPE TOKEN_COMM

static char* strcopy(char* src) {
    size_t srcsize = strlen(src) + 1;
    char* dst = malloc(srcsize);
    memset(dst, '\0', srcsize);
    strcpy(dst, src);
    return dst;
}

void cyacc_init(cyacc_t * yacc, clexer_t * lexer) {
    yacc->lexer = lexer;
    yacc->pos = 0;
    yacc->lnum = 0;
}

int cyacc_parse(cyacc_t * yacc) {
    char token[MAX_TOK_SIZE];
    int toktype = -1;
    clexer_t* lexer =  yacc->lexer;
    char* key = NULL;
    char* var = NULL;

    while ((toktype = clexer_get_token(lexer, token, MAX_TOK_SIZE)) != TOKEN_ENDFL) {
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

void cyacc_destroy(cyacc_t* yacc) {
}
