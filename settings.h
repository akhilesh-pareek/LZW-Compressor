#ifndef SETTINGS_H		// macro gaurd
#define SETTINGS_H

// settings file for the compressor and decompressor can be changed from here

// **IMPORTANT** : char by default can be signed or unsigned, machine dependent
// explicity use unsigned char for range [0...255]

// the range of character value used (default: 256, for 8-bit unsigned char)
#define MAXCHARS 256

/* table size for storing symbols. (default: 4092, for 12-bit integer)
   The table size depends on the number of bits in the integer used for representing symbols
*/
// a 12 bit integer is used, dont change this value
#define TABLE_SIZE 4096


// size of buffer used for reading and writing
// the buffer is used to minimize read and write operations on the file stored on disk
// keep buffer size below 100000
#define BUFF_SIZE 8192

// when number of nodes in trie MAX_NODES, then the trie must be emptied
// set values according to memory requirements
// each TrieNode takes 1032 bytes, so for less than 256 MB, use MAX_NODES = 2 ^ 16
#define MAX_NODES (1u << 18)


// iter value, is used to determine whether the symbol stored is relevant in current context
#define MAX_ITER (1u << 30)

// to avoid multiple definitions of symbolic constant "NULL"
#ifndef NULL
#define NULL 0
#endif

#endif		// end of macro gaurd
