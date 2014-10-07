SOURCE = visionloc.cc
SOURCES_MULTI = misc.cc camera.cc visionloc_multi.cc
OBJECTS_CAM = camdmtx.o
OBJECTS_IMG = imagedmtx.o

CXXFLAGS_SO = -DLINUX -I/usr/include/opencv -fPIC
CXXFLAGS = -I/usr/include/opencv
CXXLIBS = -ldmtx -lpthread -lopencv_core -lopencv_imgproc -lopencv_highgui -O2

all: libvisionloc_multi.so libvisionloc.so camdmtx imagedmtx

libvisionloc.so: $(SOURCES)
	$(CXX) $(CXXFLAGS_SO) -shared -Wl,-soname,$@ -o $@ $^ $(CXXLIBS)

libvisionloc_multi.so: $(SOURCES_MULTI)
	$(CXX) $(CXXFLAGS_SO) -shared -Wl,-soname,$@ -o $@ $^ $(CXXLIBS)

camdmtx: $(OBJECTS_CAM)
	$(CXX) camdmtx.cc $(CXXFLAGS) -o $@ $(CXXLIBS)

imagedmtx: $(OBJECTS_IMG)
	$(CXX) imagedmtx.cc $(CXXFLAGS) -o $@ $(CXXLIBS)

clean: 
	rm *.o *.so camdmtx imagedmtx
