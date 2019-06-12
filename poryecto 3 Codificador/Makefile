#MakeFile

TARGETS= desencriptadorE
CROSS_TOOL =
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -Wall -g -std=gnu99 -pthread
LDFLAGS = -lm

all: clean $(TARGETS)

$(TARGETS):
	$(CC_C) $(CFLAGS) $@.c -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGETS)
