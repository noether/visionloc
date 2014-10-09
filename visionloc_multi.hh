#include <vector>
#include <stdint.h>

struct Marker
{
    uint8_t id;
    uint16_t corner_posX;
    uint16_t corner_posY;
    uint16_t center_posX;
    uint16_t center_posY;
    double heading;
};

extern "C" void start_visionloc();
extern "C" void stop_visionloc();

extern "C" double* read_camera(int);
extern "C" double* read_all_cameras();

extern "C" int get_tag(int);

std::vector<Marker> get_markers_from_camera(int);
