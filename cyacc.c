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
#include <vmapper.h>

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

cyacc_t * new_cyacc(clexer_t * lexer, vmapper_t* vmapper) {
    cyacc_t *yacc = malloc(sizeof(cyacc_t));
    if (yacc == NULL) return NULL;
    yacc->lexer = lexer;
    yacc->vmapper = vmapper;
    yacc->pos = 0;
    yacc->lnum = 0;
    return yacc;
}


void cyacc_init(cyacc_t * yacc, clexer_t * lexer, vmapper_t* vmapper) {
    yacc->lexer = lexer;
    yacc->vmapper = vmapper;
    yacc->pos = 0;
    yacc->lnum = 0;
}

int cyacc_parse(cyacc_t * yacc) {
    char token[MAX_TOK_SIZE];
    int toktype = -1;
    clexer_t* lexer =  yacc->lexer;
    char* key = NULL;
    char* val = NULL;

    while (true) {
        toktype = clexer_get_token(lexer, token, MAX_TOK_SIZE);
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
                val = strcopy(token);
                break;
            }
            case 3: {
                if (toktype != TOKEN_NEWLN && toktype != TOKEN_ENDFL) {
                    return -1;
                }
                yacc->pos = 0;
                printf("keyval = [%s], [%s]\n", key, val);
                vmapper_set(yacc->vmapper, key, val);
                free(key);
                free(val);
                break;
            }
        }
        if (toktype == TOKEN_ENDFL) break;
    }
    return 0;
}

void cyacc_destroy(cyacc_t* yacc) {
    (void)yacc;
}

void cyacc_free(cyacc_t* yacc) {
    free(yacc);
}
