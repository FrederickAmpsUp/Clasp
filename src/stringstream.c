#include <clasp/stringstream.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

StringStream *new_stream(char *str) {
    char *new = malloc(strlen(str) + 1);
    strcpy(new, str);
    new[strlen(new)-1] = '\0';

    StringStream *stream = malloc(sizeof(StringStream));
    stream->data = new;
    stream->idx = 0;

    return stream;
}

char stream_read(StringStream *s) {
    if (s->idx >= strlen(s->data)) return EOF;
    return s->data[s->idx++];
}