/*
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <bstream.h>
#include <tclexer.h>

int main(void) {

    char* src = " key1 = var1 # comment 1\nkey2 = var2 # comment 2 and 3\n# comment 4\nkey3 = var3";

    bstream_t stream;
    bstream_init(&stream);
    bstream_write(&stream, src, strlen(src));


    tclexer_t lexer;
    tclexer_init(&lexer, &stream);

    int token_typ = TOKEN_NULL;
    char token[MAX_TOK_SIZE];
    while ((token_typ = tclexer_get_token(&lexer, token, MAX_TOK_SIZE)) != TOKEN_ENDFL) {
        printf("%d:[%s]\n", token_typ, token);
    }

    tclexer_destroy(&lexer);
    bstream_destroy(&stream);

    return 0;
}
