CC := g++
SRCDIR := src
BUILDDIR := build
BINDIR := bin
PRGNAME := node
TARGET := $(BINDIR)/$(PRGNAME)

CFLAGS := -Wall
INC := -I include

MODULES := node addressbook entry listener sender
OBJECTS := $(patsubst %, $(BUILDDIR)/%.o, $(MODULES))

all: $(TARGET)

debug: CFLAGS += -g
debug: $(TARGET)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $^ -o $@

$(BINDIR):
	mkdir $(BINDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJECTS): | $(BUILDDIR)

$(BUILDDIR):
	mkdir $(BUILDDIR)

thread: $(TARGET)
	$(CC) $^ -o $@ -lpthread

graphgen: $(BINDIR)/graphgen
	$(CC) $^ -o $@

$(BINDIR)/graphgen: $(SRCDIR)/graphgen.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(TARGET) $(BUILDDIR)/*.o
