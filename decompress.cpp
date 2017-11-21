#include <cstring>
#include <cstdio>
#include "Decompressor.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        // raise exception
        printf("Invalid Arguments!\n");
        return 0;
    }
    Decompressor obj;

    // pass the name of file to be decompressed
    obj.decompress(argv[1]); 	// returns total symbols occured     
    return 0;   // SUCCESS
}
