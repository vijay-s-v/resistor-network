#ifndef RESISTOR_H
#define RESISTOR_H
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

class Resistor{
	friend class ResistorList;
	friend class NodeList;
	private: 
		string name;
		int endPts[2];
		double resistance;
		Resistor * next;
	public:
		Resistor(string n, double r, int ePts[]);
		~Resistor();
		void printResistor();
	/*
	Don't need because ResistorList is a friend of Resistor
	public string getName();
	public double getResistance();
	public void setResistance(double r);
	*/
};

#endif
