# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)

# Default values for the chosen SAT Solver and Algorithm
SOLVER		?= cadical

# Directories for the source of the SAT Solvers
CADICAL_DIR	= lib/cadical-1.9.5

# main directories of this project
BUILD_DIR 	:= ./build
SRC_DIRS 	:= ./src
INC_DIRS 	:= ./include


# include source of the chosen SAT Solver and set target executable name
ifeq ($(SOLVER), cadical)
	INC_DIRS	+= ./$(CADICAL_DIR)/src
	TARGET_EXEC	:= serial-solver
endif

# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. The shell will incorrectly expand these otherwise, but we want to send the * directly to the find command.
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')

# Prepends BUILD_DIR and appends .o to every src file
# As an example, ./your_dir/hello.cpp turns into ./build/./your_dir/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS	:= $(addprefix -I,$(INC_DIRS))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS	:= $(INC_FLAGS) -MMD -MP

##################################################################################################
###### CUSTOM ####################################################################################
##################################################################################################
CPPFLAGS	+= -Wall -Wno-parentheses -Wno-sign-compare -std=c++20 -O3

ifeq ($(SOLVER), cadical)
	LDFLAGS		+= $(CADICAL_DIR)/build/libcadical.a
endif

# debug
#CXXFLAGS	+= -g3
#CPPFLAGS 	+= -D D_LOG

OUTPUT_DIR	:= ./build/bin

###################################################################################################

# The final build step.
$(OUTPUT_DIR)/$(TARGET_EXEC): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Build step for C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

#.ONESHELL:
cadical:
	@echo "Compiling CaDiCal..."
	cd $(CADICAL_DIR) && \
	./configure && make

clean-src:
	@if [ -d "build/src" ]; then \
		rm -rf build/src/*; \
		echo "Cleaned build/src directory."; \
	else \
		echo "build/src directory does not exist. Nothing to clean."; \
	fi

clean:
	@echo "Cleaning..."
	rm -r $(BUILD_DIR)

.PHONY: all clean clean-src

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)
