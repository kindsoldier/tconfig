/*
 * Copyright 2022 Oleg Borodin  <borodin@unix7.org>
 */


#include <stdio.h>
#include <stdlib.h>

#include <massert.h>

void x__assert  (char* path, int line, char* func) {
    printf("%s:%d: assert error in %s\n", path, line, func);
    abort();
}
