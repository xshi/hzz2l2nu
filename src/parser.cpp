#include "parser.h"
#include "expression.h"
#include <sstream>
#include <iostream>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;
using std::set;

Expression Parser::parse( const string & expression, set<string> & parameters) {
	//cout << "Parser::parse : " << expression << endl;
	vector<string> exprElements = tokenize(expression);
	if (exprElements.size() == 3) {
		Expression left = parse(exprElements[0], parameters);
		Expression::Operator op = identifyOperator(exprElements[1]);
		Expression right = parse(exprElements[2], parameters);
		return Expression(left, op, right);
	} else if (exprElements.size() == 2) {
		Expression::Operator op = identifyOperator(exprElements[0]);
		Expression right = parse(exprElements[1], parameters);
		return Expression(op, right);
	} else if (exprElements.size() == 1) {
		if (expression == exprElements[0])
			return parseSingleUnit(exprElements[0], parameters);
		else
			return parse(exprElements[0], parameters);
	} else if (exprElements.size() == 0) {
		return parse("1", parameters); 
	} else {
		cout << string("ERROR: Can't parse expression : " + expression + "!") << endl;
		return parse("1", parameters);
	}
}

Expression::Operator Parser::identifyOperator( const std::string & expr ) {
	if (expr == ">")
		return Expression::GR;
	else if (expr == ">=")
		return Expression::GEQ;
	else if (expr == "<")
		return Expression::LE;
	else if (expr == "<=")
		return Expression::LEQ;
	else if (expr == "==")
		return Expression::EQ;
	else if (expr == "!=")
		return Expression::NEQ;
	else if (expr == "!")
		return Expression::NOT;
	else if (expr == "&&")
		return Expression::AND;
	else if (expr == "||")
		return Expression::OR;
	else if (expr == "+")
		return Expression::SUM;
	else if (expr == "*")
		return Expression::PRO;
	else
		throw string("ERROR: Can't recognize operator type: " + expr + "!");
}

Expression Parser::parseSingleUnit( const std::string & expr, std::set<std::string> & parName ) {
	if (expr[0] == '.' || isdigit(expr[0])) {
		stringstream ss;
		ss << expr;
		double val;
		ss >> val;
		return Expression(val);
	} else {
		parName.insert(expr);
		return Expression(expr);
	}
}

size_t Parser::findClosingParenthesis( const std::string & expr, size_t posOpening ) {
	unsigned nOpened = 0;
	unsigned i = posOpening;
	for (; i < expr.size(); ++i) {
		if (expr[i] == '(')
			nOpened++;
		if (expr[i] == ')')
			nOpened--;
		if (nOpened <= 0)
			break;
	}
	return i;
}

vector<string> Parser::tokenize(const std::string & text) {
	vector<string> tokens;
//	cout << "Parser:: tokenize : " << text << endl;
	string cleanExp = removeWS(text);
	while (cleanExp.size()) {
		size_t posP = cleanExp.find('(');
		size_t posWS = findWhiteSpace(cleanExp);
		if (posP != string::npos || posWS != string::npos) {
			if (posP < posWS && posP == 0) {
				size_t posCP = findClosingParenthesis(cleanExp, posP);
				tokens.push_back(cleanExp.substr(posP + 1, posCP - posP - 1));
				cleanExp.erase(posP, posCP - posP + 1);
				cleanExp = removeWS(cleanExp);
			} else if (posWS < posP) {
				tokens.push_back( cleanExp.substr(0, posWS) );
				cleanExp.erase(0, posWS);
				cleanExp = removeWS(cleanExp);
			} else
				throw string("ERROR: Can't properly tokenize expression!");
		} else {
			tokens.push_back(cleanExp);
			break;
		}
	}

//	cout << "TOKENS:" << endl;
//	for (unsigned i = 0; i < tokens.size(); ++i)
//		cout << tokens[i] << endl;
	return tokens;
}

std::string Parser::removeWS(const std::string expr) {
	int i = 0;
	while (i < int(expr.size()) && isspace(expr[i]))
		i++;
	string output = expr.substr(i);
	i = output.size() - 1;
	while (i > 0 && isspace(output[i]))
		i--;
	output.erase(i + 1);
	return output;
}

size_t Parser::findWhiteSpace(const std::string & expr) {
	for (unsigned i = 0; i < expr.size(); ++i)
		if (isspace(expr[i]))
			return i;
	return string::npos;
}
