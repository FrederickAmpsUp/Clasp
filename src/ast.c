#include <clasp/ast.h>
#include <stdlib.h>
#include <stdio.h>

ClaspASTNode *new_node(ClaspASTNodeType t, union ASTNodeData *data) {
    ClaspASTNode *node = malloc(sizeof(ClaspASTNode));
    node->type = t;
    node->data = *data;
    return node;
}


ClaspASTNode *binop(ClaspASTNode *left, ClaspASTNode *right, ClaspToken *op) {
    union ASTNodeData data = {
        .binop = {
            .left = left,
            .op = op,
            .right = right
        }
    };
    return new_node(AST_BINOP, &data);
}
ClaspASTNode *unop(ClaspASTNode *right, ClaspToken *op) {
    union ASTNodeData data = {
        .unop = {
            .op = op,
            .right = right
        }
    };
    return new_node(AST_UNOP, &data);
}
ClaspASTNode *lit_num(ClaspToken *n) {
    union ASTNodeData data = {
        .lit_num = {
            .value = n
        }
    };
    return new_node(AST_LIT_NUMBER, &data);
}

void *visit(ClaspASTNode *node, ClaspASTVisitor v) {
    if (node->type < 0 || node->type > CLASP_NUM_VISITORS) {
        fprintf(stderr, "Internal error, please report this message: \n\n\"Unknown AST node type: %d\"\n", node->type);
        exit(1);
    }
    return v[node->type](node);
}