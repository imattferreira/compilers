#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "Token.h"
#include "AST.h"

using namespace std;

// Forward declarations
class ExpressionNode;
class StatementNode;
class BlockNode;
class ProgramNode;
class FunctionNode;
class VariableDeclarationNode;
class ParameterNode;

class Parser
{
public:
  Parser(vector<Token> tokens);
  ProgramNode *analisar();

private:
  vector<Token> tokens;
  int posicao_atual;
  Token token_atual;

  void avancar();
  void erro(string msg);

  // Parsing de expressões (retornam AST)
  ExpressionNode *parseExpression();
  ExpressionNode *E();
  ExpressionNode *ELinha(ExpressionNode *left);
  ExpressionNode *T();
  ExpressionNode *TLinha(ExpressionNode *left);
  ExpressionNode *F();

  // Parsing de statements
  StatementNode *parseStatement();
  StatementNode *parseIf();
  StatementNode *parseWhile();
  StatementNode *parseFor();
  VariableDeclarationNode *parseVariableDeclaration();
  StatementNode *parseAssignment();
  StatementNode *parseReturn();

  // Parsing de blocos e funções
  BlockNode *parseBlock();
  FunctionNode *parseFunction();
  vector<ParameterNode *> parseParamList();

  // Parsing de programa
  ProgramNode *parseProgram();

  // Verificadores de tokens
  bool isTipo(const Token &token);
  bool isPalavraReservada(const string &palavra);
};

#endif // PARSER_H
