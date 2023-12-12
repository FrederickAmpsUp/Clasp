#ifndef STRINGSTREAM_H
#define STRINGSTREAM_H

#include <clasp/lexer.h>

typedef struct {
    char *data;
    unsigned int idx;
} StringStream;

StringStream* new_stream(char *str);
char stream_read(StringStream *s);

#endif // STRINGSTREAM_H