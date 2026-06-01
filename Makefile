CC ?= cc
CFLAGS ?= -std=c99 -Wall -Wextra -pedantic -O2
INCLUDES = -Iinclude
LDFLAGS = -lm

SRCDIR = src
TESTDIR = test

SOURCES = $(SRCDIR)/tempo.c $(SRCDIR)/polyrhythm.c $(SRCDIR)/groove.c \
          $(SRCDIR)/syncopation.c $(SRCDIR)/meter.c

HEADERS = include/tempo.h include/polyrhythm.h include/groove.h \
          include/syncopation.h include/meter.h include/rhythm_api.h

OBJECTS = $(SOURCES:.c=.o)

.PHONY: all test clean

all: librhythmmath.a

librhythmmath.a: $(OBJECTS)
	ar rcs $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

test: test_rhythm
	./test_rhythm

test_rhythm: $(TESTDIR)/test_rhythm.c $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(TESTDIR)/test_rhythm.c $(SOURCES) $(LDFLAGS)

clean:
	rm -f $(OBJECTS) librhythmmath.a test_rhythm
