#include <clasp/lexical.hpp>
#include <clasp/parser.hpp>
#include <clasp/target.hpp>
#include <clasp/exception.hpp>
#include <sstream>
#include <iostream>

int main(int argc, char **argv) {
    std::istringstream is("a = b = c = d + (5 * 8 < 9) - ~yoink ^ 7 ^ !6;");

    clasp::lexical::Scanner lexer(is);
    clasp::ast::Parser parser(lexer);

    try {
        clasp::ast::BaseStatement::Ptr ast = parser.statement();

        clasp::target::BuildTarget target("target/lib/transpile_c.so");

        std::vector<uint8_t> res = target.run(ast);
        std::cout << std::string(res.begin(), res.end()) << std::endl;
    } catch (clasp::error::SyntaxError& e) {
        std::cout << e.what() << std::endl;
    }


    return 0;
}