CC := g++
SRCDIR := src
BUILDDIR := build
BINDIR := bin
PROTDIR := prototype

PRGNAME_NODE := node
TARGET_NODE := $(BINDIR)/$(PRGNAME_NODE)
PRGNAME_CTRL := controller
TARGET_CTRL := $(BINDIR)/$(PRGNAME_CTRL)
PRGNAME_GG := graphgen
TARGET_GG := $(BINDIR)/$(PRGNAME_GG)

TARGETS := $(TARGET_NODE) $(TARGET_CTRL) $(TARGET_GG)

CFLAGS := -Wall
INC := -I include

MODULES_NODE := node addressbook entry listener sender
OBJECTS_NODE := $(patsubst %, $(BUILDDIR)/%.o, $(MODULES_NODE))

MODULES_CTRL := controller sender listener
OBJECTS_CTRL := $(patsubst %, $(BUILDDIR)/%.o, $(MODULES_CTRL))

MODULES_GG := graphgen
OBJECTS_GG := $(patsubst %, $(BUILDDIR)/%.o, $(MODULES_GG))

all: $(TARGET_NODE) $(TARGET_CTRL) $(TARGET_GG)

debug: CFLAGS += -g
debug: $(TARGET)

$(PRGNAME_NODE): $(TARGET_NODE)
$(TARGET_NODE): $(OBJECTS_NODE) | $(BINDIR)
	$(CC) $^ -o $@

$(PRGNAME_CTRL): $(TARGET_CTRL)
$(TARGET_CTRL): $(OBJECTS_CTRL) | $(BINDIR)
	$(CC) $^ -o $@

$(PRGNAME_GG): $(TARGET_GG)
$(TARGET_GG): $(OBJECTS_GG) | $(BINDIR)
	$(CC) $^ -o $@

$(BINDIR):
	mkdir $(BINDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJECTS_NODE): | $(BUILDDIR)
$(OBJECTS_CTRL): | $(BUILDDIR)
$(OBJECTS_GG): | $(BUILDDIR)

$(BUILDDIR):
	mkdir $(BUILDDIR)

prototype: $(PROTDIR)/client $(PROTDIR)/server
$(PROTDIR)/client: $(PROTDIR)/client.o
	$(CC) $^ -o $@
$(PROTDIR)/server: $(PROTDIR)/server.o
	$(CC) $^ -o $@
$(PROTDIR)/%.o: $(PROTDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -vf $(TARGETS) $(PROTDIR)/client $(PROTDIR)/server $(BUILDDIR)/*.o $(PROTDIR)/*.o
