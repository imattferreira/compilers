#include <iostream>
#include "Lexer.h"
#include "Parser.h"
#include "AST.h"

using namespace std;

int main() {
    string codigo = "int main() { i++; return 0; }";
    Lexer lexer(codigo);
    vector<Token> tokens = lexer.Analisar();
    cout << "Tokens: ";
    for (auto token : tokens) {
        cout << token.getLexema() << " ";
    }
    cout << endl;
    
    Parser parser(tokens);
    try {
        ProgramNode *ast = parser.analisar();
        cout << "AST:" << endl << ast->toString() << endl;
        delete ast;
    } catch (exception &e) {
        cout << "Erro: " << e.what() << endl;
    }
    return 0;
}
