#ifndef LEPTON_H
#define LEPTON_H

#include <string>
#include <unordered_map>

class TLorentzVector;

class Lepton {
	private :
		std::unordered_map<std::string, float> varsF;
		std::unordered_map<std::string, int> varsI;
		std::unordered_map<std::string, bool> varsB;

	public :
		virtual TLorentzVector lorentzVector() const;
		void addFloatVar(const std::string &, float);
		void addIntVar(const std::string &, int);
		void addBoolVar(const std::string &, bool);
		float getVarF(const std::string &) const;
		int getVarI(const std::string &) const;
		bool getVarB(const std::string &) const;
		void print() const;
};

#endif // LEPTON_H
