#ifndef NODE_H
#define NODE_H
#include "ResistorList.h"

class Node{
	friend class NodeList;
	friend class Rparser;
	private: 
		int nodeID;
		double nodeVoltage;
		bool voltSet;
		ResistorList * resList;
		Node * next;
	public: 
		Node(int nID);
		~Node();
		void setVoltage(double v);
		double getVoltage();
		void printNode();
};

#endif
