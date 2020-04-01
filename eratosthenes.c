/**
 *	@file	eratosthenes.c
 *	@author Dominik Horky, FIT
 *	@date	19.03.2020
 *	@brief  Modul obsahujici implementaci Eratosthenova sita v C nad bitset polem.
 *	@note	Reseni IJC-DU1, priklad a)
 *	@details Prelozeno na gcc 9.2.1 (20200130, Manjaro Linux)
 */

#include "eratosthenes.h"


void Eratosthenes(bitset_t* pole) {
    // 0, 1 nejsou prvocisla
    bitset_setbit(pole, 0, 1);
    bitset_setbit(pole, 1, 1);
    // vsechna suda cisla krome 2 nejsou prvocisla
    for (unsigned int n = 2; n*2  < bitset_size(pole); n++) {
        bitset_setbit(pole, n*2, 1);
    }
    // pracuje do druhe odmocniny velikosti pole
    for(unsigned int i = 3; i < sqrt(bitset_size(pole)); i += 2) {
        if (!(bitset_getbit(pole, i))) { // index je 0 (=> je prvocislo)
                // nastavi vsechny nasobky aktualniho indexu na 1
                for (unsigned int n = i*i; n < bitset_size(pole); n += i*2) {
                    if (!(bitset_getbit(pole, n))) { // pouze pokud se to neprovedlo uz driv (=> index je 0)
                        bitset_setbit(pole, n, 1);
                    }
                }
        }
    }
}