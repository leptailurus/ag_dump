APP=ag_dump

SRC_DIR=src
INC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin

CC=gcc
LD=gcc
CFLAGS=-O2 -c -Wall -pedantic -std=c11
LFLGAS=
DFLAGS=-g3 -O0 -DDEBUG
INCFLAGS=-I$(INC_DIR)

SOURCES=$(wildcard $(SRC_DIR)/*.c)
HEADERS=$(wildcard $(INC_DIR)/*.h)
OBJECTS=$(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPENDS=$(OBJ_DIR)/.depends


.PHONY: all
all: $(BIN_DIR)/$(APP)

.PHONY: debug
debug: CFLAGS+=$(DFLAGS)
debug: all


$(BIN_DIR)/$(APP): $(OBJECTS) | $(BIN_DIR)
	$(LD) $(LFLGAS) -o $@ $^

$(OBJ_DIR)/%.o: | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCFLAGS) -o $@ $<

$(DEPENDS): $(SOURCES) | $(OBJ_DIR)
	$(CC) $(INCFLAGS) -MM $(SOURCES) | sed -e 's!^!$(OBJ_DIR)/!' >$@

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif


$(BIN_DIR):
	mkdir -p $@
$(OBJ_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
