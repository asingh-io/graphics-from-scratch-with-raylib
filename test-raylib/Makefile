RAYLIB_PATH ?= ../raylib/src

CC = gcc
# INC = -I$(RAYLIB_PATH)/src
#CFLAGS = -Wall -std=c99 -g $(INC)

LFLAGS = -L$(RAYLIB_PATH)

LIBS = -lraylib -lm

# OBJS = $(SRCS:.c=.o)

TARGET ?= test-raylib 
SRC_DIRS ?= ./

SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(RAYLIB_PATH) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS ?= $(INC_FLAGS) -Wall -std=c99 -g -MMD -MP

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LOADLIBES) $(LFLAGS) $(LIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

-include $(DEPS)
