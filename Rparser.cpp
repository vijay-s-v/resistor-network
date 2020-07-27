#include "Rparser.h"

Rparser::Rparser(){
	nList = new NodeList();
}

Rparser::~Rparser(){
	delete nList;
}

int Rparser::runParser(){
	cout.setf(ios::fixed);
	while(true){
		string line, cmd;
		cout << "> ";
		getline(cin, line, '\n');
		if(cin.eof()){break;}
		stringstream * ss = new stringstream(line);
		*ss >> cmd;
		
		if(cmd == "setV"){setV(ss);}
		else if(cmd == "unsetV"){unsetV(ss);}
		else if(cmd == "solve"){solve();}
		else if(cmd == "insertR"){insertR(ss);}
		else if(cmd == "modifyR"){modifyR(ss);}
		else if(cmd == "printR"){printR(ss);}
		else if(cmd == "printNode"){printNode(ss);}
		else if(cmd == "deleteR"){deleteR(ss);}

		delete ss;
		ss = NULL;
	}
	return 0;
}

void Rparser::setV(stringstream * ss){
	int nID;
	double v;
	*ss >> nID;
	*ss >> v;
	nList->insertNode(nID);
	Node * nodePtr = nList->head;
	while(nodePtr != NULL){
		if(nodePtr->nodeID == nID){
			nodePtr->setVoltage(v);
			cout << "Set: node " << nID << " to " << setprecision(2) << v << " Volts" << endl; 
			return;
		}
		nodePtr = nodePtr->next;
	}
}

void Rparser::unsetV(stringstream * ss){
	int nID;
	*ss >> nID;
	Node * nodePtr = nList->head;
	while(nodePtr != NULL){
		if(nodePtr->nodeID == nID){
			nodePtr->nodeVoltage = 0;
			nodePtr->voltSet = false;
			cout << "Unset: the solver will determine the voltage of node " << nID << endl;
			return;
		}
		nodePtr = nodePtr->next;
	}
}

void Rparser::solve(){
	//check to see if at least one node has its voltage set
	bool solvable = false;
	Node * nodePtr = nList->head;
	while(nodePtr != NULL){
		if(nodePtr->voltSet){
			solvable = true;
			break;
		}
		nodePtr = nodePtr->next;
	}
	if(!solvable){
		cout << "Error: no nodes have their voltage set" << endl;
		return;
	}

	//set all unknown nodes to voltage 0
	nodePtr = nList->head;
	while(nodePtr != NULL){
		if(!nodePtr->voltSet){
			nodePtr->nodeVoltage = 0;
		}
		nodePtr = nodePtr->next;
	}

	//cout << "*init all unknown to 0V" << endl;

	//run the solving algorithm
	double maxChange = 0.0002;
	while(maxChange > MIN_ITERATION_CHANGE){
		//cout << "*running solver algorithm" << endl;
		//compute every node that does not have a set voltage
		double oldVoltage;
		nodePtr = nList->head;
		double currMaxChange = 0;

		while(nodePtr != NULL){
			if(!nodePtr->voltSet){
				oldVoltage = nodePtr->nodeVoltage;
				//cout << "*computing voltage at node " << nodePtr->nodeID << endl;
				nList->computeVoltage(nodePtr); //computes the node's voltage based on eqn 3
				if(abs(oldVoltage - nodePtr->nodeVoltage) > currMaxChange){
					maxChange = abs(oldVoltage - nodePtr->nodeVoltage);
				}
				//cout << "*Old voltage: " << oldVoltage << "V, New voltage: " << nodePtr->nodeVoltage << "V" << endl << endl;
			}
			nodePtr = nodePtr->next;
		}
		//cout << "*maxChange: " << maxChange << endl;
	}

	//done algorithm, now output
	cout << "Solve:" << endl;
	nodePtr = nList->head;
	while(nodePtr != NULL){
		if(nodePtr->resList->head != NULL){
			cout << "  ";
			cout << "Node " << nodePtr->nodeID << ": " << setprecision(2) << nodePtr->nodeVoltage << " V" << endl;
		}
		nodePtr = nodePtr->next;
	}
}

void Rparser::insertR(stringstream * ss){
	string n;
	double r;
	int n1, n2;
	*ss >> n;
	*ss >> r;
	*ss >> n1;
	*ss >> n2;
	
	if(nList->resExists(n)){
		cout << "Error: resistor " << n << " already exists" << endl;
	}

	nList->insertNode(n1);
	nList->insertNode(n2);
	Node * node1 = nList->findNode(n1);
	Node * node2 = nList->findNode(n2);
	int resID[2] = {n1, n2};
	node1->resList->insertRes(n, r, resID);
	node2->resList->insertRes(n, r, resID);
	cout << "Inserted: resistor " << n << " "  << setprecision(2) << r << " Ohms " << n1 << " -> " << n2 << endl;
}

void Rparser::modifyR(stringstream * ss){
	string n;
	double r;
	bool modified;
	*ss >> n;
	*ss >> r;
	
	/*
	if(!nList.resExists(n)){
		cout << "Error: resistor " << n << " not found" << endl;
	}
	*/
	
	modified = nList->setResistance(n, r);
	if(!modified){
		cout << "Error: resistor " << n << " not found" << endl;
	}
}

void Rparser::deleteR(stringstream * ss){
	string n;
	*ss >> n;
	if(n == "all"){
		nList->deleteAllRes();
		cout << "Deleted: all resistors" << endl;
		return;
	}
	if(!nList->resExists(n)){
		cout << "Error: resistor " << n << " not found" << endl;
		return;
	}
	nList->deleteRes(n);
	nList->deleteRes(n);
	cout << "Deleted: resistor " << n << endl;
	return;
}

void Rparser::printR(stringstream * ss){
	string n;
	*ss >> n;
	
	if(!nList->resExists(n)){
		cout << "Error: resistor " << n << " not found" << endl;
		return;
	}
	
	// Get pointer to resistor object with name n
	Resistor * resPtr = NULL;
	Node * nodePtr = nList->head;
	while(resPtr == NULL){
		resPtr = nodePtr->resList->findRes(n);
		nodePtr = nodePtr->next;
	}
	cout << "Print:" << endl;
	resPtr->printResistor();
}

void Rparser::printNode(stringstream * ss){
	string test;
	int nID;
	*ss >> test;
	stringstream convert(test);
	Node * nodePtr = nList->head;
	
	if(test == "all"){
		cout << "Print:" << endl;
		while(nodePtr != NULL){
			nodePtr->printNode();
			nodePtr = nodePtr->next;
		}
		return;
	}
	
	convert >> nID;
	
	while(nodePtr != NULL){
		if(nodePtr->nodeID == nID){
			cout << "Print:" << endl;
			nodePtr->printNode();
		}
		nodePtr = nodePtr->next;
	}
}