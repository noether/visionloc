#include <unistd.h>
#include <iostream>
#include <visionloc.hh>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

struct cam_properties {
    int id;
    int width;
    int height;

    std::string window;
    cv::Mat frame;
};

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage example_libvisionloc num_markers" << std::endl;
        return -1;
    }

    int nm = std::atoi(argv[1]);

    std::vector<Marker> markers;
    int tag;

    start_visionloc();

    std::vector<int> cam_ids = get_active_cameras();

    int num_cameras = cam_ids.size();
    struct cam_properties * cams = new struct cam_properties[num_cameras];

    std::cout << num_cameras << " camera(s)" << std::endl;
    for (int n = 0; n < num_cameras; n++) {
        cams[n].id = cam_ids[n];
        cams[n].width = get_cam_width(cams[n].id);
        cams[n].height = get_cam_height(cams[n].id);

        std::ostringstream oss;
        oss << "Cam" << cams[n].id;
        cams[n].window = oss.str();

        cv::namedWindow(cams[n].window, CV_WINDOW_AUTOSIZE);

        std::cout << "  Cam " << cams[n].id  << ": " << 
            cams[n].width << "x" <<
            cams[n].height << std::endl;
        set_expected_num_of_markers(cams[n].id, nm);
    }

    int i;
    for(i = 0; i < 1000; i++) {
        for (int n = 0; n < num_cameras; n++) {
            cv::Mat *grayframe = get_frame_from_camera(cams[n].id);
            cvtColor(*grayframe, cams[n].frame, CV_GRAY2RGB);
            delete grayframe;

            markers = get_markers_from_camera(cams[n].id);
            tag = get_tag(cams[n].id);
            std::cout << "Num of markers recognized on camera " << cams[n].id << ": " << markers.size();
            if (markers.size())
                std::cout << ", tag " << tag;
            std::cout << std::endl;
            for (std::vector<Marker>::iterator itm = markers.begin(); 
                    itm != markers.end(); ++itm)
            {
                std::cout << "    Marker " << (int)(itm->id) << " at ("
                    << itm->wc_corner_posX <<", "
                    << itm->wc_corner_posY <<")" << std::endl;
                std::ostringstream oss;
                oss << (int)(itm->id);

                cv::putText(cams[n].frame,
                    oss.str(),
                    cv::Point(itm->cam_center_posX, cams[n].height - itm->cam_center_posY),
                    cv::FONT_HERSHEY_PLAIN,
                    5,
                    cv::Scalar(0,255,0),
                    5);

            }
            cv::imshow(cams[n].window, cams[n].frame);
        }

        if (cv::waitKey(30) == 27){
            break;
        }
        sleep(1);
    }

    stop_visionloc();
    delete cams;
}
