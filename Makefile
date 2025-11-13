CFLAGS=-Wall -Wextra -pedantic -std=c99
SRCS=$(wildcard src/*.c)
OBJS=$(patsubst src/%.c,src/%.o,$(SRCS))

.PHONY: clean depend

battleship: $(OBJS)
	$(CC) -o bin/battleship $(OBJS) 

clean:
	rm -f bin/battleship $(OBJS)
depend:
	makedepend $(SRCS) -Y
# DO NOT DELETE

src/board.o: src/board.h
src/main.o: src/board.h src/menu.h src/utility.h
src/menu.o: src/menu.h src/utility.h
src/utility.o: src/utility.h
