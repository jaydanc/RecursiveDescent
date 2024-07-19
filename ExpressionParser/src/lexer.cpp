/*
 * Author: Jaydan Cowell
 * Date: 2024-06-02
 */
#include "lexer.h"
#include "errors.h"

Lexer::Lexer() : expression(UNINITIALISED_EXPRESSION) {}

/*
 * Tokenisation
 *
 * Populates the lexical analyser's token vector using regular expression.
 * Regular expressions are simple to use, time-efficient and able to deal with the limited syntax
 * present within arithmetic expresions.
 *
 * @param expr The expression to convert to tokens
 */
void Lexer::Tokenise(const std::string &expr)
{
  expression = expr;

  /* Collect invalid tokens using regular expression pattern. If any exist, this is an error condition. */
  std::vector<std::string> invalid_tokens = regexFindSubstrings(REGEX_INVALID_TOKEN_PATTERN);
  if (!invalid_tokens.empty())
  {
    throw ExpressionParserErrors::InvalidTokenException(invalid_tokens);
  }

  /* Find valid tokens using regular expression. If none are found, this is an issue. */
  std::vector<std::string> valid_tokens = regexFindSubstrings(REGEX_TOKEN_PATTERN);
  if (valid_tokens.empty())
  {
    throw ExpressionParserErrors::EmptyExpressionException();
  }

  /* Parse each token found and create a struct representation */
  for (const std::string &raw_token : valid_tokens)
  {
    char token_type = raw_token[0];
    Token token{.operation = TokenOperation::None, .value = TOKEN_VALUE_NOT_APPLICABLE, .raw = raw_token};

    switch (token_type)
    {
    case '(': token.operation = TokenOperation::L_Brace; break;
    case ')': token.operation = TokenOperation::R_Brace; break;
    case '-': token.operation = TokenOperation::Subtraction; break;
    case '+': token.operation = TokenOperation::Addition; break;
    case '*': token.operation = TokenOperation::Multiplication; break;
    case '/': token.operation = TokenOperation::Division; break;
    default:
      /* If this token is not any of the above, it can only be a number.
         We save its value for later evaluation. */
      token.operation = TokenOperation::Literal;
      token.value = std::stoi(raw_token);
      break;
    }

    token_container.push_back(token);
  }
}

/*
 * Get Token
 *
 * Retrieves a token from the vector based on its index
 *
 * @param token_idx Index into the vector of the token to be returned
 * @return A reference to the stored token struct
 */
const Lexer::Token &Lexer::getToken(const int &token_idx)
{
  /* If the token index is out of bounds with respect to the the vector */
  if (token_idx < 0 || token_idx >= token_container.size())
  {
    throw ExpressionParserErrors::TokenIndexOutOfRange();
  }

  return token_container[token_idx];
}

/*
 * Get Token Count
 *
 * @return Number of elements in the token vector
 */
size_t Lexer::getTokenCount()
{ 
  return token_container.size();
}

/*
 * Get Token Count
 *
 * Empties the token vector and resets state of Lexer instance
 */
void Lexer::clearTokens()
{
  token_container.clear();
  expression = UNINITIALISED_EXPRESSION;
}

/*
 * Regular Expression Find Substrings
 *
 * Uses regular expressions to find substrings according to the provided pattern.
 *
 * @param pattern Reference to the pattern string to match
 * @return An array of substrings which met the pattern criteria
 */
std::vector<std::string> Lexer::regexFindSubstrings(const std::string &pattern) const
{
  std::vector<std::string> output;
  std::regex token_pattern(pattern);

  auto tokens_begin = std::sregex_iterator(expression.begin(), expression.end(), token_pattern);
  auto tokens_end = std::sregex_iterator();

  for (auto it = tokens_begin; it != tokens_end; it++)
  {
    output.push_back(it->str());
  }

  return output;
}