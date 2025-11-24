#include <iostream>
#include "Lexer.h"
#include "Parser.h"
#include "AST.h"

using namespace std;

void mostrarAst(string codigo)
{
  try
  {
    Lexer lexer(codigo);
    vector<Token> tokens = lexer.Analisar();
    cout << "Tokens: ";
    for (auto token : tokens)
    {
      cout << token.getLexema() << " ";
    }
    cout << endl
         << endl;

    Parser parser(tokens);
    ProgramNode *ast = parser.analisar();

    cout << "AST Construida:" << endl;
    cout << ast->toString() << endl
         << endl;

    delete ast;
  }
  catch (exception &e)
  {
    cout << "Erro: " << e.what() << endl;
  }
}

void testarExpressaoAritmetica()
{
  cout << "\n=== Teste: Expressao Aritmetica ===" << endl;
  string codigo = "10 + 20 * 30";
  mostrarAst(codigo);
}

void testarAtribuicao()
{
  cout << "\n=== Teste: Atribuicao ===" << endl;
  string codigo = "int x = 10";
  mostrarAst(codigo);
}

void testarAtribuicao2()
{
  cout << "\n=== Teste: Atribuicao 2===" << endl;
  string codigo = "int menino neymar + 23";
  mostrarAst(codigo);
}

void testarProgramaCompleto()
{
  cout << "\n=== Teste: Programa Completo ===" << endl;
  string codigo = "int main() { int x = 10; int y = 20; return x + y; } ";

  mostrarAst(codigo);
}

void testarProgramaCompleto2()
{
  cout << "\n=== Teste: Programa Completo 2 ===" << endl;
  string codigo = "int main() { double _neymar_10 = 9.75, messi = 0; string texto = \"santos\"; if (xxx >= r9) while (teste == 10 || teste2 != 20 && !sair) a[75] = (123.456 - 5) / 2; i++; return 0; }";

  mostrarAst(codigo);
}

void testarEstruturasControle()
{
  cout << "\n=== Teste: Estruturas de Controle ===" << endl;
  string codigo = "int main() { int x = 10; if (x > 0) { x = x + 1; } while (x < 20) { x = x + 1; } return x; } ";

  mostrarAst(codigo);
}

void testarFor()
{
  cout << "\n=== Teste: Loop For ===" << endl;
  string codigo = "int main() { int i; for (i = 0; i < 10; i = i + 1) { int x = i; } return 0; } ";

  mostrarAst(codigo);
}

int main()
{
  cout << "Iniciando Testes do Compilador" << endl
       << "===============================" << endl;

  testarAtribuicao();
  // testarAtribuicao2();
  // testarProgramaCompleto();
  // testarProgramaCompleto2();
  // testarEstruturasControle();
  // testarFor();

  cout << "Todos os testes concluidos com sucesso!" << endl;

  return 0;
}
