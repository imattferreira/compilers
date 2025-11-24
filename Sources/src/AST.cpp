#include "AST.h"
#include <sstream>

using namespace std;

// LiteralNode
string LiteralNode::toString(int indent) const
{
  return string(indent, ' ') + "Literal(" + value + ")";
}

// IdentifierNode
string IdentifierNode::toString(int indent) const
{
  return string(indent, ' ') + "Identifier(" + name + ")";
}

// BinaryOpNode
BinaryOpNode::~BinaryOpNode()
{
  delete left;
  delete right;
}

string BinaryOpNode::toString(int indent) const
{
  stringstream ss;
  ss << string(indent, ' ') << "BinaryOp(" << op << ")" << endl;
  ss << left->toString(indent + 2) << endl;
  ss << right->toString(indent + 2);
  return ss.str();
}

// FunctionCallNode
FunctionCallNode::~FunctionCallNode()
{
  for (auto arg : args)
  {
    delete arg;
  }
}

string FunctionCallNode::toString(int indent) const
{
  stringstream ss;
  ss << string(indent, ' ') << "FunctionCall(" << name << ")" << endl;
  for (auto arg : args)
  {
    ss << arg->toString(indent + 2) << endl;
  }
  return ss.str();
}

// BlockNode
BlockNode::~BlockNode()
{
  for (auto stmt : statements)
  {
    delete stmt;
  }
}

string BlockNode::toString(int indent) const
{
  stringstream ss;
  ss << string(indent, ' ') << "Block {" << endl;
  for (auto stmt : statements)
  {
    ss << stmt->toString(indent + 2) << endl;
  }
  ss << string(indent, ' ') << "}";
  return ss.str();
}

// VariableDeclarationNode
VariableDeclarationNode::~VariableDeclarationNode()
{
  if (initialValue)
  {
    delete initialValue;
  }
}

string VariableDeclarationNode::toString(int indent) const
{
  stringstream ss;
  ss << string(indent, ' ') << "VarDecl(" << type << " " << name;
  if (initialValue)
  {
    ss << " = " << initialValue->toString(0);
  }
  ss << ")";
  return ss.str();
}

// AssignmentNode
AssignmentNode::~AssignmentNode()
{
  delete value;
}

string AssignmentNode::toString(int indent) const
{
  stringstream ss;
  ss << string(indent, ' ') << "Assign(" << name << " = " << endl;
  ss << value->toString(indent + 2) << ")";
  return ss.str();
}

// IfStatementNode
IfStatementNode::~IfStatementNode()
{
  delete condition;
  delete thenBlock;
  if (elseBlock)
  {
    delete elseBlock;
  }
}

string IfStatementNode::toString(int indent) const
{
  stringstream ss;
  ss << string(indent, ' ') << "If" << endl;
  ss << string(indent + 2, ' ') << "Condition:" << endl;
  ss << condition->toString(indent + 4) << endl;
  ss << string(indent + 2, ' ') << "Then:" << endl;
  ss << thenBlock->toString(indent + 4) << endl;
  if (elseBlock)
  {
    ss << string(indent + 2, ' ') << "Else:" << endl;
    ss << elseBlock->toString(indent + 4) << endl;
  }
  return ss.str();
}

// WhileStatementNode
WhileStatementNode::~WhileStatementNode()
{
  delete condition;
  delete body;
}

string WhileStatementNode::toString(int indent) const
{
  stringstream ss;
  ss << string(indent, ' ') << "While" << endl;
  ss << string(indent + 2, ' ') << "Condition:" << endl;
  ss << condition->toString(indent + 4) << endl;
  ss << string(indent + 2, ' ') << "Body:" << endl;
  ss << body->toString(indent + 4);
  return ss.str();
}

// ForStatementNode
ForStatementNode::~ForStatementNode()
{
  if (init)
  {
    delete init;
  }
  if (condition)
  {
    delete condition;
  }
  if (update)
  {
    delete update;
  }
  delete body;
}

string ForStatementNode::toString(int indent) const
{
  stringstream ss;
  ss << string(indent, ' ') << "For" << endl;
  if (init)
  {
    ss << string(indent + 2, ' ') << "Init:" << endl;
    ss << init->toString(indent + 4) << endl;
  }
  if (condition)
  {
    ss << string(indent + 2, ' ') << "Condition:" << endl;
    ss << condition->toString(indent + 4) << endl;
  }
  if (update)
  {
    ss << string(indent + 2, ' ') << "Update:" << endl;
    ss << update->toString(indent + 4) << endl;
  }
  ss << string(indent + 2, ' ') << "Body:" << endl;
  ss << body->toString(indent + 4);
  return ss.str();
}

// ReturnStatementNode
ReturnStatementNode::~ReturnStatementNode()
{
  if (value)
  {
    delete value;
  }
}

string ReturnStatementNode::toString(int indent) const
{
  stringstream ss;
  ss << string(indent, ' ') << "Return";
  if (value)
  {
    ss << endl
       << value->toString(indent + 2);
  }
  return ss.str();
}

// ExpressionStatementNode
ExpressionStatementNode::~ExpressionStatementNode()
{
  delete expr;
}

string ExpressionStatementNode::toString(int indent) const
{
  return expr->toString(indent);
}

// FunctionNode
FunctionNode::~FunctionNode()
{
  for (auto param : params)
  {
    delete param;
  }
  delete body;
}

string FunctionNode::toString(int indent) const
{
  stringstream ss;
  ss << string(indent, ' ') << "Function(" << returnType << " " << name << "(";
  for (size_t i = 0; i < params.size(); i++)
  {
    ss << params[i]->getType() << " " << params[i]->getName();
    if (i < params.size() - 1)
    {
      ss << ", ";
    }
  }
  ss << "))" << endl;
  ss << body->toString(indent + 2);
  return ss.str();
}

// ProgramNode
ProgramNode::~ProgramNode()
{
  for (auto func : functions)
  {
    delete func;
  }
}

string ProgramNode::toString(int indent) const
{
  stringstream ss;
  ss << string(indent, ' ') << "Program {" << endl;
  for (auto func : functions)
  {
    ss << func->toString(indent + 2) << endl;
  }
  ss << string(indent, ' ') << "}";
  return ss.str();
}
