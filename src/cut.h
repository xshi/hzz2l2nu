#include "expression.h"
#include "variableGetter.h"
#include <memory>
#include <set>

#ifndef CUT_H
#define CUT_H

class Event;

class Cut {
	private:
		std::string name_;
		Expression cut_;
		std::set<VariableGetter> vars_;
//		Cut(const Cut & cut);
	public:
		Cut(const Event & ev, const std::string & cutExpr);
		Cut & operator=(const Cut & c);
		bool operator==(const Cut & c);
		virtual bool operator()() const;
		virtual const std::string & GetName() const {return name_;}
};

#endif // CUT_H
