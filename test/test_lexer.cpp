#include <clasp/lexical.hpp>
#include <sstream>
#include <iostream>

int main(int argc, char **argv) {
    std::istringstream is("+ - */+ *");

    clasp::lexical::Scanner lexer(is);
    clasp::lexical::Token::Ptr tok;

    while ((tok = lexer.scan())->type != clasp::lexical::Token::Type::END_OF_FILE) {
        std::cout << tok->value << std::endl;
    }

    return 0;
}