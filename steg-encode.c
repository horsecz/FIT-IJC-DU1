/**
 *	@file	steg-encode.c
 *	@author Dominik Horky, FIT
 *	@date	16.03.2020
 *	@brief  'Hlavni' modul pro zakodovani zpravy do PPM souboru.
 *	@note	Reseni IJC-DU1, priklad b) - dobrovolne vypracovani steg-encode
 *	@details Prelozeno na gcc 9.2.1 (20200130, Manjaro Linux)
 */

#include "ppm.h"
#include <stdio.h>
#include <string.h>


/**
 * @brief Ulozi zpravu do pole ze stdin
 * @param message pole, do ktereho se zprava nahraje
 * @param struc nahrana struktura dat z PPM souboru
 */
void saveMessage(char *message, struct ppm* struc) {
    int ch = fgetc(stdin);
    for (unsigned i = 0; ch != EOF;) {
        if (i >= sizeof(char) * struc->xsize * struc->ysize * 3) // velikost zpravy presahne velikost dat
            error_exit("Zadavana zprava je prilis velka (vetsi nez vstupni data).\n");
        message[i++] = ch;
        ch = fgetc(stdin);
    }
}

/**
 * @brief Rozlozi zpravu na bity a pripravi ji na zakodovani do obrazku.
 * @details Kazdy jednotlivy znak zpravy rozlozi na bity (pocet bitu je CHAR_BIT) a ulozi do bitset pole.
 * @param message pole, ve kterem je ulozena zprava
 * @param msgsize velikost zpravy
 * @param decoded bitset pole, do ktereho se nahraji rozlozene znaky zpravy
 */
void messageToBits(char *message, unsigned msgsize, bitset_t *decoded) {
    unsigned bit_val;
    int k = 0;
    for (unsigned i = 0; i < msgsize; i++) {
        for (int j = 0; j < CHAR_BIT; j++) {
            bit_val = bit_get(message[i], j);
            bitset_setbit(decoded, k, bit_val);
            k++;
        }
    }
}

/**
 * @brief Main zde zajistuje vytvoreni pole a zakodovani zpravy do PPM. Zprava se zadava na stdin.
 * @param argc pocet argumentu
 * @param argv argumenty programu - program akceptuje dve varianty argumentu:\n
 *  [nazev souboru] -> jediny argument -> zakoduje zpravu do zadaneho souboru\n
 *  --msgsize [nazev souboru] -> dva argumenty -> pouze zjisti maximalni pocet znaku, ktere se daji zakodovat\n
 * @return Vraci 0 pokud vse probehlo v poradku nebo nenulovou hodnotu pokud nastala chyba.
 */
int main(int argc, char **argv) {
    int res;
    // prepise (pokud je to mozne) PPM data a zakoduje zpravu
    if (argc == 2 && strcmp(argv[1], "--msgsize")) {
        // precte obsah souboru a nahraje do struktury
        struct ppm *struc = ppm_read(argv[1]);
        if (struc == NULL)
            return 1;

        // zadani zpravy ze stdin
        fprintf(stdout, "Zadejte zpravu: ");
        char *message = (char *) malloc(sizeof(char)*struc->xsize*struc->ysize*3);
        saveMessage(message, struc);
        fprintf(stdout, "\n");

        // vytvori pole pro zakodovanou zpravu a rozlozi znaky na bity
        unsigned msgsize = strlen(message);
        bitset_alloc(decoded, (msgsize + 1) * CHAR_BIT);
        messageToBits(message, msgsize, decoded);

        // prepise data
        res = ppm_write(argv[1], decoded, struc);
        if (res) {
            error_exit("Chyba v zapisu\n");
        }

        // konec
        printf("Zprava byla uspesne zakodovana.\n");
        bitset_free(decoded);
    }
    // pouze vypise maximalni pocet znaku co je mozne zapsat do souboru
    else if (argc == 3 && !strcmp(argv[1], "--msgsize")) {
        struct ppm *struc = ppm_read(argv[2]);
        if (struc == NULL)
            return 1;

        res = ppm_write(argv[2], NULL, struc);
    }
    else
        error_exit("Spatny pocet argumentu nebo spatny syntax.\nPouziti:\n./steg-encode <soubor>\n./steg-encode --msgsize <soubor>\n");

    return 0;
}