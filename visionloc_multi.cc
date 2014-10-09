#include <dmtx.h>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <pthread.h>
#include <vector>
#include <iostream>
#include "camera.hh"
#include "misc.hh"

double out[256];
std::vector<Camera> cameras;

extern "C"
void start_visionloc()
{
    parser_xml_to_cameras(&cameras);
    for (std::vector<Camera>::iterator it = cameras.begin() ; it != cameras.end(); ++it)
        it->run();
}

extern "C"
void stop_visionloc()
{
    for (std::vector<Camera>::iterator it = cameras.begin() ; it != cameras.end(); ++it)
        it->stop();
}

extern "C"
double* read_camera(int num_camera)
{
    int j = 0;

    for (std::vector<Camera>::iterator it = cameras.begin() ; it != cameras.end(); ++it)
    {
        if(it->get_id_cam() == num_camera)
        {
            int num_recog;
            std::vector<Marker> m(it->get_markers());

            out[j++] = it->get_id_cam();
            out[j++] = m.size();

            for (std::vector<Marker>::iterator itm = m.begin(); itm != m.end(); ++itm){
                out[j++] = itm->id;
                out[j++] = itm->corner_posX;
                out[j++] = itm->corner_posY;
                out[j++] = itm->center_posX;
                out[j++] = itm->center_posY;
                out[j++] = itm->heading;
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
        int num_recog;
        std::vector<Marker> m(it->get_markers());

        out[j++] = it->get_id_cam();
        out[j++] = m.size();

        for (std::vector<Marker>::iterator itm = m.begin(); itm != m.end(); ++itm){
            out[j++] = itm->id;
            out[j++] = itm->corner_posX;
            out[j++] = itm->corner_posY;
            out[j++] = itm->center_posX;
            out[j++] = itm->center_posY;
            out[j++] = itm->heading;
        }
    }

    return out;
}

std::vector<Marker> get_markers_from_camera(int num_camera)
{

    for (std::vector<Camera>::iterator it = cameras.begin() ; it != cameras.end(); ++it)
    {
         if(it->get_id_cam() == num_camera)
             return it->get_markers();
    }
}
