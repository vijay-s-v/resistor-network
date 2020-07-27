#include "Resistor.h"

Resistor::Resistor(string n, double r, int ePts[]){
	name = n;
	resistance = r;
	endPts[0] = ePts[0];
	endPts[1] = ePts[1];
	next = NULL;
}

Resistor::~Resistor(){
	delete next;
}

void Resistor::printResistor(){
	cout << setw(20) << left << name << " " << setw(8) << right << setprecision(2) << resistance << " Ohms " << endPts[0] << " -> " << endPts[1] << endl;
}