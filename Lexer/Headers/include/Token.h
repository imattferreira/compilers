#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include "TipoDeToken.h"

using namespace std;

class Token {
  public:
    Token(TipoDeToken tipo, string lexema) : tipo(tipo), lexema(lexema) {}

  TipoDeToken getTipo() { return tipo; }

  string getLexema() { return lexema; }


  private:
      TipoDeToken tipo;
      string lexema;
};

#endif
