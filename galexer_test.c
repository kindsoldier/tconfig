/*
 *
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 *
 */

#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include <galexer.h>

int main(int argc, char **argv) {
        (void)argc;
        (void)argv;

        char* arg = "--qwerty=-num-12345";

        galexer_t lex;
        galexer_init(&lex, arg);

        char token[1024];
        int toktype = TOKEN_NULL;
        int i = 0;
        while (toktype != TOKEN_ENDF) {
            toktype = galexer_gettok(&lex, token);
            printf("%d: %d [%s]\n", i, toktype, token);
            i++;
        }

        return 0;
}
