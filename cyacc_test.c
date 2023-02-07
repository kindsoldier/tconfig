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

#include <bstream.h>
#include <clexer.h>
#include <cyacc.h>

int main(int argc, char** argv) {

    char* src = "# string comment \n# next comment \nkey1 = var1 # comment 1\nkey2 = var2 ; comment 2 and 3\n# comment 3\n";

    bstream_t stream;

    bstream_init(&stream);
    bstream_write(&stream, src, strlen(src));
    bstream_dump(&stream);

    clexer_t lexer;
    clexer_init(&lexer, &stream);

    cyacc_t yacc;
    cyacc_init(&yacc, &lexer);
    int res = cyacc_parse(&yacc);

    if (res < 0) {
        printf("parsing error pos %d line %d\n", yacc.pos, yacc.lnum);
    }

    cyacc_destroy(&yacc);
    clexer_destroy(&lexer);
    bstream_destroy(&stream);
    return 0;
}
