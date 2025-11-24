/*
E  -> T E'
E' -> "+" T E' | e
T  -> IDENTIFICADOR | NUMERO_INTEIRO
*/
#include "Parser.h"
#include "AST.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
using namespace std;

Parser::Parser(vector<Token> tokens)
    : tokens(tokens),
      posicao_atual(0),
      token_atual(tokens.empty() ? Token(TipoDeToken::DESCONHECIDO, "") : tokens[0])
{
}

void Parser::avancar()
{
  posicao_atual++;
  if (posicao_atual < tokens.size())
  {
    token_atual = tokens[posicao_atual];
  }
  else
  {
    token_atual = Token(TipoDeToken::DESCONHECIDO, "");
  }
}

void Parser::erro(string msg)
{
  string erro_completo = "Erro sintatico: " + msg + " proximo a '" + token_atual.getLexema() + "'";
  throw runtime_error(erro_completo);
}

bool Parser::isTipo(const Token &token)
{
  if (token.getTipo() == TipoDeToken::PALAVRA_RESERVADA)
  {
    return token.getLexema() == "int" ||
           token.getLexema() == "double" ||
           token.getLexema() == "string";
  }
  return false;
}

bool Parser::isPalavraReservada(const string &palavra)
{
  return token_atual.getTipo() == TipoDeToken::PALAVRA_RESERVADA &&
         token_atual.getLexema() == palavra;
}

ProgramNode *Parser::analisar()
{
  return parseProgram();
}

ProgramNode *Parser::parseProgram()
{
  vector<FunctionNode *> functions;

  while (token_atual.getTipo() != TipoDeToken::DESCONHECIDO)
  {
    // Verifica se é uma função (tipo seguido de identificador e parênteses)
    if (isTipo(token_atual))
    {
      int saved_pos = posicao_atual;
      Token saved_token = token_atual;

      avancar(); // avança do tipo
      bool isFunction = false;
      if (token_atual.getTipo() == TipoDeToken::IDENTIFICADOR ||
          token_atual.getTipo() == TipoDeToken::PALAVRA_RESERVADA)
      {
        avancar(); // avança do identificador
        if (token_atual.getTipo() == TipoDeToken::ABRE_PARENTESES)
        {
          isFunction = true;
        }
      }

      // Restaura posição
      posicao_atual = saved_pos;
      token_atual = saved_token;

      if (isFunction)
      {
        FunctionNode *func = parseFunction();
        functions.push_back(func);
      }
      else
      {
        // É uma declaração de variável no nível do programa
        // Cria uma função wrapper
        vector<StatementNode *> statements;
        while (token_atual.getTipo() != TipoDeToken::DESCONHECIDO)
        {
          if (isTipo(token_atual))
          {
            statements.push_back(parseVariableDeclaration());
          }
          else
          {
            break;
          }
        }
        BlockNode *body = new BlockNode(statements);
        FunctionNode *wrapper = new FunctionNode("void", "__global__", vector<ParameterNode *>(), body);
        functions.push_back(wrapper);
      }
    }
    else
    {
      break;
    }
  }

  return new ProgramNode(functions);
}

FunctionNode *Parser::parseFunction()
{
  // TIPO IDENTIFICADOR "(" ParamList? ")" Block
  if (!isTipo(token_atual))
  {
    erro("Esperado tipo de retorno da funcao");
  }
  string returnType = token_atual.getLexema();
  avancar();

  if (token_atual.getTipo() != TipoDeToken::IDENTIFICADOR &&
      token_atual.getTipo() != TipoDeToken::PALAVRA_RESERVADA)
  {
    erro("Esperado identificador de funcao");
  }
  string name = token_atual.getLexema();
  avancar();

  if (token_atual.getTipo() != TipoDeToken::ABRE_PARENTESES)
  {
    erro("Esperado '(' apos nome da funcao");
  }
  avancar();

  vector<ParameterNode *> params;
  if (isTipo(token_atual))
  {
    params = parseParamList();
  }

  if (token_atual.getTipo() != TipoDeToken::FECHA_PARENTESES)
  {
    erro("Esperado ')' apos parametros");
  }
  avancar();

  BlockNode *body = parseBlock();

  return new FunctionNode(returnType, name, params, body);
}

