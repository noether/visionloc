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
    uint16_t posX;
    uint16_t posY;
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
                    marker.posX = reg->bottomLine.locPos.X;
                    marker.posY = reg->bottomLine.locPos.Y;

                    // Angle in degress, w.r.t. the horizontal,
                    // of the bottom solid border, counter-clockwise
                    marker.heading = atan2(reg->bottomLine.locNeg.Y 
                            - reg->bottomLine.locPos.Y, reg->bottomLine.locNeg.X 
                            - reg->bottomLine.locPos.X)*180/M_PI;

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
        out[j++] = it->posX;
        out[j++] = it->posY;
        out[j++] = it->heading;
    }
    
    return out;
}
