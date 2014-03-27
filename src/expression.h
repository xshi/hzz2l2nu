#include <memory>
#include <string>
#include <map>

#ifndef EXPRESSION_H
#define EXPRESSION_H

class Expression {
	public:
		enum Operator { GR, GEQ, LE, LEQ, EQ, NEQ, NOT, AND, OR, SUM, PRO };
		enum ExprT { VALUE, VARIABLE, UNARY, BINARY };
		Expression() {};
		Expression(double val);
		Expression(const std::string & varName);
		Expression(Operator op, const Expression & right);
		Expression(const Expression & left, Operator op, const Expression & right);
		Expression(const Expression & ex);
		Expression & operator=(const Expression & ex);
		double eval(const std::map<std::string, double> & parameters) const;
		unsigned nParameters() const;
		std::string print(const std::map<std::string, double> & parameters = std::map<std::string, double>() ) const;
	private:
		ExprT type_;
		double value_;
		std::string varName_;
		std::unique_ptr<Expression> lexpr_;
		Operator op_;
		std::unique_ptr<Expression> rexpr_;
};

#endif // EXPRESSION_H
