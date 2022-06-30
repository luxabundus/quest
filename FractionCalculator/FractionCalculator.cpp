#include "Fraction.h"


int main()
{
	while (true)
	{
		try
		{
			// Print input prompt.
			std::cout << "? ";

			// Get full command line (terminated by CR).
			std::string input;
			std::getline(std::cin, input);

			// Detect exit.
			if (input == "exit")
			{
				break;
			}

			// Parse individual parameters.
			std::string op;
			Fraction left, right;

			std::stringstream(input) >> std::ws >> left >> std::ws >> op >> std::ws >> right;

			// Execute the specified operation.
			Fraction result;

			if (op == "*")	// multiply
			{
				result = left * right;
			}
			else if (op == "/")	// divide
			{
				result = left / right;
			}
			else if (op == "+")	// add
			{
				result = left + right;
			}
			else if (op == "-")	// subtract
			{
				result = left - right;
			}
			else if (op == "%")	// modulo
			{
				result = left % right;
			}
			else
			{
				throw std::invalid_argument("invalid operator '" + op + "'");
			}

			// Print the result.
			std::cout << "= " << result;
		}
		catch (std::exception &x)
		{
			std::cout << std::endl << "!!! " << x.what() << " !!!";
		}

		std::cout << std::endl << std::endl;
	}

	return 0;
}

