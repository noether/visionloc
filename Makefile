SOURCES=visionloc.cc
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))
OBJECTS_CAM = camdmtx.o
OBJECTS_IMG = imagedmtx.o

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
CXXFLAGS_SO = -DLINUX -I/usr/include/opencv -fPIC
CXXFLAGS = -I/usr/include/opencv
CXXLIBS = -ldmtx -lpthread -lopencv_core -lopencv_imgproc -lopencv_highgui -O2
endif

all: libvisionloc.so camdmtx imagedmtx

libvisionloc.so: $(OBJECTS)
	$(CXX) $(CXXFLAGS_SO) -shared -Wl,-soname,$@ -o $@ $^ $(CXXLIBS)

camdtmx: $(OBJECTS_CAM)
	$(CXX) camdmtx.cc $(CXXFLAGS) -o $@ $(CXXLIBS)

imagedtmx: $(OBJECTS_IMG)
	$(CXX) imagedmtx.cc $(CXXFLAGS) -o $@ $(CXXLIBS)

clean: 
	rm *.o *.so camdmtx imagedmtx
