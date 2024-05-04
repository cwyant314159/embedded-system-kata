include common/build-files/file_io.mk
include common/build-files/toolchain.mk

# To prevent conflicts between the different builds, each exercise will put all
# of its object (.o) files in its own directory. These object directories will
# live under the OBJ_ROOT_DIR, so that the clean command can delete this one 
# folder instead of keeping track of each exercise's object directory.
#
# The exercise specific Makefile will append sub-directories and store the 
# resultant path into the OBJ variable used by the build targets below.
OBJ_ROOT_DIR := obj

# Similar to the OBJ_ROOT_DIR variable, all binary files will reside under this
# directory for easier clean up.
#
# The exercise specific Makefile will append sub-directories and store the
# resultant path into the BINARY_DIR variable used below.
BIN_ROOT_DIR := bin

# Include the exercise specific variables. If no KATA value was specified on the
# command line, the last KATA will be used by default.
KATA ?=

ifeq ($(KATA),)
include 03_hello_morse/exercise.mk
else ifeq ($(KATA),0)
include 00_bringup/exercise.mk
else ifeq ($(KATA),1)
include 01_blinky_timer/exercise.mk
else ifeq ($(KATA),2)
include 02_sos/exercise.mk
else ifeq ($(KATA),3)
include 03_hello_morse/exercise.mk
else
$(error Invalid KATA number: $(KATA))
endif

# Now that the INC_FLAGS variable is defined, the rest of the compiler flags can
# be included.
include common/build-files/compiler_flags.mk

# Using the list of source files specified by the exercise specific file, create
# the listing of the compiled object files. The exercise specific file is
# responsible for setting up the OBJ_DIR variable for this.
OBJS := $(SRC_FILES:=.o)
OBJS := $(foreach obj, $(OBJS), $(addprefix $(OBJ_DIR)/, $(obj)))

# Specify the path to the compiled ELF and hex files for the build target. The
# hex file has the same name as the ELF file with the extension substituted for
# .hex. The exercise specific file is responsible for setting up the BINARY_DIR
# variable for this.
ELF_FILE := $(BINARY_DIR)/$(BINARY_NAME).elf
HEX_FILE := $(ELF_FILE:.elf=.hex)


# List of phony targets that do not have a generated output.
.PHONY: flash verify erase clean

# Build the application.
#
build: $(HEX_FILE)


# Target to objcopy the ELF file into a hex file the microcontroller can
# understand.
# 
# In addition to the hex file this target also builds:
#   - a disassembly listing of the application (*.lss)
#   - the application size report
#   - a list of object names (numerically sorted by size)
#
$(HEX_FILE): $(ELF_FILE)
	$(OBJCOPY) -O ihex $< $@
	$(OBJDUMP) --disassemble $< > $(BINARY_DIR)/$(BINARY_NAME).lss
	$(SIZE) -A -t $< > $(BINARY_DIR)/$(BINARY_NAME).size.txt
	$(NM) --numeric-sort --print-size $< | $(CXXFILT) > $(BINARY_DIR)/$(BINARY_NAME).names.txt


# linker
#
$(ELF_FILE): $(OBJS) $(LDSCRIPT)
	@$(MKDIR) $$(dirname $@)
	$(LD) $(LDFLAGS) $(OBJS) -o $@


# assembler
#
$(OBJ_DIR)/%.s.o: %.s
	@$(MKDIR) $$(dirname $@)
	$(AS) $(ASFLAGS) -o $@ $<


# C compiler
#
$(OBJ_DIR)/%.c.o: %.c
	@$(MKDIR) $$(dirname $@)
	$(CC) $(CFLAGS) -o $@ $<

# C++ compiler
#
$(OBJ_DIR)/%.cpp.o: %.cpp
	@$(MKDIR) $$(dirname $@)
	$(CXX) $(CXXFLAGS) -o $@ $<


# Delete compiled artifacts
#
clean:
	$(RM) $(OBJ_ROOT_DIR)
	$(RM) $(BIN_ROOT_DIR)

# The next few build targets are the microcontroller flash, erase, verify steps.
# To insulate the Makefile from details about the particulars of programming
# the controller, the actual programming commands are handled by the loader.py
# script in the scripts directory.
#
# See the help text for the script for information about usage and command line
# parameters.
#
flash: $(HEX_FILE)
	@python3 scripts/loader.py $<

verify: $(HEX_FILE)
	@python3 scripts/loader.py --verify $<

erase:
	@python3 scripts/loader.py --erase


# Include generated dep (.d) files if they exist.
#
-include $(OBJS:.o=.d)
