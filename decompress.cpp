#include <cstdio>
#include "Decompressor.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Invalid Arguments!\n");
        return -1; 	// failure
    }
    
    Decompressor obj;
    // pass the name of file to be decompressed
    obj.decompress(argv[1]); 	// returns total symbols occured     
    return 0;   // success
}
