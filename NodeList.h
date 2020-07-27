#ifndef NODELIST_H
#define NODELIST_H
#include "Node.h"

class NodeList{
	//NodeList is a LinkedList sorted in ascending order of nodeID
	friend class Rparser;
	private:
		Node * head;
	public: 
		NodeList();
		~NodeList();
		Node * findNode(int nID);
		Node * insertNode(int nID); //insert new node if nID not in NodeList, return pointer to Node if nID in NodeList
		bool resExists(string n);
		bool setResistance(string n, double r);
		bool deleteRes(string n);
		void deleteAllRes();
		void computeVoltage(Node * nP);
};

#endif
