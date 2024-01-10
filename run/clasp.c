#include <clasp/clasp.h>
#include <clasp/fstream.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: %s <filename> <target>\n", argv[0]);
        return -1;
    }

    char *filename = argv[1];
    FileStream *stream = new_fstream(filename);


    ClaspLexer *lexer = malloc(sizeof(ClaspLexer));
    new_lexer(lexer, (StreamReadFn)&fstream_read, stream);
    ClaspParser *parser = malloc(sizeof(ClaspParser));
    new_parser(parser, lexer);

    ClaspASTNode *ast = parser_compile(parser);
    ClaspTarget *target = new_target(argv[2]);

    if (target->type != TARGET_VISITOR) {
        printf("Error: only AST visitor targets are currently supported.\n");
    }
        // use the target :)
    visit(ast, target->visitor);

    return 0;
}