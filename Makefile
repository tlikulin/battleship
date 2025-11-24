CFLAGS=-Wall -Wextra -pedantic -std=c99
SRCS=$(wildcard src/*.c)
OBJS=$(patsubst src/%.c,src/%.o,$(SRCS))

.PHONY: clean depend

battleship: $(OBJS)
	mkdir -p bin && $(CC) -o bin/battleship $(OBJS) 

clean:
	rm -f bin/battleship $(OBJS)
depend:
	makedepend $(SRCS) -Y
# DO NOT DELETE

src/board.o: src/board.h
src/main.o: src/menus.h src/utility.h
src/menus.o: src/board.h src/menus.h src/save-load.h src/utility.h
src/save-load.o: src/board.h src/save-load.h
src/utility.o: src/utility.h
