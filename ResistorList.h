#ifndef RESISTORLIST_H
#define RESISTORLIST_H
#include "Resistor.h"

class ResistorList{
	friend class Node;
	friend class NodeList;
	friend class Rparser;
	private:
		Resistor * head;
	public:
		ResistorList();
		~ResistorList();
		void insertRes(string n, double r, int ePts[]);
		Resistor * findRes(string n);
		bool deleteRes(Resistor * resPtr);
		void printList();
		int getNumRes();
};

#endif
