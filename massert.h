/*
 * Copyright 2022 Oleg Borodin  <borodin@unix7.org>
 */


#ifndef MASSERT_H_QWERTY
#define MASSERT_H_QWERTY

#define massert(expr) if (!(expr)) \
    { x__assert(__FILE__, __LINE__, (const char*)__func__); }

#define MASSERT(expr) if (!(expr)) \
    { x__assert(__FILE__, __LINE__, (const char*)__func__); }

void x__assert  (char* path, int line, const char* func);
#endif
