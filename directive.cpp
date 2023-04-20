#ifndef DIRECTIVE_CPP
#define DIRECTIVE_CPP

#ifndef MAIN
#define MAIN
#include "lexer.cpp"
#include <fstream>

struct File {
    string filename;
    string   nspace;
};

int main () {
    string code{""};
    string directory {
        R"(.\myApp\src)"
    };

    string filename = directory + R"(\main.csp)";

    vector<File> files; 

    ifstream file (
        filename
    );

    while( file ) {
        code += file.get();
    }

    code = code.substr(0, code.length() - 1); // This shouldn't do anything, but removing it breaks the program

    vector<Token> tokens = parse_tokens(
        code
    );

    vector<Token> new_tokens;

    for (Token token : tokens) {

    }

    print_tokens(new_tokens);
}
#endif
#endif