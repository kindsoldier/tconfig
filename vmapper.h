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
} vmapper_t;

vmapper_t* new_vmapper(void);
void vmapper_init(vmapper_t* vmapper);
int vmapper_check_capa(vmapper_t* vmapper);
int vmapper_bind_string(vmapper_t* vmapper, char* name, char** val);
int vmapper_bind_int(vmapper_t* vmapper, char* name, int* val);
int vmapper_bind_bool(vmapper_t* vmapper, char* name, bool* val);

int vmapper_set_int(vmapper_t* vmapper, char* key, char* val);
int vmapper_set_string(vmapper_t* vmapper, char* key, char* val);
int vmapper_set_bool(vmapper_t* vmapper, char* key, char* val);

int vmapper_set(vmapper_t* vmapper, char* key, char* val);
int vmapper_bind(vmapper_t* vmapper, int type, char* name, void* val);

void vmapper_destroy(vmapper_t* vmapper);
void vmapper_free(vmapper_t* vmapper);

#endif
