#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <dmtx.h>

#include <math.h>
#include <iostream>

int main(int argc, char** argv)
{
    int width = 320;
    int height = 200;

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
                    atan2(reg->bottomLine.locNeg.Y - reg->bottomLine.locBeg.Y,
                            reg->bottomLine.locNeg.X - reg->bottomLine.locBeg.X)
                    *180/M_PI << std::endl;
                std::cout << std::endl;
                            }
            dmtxRegionDestroy(&reg);
        }

        uint8_t c = cvWaitKey(33);
        if(c == 27)
            break;
    }

    return 0;
}

