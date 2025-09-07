#include "Lexer.h"

int i;
int estado_atual;
vector<Token> tokens;
string lexema;

Lexer :: Lexer(string codigo)
{
  this->codigo = codigo;
}

void Lexer :: q0()
{
  char c = codigo[i];
  if (isEspaco(c)) {
    i++;
  }
  else {
    if(isDigito(c)) {
      lexema = c;
      estado_atual = 1;
      i++;
    }
    else {
      // tratar outros casos
      i++;
    }
  }
}

void Lexer :: q1()
{

}

void inicializar()
{
  i = 0;
  estado_atual = 0;
  tokens.clear();
  lexema = "";
}

vector<Token> Lexer :: Analisar()
{
  inicializar();
  while (i < codigo.size()) {
    switch(estado_atual){
      case 0:
        q0();
        break;
      case 1:
        q1();
        break;
    }
  }
  return tokens;
}

bool Lexer :: isEspaco(char c)
{
  return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

bool Lexer :: isDigito(char c)
{
  return c >= '0' && c <= '9';
}
