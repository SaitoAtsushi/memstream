SRCS := input.c output.c
EXAMPLE_SRCS := example1.c example2.c
OBJS := $(patsubst %.c,%.o,$(SRCS))
EXAMPLE_OBJS := $(patsubst %.c,%.o,$(EXAMPLE_SRCS))
CC = gcc
PREFIX = /usr/local
CFLAGS = -Wall
LDFLAGS =
LIBS =

UNAME = $(shell uname -a)
ifneq (,$(findstring MINGW, $(UNAME)))
	EXT = .exe
	TARGET := libmemstream.a
else
	EXT =
	TARGET :=
endif

EXAMPLE_EXES := $(patsubst %.c,%$(EXT),$(EXAMPLE_SRCS))

.PHONY: all clean example install

all: $(TARGET)

$(TARGET) : $(OBJS)
	ar ru $@ $(OBJS)

%.o: %.c memstream.h
	$(CC) -c $(CFLAGS) $< -o $@

install:
	cp $(TARGET) $(DESTDIR)$(PREFIX)/lib/$(TARGET)
	cp memstream.h $(DESTDIR)$(PREFIX)/include/memstream.h

examples: example1$(EXT) example2$(EXT)

example1$(EXT): example1.o libmemstream.a
	$(CC) $(CFLAGS) $< -o $@ -L. -lmemstream

example2$(EXT): example2.o libmemstream.a
	$(CC) $(CFLAGS) $< -o $@ -L. -lmemstream

clean:
	rm -f libmemstream.a $(OBJS) $(EXAMPLE_EXES) $(EXAMPLE_OBJS) *~
