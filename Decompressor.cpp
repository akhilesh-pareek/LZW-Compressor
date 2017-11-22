#include <cstring>
#include <cstdio>
#include "Decompressor.h"
#include "uInt12.h"
#include "SymbolTable.h"
#include "settings.h"

void Decompressor::createTargetFileName(char *targetFileName, char *sourceFileName) {
    int n = strlen(sourceFileName);
    strcpy(targetFileName, sourceFileName);
    
    // sourceFileName ends with ".cmpr" then replace it with ".txt" for targetFileName
    // else append ".txt".
    int srcSize = strlen(sourceFileName);
    if (srcSize >= 5 && strcmp(sourceFileName + srcSize - 5, ".cmpr") == 0)
    	strcpy(targetFileName + srcSize - 5, ".txt");
    else
    	strcat(targetFileName, ".txt");
}

void Decompressor::decompress(char *sourceFileName) {
	// create a name for the target file
    char targetFileName[strlen(sourceFileName) + 5];
    createTargetFileName(targetFileName, sourceFileName);

    // open the source and target files
    FILE *sourceFile = fopen(sourceFileName, "rb");		// open as a binary file
    FILE *targetFile = fopen(targetFileName, "w");		// open as a text file

    /* decompression logic */ 
    // using SymbolTable data structure for O(1) insertion of string patterns, and O(pattern_size) generation 
    // string patterns represented by given index
	
	// access symbols in consecutive pairs of (curr, next);
	// curr is used to generate the string to be written in targetFile
	// curr and next, are used together to create the new pattern string to be inserted in the table
	
	
	// we need temp array because, two integers are stored together in uInt12
	// and we need to access numbers in pairs, i.e. curr and next number
	int temp[3];		// for storing values of integers in u1 and u2

	// the Symbol table for storing patterns
	// Symbol Table is added as a member function of Decompressor class, so that Decompressor::writeToFile() can also access it
	// SymbolTable table;
	
	uInt12Node u1, u2;
	// if file is not empty, start reading from it in pairs
	if (fread(&u1, sizeof(uInt12Node), 1, sourceFile) > 0) {
		
		// for managing uInt12Node u1 and u2
		uInt12 manager;

		while (fread(&u2, sizeof(uInt12Node), 1, sourceFile) > 0) {
			manager = uInt12(u1);
			temp[0] = manager.getFirst();
			temp[1] = manager.getSecond();
			manager = uInt12(u2);
			temp[2] = manager.getFirst();
			
			for (int i=0; i<2; ++i) {
				int curr = temp[i];
				int next = temp[i+1];

				// write string stored at index curr in table, to targetFile
				writeToFile(targetFile, curr);

				// the values to be inserted in the table node
				char first = table.getNodeFromArr(curr).getFirst();
				int prevIndex = curr;
				char last;
				
				// char last will be obtained according to following conditions
				// i.e if next < currSize of table, then it is first char of node stored at "next" in table
				if (next < table.getCurrIndex())
					last = table.getNodeFromArr(next).getFirst();

				// else, last is first char of node stored at "curr" in the table
				// because the string represented by "next" is added in the end of this iteration, so it is not present at this moment
				else 
					last = first;

				// insert the values in the table
				table.insert(first, last, prevIndex);

				// if the table is full, reinitialize it by setting table.currIndex to MAXCHARS
				if (table.getCurrIndex() == TABLE_SIZE)
					table.setCurrIndex(MAXCHARS);
			}
			u1 = u2;
		}

		manager = uInt12(u1);
		// finally write the remaining patterns to the file
		
		writeToFile(targetFile, manager.getFirst());

		// read count from "storeCount.temp" file
		// this is a temporary solution, will be fixed later
		int count;
		FILE *fptr = fopen("storeCount.temp", "r");
		fscanf(fptr, "%d", &count);

		// if there are two indexes stored in last uInt12
		// add the new pattern to symbol table and write the pattern corresponding to the second index in the target file
		if (count == 2) {
			int curr = manager.getFirst();
			int next = manager.getSecond();
			char first = table.getNodeFromArr(curr).getFirst();
			int prevIndex = curr;
			char last;
			if (next < table.getCurrIndex())
				last = table.getNodeFromArr(next).getFirst();
			else
				last = first;	
			table.insert(first, last, prevIndex);		
			writeToFile(targetFile, next);
		}
		fclose(fptr);
	}

	// close all files
	fclose(sourceFile);
	fclose(targetFile);
}

// utility function to write string at "index" in table, to targetFile
void Decompressor::writeToFile(FILE *targetFile, int index) {
	std::string res;
	while (index != -1) {
		res += table.getNodeFromArr(index).getLast();
		index = table.getNodeFromArr(index).getPrevIndex();
	}	
	// write the string to targetFile
	// the generated string is reverse of actual string
	for (int i=(int)res.size()-1; i>=0; --i) 
		fprintf(targetFile, "%c", res[i]);
}