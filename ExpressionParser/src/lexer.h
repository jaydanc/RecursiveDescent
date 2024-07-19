/* 
 * Author: Jaydan Cowell
 * Date: 2024-06-02
 */
#pragma once

#include <regex>
#include <string>
#include <vector>

#define REGEX_TOKEN_PATTERN "((\\d+|[-+*\/()]))"
#define REGEX_INVALID_TOKEN_PATTERN "([^0-9+\\- *\/()])"
#define TOKEN_VALUE_NOT_APPLICABLE -1
#define UNINITIALISED_EXPRESSION "Uninit"

/*
 * Lexical Analyser
 *
 * Performs lexical analysis of an arithmetic expression to extract appropriate
 * tokens
 */
class Lexer
{
public:
  /*
   * Token Operation
   *
   * States the arithmetic operation a token represents
   */
  enum class TokenOperation
  {
    None,
    Literal,
    L_Brace,
    R_Brace,
    Subtraction,
    Addition,
    Multiplication,
    Division
  };

  /*
   * Token Entity
   *
   * Represents a token extracted by the lexer.
   */
  struct Token
  {
    /* Operation the token represents */
    TokenOperation operation;

    /* Value of any literal types, TOKEN_VALUE_NOT_APPLICABLE for non-literal token operations */
    int value;

    /* A raw representation of the token as a string. Useful for providing additional error information. */
    std::string raw;
  };

  Lexer();

  void Tokenise(const std::string &expr);

  const Token &getToken(const int &token_idx);

  size_t getTokenCount();

  void clearTokens();

private:
  std::vector<std::string> regexFindSubstrings(const std::string &pattern) const;

  /* A vector to store extracted tokens */
  std::vector<Token> token_container;

  /* The arithmetic expression that will be tokenised */
  std::string expression;
};
