#ifndef UINT12_H	// macro gaurd
#define UINT12_H

#include "settings.h"

// used for storing two 12 bit integers
// first integer: all 8 bits of c1, and first 4 bits of c2
// second integer: last 4 bits of c2 and all 8 bits of c3

//		first 	     second	
// --------  ----|----  --------
//   c1			c2		   c3

class uInt12Node {
	unsigned char c1;
	unsigned char c2;
	unsigned char c3;
public:
	uInt12Node();

	// accessor functions
	unsigned char getC1();
	unsigned char getC2();
	unsigned char getC3();

	// mutator functions
	void setC1(unsigned char val);
	void setC2(unsigned char val);
	void setC3(unsigned char val);
};



class uInt12 {
	uInt12Node base;
	unsigned count;	// count stores number of elements saved in node base, i.e. 0, 1 or 2
public:
	uInt12();
	uInt12(uInt12Node x);

	int getCount();

	uInt12Node getBase();

	// get first element stored in node
	int getFirst();

	// get second element stored in node
	int getSecond();

	void setFirst(int val);
	void setSecond(int val);

	// insert new value into base
	void insert(int val);

	// clear all bits c1, c2 and c3 of uInt12Node base;
	void clear();
};


#endif		// end of macro gaurd