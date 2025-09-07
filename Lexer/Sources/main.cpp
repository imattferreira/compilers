#include <iostream>
#include "Token.h"
#include "Lexer.h"

using namespace std;

int main()
{
    string codigo = "int main() { int x = 10; }";
    Lexer lexer(codigo);
    vector<Token> tokens = lexer.Analisar();


    return 0;
}
