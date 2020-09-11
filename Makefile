# Compiler
CC = gcc
LDFLAGS = -o $(BIN) $(LIBPATH) $(LIBS)
CFDEBUG = $(CFLAGS) -g -DDEBUG $(LDFLAGS)
RM = /bin/rm -f

BIN = employee-manager-rpc
BUILD_DIR = bin
POSTGRES = /usr/include/postgresql

LIBS = -lpq -lserc
LIBS_DIR = -L/usr/local/lib
INCLUDES_DIR = /usr/include /usr/lib
INCLUDES = -I$(INCLUDES_DIR) -I$(POSTGRES)
CFLAGS = -std=c18 -Wall

# All .c source files
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)

.PHONY: all
all: $(BIN)

$(BIN): $(OBJS)
	@$(CC) $(CFLAGS) $^ $(INCLUDES_DIR) $(INCLUDES) $(LIBS_DIR) $(LIBS) -o $@
	@printf "\e[33mLinking\e[90m %s\e[0m\n" $@
	@printf "\e[34Done!\e[0m\n" $@

%.o: %.c
	@$(CC) -c $< -o $@
	@printf "\e[36mCompile\e[90m %s\e[0m\n" $@

# $(CC) $(SRCS) $(CFLAGS) $(INCLUDES) $(LIBS) $(LIBS_DIR) -o $(BUILD_DIR)/$(BIN) 


# prevent confusion with any files named "clean"
.PHONY: clean
clean:
	@$(RM) $(BUILD_DIR)/$(BIN) $(OBJ)
	@printf "\e[34mAll clear!\e[0m\n"

depend: $(SRCS)
	makedepend $(INCLUDES) $^

debug_code:
	$(RM) debug/debug
	$(CC) -g -o debug/debug $(SRCS) $(CFLAGS) $(INCLUDES) $(LIBS)


