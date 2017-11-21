#include "uInt12.h"
#include "settings.h"


// definitions for uInt12Node
uInt12Node::uInt12Node() {
	c1 = c2 = c3 = 0;
}

unsigned char uInt12Node::getC1() {
	return c1;
}

unsigned char uInt12Node::getC2() {
	return c2;
}   

unsigned char uInt12Node::getC3() {
	return c3;
}

void uInt12Node::setC1(unsigned char val) {
	c1 = val;
}

void uInt12Node::setC2(unsigned char val) {
	c2 = val;
}

void uInt12Node::setC3(unsigned char val) {
	c3 = val;
}


// definitions for uInt12
uInt12::uInt12() {
	count = 0;
	base = uInt12Node();	// initially set all bits to 0
}

uInt12::uInt12(uInt12Node x) {
	count = 2;
	base = x;
}

uInt12Node uInt12::getBase() {
	return base;
}

int uInt12::getCount() {
	return count;
}

// all char values are typecasted into integers for arithmetic, so need for explicit conversion
int uInt12::getFirst() {
	unsigned char c1 = base.getC1();
	unsigned char c2 = base.getC2();
	return (c1 << 4) + (c2 >> 4);
}

int uInt12::getSecond() {
	unsigned char c2 = base.getC2();
	unsigned char c3 = base.getC3();
	return ((((1 << 4) - 1) & c2) << 8) + c3;
}	

void uInt12::setFirst(int val) {
	unsigned char c1 = base.getC1(); 
	unsigned char c2 = base.getC2();
	c1 = val >> 4;
	c2 = ((val & ((1 << 4) - 1)) << 4) | (c2 & ((1 << 4) - 1));
	base.setC1(c1);
	base.setC2(c2);
}

void uInt12::setSecond(int val) {
	unsigned char c2 = base.getC2();
	unsigned char c3 = base.getC3(); 
	c2 = (c2 & (~((1 << 4) - 1))) | (val >> 8);
	c3 = (val & (1 << 8) - 1);
	base.setC2(c2);
	base.setC3(c3);
}

// assuming that, val < (1 << 12)
void uInt12::insert(int val) {
	if (count > 1)
		return ;
	if (count == 0)
		setFirst(val);
	else if (count == 1)
		setSecond(val);
	count++;
} 

void uInt12::clear() {
	count = 0;
	base.setC1(0);
	base.setC2(0);
	base.setC3(0);
}
