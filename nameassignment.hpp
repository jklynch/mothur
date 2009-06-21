#ifndef NAMEASSIGNMENT_HPP
#define NAMEASSIGNMENT_HPP

#include "mothur.h"
#include "listvector.hpp"

class NameAssignment : public map<string,int> {
public:
	NameAssignment(string);
	void readMap();
	ListVector getListVector();
	int get(string);
	void print();
private:
	ifstream fileHandle;
	ListVector list;
};




#endif
