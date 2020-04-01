/**
 *	@file	ppm.h
 *	@author Dominik Horky, FIT
 *	@date	16.03.2020
 *	@brief  Hlavickovy soubor modulu ppm.c
 *	@note	Reseni IJC-DU1, priklad b)
 *	@details Prelozeno na gcc 9.2.1 (20200130, Manjaro Linux)
 */

#include "error.h"
#include "eratosthenes.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/**maximalni vyska obrazku*/
#define MAX_PPM_SIZE_X 8000
/**maximalni sirka obrazku*/
#define MAX_PPM_SIZE_Y 8000

/**implementacni limit - maximalni velikost obrazku (objem dat)*/
#define MAX_PPM_SIZE MAX_PPM_SIZE_X*MAX_PPM_SIZE_Y*3

/**pocatecni prvocislo, od ktereho zacina zprava (viz. zadani)*/
#define START_PRIME 23

/**
 * @struct ppm
 * @brief Struktura pro PPM soubor.
 * @details Spravny (akceptovany) format PPM souboru:\n\n
 *  "P6" <ws>+\n
 *  <xsizetxt> <ws>+ <ysizetxt> <ws>+\n
 *  "255" <ws>\n
 *  <binární data, 3*xsize*ysize bajtů RGB>\n
 *  <EOF>
 */
struct ppm {
    unsigned xsize; /**<vyska obrazku (dat)*/
    unsigned ysize; /**<sirka obrazku (dat)*/
    char data[]; /**<pole pro ulozeni binarnich dat - obrazku*/
};

/**
 * @brief Precte PPM soubor, zkontroluje spravnost dat a nahraje data do struktury.
 * @param filename nazev souboru (*.ppm)
 * @post Bude pripravena struktura s validnimi daty pro dekodovani.
 * @return Vrati strukturu s nahranymi daty a udaji o velikosti nebo NULL v pripade chyby pri otevreni souboru.
 */
struct ppm * ppm_read(const char * filename);

/**
 * @brief Uvolni strukturu z pameti.
 * @param p nazev struktury
 * @post Struktura bude uvolnena z pameti.
 */
void ppm_free(struct ppm *p);

/**
 * @brief Vypocita pocet vsech prvocisel v bitset poli.
 * @param array bitset pole
 * @pre Ocekava bitset pole, na kterem bylo provedeno Eratosthenovo sito.
 * @return Pocet prvocisel v poli.
 */
int countPrimes(bitset_t *array);

/**
 * @brief Funkce zajistuje zapis do souboru, overeni spravneho poctu znaku (a pripadny vypis).
 * @param filename nazev souboru, do ktereho se bude zapisovat
 * @param decoded bitset pole zpravy, ktera je z charu rozlozena na bity - pokud je NULL, program jen vypise\n
 *  maximalni pocet znaku, co se lze zakodovat do souboru
 * @param picture struktura obsahujici data souboru, ktery bude zmenen
 * @return Vraci 0 pokud vse probehlo v poradku nebo nenulovou hodnotu pokud nastala chyba.
 */
int ppm_write(const char* filename, bitset_t* decoded, struct ppm* picture);