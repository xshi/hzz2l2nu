// ROOT Libraries
#include <TLeaf.h>
// Standard Libraries
#include <cstdlib>
#include <iostream>
#include <sstream>
// Other
#include "event.h"

using std::cout;
using std::endl;
using std::pair;
using std::string;
using std::stringstream;
using std::vector;
using std::map;

Event::Event( TTree *tree ) {
	TObjArray * treeLeaves = tree->GetListOfLeaves();
	int nEntries = treeLeaves->GetEntries();
	TObjArray::Iterator_t iter( treeLeaves );
	for( int i = 0; i < nEntries; ++i ) {
		iter.Next();
		TLeaf * leafPointer = dynamic_cast<TLeaf *>( *iter );
		//cout << leafPointer->GetTypeName() << endl;
		//cout << leafPointer->GetLen() << endl;
		//cout << leafPointer->GetBranch()->GetName() << endl;
		//cout << leafPointer->GetBranch()->GetClassName() << endl;

		string varName = leafPointer->GetBranch()->GetName();
		if (variables.find(varName) != variables.end()) {
			cout << "ERROR: Variable \"" << varName << "\" has already been	found!" << endl;
			exit( EXIT_FAILURE );
		}

		TLeaf * leafCount = leafPointer->GetLeafCount();
		int leafLength;
		if (leafCount)
			leafLength = leafCount->GetMaximum();
		else
			leafLength = leafPointer->GetLen();
		bool array = (leafCount || leafLength > 1);

		if ( leafPointer->GetBranch()->GetClassName() != string("")
				&& string(leafPointer->GetTypeName()) != string(leafPointer->GetBranch()->GetClassName()) )
			continue;
		if( leafPointer ) {
			if ( leafPointer->GetTypeName() == string("UInt_t") && !array ) {
				SingleVariableContainer<unsigned> * tempWrapper = new SingleVariableContainer<unsigned>(varName);
				tree->SetBranchAddress( varName.c_str(), tempWrapper->getPtr() );
				variables[varName] = tempWrapper;
			} else if ( leafPointer->GetTypeName() == string("Int_t") && !array ) {
				SingleVariableContainer<int> * tempWrapper = new SingleVariableContainer<int>(varName);
				tree->SetBranchAddress( varName.c_str(), tempWrapper->getPtr() );
				variables[varName] = tempWrapper;
			} else if ( leafPointer->GetTypeName() == string("Float_t") && !array ) {
				SingleVariableContainer<float> * tempWrapper = new SingleVariableContainer<float>(varName);
				tree->SetBranchAddress( varName.c_str(), tempWrapper->getPtr() );
				variables[varName] = tempWrapper;
			} else if ( leafPointer->GetTypeName() == string("Double_t") && !array ) {
				SingleVariableContainer<double> * tempWrapper = new SingleVariableContainer<double>(varName);
				tree->SetBranchAddress( varName.c_str(), tempWrapper->getPtr() );
				variables[varName] = tempWrapper;
			} else if ( leafPointer->GetTypeName() == string("Bool_t") && !array ) {
				SingleVariableContainer<bool> * tempWrapper = new SingleVariableContainer<bool>(varName);
				tree->SetBranchAddress( varName.c_str(), tempWrapper->getPtr() );
				variables[varName] = tempWrapper;
			} else if ( leafPointer->GetTypeName() == string("UInt_t") && array ) {
				ArrayVariableContainer<unsigned> * tempWrapper = new ArrayVariableContainer<unsigned>(varName, leafLength);
				tree->SetBranchAddress( varName.c_str(), tempWrapper->getPtr() );
				variables[varName] = tempWrapper;
			} else if ( leafPointer->GetTypeName() == string("Int_t") && array ) {
				ArrayVariableContainer<int> * tempWrapper = new ArrayVariableContainer<int>(varName, leafLength);
				tree->SetBranchAddress( varName.c_str(), tempWrapper->getPtr() );
				variables[varName] = tempWrapper;
			} else if ( leafPointer->GetTypeName() == string("Float_t") && array ) {
				ArrayVariableContainer<float> * tempWrapper = new ArrayVariableContainer<float>(varName, leafLength);
				tree->SetBranchAddress( varName.c_str(), tempWrapper->getPtr() );
				variables[varName] = tempWrapper;
			} else if ( leafPointer->GetTypeName() == string("Double_t") && array ) {
				ArrayVariableContainer<double> * tempWrapper = new ArrayVariableContainer<double>(varName, leafLength);
				tree->SetBranchAddress( varName.c_str(), tempWrapper->getPtr() );
				variables[varName] = tempWrapper;
			} else if ( leafPointer->GetTypeName() == string("Bool_t") && array ) {
				ArrayVariableContainer<bool> * tempWrapper = new ArrayVariableContainer<bool>(varName, leafLength);
				tree->SetBranchAddress( varName.c_str(), tempWrapper->getPtr() );
				variables[varName] = tempWrapper;
			} else if ( leafPointer->GetTypeName() == string("vector<int>") && !array ) {
				VectorVariableContainer<int> * tempWrapper = new VectorVariableContainer<int>(varName);
				tree->SetBranchAddress( varName.c_str(), tempWrapper->getPtrToPtr() );
				variables[varName] = tempWrapper;
			} else if ( leafPointer->GetTypeName() == string("vector<float>") && !array ) {
				VectorVariableContainer<float> * tempWrapper = new VectorVariableContainer<float>(varName);
				tree->SetBranchAddress( varName.c_str(), tempWrapper->getPtrToPtr() );
				variables[varName] = tempWrapper;
			} else if ( leafPointer->GetTypeName() == string("vector<double>") && !array ) {
				VectorVariableContainer<double> * tempWrapper = new VectorVariableContainer<double>(varName);
				tree->SetBranchAddress( varName.c_str(), tempWrapper->getPtrToPtr() );
				variables[varName] = tempWrapper;
			} else {
				cout << "ERROR: Don't know what to do with this type: " << leafPointer->GetTypeName() << '[' << leafLength << "] " << varName << " !" << endl;
				//exit (EXIT_FAILURE);
			}
		}
	}
}

Event::~Event() {
	for (auto iter = variables.begin(); iter != variables.end(); ++iter)
		delete (*iter).second;
}

//==================================================================================

VariableContainer * Event::findVariable(const std::string & name) const {
	auto location = variables.find(name);
	if (location != variables.end())
		return (*location).second;
	throw string("ERROR VariableContainer * Event::findVariable(const std::string & name): Can't find variable " + name + "!");
}

