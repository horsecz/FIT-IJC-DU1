/**
 *	@file	bitset.h
 *	@author Dominik Horky, FIT
 *	@date	14.03.2020
 *	@brief  Definice maker a inline funkci pouzivanych v celem projektu.
 *	@note	Reseni IJC-DU1, priklad a) b)
 *	@details Prelozeno na gcc 9.2.1 (20200130, Manjaro Linux)
 */

#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include "error.h"

/**typ pouzivany pro indexy v bitset poli*/
typedef unsigned long bitset_index_t;
 /**datovy typ bitset pole*/
typedef unsigned long  bitset_t;

/**
 * @def bitset_create
 * @brief Makro, ktere vytvori bitset pole o pozadovane velikosti v bitech.
 * @details Pole je automaticky nulovane a na prvnim indexu je ulozen udaj o jeho velikosti. (pole bude automaticke/lokalni)
 * @param jmeno_pole nazev pole, ktere bude vytvoreno
 * @param velikost pozadovana velikost pole v bitech
 * @post Bude vytvoreno pole s pozadovanym nazvem a velikosti.
 * @note Pri spatne velikosti pole vypise static_assert - chybove hlaseni pri prekladu.
 * @note Pokud bude pole prilis velke (jako např. pro tento projekt), je nutne navysit kapacitu zasobniku!
 */
#define bitset_create(jmeno_pole,velikost) \
            static_assert((velikost > 0), "Pole nesmi mit nulovou nebo zapornou velikost!"); \
            bitset_t jmeno_pole[1 + (velikost/(sizeof(bitset_t)*CHAR_BIT)) + ((velikost%(sizeof(bitset_t)*CHAR_BIT)) > 0 ? 1 : 0)] = { velikost };

/**
 * @def bitset_alloc
 * @brief Vytvori a alokuje bitset pole s pozadovanym nazvem a velikosti v bitech.
 * @details Dale take je pole vynulovano a na prvnim indexu je udaj o velikosti pole. (pole bude dynamicke)
 * @param jmeno_pole nazev pole, ktere bude vytvoreno
 * @param velikost velikost pole v bitech
 * @pre Ocekava dostatek volne pameti (RAM) pro alokaci pole.
 * @post Bude vytvoreno (a alokovano) pole s pozadovanym nazvem a velikosti.
 * @note Pri spatne velikosti pole nebo chybne alokaci vypise chybovou hlasku a ukonci program.
 */
#define bitset_alloc(jmeno_pole,velikost) \
            assert(velikost > 0); \
            bitset_t *jmeno_pole = (bitset_t *)calloc(1 + (velikost/(sizeof(bitset_t)*CHAR_BIT)) + ((velikost%(sizeof(bitset_t)*CHAR_BIT)) > 0 ? 1 : 0), CHAR_BIT); \
            if (jmeno_pole == NULL) \
                error_exit("bitset_alloc: Chyba alokace paměti\n", velikost); \
            else \
                jmeno_pole[0] = velikost;

/**
 * @def bit_set
 * @brief Nastavi zadany bit cisla na pozadovanou hodnotu 0 nebo 1.
 * @details Pouzito pri dekodovani PPM obrazku v modulu steg-decode.
 * @param number cislo, kteremu se nastavi/zmeni pozadovany bit
 * @param index cozice bitu, ktery bude nastaven
 * @param value nova hodnota bitu na indexu
 * @post Cislo 'number' bude mit na pozici 'index' nastavenou hodnotu 'value' - 0 nebo 1.
 * @note Pokud bude index mimo rozsah, vypise chybovou hlasku.
 * @see steg-decode.c
 */
#define bit_set(number,index,value) \
            if ((bitset_index_t) index < sizeof(number)*CHAR_BIT) { \
                if (value) \
                    number[1 + (index/(sizeof(number[0])*CHAR_BIT))] |= (unsigned long) (1UL << (index%(sizeof(number[0])*CHAR_BIT))); \
                 else \
                    number[1 + (index/(sizeof(number[0])*CHAR_BIT))] &= (unsigned long) ~( 1UL << (index%(sizeof(number[0])*CHAR_BIT))); } \
            else \
                error_exit("bit_set: Index %lu mimo rozsah 0..%lu\n", (unsigned long) index, (unsigned long) (sizeof(number)*CHAR_BIT));

/**
 * @def bit_get
 * @brief Zjisti a "vrati" hodnotu bitu v cisle na pozadovane pozici.
 * @details Pouzivane v modulu steg-decode.
 * @param number cislo
 * @param index bit, jehoz hodnotu chci zjistit
 * @post "Vrati" hodnoti bitu na pozici 'index' cisla 'number'.
 * @note Pokud je index mimo rozsah cisla, vypise chybove hlaseni.
 * @see steg-decode.c
 */
#define bit_get(number,index) \
            (((bitset_index_t) index < sizeof(number)*CHAR_BIT) ? (number & (1 << (index%(sizeof(number)*CHAR_BIT))) ) : (error_exit("bit_get: Index %lu mimo rozsah 0..%lu\n", (unsigned long) index, (unsigned long) sizeof(number)*CHAR_BIT), 0))

