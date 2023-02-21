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

#include <tclexer.h>
#include <tccomp.h>
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

tccomp_t * new_tccomp(tclexer_t * lexer, vmapper_t* vmapper) {
    tccomp_t *comp = malloc(sizeof(tccomp_t));
    if (comp == NULL) return NULL;
    comp->lexer = lexer;
    comp->vmapper = vmapper;
    comp->pos = 0;
    comp->lnum = 0;
    return comp;
}


void tccomp_init(tccomp_t * comp, tclexer_t * lexer, vmapper_t* vmapper) {
    comp->lexer = lexer;
    comp->vmapper = vmapper;
    comp->pos = 0;
    comp->lnum = 0;
}

int tccomp_parse(tccomp_t * comp) {
    char token[MAX_TOK_SIZE];
    int toktype = -1;
    tclexer_t* lexer =  comp->lexer;
    char* key = NULL;
    char* val = NULL;

    while (true) {
        toktype = tclexer_get_token(lexer, token, MAX_TOK_SIZE);
        if (toktype == TOKEN_SPACE) {
            continue;
        }
        if (toktype == TOKEN_COMM) {
            continue;
        }
        //printf("tok=%d pos=%d line=%d [%s]\n", toktype, comp->pos, comp->lnum, token);

        if (toktype == TOKEN_NEWLN) {
            comp->lnum++;
        }

        switch (comp->pos) {
            case 0: {
                if (toktype == TOKEN_NEWLN) {
                    comp->pos = 0;
                    break;
                }
                if (toktype != TOKEN_WORD) {
                    return -1;
                }
                comp->pos++;
                key = strcopy(token);
                break;
            }
            case 1: {
                if (toktype != TOKEN_OPER) {
                    return -1;
                }
                comp->pos++;
                break;
            }
            case 2: {
                if (toktype != TOKEN_WORD) {
                    return -1;
                }
                comp->pos++;
                val = strcopy(token);
                break;
            }
            case 3: {
                if (toktype != TOKEN_NEWLN && toktype != TOKEN_ENDFL) {
                    return -1;
                }
                comp->pos = 0;
                printf("keyval = [%s], [%s]\n", key, val);
                vmapper_set(comp->vmapper, key, val);
                free(key);
                free(val);
                break;
            }
        }
        if (toktype == TOKEN_ENDFL) break;
    }
    return 0;
}

void tccomp_destroy(tccomp_t* comp) {
    (void)comp;
}

void tccomp_free(tccomp_t* comp) {
    free(comp);
}
