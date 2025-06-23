CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lm

SRC = seamcarving.c helper.c main.c
OBJ = $(SRC:.c=.o)
EXEC = seamcarving

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(EXEC)
