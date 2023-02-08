/*
 *
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 *
 */


#include <stdio.h>
#include <fcntl.h>
#include <tconfig.h>
#include <massert.h>

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;


    tconfig_t tconfig;
    tconfig_init(&tconfig);

    int intkey = 0;
    char* strkey = NULL;

    tconfig_bind(&tconfig, TCONF_INT, "intkey", &intkey);
    tconfig_bind(&tconfig, TCONF_STR, "strkey", &strkey);

    ssize_t rsize = tconfig_read(&tconfig, "test.conf");
    printf("%ld\n", rsize);
    MASSERT(rsize > 0);

    int res = tconfig_parse(&tconfig);
    MASSERT(res == -1);

    tconfig_destroy(&tconfig);

    printf("int key = %d\n", intkey);
    printf("str key = %s\n", strkey);
    return 0;
}
