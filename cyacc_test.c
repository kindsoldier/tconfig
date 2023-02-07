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
#include <mapper.h>
#include <massert.h>


int main(int argc, char** argv) {

    char* src = "key1 = var1 # comment 1\nkey2 = var2 # comment 2 and 3\n# comment 4\nkey3 = var3";

    bstream_t stream;

    bstream_init(&stream);

    clexer_t lexer;
    clexer_init(&lexer, &stream);

    mapper_t mapper;
    mapper_init(&mapper);

    cyacc_t yacc;
    cyacc_init(&yacc, &lexer, &mapper);

    bstream_write(&stream, src, strlen(src));

    int   port   = 0;
    bool  flag   = false;
    char* ident  = NULL;

    mapper_bind_integer(&mapper, "port", &port);
    mapper_bind_string(&mapper, "ident", &ident);
    mapper_bind_bool(&mapper, "flag", &flag);

    mapper_set(&mapper, "port", "12345");
    mapper_set(&mapper, "ident", "qwerty");
    mapper_set(&mapper, "flag", "true");

    int res = cyacc_parse(&yacc);

    if (res < 0) {
        printf("parsing error pos %d line %d\n", yacc.pos, yacc.lnum);
    }

    printf("port = %d\n", port);
    MASSERT(port = 12345);

    printf("ident = %s\n", ident);
    MASSERT(strcmp(ident, "qwerty") == 0);
    free(ident);

    printf("flag = %d\n", flag);
    MASSERT(flag = true);

    cyacc_destroy(&yacc);
    clexer_destroy(&lexer);
    mapper_destroy(&mapper);
    bstream_destroy(&stream);

    free(ident);

    return 0;
}
