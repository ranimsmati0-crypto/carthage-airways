CC = gcc
DEFS = -DPACKAGE=\"personnel\" -DPACKAGE_DATA_DIR=\"$(CURDIR)\" -DPACKAGE_SOURCE_DIR=\"$(CURDIR)\"
CFLAGS = -Wall -Isrc -I../../src/personnel $(DEFS) `pkg-config --cflags gtk+-2.0`
LIBS = `pkg-config --libs gtk+-2.0`
SRC = $(wildcard src/*.c) $(wildcard ../../src/personnel/*.c)
HDR = $(wildcard src/*.h) $(wildcard ../../src/personnel/*.h)

personnel: $(SRC) $(HDR) ; $(CC) -o personnel $(SRC) $(CFLAGS) $(LIBS)

clean: ; rm -f personnel
