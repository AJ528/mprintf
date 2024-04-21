# while it is tempting to adjust this makefile so you can have a debug and release target
# I have been unable to find any elegant way to accomplish this.

# set debug to 0 or 1
# adjust optimization flag accordingly below
debug = 1

# names of directories for compiled objects
BIN_DIR = bin
OBJ_DIR = obj
DEP_DIR = dep

# name of the output image
TARGET_NAME := mprintf
TARGET := $(BIN_DIR)/$(TARGET_NAME).elf

# locations of directories containing source files.
# these locations should be specified relative to the makefile location.
SRC_DIRS = \
	src

# locations of directories containing header files.
# these locations should be specified relative to the makefile location.
INC_DIRS = 		\
	inc

# predefined macros
DEFINES =
	

# sets OPTIMIZE_FLAGS based on debug above
ifeq ($(debug), 1)
	DEFINES += DEBUG
	OPTIMIZE_FLAGS = -ggdb3 -Og
else
# change optimization options to whatever suits you
	DEFINES += RELEASE
	OPTIMIZE_FLAGS = -O2
endif

# creates the list of define flags to pass to the compiler
DEFINE_FLAGS := $(addprefix -D,$(DEFINES))
# creates the list of include flags to pass to the compiler
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
# adds the source directories to Make's search path
VPATH = $(SRC_DIRS)

# compiler you want to use
CC = gcc

# define flags
COMMON_FLAGS = $(DEFINE_FLAGS)

# compiler, assembler, and linker flags all start with the same flags
CFLAGS = $(COMMON_FLAGS)
ASFLAGS = $(COMMON_FLAGS)
LDFLAGS = $(COMMON_FLAGS)

# add on compiler-specific flags
# optimization flags
CFLAGS += $(OPTIMIZE_FLAGS)
# C standard to compile against. gnu17 means use C17 standard and add GNU extensions
CFLAGS += -std=gnu17
# put functions and data into individual sections
CFLAGS += -ffunction-sections -fdata-sections
# enable all warning messages from the compiler
CFLAGS += -Wall
# enable some extra warnings from the compiler
CFLAGS += -Wextra

# add on compiler-specific flags
# optimization flags
ASFLAGS += $(OPTIMIZE_FLAGS)

# add on linker-specific flags
# if any system libraries are used, include their code with the executable by statically linking it
# LDFLAGS += -static
# note: if you want to use the "-Wl" to pass options to the linker, there must be NO SPACES
# remove empty sections only if not for debug
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Xlinker -Map=$(OBJ_DIR)/$(TARGET_NAME).map
LDFLAGS += -lgcc

# is there any point in linking -lc -lm if those also get removed by the linker script?
# flags to investigate:
# LDFLAGS += -Wl,--start-group -lc -lm -Wl,--end-group


# creates the list of .c source files by looking for every .c file in the source directories
CSRCS := $(foreach x, $(SRC_DIRS), $(wildcard $(addprefix $(x)/*,.c)))
# creates the list of .S (uppercase 'S') source files by looking for every .S file in the source directories
SSRCS := $(foreach x, $(SRC_DIRS), $(wildcard $(addprefix $(x)/*,.S)))
# creates the list of .s (lowercase 's') source files by looking for every .s file in the source directories
sSRCS := $(foreach x, $(SRC_DIRS), $(wildcard $(addprefix $(x)/*,.s)))

# creates a list of all .c, .S, and .s source files in one place
OBJ_SRCS := $(CSRCS) $(SSRCS) $(sSRCS)
# creates a list of object files by taking every source file, removing the file extension
# and replacing it with an "o"
OBJS := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(notdir $(basename $(OBJ_SRCS)))))

# creates a list of all source files that need a dependency file
DEP_SRCS := $(CSRCS) $(SSRCS)
# creates a list of dependecy files by taking every .c and .S source file, removing the file extension
# and replacing it with a "d"
DEPS := $(addprefix $(DEP_DIR)/, $(addsuffix .d, $(notdir $(basename $(DEP_SRCS)))))

.PHONY: run

run:
	$(MAKE) image
	$(TARGET)

# rule for linking the overall image from object files. This is the default rule that is called
# if you type "make" with no arguments. The prerequisites are the object files and the existence
# of the binary directory. 
# listing pdebug as a prerequisite means it gets called everytime this rule is ran.
$(TARGET): $(OBJS) pdebug | $(BIN_DIR)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

# rule to make object files from .c source files. The recipe to build the .o file is specified here. 
# the prerequisites listed here do not include the header (.h) files needed to compile each object file
# header file prerequisites are added by the dependency (.d) files.
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

# rule to make the dependency files for .c source files. There is one dependency file per .c file.
# the dependency file lists all the prerequisite headers for each object (.o) file.
$(DEP_DIR)/%.d: %.c | $(DEP_DIR)
	@echo DEPEND: $(CC) -MM $(CFLAGS) $<
# The purpose of the sed command is to translate (for example):
# main.o : main.c defs.h
# into:
# main.o main.d : main.c defs.h
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $(INC_FLAGS) $< > $@.$$$$; \
	sed 's,\($*\.o\)[ :]*,$(OBJ_DIR)/\1 $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# rule to make object files from .S (uppercase 'S') source files. The recipe to build the .o file is specified here.
# the prerequisites listed here do not include the header (.h) files needed to compile each object file
# header file prerequisites are added by the dependency (.d) files.
$(OBJ_DIR)/%.o: %.S | $(OBJ_DIR)
	$(CC) $(ASFLAGS) $(INC_FLAGS) -c $< -o $@

# rule to make the dependency files for .S source files. There is one dependency file per .S file.
# the dependency file lists all the prerequisite headers for each object (.o) file.
$(DEP_DIR)/%.d: %.S | $(DEP_DIR)
	@echo DEPEND: $(CC) -MM $(CFLAGS) $<
# The purpose of the sed command is to translate (for example):
# main.o : main.S defs.h
# into:
# main.o main.d : main.S defs.h
	@set -e; rm -f $@; \
	$(CC) -MM $(ASFLAGS) $(INC_FLAGS) $< > $@.$$$$; \
	sed 's,\($*\.o\)[ :]*,$(OBJ_DIR)/\1 $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# rule to make object files from .s (lowercase 's') source files 
# the prerequisites listed here do not include the header (.h) files needed to compile each object file
# header file prerequisites are added by the dependency (.d) files.
$(OBJ_DIR)/%.o: %.s | $(OBJ_DIR)
	$(CC) $(ASFLAGS) $(INC_FLAGS) -c $< -o $@

# no rule needed to make depdency files for .s source files
# .s (lowercase 's') cannot have "#include" or other c preprocessor stuff

# rule to make the build directories if they do not already exist
$(BIN_DIR) $(OBJ_DIR) $(DEP_DIR):
	mkdir -p $@


# .PHONY targets will be run every time they are called.
# any special recipes you want to run by name should be a phony target.
.PHONY: clean pdebug image

image: 
	$(MAKE) $(TARGET)

# recipe to print some debug information
pdebug:
	@echo "Source Directories = " $(SRC_DIRS)
	@echo "Include Directories = " $(INC_DIRS)

# recipe to remove the build directories and clean up the workspace
clean:
	rm -r $(BIN_DIR) $(OBJ_DIR) $(DEP_DIR)

# if we are not cleaning the workspace, include the dependency files.
# the rules in included files are combined with pre-existing rules to
# fully define the prerequisites for each target output.
ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif