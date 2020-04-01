/**
 *	@file	steg-decode.c
 *	@author Dominik Horky, FIT
 *	@date	14.03.2020
 *	@brief  'Hlavni' modul pro dekodovani zpravy z PPM souboru.
 *	@note	Reseni IJC-DU1, priklad b)
 *	@details Prelozeno na gcc 9.2.1 (20200130, Manjaro Linux)
 */

#include "ppm.h"
#include <stdio.h>

/**
 * @brief Main zde zajistuje vytvoreni pole, dekodovani a vystup zpravy na stdout.
 * @param argc pocet argumentu
 * @param argv argumenty programu - program akceptuje jediny argument -> nazev *.ppm souboru pro dekodovani
 * @note Program si alokuje pamet i pro pole, kam uklada samotnou zpravu. Je to proto, aby pri chybnem formatu\n
 *      retezce se nevypsalo neco, co nedava smysl (napr. retezec bez koncove nuly), ale jen chybova hlaska.\n
 *      Pokud se alokace pameti pro toto pole nezdari, program vypisuje vsechny znaky rovnou a pote vypise hlasku.
 * @return Vraci 0 pokud vse probehlo v poradku nebo nenulovou hodnotu pokud nastala chyba.
 */
int main(int argc, char** argv) {
    if (argc != 2) {
        error_exit("Spatny pocet argumentu.\nPouziti: ./steg-decode <soubor>\n");
    }

    // vytvori a nacte data ze souboru do struktury
    struct ppm* picture = ppm_read(argv[1]);
    if (picture == NULL) {
        return 1;
    }

    // vytvori pole a provede na nem eratosthenovo sito
    bitset_alloc(data, picture->xsize*picture->ysize*3);
    Eratosthenes(data);
    int primes_count = countPrimes(data);

    // vytvori se potrebne promenne a dekoduje se zprava
    unsigned char character[2] = { 0 };
    unsigned long bit_pos = 0;
    int count = 0;
    char *message = (char*) malloc(primes_count); // alokuje pole o velikosti celkoveho poctu prvocisel v poli
    for (bitset_index_t i = START_PRIME; i < bitset_size(data); i++) {
        if (!(bitset_getbit(data, i))) {
            bit_set(character, bit_pos, (picture->data[i] & 1));
            bit_pos++;
        }
        if (bit_pos == CHAR_BIT) {
            if (character[1] == 0 || character[1] == '\0') {
                break;
            }
            // pokud se nepodari alokace pameti pro zpravu - zprava se rovnou vypise
            if (message == NULL)
                fprintf(stdout, "%c", character[1]);
            else
                message[count++] = character[1];

            bit_pos = 0;
        }
    }

    bitset_free(data);
    ppm_free(picture);
    // pokud cyklus skonci a posledni nacteny znak neni konec retezce => chyba
    if (character[1] != '\0') {
        if (message == NULL) {
            fprintf(stdout, "\n");
            error_exit("Invalidni data - retezec (zprava) neni radne ukoncena!\n (Chybna alokace pameti dale zpusobila "
                       "jeji vytisteni i pres chybny format)\n");
        }
        free(message);
        error_exit("Invalidni data - retezec (zprava) neni radne ukoncena!\n");
    }

    // pokud se alokace nezdarila, nic se uvolnovat z pameti nebude
    if (message == NULL) {
        fprintf(stdout, "\n");
    } else {
        fprintf(stdout, "%s\n", message);
        free(message);
    }
    return 0;
}