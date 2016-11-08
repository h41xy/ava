CC := g++
SRCDIR := src
BUILDDIR := build
BINDIR := bin
PRGNAME := node
TARGET := $(BINDIR)/$(PRGNAME)

SRCEXT := cpp
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

listener.o: src/listener.cpp
	$(CC) $(CFLAGS) $(INC) -c $< -o $(BUILDDIR)/$@

sender.o: src/sender.cpp
	$(CC) $(CFLAGS) $(INC) -c $< -o $(BUILDDIR)/$@

thread: $(TARGET)
	$(CC) $^ -o $@ -lpthread

clean:
	rm $(TARGET) $(BUILDDIR)/*.o
