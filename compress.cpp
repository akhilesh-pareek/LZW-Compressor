#include <cstring>
#include <cstdio>
#include "Compressor.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        // raise exception
        printf("Invalid Arguments!\n");
        return 0;
    }
    Compressor obj;
    
    // pass the name of file to be compresed
    obj.compress(argv[1]);	// returns total characters read      
    return 0;   // SUCCESS
}