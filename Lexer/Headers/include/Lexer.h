#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <vector>
#include "Token.h"

class Lexer {
  public:
    Lexer(string codigo);
    vector<Token> Analisar();

    protected:

    private:
      string codigo;
      void q0();
      void q1();
      bool isEspaco(char c);
      bool isDigito(char c);


};

#endif
