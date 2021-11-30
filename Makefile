# Copyright 2021 Dumitru Bianca Stefania

CFLAGS=-Wall -Wextra -std=c99 -g

build:
	gcc $(CFLAGS) -o my_octave my_octave.c my_octave_utils.c strassen.c

pack:
	zip -FSr 312CA_BiancaStefaniaDumitru_Tema2.zip README Makefile *.c *.h

clean:
	rm -f my_octave

.PHONY: pack clean