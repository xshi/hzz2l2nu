#ifndef PHOTONPRESCALE
#define PHOTONPRESCALE

#include <string>
#include <map>
#include <vector>

class PhotonPrescale {
	public :
		class PhotonTrigger {
			public :
				PhotonTrigger(const std::string & trgName, double t, double offs, unsigned idx) : name(trgName), threshold(t), offset(offs), index(idx) {};
				std::string getTriggerName() const {
					return name;
				}
				double getThreshold() const {
					return threshold;
				}
				void readInPrescales(const std::string & inputFileName);
				unsigned getIndex() const {return index;}
				double getOffset() const {return offset;}
			private :
				std::string name;
				double threshold;
				double offset;
				unsigned index;
		};
		void addTrigger(const std::string & tN, double th, double offset, unsigned idx);
		double nextThreshold(double pt) const;
		unsigned getIndex(double pt);
	private :
		std::vector<PhotonTrigger> triggers;
};

#endif // PHOTONPRESCALE
