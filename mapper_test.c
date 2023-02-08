/*
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <mapper.h>
#include <massert.h>

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    mapper_t mapper;
    mapper_init(&mapper);

    int   port   = 0;
    bool  flag   = false;
    char* ident  = NULL;

    mapper_bind_int(&mapper, "port", &port);
    mapper_bind_string(&mapper, "ident", &ident);
    mapper_bind_bool(&mapper, "flag", &flag);

    mapper_set(&mapper, "port", "12345");
    mapper_set(&mapper, "ident", "qwerty");
    mapper_set(&mapper, "flag", "true");

    printf("port = %d\n", port);
    MASSERT(port = 12345);

    printf("ident = %s\n", ident);
    MASSERT(strcmp(ident, "qwerty") == 0);
    free(ident);

    printf("flag = %d\n", flag);
    MASSERT(flag = true);

    mapper_destroy(&mapper);

    return 0;
}
