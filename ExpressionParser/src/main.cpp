/*
 * Author: Jaydan Cowell
 * Date: 2024-06-02
 *
 * A recursive descent (LL) parser for simple arithmic expressions with modified operator
 * precedence.
 */
#include <iostream>

#include "rd_parser.h"

bool evaluate(const char *expression, int &result);

/*
 * Entry Point
 *
 * The initial point of entry for the recursive descent parsing application.
 *
 * @return Exit Reason
 */
int main()
{
  int result = -1;
  if (evaluate("5+6*6", result))
  {
    std::cout << "5+6*6 = " << result << std::endl;
  }
  else
  {
    std::cerr << "Failed to evaluate 5+6*6" << std::endl;
  }

  return 0;
}

/*
 * Evaluate
 *
 * Evaluates a simple arithmetic expression
 *
 * @param expression The expression to evaluate
 * @param result The expression's result
 * @return Result of evaluation (true/false)
 */
bool evaluate(const char *expression, int &result)
{
  try
  {
    /* Employs a top-down approach (Recursive Descent) to evaluate the expression due to the simplicity of the grammar. */
    RDParser recursive_descent_parser;
    result = recursive_descent_parser.Parse(expression);
    return true;
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    result = -1;
    return false;
  }
}