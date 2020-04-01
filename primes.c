/**
 *	@file	primes.c
 *	@author Dominik Horky, FIT
 *	@date	14.03.2020
 *	@brief  'Hlavni' modul, ktery se stara o vytvoreni bitset pole, aplikace Eratosthenova
 *	        sita na pole a nasledny vystup poslednich 10 prvocisel.
 *	@note	Reseni IJC-DU1, priklad a)
 *	@details Prelozeno na gcc 9.2.1 (20200130, Manjaro Linux)
 */

#include <time.h>
#include <stdio.h>
#include "eratosthenes.h"

/**maximalni velikost bitset pole a zaroven hranice, pokud se pocitaji prvocisla*/
#define BITSET_SIZE 5*100*1000*1000
/**pocet poslednich prvocisel, ktere se vytisknou na stdout (od konce)*/
#define PRINT_LAST_NUMBERS 10

/**pokud bude definovano, vypise se pred ukoncenim programu cas algoritmu*/
#if 1
#define PRINT_TIME
#endif

/**pokud bude definovano, vytvori a pouzije se dynamicke (bitset) pole misto lokalniho*/
#if 0
#define ARRAY_DYNAMIC
#endif

/**
 * @brief Main zajistuje vytvoreni pole a vystup pozadovaneho poctu prvocisel na stdout.
 * @return Vraci 0 pokud vse probehlo v poradku nebo nenulovou hodnotu pokud nastala chyba.
 */
int main(void) {
    // vytvori pole a aplikuje na nej sito
#ifdef ARRAY_DYNAMIC
    bitset_alloc(pole, BITSET_SIZE);
#else
    bitset_create(pole, BITSET_SIZE);
#endif
    // provede algoritmus eratosthenova sita nad polem
    Eratosthenes(pole);

    // vystup poslednich PRIME_LAST_NUMBERS prvocisel na stdout
    int prime_counter = 0;
    for (bitset_index_t i = bitset_size(pole)-1;; i--) {
        if (!(bitset_getbit(pole, i))) { // prohleda pole od konce
            prime_counter++;
        }
        if (prime_counter >= PRINT_LAST_NUMBERS || !i) {  // nasel jsem pozadovany pocet prvocisel nebo jsem na konci
            for (bitset_index_t j = i; j < bitset_size(pole); j++) {
                if (!(bitset_getbit(pole, j))) // vystup na stdout
                    fprintf(stdout, "%lu\n", j);
            }
            break;
        }
    }

#ifdef ARRAY_DYNAMIC
    bitset_free(pole);
#endif

#ifdef PRINT_TIME
    fprintf(stderr, "\nTime=%.3g\n", (double)(clock())/CLOCKS_PER_SEC);
#endif

    return 0;
}  
