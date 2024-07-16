#include <clasp/lexical.hpp>
#include <sstream>
#include <iostream>

int main(int argc, char **argv) {
    std::istringstream is("this_is_an_90909_identifier _soIsThis23 5 ThatWasA_Number");

    clasp::lexical::Scanner lexer(is);
    clasp::lexical::Token::Ptr tok;

    while ((tok = lexer.scan())->type != clasp::lexical::Token::Type::END_OF_FILE) {
        std::cout << tok->value << std::endl;
    }

    return 0;
}