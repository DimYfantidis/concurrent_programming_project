CC=gcc


all: bincoef manna_pnueli


bincoef: bincoef.c 
	@$(CC) -O3 bincoef.c -I/usr/include/x86_64-linux-gnu -L/usr/lib/x86_64-linux-gnu -lgmp -pthread -o bincoef > /dev/null

manna_pnueli: manna_pnueli.c
	@$(CC) -O3 manna_pnueli.c -pthread -o manna_pnueli > /dev/null


clear:
	@if test -f "manna_pnueli"; then rm manna_pnueli; fi
	@if test -f "bincoef"; then rm bincoef; fi
