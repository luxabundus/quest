#pragma once

#include <exception>
#include <algorithm>
#include <iostream>
#include <sstream>


class Fraction
{
public:
	Fraction();
	Fraction(const Fraction &origin);
	explicit Fraction(double dbl);
	explicit Fraction(const std::string &str);

	bool mixed() const;
	bool zero() const;
	void clear();

	void fromString(const std::string &str);
	std::string toString(bool asMixed = true) const;

	void fromDouble(double dbl);
	double toDouble() const;

	operator double() const;

	Fraction operator * (const Fraction &rhs) const;
	Fraction operator / (const Fraction &rhs) const;
	Fraction operator + (const Fraction &rhs) const;
	Fraction operator - (const Fraction &rhs) const;
	Fraction operator % (const Fraction &rhs) const;

	Fraction &operator = (const Fraction &rhs);

protected:
	Fraction(int numerator, int denominator);

	void reduce();
	int calcGcd(int a, int b) const;

private:
	int m_numerator;
	int m_denominator;
};


/*
* I/O Stream Operators
*/
inline std::istream &operator >> (std::istream &in, Fraction &fraction)
{
	std::string str;
	in >> str;

	fraction.fromString(str);

	return in;
}

inline std::ostream &operator << (std::ostream &out, const Fraction &fraction)
{
	// Print as a mixed fraction (default).
	out << fraction.toString();

	return out;
}


/*
* Class Implementation
*/
inline Fraction::Fraction() :
	m_numerator(0), m_denominator(1)
{
}

inline Fraction::Fraction(const Fraction &origin) :
	m_numerator(origin.m_numerator), m_denominator(origin.m_denominator)
{
}

inline Fraction::Fraction(double dbl) :
	m_numerator(0), m_denominator(1)
{
	fromDouble(dbl);
}

inline Fraction::Fraction(const std::string &str) :
	m_numerator(0), m_denominator(1)
{
	fromString(str);
}

inline Fraction::Fraction(int numerator, int denominator) :
	m_numerator(numerator), m_denominator(denominator)
{
	// Force zero if numerator is zero, i.e., 0/5 = 0/1.
	if (m_numerator == 0)
	{
		m_denominator = 1;
	}
	// Validate denominator, i.e., cannot be zero or negative.
	else if (m_denominator <= 0)
	{
		throw std::invalid_argument("invalid denominator value '" + std::to_string(m_denominator) + "'");
	}

	// Ensure smallest possible fraction.
	reduce();
}

inline void Fraction::reduce()
{
	int gcd = calcGcd(std::abs(m_numerator), m_denominator);
	if (gcd > 1)
	{
		m_numerator /= gcd;
		m_denominator /= gcd;
	}
}

inline int Fraction::calcGcd(int a, int b) const
{
	return b ? calcGcd(b, a % b) : a;
}

inline bool Fraction::mixed() const
{
	return std::abs(m_numerator) > m_denominator;
}

inline bool Fraction::zero() const
{
	return m_numerator == 0;
}

inline void Fraction::clear()
{
	m_numerator = 0;
	m_denominator = 1;
}

inline void Fraction::fromString(const std::string &str)
{
	clear();

	// Valid inputted string, i.e., not empty.
	if (str.empty())
	{
		throw std::exception("empty fraction string");
	}

	// Scan the inputted string for individual components.
	char op1 = 0, op2 = 0;
	int part1 = 0, part2 = 0, part3 = 0;

	int count = sscanf_s(str.c_str(), "%d%c%d%c%d", &part1, &op1, 1, &part2, &op2, 1, &part3);

	// First, try mixed fraction, e.g., 2&3/4.
	if ((count == 5) && (op1 == '&') && (op2 == '/'))
	{
		// Validate positive numerator.
		if (part2 < 0)
		{
			throw std::invalid_argument("invalid numerator '" + str + "'");
		}
		// Validate positive, non-zero denominator.
		else if (part3 <= 0)
		{
			throw std::invalid_argument("invalid denominator '" + str + "'");
		}

		// Convert to (possible) improper faction, i.e. 2&3/4 = 11/4.
		int sign = (part1 < 0) ? -1 : 1; // Preserve sign during numerator computation; reapply at end.
		m_numerator = (part2 + (std::abs(part1) * part3)) * sign;	
		m_denominator = part3;

	}
	// Else, try simple fraction (could be improper), e.g., 23/4.
	else if ((count == 3) && (op1 == '/'))
	{
		// Validate positive, non-zero denominator.
		if (part2 <= 0)
		{
			throw std::invalid_argument("invalid denominator '" + str + "'");
		}

		m_numerator = part1;
		m_denominator = part2;
	}
	// Else, try a single number, e.g., 23.
	else if (count == 1)
	{
		m_numerator = part1;
		m_denominator = 1;
	}
	// Otherwise, throw invalid format error.
	else
	{
		throw std::invalid_argument("invalid fraction '" + str + "'");
	}

	reduce();
}