vector<ParameterNode *> Parser::parseParamList()
{
  // TIPO IDENTIFICADOR ("," TIPO IDENTIFICADOR)*
  vector<ParameterNode *> params;

  do
  {
    if (!isTipo(token_atual))
    {
      erro("Esperado tipo do parametro");
    }
    string type = token_atual.getLexema();
    avancar();

    if (token_atual.getTipo() != TipoDeToken::IDENTIFICADOR)
    {
      erro("Esperado identificador do parametro");
    }
    string name = token_atual.getLexema();
    avancar();

    params.push_back(new ParameterNode(type, name));

    if (token_atual.getTipo() == TipoDeToken::VIRGULA)
    {
      avancar();
    }
    else
    {
      break;
    }
  } while (true);

  return params;
}

BlockNode *Parser::parseBlock()
{
  // "{" Statement* "}"
  if (token_atual.getTipo() != TipoDeToken::ABRE_CHAVES)
  {
    erro("Esperado '{' para inicio do bloco");
  }
  avancar();

  vector<StatementNode *> statements;
  while (token_atual.getTipo() != TipoDeToken::FECHA_CHAVES &&
         token_atual.getTipo() != TipoDeToken::DESCONHECIDO)
  {
    StatementNode *stmt = parseStatement();
    statements.push_back(stmt);
  }

  if (token_atual.getTipo() != TipoDeToken::FECHA_CHAVES)
  {
    erro("Esperado '}' para fim do bloco");
  }
  avancar();

  return new BlockNode(statements);
}

StatementNode *Parser::parseStatement()
{
  // Statement -> VariableDeclaration | Assignment | IfStatement | WhileStatement | ForStatement | ReturnStatement | ExpressionStatement

  if (isTipo(token_atual))
  {
    return parseVariableDeclaration();
  }
  else if (isPalavraReservada("if"))
  {
    return parseIf();
  }
  else if (isPalavraReservada("while"))
  {
    return parseWhile();
  }
  else if (isPalavraReservada("for"))
  {
    return parseFor();
  }
  else if (isPalavraReservada("return"))
  {
    return parseReturn();
  }
  else if (token_atual.getTipo() == TipoDeToken::IDENTIFICADOR)
  {
    // Pode ser assignment, array assignment ou expression statement
    // Fazemos lookahead para determinar
    Token peek = token_atual;
    int saved_pos = posicao_atual;

    avancar();
    // Verifica se é acesso a array seguido de atribuição
    if (token_atual.getTipo() == TipoDeToken::ABRE_COLCHETES)
    {
      // É acesso a array - pode ser atribuição a array
      avancar();         // consome [
      parseExpression(); // consome índice
      if (token_atual.getTipo() != TipoDeToken::FECHA_COLCHETES)
      {
        erro("Esperado ']'");
      }
      avancar(); // consome ]
      if (token_atual.getTipo() == TipoDeToken::OPERADOR_ATRIBUICAO)
      {
        // É atribuição a array
        posicao_atual = saved_pos;
        token_atual = peek;
        return parseAssignment();
      }
      else
      {
        // É expression statement com acesso a array
        posicao_atual = saved_pos;
        token_atual = peek;
        ExpressionNode *expr = parseExpression();
        if (token_atual.getTipo() != TipoDeToken::PONTO_E_VIRGULA)
        {
          erro("Esperado ';' apos expressao");
        }
        avancar();
        return new ExpressionStatementNode(expr);
      }
    }
    else if (token_atual.getTipo() == TipoDeToken::OPERADOR_ATRIBUICAO)
    {
      // É assignment
      posicao_atual = saved_pos;
      token_atual = peek;
      return parseAssignment();
    }
    else
    {
      // É expression statement (pode ser identifier, function call, etc.)
      posicao_atual = saved_pos;
      token_atual = peek;
      ExpressionNode *expr = parseExpression();
      if (token_atual.getTipo() != TipoDeToken::PONTO_E_VIRGULA)
      {
        erro("Esperado ';' apos expressao");
      }
      avancar();
      return new ExpressionStatementNode(expr);
    }
  }
  else
  {
    // Expression statement
    ExpressionNode *expr = parseExpression();
    if (token_atual.getTipo() != TipoDeToken::PONTO_E_VIRGULA)
    {
      erro("Esperado ';' apos expressao");
    }
    avancar();
    return new ExpressionStatementNode(expr);
  }
}

