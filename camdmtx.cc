#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <dmtx.h>
#include <math.h>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    int width;
    int height;

    cv::Mat frame;
    cv::VideoCapture cap;
    int frnb = 0;

    if(argc == 2){
        cv::VideoCapture _cap(argv[1]);
        cap = _cap;
        cap >> frame;
        width = frame.size().width;
        height = frame.size().height;
        frnb = cap.get(CV_CAP_PROP_FRAME_COUNT);
    }

    if(argc == 3){
        std::string ws(argv[1]);
        std::string hs(argv[2]);
        width = atoi(ws.c_str());
        height = atoi(hs.c_str());

        cv::VideoCapture _cap(0);
        cap = _cap;

        if(!cap.isOpened())
            return -1;
        cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);
    }

    cv::Mat greyMat(width, height, CV_8UC1);

    DmtxImage      *img;
    DmtxDecode     *dec;
    DmtxRegion     *reg;
    DmtxMessage    *msg;
    DmtxTime        timeout;


    for(;;){

        if(argc == 2)
        {
            int frid = cap.get(CV_CAP_PROP_POS_FRAMES);
            if((frnb-3) <= frid)
                break;
        }

        cap >> frame;
        cvtColor(frame, greyMat, CV_BGR2GRAY);

        uint8_t *pxl = (uint8_t*)greyMat.data;

        img = dmtxImageCreate(pxl, width, height, DmtxPack8bppK);
        dec = dmtxDecodeCreate(img, 1);
        dmtxDecodeSetProp(dec, DmtxPropSymbolSize, DmtxSymbol10x10);

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
                double rotate;
                DmtxVector2 p00, p10, p11, p01;
                p00.X = p00.Y = p10.Y = p01.X = 0.0;
                p10.X = p01.Y = p11.X = p11.Y = 1.0;

                dmtxMatrix3VMultiplyBy(&p00, reg->fit2raw);
                dmtxMatrix3VMultiplyBy(&p10, reg->fit2raw);
                dmtxMatrix3VMultiplyBy(&p11, reg->fit2raw);
                dmtxMatrix3VMultiplyBy(&p01, reg->fit2raw);
                rotate = 180 / M_PI * atan2(p10.Y - p00.Y, p10.X - p00.X);

                std::cout << "Corner Position: " << (int)p00.X << ", " <<
                    (int)p00.Y << std::endl;

                uint16_t cx1 = (p10.X - p00.X)/2 + p00.X;
                uint16_t cx2 = (p11.X - p01.X)/2 + p01.X;
                uint16_t cy1 = (p01.Y - p00.Y)/2 + p00.Y;
                uint16_t cy2 = (p11.Y - p10.Y)/2 + p10.Y;

                uint16_t center_posX = (cx1 + cx2)/2;
                uint16_t center_posY = (cy1 + cy2)/2;

                std::cout << "Center Position: " << center_posX << ", " <<
                    center_posY << std::endl;

                // Angle in degress, w.r.t. the horizontal,
                // of the bottom solid border, counter-clockwise
                std::cout << "Bottom Line angle " << rotate << std::endl;
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

