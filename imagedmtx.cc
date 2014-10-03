#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <dmtx.h>
#include <math.h>
#include <string>
#include <cstdlib>


int main(int argc, char** argv)
{
    std::string nm(argv[2]);
    int numOfMarkers = atoi(nm.c_str());
    cv::Mat image = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

    DmtxImage      *img;
    DmtxDecode     *dec;
    DmtxRegion     *reg;
    DmtxMessage    *msg;


    if(argc != 3){
        std::cout << "Usage imagedmtx path_to_image numOfMarkers_to_search" << std::endl;
        return -1;
    }

    cv::imshow("Diplay Image", image);

    uint8_t *pxl = (uint8_t*)image.data;
    img = dmtxImageCreate(pxl, image.size().width, image.size().height, DmtxPack8bppK);

    dec = dmtxDecodeCreate(img, 1);
    dmtxDecodeSetProp(dec, DmtxPropSymbolSize, DmtxSymbol10x10);

    std::cout << "Treshold: " << 
        dmtxDecodeGetProp(dec, DmtxPropEdgeThresh)
        << std::endl;

    int i; // Number of robots to search

    for(i = 0; i < numOfMarkers; i++){
        reg = dmtxRegionFindNext(dec, NULL);
        if(reg != NULL) {
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
    }
    dmtxDecodeDestroy(&dec);
    dmtxImageDestroy(&img);

    return 0;
}

