CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g
all: pwd_80_22 sigquit sigusr1
pwd_80_22: pwd.c
	$(CC) $(CFLAGS) $< -o $@
sigquit: sigquit.c
	$(CC) $(CFLAGS) -rdynamic $< -o $@
sigusr1: sigusr1.c
	$(CC) $(CFLAGS) $< -o $@
install: pwd_80_22
	install -m 755 pwd_80_22 /usr/local/bin/pwd_80_22
clean:
	rm -f pwd_80_22 sigquit sigusr1
.PHONY: all install clean
