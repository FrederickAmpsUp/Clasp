#ifndef VARIABLE_H
#define VARIABLE_H

#include <clasp/ast.h>
#include <stdint.h>

typedef struct ClaspVariable {
    char *name;
    uint16_t scope;
    struct ClaspType *type;
} ClaspVariable;

#endif // VARIABLE_H