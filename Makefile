CC      := gcc
CFLAGS  := -std=c99 -Wall -Wextra -pedantic
LDFLAGS := -lpthread
SRC     := core.c queue.c networking.c helpers.c
BIN     := sweepy

default: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN) $^

clean:
	rm $(BIN)

.PHONY: clean
