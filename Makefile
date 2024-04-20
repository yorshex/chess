dir_guard=@mkdir -p $(@D)

CC ?= gcc
OBJDIR ?= obj
CFLAGS=-std=c99 -I./raylib/src
LIBS=-l:libraylib.a -lm
OBJ=chess.o \
	draw.o \
	fen_to_chess.o \
	util.o \
	logic.o \
	input.o

.PHONY: build run

build: ./lib/libraylib.a $(addprefix $(OBJDIR)/, $(OBJ))
	$(CC) $(CFLAGS) -o chess $(addprefix $(OBJDIR)/, $(OBJ)) -L./lib $(LIBS)

clean:
	rm -f chess $(addprefix $(OBJDIR)/, $(OBJ))

run: build
	./chess

$(OBJDIR)/%.o: %.c chess.h
	$(dir_guard)
	$(CC) -c $(CFLAGS) -o $@ $<

./lib/libraylib.a:
	$(dir_guard)
	cd ./raylib/src; make raylib
	mv ./raylib/src/libraylib.a ./lib/libraylib.a
	cd ./raylib/src; make clean
