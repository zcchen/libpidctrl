# Program Name
LIB_TARGET_NAME = pidctrl

# Code Setting
BUILD_DIR       = build
SRC_DIR         = src
HEAD_DIR        = inc
TEST_DIR        = tests

LIB_SOURCES     = $(wildcard $(SRC_DIR)/*.c)
LIB_OBJECTS     = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(LIB_SOURCES))

TEST_SOURCES    = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS    = $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/test-%, $(TEST_SOURCES))

LIB_TARGET      = lib$(LIB_TARGET_NAME).a

# Compiler Setting
CC_PREFIX       =
CC_CFLAGS       =
CC_LDFLAGS      =

# Compiling Setting
OPTIMIZE        = -O2
DEFS            =
LIBS            =
CC              = $(CC_PREFIX)gcc
CXX             = $(CC_PREFIX)g++
AR              = $(CC_PREFIX)ar
OBJCOPY         = $(CC_PREFIX)objcopy
OBJDUMP         = $(CC_PREFIX)objdump
STD_FLAGS       = -std=c99
CXXFLAGS        =
override CFLAGS  = -Wall $(STD_FLAGS) $(OPTIMIZE) $(DEFS) -I${HEAD_DIR} $(CFLAGS_AVR)

all: $(BUILD_DIR) $(LIB_TARGET)

test: $(BUILD_DIR) $(LIB_TARGET) $(TEST_OBJECTS)

$(BUILD_DIR):
	-mkdir -p $(BUILD_DIR)

$(LIB_TARGET): $(LIB_OBJECTS)
	$(AR) rcs $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(STD_FLAGS) $(LDFLAGS) $< -c -o $@

$(BUILD_DIR)/test-%: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) $(STD_FLAGS) $(LDFLAGS) -lm $^ -L. -l$(LIB_TARGET_NAME) -o $(@)

clean:
	-rm -rf $(BUILD_DIR)
	-rm -rf $(LIB_TARGET)

