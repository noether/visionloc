#include <unistd.h>
#include <vector>
#include <stdexcept>
#include <opencv2/core/core.hpp>
#include "camera.hh"
#include "misc.hh"

#include <iostream>

double out[256];
int tag;
std::vector<Camera> cameras;

extern "C"
void start_visionloc()
{
    parser_xml_to_cameras(&cameras);
    for (std::vector<Camera>::iterator it = cameras.begin() ; it != cameras.end(); ++it)
    {
        sleep(1);
        it->run();
    }
}

extern "C"
void stop_visionloc()
{
    for (std::vector<Camera>::iterator it = cameras.begin() ; it != cameras.end(); ++it) {
        it->stop();
    }
    cameras.clear();
}

extern "C"
double* read_camera(int num_camera)
{
    int j = 0;

    for (std::vector<Camera>::iterator it = cameras.begin() ; it != cameras.end(); ++it)
    {
        if(it->get_id_cam() == num_camera)
        {
            //int num_recog;
            std::vector<Marker> m(it->get_markers());

            out[j++] = it->get_id_cam();
            out[j++] = m.size();

            for (std::vector<Marker>::iterator itm = m.begin(); itm != m.end(); ++itm){
                out[j++] = itm->id;
                out[j++] = itm->cam_corner_posX;
                out[j++] = itm->cam_corner_posY;
                out[j++] = itm->cam_center_posX;
                out[j++] = itm->cam_center_posY;
                out[j++] = itm->cam_heading;
            }

            break;
        }
    }
    return out;
}

extern "C"
double* read_all_cameras()
{
    int j = 0;

    for (std::vector<Camera>::iterator it = cameras.begin() ; it != cameras.end(); ++it)
    {
        //int num_recog;
        std::vector<Marker> m(it->get_markers());

        out[j++] = it->get_id_cam();
        out[j++] = m.size();

        for (std::vector<Marker>::iterator itm = m.begin(); itm != m.end(); ++itm){
            out[j++] = itm->id;
            out[j++] = itm->cam_corner_posX;
            out[j++] = itm->cam_corner_posY;
            out[j++] = itm->cam_center_posX;
            out[j++] = itm->cam_center_posY;
            out[j++] = itm->cam_heading;
        }
    }

    return out;
}

extern "C"
int get_tag(int num_camera)
{
    for (std::vector<Camera>::iterator it = cameras.begin() ; it != cameras.end(); ++it)
    {
        if(it->get_id_cam() == num_camera)
        {
            return it->get_tag();
        }
    }
    return -1;
}

extern "C"
int set_expected_num_of_markers(int num_camera, int num_markers)
{
    for (std::vector<Camera>::iterator it = cameras.begin() ; it != cameras.end(); ++it)
    {
        if(it->get_id_cam() == num_camera)
        {
            it->set_expected_num_of_markers(num_markers);
            return 0;
        }
    }
    return -1;
}

extern "C" int get_cam_width(int num_camera)
{
    for (std::vector<Camera>::iterator it = cameras.begin() ; it != cameras.end(); ++it)
    {
        if(it->get_id_cam() == num_camera)
        {
            return it->get_width();
        }
    }
    return -1;
}

extern "C" int get_cam_height(int num_camera)
{
    for (std::vector<Camera>::iterator it = cameras.begin() ; it != cameras.end(); ++it)
    {
        if(it->get_id_cam() == num_camera)
        {
            return it->get_height();
        }
    }
    return -1;
}

std::vector<int> get_active_cameras(void)
{
    std::vector<int> cam_ids;
    for (std::vector<Camera>::iterator it = cameras.begin() ; it != cameras.end(); ++it)
    {
        cam_ids.push_back(it->get_id_cam());
    }
    return cam_ids;
}

std::vector<Marker> get_markers_from_camera(int num_camera)
{

    for (std::vector<Camera>::iterator it = cameras.begin() ; it != cameras.end(); ++it)
    {
         if(it->get_id_cam() == num_camera)
             return it->get_markers();
    }
    throw std::invalid_argument("get_markers_from_camera: camera not found");
}

cv::Mat* get_frame_from_camera(int num_camera)
{
    for (std::vector<Camera>::iterator it = cameras.begin() ; it != cameras.end(); ++it)
    {
        if(it->get_id_cam() == num_camera)
            return it->get_frame();
    }
    throw std::invalid_argument("get_frame_fromcamera not found");
}