VariableDeclarationNode *Parser::parseVariableDeclaration()
{
  // TIPO IDENTIFICADOR ("=" Expression)? ("," IDENTIFICADOR ("=" Expression)?)* ";"
  if (!isTipo(token_atual))
  {
    erro("Esperado tipo para declaracao de variavel");
  }
  string type = token_atual.getLexema();
  avancar();

  if (token_atual.getTipo() != TipoDeToken::IDENTIFICADOR)
  {
    erro("Esperado identificador para declaracao de variavel");
  }
  string name = token_atual.getLexema();
  avancar();

  ExpressionNode *initialValue = nullptr;
  if (token_atual.getTipo() == TipoDeToken::OPERADOR_ATRIBUICAO)
  {
    avancar();
    initialValue = parseExpression();
  }

  // Processa declarações adicionais separadas por vírgula
  // Por enquanto, apenas consome elas (não cria múltiplos statements)
  // TODO: Implementar suporte completo para múltiplas declarações retornando vector
  while (token_atual.getTipo() == TipoDeToken::VIRGULA)
  {
    avancar();
    if (token_atual.getTipo() != TipoDeToken::IDENTIFICADOR)
    {
      erro("Esperado identificador apos virgula");
    }
    avancar(); // consome o identificador
    if (token_atual.getTipo() == TipoDeToken::OPERADOR_ATRIBUICAO)
    {
      avancar();
      parseExpression(); // consome a expressão mas não armazena
    }
  }

  if (token_atual.getTipo() != TipoDeToken::PONTO_E_VIRGULA)
  {
    erro("Esperado ';' apos declaracao de variavel");
  }
  avancar();

  return new VariableDeclarationNode(type, name, initialValue);
}

StatementNode *Parser::parseAssignment()
{
  // IDENTIFICADOR ("[" Expression "]")? "=" Expression ";"
  if (token_atual.getTipo() != TipoDeToken::IDENTIFICADOR)
  {
    erro("Esperado identificador para atribuicao");
  }
  string name = token_atual.getLexema();
  avancar();

  if (token_atual.getTipo() == TipoDeToken::ABRE_COLCHETES)
  {
    avancar();
    if (token_atual.getTipo() != TipoDeToken::FECHA_COLCHETES)
    {
      erro("Esperado ']' apos indice do array");
    }
    avancar();
  }

  if (token_atual.getTipo() != TipoDeToken::OPERADOR_ATRIBUICAO)
  {
    erro("Esperado '=' para atribuicao");
  }
  avancar();

  ExpressionNode *value = parseExpression();

  if (token_atual.getTipo() != TipoDeToken::PONTO_E_VIRGULA)
  {
    erro("Esperado ';' apos atribuicao");
  }
  avancar();

  // Se for atribuição a array, usa ArrayAccessNode como nome
  // Por enquanto, vamos usar o nome simples (AssignmentNode não suporta arrays ainda)
  // TODO: Modificar AssignmentNode para suportar arrays
  return new AssignmentNode(name, value);
}

