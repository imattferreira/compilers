#ifndef AST_H
#define AST_H

#include <vector>
#include <string>
#include "Token.h"

using namespace std;

// Classe base abstrata para todos os nós da AST
class ASTNode
{
public:
  virtual ~ASTNode() = default;
  virtual string toString(int indent = 0) const = 0;
};

// Classe base para expressões
class ExpressionNode : public ASTNode
{
public:
  virtual ~ExpressionNode() = default;
};

// Classe base para statements
class StatementNode : public ASTNode
{
public:
  virtual ~StatementNode() = default;
};

// Nó para literais (números inteiros)
class LiteralNode : public ExpressionNode
{
public:
  LiteralNode(const string &value) : value(value) {}
  string toString(int indent = 0) const override;

private:
  string value;
};

// Nó para identificadores
class IdentifierNode : public ExpressionNode
{
public:
  IdentifierNode(const string &name) : name(name) {}
  string toString(int indent = 0) const override;
  string getName() const { return name; }

private:
  string name;
};

// Nó para acesso a arrays
class ArrayAccessNode : public ExpressionNode
{
public:
  ArrayAccessNode(const string &name, ExpressionNode *index)
      : name(name), index(index) {}
  ~ArrayAccessNode();
  string toString(int indent = 0) const override;

private:
  string name;
  ExpressionNode *index;
};

// Nó para operações unárias
class UnaryOpNode : public ExpressionNode
{
public:
  UnaryOpNode(const string &op, ExpressionNode *operand)
      : op(op), operand(operand) {}
  ~UnaryOpNode();
  string toString(int indent = 0) const override;

private:
  string op;
  ExpressionNode *operand;
};

// Nó para operações binárias
class BinaryOpNode : public ExpressionNode
{
public:
  BinaryOpNode(ExpressionNode *left, const string &op, ExpressionNode *right)
      : left(left), op(op), right(right) {}
  ~BinaryOpNode();
  string toString(int indent = 0) const override;

private:
  ExpressionNode *left;
  string op;
  ExpressionNode *right;
};

// Nó para chamadas de função
class FunctionCallNode : public ExpressionNode
{
public:
  FunctionCallNode(const string &name, vector<ExpressionNode *> args)
      : name(name), args(args) {}
  ~FunctionCallNode();
  string toString(int indent = 0) const override;

private:
  string name;
  vector<ExpressionNode *> args;
};

// Nó para blocos de código
class BlockNode : public StatementNode
{
public:
  BlockNode(vector<StatementNode *> statements) : statements(statements) {}
  ~BlockNode();
  string toString(int indent = 0) const override;
  vector<StatementNode *> getStatements() const { return statements; }

private:
  vector<StatementNode *> statements;
};

// Nó para declarações de variáveis
class VariableDeclarationNode : public StatementNode
{
public:
  VariableDeclarationNode(const string &type, const string &name, ExpressionNode *initialValue = nullptr)
      : type(type), name(name), initialValue(initialValue) {}
  ~VariableDeclarationNode();
  string toString(int indent = 0) const override;

private:
  string type;
  string name;
  ExpressionNode *initialValue;
};

// Nó para atribuições
class AssignmentNode : public StatementNode
{
public:
  AssignmentNode(const string &name, ExpressionNode *value)
      : name(name), value(value) {}
  ~AssignmentNode();
  string toString(int indent = 0) const override;

private:
  string name;
  ExpressionNode *value;
};

// Nó para if statement
class IfStatementNode : public StatementNode
{
public:
  IfStatementNode(ExpressionNode *condition, BlockNode *thenBlock, BlockNode *elseBlock = nullptr)
      : condition(condition), thenBlock(thenBlock), elseBlock(elseBlock) {}
  ~IfStatementNode();
  string toString(int indent = 0) const override;

private:
  ExpressionNode *condition;
  BlockNode *thenBlock;
  BlockNode *elseBlock;
};

// Nó para while statement
class WhileStatementNode : public StatementNode
{
public:
  WhileStatementNode(ExpressionNode *condition, BlockNode *body)
      : condition(condition), body(body) {}
  ~WhileStatementNode();
  string toString(int indent = 0) const override;

private:
  ExpressionNode *condition;
  BlockNode *body;
};

// Nó para for statement
class ForStatementNode : public StatementNode
{
public:
  ForStatementNode(StatementNode *init, ExpressionNode *condition, ExpressionNode *update, BlockNode *body)
      : init(init), condition(condition), update(update), body(body) {}
  ~ForStatementNode();
  string toString(int indent = 0) const override;

private:
  StatementNode *init;
  ExpressionNode *condition;
  ExpressionNode *update;
  BlockNode *body;
};

// Nó para return statement
class ReturnStatementNode : public StatementNode
{
public:
  ReturnStatementNode(ExpressionNode *value = nullptr) : value(value) {}
  ~ReturnStatementNode();
  string toString(int indent = 0) const override;

private:
  ExpressionNode *value;
};

// Nó para expression statement (expressão seguida de ;)
class ExpressionStatementNode : public StatementNode
{
public:
  ExpressionStatementNode(ExpressionNode *expr) : expr(expr) {}
  ~ExpressionStatementNode();
  string toString(int indent = 0) const override;

private:
  ExpressionNode *expr;
};

// Nó para parâmetros de função
class ParameterNode
{
public:
  ParameterNode(const string &type, const string &name) : type(type), name(name) {}
  string getType() const { return type; }
  string getName() const { return name; }

private:
  string type;
  string name;
};

// Nó para definições de funções
class FunctionNode : public ASTNode
{
public:
  FunctionNode(const string &returnType, const string &name, vector<ParameterNode *> params, BlockNode *body)
      : returnType(returnType), name(name), params(params), body(body) {}
  ~FunctionNode();
  string toString(int indent = 0) const override;

private:
  string returnType;
  string name;
  vector<ParameterNode *> params;
  BlockNode *body;
};

// Nó raiz do programa (contém múltiplas funções)
class ProgramNode : public ASTNode
{
public:
  ProgramNode(vector<FunctionNode *> functions) : functions(functions) {}
  ~ProgramNode();
  string toString(int indent = 0) const override;

private:
  vector<FunctionNode *> functions;
};

#endif // AST_H
