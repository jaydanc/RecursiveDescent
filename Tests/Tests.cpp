/*
 * Author: Jaydan Cowell
 * Date: 2024-06-02
 */
#include "CppUnitTest.h"

#include "../ExpressionParser/src/rd_parser.h"
#include "../ExpressionParser/src/errors.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(ExpressionParserTests)
	{
	public:

		TEST_METHOD(InvalidExpressionLexerExcept)
		{
			RDParser parser;
			auto function = [&parser] { parser.Parse("1 + 3 + test"); };
			Assert::ExpectException<ExpressionParserErrors::InvalidTokenException>(function);
		}

		TEST_METHOD(SimpleTerm)
		{
			RDParser parser;
			int result = parser.Parse("1 + 3");
			Assert::AreEqual(result, 4);
		}

		TEST_METHOD(BracePrecedes)
		{
			RDParser parser;
			int result = parser.Parse("4 + (12 / (1 * 2))");
			Assert::AreEqual(result, 10);
		}

		TEST_METHOD(NegationApplied)
		{
			RDParser parser;
			int result = parser.Parse("1 + 4 * -2");
			Assert::AreEqual(result, -10);
		}

		TEST_METHOD(SubtractionApplied)
		{
			RDParser parser;
			int result = parser.Parse("1 - 5 * 2");
			Assert::AreEqual(result, -8);
		}

		TEST_METHOD(FactorTermHasEqualPrecedence)
		{
			RDParser parser;
			int result = parser.Parse("1 + 3 * 4");
			Assert::AreEqual(result, 16);
		}

		TEST_METHOD(Division)
		{
			RDParser parser;
			int result = parser.Parse("4 + (12 / (1 * 2))");
			Assert::AreEqual(result, 10);
		}

		TEST_METHOD(DivideByZeroExcepts)
		{
			RDParser parser;
			auto function = [&parser] { parser.Parse("5 / 0"); };
			Assert::ExpectException<ExpressionParserErrors::DivideByZeroException>(function);
		}

		TEST_METHOD(UnexpectedTokenAtEndExcepts)
		{
			RDParser parser;
			auto function = [&parser] { parser.Parse("5 + 6 + 4 +"); };
			Assert::ExpectException<ExpressionParserErrors::UnexpectedTokenException>(function);
		}

		TEST_METHOD(InvalidTokenCombinationExcepts)
		{
			RDParser parser;
			auto function = [&parser] { parser.Parse("5 + 6 *+ 4"); };
			Assert::ExpectException<ExpressionParserErrors::UnexpectedTokenException>(function);
		}

		TEST_METHOD(UnmatchedLeftBraceExcepts)
		{
			RDParser parser;
			auto function = [&parser] { parser.Parse("(1 + (12 * 2) "); };
			Assert::ExpectException<ExpressionParserErrors::ParenthesesMismatchException>(function);
		}

		TEST_METHOD(UnmatchedRightBraceExcepts)
		{
			RDParser parser;
			auto function = [&parser] { parser.Parse("5 + 6) *+ 4"); };
			Assert::ExpectException<ExpressionParserErrors::UnexpectedParenthesesException>(function);
		}

		TEST_METHOD(IncorrectLeftBracePositionExcepts)
		{
			RDParser parser;
			auto function = [&parser] { parser.Parse("5( + 6 *+ 4"); };
			Assert::ExpectException<ExpressionParserErrors::UnexpectedParenthesesException>(function);
		}

		TEST_METHOD(IncorrectRightBracePositionExcepts)
		{
			RDParser parser;
			auto function = [&parser] { parser.Parse("5 + )6 *+ 4"); };
			Assert::ExpectException<ExpressionParserErrors::UnexpectedTokenException>(function);
		}

		TEST_METHOD(MultipleNegationValid)
		{
			RDParser parser;
            int result = parser.Parse("----5+---6*6");
            Assert::AreEqual(result, -6);
		}
	};
}
