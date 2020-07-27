#include "ResistorList.h"

ResistorList::ResistorList(){
	head = NULL;
}

ResistorList::~ResistorList(){
	Resistor * resPtr;
	while(head != NULL){
		resPtr = head;
		head = resPtr->next;
		resPtr->next = NULL;
		delete resPtr;
	}
}

void ResistorList::insertRes(string n, double r, int ePts[]){
	Resistor * newRes = new Resistor(n, r, ePts);
	Resistor * resPtr = head;
	Resistor * prevRes = NULL;
	
	//list initially empty
	if(resPtr == NULL){
		head = newRes;
		newRes->next = NULL;
		return;
	}
	
	while(resPtr != NULL){
		prevRes = resPtr;
		resPtr = resPtr->next;
	}
	
	prevRes->next = newRes;
	newRes->next = NULL;
	return;
}

Resistor * ResistorList::findRes(string n){
	Resistor * p = head;
	while(p != NULL){
		if(p->name == n){
			return p;
		}
		p = p->next;
	}
	return NULL;
}

bool ResistorList::deleteRes(Resistor * resPtr){
	Resistor * p = head;
	Resistor * prev = NULL;
	
	while(p != resPtr){
		prev = p;
		p = p->next;
	}
	
	if(p == NULL){
		return false;
	}
	if(prev == NULL){
		head = p->next;
	}
	else{
		prev->next = p->next;
	}

	p->next = NULL;
	delete p;
	return true;
}

void ResistorList::printList(){
	Resistor * p = head;
	while(p != NULL){
		cout << "  ";
		p->printResistor();
		p = p->next;
	}
}

int ResistorList::getNumRes(){
	int numRes = 0;
	Resistor * p = head;
	while(p != NULL){
		numRes++;
		p = p->next;
	}
	return numRes;
}

/*
class ResistorList{
	private Resistor * head;
	public ResistorList();
	public ~ResistorList();
	public void insertRes(string n, double r, int ePts[2]);
	public Resistor * findRes(string n);
	public bool deleteRes(Resistor * resPtr);
};
*/
