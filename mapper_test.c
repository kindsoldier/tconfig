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

    int   port   = 0;
    bool  flag   = false;
    char* ident  = NULL;

    mapper_t mapper;
    mapper_init(&mapper);

    mapper_bind_integer(&mapper, "port", &port);
    mapper_bind_string(&mapper, "ident", &ident);
    mapper_bind_bool(&mapper, "flag", &flag);

    mapper_set_int(&mapper, "port", "12345");
    mapper_set_string(&mapper, "ident", "qwerty");
    mapper_set_bool(&mapper, "flag", "true");

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
