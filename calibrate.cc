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

int main(void)
{

    std::vector<Marker> markers[2];

    start_visionloc();

    std::vector<int> cam_ids = get_active_cameras();

    if (cam_ids.size() != 2) {
	    std::cerr << "2 cameras needed, " << cam_ids.size() << " active" << std::endl;
	    stop_visionloc();
	    return -1;
    }

    struct cam_properties cams[2];

    for (int n = 0; n < 2; n++) {
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
        set_expected_num_of_markers(cams[n].id, 2);
    }

    int i;
    for(i = 0; i < 10; i++) {
        for (int n = 0; n < 2; n++) {
	
            cv::Mat *grayframe = get_frame_from_camera(cams[n].id);
	    cvtColor(*grayframe, cams[n].frame, CV_GRAY2RGB);
	    delete grayframe;

            markers[n] = get_markers_from_camera(cams[n].id);
            for (std::vector<Marker>::iterator itm = markers[n].begin(); 
                    itm != markers[n].end(); ++itm){
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
	if (markers[0].size() == 2 && markers[1].size() == 2) {
		// they must be equal
		break;
	}

        if (cv::waitKey(30) == 27){
	    stop_visionloc();
	    return 0;
        }
        sleep(1);
    }

    stop_visionloc();
    
    int Pidx, Qidx;
    if (markers[1][0].id == markers[0][0].id) {
	    Pidx=0;
    } else if (markers[1][1].id == markers[0][0].id) {
	    Pidx=1;
    } else {
	    std::cerr << "Marker " << (int)(markers[0][0].id) << " not found on camera B:" << std::endl;
	    std::cerr << "Camera A markers: " << (int)(markers[0][0].id) << ", " << (int)(markers[0][1].id) << std::endl;
	    std::cerr << "Camera B markers: " << (int)(markers[1][0].id) << ", " << (int)(markers[1][1].id) << std::endl;
	    return -1;
    }
    Qidx = Pidx^1;
    if (markers[1][Qidx].id != markers[0][1].id) {
	    std::cerr << "Marker " << (int)(markers[0][1].id) << " not found on camera B:" << std::endl;
	    std::cerr << "Camera A markers: " << (int)(markers[0][0].id) << ", " << (int)(markers[0][1].id) << std::endl;
	    std::cerr << "Camera B markers: " << (int)(markers[1][0].id) << ", " << (int)(markers[1][1].id) << std::endl;
	    return -1;
    }

    double PAx = markers[0][0].wc_center_posX;
    double PAy = markers[0][0].wc_center_posY;
    double QAx = markers[0][1].wc_center_posX;
    double QAy = markers[0][1].wc_center_posY;

    double PBx = markers[1][Pidx].cam_center_posX;
    double PBy = markers[1][Pidx].cam_center_posY;
    double QBx = markers[1][Qidx].cam_center_posX;
    double QBy = markers[1][Qidx].cam_center_posY;

    double aA = atan2(QAy-PAy, QAx-PAx);
    double aB = atan2(QBy-PBy, QBx-PBx);

    double lAcm = sqrt((PAx-QAx)*(PAx-QAx)+(PAy-QAy)*(PAy-QAy));
    double lBpx = sqrt((PBx-QBx)*(PBx-QBx)+(PBy-QBy)*(PBy-QBy));

    double angle = aB -aA;
    double resB = lBpx / lAcm;

    double offset_x = PAx - (cos(angle)*PBx/resB - sin(angle)*PBy/resB);
    double offset_y = PAy - (sin(angle)*PBx/resB + cos(angle)*PBy/resB);

    std::cout << "WCHeight: " << cams[1].height/resB << std::endl;
    std::cout << "WCOffX: " << offset_x << std::endl;
    std::cout << "WCOffy: " << offset_y << std::endl;
    std::cout << "WCOffAngle: " << angle << std::endl;

}
