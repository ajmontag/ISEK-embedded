OPENCV_FLAGS=`pkg-config --cflags opencv`
OPENCV_LIBS= `pkg-config --libs opencv`
XML_LIBS=`pkg-config libxml++-2.6 --libs`
XML_FLAGS=`pkg-config libxml++-2.6 --cflags`
GLIB_LIBS=`pkg-config glib-2.0 --libs`
GLIB_FLAGS=`pkg-config glib-2.0 --cflags`
FLAGS = -Wall -g
COMPILEONLY = $(CXX) $(FLAGS) -c $<
LINK = $(CXX) $(FLAGS) -o $@ $+

all: facedetect calibrate

calibrate: calibrate.o arduino_comm.o configurator.o
	$(LINK) -lm $(OPENCV_LIBS) $(GLIB_LIBS) $(XML_LIBS)

calibrate.o: calibrate.cpp TwoAxisControl.h TwoAxisCalibration.h configurator.h
	$(COMPILEONLY) $(OPENCV_FLAGS) $(GLIB_FLAGS) $(XML_FLAGS)

facedetect: facedetect.o arduino_comm.o configurator.o
	$(LINK) -lm -lglib-2.0 $(OPENCV_LIBS) $(XML_LIBS) $(GLIB_LIBS)
 
facedetect.o: facedetect.cpp TwoAxisControl.h configurator.h
	$(COMPILEONLY) $(OPENCV_FLAGS) $(GLIB_FLAGS) $(XML_FLAGS)

arduino_comm.o: arduino_comm.c arduino_comm.h
	$(COMPILEONLY)

configurator.o: configurator.cpp configurator.h TwoAxisControl.h TwoAxisCalibration.h
	$(COMPILEONLY) $(XML_FLAGS) $(GLIB_FLAGS)

clean:
	rm *.o
	rm facedetect
	rm calibrate
