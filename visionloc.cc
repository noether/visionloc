#include <dmtx.h>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <pthread.h>
#include <vector>
#include <iostream>

void* localization_cam(void *ptr);
double out[256];

struct Marker
{
    uint8_t id;
    uint16_t corner_posX;
    uint16_t corner_posY;
    uint16_t center_posX;
    uint16_t center_posY;
    double heading;
};

typedef struct str_Arg_localization_cam
{
    int numMarkers;
    int width;
    int height;
}Arg_localization_cam;

Arg_localization_cam arg_th;


uint8_t numRecog;
std::vector<Marker> markers;

pthread_t worker_thread;

pthread_mutex_t mutexLocalization = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexStopThread = PTHREAD_MUTEX_INITIALIZER;
int stopWorkerTh = 0;
int worker_running = 0;

extern "C"
void start_visionloc(int numMarkers, int width, int height)
{
    arg_th.numMarkers = numMarkers;
    arg_th.width = width;
    arg_th.height = height;

    if(!worker_running){
        worker_running = 1;
        stopWorkerTh = 0;
        pthread_create(&worker_thread, NULL,  &localization_cam,
                &arg_th);
    }else
        std::cerr << "Vision thread already is running";

}

void* localization_cam(void *ptr)
{
    Arg_localization_cam *arg;
    arg = (Arg_localization_cam *) ptr;

    int width = arg->width;
    int height = arg->height;

    cv::VideoCapture cap(0);
    if(!cap.isOpened())
        std::cerr << "Camera is already in use" << std::endl;
    cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);

    cv::Mat greyMat(width, height, CV_8UC1);
    cv::Mat frame;

    DmtxImage      *img;
    DmtxDecode     *dec;
    DmtxRegion     *reg;
    DmtxMessage    *msg;
    DmtxTime        timeout;


    for(;;){

        std::vector<Marker> local_markers;

        pthread_mutex_lock(&mutexStopThread);
        if(stopWorkerTh){
            pthread_mutex_unlock(&mutexStopThread);
            pthread_exit(0);
        }
        pthread_mutex_unlock(&mutexStopThread);

        cap >> frame;

        cvtColor(frame, greyMat, CV_BGR2GRAY);

        uint8_t *pxl = (uint8_t*)greyMat.data;
        img = dmtxImageCreate(pxl, width, height, DmtxPack8bppK);

        dec = dmtxDecodeCreate(img, 1);
        dmtxDecodeSetProp(dec, DmtxPropSymbolSize, DmtxSymbol10x10);

        uint8_t local_numRecog = 0;
        int i;
        for(i = 0; i < arg->numMarkers; i++){
            // If the algorithm does not find any marker in 100ms, it skips the frame
            timeout = dmtxTimeAdd(dmtxTimeNow(), 100);
            reg = dmtxRegionFindNext(dec, &timeout);

            if(reg != NULL) {
                local_numRecog++;
                msg = dmtxDecodeMatrixRegion(dec, reg, DmtxUndefined);

                if(msg != NULL) {
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

        pthread_mutex_lock(&mutexLocalization);
        markers = local_markers;
        numRecog = local_numRecog;
        pthread_mutex_unlock(&mutexLocalization);
    }
}

extern "C"
void stop_visionloc()
{
    pthread_mutex_lock(&mutexStopThread);
    stopWorkerTh = 1;
    pthread_mutex_unlock(&mutexStopThread);

    pthread_join(worker_thread, NULL);
    worker_running = 0;
}

extern "C"
double* read_visionloc()
{
    std::vector<Marker> copy;
    uint8_t numMarkers;
    pthread_mutex_lock(&mutexLocalization);
    copy = markers;
    numMarkers = numRecog;
    pthread_mutex_unlock(&mutexLocalization);

    uint8_t j = 0;
    out[j++] = numMarkers;

    for (std::vector<Marker>::iterator it = copy.begin(); it != copy.end(); ++it){
        out[j++] = it->id;
        out[j++] = it->corner_posX;
        out[j++] = it->corner_posY;
        out[j++] = it->center_posX;
        out[j++] = it->center_posY;
        out[j++] = it->heading;
    }
    
    return out;
}
