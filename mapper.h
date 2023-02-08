/*
 * Copyright 2023 Oleg Borodin  <borodin@unix7.org>
 */

#ifndef MAPPER_H_QWERTY
#define MAPPER_H_QWERTY

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    char*   name;
    int     type;
    void*   vptr;
} mlink_t;

#define MAPPER_STR      1
#define MAPPER_INT      2
#define MAPPER_BOOL     3

typedef struct {
    mlink_t**   mlinks;
    size_t      size;
    size_t      capa;
    bool        err;
    char*       errstr;
} mapper_t;

mapper_t* new_mapper(void);
void mapper_init(mapper_t* mapper);
int mapper_check_capa(mapper_t* mapper);
int mapper_bind_string(mapper_t* mapper, char* name, char** val);
int mapper_bind_int(mapper_t* mapper, char* name, int* val);
int mapper_bind_bool(mapper_t* mapper, char* name, bool* val);

int mapper_set_int(mapper_t* mapper, char* key, char* val);
int mapper_set_string(mapper_t* mapper, char* key, char* val);
int mapper_set_bool(mapper_t* mapper, char* key, char* val);

int mapper_set(mapper_t* mapper, char* key, char* val);
int mapper_bind(mapper_t* mapper, int type, char* name, void* val);

void mapper_destroy(mapper_t* mapper);
void mapper_free(mapper_t* mapper);

#endif
