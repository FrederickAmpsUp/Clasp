#include <clasp/clasp.h>

void *visit_binop(ClaspASTNode *binop, void *args);
void *visit_unop(ClaspASTNode *unop, void *args);
void *visit_postfix(ClaspASTNode *postfix, void *args);
void *visit_lit_num(ClaspASTNode *lit_num, void *args);
void *visit_var_ref(ClaspASTNode *var_ref, void *args);
void *visit_fn_call(ClaspASTNode *fn_call, void *args);
void *visit_expr_stmt(ClaspASTNode *expr_stmt, void *args);
void *visit_block_stmt(ClaspASTNode *block_stmt, void *args);
void *visit_var_decl(ClaspASTNode *var_decl, void *args);
void *visit_fn_decl(ClaspASTNode *fn_decl, void *args);
void *visit_if(ClaspASTNode *if_stmt, void *args);
void *visit_while(ClaspASTNode *while_stmt, void *args);
void *visit_single_type(ClaspASTNode *single_type, void *args);

static ClaspASTVisitor _PRINTER = {
    [AST_EXPR_BINOP     ] = &visit_binop,
    [AST_EXPR_UNOP      ] = &visit_unop,
    [AST_EXPR_POSTFIX   ] = &visit_postfix,
    [AST_EXPR_LIT_NUMBER] = &visit_lit_num,
    [AST_EXPR_VAR_REF   ] = &visit_var_ref,
    [AST_EXPR_FN_CALL   ] = &visit_fn_call,

    [AST_EXPR_STMT      ] = &visit_expr_stmt,
    [AST_BLOCK_STMT     ] = &visit_block_stmt,
    [AST_VAR_DECL_STMT  ] = &visit_var_decl,
    [AST_LET_DECL_STMT  ] = &visit_var_decl,
    [AST_CONST_DECL_STMT] = &visit_var_decl,

    [AST_FN_DECL_STMT   ] = &visit_fn_decl,

    [AST_IF_STMT        ] = &visit_if,
    [AST_WHILE_STMT     ] = &visit_while,

    [AST_TYPE_SINGLE    ] = &visit_single_type,
};

void *visit_binop(ClaspASTNode *binop, void *args) {
    printf("(binop: left=");
    visit(binop->data.binop.left, args, _PRINTER);
    printf(" op=%s right=", binop->data.binop.op->data);
    visit(binop->data.binop.right, args, _PRINTER);
    printf(")");
    return NULL;
}
void *visit_unop(ClaspASTNode *unop, void *args) {
    printf("(unop: op=%s right=", unop->data.unop.op->data);
    visit(unop->data.unop.right, args, _PRINTER);
    printf(")");
    return NULL;
}

void *visit_postfix(ClaspASTNode *post, void *args) {
    printf("(postfix: left=");
    visit(post->data.postfix.left, args, _PRINTER);
    printf(" op=%s)", post->data.postfix.op->data);
    return NULL;
}
void *visit_lit_num(ClaspASTNode *lit, void *args) {
    printf("(num_literal: val=%s)", lit->data.lit_num.value->data);
    return NULL;
}

void *visit_var_ref(ClaspASTNode *var, void *args) {
    printf("(var_ref: name=%s)", var->data.var_ref.varname->data);
    return NULL;
}

void *visit_fn_call(ClaspASTNode *fn, void *args) {
    printf("(fn_call: ref=");
    visit(fn->data.fn_call.referencer, args, _PRINTER);
    printf(" args=[  ");

    for (int i = 0; i < cvector_size(fn->data.fn_call.args); ++i) {
        printf("\b\b");
        visit(fn->data.fn_call.args[i], args, _PRINTER);
        printf(",   ");
    }
    printf("\b\b\b\b])");
}

void *visit_expr_stmt(ClaspASTNode *ast, void *args) {
    printf("(exprStmt: ");
    visit(ast->data.expr_stmt.expr, args, _PRINTER);
    printf(")\n");
    return NULL;
}

void *visit_block_stmt(ClaspASTNode *ast, void *args) {
    printf("(blockStmt:\n");
    for (int i = 0; i < cvector_size(ast->data.block_stmt.body); ++i) {
        visit(ast->data.block_stmt.body[i], args, _PRINTER);
    }
    printf(")\n");
    return NULL;
}

void *visit_var_decl(ClaspASTNode *ast, void *args) {
    switch (ast->type) {
        case AST_VAR_DECL_STMT:   printf("(varDecl:");   break;
        case AST_LET_DECL_STMT:   printf("(letDecl:");   break;
        case AST_CONST_DECL_STMT: printf("(constDecl:"); break;
    }
    printf(" name=\"%s\"", ast->data.var_decl_stmt.name->data);
    if (ast->data.var_decl_stmt.type) {
        printf(" type=");
        visit(ast->data.var_decl_stmt.type, args, _PRINTER);
    }

    if (ast->data.var_decl_stmt.initializer) {
        printf(" initializer=");
        visit(ast->data.var_decl_stmt.initializer, args, _PRINTER);
    }
    printf(")\n");
}
void *visit_let_decl(ClaspASTNode *ast, void *args) {
    return visit_var_decl(ast, args);
}
void *visit_const_decl(ClaspASTNode *ast, void *args) {
    return visit_var_decl(ast, args);
}

void *visit_fn_decl(ClaspASTNode *ast, void *args) {
    printf("fnDecl: name=\"%s\" ret=", ast->data.fn_decl_stmt.name->data);
    visit(ast->data.fn_decl_stmt.ret_type, args, _PRINTER);
    printf(" args=[  ");
    for (int i = 0; i < cvector_size(ast->data.fn_decl_stmt.args); ++i) {
        struct ClaspArg *arg = ast->data.fn_decl_stmt.args[i];
        printf("\b\b(%s ", arg->name->data);
        visit(arg->type, args, _PRINTER);
        printf("),   ");
    }
    printf("\b\b\b\b] body=");
    visit(ast->data.fn_decl_stmt.body, args, _PRINTER);
    printf(")\n");
}

void *visit_if(ClaspASTNode *ast, void *args) {
    printf("(ifStmt: cond=");
    visit(ast->data.cond_stmt.cond, args, _PRINTER);
    printf(" body=");
    visit(ast->data.cond_stmt.body, args, _PRINTER);
    printf(")\n");
}

void *visit_while(ClaspASTNode *ast, void *args) {
    printf("(whileStmt: cond=");
    visit(ast->data.cond_stmt.cond, args, _PRINTER);
    printf(" body=");
    visit(ast->data.cond_stmt.body, args, _PRINTER);
    printf(")\n");
}

void *visit_single_type(ClaspASTNode *ast, void *args) {
    printf("[single name=\"%s\"]", ast->data.single.name->data);
}

void target_run(ClaspASTNode *ast, void *args) {
    (void) visit(ast, args, _PRINTER);
}