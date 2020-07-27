#include "NodeList.h"

NodeList::NodeList(){
	head = NULL;
}

NodeList::~NodeList(){
	Node * nodePtr;
	while(head != NULL){
		nodePtr = head;
		head = nodePtr->next;
		nodePtr->next = NULL;
		delete nodePtr;
	}
}

Node * NodeList::findNode(int nID){
	Node * nodePtr = head;
	while(nodePtr != NULL/* || nodePtr->nodeID < nID*/){
		if(nodePtr->nodeID == nID){
			return nodePtr;
		}
		nodePtr = nodePtr->next;
	}
	return NULL;
}

Node * NodeList::insertNode(int nID){
	//check to see if node already exists
	Node * nodePtr;
	nodePtr = findNode(nID);
	if(nodePtr != NULL){
		return nodePtr;
	}
	
	//node doesn't exist, insert new node
	nodePtr = head;
	Node * prev = NULL;
	Node * newNode = new Node(nID);
	while(nodePtr != NULL && nodePtr->nodeID < nID){
		//if(nodePrt->nodeID < nID){break;}
		prev = nodePtr;
		nodePtr = nodePtr->next;
	}

	newNode->next = nodePtr;
	
	//list initially empty
	if(prev == NULL){
		head = newNode;
	}
	
	else{
		prev->next = newNode;
	}
	return NULL;
}

bool NodeList::resExists(string n){
	Node * nodePtr = head;
	while(nodePtr != NULL){
		if(nodePtr->resList->findRes(n) != NULL){
			return true;
		}
		nodePtr = nodePtr->next;
	}
	return false;
}

bool NodeList::setResistance(string n, double r){
	//check if the resistor exists in the network
	if(!this->resExists(n)){
		return false;
	}
	
	Node * nodePtr = head;
	Resistor * resPtr = NULL;
	//change first resistor object's resistance
	while(resPtr == NULL){
		resPtr = nodePtr->resList->findRes(n);
		nodePtr = nodePtr->next;
	}
	double oldRes = resPtr->resistance;
	resPtr->resistance = r;
	//change second resistor object's resistance
	resPtr = NULL;
	while(resPtr == NULL || nodePtr != NULL){
		resPtr = nodePtr->resList->findRes(n);
		nodePtr = nodePtr->next;
	}
	resPtr->resistance = r;
	cout << "Modified: resistor " << n << " from " << setprecision(2) << oldRes << " Ohms to " << r << " Ohms" << endl;
	return true;
}

bool NodeList::deleteRes(string n){
	//check if the resistor exists in the network
	if(!this->resExists(n)){
		return false;
	}
	
	Node * nodePtr = head;
	Node * prev = NULL;
	Resistor * resPtr = NULL;
	//delete first resistor object
	while(resPtr == NULL){
		resPtr = nodePtr->resList->findRes(n);
		prev = nodePtr;
		nodePtr = nodePtr->next;
	}
	prev->resList->deleteRes(resPtr);
	//delete second resistor object, having seg faults
	//Try running delteRes twice in Rparser instead
	/*
	resPtr == NULL;
	while(resPtr == NULL || nodePtr != NULL){
		cout << "*Finding next R" << endl;
		resPtr = nodePtr->resList->findRes(n);
		prev = nodePtr;
		nodePtr = nodePtr->next;
	}
	cout << "*Found next R, deleting" << endl;
	prev->resList->deleteRes(resPtr);
	cout << "*Deleted next R, done" << endl;
	return true;
	*/
}

void NodeList::deleteAllRes(){
	Node * nodePtr = head;
	while(nodePtr != NULL){
		delete nodePtr->resList;
		nodePtr->resList = new ResistorList();
		nodePtr = nodePtr->next;
	}
}

void NodeList::computeVoltage(Node * nP){
	int maxConnections = nP->resList->getNumRes();
	int nodeArray[maxConnections];
	double resArray[maxConnections];
	int i = 0;
	Resistor * resPtr = nP->resList->head;
	while(resPtr != NULL){
		int ePts[2] = {resPtr->endPts[0], resPtr->endPts[1]};
		if(ePts[0] != nP->nodeID){
			nodeArray[i] = ePts[0];
		}
		else{
			nodeArray[i] = ePts[1];
		}
		resArray[i] = resPtr->resistance;
		resPtr = resPtr->next;
		i++;
	}

	//now we have an array of nodes connected to nP (with duplicates)
	//and an array of resistance between the nodes

	//get number of nodes that nP is connected to
	int numConnections = 0;
	Node * nodePtr = head;
	while(nodePtr != NULL){
		Resistor * resPtr = nodePtr->resList->head;
		while(resPtr != NULL){
			int ePts[2] = {resPtr->endPts[0], resPtr->endPts[1]};
			if(ePts[0] == nP->nodeID || ePts[1] == nP->nodeID){
				numConnections++;
				break;
			}
			resPtr = resPtr->next;
		}
		nodePtr = nodePtr->next;
	}
	numConnections--; //make correction, numConn will always be over 1

	int minNodeArray[numConnections];
	double minResArray[numConnections];

	for(int i = 0; i < numConnections; i ++){
		minResArray[i] = 0;
		minNodeArray[i] = 1234567890; //to avoid not including nodes that have the ID 0
	}

	//instantiate the two new arrays with unique node connections
	//and total resistance between the two nodes

	int x = 0; //where we insert into the minNode array
	for(int i = 0; i < maxConnections; i++){
		bool doNotInsert = false;
		for(int j = 0; j < numConnections; j++){
			if(nodeArray[i] == minNodeArray[j]){
				doNotInsert = true;
				break;
			}
		}
		if(!doNotInsert){
			minNodeArray[x] = nodeArray[i];
			x++;
		}
		for(int k = 0; k < numConnections; k++){
			if(minNodeArray[k] == nodeArray[i]){
				minResArray[k] += resArray[i];
				break;
			}
		}
	}

	double newVoltage;
	double equiResistance = 0;
	double equiCurrent = 0;

	for(int i = 0; i < numConnections; i++){
		equiResistance += (1/minResArray[i]);
	}

	//cout << "*computed resistance" << endl;

	equiResistance = (1/equiResistance);

	//cout << "*computing current" << endl;

	for(int i = 0; i < numConnections; i++){
		Node * p = findNode(minNodeArray[i]);
		double nVoltage = p->nodeVoltage;
		equiCurrent += nVoltage/minResArray[i];
	}

	//cout << "*computed current" << endl;
	newVoltage = equiResistance * equiCurrent;
	nP->nodeVoltage = newVoltage;
}