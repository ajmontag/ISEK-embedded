
CC = /usr/bin/clang++

FLAGS = -Wall -g

COMPILEONLY = $(CC) $(FLAGS) -c $<

LINK = $(CC) $(FLAGS) -o $@ $+

SERIAL_FLAGS = `pkg-config --libs --cflags libserial`

all: test_main

test_main: test_main.o arduino_comm.o
	$(LINK) $(SERIAL_FLAGS) 

test_main.o: test_main.cpp ../TwoAxisControl.h ../arduino_comm.h
	$(COMPILEONLY)

arduino_comm.o: ../arduino_comm.c ../arduino_comm.h
	$(COMPILEONLY)

# utils
clean: 
	rm test_main *.o
