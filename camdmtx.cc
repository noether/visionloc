#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <dmtx.h>
#include <math.h>
#include <iostream>

int main(int argc, char** argv)
{
    int width = 1600;
    int height = 1200;

    cv::VideoCapture cap(0);
    if(!cap.isOpened())
        return -1;
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

        cap >> frame;

        cvtColor(frame, greyMat, CV_BGR2GRAY);

        uint8_t *pxl = (uint8_t*)greyMat.data;

        img = dmtxImageCreate(pxl, width, height, DmtxPack8bppK);
        dec = dmtxDecodeCreate(img, 1);

        // If the algorithm does not find any robot in 100ms, it skips the frame
        timeout = dmtxTimeAdd(dmtxTimeNow(), 100);
        reg = dmtxRegionFindNext(dec, &timeout);

        if(reg != NULL) {
            std::cout << "Found: " << std::endl;
            msg = dmtxDecodeMatrixRegion(dec, reg, DmtxUndefined);

            if(msg != NULL) {
                // ID of the robot
                std::cout << "ID: " << msg->output << std::endl;

                // Intersection of the two solid borders, in pixels
                std::cout << "Position: " << reg->bottomLoc.X << ", " <<
                    reg->bottomLoc.Y << std::endl;

                // Angle in degress, w.r.t. the horizontal,
                // of the bottom solid border, counter-clockwise
                std::cout << "Bottom Line angle atan2: " <<
                    atan2(reg->bottomLine.locNeg.Y 
                            - reg->bottomLine.locPos.Y, reg->bottomLine.locNeg.X 
                            - reg->bottomLine.locPos.X)*180/M_PI;
                std::cout << std::endl;
            }
            dmtxMessageDestroy(&msg);
        }

        dmtxRegionDestroy(&reg);
        dmtxImageDestroy(&img);
        dmtxDecodeDestroy(&dec);
        cv::namedWindow("frame", CV_WINDOW_AUTOSIZE);
        cv::imshow("frame", greyMat);

        if (cv::waitKey(30) == 27)
            break; 
    }
    
    cap.release();
    cv::destroyAllWindows();

    return 0;
}

