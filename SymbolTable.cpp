#include <string>
#include "SymbolTable.h"
#include "settings.h"

// definitions for SymbolTableNode
SymbolTableNode::SymbolTableNode() {
	// default constructor for creating array of SymbolTableNode
}

SymbolTableNode::SymbolTableNode(char first, char last, int prevIndex) {
	this->first = first;
	this->last = last;
	this->prevIndex = prevIndex;
}

char SymbolTableNode::getFirst() {
	return first;
}

char SymbolTableNode::getLast() {
	return last;
}

int SymbolTableNode::getPrevIndex() {
	return prevIndex;
}


// definitions for SymbolTable
SymbolTable::SymbolTable() {
	for (int i=0; i<MAXCHARS; ++i)
		arr[i] = SymbolTableNode((char)i, (char)i, -1);
	
	// this is the index from which new patterns must start filling the table
	currIndex = MAXCHARS;	
}

int SymbolTable::getCurrIndex() {
	return currIndex;
}

void SymbolTable::setCurrIndex(int val) {
	currIndex = val;
}

SymbolTableNode SymbolTable::getNodeFromArr(int index) {
	return arr[index];
}

void SymbolTable::insert(char first, char last, int prevIndex) {
	arr[currIndex++] = SymbolTableNode(first, last, prevIndex);
}

void SymbolTable::emptyTable() {
	// now table will start filling again from MAXCHARS
	currIndex = MAXCHARS;	
}