CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := bin/node

SRCEXT := cpp
CFLAGS := -Wall
INC := -I include

MODULES := main addressbook entry listener sender
OBJECTS := $(patsubst %, $(BUILDDIR)/%.o, $(MODULES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

listener.o: src/listener.cpp
	$(CC) $(CFLAGS) $(INC) -c $< -o $(BUILDDIR)/$@

sender.o: src/sender.cpp
	$(CC) $(CFLAGS) $(INC) -c $< -o $(BUILDDIR)/$@

thread: $(TARGET)
	$(CC) $^ -o $@ -lpthread

clean:
	rm bin/node $(BUILDDIR)/*.o
