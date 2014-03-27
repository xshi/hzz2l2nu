// ROOT Libraries
#include <TLorentzVector.h>
// Standard Libraries
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
// Other
#include "event.h"
#include "variableGetter.h"

using std::cout;
using std::endl;
using std::string;
using std::stringstream;

VariableGetter::VariableGetter(const string & hName, const Event & ev) {
	var = ev.findVariable(hName);
	if (!var)
		throw string("ERROR: VariableGetter::VariableGetter : Can't find this variable: " + hName + "!");
	if (dynamic_cast<const SingleVariableContainer<unsigned> *>(var))
		type = UNS;
	else if (dynamic_cast<const SingleVariableContainer<int> *>(var))
		type = INT;
	else if (dynamic_cast<const SingleVariableContainer<double> *>(var))
		type = DBL;
	else if (dynamic_cast<const VectorVariableContainer<int> *>(var))
		type = VEI;
	else if (dynamic_cast<const VectorVariableContainer<float> *>(var))
		type = VEF;
	else
		throw string("ERROR: VariableGetter::VariableGetter : Don't know what to do with this variable: " + hName + "!");
	name = hName;
}

double VariableGetter::getValue() const {
	if (type == UNS) {
		const SingleVariableContainer<unsigned> * tmpVar = dynamic_cast<const SingleVariableContainer<unsigned> *>(var);
		return tmpVar->getVal();
	} else if (type == INT) {
		const SingleVariableContainer<int> * tmpVar = dynamic_cast<const SingleVariableContainer<int> *>(var);
		return tmpVar->getVal();
	} else if (type == DBL) {
		const SingleVariableContainer<double> * tmpVar = dynamic_cast<const SingleVariableContainer<double> *>(var);
		return tmpVar->getVal();
	} else
		throw string("ERROR: VariableGetter::getValue() : This is not simple variable: " + getName() + "!");
}

double VariableGetter::getValue(unsigned i) const {
	if (type == VEI) {
		const VectorVariableContainer<int> * tmpVar = dynamic_cast<const VectorVariableContainer<int> *>(var);
		return tmpVar->getVal(i);
	} else if (type == VEF) {
		const VectorVariableContainer<float> * tmpVar = dynamic_cast<const VectorVariableContainer<float> *>(var);
		return tmpVar->getVal(i);
	} else
		throw string("ERROR: VariableGetter::getValue() : This is not vector variable: " + getName() + "!");
}

int VariableGetter::size() const {
	if (type == VEI) {
		const VectorVariableContainer<int> * tmpVar = dynamic_cast<const VectorVariableContainer<int> *>(var);
		return tmpVar->getPtr()->size();
	} else if (type == VEF) {
		const VectorVariableContainer<float> * tmpVar = dynamic_cast<const VectorVariableContainer<float> *>(var);
		return tmpVar->getPtr()->size();
	} else
		return -1;
}
