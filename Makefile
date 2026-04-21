CC = gcc
CFLAGS = -g -std=c11 -Wall -Wextra -pedantic

all: no-comment primes primes-i

# priklad B: stavovy automat

no-comment: no-comment.o error.o
	$(CC) $(CFLAGS) no-comment.o error.o -o no-comment

no-comment.o: no-comment.c error.h
	$(CC) $(CFLAGS) -c no-comment.c

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c



# priklad A: hladanie prvocisel makrami

primes: primes.o eratosthenes.o error.o
	$(CC) $(CFLAGS) -O2 primes.o eratosthenes.o error.o -lm -o primes

primes.o: primes.c bitarray.h error.h
	$(CC) $(CFLAGS) -O2 -c primes.c -o primes.o

eratosthenes.o: eratosthenes.c bitarray.h
	$(CC) $(CFLAGS) -O2 -c eratosthenes.c -o eratosthenes.o



# priklad A: hladanie prvocisel inline funkciami

primes-i: primes-i.o eratosthenes-i.o error.o
	$(CC) $(CFLAGS) -O0 primes-i.o eratosthenes-i.o error.o -lm -o primes-i

primes-i.o: primes.c bitarray.h error.h
	$(CC) $(CFLAGS) -O0 -DUSE_INLINE -c primes.c -o primes-i.o

eratosthenes-i.o: eratosthenes.c bitarray.h error.h
	$(CC) $(CFLAGS) -O0 -DUSE_INLINE -c eratosthenes.c -o eratosthenes-i.o



run: primes primes-i
	ulimit -s 65000 && ./primes
	ulimit -s 65000 && ./primes-i



clean:
	rm -f *.o no-comment primes primes-i

clean-win:
	del /Q /F *.o no-comment.exe primes.exe primes-i.exe