StatementNode *Parser::parseIf()
{
  // "if" "(" Expression ")" Block ("else" Block)?
  if (!isPalavraReservada("if"))
  {
    erro("Esperado 'if'");
  }
  avancar();

  if (token_atual.getTipo() != TipoDeToken::ABRE_PARENTESES)
  {
    erro("Esperado '(' apos 'if'");
  }
  avancar();

  ExpressionNode *condition = parseExpression();

  if (token_atual.getTipo() != TipoDeToken::FECHA_PARENTESES)
  {
    erro("Esperado ')' apos condicao do if");
  }
  avancar();

  BlockNode *thenBlock = nullptr;
  // Se não houver '{', cria um bloco com um único statement
  if (token_atual.getTipo() != TipoDeToken::ABRE_CHAVES)
  {
    StatementNode *stmt = parseStatement();
    vector<StatementNode *> statements;
    statements.push_back(stmt);
    thenBlock = new BlockNode(statements);
  }
  else
  {
    thenBlock = parseBlock();
  }

  BlockNode *elseBlock = nullptr;
  if (isPalavraReservada("else"))
  {
    avancar();
    if (token_atual.getTipo() != TipoDeToken::ABRE_CHAVES)
    {
      StatementNode *stmt = parseStatement();
      vector<StatementNode *> statements;
      statements.push_back(stmt);
      elseBlock = new BlockNode(statements);
    }
    else
    {
      elseBlock = parseBlock();
    }
  }

  return new IfStatementNode(condition, thenBlock, elseBlock);
}

StatementNode *Parser::parseWhile()
{
  // "while" "(" Expression ")" Block
  if (!isPalavraReservada("while"))
  {
    erro("Esperado 'while'");
  }
  avancar();

  if (token_atual.getTipo() != TipoDeToken::ABRE_PARENTESES)
  {
    erro("Esperado '(' apos 'while'");
  }
  avancar();

  ExpressionNode *condition = parseExpression();

  if (token_atual.getTipo() != TipoDeToken::FECHA_PARENTESES)
  {
    erro("Esperado ')' apos condicao do while");
  }
  avancar();

  BlockNode *body = nullptr;
  // Se não houver '{', cria um bloco com um único statement
  if (token_atual.getTipo() != TipoDeToken::ABRE_CHAVES)
  {
    StatementNode *stmt = parseStatement();
    vector<StatementNode *> statements;
    statements.push_back(stmt);
    body = new BlockNode(statements);
  }
  else
  {
    body = parseBlock();
  }

  return new WhileStatementNode(condition, body);
}

