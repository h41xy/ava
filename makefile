CC := g++
SRCDIR := src
BUILDDIR := build
BINDIR := bin

PRGNAME_NODE := node_exec
TARGET_NODE := $(BINDIR)/$(PRGNAME_NODE)

PRGNAME_CTRL := controller
TARGET_CTRL := $(BINDIR)/$(PRGNAME_CTRL)

TARGETS := $(TARGET_NODE) $(TARGET_CTRL)

CFLAGS := -Wall
INC := -I include

MODULES_NODE := entry addressbook listener sender message node node_exec 
OBJECTS_NODE := $(patsubst %, $(BUILDDIR)/%.o, $(MODULES_NODE))

MODULES_CTRL := entry sender listener message controller 
OBJECTS_CTRL := $(patsubst %, $(BUILDDIR)/%.o, $(MODULES_CTRL))

all: $(TARGET_NODE) $(TARGET_CTRL)

debug: CFLAGS += -g
debug: $(TARGET_NODE) $(TARGET_CTRL)

$(PRGNAME_NODE): $(TARGET_NODE)
$(TARGET_NODE): $(OBJECTS_NODE) | $(BINDIR)
	$(CC) $^ -o $@

$(PRGNAME_CTRL): $(TARGET_CTRL)
$(TARGET_CTRL): $(OBJECTS_CTRL) | $(BINDIR)
	$(CC) $^ -o $@

$(BINDIR):
	mkdir $(BINDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJECTS_NODE): | $(BUILDDIR)
$(OBJECTS_CTRL): | $(BUILDDIR)

$(BUILDDIR):
	mkdir $(BUILDDIR)

clean:
	rm -vf $(TARGETS) $(PROTDIR)/client $(PROTDIR)/server $(BUILDDIR)/*.o $(PROTDIR)/*.o
