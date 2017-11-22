#ifndef DECOMPRESSOR_H      // macro gaurd
#define DECOMPRESSOR_H

#include <string>
#include "SymbolTable.h"
#include "settings.h"

class Decompressor {
	SymbolTable table;

public:
	void createTargetFileName(char *targetFileName, char *sourceFileName);
	void writeToFile(FILE *targetFile, int index);
	void decompress(char *sourceFileName);
};

#endif      // end of macro gaurd
