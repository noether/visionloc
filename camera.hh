#ifndef CAMERA_HH
#define CAMERA_HH 1

#include <dmtx.h>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <pthread.h>
#include <vector>
#include <iostream>

struct Marker
{
    uint8_t id;
    uint16_t corner_posX;
    uint16_t corner_posY;
    uint16_t center_posX;
    uint16_t center_posY;
    double heading;
};

class Camera {
    
    private:
        int _id_cam;
        int _width;
        int _height;
        int _expected_num_of_markers;
        int _num_markers_recog;
        std::vector<Marker> _markers;

        pthread_mutex_t _mutexLocalization;
        pthread_t _worker_thread;
        int _workerTh_running;
        volatile int _stop_workerTh;
        void * _localization_algorithm(void);
        static void * _localization_algorithm_helper(void *);

    public:
        Camera(int, int, int, int);
        ~Camera(void);

        int get_id_cam();
        std::vector<Marker> get_markers(int *);
        
        void run(void);
        void stop(void);
};

#endif
