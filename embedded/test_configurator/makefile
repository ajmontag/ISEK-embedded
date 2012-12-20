XML_LIBS=`pkg-config libxml++-2.6 --libs`
XML_FLAGS=`pkg-config libxml++-2.6 --cflags`
GLIB_LIBS=`pkg-config glib-2.0 --libs`
GLIB_FLAGS=`pkg-config glib-2.0 --cflags`
CXX=g++
FLAGS = -Wall -g
COMPILEONLY = $(CXX) $(FLAGS) -c $<
LINK = $(CXX) $(FLAGS) -o $@ $+

test: test_configurator.o configurator.o arduino_comm.o
	$(LINK) -lglib-2.0 $(XML_LIBS) $(GLIB_LIBS)

test_configurator.o: test_configurator.cpp ../configurator.h
	$(COMPILEONLY)

configurator.o: ../configurator.cpp ../configurator.h
<<<<<<< .mine
	$(COMPILEONLY) $(XML_FLAGS) $(GLIB_FLAGS)

arduino_comm.o: ../arduino_comm.c ../arduino_comm.h
	$(COMPILEONLY)
=======
	$(COMPILEONLY) $(XML_FLAGS) $(GLIB_FLAGS)

arduino_comm.o: ../arduino_comm.c ../arduino_comm.h
	$(COMPILEONLY)

clean:
	rm *.o
	rm test>>>>>>> .r83
