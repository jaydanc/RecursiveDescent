/*
 * Author: Jaydan Cowell
 * Date: 2024-06-02
 */
#include "rd_parser.h"
#include "errors.h"

/*
 * RDParser Constructor
 *
 * Initialises the current token pointer to zero
 */
RDParser::RDParser() : current_token_idx(0) {}

/*
 * Parse
 *
 * @param expr Reference to the expression to parse
 * @return Result of the evaluated expression
 */
int RDParser::Parse(const std::string &expr)
{
  current_token_idx = 0;

  lexer.clearTokens();
  lexer.Tokenise(expr);

  /* Build the abstract syntax tree (AST) using recursive descent */
  std::unique_ptr<Expression> expression = parseExpression();

  if (current_token_idx < lexer.getTokenCount())
  {
    /* If we have escaped recursion and still have tokens left, it could only
       be that the parentheses are formatted incorrectly; incorrect parentheses
       will not make it past the parseBinary state. */
    throw ExpressionParserErrors::UnexpectedParenthesesException();
  }

  /* Evaluate the AST */
  return expression->Evaluate();
}

/*
 * Token Match and Advance
 *
 * Will attempt to match the current token with an operation. If the current token matches,
 * the token pointer is updated the next token available.
 *
 * @param operation Reference to token operation to match with
 * @return True if a match is found, otherwise false
 */
bool RDParser::tokenMatchAndAdvance(const Lexer::TokenOperation &operation)
{
  if (current_token_idx < lexer.getTokenCount() && (lexer.getToken(current_token_idx)).operation == operation)
  {
    current_token_idx++;
    return true;
  }

  return false;
}

/*
 * Parse Expression
 *
 * Entry point into the recursive parse
 * Fulfils 'Expression -> Binary' portion of the Context-Free-Grammar (CFG) specification.
 *
 * @return Safe pointer to the next tree node
 */
std::unique_ptr<RDParser::Expression> RDParser::parseExpression()
{ 
  return parseBinary(); 
}

/*
 * Parse Binary
 *
 * Builds a binary node representing addition, subtraction, multiplication and division
 * 'Binary -> Unary | (("*" | "/" | "+" | "-") Unary)*' portion of the CFG
 *
 * @return Safe pointer to the next tree node
 */
std::unique_ptr<RDParser::Expression> RDParser::parseBinary()
{
  std::unique_ptr<Expression> left = parseUnary();

  while (tokenMatchAndAdvance(Lexer::TokenOperation::Addition) ||
         tokenMatchAndAdvance(Lexer::TokenOperation::Subtraction) ||
         tokenMatchAndAdvance(Lexer::TokenOperation::Multiplication) ||
         tokenMatchAndAdvance(Lexer::TokenOperation::Division))
  {
    const Lexer::Token &previous_token = lexer.getToken(current_token_idx - 1);
    left = std::make_unique<Binary>(previous_token.operation, std::move(left), parseUnary());
  }

  return left;
}

/*
 * Parse Unary
 *
 * Builds a unary node representing negation
 * 'Unary -> ("-") Unary | Primary' portion of the CFG
 *
 * @return Safe pointer to the next tree node
 */
std::unique_ptr<RDParser::Expression> RDParser::parseUnary()
{
  if (tokenMatchAndAdvance(Lexer::TokenOperation::Subtraction))
  {
    return std::make_unique<Unary>(parseUnary());
  }

  return parsePrimary();
}

/*
 * Parse Primary
 *
 * Build the literal nodes or enters the nesting of parentheses
 * 'Primary -> Literal | "(" Expression ")"' portion of the CFG
 *
 * @return Safe pointer to the next tree node
 */
std::unique_ptr<RDParser::Expression> RDParser::parsePrimary()
{
  if (tokenMatchAndAdvance(Lexer::TokenOperation::Literal))
  {
    int value = lexer.getToken(current_token_idx - 1).value;
    return std::make_unique<Literal>(value);
  }

  if (tokenMatchAndAdvance(Lexer::TokenOperation::L_Brace))
  {
    std::unique_ptr<Expression> expression = parseExpression();

    if (!tokenMatchAndAdvance(Lexer::TokenOperation::R_Brace))
    {
      /* If the next token is not a closing brace, this expression is invalid */
      throw ExpressionParserErrors::ParenthesesMismatchException();
    }

    return expression;
    ;
  }

  /* This branch is only reached in the event of an error; the token has not been expected and the order it appears in
   * is incorrect */

  /* We want to inform the user of which token was unexpected. The token pointer will have been incremented regardless
   * by this point (see parseBinary), If this is the last token in the expression, an overflow would occur. We
   * clamp this to avoid that scenario.
   */
  int bad_token_idx = current_token_idx;
  int token_max_index = lexer.getTokenCount() - 1;
  if (bad_token_idx > token_max_index)
  {
    bad_token_idx = token_max_index;
  }

  const Lexer::Token &bad_token = lexer.getToken(bad_token_idx);
  throw ExpressionParserErrors::UnexpectedTokenException(bad_token.raw);
}

/*
 * Binary Node Constructor
 *
 * Initialises the binary node with its operation, left, and right hand side expression.
 *
 * @param op Operation of this term/factor
 * @param left Pointer to expression on the left hand side
 * @param right Pointer to expression on the right hand side
 **/
RDParser::Binary::Binary(Lexer::TokenOperation op, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
    : operation(op), left(std::move(left)), right(std::move(right))
{
}

/*
 * Binary Evaluate
 *
 * Performs an arithmetic evaluation on the binary node dependent upon its operation
 *
 * @return Result of evaluation = left op right
 */
int RDParser::Binary::Evaluate() const
{
  switch (operation)
  {
  case Lexer::TokenOperation::Addition: return left->Evaluate() + right->Evaluate();

  case Lexer::TokenOperation::Subtraction: return left->Evaluate() - right->Evaluate();

  case Lexer::TokenOperation::Multiplication: return left->Evaluate() * right->Evaluate();

  case Lexer::TokenOperation::Division:
  {
    int right_value = right->Evaluate();
    if (right_value == 0)
    {
      /* Undefined result encountered */
      throw ExpressionParserErrors::DivideByZeroException();
    }

    return left->Evaluate() / right_value;
  }

  default: throw ExpressionParserErrors::UnknownOperatorException();
  }
}

/*
 * Unary Node Constructor
 *
 * Initialises the unary node with a right hand side expression
 *
 * @param right Pointer to expression on the right hand side
 **/
RDParser::Unary::Unary(std::unique_ptr<Expression> right) : right(std::move(right)) {}

/*
 * Unary Evaluate
 *
 * @return The negated value of its right hand side expression.
 */
int RDParser::Unary::Evaluate() const 
{ 
  return -right->Evaluate(); 
}

/*
 * Literal Node Constructor
 *
 * Initialises the literal node with an integer value
 *
 * @param value Integer value to assign to this node
 **/
RDParser::Literal::Literal(int value) : value(value) {}

/*
 * Unary Evaluate
 *
 * @return The integer value of the literal
 */
int RDParser::Literal::Evaluate() const 
{ 
  return value; 
}