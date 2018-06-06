// Sample input 1, in CPP.

#include <cassert>

long long GetNumberOfBlocks() {
	return 3LL;
}

long long GetBlockWeight(long long i) {
	switch ((int)i) {
	case 0: return 1LL;
	case 1: return 1LL;
	case 2: return 1LL;
	default: assert(0);
	}
	return 0;
}

//long long GetNumberOfBlocks() {
//	return 6LL;
//}
//
//long long GetBlockWeight(long long i) {
//	switch ((int)i) {
//	case 0: return 1LL;
//	case 1: return 2LL;
//	case 2: return 1LL;
//	case 3: return 4LL;
//	case 4: return 3LL;
//	case 5: return 1LL;
//	default: assert(0);
//	}
//	return 0;
//}
