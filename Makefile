SOURCES=visionloc.cc
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))
OBJECTS_CAM = camdtmx.o
OBJECTS_IMG = imagedtmx.o

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
CXXFLAGS_SO = -DLINUX -I/usr/include/opencv -fPIC
CXXFLAGS = -I/usr/include/opencv
CXXLIBS = -ldmtx -lpthread -lopencv_core -lopencv_imgproc -lopencv_highgui -O2
endif

all: libvisionloc.so camdtmx imagedtmx

libvisionloc.so: $(OBJECTS)
	$(CXX) $(CXXFLAGS_SO) -shared -Wl,-soname,$@ -o $@ $^ $(CXXLIBS)

camdtmx: $(OBJECTS_CAM)
	$(CXX) camdtmx.cc $(CXXFLAGS) -o $@ $(CXXLIBS)

imagedtmx: $(OBJECTS_IMG)
	$(CXX) imagedtmx.cc $(CXXFLAGS) -o $@ $(CXXLIBS)

clean: 
	rm *.o *.so camdtmx imagedtmx
