CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := bin/node

SRCEXT := cpp
CFLAGS := -Wall
INC := -I include

MODULES := main addressbook entry
OBJECTS := $(patsubst %, $(BUILDDIR)/%.o, $(MODULES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm bin/node $(BUILDDIR)/*.o
