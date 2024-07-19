/*
 * Author: Jaydan Cowell
 * Date: 2024-06-02
 */
#pragma once

#include <stdexcept>
#include <string>
#include <vector>

namespace ExpressionParserErrors
{
  class LexerException : public std::runtime_error
  {
  public:
    LexerException(const std::string &message) : std::runtime_error("Lexer:: " + message) {}
  };

  class InvalidTokenException : public LexerException
  {
  public:
    InvalidTokenException(const std::vector<std::string> &tokens) : LexerException(build_error_details(tokens)) {}

  private:
    /*
     * Build Error Details
     *
     * @param tokens A reference to a list of the invalid tokens found
     * @return An error message with the invalid tokens concatenated
     */
    static std::string build_error_details(const std::vector<std::string> &tokens)
    {
      std::string error_message = "Invalid token(s) detected in expression : ";
      for (const std::string &raw_token : tokens)
      {
        error_message += raw_token + ",";
      }
      return error_message;
    }
  };

  class EmptyExpressionException : public LexerException
  {
  public:
    EmptyExpressionException() : LexerException("Empty expression is invalid") {}
  };

  class TokenIndexOutOfRange : public LexerException
  {
  public:
    TokenIndexOutOfRange() : LexerException("Token index is out of range") {}
  };

  class RDParserException : public std::runtime_error
  {
  public:
    RDParserException(const std::string &message) : std::runtime_error("RDParser:: " + message) {}
  };

  class ParenthesesMismatchException : public RDParserException
  {
  public:
    ParenthesesMismatchException() : RDParserException("Mismatched parentheses in expression") {}
  };

  class UnexpectedParenthesesException : public RDParserException
  {
  public:
    UnexpectedParenthesesException() : RDParserException("Unexpected parentheses in expression") {}
  };

  class DivideByZeroException : public RDParserException
  {
  public:
    DivideByZeroException() : RDParserException("Division by zero") {}
  };

  class UnexpectedTokenException : public RDParserException
  {
  public:
    UnexpectedTokenException(const std::string &token) : RDParserException("Unexpected token encountered: " + token) {}
  };

  class UnknownOperatorException : public RDParserException
  {
  public:
    UnknownOperatorException() : RDParserException("Unknown operator") {}
  };

}