#ifdef USE_INLINE

inline void bitset_free(bitset_t* jmeno_pole) {
    free(jmeno_pole);
}

inline bitset_t bitset_size(bitset_t* jmeno_pole) {
    return jmeno_pole[0];
}

inline bitset_t bitset_getbit(bitset_t* jmeno_pole, bitset_index_t index) {
    return (((bitset_index_t) index < (unsigned long) bitset_size(jmeno_pole)) ? jmeno_pole[1 + (index/(sizeof(*jmeno_pole)*CHAR_BIT))] & ((bitset_t) 1 << (index%(sizeof(*jmeno_pole)*CHAR_BIT))) : (error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu\n", (unsigned long) index, (unsigned long) bitset_size(jmeno_pole)), (bitset_t) 0));
}

inline void bitset_setbit(bitset_t* jmeno_pole, bitset_index_t index, int vyraz) {
    if ((bitset_index_t) index < bitset_size(jmeno_pole)) {
        if (vyraz)
            jmeno_pole[1 + (index/(sizeof(*jmeno_pole)*CHAR_BIT))] |= ((bitset_t) 1 << (index%(sizeof(*jmeno_pole)*CHAR_BIT)));
        else
            jmeno_pole[1 + (index/(sizeof(*jmeno_pole)*CHAR_BIT))] &= ~((bitset_t) 1 << (index%(sizeof(*jmeno_pole)*CHAR_BIT)));
    }
    else
        error_exit("bitset_setbit: Index %lu mimo rozsah 0..%lu\n", (unsigned long) index, (unsigned long) bitset_size(jmeno_pole));
}

#else

/**
 * @def bitset_free
 * @brief Uvolni alokovane pole z pameti.
 * @param jmeno_pole nazev pole, ktere ma byt uvolneno
 * @pre Ocekava existujici a alokovane pole.
 * @post Uvolnene pole z pameti.
 * @note Pri vytvareni 'primes-i' se pouzije inline funkce shodneho nazvu a parametru (vraci void).
 */
#define bitset_free(jmeno_pole) \
            free(jmeno_pole);

/**
 * @def bitset_size
 * @brief Zjisti velikost bitset pole.
 * @param jmeno_pole nazev pole
 * @pre Ocekava existujici pole
 * @post "Vrati" velikost pole v bitech (je ulozena na prvnim indexu).
 * @note Pri vytvareni 'primes-i' se pouzije inline funkce shodneho nazvu a parametru (vraci bitset_t).
 */
#define bitset_size(jmeno_pole) \
            jmeno_pole[0]

/**
 * @def bitset_getbit
 * @brief Zjisti hodnotu bitu na zadanem indexu.
 * @param jmeno_pole nazev pole
 * @param index index v bitset poli
 * @pre Ocekava existujici pole a index v rozsahu velikosti pole.
 * @post "Vrati" hodnotu bitu.
 * @note Pokud je index mimo rozsah pole, vypise chybovou hlasku.
 * @note Pri vytvareni 'primes-i' se pouzije inline funkce shodneho nazvu a parametru (vraci bitset_t).
 */
#define bitset_getbit(jmeno_pole,index) \
            (((bitset_index_t) index < (unsigned long) bitset_size(jmeno_pole)) ? jmeno_pole[1 + (index/(sizeof(*jmeno_pole)*CHAR_BIT))] & ((bitset_t) 1 << (index%(sizeof(*jmeno_pole)*CHAR_BIT))) : (error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu\n", (unsigned long) index, (unsigned long) bitset_size(jmeno_pole)), (unsigned long) 0))

/**
 * @def bitset_setbit
 * @brief Nastavi hodnoti bitu na zadanem indexu.
 * @param jmeno_pole nazev pole
 * @param index pozice-index v poli
 * @param vyraz hodnota, ktera ma byt nastavena
 * @pre Ocekava existujici pole a index v rozsahu pole.
 * @post Nastavi hodnotu bitu na pozadovanou hodnotu (0 nebo 1).
 * @note Pokud je index mimo rozsah pole, vypise chybove hlaseni.
 * @note Pri vytvareni 'primes-i' se pouzije inline funkce shodneho nazvu a parametru (vraci void).
 */
#define bitset_setbit(jmeno_pole,index,vyraz) \
            if ((bitset_index_t) index < bitset_size(jmeno_pole)) { \
                if (vyraz) \
                    jmeno_pole[1 + (index/(sizeof(*jmeno_pole)*CHAR_BIT))] |= ((bitset_t) 1 << (index%(sizeof(*jmeno_pole)*CHAR_BIT))); \
                else \
                    jmeno_pole[1 + (index/(sizeof(*jmeno_pole)*CHAR_BIT))] &= ~((bitset_t) 1 << (index%(sizeof(*jmeno_pole)*CHAR_BIT))); \
            } \
            else \
                error_exit("bitset_setbit: Index %lu mimo rozsah 0..%lu\n", (unsigned long) index, (unsigned long) bitset_size(jmeno_pole));

#endif