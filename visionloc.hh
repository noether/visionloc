#ifndef VISIONLOC_MULTI_HH
#define VISIONLOC_MULTI_HH 1

#include <vector>
#include <stdint.h>
#include <opencv2/core/core.hpp>

struct Marker
{
    uint8_t id;
    uint16_t cam_corner_posX;
    uint16_t cam_corner_posY;
    uint16_t cam_center_posX;
    uint16_t cam_center_posY;
    double cam_heading;
    double wc_heading;
    double wc_corner_posX;
    double wc_corner_posY;
    double wc_center_posX;
    double wc_center_posY;
};

extern "C" void start_visionloc();
extern "C" void stop_visionloc();

extern "C" double* read_camera(int);
extern "C" double* read_all_cameras();

extern "C" int get_tag(int);

extern "C" int set_expected_num_of_markers(int num_camera, int num_markers);

extern "C" int get_cam_width(int cam);
extern "C" int get_cam_height(int cam);

std::vector<int> get_active_cameras(void);

std::vector<Marker> get_markers_from_camera(int);
cv::Mat* get_frame_from_camera(int);		// Creates an object cv::Mat. Must be deleted by the caller.

#endif
