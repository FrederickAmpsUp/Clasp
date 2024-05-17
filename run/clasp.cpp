#include <iostream>
#include <clasp/ast.hpp>
#include <clasp/scanner.hpp>
#include <sstream>

int main(int argc, char **argv) {
    std::shared_ptr<std::istream> stream = std::make_shared<std::istringstream>("abcd _aj_93 93fjj 87.5\xff");
    Clasp::Scanner::Scanner scanner(stream);

    std::shared_ptr<Clasp::Scanner::Token> tok = std::make_shared<Clasp::Scanner::Token>();
    tok->type = Clasp::Scanner::Token::Type::NONE;

        // print tokens
    while (tok->type != Clasp::Scanner::Token::Type::END_OF_FILE) {
        tok = scanner.scan();
        std::cout << tok->toString() << std::endl;
    }

    return 0;
}