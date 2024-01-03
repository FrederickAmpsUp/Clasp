#ifndef VISITOR_H
#define VISITOR_H

/**
 * Visitor functions for different AST node types.
 * These are automatically called by the visit() function in clasp/ast.h
*/
void *visitBinop(ClaspASTNode *binop);
void *visitUnop(ClaspASTNode *unop);
void *visitPostfix(ClaspASTNode *postfix);
void *visitNumLiteral(ClaspASTNode *lit);
void *visitVarRef(ClaspASTNode *var);
void *visitFnCall(ClaspASTNode *fn);

void *visitExprStmt(ClaspASTNode *expr);
void *visitBlockStmt(ClaspASTNode *block);
void *visitVarDecl(ClaspASTNode *var);
void *visitFnDecl(ClaspASTNode *fn);

void *visitIfStmt(ClaspASTNode *stmt);
void *visitWhileStmt(ClaspASTNode *stmt);

void *visitSingleType(ClaspASTNode *type);

ClaspASTVisitor clasp_ast_printer = {
    [AST_EXPR_BINOP     ] = &visitBinop,
    [AST_EXPR_UNOP      ] = &visitUnop,
    [AST_EXPR_POSTFIX   ] = &visitPostfix,
    [AST_EXPR_LIT_NUMBER] = &visitNumLiteral,
    [AST_EXPR_VAR_REF   ] = &visitVarRef,
    [AST_EXPR_FN_CALL   ] = &visitFnCall,

    [AST_EXPR_STMT      ] = &visitExprStmt,
    [AST_BLOCK_STMT     ] = &visitBlockStmt,
    [AST_VAR_DECL_STMT  ] = &visitVarDecl,
    [AST_LET_DECL_STMT  ] = &visitVarDecl,
    [AST_CONST_DECL_STMT] = &visitVarDecl,

    [AST_FN_DECL_STMT   ] = &visitFnDecl,

    [AST_IF_STMT        ] = &visitIfStmt,
    [AST_WHILE_STMT     ] = &visitWhileStmt,

    [AST_TYPE_SINGLE    ] = &visitSingleType,
};

#endif // VISITOR_H