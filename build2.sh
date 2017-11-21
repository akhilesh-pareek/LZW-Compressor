echo "Building Decompressor"
g++ -c decompress.cpp Decompressor.cpp SymbolTable.cpp uInt12.cpp
g++ decompress.o Decompressor.o SymbolTable.o uInt12.o -o decompress
echo "Build Finished"