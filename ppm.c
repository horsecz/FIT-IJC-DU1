/**
 *	@file	ppm.c
 *	@author Dominik Horky, FIT
 *	@date	19.03.2020
 *	@brief  Modul definujici funkce pro zpracovani PPM souboru.
 *	@note	Reseni IJC-DU1, priklad b) + dobrovolne vypracovani ppm_write
 *	@details Prelozeno na gcc 9.2.1 (20200130, Manjaro Linux)
 */

#include "ppm.h"
#include <stdio.h>
#include <errno.h>

struct ppm* ppm_read(const char* filename) {
    FILE *ppm = NULL;
    int xsize = -1;
    int ysize = -1;
    int colors = -1;
    char firstLine[4] = { 0 };

    // otevre soubor .ppm a v pripade chyby vypise hlaseni (z errno.h)
    ppm = fopen(filename, "r");
    if (ppm == NULL) {
        (warning_msg("Otevreni souboru se nezdarilo - "), perror(""));
        return NULL;
    }

    // zkontroluje format
    fscanf(ppm, "%c %c \n%d %d \n%d \n", &firstLine[0], &firstLine[1], &xsize, &ysize, &colors);
    if (strcmp(firstLine, "P6") || xsize < 0 || ysize < 0 || colors != 255) {
        fclose(ppm);
        warning_msg("Spatny format PPM souboru\n");
        return NULL;
    }
    if (xsize*ysize*3 > MAX_PPM_SIZE) {
        fclose(ppm);
        warning_msg("Implementacni limit maximalni velikosti obrazku byl prekrocen. (%d x %d x 3)\n", MAX_PPM_SIZE_X, MAX_PPM_SIZE_Y);
        return NULL;
    }

    // vytvori novou strukturu
    struct ppm* new_ppm = malloc(sizeof(*new_ppm) + (sizeof(char))*xsize*ysize*3+2);
    if (new_ppm == NULL) {
        fclose(ppm);
        warning_msg("Alokace struktury se nezdarila\n");
        return NULL;
    }

    new_ppm->xsize = xsize;
    new_ppm->ysize = ysize;

    // precte soubor a ulozi jeho obsah do pole ve strukture, zkontroluje spravnost dat
    int read = fread(new_ppm->data, 1, xsize*ysize*3*sizeof(char)+2, ppm);
    if (!read || (ferror(ppm) && feof(ppm)) || read != xsize*ysize*3 || !feof(ppm)) {
        fclose(ppm);
        ppm_free(new_ppm);
        warning_msg("Cteni ze souboru se nezdarilo (chybna data)\n");
        return NULL;
    }

    fclose(ppm);
    return new_ppm;
}

void ppm_free(struct ppm *p) {
    free(p);
}

int countPrimes(bitset_t *array) {
    int primes_count = 0;
    for (bitset_t i = START_PRIME; i < bitset_size(array); i += 2) {
        if (!(bitset_getbit(array, i)))
            primes_count++;
    }
    return primes_count;
}

int ppm_write(const char* filename, bitset_t* decoded, struct ppm* picture) {
    FILE *ppm = NULL;
    int xsize = -1;
    int ysize = -1;
    int colors = -1;
    char firstLine[4] = { 0 };

    // otevre soubor .ppm a v pripade chyby vypise hlaseni (z errno.h)
    ppm = fopen(filename, "r+b");
    if (ppm == NULL) {
        (warning_msg("Otevreni souboru se nezdarilo - "), perror(""));
        return 1;
    }

    // nacte pocatecni data o PPM (potreba pro xsize, ysize + posun na zacatek dat)
    fscanf(ppm, "%c %c \n%d %d \n%d \n", &firstLine[0], &firstLine[1], &xsize, &ysize, &colors);

    bitset_alloc(data, xsize*ysize*3);
    Eratosthenes(data);

    // inicializuje potrebne promenne
    char bit;
    char bit_data[2] = { 0 };
    int start_pos = ftell(ppm);
    unsigned int j = 0;
    unsigned primes = countPrimes(data);

    // vypis max. poctu znaku a ukonceni
    if (decoded == NULL) {
        fprintf(stdout, "Maximalni velikost zpravy (poc. znaku) v tomto souboru je: %d\n", primes/8);
        fclose(ppm);
        return 0;
    }

    // zkontroluje zda je obrazek dostatecne velky pro zakodovani zpravy
    if (bitset_size(decoded)/CHAR_BIT > primes/CHAR_BIT)
        error_exit("Zprava je prilis velka pro tento soubor!\n\nPocet znaku vasi zpravy: %d\nMax. pocet znaku: %d\n", bitset_size(decoded)/CHAR_BIT, primes/CHAR_BIT);

    // prepisovani dat v souboru
    for (bitset_index_t i = START_PRIME; j < bitset_size((decoded)); i++) {
        if (!(bitset_getbit(data, i))) {
            bit = (bitset_getbit(decoded, j) > 0 ? 1 : 0);
            bit_data[1] = picture->data[i];
            bit_set(bit_data, 0, bit);
            fseek(ppm, (unsigned) i+start_pos, SEEK_SET);
            fwrite(&bit_data[1], 1, 1, ppm);
            j++;
        }
    }

    bitset_free(data);
    ppm_free(picture);
    fclose(ppm);
    return 0;
}