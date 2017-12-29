LIB_OBJECTS = visionloc.o camera.o parser.o

CXXFLAGS = -DLINUX -I/usr/include/opencv -O2 -Wall 
CXXFLAGS_SO = $(CXXFLAGS) -fPIC
CXXLIBS = -ldmtx -lpthread -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lopencv_imgcodecs -ltinyxml2

.PHONY: all clean install

all: libvisionloc.so imagedmtx example_libvisionloc calibration_2_cameras

%.o: %.cc
	$(CXX) $(CXXFLAGS_SO) -c -o $@ $^

libvisionloc.so: $(LIB_OBJECTS)
	$(CXX) -shared -Wl,-soname,$@ -o $@ $^ $(CXXLIBS) 

imagedmtx: imagedmtx.cc
	$(CXX) $(CXXFLAGS) -o $@ imagedmtx.cc $(CXXLIBS)

example_libvisionloc: example_libvisionloc.cc
	$(CXX) $(CXXFLAGS) -o $@ $^ -lvisionloc -lopencv_core -lopencv_imgproc -lopencv_highgui

calibration_2_cameras: calibration_2_cameras.cc
	$(CXX) $(CXXFLAGS) -o $@ $^ -lvisionloc -lopencv_core -lopencv_imgproc -lopencv_highgui

clean: 
	rm *.o *.so imagedmtx example_libvisionloc calibrate

install: 
	rm -f /usr/local/lib/libvisionloc.so
	ln -s `pwd`/libvisionloc.so /usr/local/lib/libvisionloc.so
	rm -f /usr/local/include/visionloc.hh
	ln -s `pwd`/visionloc.hh /usr/local/include/visionloc.hh
