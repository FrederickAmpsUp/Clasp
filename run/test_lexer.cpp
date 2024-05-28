#include <clasp/lexer.hpp>
#include <iostream>
#include <sstream>
#include <istream>

int main(int argc, char **argv) {
    auto input = std::stringstream("+ += - / ++ *  ");
    clasp::lexical::Analyzer lexer(input);

    clasp::lexical::Token::Ptr token = nullptr;

    while ((token = lexer.scan())->type != clasp::lexical::Token::Type::END_OF_FILE) {
        std::cout << token->to_string() << std::endl;
    }

    return 0;
}