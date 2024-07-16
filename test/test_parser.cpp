#include <clasp/lexical.hpp>
#include <clasp/parser.hpp>
#include <sstream>
#include <iostream>

int main(int argc, char **argv) {
    std::istringstream is("9 + -3 * 6");

    clasp::lexical::Scanner lexer(is);
    clasp::ast::Parser parser(lexer);

    clasp::ast::BaseExpression::Ptr ast = parser.expression();

    std::cout << clasp::util::ASTStringifier().stringify(ast) << std::endl;

    return 0;
}