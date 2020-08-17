# Compiler
CC = gcc
LDFLAGS = -o $(BIN) $(LIBPATH) $(LIBS)
CFDEBUG = $(CFLAGS) -g -DDEBUG $(LDFLAGS)
RM = /bin/rm -f

BIN = employee-manager-rpc
BUILD_DIR = bin
POSTGRES = /usr/include/postgresql

LIBS = -lpq
INCLUDES_DIR = /usr/include
INCLUDES = -I$(INCLUDES_DIR) -I$(POSTGRES)
CFLAGS = -std=c18 -Wall

# All .c source files
SRC = main.c $(wildcard src/*.c)

all: $(BIN)

$(BIN):
	$(CC) $(SRC) $(CFLAGS) $(INCLUDES) $(LIBS) -o $(BUILD_DIR)/$(BIN) 

# prevent confusion with any files named "clean"
.PHONY: clean
clean:
	$(RM) *.o *~ $(BUILD_DIR)/$(BIN)

depend: $(SRC)
	makedepend $(INCLUDES) $^

debug_code:
	$(RM) debug/debug
	$(CC) -g -o debug/debug $(SRC) $(CFLAGS) $(INCLUDES) $(LIBS)


