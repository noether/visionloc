LIB_OBJECTS = visionloc.o camera.o parser.o

CXXFLAGS = -DLINUX -I/usr/include/opencv -O2 -Wall 
CXXFLAGS_SO = $(CXXFLAGS) -fPIC
CXXLIBS = -ldmtx -lpthread -lopencv_core -lopencv_imgproc -lopencv_highgui -ltinyxml2 

.PHONY: all clean install

all: libvisionloc.so imagedmtx example_libvisionloc calibrate

%.o: %.cc
	$(CXX) $(CXXFLAGS_SO) -c -o $@ $^

libvisionloc.so: $(LIB_OBJECTS)
	$(CXX) $(CXXLIBS) -shared -Wl,-soname,$@ -o $@ $^ 


imagedmtx: imagedmtx.cc 
	$(CXX) $(CXXFLAGS) $(CXXLIBS) -o $@ imagedmtx.cc

example_libvisionloc: example_libvisionloc.cc
	$(CXX) $(CXXFLAGS) -lvisionloc -lopencv_core -lopencv_imgproc -lopencv_highgui -o $@ $^

calibrate: calibrate.cc
	$(CXX) $(CXXFLAGS) -lvisionloc -lopencv_core -lopencv_imgproc -lopencv_highgui -o $@ $^

clean: 
	rm *.o *.so imagedmtx example_libvisionloc calibrate

install: 
	rm -f /usr/local/lib/libvisionloc.so
	ln -s `pwd`/libvisionloc.so /usr/local/lib/libvisionloc.so
	rm -f /usr/local/include/visionloc.hh
	ln -s `pwd`/visionloc.hh /usr/local/include/visionloc.hh
