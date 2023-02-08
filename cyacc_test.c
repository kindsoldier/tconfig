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
#include <cyacc.h>
#include <vmapper.h>
#include <massert.h>


int main(void) {

    char* src = "key1 = var1 # comment 1\nkey2 = var2 # comment 2 and 3\n# comment 4\nkey3 = var3";

    bstream_t stream;

    bstream_init(&stream);

    clexer_t lexer;
    clexer_init(&lexer, &stream);

    vmapper_t vmapper;
    vmapper_init(&vmapper);

    cyacc_t yacc;
    cyacc_init(&yacc, &lexer, &vmapper);

    bstream_write(&stream, src, strlen(src));

    int   port   = 0;
    bool  flag   = false;
    char* ident  = NULL;

    vmapper_bind_int(&vmapper, "port", &port);
    vmapper_bind_string(&vmapper, "ident", &ident);
    vmapper_bind_bool(&vmapper, "flag", &flag);

    vmapper_set(&vmapper, "port", "12345");
    vmapper_set(&vmapper, "ident", "qwerty");
    vmapper_set(&vmapper, "flag", "true");

    int res = cyacc_parse(&yacc);

    if (res < 0) {
        printf("parsing error pos %d line %d\n", yacc.pos, yacc.lnum);
    }

    printf("port = %d\n", port);
    MASSERT(port == 12345);

    printf("ident = %s\n", ident);
    MASSERT(strcmp(ident, "qwerty") == 0);
    free(ident);

    printf("flag = %d\n", flag);
    MASSERT(flag == true);

    cyacc_destroy(&yacc);
    clexer_destroy(&lexer);
    vmapper_destroy(&vmapper);
    bstream_destroy(&stream);

    return 0;
}
