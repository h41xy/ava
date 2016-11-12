CC := g++
SRCDIR := src
BUILDDIR := build
BINDIR := bin
PROTDIR := prototype
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
$(BINDIR)/graphgen: $(BUILDDIR)/graphgen.o
	$(CC) $^ -o $@

$(BUILDDIR)/graphgen.o: $(SRCDIR)/graphgen.cpp
	$(CC) $(CFLAGS) -c $< -o $@

prototype: $(PROTDIR)/client $(PROTDIR)/server
$(PROTDIR)/client: $(PROTDIR)/client.o
	$(CC) $^ -o $@
$(PROTDIR)/server: $(PROTDIR)/server.o
	$(CC) $^ -o $@
$(PROTDIR)/%.o: $(PROTDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -vf $(TARGET) $(PROTDIR)/client $(PROTDIR)/server $(BUILDDIR)/*.o $(PROTDIR)/*.o
