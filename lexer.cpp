#ifndef LEXER_CPP
#define LEXER_CPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Token {
    string type;
    string value;
};

void error (string type, string err) {
    cerr << type << ": " << err << endl;
    exit(-1);
}

void error (string type, string err, int code) {
    cerr << type << ": " << err << endl;
    exit(code);
}

const std::vector<std::string> claspTokens = {"::", "->", "+=", "-=", "*=", "/=", "%=", "++", "--", "==", "!=", "<=", ">=", ">>", "<<", "&&", "||", "+", "-", "*", "/", "%", "(", ")", "{", "}", "[", "]", ",", ";", ":", "?"};

const std::vector<std::string> claspPunctuationTokens = {"->", "(", ")", "{", "}", ";", "[", "]", ":"};
const std::vector<std::string> claspOperators = {"+", "-", "*", "/", "%", "&", "+=", "-=", "*=", "/=", "%=", "==", "!=", "<=", ">=", "<<", "&&", "||", ">>", "<<=", ">>=", "="}; // TODO: organise this

// https://chat.openai.com/
void padTokens(string& code, const vector<string>& tokens) {
    string result;
    bool inQuotes;
    int i = 0;
    while (i < code.length()) {
        bool matched = false;
        if (!inQuotes) {
            for (const string& token : tokens) {
                if (code.substr(i, token.length()) == token) {
                    result += " " + token + " ";
                    i += token.length();
                    matched = true;
                    break;
                }
            }
        }
        if (!matched) {
            if (code[i] == '"') {
                inQuotes = !inQuotes;
            }
            result += code[i];
            i++;
        }
    }
    code = result;
}

bool isdigit(const char str) {
    return (str >= '0' && str <= '9') || str == '.' || str == '_';
}

bool iswhitespace(const char str) {
    if (str == ' ')
        return true;
    if (str == '\n')
        return true;
    if (str == '\t')
        return true;
    return false;
}

vector<Token> tokenize ( const string claspCode ) {
    int i = 0;

    char c;
    char next;

    vector<Token> tokens;

    while (i < claspCode.length()) {
        c = claspCode[i];
        next = claspCode[i + 1];

        if (isdigit(c)) {
            int j = i;
            string num;
            bool err = false;
            while (!iswhitespace(c = claspCode[j++])) {
                num += c;
                if (!isdigit(c)) {
                    err = true;
                }
            }
            if(err)
                error (
                    "NumberFormatError",
                    "Invalid number: \"" + num + "\""
                );
            tokens.push_back({"NUMBER", num});
            i = j;

        } else if(c == '#') {
            string directive;
            char current;
            while (!iswhitespace(current = claspCode[i++])) {
                directive += current;
            }
            tokens.push_back(Token{"DIRECTIVE", directive});
        } else if (iswhitespace(c)) {
            i++;
        }else if (c == '"') {
            string str;
            i++;
            while ((c = claspCode[i++]) != '"') {
                str += c;
            }
            i++;
            tokens.push_back(Token{"STRING", str});
        } else {
            string tok;
            char current;
            i -= 1;
            while (iswhitespace(claspCode[++i]));
            while (!iswhitespace(current = claspCode[i++])) {
                tok += current;
            }
            i--;
            if (find(claspOperators.begin(), claspOperators.end(), tok) != std::end(claspOperators)) {
                tokens.push_back(Token{"OPERATOR", tok});
            } else if (find(claspPunctuationTokens.begin(), claspPunctuationTokens.end(), tok) != std::end(claspPunctuationTokens)) {
                tokens.push_back(Token{"PUNCTUATION", tok});
            } else {
                tokens.push_back(Token{"IDENTIFIER", tok});
            }
        }
    }
    return tokens;
}

void print_tokens ( vector<Token> tokens ) {
    for (Token token : tokens) {
        cout << token.type << ": \"" << token.value << "\"" << endl;
    }
}

vector<Token> parse_tokens (string code) {
    padTokens (code, claspTokens);
    return tokenize (code);
}

#ifndef MAIN
#define MAIN
int main ( int argc, char *argv[] ) {
    string code = "(6 + 5 * \"null\") ";
    
    vector<Token> tokens = parse_tokens(code);

    print_tokens(tokens);
    return 0;
}
#endif
#endif