#
#	Makefile
#
#	Reseni:     	IJC-DU1
#	Datum:      	19. 03. 2020
# 	Autor:      	Dominik Horky, FIT
#	Vystup gcc -v:	gcc version 9.2.1 20200130 (Arch Linux 9.2.1+20200130-2)

# 	Pouziti:
# 		make / make all	->	zkompiluje a vytvori spustitelne soubory (ty co nejsou vytvoreny nebo jen ty zmenene)
# 		make run		->	(zkompiluje a) spusti primes a primes-i
# 		make clean		->	smaze .o a spustelne soubory

#
# PREKLADAC, PARAMETRY PREKLADU
#

CC=gcc
CFLAGS=-g -std=c11 -pedantic -Wall -Wextra -lm -O2

#
# MAKE
#

all: primes primes-i steg-decode steg-encode

#
# VYTVORENI OBJEKTOVYCH SOUBORU
#

# vytvori objektove soubory pouzivane vsemi ostatnimi moduly
error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c
bitset.o: bitset.c bitset.h error.h
	$(CC) $(CFLAGS) -c bitset.c

# vytvori objektovy soubor modulu pouzivaneho pro 'primes'
eratosthenes.o: eratosthenes.c eratosthenes.h bitset.h error.h
	$(CC) $(CFLAGS) -c eratosthenes.c

# vytvori objektove soubory modulu pouzivajicich inline funkce ('primes-i')
bitset-i.o: bitset.c bitset.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c bitset.c -o bitset-i.o
eratosthenes-i.o: eratosthenes.c eratosthenes.h bitset.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c eratosthenes.c -o eratosthenes-i.o

# vytvori objektovy soubor pouzivany modulem 'steg-decode'
ppm.o: ppm.c ppm.h error.h
	$(CC) $(CFLAGS) -c ppm.c

# vytvori objektove soubory "hlavnich" modulu
primes.o: primes.c eratosthenes.h bitset.h error.h
	$(CC) $(CFLAGS) -c primes.c
primes-i.o: primes.c eratosthenes.h bitset.h error.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c primes.c -o primes-i.o
steg-decode.o: steg-decode.c ppm.h eratosthenes.h bitset.h error.h
	$(CC) $(CFLAGS) -c steg-decode.c
steg-encode.o: steg-encode.c ppm.h eratosthenes.h bitset.h error.h
	$(CC) $(CFLAGS) -c steg-encode.c

#
# VYTVORENI SPUSTITELNYCH SOUBORU
#

# vytvori spustitelne soubory 'primes', 'primes-i' a 'steg-decode'
primes: primes.o eratosthenes.o bitset.o error.o
	$(CC) $(CFLAGS) primes.o eratosthenes.o bitset.o error.o -o primes
primes-i: primes-i.o eratosthenes-i.o bitset-i.o error.o
	$(CC) $(CFLAGS) primes-i.o eratosthenes-i.o bitset-i.o error.o -o primes-i
steg-decode: steg-decode.o ppm.o eratosthenes.o error.o bitset.o
	$(CC) $(CFLAGS) steg-decode.o ppm.o eratosthenes.o error.o bitset.o -o steg-decode
steg-encode: steg-encode.o ppm.o eratosthenes.o error.o bitset.o
	$(CC) $(CFLAGS) steg-encode.o ppm.o eratosthenes.o error.o bitset.o -o steg-encode

#
# DODATECNA FUNKCIONALITA
# (zvysi stack a spusti primes)

run: primes primes-i
	ulimit -s 65000 && ./primes && ./primes-i
clean: 
	rm -rf *.o primes primes-i steg-decode steg-encode
