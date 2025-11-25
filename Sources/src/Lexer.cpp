#include "Lexer.h"
#include <stdexcept>
#include <iostream>
#include <set>

Lexer::Lexer(const string &codigo)
{
  this->codigo = codigo;
  inicializar();
}

void Lexer::inicializar()
{
  tokens.clear();
  i = 0;
  estado_atual = 0;
  lexema = "";
}

bool Lexer::isEspaco(char c)
{
  return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool Lexer::isDigito(char c)
{
  return c >= '0' && c <= '9';
}

bool Lexer::isOperadorAritmetico(char c)
{
  return c == '+' || c == '-' || c == '*' || c == '/';
}

bool Lexer::isLetra(char c)
{
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lexer::isUnderline(char c)
{
  return c == '_';
}

bool Lexer::isOperadorRelacional(char c)
{
  return c == '>' || c == '<' || c == '=';
}

bool Lexer::isOperadorLogico(char c)
{
  return c == '&' || c == '|';
}

bool Lexer::isPontuacao(char c)
{
  return c == ';' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ':' || c == '?';
}

vector<Token> Lexer::Analisar()
{
  while (i < codigo.size())
  {
    switch (estado_atual)
    {
    case 0:
      q0();
      break;
    case 1:
      q1();
      break;
    case 2:
      q2();
      break;
    case 3:
      q3();
      break;
    case 4:
      q4();
      break;
    case 5:
      q5();
      break;
    case 6:
      q6();
      break;
    case 7:
      q7();
      break;
    case 8:
      q8();
      break;
    case 9:
      q9();
      break;
    case 10:
      q10();
      break;
    default:
      throw runtime_error("Estado invalido");
    }
  }
  return tokens;
}

// Estado q0: Estado inicial
void Lexer::q0()
{
  if (i >= codigo.size())
  {
    return;
  }
  char c = codigo[i];
  if (isEspaco(c))
  {
    i++;
  }
  else if (isDigito(c))
  {
    lexema = c;
    estado_atual = 1;
    i++;
  }
  else if (isOperadorAritmetico(c))
  {
    if (c == '+' && i + 1 < codigo.size() && codigo[i + 1] == '+')
    {
      lexema = "++";
      estado_atual = 7;
      i += 2;
    }
    else if (c == '-' && i + 1 < codigo.size() && codigo[i + 1] == '-')
    {
      lexema = "--";
      estado_atual = 8;
      i += 2;
    }
    else
    {
      lexema = c;
      estado_atual = 2;
      i++;
    }
  }
  else if (c == '"')
  {
    lexema = "";
    estado_atual = 6;
    i++;
  }
  else if (isLetra(c) || isUnderline(c))
  {
    lexema = c;
    estado_atual = 3;
    i++;
  }
  else if (c == '!')
  {
    lexema = c;
    estado_atual = 9;
    i++;
  }
  else if (isOperadorRelacional(c))
  {
    lexema = c;
    estado_atual = 4;
    i++;
  }
  else if (isOperadorLogico(c))
  {
    lexema = c;
    estado_atual = 9;
    i++;
  }
  else if (isPontuacao(c))
  {
    lexema = c;
    estado_atual = 5;
    i++;
  }
  else
  {
    string msg = "Caractere invalido: ";
    msg += c;
    throw runtime_error(msg);
  }
}

// Estado q1: Reconhece números inteiros e reais
void Lexer::q1()
{
  if (i >= codigo.size())
  {
    tokens.push_back(Token(TipoDeToken::NUMERO_INTEIRO, lexema));
    estado_atual = 0;
    return;
  }
  char c = codigo[i];
  if (isDigito(c))
  {
    lexema.push_back(c);
    i++;
  }
  else if (c == '.')
  {
    lexema.push_back(c);
    estado_atual = 10;
    i++;
  }
  else
  {
    tokens.push_back(Token(TipoDeToken::NUMERO_INTEIRO, lexema));
    lexema = "";
    estado_atual = 0;
  }
}

// Estado q2: Reconhece operadores aritméticos
void Lexer::q2()
{
  tokens.push_back(Token(TipoDeToken::OPERADOR_ARITMETICO, lexema));
  lexema = "";
  estado_atual = 0;
}

// Estado q3: Reconhece identificadores e palavras reservadas
void Lexer::q3()
{
  const set<string> palavrasReservadas = {"int", "double", "string", "main", "if", "else", "while", "for", "do", "return"};

  if (i >= codigo.size())
  {
    if (palavrasReservadas.count(lexema))
    {
      tokens.push_back(Token(TipoDeToken::PALAVRA_RESERVADA, lexema));
    }
    else
    {
      tokens.push_back(Token(TipoDeToken::IDENTIFICADOR, lexema));
    }
    estado_atual = 0;
    return;
  }

  char c = codigo[i];
  if (isLetra(c) || isUnderline(c) || isDigito(c))
  {
    lexema.push_back(c);
    i++;
  }
  else
  {
    if (palavrasReservadas.count(lexema))
    {
      tokens.push_back(Token(TipoDeToken::PALAVRA_RESERVADA, lexema));
    }
    else
    {
      tokens.push_back(Token(TipoDeToken::IDENTIFICADOR, lexema));
    }
    lexema = "";
    estado_atual = 0;
  }
}

// Estado q4: Reconhece operadores relacionais e de atribuição
void Lexer::q4()
{
  if (i < codigo.size() && codigo[i] == '=')
  {
    lexema.push_back(codigo[i]);
    i++;
  }
  if (lexema == "=")
  {
    tokens.push_back(Token(TipoDeToken::OPERADOR_ATRIBUICAO, lexema));
  }
  else
  {
    tokens.push_back(Token(TipoDeToken::OPERADOR_RELACIONAL, lexema));
  }
  lexema = "";
  estado_atual = 0;
}

// Estado q5: Reconhece pontuação de 1 caractere
void Lexer::q5()
{
  TipoDeToken tipo;
  if (lexema == ";")
  {
    tipo = TipoDeToken::PONTO_E_VIRGULA;
  }
  else if (lexema == "(")
  {
    tipo = TipoDeToken::ABRE_PARENTESES;
  }
  else if (lexema == ")")
  {
    tipo = TipoDeToken::FECHA_PARENTESES;
  }
  else if (lexema == "{")
  {
    tipo = TipoDeToken::ABRE_CHAVES;
  }
  else if (lexema == "}")
  {
    tipo = TipoDeToken::FECHA_CHAVES;
  }
  else if (lexema == "[")
  {
    tipo = TipoDeToken::ABRE_COLCHETES;
  }
  else if (lexema == "]")
  {
    tipo = TipoDeToken::FECHA_COLCHETES;
  }
  else if (lexema == ",")
  {
    tipo = TipoDeToken::VIRGULA;
  }
  else if (lexema == ".")
  {
    tipo = TipoDeToken::PONTO;
  }
  else if (lexema == ":")
  {
    tipo = TipoDeToken::DOIS_PONTOS;
  }
  else if (lexema == "?")
  {
    tipo = TipoDeToken::INTERROGACAO;
  }
  else
  {
    tipo = TipoDeToken::DESCONHECIDO;
  }

  tokens.push_back(Token(tipo, lexema));
  lexema = "";
  estado_atual = 0;
}

// Estado q6: Reconhece strings
void Lexer::q6()
{
  while (i < codigo.size() && codigo[i] != '"')
  {
    lexema.push_back(codigo[i]);
    i++;
  }
  if (i >= codigo.size() || codigo[i] != '"')
  {
    throw runtime_error("String nao terminada");
  }
  i++;
  tokens.push_back(Token(TipoDeToken::STRING, lexema));
  lexema = "";
  estado_atual = 0;
}

// Estado q7: Reconhece INCREMENTO (++)
void Lexer::q7()
{
  tokens.push_back(Token(TipoDeToken::INCREMENTO, lexema));
  lexema = "";
  estado_atual = 0;
}

// Estado q8: Reconhece DECREMENTO (--)
void Lexer::q8()
{
  tokens.push_back(Token(TipoDeToken::DECREMENTO, lexema));
  lexema = "";
  estado_atual = 0;
}

// Estado q9: Reconhece operadores lógicos (&&, ||) e negação (!)
void Lexer::q9()
{
  if (i < codigo.size() && ((lexema == "&" && codigo[i] == '&') ||
                            (lexema == "|" && codigo[i] == '|')))
  {
    lexema.push_back(codigo[i]);
    i++;
    tokens.push_back(Token(TipoDeToken::OPERADOR_LOGICO, lexema));
  }
  else if (lexema == "!")
  {
    // Verifica se é != (operador relacional) ou apenas ! (negação lógica)
    if (i < codigo.size() && codigo[i] == '=')
    {
      lexema.push_back(codigo[i]);
      i++;
      tokens.push_back(Token(TipoDeToken::OPERADOR_RELACIONAL, lexema));
    }
    else
    {
      tokens.push_back(Token(TipoDeToken::OPERADOR_LOGICO, lexema));
    }
  }
  else
  {
    throw runtime_error("Operador logico invalido");
  }

  lexema = "";
  estado_atual = 0;
}

// Estado q10: Reconhece a parte decimal de um número real
void Lexer::q10()
{
  while (i < codigo.size() && isDigito(codigo[i]))
  {
    lexema.push_back(codigo[i]);
    i++;
  }
  tokens.push_back(Token(TipoDeToken::NUMERO_REAL, lexema));
  lexema = "";
  estado_atual = 0;
}
