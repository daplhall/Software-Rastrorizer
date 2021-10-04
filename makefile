SRC_DIR  := ./src
INC_DIR  := ./include
BIN_DIR  := ./bin/release
DBIN_DIR := ./bin/debug
OBJ_DIR  := ./obj/release
DOBJ_DIR := ./obj/debug

EXE      := rastorizor.exe

CC       := g++
LINKER   := g++
# -fopt-info-optimized=Auto_Vec
CPPFLAGS := -I$(INC_DIR) -Wall
LFALGS   := -I$(INC_DIR)
DBFLAG   := -O0 -g -pg
DBLFLAG  := -pg

SOURCES  := $(wildcard $(SRC_DIR)/*.cpp)
INCLUDES := $(wildcard $(INC_DIR)/*.h)
OBJECTS  := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DOBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(DOBJ_DIR)/%.o)
#-fkeep-inline-functions 
.PHONEY: clean all run debug gdb ani test plot
# -D<name> defines as macro, we can progam such that we can react to stuff.
all: CPPFLAGS += -O3 -DNDEBUG # -DNDEBUG disables asserts, like says #deifne NDEBUG
all: $(BIN_DIR)/$(EXE)

debug: $(DBIN_DIR)/$(EXE)

# DEBUG
$(DBIN_DIR)/$(EXE): $(DOBJECTS)
	$(LINKER) $(DOBJECTS) $(DBLFLAG) -o $@

$(DOBJECTS): $(DOBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) $(DBFLAG) -c $< -o $@

# RELEASE
$(BIN_DIR)/$(EXE): $(OBJECTS)
	$(LINKER) $(OBJECTS) -o $@

$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@
#####

run:
	
clean:
	rm $(OBJECTS) $(DOBJECTS) $(BIN_DIR)/$(EXE) $(DBIN_DIR)/$(EXE)
