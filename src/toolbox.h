#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <string>
#include <vector>
#include <iostream>
#include <functional>

class Event;

double min( double a, double b );
double max( double a, double b );
double deltaPhi( double phi1, double phi2 );
double deltaR( double eta1, double phi1, double eta2, double phi2);
std::string double2string(double num);
double string2double(const std::string & num);
template <typename T>
T min( const std::vector<T> values) {
	T min = 999999999;
	for (unsigned i = 0; i < values.size(); ++i)
		if ( values[i] < min )
			min = values[i];
	return min;
}
template <typename T>
T max(T a, T b) {
	if (a > b)
		return a;
	else
		return b;
}
double calculateParA(double a1, double b1, double c1, double b2, double c2, double x);
double calculateParD(double a1, double b1, double c1, double d1, double a2, double b2, double c2, double x);
double myFunc(double a, double b, double c, double d, double x);
double ptFunc(double zpt, int nParIn, const double * paramsIn);
double ptFuncIntegral(double zpt, double a, double b);
unsigned evCategory(int nhardjet, int nsoftjet, double delEta, double mjj, bool isPhotonSample);
std::vector<std::string> tokenize(std::string text, char token);
std::string encode(const std::string & str, unsigned len = 128);

class EventAdr;
namespace std {
	template <>
		class hash<EventAdr>{
			public :
				size_t operator()(EventAdr ev) const;
		};
}

class EventAdr {
	private :
		unsigned run;
		unsigned lumi;
		unsigned event;
	public :
		EventAdr() : run(0), lumi(0), event(0) {};
		EventAdr(unsigned r, unsigned l, unsigned e) : run(r), lumi(l), event(e) {};
		bool operator==(const EventAdr & ev) const {
			return run == ev.run && lumi == ev.lumi && event == ev.event;
		}
		bool operator<(const EventAdr & ev) const {
			if (run == ev.run) {
				if (lumi == ev.lumi)
					return event < ev.event;
				else
					return lumi < ev.lumi;
			} else
				return run < ev.run;
		}
		friend std::ostream & operator<<(std::ostream & os, const EventAdr & ev);
		friend size_t std::hash<EventAdr>::operator()(EventAdr) const;
};

std::ostream & operator<<(std::ostream & os, const EventAdr & ev);
double metCutValue(double mh);
std::pair<double, double> mtCutValue(double mh);
double BW(double m, double G, double mH);

class BreitWignerNR {
	private:
		double c_;
		double br_;
		double mh_;
		double n_;
		double g_;
		void gamma();
	public:
		BreitWignerNR() : c_(-1.0), br_(-1.0), mh_(-1.0), n_(-1.0), g_(-1.0) {}
		BreitWignerNR(double C, double BR, double MH, double N) : c_(C), br_(BR), mh_(MH), n_(N) {
			gamma();
		}
		double operator()(double m) const;
};
#endif // EVENT_H
