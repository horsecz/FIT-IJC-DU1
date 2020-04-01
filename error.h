/**
 *	@file	error.h
 *	@author Dominik Horky, FIT
 *	@date	13.03.2020
 *	@brief  Hlavickovy soubor k modulu error.c
 *	@note	Reseni IJC-DU1, priklad b)
 *	@details Prelozeno na gcc 9.2.1 (20200130, Manjaro Linux)
 */

/**
 * @brief Vypise chybove hlaseni na stderr.
 * @details Chova se jako printf.
 * @param fmt fixni zacatek kazdeho chyboveho hlaseni
 * @param ... dalsi argumenty - dodatecny text, int, char aj. hodnoty, ..
 */
void warning_msg(const char *fmt, ...);

/**
 * @brief Vypise chybove hlaseni na stderr a ukonci program.
 * @details Chova se jako printf.
 * @param fmt fixni zacatek kazdeho chyboveho hlaseni
 * @param ... dalsi argumenty - dodatecny text, int, char aj. hodnoty, ..
 * @post Chybove hlaseni se vypise na stderr a program bude ukoncen s hodnotou 1.
 */
void error_exit(const char *fmt, ...);
