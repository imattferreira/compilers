#include <iostream>
#include <vector>
#include "Lexer.h"
#include "Token.h"

using namespace std;

int main()
{
    string codigo = "int main() {\n"
                    "  int x = 10;\n"
                    "  if (x > 5 && y != 3) {\n"
                    "    string s = \"Hello world\";\n"
                    "    x++;\n"
                    "  } else if (y <= 0.5) {\n"
                    "    x--;\n"
                    "  }\n"
                    "}";

    try {
        Lexer lexer(codigo);
        vector<Token> listaDeTokens = lexer.Analisar();
        for (const auto& token : listaDeTokens) {
            cout << "Token: " << token.getLexema() << " (" << Token::tipoParaString(token.getTipo()) << ")" << endl;
        }
    } catch (const exception& e) {
        cout << "Erro: " << e.what() << endl;
    }

    return 0;
}
