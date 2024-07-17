#include <clasp/lexical.hpp>
#include <clasp/parser.hpp>
#include <clasp/exception.hpp>
#include <sstream>
#include <iostream>

int main(int argc, char **argv) {
    std::istringstream is("1 = 2 = 3 = 3 + (5 * 8 < 9) - ~5 ^ 7 ^ !6");

    clasp::lexical::Scanner lexer(is);
    clasp::ast::Parser parser(lexer);

    try {
        clasp::ast::BaseExpression::Ptr ast = parser.expression();
        std::cout << clasp::util::ASTStringifier().stringify(ast) << std::endl;
    } catch (clasp::error::SyntaxError& e) {
        std::cout << e.what() << std::endl;
    }


    return 0;
}