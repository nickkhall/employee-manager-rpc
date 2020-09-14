#### Configuration ################################################################################

_PROJ     = employee-manager-rpc#                   # Name of the project and generated executable
_SDIR     = src#                                    # Path to dir of source files (relative to ./)
_HDIR     = include#                                # Path to dir of header files (relative to ./)
_BDIR     = bin#                                    # Path to dir of binary files (relative to ./)
_SSUF     = c#                                      # Suffix of source files
_HSUF     = h#                                      # Suffix of header files
_CC       = gcc#                                    # Compiler to be used
_CFLAGS   = -std=c11 -Wall -Werror#                 # Compiler build flags
_PSQL     = /usr/include/postgresql#
_LIBS     = -lpq -lserc -ldbc -lsockc#
_LIBS_DIR = -L/usr/local/lib#
_SCRIPT   = :#                                      # Any shell script to run before build (replace ':')
_SHELL    = /bin/bash#                              # Shell to be used by makefile
_CARGS    = -I$(_HDIR) -I$(_PSQL) -I/usr/include \
						$(_LIBS_DIR) $(_LIBS) $(_CFLAGS)#       # Full set of compiler arguments
PURPLE    = \033[0;35m#                             # Encoding of purple color for terminal output
CYAN      = \033[0;36m#                             # Encoding of cyan color for terminal output
NC        = \033[0m#                                # Encoding of no color for terminal output

_DEBUG_DIR   = debug#                               # Debug directory
_DEBUG_EXE   = $(_DEBUG_DIR)/debug#                 # Debug executable name
_DEBUG_FLAGS = -ggdb -O0 -DDEBUG#                   # Debug flags
_DEBUG_PORT  = 12347#                               # Debug port
_DEBUG_CONF  = $(_DEBUG_DIR)/debug_conf.gdb

##### File Lists ##################################################################################

# Header Files
HEDRS   = $(shell find $(_HDIR) -print | grep .$(_HSUF))

# Object Files
OBJS    = $(shell find $(_SDIR) -print | grep .$(_SSUF) | \
						sed -r "s/($(_SDIR))\/(.*)\.($(_SSUF))/$(_BDIR)\/obj\/\2\.o/" | \
						sed -r "s/src//")

# Sources
SRCS    = $(shell find $(_SDIR) -type f -name *.c)

# Options
OPTS =

##### Dependency Rules ############################################################################

.PHONY: all run clean debug gdb_debug gdb_debugger 
.SILENT: $(_BDIR)/$(_PROJ) $(OBJS) $(_BDIR) run clean

# Default
all: $(_BDIR)/$(_PROJ)

# Link all compiled object files
$(_BDIR)/$(_PROJ): $(OBJS)
	echo "------------------------------------------------------------------------------------------------------------"
	echo ""
	echo "Employee Manager RPC: compling object files:\n${CYAN}$^${NC}\n  -- from command ${PURPLE}$@${NC}"
	echo ""
	echo "------------------------------------------------------------------------------------------------------------"
	echo ""
	$(_CC) -o $@ $^ $(_CARGS) && echo "Employee Manager RPC: successfully built executable ${CYAN}$@${NC}"
	echo ""
	echo "------------------------------------------------------------------------------------------------------------"

# Compile all outdated source files into their respective object files
$(_BDIR)/obj/%.o: $(_SDIR)/%.$(_SSUF) $(HEDRS) | $(_BDIR)
	echo "Employee Manager RPC: compiling source file ${PURPLE}$<${NC}\n  -- Object File: $@\n" && \
	$(_CC) -c $< -o $@ $(_CARGS)

# Ensure target folders for binaries exist and run any additional user defined shell script
$(_BDIR):
	mkdir -p $(_BDIR)/obj && $(_SCRIPT)

# Run the built executable of your project
run: $(_BDIR)/$(_PROJ)
	echo "Employee Manager RPC: launching executable ${CYAN}$(_BDIR)/$(_PROJ)${NC}:" && \
	$(_BDIR)/$(_PROJ)

# Delete all binaries and any editor backups of source and header files
clean:
	echo "Employee Manager RPC: cleaning up..." && \
	rm -rf $(_BDIR) $(_SDIR)/*~ $(_HDIR)/*~

# Debug executubale
debug: $(_DEBUG_EXE)

$(_DEBUG_EXE): $(OBJS)
	echo "OBJECT FILE : $(_CC) $(_CARGS) $(_DEBUG_FLAGS) -o $(_DEBUG_EXE) $^ ()()()()()()()()()()()()"
	echo ""
	$(_CC) $(_CARGS) $(_DEBUG_FLAGS) -c -o $(_DEBUG_EXE) $^

$(_DEBUG_DIR)/%.o: %.c
	echo "C SOURCE FILE : $(_CC) $(_CARGS) $(_DEBUG_FLAGS) -c -o $(_DEBUG_DIR)/$@ $< ----------- "
	echo ""
	$(_CC) $(_CARGS) $(_DEBUG_FLAGS) -o $(_DEBUG_DIR)/$@ $<

# GDB debug exectubale
gdb_debug:	
	gdb -x $(_DEBUG_CONF)

# GDB debugger server
gdb_debugger:	
	$(shell tmux splitw -h "gdbserver localhost:$(_DEBUG_PORT) $(_DEBUG_EXE)") 
	echo "Employee Manager RPC: Debugging on port $(_DEBUG_PORT)..."
