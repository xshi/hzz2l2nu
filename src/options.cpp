#include "options.h"
#include <cstdlib>
#include <iostream>
#include <utility>
#include <fstream>
#include "toolbox.h"

using std::ifstream;
using std::pair;
using std::string;
using std::cout;
using std::endl;

void Options::readInOptions(const std::string & fileName) {
	ifstream in(fileName.c_str());
	if (!in.is_open())
		throw string("ERROR: Can't open configuration file: " + fileName + "!");
	while (!in.eof()) {
		string tempLine;
		getline(in, tempLine);
		if (!tempLine.size() || tempLine[0] == '\n' || tempLine[0] == '#')
			continue;
		size_t pos = tempLine.find('=');
		if (pos != string::npos) {
			string optName = tempLine.substr(0, pos);
			string optValue = tempLine.substr(pos + 1);
			if (optValue == "true" || optValue == "false")
				boolOptions[optName] = (optValue == "true" ? true : false);
			else
				stringOptions[optName] = optValue;
		}
	}
}

bool Options::checkBoolOption(const std::string & name) const {
	auto pos = boolOptions.find(name);
	if ( pos != boolOptions.end())
		return pos->second;
	else
		throw string("ERROR: Can't find requested bool option: " + name + "!");
}

double Options::checkDoubleOption(const std::string & name) const {
	auto pos = stringOptions.find(name);
	if (pos != stringOptions.end())
		return string2double(pos->second);
	else
		throw string("ERROR: Can't find requested double option: " + name + "!");
}

const string & Options::checkStringOption(const std::string & name) const {
	auto pos = stringOptions.find(name);
	if (pos != stringOptions.end())
		return pos->second;
	else
		throw string("ERROR: Can't find requested string option: " + name + "!");
}

void Options::addBoolOption(const string & name, bool value) {
	boolOptions[name] = value;
}

void Options::addDoubleOption(const string & name, double value) {
	stringOptions[name] = double2string(value);
}

void Options::addStringOption( const std::string & name, const std::string & value ) {
	stringOptions[name] = value;
}

void Options::print() const {
	for (auto iter = boolOptions.begin(); iter != boolOptions.end(); ++iter)
		cout << iter->first << "=" << iter->second << endl;
	for (auto iter = stringOptions.begin(); iter != stringOptions.end(); ++iter)
		cout << iter->first << "=" << iter->second << endl;
}
