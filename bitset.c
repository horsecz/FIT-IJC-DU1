/**
 *	@file	bitset.c
 *	@author Dominik Horky, FIT
 *	@date	13.03.2020
 *	@brief  Modul umoznujici externi pouziti inline funkci z 'bitset.h'.
 *	@note	Reseni IJC-DU1, priklad a) b)
 *	@details Prelozeno na gcc 9.2.1 (20200130, Manjaro Linux)
 */

#include "bitset.h"

#ifdef USE_INLINE

extern void bitset_free(bitset_t* jmeno_pole);
extern bitset_t bitset_size(bitset_t* jmeno_pole);
extern bitset_t bitset_getbit(bitset_t* jmeno_pole, bitset_index_t index);
extern void bitset_setbit(bitset_t* jmeno_pole, bitset_index_t index, int vyraz);

#endif