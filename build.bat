echo "Building compress.exe"
g++ -c Trie.cpp Compressor.cpp compress.cpp uInt12.cpp
g++ Trie.o Compressor.o compress.o uInt12.o -o compress
echo "Build Finished"
