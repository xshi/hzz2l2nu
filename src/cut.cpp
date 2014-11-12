// Standard Libraries
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
// ROOT Libraries
#include <TLorentzVector.h>
// Other
#include "cut.h"
#include "event.h"
#include "parser.h"
#include <map>
#include <utility>
#include <fstream>

using std::map;
using std::setw;
using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::set;

Cut::Cut(const Event & ev, const std::string & cutExpr) : name_(cutExpr) {
	set<string> parameters;
	cut_ = Parser::parse(cutExpr, parameters);
	for (auto iter = parameters.begin(); iter != parameters.end(); ++iter) {
		vars_.insert( VariableGetter(*iter, ev) );
	}
}

Cut & Cut::operator=(const Cut & c) {
	if (this == &c)
		return *this;

	name_ = c.name_;
	cut_ = c.cut_;
	vars_ = c.vars_;

	return *this;
}

bool Cut::operator()() const {
	map<string, double> parVals;
	for (auto it = vars_.begin(); it != vars_.end(); ++it)
		parVals.insert( make_pair(it->getName(), it->getValue()) );
	return cut_.eval(parVals);
}

