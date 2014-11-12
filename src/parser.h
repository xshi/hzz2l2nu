#include <string>
#include <set>
#include <vector>
#include "expression.h"

#ifndef PARSER_H
#define PARSER_h

class Parser {
	public:
		static Expression parse( const std::string & expression, std::set<std::string> & parameters);
	private:
		static Expression::Operator identifyOperator( const std::string & expr );
		static Expression parseSingleUnit( const std::string & expr, std::set<std::string> & parName );
		static size_t findClosingParenthesis( const std::string & expr, size_t posOpening );
		static std::vector<std::string> tokenize(const std::string & expr);
		static std::string removeWS(const std::string expr);
		static size_t findWhiteSpace(const std::string & expr);
};

#endif // PARSER_H
