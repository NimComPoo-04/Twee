# The program requires atleast c99 standard

CC = gcc
CFLAGS = -Wall -Wextra -Werror -ggdb -std=c99 -pedantic -ggdb -O2

SRC = main.c twee.c
OBJ = $(patsubst %.c, %.o, $(SRC))

EXE = twee.exe

# Turn this on if your are using Linux
RM = rm

# Turn this on if you are using Window
# RM = del

all: $(EXE)

main.o: main.c
twee.o: twee.c twee.h

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ):
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJ) $(EXE)
