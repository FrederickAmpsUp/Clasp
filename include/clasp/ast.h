#ifndef AST_H
#define AST_H

#include <clasp/lexer.h>

typedef enum {
    AST_BINOP,
    AST_UNOP,
} ClaspASTNodeType;

struct ClaspASTNode;
union ASTNodeData {
    struct {
        struct ClaspASTNode *left;
        struct ClaspASTNode *right;
        ClaspToken *op;
    } binop;
    struct {
        struct ClaspASTNode *right;
        ClaspToken *op;
    } unop;
};
typedef struct {
    ClaspASTNodeType type;
    union ASTNodeData data;
} ClaspASTNode;

ClaspASTNode *new_node(ClaspASTNodeType type, union ASTNodeData *data);

#endif // AST_H