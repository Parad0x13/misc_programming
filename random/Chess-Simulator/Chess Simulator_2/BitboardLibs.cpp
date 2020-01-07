#include "BitboardLibs.h"
#include "libs.h"

// http://chessprogramming.wikispaces.com/Flipping+Mirroring+and+Rotating
uint64_t flipVertical(uint64_t x) {
	return
	( (x << 56) ) |
	( (x << 40) & uint64_t(0x00ff000000000000) ) |
	( (x << 24) & uint64_t(0x0000ff0000000000) ) |
	( (x <<  8) & uint64_t(0x000000ff00000000) ) |
	( (x >>  8) & uint64_t(0x00000000ff000000) ) |
	( (x >> 24) & uint64_t(0x0000000000ff0000) ) |
	( (x >> 40) & uint64_t(0x000000000000ff00) ) |
	( (x >> 56) );
}

void renderBitmask(uint64_t input) {
	for(int y=7;y>=0;y--) {
		for(int x=7;x>=0;x--) {
			int index = 8*y + x;
			cout << isBitSet(input, index);
		}
		cout << endl;
	}
}
