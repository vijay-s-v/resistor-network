#include "ResistorList.h"
#include "Node.h"

Node::Node(int nID){
	nodeID = nID;
	resList = new ResistorList();
	nodeVoltage = 0;
	voltSet = false;
	next = NULL;
}

Node::~Node(){
	delete resList;
	delete next;
}

void Node::setVoltage(double v){
	nodeVoltage = v;
	voltSet = true;
}

double Node::getVoltage(){
	return nodeVoltage;
}

void Node::printNode(){
	int numRes = resList->getNumRes();
	if(numRes > 0){
		cout << "Connections at node " << nodeID << ": " << numRes << " resistor(s)" << endl;
		resList->printList();
	}
	else if(voltSet){
		cout << "Connections at node " << nodeID << ": " << setprecision(2) << nodeVoltage << " V" << endl;
	}
}
