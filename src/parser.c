/**
 * Clasp language Parser Implementation
 * Authored 12/2023-present
 * 
 * This program is part of the Clasp Source Libraries
 * 
 * Copyright (c) 2024, Frederick Ziola
 *                      frederick.ziola@gmail.com
 * 
 * SPDX-License-Identifier: GPL-3.0
 * 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <clasp/parser.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <clasp/ast.h>
#include <clasp/err.h>
#include <cvector/cvector.h>

void new_parser(ClaspParser *p, ClaspLexer *l) {
    p->lexer = l;
    p->variables = malloc(sizeof(hashmap_t));
    hashmap_create(5, p->variables);
    p->puncNextStmt = true;
    p->scope = 0;
}
#define consume(p, t, ...) consume_impl(p, t, (sizeof((int[]){__VA_ARGS__})/sizeof(int)), __VA_ARGS__)
static bool consume_impl(ClaspParser *p, ClaspToken **t, int n, ...) {
    va_list ptr;
    va_start(ptr, n);

    for (int i = 0; i < n; ++i) {
        if (lexer_has(p->lexer, va_arg(ptr, ClaspTokenType))) {
            if (t) *t = lexer_next(p->lexer);
            else (void) lexer_next(p->lexer);
            va_end(ptr);
            return true;
        }
    }
    va_end(ptr);
    if (t) *t = NULL;
    return false;
}

static const ClaspTokenType const SYNC_TOKENS[] = {
    TOKEN_SEMICOLON, // TODO: more of these? maybe, idk. this seems to work *ok* as is but needs more work for sure
    TOKEN_LEFT_CURLY,
    TOKEN_EOF,
};

static void parser_panic(ClaspParser *p) {
    const int n_sync = sizeof(SYNC_TOKENS) / sizeof(ClaspTokenType);

    while (true) {
        for (int i = 0; i < n_sync; ++i) {
            if (p->lexer->previous->type == SYNC_TOKENS[i]) {
                return;
            }
        }
        (void) lexer_next(p->lexer);
    }
}

// Do an error. This halts the current parse function and starts a new statement after the first sync token.
#define ERROR(message) do {\
    ClaspToken *errtok = p->lexer->previous;\
    parser_panic(p);\
    token_err(errtok, message);\
    return NULL;\
} while (0)

ClaspASTNode *parser_compile(ClaspParser *p) {
    cvector(ClaspASTNode *) block = NULL;
    while (!consume(p, NULL, TOKEN_EOF)) {
        ClaspASTNode *stmt = parser_stmt(p);
        cvector_push_back(block, stmt);
    }
    return block_stmt(block);
}

void parser_add_var(ClaspParser *p, ClaspVariable *v) {
    if(hashmap_put(p->variables, v->name, strlen(v->name), v)) {
        general_err("hashmap_put error.\n");
    }
}

ClaspASTNode *parser_stmt(ClaspParser *p) {
    while (consume(p, NULL, TOKEN_SEMICOLON));
    if (consume(p, NULL, TOKEN_EOF)) return NULL;

    if (consume(p, NULL, TOKEN_LEFT_CURLY)) {
        cvector(ClaspASTNode *) block = NULL;
        while (!consume(p, NULL, TOKEN_RIGHT_CURLY)) {
            cvector_push_back(block, parser_stmt(p));
        }
        return block_stmt(block);
    }

    if (consume(p, NULL, TOKEN_KW_VAR)) { // var declaration
        ClaspToken *name = lexer_next(p->lexer); // Name token
        ClaspASTNode *type = NULL, *initializer = NULL;
        ClaspToken *op;
        if (consume(p, &op, TOKEN_COLON, TOKEN_EQ)) {
            if (op->type == TOKEN_COLON) { // We have a typename
                type = parser_type(p);
                if (consume(p, NULL, TOKEN_EQ)) { // Typename and initializer
                    initializer = parser_expression(p);
                    if (initializer == NULL) return NULL;
                }
            } else { // Initializer only
                initializer = parser_expression(p);
                if (initializer == NULL) return NULL;
            }
            if (!consume(p, NULL, TOKEN_SEMICOLON) && p->puncNextStmt) {
                ERROR("Expected semicolon after variable declaration.");
            }
            if (!p->puncNextStmt) p->puncNextStmt = true;
            ClaspVariable *var = malloc(sizeof(ClaspVariable));
            var->name = name->data;
            struct ClaspType *vtype = malloc(sizeof(struct ClaspType));
            if (type == NULL) type = initializer->exprType->type;
            vtype->type = type;
            vtype->flag = TYPE_MUTABLE;
            var->type = vtype;
            var->scope = p->scope;
            parser_add_var(p, var);
            return var_decl(name, type, initializer);
        } else {
            ERROR("Expected typename or assignment after variable name.");
        }
    }

    if (consume(p, NULL, TOKEN_KW_LET)) { // let (immutable) declaration
        ClaspToken *name = lexer_next(p->lexer); // Name token
        ClaspASTNode *type = NULL, *initializer = NULL;
        ClaspToken *op;
        if (consume(p, &op, TOKEN_COLON, TOKEN_EQ)) {
            if (op->type == TOKEN_COLON) { // We have a typename
                type = parser_type(p);
                if (consume(p, NULL, TOKEN_EQ)) // Typename and initializer
                    initializer = parser_expression(p);
            } else { // Initializer only
                initializer = parser_expression(p);
            }
            if (initializer == NULL) return NULL;
            if (!consume(p, NULL, TOKEN_SEMICOLON) && p->puncNextStmt) {
                ERROR("Expected semicolon after immutable variable declaration.");
            }
            if (!p->puncNextStmt) p->puncNextStmt = true;
            ClaspVariable *var = malloc(sizeof(ClaspVariable));
            var->name = name->data;
            struct ClaspType *vtype = malloc(sizeof(struct ClaspType));
            if (type == NULL) type = initializer->exprType->type;
            vtype->type = type;
            vtype->flag = 0;
            var->type = vtype;
            var->scope = p->scope;
            parser_add_var(p, var);
            return let_decl(name, type, initializer);
        } else {
            ERROR("Expected typename or assignment after immutable variable name.");
        }
    }

    if (consume(p, NULL, TOKEN_KW_CONST)) { // const declaration
        ClaspToken *name = lexer_next(p->lexer); // Name token
        ClaspASTNode *type = NULL, *initializer = NULL;
        ClaspToken *op;
        if (consume(p, &op, TOKEN_COLON, TOKEN_EQ)) {
            if (op->type == TOKEN_COLON) { // We have a typename
                type = parser_type(p);
                if (!consume(p, NULL, TOKEN_EQ)) // Initializers are required for consts
                    ERROR("Expected assignment to const variable.");
            }

            initializer = parser_expression(p);
            if (initializer == NULL) return NULL;
            if (!consume(p, NULL, TOKEN_SEMICOLON) && p->puncNextStmt) {
                ERROR("Expected semicolon after constant declaration.");
            }
            if (!p->puncNextStmt) p->puncNextStmt = true;
            ClaspVariable *var = malloc(sizeof(ClaspVariable));
            var->name = name->data;
            struct ClaspType *vtype = malloc(sizeof(struct ClaspType));
            if (type == NULL) type = initializer->exprType->type;
            vtype->type = type;
            vtype->flag = TYPE_CONST;
            var->type = vtype;
            var->scope = p->scope;
            if (!(initializer->exprType->flag & TYPE_CONST)) {
                ERROR("Constant initalization to non-constant expression.");
            }
            parser_add_var(p, var);
            return const_decl(name, type, initializer);
        } else {
            ERROR("Expected typename or assignment after constant name.");
        }
    }

    if (consume(p, NULL, TOKEN_KW_FN)) {
        ClaspToken *name = lexer_next(p->lexer);
        if (!consume(p, NULL, TOKEN_LEFT_PAREN)) {
            ERROR("Expected opening parenthesis after function name.");
        }
        cvector(struct ClaspArg *) args = NULL;

        while (true) {
            ClaspToken *argname = lexer_next(p->lexer);
            if (!consume(p, NULL, TOKEN_COLON)) {
                ERROR("Expected colon after function argument name.");
            }
            ClaspASTNode *argtype = parser_type(p);
            if (argtype == NULL) return NULL;

            struct ClaspArg *arg = malloc(sizeof(struct ClaspArg));
            arg->name = argname;
            arg->type = argtype;

            cvector_push_back(args, arg);

            if (consume(p, NULL, TOKEN_RIGHT_PAREN)) break;
            if (!consume(p, NULL, TOKEN_COMMA)) {
                ERROR("Expected ',' or ')' after function argument declaration.");
            }
        }

        if (!consume(p, NULL, TOKEN_RIGHT_POINT)) {
            ERROR("Expected return type specifier after function argument list."); // TODO: show the function name here, once varargs are introduced to the ERROR macro
        }
        ClaspASTNode *rettype = parser_type(p);
        p->scope++;
        ClaspASTNode *body = parser_stmt(p);
        p->scope--;
        
        return fn_decl(name, rettype, args, body);
    }

    ClaspToken *cond_type;
    if (consume(p, &cond_type, TOKEN_KW_IF, TOKEN_KW_WHILE)) {
        if (!consume(p, NULL, TOKEN_LEFT_PAREN)) {
            switch(cond_type->type) {
                case TOKEN_KW_IF:    ERROR("Expected opening parenthesis after if keyword."   ); break;
                case TOKEN_KW_WHILE: ERROR("Expected opening parenthesis after while keyword."); break;
            }
        }
        ClaspASTNode *cond = parser_expression(p);

        if (!consume(p, NULL, TOKEN_RIGHT_PAREN)) {
            switch(cond_type->type) {
                case TOKEN_KW_IF:    ERROR("Expected closing parenthesis after if conditional."   ); break;
                case TOKEN_KW_WHILE: ERROR("Expected closing parenthesis after while conditional."); break;
            }
        }

        p->scope++;
        ClaspASTNode *body = parser_stmt(p);
        p->scope--;

        switch(cond_type->type) {
            case TOKEN_KW_IF:    return    if_stmt(cond, body); break;
            case TOKEN_KW_WHILE: return while_stmt(cond, body); break;
        }
        return NULL; // juuuust to be safe.
    }

    if (consume(p, NULL, TOKEN_KW_FOR)) {
        if (!consume(p, NULL, TOKEN_LEFT_PAREN)) {
            ERROR("Expected opening parenthesis after for keyword.");
        }
        cvector(ClaspASTNode *) out = NULL;

        ClaspASTNode *setup = parser_stmt(p); // The setup statement (eg. var i: int = 0)
        ClaspASTNode *cond = parser_expression(p); // The exit condition. (eg i < 10)
        if (!consume(p, NULL, TOKEN_SEMICOLON)) {
            ERROR("Expected semicolon after for loop conditional.");
        }
        p->puncNextStmt = false; // No semicolon after the last statement in a for loop
        ClaspASTNode *inc = parser_stmt(p); // The increment statement (eg. i++)
        if (!consume(p, NULL, TOKEN_RIGHT_PAREN)) {
            ERROR("Expected closing parenthesis after for loop increment statement.");
        }
        p->scope++;
        ClaspASTNode *body = parser_stmt(p); // The body of the loop.
        p->scope--;
        cvector(ClaspASTNode *) bodyFull = NULL;
        cvector_push_back(bodyFull, body);
        cvector_push_back(bodyFull, inc );
        ClaspASTNode *bodyFinal = block_stmt(bodyFull);

        cvector_push_back(out, setup); // Setup statement
        ClaspASTNode *loop = while_stmt(cond, bodyFinal);
        cvector_push_back(out, loop);

        return block_stmt(out);
    }
    
        // Fall-back to expression statements
    ClaspASTNode *expr = parser_expression(p);
    if (!consume(p, NULL, TOKEN_SEMICOLON) && p->puncNextStmt) {
        ERROR("Expected semicolon after expression statement.");
    }
    if (!p->puncNextStmt) p->puncNextStmt = true;
    return expr_stmt(expr);
}

// TODO: add other type nodes here
ClaspASTNode *parser_type(ClaspParser *p) {
    ClaspToken *typename;
    if (consume(p, &typename, TOKEN_ID)) {
        return type_single(typename);
    } else {
        ERROR("Temp error: unfinished type parsing system.");
    }
}

ClaspASTNode *parser_expression(ClaspParser *p) {
    return parser_assignment(p);
}
ClaspASTNode *parser_assignment(ClaspParser *p) {
    ClaspASTNode *left = parser_equality(p); // Left operand.

    ClaspToken *op;
    while (consume(p, &op,
            TOKEN_EQ,
            TOKEN_PLUS_EQ,  // Assignment operators.
            TOKEN_MINUS_EQ,
            TOKEN_ASTERIX_EQ,
            TOKEN_SLASH_EQ,
            TOKEN_PERC_EQ,
            TOKEN_CARAT_EQ,
            TOKEN_TILDE_EQ)) {
        if (!(left->exprType->flag & TYPE_MUTABLE)) { // Trying to assign to an immutable/const expression
            ERROR("Assignment to immutable or const expression.");
        }
        ClaspASTNode *right = parser_assignment(p); // right operand, right-associativity
        left = binop(left, right, op);
    }
    return left;
}
ClaspASTNode *parser_equality(ClaspParser *p) {
    ClaspASTNode *left = parser_comparison(p);  // Left operand

    ClaspToken *op;
    while (consume(p, &op, TOKEN_EQ_EQ, TOKEN_BANG_EQ)) {
        ClaspASTNode *right = parser_comparison(p); // Right operand
        left = binop(left, right, op);
    }
    return left;
}
ClaspASTNode *parser_comparison(ClaspParser *p) {
    ClaspASTNode *left = parser_term(p);  // Left operand

    ClaspToken *op;
    while (consume(p, &op, TOKEN_LESS, TOKEN_GREATER, TOKEN_LESS_EQ, TOKEN_GREATER_EQ)) {
        ClaspASTNode *right = parser_term(p); // Right operand
        left = binop(left, right, op);
    }
    return left;
}
ClaspASTNode *parser_term(ClaspParser *p) {
    ClaspASTNode *left = parser_factor(p);  // Left operand

    ClaspToken *op;
    while (consume(p, &op, TOKEN_PLUS, TOKEN_MINUS)) {
        ClaspASTNode *right = parser_factor(p); // Right operand
        left = binop(left, right, op);
    }
    return left;
}
ClaspASTNode *parser_factor(ClaspParser *p) {
    ClaspASTNode *left = parser_exponent(p); // Left operand

    ClaspToken *op;
    while (consume(p, &op, TOKEN_ASTERIX, TOKEN_SLASH, TOKEN_PERC)) {
        ClaspASTNode *right = parser_exponent(p); // Right operand
        left = binop(left, right, op);
    }
    return left;
}
ClaspASTNode *parser_exponent(ClaspParser *p) {
    ClaspASTNode *left = parser_unary(p); // left operand

    ClaspToken *op;
    while (consume(p, &op, TOKEN_CARAT)) {
        ClaspASTNode *right = parser_exponent(p); // right operand, right-associativity
        left = binop(left, right, op);
    }
    return left;
}
ClaspASTNode *parser_unary(ClaspParser *p) {
    ClaspToken *op;
    if (consume(p, &op, TOKEN_PLUS, TOKEN_MINUS, TOKEN_BANG, TOKEN_TILDE)) {
        ClaspASTNode *right = parser_unary(p); // right operand
        return unop(right, op);
    }
    return parser_postfix(p);
}
ClaspASTNode *parser_postfix(ClaspParser *p) {
    ClaspASTNode *prim = parser_primary(p);
    ClaspToken *op;
    while (consume(p, &op, TOKEN_PLUS_PLUS, TOKEN_MINUS_MINUS, TOKEN_LEFT_PAREN)) {
        if (op->type == TOKEN_LEFT_PAREN) { // function call
            cvector(ClaspASTNode *) args = NULL;
            ClaspASTNode *arg;
            while (1) {
                if (consume(p, NULL, TOKEN_RIGHT_PAREN)) break;
                arg = parser_expression(p);
                cvector_push_back(args, arg);
                if (consume(p, NULL, TOKEN_RIGHT_PAREN)) break;
                if (!consume(p, NULL, TOKEN_COMMA)) {
                    ERROR("Expected ',' or ')' after function argument.");
                } 
            }

            prim = fn_call(prim, args);
            continue;
        }
        prim = postfix(prim, op);
    }
    return prim;
}
ClaspASTNode *parser_primary(ClaspParser *p) {
    ClaspToken *val;
    if (consume(p, &val, TOKEN_NUMBER)) { // Numeric literals
        return lit_num(val);
    }

    if (consume(p, &val, TOKEN_ID)) { // Variable/fnname references
        return var_ref(p->variables, val);
    }
    
    if (consume(p, NULL, TOKEN_LEFT_PAREN)) {  // Parenthesized expression
        ClaspASTNode *expr =  parser_expression(p);
        if (!consume(p, NULL, TOKEN_RIGHT_PAREN)) {
            token_err(lexer_next(p->lexer), "Expected closing parenthesis after expression.");
            parser_panic(p);
            return NULL;
        } return expr;
    }

    ERROR("Expected expression.");
}

#undef ERROR