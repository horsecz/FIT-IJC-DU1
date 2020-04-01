/**
 *	@file	eratosthenes.h
 *	@author Dominik Horky, FIT
 *	@date	13.03.2020
 *	@brief  Hlavickovy soubor k 'eratosthenes.c'
 *	@note   Reseni IJC-DU1, priklad a)
 *	@details Prelozeno na gcc 9.2.1 (20200130, Manjaro Linux)
 */

#include <math.h>
#include "bitset.h"

/**
 *  @brief  Implementace Eratosthenova sita nad bitset polem.
 *  @details Hodnoty na indexech, ktere nejsou prvocislem budou nastaveny na 1, ostatni na 0.
 *  @param  pole Pole, nad kterym se vykona algoritmus
 *  @pre Ocekava existujici a vynulovane bitset pole.
 *  @post Pole na indexech, ktere jsou prvocislo maji hodnotu 0, ostatni 1.
 */
void Eratosthenes(bitset_t* pole);