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
#include <clexer.h>

int main(int argc, char** argv) {

    char* src = "key1 = var1 # comment 1\nkey2 = var2 # comment 2 and 3\n# comment 4\n";

    bstream_t stream;
    bstream_init(&stream);
    bstream_write(&stream, src, strlen(src));

    clexer_t lexer;
    clexer_init(&lexer, &stream);

    int token_typ = TOKEN_NULL;
    char token[MAX_TOK_SIZE];
    while ((token_typ = clexer_get_token(&lexer, token, MAX_TOK_SIZE)) != TOKEN_ENDFL) {
        printf("%d:[%s]\n", token_typ, token);
    }

    clexer_destroy(&lexer);
    bstream_destroy(&stream);

    return 0;
}
