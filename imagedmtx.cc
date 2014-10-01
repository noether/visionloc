#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <dmtx.h>
#include <math.h>


int main(int argc, char** argv)
{
    int width = 1600;
    int height = 1200;

    cv::Mat image(width, height, CV_8UC1);
    image = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

    DmtxImage      *img;
    DmtxDecode     *dec;
    DmtxRegion     *reg;
    DmtxMessage    *msg;


    if(argc != 2){
        std::cout << "No image data" << std::endl;
        return -1;
    }

    cv::imshow("Diplay Image", image);

    uint8_t *pxl = (uint8_t*)image.data;
    img = dmtxImageCreate(pxl, width, height, DmtxPack8bppK);

    dec = dmtxDecodeCreate(img, 1);

    std::cout << "Treshold: " << 
        dmtxDecodeGetProp(dec, DmtxPropEdgeThresh)
        << std::endl;

    int i; // Number of robots to search
    
    for(i = 0; i < 3; i++){
        reg = dmtxRegionFindNext(dec, NULL);
        if(reg != NULL) {
            msg = dmtxDecodeMatrixRegion(dec, reg, DmtxUndefined);
            if(msg != NULL) {
                // ID of the robot
                std::cout << "ID: " << msg->output << std::endl;
                
                // Intersection of the two solid borders, in pixels
                std::cout << "Position: " << reg->bottomLine.locPos.X 
                    << ", " << reg->bottomLine.locPos.Y << std::endl;
                
                // Angle in degress, w.r.t. the horizontal,
                // of the bottom solid border, counter-clockwise
                std::cout << "Bottom Line angle atan2: " <<
                    atan2(reg->bottomLine.locNeg.Y - reg->bottomLine.locPos.Y,
                            reg->bottomLine.locNeg.X - reg->bottomLine.locPos.X)
                    *180/M_PI << std::endl;
                std::cout << std::endl;
            }
            dmtxRegionDestroy(&reg);
        }
    }

    return 0;
}

