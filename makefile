CC := g++
SRCDIR := src
BUILDDIR := build

SRCEXT := cpp
CFLAGS := -Wall
INC := -I include

all: $(BUILDDIR)/*.o
	@echo " Linking....."
	$(CC) $(INC) $(BUILDDIR)/*.o -o bin/node

entry: $(SRCDIR)/entry.cpp include/entry.h
	$(CC) $(CFLAGS) $(INC) -c $(SRCDIR)/entry.cpp -o $(BUILDDIR)/entry.o

addressbook: $(SRCDIR)/addressbook.cpp include/addressbook.h
	$(CC) $(CFLAGS) $(INC) -c $(SRCDIR)/addressbook.cpp -o $(BUILDDIR)/addressbook.o

test: test.cpp $(SRCDIR)/entry.o $(SRCDIR)/addressbook.o
	$(CC) $(CFLAGS) $(INC) test/test.cpp $(BUILDDIR)/*.o -o test/test

clean:
	rm bin/node $(BUILDDIR)/*.o
