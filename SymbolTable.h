#ifndef SYMBOLTABLE_H		// macro gaurd
#define SYMBOLTABLE_H

#include "settings.h"

// we need only the last char and prevIndex to generate the pattern
// we need first char when we are adding a new node in the symbol table

// {first, last, prevIndex}

// e.g. for generation - "ana", where "a" - 97, "an" - 256, "ana" - 257
// then if we store 256 with "ana", 97 with "an" and -1 with "a", then we can obtain "ana"
// from index 257
					
// e.g. for inserting new node, "ab", where 97 - {"a", "a", -1} , 98 - {"b", "a", -1}
// to add "ab" in symbol table, we insert {"a", "b", 97} in the symbol table.

class SymbolTableNode {
	char first;	 
	char last;	
	int prevIndex;
public:
	SymbolTableNode();
	SymbolTableNode(char first, char last, int prevIndex);
	
	// accessor functions for private members
	char getFirst();
	char getLast();
	int getPrevIndex();
};


class SymbolTable {
	// TABLE_SIZE is defined in "settings.h"
	SymbolTableNode arr[TABLE_SIZE];		
	
	// the index where a new table node must be inserted
	int currIndex;		
public:
	SymbolTable();

	// accessor function to get value of currIndex
	int getCurrIndex();

	// modifier function for value of currIndex
	void setCurrIndex(int val);

	// accessor function to get a particular node from arr
	SymbolTableNode getNodeFromArr(int index);

	// to insert a new string pattern in the symbol table
	void insert(char first, char last, int prevIndex);

	// to again start filling from index = MAXCHARS
	void emptyTable();

};

#endif		// end of macro gaurd