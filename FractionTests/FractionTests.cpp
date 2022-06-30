#include "pch.h"
#include "CppUnitTest.h"
#include "../FractionCalculator/Fraction.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FractionTests
{
	TEST_CLASS(FractionTests)
	{
	public:
		
		TEST_METHOD(Multiplication)
		{
			Fraction result = Fraction("1/2") * Fraction("3/4");
			Assert::AreEqual(std::string("3/8"), result.toString());
		}
		TEST_METHOD(Division)
		{
			Fraction result = Fraction("1/2") / Fraction("3/4");
			Assert::AreEqual(std::string("2/3"), result.toString());
		}
		TEST_METHOD(Addition)
		{
			Fraction result = Fraction("1/2") + Fraction("3/4");
			Assert::AreEqual(std::string("1&1/4"), result.toString());
		}
		TEST_METHOD(Subtraction)
		{
			Fraction result = Fraction("1/2") - Fraction("3/4");
			Assert::AreEqual(std::string("-1/4"), result.toString());
		}
		TEST_METHOD(Modulo)
		{
			Fraction result = Fraction("28/11") % Fraction("2");
			Assert::AreEqual(std::string("6/11"), result.toString());
		}
	};
}
