CFLAGS=-std=c89 -Wall -Wextra -pedantic

mm: mm.o io.o round.o mm_color.o

mm.o: mm.c io.h round.h mm_color.h

io.o: io.c io.h mm_color.h round.h

round.o: round.c round.h mm_color.h

mm_color.o: mm_color.c mm_color.h

clean:
	rm -f *.o mm


