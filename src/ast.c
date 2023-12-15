#include <clasp/ast.h>
#include <stdlib.h>
#include <stdio.h>

ClaspASTNode *new_node(ClaspASTNodeType t, union ASTNodeData *data) {
    ClaspASTNode *node = malloc(sizeof(ClaspASTNode));
    node->type = t;
    node->data = *data;
    return node;
}