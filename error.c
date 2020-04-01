/**
 *	@file	error.c
 *	@author Dominik Horky, FIT
 *	@date	13.03.2020
 *	@brief  Modul s definicemi funkci pouzivanymi k vypisu chybovych hlaseni.
 *	@note	Reseni IJC-DU1, priklad b)
 *	@details Prelozeno na gcc 9.2.1 (20200130, Manjaro Linux)
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "error.h"


void warning_msg(const char *fmt, ...) {
    va_list arguments = {0};
    vfprintf(stderr, "CHYBA: ", arguments);

    va_start(arguments, fmt);
    vfprintf(stderr, fmt, arguments);
    va_end(arguments);
}

void error_exit(const char *fmt, ...) {
    va_list arguments = {0};
    vfprintf(stderr, "CHYBA: ", arguments);

    va_start(arguments, fmt);
    vfprintf(stderr, fmt, arguments);
    va_end(arguments);
    exit(1);
}