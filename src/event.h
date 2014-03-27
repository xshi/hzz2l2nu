#ifndef EVENT_H
#define EVENT_H

// ROOT Libraries
#include <TTree.h>
// Standard Libraries
#include <string>
#include <utility>
#include <unordered_map>
#include <map>
#include <sstream>
#include <iostream>


class VariableContainer {
	private:
		std::string name;
		VariableContainer(const VariableContainer &);
		VariableContainer & operator=(const VariableContainer &);
	public:
		VariableContainer(const std::string & n) : name(n) {};
		virtual ~VariableContainer() {};
		const std::string & getName() const {
			return name;
		};
};

template <typename T> class SingleVariableContainer : public VariableContainer {
	private:
		T * varPtr;
		SingleVariableContainer(const SingleVariableContainer &);
		SingleVariableContainer & operator=(const SingleVariableContainer &);
	public:
		SingleVariableContainer(const std::string & n) : VariableContainer(n) {
			varPtr = new T;
		}
		virtual ~SingleVariableContainer() {
			delete varPtr;
		}
		T getVal() const {
			return *varPtr;
		}
		T * getPtr() const {
			return varPtr;
		}
};

template <typename T> class ArrayVariableContainer : public VariableContainer {
	private:
		unsigned size_;
		T * varPtr;
		ArrayVariableContainer(const ArrayVariableContainer &);
		ArrayVariableContainer & operator=(const ArrayVariableContainer &);
	public:
		ArrayVariableContainer(const std::string & nm, unsigned n ) : VariableContainer(nm), size_(n) {
			varPtr = new T[size_];
		}
		virtual ~ArrayVariableContainer() {
			delete[] varPtr;
		}
		T getVal(unsigned i) const {
			if (i >= size_) {
				std::stringstream ss;
				ss << i;
				throw std::string("ERROR: ArrayVariableContainer::getVal : Index out of bounds! - ") + getName() + "[" + ss.str() + "]";
			}
			return varPtr[i];
		}
		T * getPtr() const {
			return varPtr;
		}
		unsigned size() const {
			return size_;
		}
};

template <typename T> class VectorVariableContainer : public VariableContainer {
	private:
		std::vector<T> ** varPtr;
		VectorVariableContainer(const VectorVariableContainer &);
		VectorVariableContainer & operator=(const VectorVariableContainer &);
	public:
		VectorVariableContainer(const std::string & nm ) : VariableContainer(nm) {
			varPtr = new std::vector<T> *;
			*varPtr = 0;
		}
		virtual ~VectorVariableContainer() {
			delete varPtr;
		}
		T getVal(unsigned i) const {
			if (i >= (*varPtr)->size()) {
				std::stringstream ss;
				ss << i;
				throw std::string("ERROR: VectorVariableContainer::getVal : Index out of bounds! - ") + getName() + "[" + ss.str() + "]";
			}
			return (*varPtr)->at(i);
		}
		std::vector<T> * getPtr() const {
			return *varPtr;
		}
		std::vector<T> ** getPtrToPtr() const {
			return varPtr;
		}
};

class Event {
	private:
		std::unordered_map<std::string, VariableContainer *> variables;

		Event(const Event &);
		Event & operator=(const Event &);
	public :
		Event(TTree *tree);
		~Event();
		VariableContainer * findVariable(const std::string & name) const;

		template <typename T> T getSingleVariableValue(const std::string & name) const {
			VariableContainer * tempPtr = findVariable(name);
			if (tempPtr) {
				SingleVariableContainer<T> * varPtr = dynamic_cast<SingleVariableContainer<T> *>(tempPtr);
				if (varPtr)
					return varPtr->getVal();
				else
					throw std::string("ERROR: Event::getSingleVariableValue : Variable name (" + name + ") does not match expected type!");
			} else
				throw std::string("ERROR: Event::getSingleVariableValue : Variable (" + name + ") can't be found!");
		}
		
		template <typename T> inline T getSVV(const std::string & name) const {
			return getSingleVariableValue<T>(name);
		}

		template <typename T> T * getSingleVariableAddress(const std::string & name) const {
			VariableContainer * tempPtr = findVariable(name);
			if (tempPtr) {
				SingleVariableContainer<T> * varPtr = dynamic_cast<SingleVariableContainer<T> *>(tempPtr);
				if (varPtr)
					return varPtr->getPtr();
			}
			return 0;
		}
		
		template <typename T> inline T * getSVA(const std::string & name) const {
			return getSingleVariableAddress<T>(name);
		}

		template <typename T> T getArrayVariableValue(const std::string & name, unsigned i) const {
			VariableContainer * tempPtr = findVariable(name);
			if (tempPtr) {
				ArrayVariableContainer<T> * varPtr = dynamic_cast<ArrayVariableContainer<T> *>(tempPtr);
				if (varPtr)
					return varPtr->getVal(i);
				else
					throw std::string("ERROR: Event::getArrayVariableValue : Variable name (" + name + ") does not match expected type!");
			} else
				throw std::string("ERROR: Event::getArrayVariableValue : Variable (" + name + ") can't be found!");
		}

		template <typename T> inline T getAVV(const std::string & name, unsigned i) const {
			return getArrayVariableValue<T>(name, i);
		}

		template <typename T> T * getArrayVariableAddress(const std::string & name) const {
			VariableContainer * tempPtr = findVariable(name);
			if (tempPtr) {
				ArrayVariableContainer<T> * varPtr = dynamic_cast<ArrayVariableContainer<T> *>(tempPtr);
				if (varPtr)
					return varPtr->getPtr();
			}
			return 0;
		}

		template <typename T> inline T * getAVA(const std::string & name) const {
			return getArrayVariableAddress<T>(name);
		}

		template <typename T> T getVectorVariableValue(const std::string & name, unsigned i) const {
			VariableContainer * tempPtr = findVariable(name);
			if (tempPtr) {
				VectorVariableContainer<T> * varPtr = dynamic_cast<VectorVariableContainer<T> *>(tempPtr);
				if (varPtr)
					return varPtr->getVal(i);
				else
					throw std::string("ERROR: Event::getVectorVariableValue : Variable name (" + name + ") does not match expected type!");
			} else
				throw std::string("ERROR: Event::getVectorVariableValue : Variable (" + name + ") can't be found!");
		}

		template <typename T> inline T getVVV(const std::string & name, unsigned i) const {
			return getVectorVariableValue<T>(name, i);
		}

		template <typename T> std::vector<T> * getVectorVariableAddress(const std::string & name) const {
			VariableContainer * tempPtr = findVariable(name);
			if (tempPtr) {
				VectorVariableContainer<T> * varPtr = dynamic_cast<VectorVariableContainer<T> *>(tempPtr);
				if (varPtr)
					return varPtr->getPtr();
			}
			return 0;
		}

		template <typename T> inline std::vector<T> * getVVA(const std::string & name) const {
			return getVectorVariableAddress<T>(name);
		}
};

#endif // EVENT_H
