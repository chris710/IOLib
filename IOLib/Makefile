CC=gcc
PEDANTIC_PARANOID_FREAK =       -g  
REASONABLY_CAREFUL_DUDE =	-Wall
NO_PRAYER_FOR_THE_WICKED =	-w
WARNINGS = 			$(PEDANTIC_PARANOID_FREAK)

CFLAGS = -g -O2 $(WARNINGS)
LDFLAGS= 

C_FILES = $(wildcard *.c)
BIN_FILES = $(patsubst %.c,%,$(C_FILES))
O_FILES = $(patsubst %.c,%.o,$(C_FILES))

all: test_formatted test_buffered 

test_buffered: test_buffered.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@
	echo "Please specify the files"

test_formatted: test_formatted.c
	$(CC) $(CFLAGS) $(LDFLAGS) test_formatted.c -o $@
	echo "Please specify the files"

clean:
	rm -f $(BIN_FILES) *.o *~ *.so 
