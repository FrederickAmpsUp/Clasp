#ifndef AST_H
#define AST_H

#include <clasp/lexer.h>

typedef enum {
    AST_BINOP,
    AST_UNOP,
    AST_LIT_NUMBER,
} ClaspASTNodeType;

typedef struct ClaspASTNode ClaspASTNode;

union ASTNodeData {
    struct {
        ClaspASTNode *left;
        ClaspASTNode *right;
        ClaspToken *op;
    } binop;
    struct {
        ClaspASTNode *right;
        ClaspToken *op;
    } unop;
    struct {
        ClaspToken *value;
    } lit_num;
};
typedef struct ClaspASTNode {
    ClaspASTNodeType type;
    union ASTNodeData data;
} ClaspASTNode;

ClaspASTNode *new_node(ClaspASTNodeType type, union ASTNodeData *data);
ClaspASTNode *binop(ClaspASTNode *left, ClaspASTNode *right, ClaspToken *op);
ClaspASTNode *unop(ClaspASTNode *right, ClaspToken *op);
ClaspASTNode *lit_num(ClaspToken *num);

typedef enum ClaspVisitorTypes {
    VISITOR_BINOP,
    VISITOR_UNOP,
    VISITOR_LIT_NUMBER,

    CLASP_NUM_VISITORS,
} ClaspVisitorTypes;

typedef void (*ClaspVisitorFn) (ClaspASTNode *node);
typedef ClaspVisitorFn ClaspASTVisitor[CLASP_NUM_VISITORS];

void visit(ClaspASTNode *node);

#endif // AST_H