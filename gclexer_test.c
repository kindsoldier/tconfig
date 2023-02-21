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

int main(int argc, char **argv) {
        (void)argc;
        (void)argv;

        char*_argv[] = { argv[0], "--qwerty=-num12345", "--foo=-bar" };
        int _argc = 2;

        gclexer_t lex;
        gclexer_init(&lex, _argv, _argc);

        char token[1024];
        int toktype = TOKEN_NULL;
        int i = 0;
        while (toktype != TOKEN_ENDF) {
            toktype = gclexer_gettok(&lex, token);
            printf("%d: %d [%s]\n", i, toktype, token);
            i++;
        }

        return 0;
}
