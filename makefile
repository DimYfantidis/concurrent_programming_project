CC=gcc


all: bincoef manna_pnueli


bincoef: bincoef.c bincoef.o
	$(CC) -O3 bincoef.c -I/usr/include/x86_64-linux-gnu -L/usr/lib/x86_64-linux-gnu -lgmp -pthread -o bincoef

bincoef.o: bincoef.c
	$(CC) -O3 -c bincoef.c -I/usr/include/x86_64-linux-gnu -L/usr/lib/x86_64-linux-gnu -lgmp -pthread -o bincoef.o

manna_pnueli: manna_pnueli.c manna_pnueli.o
	$(CC) -O3 manna_pnueli.c -pthread -o manna_pnueli

manna_pueli.o: manna_pnueli.c
	$(CC) -O3 -c manna_pnueli.c -pthread -o manna_pnueli.o


clear:
	@if test -f "manna_pnueli.o"; then rm manna_pnueli.o; fi
	@if test -f "manna_pnueli"; then rm manna_pnueli; fi
	@if test -f "bincoef.o"; then rm bincoef.o; fi
	@if test -f "bincoef"; then rm bincoef; fi
