# VARIABLE
CC:=gcc
CFLAGS:=-Wall
SOURCESDIR:=src
BUILDDIR:=build
BINDIR:=bin
TARGET:=$(BINDIR)/attaque_DES
SOURCES:=$(wildcard $(SOURCESDIR)/*.c)
OBJECTS:=$(patsubst $(SOURCESDIR)/%, $(BUILDDIR)/%, $(SOURCES:.c=.o))

# NETOYAGE, COMPILATIONN ET EXECUTION
all: clean run

# EXECUTION :
run: $(TARGET)
	./$(TARGET)

# EDITION DE LIEN :
$(TARGET): $(OBJECTS)
	mkdir -p $(BINDIR)
	$(CC) $^ -o $@ $(CFLAGS)

# COMPILATION :
$(BUILDDIR)/%.o: $(SOURCESDIR)/%.c
	mkdir -p $(BUILDDIR)
	$(CC) -c $< -o $@ $(CFLAGS)

# EDITION:
subl:
	subl $(SOURCES)
	subl include/*.h

# NETOYAGE :
clean:
	rm -rf $(BUILDDIR) $(BINDIR)