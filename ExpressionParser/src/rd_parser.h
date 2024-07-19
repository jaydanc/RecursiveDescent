/*
 * Author: Jaydan Cowell
 * Date: 2024-06-02
 */
#pragma once

#include "lexer.h"

/*
 * RDParser
 *
 * Performs recursive descent on expression tokens to build an abstract syntax tree.
 * Abstract syntax tree provides evaluation methods to obtain the result.
 *
 * Grammar:
 * Expression -> Binary
 * Binary -> Unary | (("*" | "/" | "+" | "-") Unary)*
 * Unary -> ("-") Unary | Primary
 * Primary -> Literal | "(" Expression ")";
 */
class RDParser
{
public:
  RDParser();
  int Parse(const std::string &expr);

private:
  /*
   * Expression Interface
   *
   * Enables a polymorphic representation of the AST for the expression grammar.
   */
  class Expression
  {
  public:
    int virtual Evaluate() const = 0;
    virtual ~Expression() = default;
  };

  /*
   * Binary Node
   *
   * Represents a term or factor within the AST
   */
  class Binary : public Expression
  {
  public:
    Binary(Lexer::TokenOperation op, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);
    int Evaluate() const override;

  private:
    Lexer::TokenOperation operation;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
  };

  /*
   * Unary Node
   *
   * Represents a unary in the AST (currently just negation)
   */
  class Unary : public Expression
  {
  public:
    Unary(std::unique_ptr<Expression> right);
    int Evaluate() const override;

  private:
    std::unique_ptr<Expression> right;
  };

  /*
   * Literal Node
   *
   * Represents an Integer in the AST
   */
  class Literal : public Expression
  {
  public:
    Literal(int value);
    int Evaluate() const override;

  private:
    int value;
  };

  bool tokenMatchAndAdvance(const Lexer::TokenOperation &operation);
  std::unique_ptr<Expression> parseExpression();
  std::unique_ptr<Expression> parseBinary();
  std::unique_ptr<Expression> parseUnary();
  std::unique_ptr<Expression> parsePrimary();

  /* Instance of the lexical analyser class responsible for generating tokens */
  Lexer lexer;

  /* Pointer to the current token the RD algorithm is processing */
  int current_token_idx;
};