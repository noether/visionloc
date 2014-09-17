SOURCES=visionloc.cc
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
CXXFLAGS = -DLINUX -I/usr/include/opencv -fPIC
CXXLIBS = -ldmtx -lpthread -lopencv_core -lopencv_imgproc -lopencv_highgui -O2
endif

libvisionloc.so: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -shared -Wl,-soname,$@ -o $@ $^ $(CXXLIBS)

clean: 
	rm *.o *.so
