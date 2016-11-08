CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := bin/node

SRCEXT := cpp
CFLAGS := -Wall -std=c++11 -lpthread
INC := -I include

MODULES := node addressbook entry listener sender
OBJECTS := $(patsubst %, $(BUILDDIR)/%.o, $(MODULES))

all: $(TARGET)

debug: CFLAGS += -g
debug: $(TARGET)

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
