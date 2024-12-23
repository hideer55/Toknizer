//Hideer Ali Jasib
//Hussein Ali Mahood
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <unordered_map>

using namespace std;

enum TokenType {
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    UNKNOWN
};

struct Token {
    TokenType type;
    string value;
};

class Lexer {
private:
    string input;
    int pos;
    unordered_map<string, TokenType> keywords = {
        {"if", KEYWORD},
        {"else", KEYWORD},
        {"while", KEYWORD},
        {"hideer", KEYWORD}
    };

    char peek(){
        return (pos < input.size()) ? input[pos] : '\0';
    }

    char advance() {
        return input[pos++];
    }

    void skipWhitespace() {
        while (isspace(peek())) {
            advance();
        }
    }

    Token lexNumber() {
        string value;
        while (isdigit(peek())) {
            value += advance();
        }
        return { NUMBER, value };
    }

    Token lexIdentifierOrKeyword() {
        string value;
        while (isalnum(peek()) || peek() == '_') {
            value += advance();
        }
        if (keywords.count(value)) {
            return { KEYWORD, value };
        }
        return { IDENTIFIER, value };
    }

    Token lexOperator() {
        char op = advance();
        return { OPERATOR, string(1, op) };
    }

public:
    Lexer(string& input) { pos = 0; this->input = input; }
    //Lexer(const string& input) : input(input), pos(0) {}

    vector<Token> tokenize() {
        vector<Token> tokens;
        while (pos < input.size()) {
            skipWhitespace();
            char current = peek();

            if (isdigit(current)) {
                tokens.push_back(lexNumber());
            }
            else if (isalpha(current) || current == '_') {
                tokens.push_back(lexIdentifierOrKeyword());
            }
            else if (ispunct(current)) {
                tokens.push_back(lexOperator());
            }
            else {
                tokens.push_back({ UNKNOWN, string(1, advance()) });
            }
        }
        return tokens;
    }
};

void printTokens(vector<Token> &tokens) {
    for (auto& token : tokens) {
        cout << "Token(Type: ";
        switch (token.type) {
        case KEYWORD: cout << "KEYWORD"; break;
        case IDENTIFIER: cout << "IDENTIFIER"; break;
        case NUMBER: cout << "NUMBER"; break;
        case OPERATOR: cout << "OPERATOR"; break;
        default: cout << "UNKNOWN"; break;
        }
        cout << ", Value: \"" << token.value << "\")\n";
    }
}

string readFile(string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Unable to open file: " + filename);
    }
    return string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
}

int main() {
    try {
        string filename;
        cout << "Enter the filename(full path) to analyze: ";
        getline(cin, filename);
        //cin >> filename;

        string code = readFile(filename);
        Lexer lexer(code);
        auto tokens = lexer.tokenize();
        printTokens(tokens);
    }
    catch (const exception e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}