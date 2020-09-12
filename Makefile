#### Configuration ################################################################################

_PROJ     = employee-manager-rpc#            # The name of the project and generated executable
_SDIR     = src#                             # Path to directory of source files (relative to ./)
_HDIR     = include#                         # Path to directory of header files (relative to ./)
_BDIR     = bin#                             # Path to directory of binary files (relative to ./)
_SSUF     = c#                               # Suffix of source files
_HSUF     = h#                               # Suffix of header files
_CC       = gcc#                             # Compiler to be used
_CFLAGS   = -std=c11 -Wall -Werror#          # Compilation flags
_PSQL     = /usr/include/postgresql
_LIBS     = -lpq -lserc -ldbc -lsockc
_LIBS_DIR = -L/usr/local/lib
_SCRIPT   = :#                               # Any shell script to run before build (replace ':')
SHELL     = /bin/bash#                       # Shell to be used by makefile
CARGS     = -I$(_HDIR) -I$(_PSQL) -I/usr/include \
		$(_LIBS_DIR) $(_LIBS) $(_CFLAGS)#  # Full set of compiler arguments
PURPLE    = \033[0;35m#                      # Encoding of purple color for terminal output
CYAN      = \033[0;36m#                      # Encoding of cyan color for terminal output
NC        = \033[0m#                         # Encoding of no color for terminal output



##### File Lists ##################################################################################

# A list of header files on which source files depend
HEDRS   = $(shell find $(_HDIR) -print | grep .$(_HSUF))

# A list of object files on which their existing source files depend
OBJS    = $(shell find $(_SDIR) -print | grep .$(_SSUF) | \
			sed -r "s/($(_SDIR))\/(.*)\.($(_SSUF))/$(_BDIR)\/obj\/\2\.o/" | \
			sed -r "s/src//")

##### Dependency Rules ############################################################################

.PHONY: run clean
.SILENT: $(_BDIR)/$(_PROJ) $(OBJS) $(_BDIR) run clean

# Default
all: $(_BDIR)/$(_PROJ)

# Link all compiled object files
$(_BDIR)/$(_PROJ): $(OBJS)
	echo -e "OBJS = $(OBJS)"
	echo -e "------------------------------------------------------------------------------------------------------------"
	echo -e ""
	echo -e "Employee Manager RPC: compling object files:\n${CYAN}$^${NC}\n  -- from command ${PURPLE}$@${NC}"
	echo -e ""
	echo -e "------------------------------------------------------------------------------------------------------------"
	echo -e ""
	$(_CC) -o $@ $^ $(CARGS) && \
	echo -e "Employee Manager RPC: successfully built executable ${CYAN}$@${NC}"
	echo -e ""
	echo -e "------------------------------------------------------------------------------------------------------------"

# Compile all outdated source files into their respective object files
$(_BDIR)/obj/%.o: $(_SDIR)/%.$(_SSUF) $(HEDRS) | $(_BDIR)
	echo -e "Employee Manager RPC: compiling source file ${PURPLE}$<${NC}\n  -- Object File: $@\n" && \
	$(_CC) -c $< -o $@ $(CARGS)

# Ensure target folders for binaries exist and run any additional user defined shell script
$(_BDIR):
	mkdir -p $(_BDIR)/obj && $(_SCRIPT)

# Run the built executable of your project
run: $(_BDIR)/$(_PROJ)
	echo -e "Employee Manager RPC: launching executable ${CYAN}$(_BDIR)/$(_PROJ)${NC}:" && \
	$(_BDIR)/$(_PROJ)

# Delete all binaries and any editor backups of source and header files
clean:
	echo -e "Employee Manager RPC: cleaning up...\n" && \
	rm -rf $(_BDIR) $(_SDIR)/*~ $(_HDIR)/*~