StatementNode *Parser::parseFor()
{
  // "for" "(" Expression? ";" Expression? ";" Expression? ")" Block
  if (!isPalavraReservada("for"))
  {
    erro("Esperado 'for'");
  }
  avancar();

  if (token_atual.getTipo() != TipoDeToken::ABRE_PARENTESES)
  {
    erro("Esperado '(' apos 'for'");
  }
  avancar();

  // Init (pode ser declaração de variável, atribuição ou expressão)
  StatementNode *init = nullptr;
  if (token_atual.getTipo() != TipoDeToken::PONTO_E_VIRGULA)
  {
    if (isTipo(token_atual))
    {
      init = parseVariableDeclaration();
      // parseVariableDeclaration já consome o ';'
    }
    else if (token_atual.getTipo() == TipoDeToken::IDENTIFICADOR)
    {
      // Pode ser atribuição ou expressão
      int saved_pos = posicao_atual;
      Token saved_token = token_atual;
      avancar();
      if (token_atual.getTipo() == TipoDeToken::OPERADOR_ATRIBUICAO)
      {
        // É atribuição
        posicao_atual = saved_pos;
        token_atual = saved_token;
        init = parseAssignment();
        // parseAssignment já consome o ';'
      }
      else
      {
        // É expressão
        posicao_atual = saved_pos;
        token_atual = saved_token;
        init = new ExpressionStatementNode(parseExpression());
        if (token_atual.getTipo() != TipoDeToken::PONTO_E_VIRGULA)
        {
          erro("Esperado ';' apos expressao de inicializacao do for");
        }
        avancar();
      }
    }
    else
    {
      init = new ExpressionStatementNode(parseExpression());
      if (token_atual.getTipo() != TipoDeToken::PONTO_E_VIRGULA)
      {
        erro("Esperado ';' apos expressao de inicializacao do for");
      }
      avancar();
    }
  }
  else
  {
    avancar(); // Consome o ';'
  }

  // Condition
  ExpressionNode *condition = nullptr;
  if (token_atual.getTipo() != TipoDeToken::PONTO_E_VIRGULA)
  {
    condition = parseExpression();
  }
  if (token_atual.getTipo() != TipoDeToken::PONTO_E_VIRGULA)
  {
    erro("Esperado ';' apos condicao do for");
  }
  avancar();

  // Update (pode ser atribuição ou expressão)
  // No contexto do for, atribuições são permitidas como expressões
  ExpressionNode *update = nullptr;
  if (token_atual.getTipo() != TipoDeToken::FECHA_PARENTESES)
  {
    // Parseia como expressão, mas permite atribuições
    // Fazemos lookahead para detectar atribuições
    if (token_atual.getTipo() == TipoDeToken::IDENTIFICADOR)
    {
      int saved_pos = posicao_atual;
      Token saved_token = token_atual;
      avancar();
      if (token_atual.getTipo() == TipoDeToken::OPERADOR_ATRIBUICAO)
      {
        // É atribuição - parseamos como expressão binária especial
        // Na verdade, vamos criar um BinaryOpNode com operador "="
        posicao_atual = saved_pos;
        token_atual = saved_token;
        string name = token_atual.getLexema();
        avancar(); // consome identificador
        avancar(); // consome "="
        ExpressionNode *value = parseExpression();
        // Criamos um BinaryOpNode especial para atribuição
        // (isso não é semanticamente correto, mas funciona para a AST)
        update = new BinaryOpNode(new IdentifierNode(name), "=", value);
      }
      else
      {
        posicao_atual = saved_pos;
        token_atual = saved_token;
        update = parseExpression();
      }
    }
    else
    {
      update = parseExpression();
    }
  }
  if (token_atual.getTipo() != TipoDeToken::FECHA_PARENTESES)
  {
    erro("Esperado ')' apos update do for");
  }
  avancar();

  BlockNode *body = parseBlock();

  return new ForStatementNode(init, condition, update, body);
}

StatementNode *Parser::parseReturn()
{
  // "return" Expression? ";"
  if (!isPalavraReservada("return"))
  {
    erro("Esperado 'return'");
  }
  avancar();

  ExpressionNode *value = nullptr;
  if (token_atual.getTipo() != TipoDeToken::PONTO_E_VIRGULA)
  {
    value = parseExpression();
  }

  if (token_atual.getTipo() != TipoDeToken::PONTO_E_VIRGULA)
  {
    erro("Esperado ';' apos return");
  }
  avancar();

  return new ReturnStatementNode(value);
}

ExpressionNode *Parser::parseExpression()
{
  // Expression -> E (relacional E)* | E (logico E)*
  ExpressionNode *left = E();

  // Processar operadores relacionais e lógicos
  while (token_atual.getTipo() == TipoDeToken::OPERADOR_RELACIONAL ||
         token_atual.getTipo() == TipoDeToken::OPERADOR_LOGICO ||
         (token_atual.getTipo() == TipoDeToken::OPERADOR_ATRIBUICAO && token_atual.getLexema() == "==") ||
         (token_atual.getTipo() == TipoDeToken::OPERADOR_RELACIONAL &&
          (token_atual.getLexema() == "==" || token_atual.getLexema() == "!=")))
  {
    string op = token_atual.getLexema();
    avancar();
    ExpressionNode *right = E();
    left = new BinaryOpNode(left, op, right);
  }

  return left;
}

// E -> T E'
ExpressionNode *Parser::E()
{
  ExpressionNode *left = T();
  return ELinha(left);
}

