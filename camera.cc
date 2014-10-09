#include "camera.hh"
#include <dmtx.h>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <pthread.h>
#include <vector>
#include <iostream>

Camera::Camera(int id_cam, int width, int height, int expected_num_of_markers) :
    _workerTh_running(0),
    _stop_workerTh(0),
    _id_cam(id_cam),
    _tag(0),
    _expected_num_of_markers(expected_num_of_markers),
    _width(width),
    _height(height)
{

}

Camera::~Camera(void)
{

}

int Camera::get_id_cam()
{
    return _id_cam;
}

int Camera::get_tag()
{
    int copy;
    
    pthread_mutex_lock(&_mutexLocalization);
    copy = _tag;
    pthread_mutex_unlock(&_mutexLocalization);
    
    return copy;
}

std::vector<Marker> Camera::get_markers()
{
    std::vector<Marker> copy;

    pthread_mutex_lock(&_mutexLocalization);
    copy = _markers;
    pthread_mutex_unlock(&_mutexLocalization);

    return _markers;
}

void* Camera::_localization_algorithm(void)
{
    cv::VideoCapture cap(_id_cam);
    if(!cap.isOpened())
        std::cerr << "Camera " << _id_cam << 
            " is already in use" << std::endl;
    cap.set(CV_CAP_PROP_FRAME_WIDTH, _width);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, _height);

    cv::Mat greyMat(_width, _height, CV_8UC1);
    cv::Mat frame;

    DmtxImage      *img;
    DmtxDecode     *dec;
    DmtxRegion     *reg;
    DmtxMessage    *msg;
    DmtxTime        timeout;

    int smth_detected = 0;

    for(;;){

        if(_stop_workerTh)
            pthread_exit(0);

        std::vector<Marker> local_markers;

        cap >> frame;

        cvtColor(frame, greyMat, CV_BGR2GRAY);

        uint8_t *pxl = (uint8_t*)greyMat.data;
        img = dmtxImageCreate(pxl, _width, _height, DmtxPack8bppK);

        dec = dmtxDecodeCreate(img, 1);
        dmtxDecodeSetProp(dec, DmtxPropSymbolSize, DmtxSymbol10x10);

        int i;
        for(i = 0; i < _expected_num_of_markers; i++){
            // If the algorithm does not find any marker in 100ms, it skips the frame
            timeout = dmtxTimeAdd(dmtxTimeNow(), 100);
            reg = dmtxRegionFindNext(dec, &timeout);

            if(reg != NULL) {
                msg = dmtxDecodeMatrixRegion(dec, reg, DmtxUndefined);

                if(msg != NULL) {
                    smth_detected = 1;
                    Marker marker;
                    // ID of the robot
                    std::istringstream ss(reinterpret_cast<char*>(msg->output));
                    ss >> marker.id;

                    // Intersection of the two solid borders, in pixels
                    
                    DmtxVector2 p00, p10, p11, p01;
                    p00.X = p00.Y = p10.Y = p01.X = 0.0;
                    p10.X = p01.Y = p11.X = p11.Y = 1.0;

                    dmtxMatrix3VMultiplyBy(&p00, reg->fit2raw);
                    dmtxMatrix3VMultiplyBy(&p10, reg->fit2raw);
                    dmtxMatrix3VMultiplyBy(&p11, reg->fit2raw);
                    dmtxMatrix3VMultiplyBy(&p01, reg->fit2raw);

                    marker.corner_posX = p00.X;
                    marker.corner_posY = p00.Y;

                    uint16_t cx1 = (p10.X - p00.X)/2 + p00.X;
                    uint16_t cx2 = (p11.X - p01.X)/2 + p01.X;
                    uint16_t cy1 = (p01.Y - p00.Y)/2 + p00.Y;
                    uint16_t cy2 = (p11.Y - p10.Y)/2 + p10.Y;

                    marker.center_posX = (cx1 + cx2)/2;
                    marker.center_posY = (cy1 + cy2)/2;

                    // Angle in degress, w.r.t. the horizontal,
                    // of the bottom solid border, counter-clockwise
                    marker.heading = 180/M_PI * atan2(p10.Y - p00.Y, p10.X - p00.X);

                    local_markers.push_back(marker);

                    dmtxMessageDestroy(&msg);
                }
                dmtxRegionDestroy(&reg);
            }else break;
        }

		dmtxImageDestroy(&img);
		dmtxDecodeDestroy(&dec);
        
        if(smth_detected)
        {
            smth_detected = 0;
            pthread_mutex_lock(&_mutexLocalization);
            _markers = local_markers;
            _tag++;
            pthread_mutex_unlock(&_mutexLocalization);
        }
    }
}

void * Camera::_localization_algorithm_helper(void * context)
{
    return ((Camera *)context)->_localization_algorithm();
}

void Camera::run(void)
{
   if(!_workerTh_running){
        _workerTh_running = 1;
        _stop_workerTh = 0;
        pthread_create(&_worker_thread, NULL,
                &Camera::_localization_algorithm_helper, this);
    }else
        std::cout << "Vision algorithm is already running for Camera " <<
            _id_cam << std::endl;
}

void Camera::stop(void)
{
    _stop_workerTh = 1;
    pthread_join(_worker_thread, NULL);
    _workerTh_running = 0;
}
