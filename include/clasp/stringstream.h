#ifndef STRINGSTREAM_H
#define STRINGSTREAM_H

#include <clasp/lexer.h>

typedef struct {
    char *data;
    unsigned int idx;
} StringStream;

#endif // STRINGSTREAM_H