// E' -> ("+" | "-") T E' | ε
ExpressionNode *Parser::ELinha(ExpressionNode *left)
{
  if (token_atual.getTipo() == TipoDeToken::OPERADOR_ARITMETICO &&
      (token_atual.getLexema() == "+" || token_atual.getLexema() == "-"))
  {
    string op = token_atual.getLexema();
    avancar();
    ExpressionNode *right = T();
    ExpressionNode *result = new BinaryOpNode(left, op, right);
    return ELinha(result);
  }
  return left;
}

// T -> F T'
ExpressionNode *Parser::T()
{
  ExpressionNode *left = F();
  return TLinha(left);
}

// T' -> ("*" | "/") F T' | ε
ExpressionNode *Parser::TLinha(ExpressionNode *left)
{
  if (token_atual.getTipo() == TipoDeToken::OPERADOR_ARITMETICO &&
      (token_atual.getLexema() == "*" || token_atual.getLexema() == "/"))
  {
    string op = token_atual.getLexema();
    avancar();
    ExpressionNode *right = F();
    ExpressionNode *result = new BinaryOpNode(left, op, right);
    return TLinha(result);
  }
  return left;
}

// F -> IDENTIFICADOR | NUMERO_INTEIRO | NUMERO_REAL | STRING | "(" Expression ")" | FunctionCall | UnaryOp
ExpressionNode *Parser::F()
{
  // Operadores unários
  if (token_atual.getTipo() == TipoDeToken::OPERADOR_LOGICO && token_atual.getLexema() == "!")
  {
    string op = token_atual.getLexema();
    avancar();
    ExpressionNode *operand = F();
    return new UnaryOpNode(op, operand);
  }

  if (token_atual.getTipo() == TipoDeToken::ABRE_PARENTESES)
  {
    avancar();
    ExpressionNode *expr = parseExpression();
    if (token_atual.getTipo() != TipoDeToken::FECHA_PARENTESES)
    {
      erro("Esperado ')'");
    }
    avancar();
    return expr;
  }
  else if (token_atual.getTipo() == TipoDeToken::IDENTIFICADOR)
  {
    string name = token_atual.getLexema();
    avancar();

    // Verifica se é acesso a array
    if (token_atual.getTipo() == TipoDeToken::ABRE_COLCHETES)
    {
      avancar();
      ExpressionNode *index = parseExpression();
      if (token_atual.getTipo() != TipoDeToken::FECHA_COLCHETES)
      {
        erro("Esperado ']' apos indice do array");
      }
      avancar();
      return new ArrayAccessNode(name, index);
    }
    // Verifica se é chamada de função
    else if (token_atual.getTipo() == TipoDeToken::ABRE_PARENTESES)
    {
      avancar();
      vector<ExpressionNode *> args;

      if (token_atual.getTipo() != TipoDeToken::FECHA_PARENTESES)
      {
        // Argumentos
        args.push_back(parseExpression());
        while (token_atual.getTipo() == TipoDeToken::VIRGULA)
        {
          avancar();
          args.push_back(parseExpression());
        }
      }

      if (token_atual.getTipo() != TipoDeToken::FECHA_PARENTESES)
      {
        erro("Esperado ')' apos argumentos da funcao");
      }
      avancar();

      return new FunctionCallNode(name, args);
    }
    else
    {
      return new IdentifierNode(name);
    }
  }
  else if (token_atual.getTipo() == TipoDeToken::NUMERO_INTEIRO ||
           token_atual.getTipo() == TipoDeToken::NUMERO_REAL)
  {
    string value = token_atual.getLexema();
    avancar();
    return new LiteralNode(value);
  }
  else if (token_atual.getTipo() == TipoDeToken::STRING)
  {
    string value = token_atual.getLexema();
    avancar();
    return new LiteralNode(value);
  }
  else
  {
    erro("Token inesperado em F");
    return nullptr; // Nunca alcançado
  }
}
