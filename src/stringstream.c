#include <clasp/stringstream.h>
#include <string.h>
#include <stdio.h>

char stream_read(StringStream *s) {
    if (s->idx >= strlen(s->data)) return EOF;
    return s->data[s->idx++];
}