#include <clasp/lexical.hpp>
#include <clasp/parser.hpp>
#include <clasp/exception.hpp>
#include <sstream>
#include <iostream>

int main(int argc, char **argv) {
    std::istringstream is("a = b = c = d + (5 * 8 < 9) - ~yoink ^ 7 ^ !6; let a: i32 = 5 + b;");

    clasp::lexical::Scanner lexer(is);
    clasp::ast::Parser parser(lexer);

    while (1)
    try {
        clasp::ast::BaseStatement::Ptr ast = parser.statement();
        std::cout << clasp::util::ASTStringifier().stringify(ast);
    } catch (clasp::error::SyntaxError& e) {
        std::cout << std::endl << e.what() << std::endl;
        break;
    }


    return 0;
}