inline std::string Fraction::toString(bool asMixed) const
{
	std::stringstream out;

	if (m_numerator == 0)
	{
		out << 0;
	}
	else if (m_denominator == 0)
	{
		out << "undefined";
	}
	else if (m_denominator == 1)
	{
		out << m_numerator;
	}
	else if (asMixed && mixed())
	{
		int whole = m_numerator / m_denominator; // Keep sign for whole number.
		int numerator = std::abs(m_numerator) % m_denominator; // Remove sign from numerator.
		out << whole << "&" << numerator << "/" << m_denominator;
	}
	// Else, print a simple fraction.
	else
	{
		out << m_numerator << "/" << m_denominator;
	}

	return out.str();
}

inline void Fraction::fromDouble(double dbl)
{
	static const int PRECISION = 1000000000;

	clear();

	// Extract integral and fractional parts.
	double integral = floor(dbl);
	double fractional = dbl - integral;

	// If this is an actual fraction, compute numerator and denominator.
	if (fractional)
	{
		fractional = round(fractional * PRECISION);

		int gcd = calcGcd((int)fractional, PRECISION);

		m_denominator = PRECISION / gcd;
		m_numerator = (int)((integral * m_denominator) + (fractional / gcd));
	}
	// Else, just assume an integer.
	else if (integral)
	{
		m_numerator = (int)integral;
		m_denominator = 1;
	}

	reduce();
}

inline double Fraction::toDouble() const
{
	return (double)m_numerator / (double)m_denominator;
}

inline Fraction::operator double() const
{
	return toDouble();
}

inline Fraction Fraction::operator * (const Fraction &rhs) const
{
	return Fraction(
		m_numerator * rhs.m_numerator,
		m_denominator * rhs.m_denominator);
}

inline Fraction Fraction::operator / (const Fraction &rhs) const
{
	// Throw error if the divisor is zero.
	if (rhs.zero())
	{
		throw std::invalid_argument("division by zero");
	}
	// Return a zero result if the dividend (this) is zero.
	else if (zero())
	{
		return Fraction();
	}
	else
	{
		// Preserve divisor's sign.
		int rhsSign = rhs.m_numerator < 0 ? -1 : 1;

		return Fraction(
			(m_numerator * rhs.m_denominator) * rhsSign, // factor in divisor sign (for numerator only)
			m_denominator * std::abs(rhs.m_numerator));
	}
}

inline Fraction Fraction::operator + (const Fraction &rhs) const
{
	// If the first (this) addend is zero, just return the second addend.
	if (zero())
	{
		return rhs;
	}
	// If the second addend is zero, just return the first addend.
	else if (rhs.zero())
	{
		return *this;
	}
	else
	{
		return Fraction(
			(m_numerator * rhs.m_denominator) + (rhs.m_numerator * m_denominator),
			m_denominator * rhs.m_denominator);
	}
}

inline Fraction Fraction::operator - (const Fraction &rhs) const
{
	// If the minuend (this) is zero, return a negative subtrahend.
	if (zero())
	{
		Fraction result(rhs);
		result.m_numerator = -result.m_numerator;
		return result;
	}
	// If the subtrahend is zero, return an unchanged minuend.
	else if (rhs.zero())
	{
		return *this;
	}
	else
	{
		return Fraction(
			(m_numerator * rhs.m_denominator) - (rhs.m_numerator * m_denominator),
			m_denominator * rhs.m_denominator);
	}
}

inline Fraction Fraction::operator % (const Fraction &rhs) const
{
	// Restrict the divisor to positive whole numbers.
	if ((rhs.m_numerator <= 0) || (rhs.m_denominator != 1))
	{
		throw std::invalid_argument("invalid modulo divisor '" + rhs.toString() + "'");
	}

	// Using: a mod b = a - (b * floor(a / b))...
	const Fraction &a = *this;
	const Fraction &b = rhs;
	return a - Fraction(b * std::floor(a / b));
}

inline Fraction &Fraction::operator = (const Fraction &rhs)
{
	m_numerator = rhs.m_numerator;
	m_denominator = rhs.m_denominator;
	return *this;
}



