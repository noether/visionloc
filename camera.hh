#ifndef CAMERA_HH
#define CAMERA_HH 1

#include "visionloc.hh"
#include <opencv2/core/core.hpp>
#include <vector>

class Camera {

    private:
        int _id_cam;
        int _tag;
        int _width;
        int _height;
        double _resolution;
        double _wc_offset_x;
        double _wc_offset_y;
        double _wc_offset_angle;
        int _expected_num_of_markers;
        std::vector<Marker> _markers;
        cv::Mat _greyMat;

        double _sin_a;
        double _cos_a;

        pthread_mutex_t _mutexLocalization;
        pthread_mutex_t _mutexFrame;
        pthread_t _worker_thread;
        int _workerTh_running;
        volatile int _stop_workerTh;
        void * _localization_algorithm(void);
        static void * _localization_algorithm_helper(void *);

    public:
        Camera(int, int, int, double, double, double, double);
        ~Camera(void);

        int get_id_cam();
        int get_width();
        int get_height();
        int get_tag();
        std::vector<Marker> get_markers();
        cv::Mat* get_frame();		// Creates an object cv::Mat. Must be deleted by the caller.

        void set_expected_num_of_markers(int);


        void run(void);
        void stop(void);
};

#endif
