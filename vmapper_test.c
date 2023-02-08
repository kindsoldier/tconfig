/*
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <vmapper.h>
#include <massert.h>

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    vmapper_t vmapper;
    vmapper_init(&vmapper);

    int   port   = 0;
    bool  flag   = false;
    char* ident  = NULL;

    vmapper_bind_int(&vmapper, "port", &port);
    vmapper_bind_string(&vmapper, "ident", &ident);
    vmapper_bind_bool(&vmapper, "flag", &flag);

    vmapper_set(&vmapper, "port", "12345");
    vmapper_set(&vmapper, "ident", "qwerty");
    vmapper_set(&vmapper, "flag", "true");

    printf("port = %d\n", port);
    MASSERT(port = 12345);

    printf("ident = %s\n", ident);
    MASSERT(strcmp(ident, "qwerty") == 0);
    free(ident);

    printf("flag = %d\n", flag);
    MASSERT(flag = true);

    vmapper_destroy(&vmapper);

    return 0;
}
