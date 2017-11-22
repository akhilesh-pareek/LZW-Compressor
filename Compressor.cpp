#include <cstring>
#include <cstdio>
#include <string>
#include "Compressor.h"
#include "Trie.h"
#include "uInt12.h"
#include "settings.h"

void Compressor::createTargetFileName(char *targetFileName, char *sourceFileName) {
    int n = strlen(sourceFileName);
    strcpy(targetFileName, sourceFileName);

    // sourceFileName ends with ".txt" then replace it with ".cmpr" for targetFileName
    // else append ".cmpr".
    int srcSize = strlen(sourceFileName);
    if (srcSize >= 4 && strcmp(sourceFileName + srcSize - 4, ".txt") == 0)
        strcpy(targetFileName + srcSize - 4, ".cmpr");
    else
        strcat(targetFileName, ".cmpr");
}

void Compressor::createFormatString(char *format, int n) {
	format[0] = '%';
	int k = 1;
	// append the number to the format string
	do {
		format[k++] = (n % 10) + '0';
		n /= 10;
	}while (n > 0);

	// the number is obtained in reverse order, so reverse the string from index 1..k-1
	for (int l=1, r=k-1; l<r; ++l, --r) {
		char temp = format[l];
		format[l] = format[r];
		format[r] = temp;
	}

	// append "s" to the format string
	format[k] = '\0';
	strcat(format, "c%n");
}

void Compressor::compress(char *sourceFileName) {
    // create a name for the target file
    char targetFileName[strlen(sourceFileName) + 6];
    createTargetFileName(targetFileName, sourceFileName);

    // open the source and target files
    FILE *sourceFile = fopen(sourceFileName, "r");
    FILE *targetFile = fopen(targetFileName, "wb");     // open as a binary file

    //compression logic
    /* using Trie as a table for fast lookup O(pattern_size), instead of an array of strings */

    // iter is used to avoid emptying trie every time number of symbols == TABLE_SIZE
    // when iter becomes MAX_ITER, then we have to empty the trie
    //**// this is needed, otherwise if we dont empty the trie, then memory requirement may become too high
    unsigned iter = 0;
    unsigned numberOfTrieNodes = 1 + MAXCHARS;      // root node + its children nodes

    Trie trie;
    trie.initTrie(iter);

    // nextIndex will be the index of the next new pattern which is not in the trie
    // all patterns from 0..MAXCHARS-1 are in trie, so nextIndex = MAXCHARS
	int nextIndex = MAXCHARS;
    char ch;    // used for storing from text file
    TrieNode *curr = trie.getRoot();

    // this if condition for making sure the file isn't empty
    if (fscanf(sourceFile, "%c", &ch) > 0) {
        // initial pattern is just the starting character
        curr = curr->getChild(ch);

        // create the format string to be passed to fscanf()
        // e.g if BUFF_SIZE=8192, then format will be "%8192c%n" ,i.e read at max 8192 characters, %n is used to store number of char succesfully read
        char format[16];
        createFormatString(format, BUFF_SIZE);

        // create buffer for reading source file
        char scanBuffer[BUFF_SIZE];		
        
        // create buffer for writing into target file
        // uInt12Node is used for storing 12-bit integers
        uInt12Node printBuffer[BUFF_SIZE];      // the actual number of symbols in printBuffer will always be < BUFF_SIZE, dur to compression
        
        // stores size of scan and print buffers
        int scanBufferSize, printBufferSize;

        uInt12 manager;     // for managing uInt12Node

        while (fscanf(sourceFile, format, scanBuffer, &scanBufferSize) > 0) {	// read data in buffer
        	// initialize printBufferSize to 0, to fill from start
        	printBufferSize = 0;
        	for (int i=0; i<scanBufferSize; ++i) {
				ch = scanBuffer[i];        	
			
	          	// if the pattern was found, then curr will move to 'pattern + ch' in the trie
	            // else it will move to character ch in the trie, in case 'pattern + ch' does not exist in the trie
                // if the below condition is true, then we need to add new pattern to the trie
                // we have to either create a new node in the trie, or update a previous node with new iter value
	            if (curr->getChild(ch) == NULL || curr->getChild(ch)->getIter() != iter) {
	                
                    // store the symbol for current pattern in printBuffer
                    // the symbol is stored in the index entry of the node curr

                    // every pattern index is within [0, 4095], except root index which is -1
                    // root index is never needed to be stored
                    manager.insert(curr->getIndex());
                    if (manager.getCount() == 2) {
                        printBuffer[printBufferSize++] = manager.getBase();
                        manager.clear();
                    }          

                    // add the new pattern in trie
                    if (curr->getChild(ch) == NULL) {    // need to create a new node	
	                    curr->newChild(ch, nextIndex, iter);
                        numberOfTrieNodes++;
                    }
                    
                    else {   // this node is already present, just change the iter value
                        curr->getChild(ch)->setIter(iter);

                        // also set the index value, because the trie is changed, since iter values are different
                        // because the symbol that was stored during previous iter value may be different
                        curr->getChild(ch)->setIndex(nextIndex);
                    } 

                    curr = trie.getRoot();             // move back to root of trie
	                nextIndex++;
	            }
	            
	            // if nextIndex == TABLE_SIZE, i.e the maximum number of patterns, then reinitialize trie and nextIndex
	            // curr will now be at root of trie
	            // it may be possible that number of trie nodes become greater than MAX_NODES before nextIndex becomes TABLE_SIZE
                // we can afford not emptying trie because number of nodes will be (MAX_NODES + TABLE_SIZE), and TABLE_SIZE is very small compared to MAX_NODES
                if (nextIndex == TABLE_SIZE) {
                    iter++;

                    // emptying trie everytime is the bottelneck, so alternative is using variable iter
                    // empty trie only when iter == MAX_ITER or when the trie contains more than MAX_NODES nodes
                    if (iter == MAX_ITER || numberOfTrieNodes > MAX_NODES) {
                        
                        // if iter reaches max value, then make iter 0 again
                        if (iter == MAX_ITER)
                            iter = 0;

                        if (numberOfTrieNodes > MAX_NODES)
                            numberOfTrieNodes = 1 + MAXCHARS;   // root + all its children  

                        trie.emptyTrie();
                        trie.initTrie(iter);    // initialize all trieNodes with value of iter
                    }    
	               
                    else      // no need to delete the whole trie, just update the child of root with iter value
                        trie.updateRootChildIterValue(iter);    

                    curr = trie.getRoot();
                    nextIndex = MAXCHARS;
	            }

	            curr = curr->getChild(ch);        
	        }

	        // write the buffer data into the target file
	        fwrite(printBuffer, sizeof(uInt12Node), printBufferSize, targetFile);
	    }

	    // add the last pattern in trie
        manager.insert(curr->getIndex());
        uInt12Node data = manager.getBase();    // since l value is required by fwrite()
        fwrite(&data, sizeof(uInt12Node), 1, targetFile); 

        // temporary fix to store number of values in last uInt12Node   
        FILE *fptr = fopen("storeCount.temp", "w");
        fprintf(fptr, "%d", manager.getCount());
        fclose(fptr);

        trie.emptyTrie();   // deallocate heap memory used by trie
    }

    // close all files
    fclose(sourceFile);
    fclose(targetFile);
}

// Compression ratio increases as the length of text increases, as more patterns are added to the table.