#ifndef RPARSER_H
#define RPARSER_H
#include <sstream>
#include <cmath>
#include "NodeList.h"
#define MIN_ITERATION_CHANGE 0.0001

class Rparser{
	private: 
		NodeList * nList;
		void insertR(stringstream * ss);
		void modifyR(stringstream * ss);
		void printR(stringstream * ss);
		void printNode(stringstream * ss);
		void deleteR(stringstream * ss);
		void setV(stringstream * ss);
		void unsetV(stringstream * ss);
		void solve();
	
	public:
		Rparser();
		~Rparser();
		int runParser();
};

#endif