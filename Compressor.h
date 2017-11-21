#ifndef COMPRESSOR_H    // macro gaurd
#define COMPRESSOR_H

#include "Trie.h"
#include "settings.h"

// an object of Compressor class must be created for running compression procedure
class Compressor {
	
public:
    void createTargetFileName(char *targetFileName, char *sourceFileName);
    void createFormatString(char *format, int n);
    void compress(char *fileName);
};

#endif    // end of macro gaurd