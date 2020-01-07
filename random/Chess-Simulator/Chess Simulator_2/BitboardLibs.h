#ifndef __BITBOARDLIBS
#define __BITBOARDLIBS

#include <iostream>
#include <string>

using namespace std;

uint64_t flipVertical(uint64_t x);
void renderBitmask(uint64_t input);

struct _pawn_movement_w {
	uint64_t squares[64];

	_pawn_movement_w() {
		fill_n(squares, 64, uint64_t(0));
		// First rank cannot be moved to
		// Second rank allows +1 and +2 push movement
		for(int d=0;d<8;d++) {
			int index = d + 8*1;
			squares[index] |= uint64_t(1)<<(index + 8*1);
			squares[index] |= uint64_t(1)<<(index + 8*2);
		}
		// The next 5 ranks allow +1 push movement
		for(int index=8*2;index<8*8 - 8*1;index++)
			squares[index] |= uint64_t(1)<<(index + 8*1);
		// The last rank has no movement positions for pawn
	}
}pawn_movement_w;

struct _pawn_movement_b {
	uint64_t squares[64];

	_pawn_movement_b() {
		for(int d=0;d<64;d++) {
			// Will convert to big endianness
			squares[d] = flipVertical(pawn_movement_w.squares[d]);
			// Will remap values to correct table
		}
	}
}pawn_movement_b;

struct _pawn_attack_normal_w {
	uint64_t squares[64];

	_pawn_attack_normal_w() {
		fill_n(squares, 64, uint64_t(0));
		// First rank cannot be attacked
		for(int d=1;d<7;d++) {
			for(int dd=0;dd<8;dd++) {
				int index = d*8 + dd;
				bool ul = true, ur = true;
				// File 'a' in ranks 2-7 can attack up-right
				// File 'h' in ranks 2-7 can attack up-left
				// All others in ranks 2-7 can attack up-left and up-right
				if(dd == 0)ul = false;
				if(dd == 7)ur = false;
				if(ur)squares[index] |= uint64_t(1)<<(index + 8*1+1);
				if(ul)squares[index] |= uint64_t(1)<<(index + 8*1-1);
			}
		}
		// Last rank cannot be attacked upon since pawn must promote
	}
}pawn_attack_normal_w;

struct _pawn_attack_enPassant_w {
	uint64_t squares[64];

	_pawn_attack_enPassant_w() {
		fill_n(squares, 64, uint64_t(0));
		// First rank cannot be attacked
		for(int d=0;d<8;d++) {
			int index = 4*8 + d;
			bool ul = true, ur = true;
			// File 'a' in rank 5 can attack up-right
			// File 'h' in rank 5 can attack up-left
			// All others in rank 5 can attack up-left and up-right
			if(d == 0)ul = false;
			if(d == 7)ur = false;
			if(ur)squares[index] |= uint64_t(1)<<(index + 8*1+1);
			if(ul)squares[index] |= uint64_t(1)<<(index + 8*1-1);
		}
		// Last rank cannot be attacked upon since pawn must promote
	}
}pawn_attack_enPassant_w;

#endif
