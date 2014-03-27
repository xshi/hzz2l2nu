#include "expression.h"
#include <string>
#include <sstream>
#include <iostream>

using std::cout;
using std::endl;
using std::stringstream;
using std::string;
using std::unique_ptr;
using std::map;

Expression::Expression(double val) : type_(VALUE), value_(val) {}

Expression::Expression(const string & varName) : type_(VARIABLE), varName_(varName) {}

Expression::Expression(Operator op, const Expression & right) : type_(UNARY),
		op_(op), rexpr_(new Expression(right)) {}

Expression::Expression(const Expression & left,
		Operator op, const Expression & right) : type_(BINARY),
		lexpr_(new Expression(left)), op_(op), rexpr_(new Expression(right)) {}

Expression::Expression(const Expression & ex) : type_(ex.type_) {
	if (type_ == VALUE)
		value_ = ex.value_;
	else if (type_ == VARIABLE)
		varName_ = ex.varName_;
	else if (type_ == UNARY) {
		op_ = ex.op_;
		rexpr_.reset(new Expression(*ex.rexpr_));
	} else if (type_ == BINARY) {
		lexpr_.reset(new Expression(*ex.lexpr_));
		op_ = ex.op_;
		rexpr_.reset(new Expression(*ex.rexpr_));
	}
}

Expression & Expression::operator=(const Expression & ex) {
	if (this == &ex)
		return *this;

	type_ = ex.type_;
	if (type_ == VALUE)
		value_ = ex.value_;
	if (type_ == VARIABLE)
		varName_ = ex.varName_;
	else if (type_ == UNARY) {
		op_ = ex.op_;
		rexpr_.reset(new Expression(*ex.rexpr_));
	} else if (type_ == BINARY) {
		lexpr_.reset(new Expression(*ex.lexpr_));
		op_ = ex.op_;
		rexpr_.reset(new Expression(*ex.rexpr_));
	}
	return *this;
}

double Expression::eval(const std::map<string, double> & parameters) const {
	if (type_ == VALUE)
		return value_;
	else if (type_ == VARIABLE) {
		auto pos = parameters.find(varName_);
		if (pos != parameters.end())
			return pos->second;
		else
			throw string("ERROR: Expression::eval - Can't find variable!");
	} else if (type_ == UNARY) {
		double rVal = rexpr_->eval( parameters );
		switch (op_) {
			case NOT:
				return !rVal;
			default:
				throw string("ERROR: Unknown operator type!");
		}
	} else if (type_ == BINARY) {
		double lVal = lexpr_->eval( parameters );
		double rVal = rexpr_->eval( parameters );
		switch (op_) {
			case GR:
				return lVal > rVal;
			case GEQ:
				return lVal >= rVal;
			case LE:
				return lVal < rVal;
			case LEQ:
				return lVal <= rVal;
			case EQ:
				return lVal == rVal;
			case NEQ:
				return lVal != rVal;
			case AND:
				return lVal && rVal;
			case OR:
				return lVal || rVal;
			case SUM:
				return lVal + rVal;
			case PRO:
				return lVal * rVal;
			default:
				throw string("ERROR: Unknown operator type!");
		}
	} else
		throw string("ERROR: Unknown expression type!");
}

unsigned Expression::nParameters() const {
	if (type_ == VALUE)
		return 0;
	else if (type_ == VARIABLE)
		return 1;
	else if (type_ == UNARY)
		return rexpr_->nParameters();
	else if (type_ == BINARY)
		return lexpr_->nParameters() + rexpr_->nParameters();
	else
		throw string("ERROR: Unknown expression type!");
}

string Expression::print(const map<string, double> & parameters) const {
	if (type_ == VALUE) {
		stringstream ss;
		ss << value_;
		return ss.str();
	} else if ( type_ == VARIABLE ) {
		if (parameters.size()) {
			auto pos = parameters.find(varName_);
			double val;
			if (pos != parameters.end())
				val = pos->second;
			else
				throw string("ERROR: Expression::eval - Can't find variable!");
			stringstream ss;
			ss << val;
			return ss.str();
		} else
			return varName_;
	} else if ( type_ == UNARY ) {
		string opStr;
		switch (op_) {
			case NOT:
				opStr = "!";
				break;
			default:
				throw string("ERROR: Unknown operator type!");
		}
		return opStr + " " + rexpr_->print(parameters);
	} else if ( type_ == BINARY ) {
		string opStr;
		switch (op_) {
			case GR:
				opStr = ">";
				break;
			case GEQ:
				opStr = ">=";
				break;
			case LE:
				opStr = "<";
				break;
			case LEQ:
				opStr = "<=";
				break;
			case EQ:
				opStr = "==";
				break;
			case NEQ:
				opStr = "!=";
				break;
			case AND:
				opStr = "&&";
				break;
			case OR:
				opStr = "||";
				break;
			case SUM:
				opStr = "+";
				break;
			case PRO:
				opStr = "*";
				break;
			default:
				throw string("ERROR: Unknown operator type!");
		}
		return "(" + lexpr_->print(parameters) + " " + opStr + " " + rexpr_->print(parameters) + ")";
	} else
		throw string("ERROR: Unknown expression type!");
}
