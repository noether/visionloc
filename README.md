# LIGHT VISION LOCALIZATION ALGORITHM

Authors: H. Garcia de Marina and Miguel Martinez
Contact mail: noeth3r at gmail dot com  
Webpage/Forum: http://dobratech.com/tools/visionloc  
Licence: GNU GPL V2, http://www.gnu.org/licenses/gpl-2.0.en.html

Light C++ library for the localization of markers on a 2D surface. The functions from this library can be called from C, C++ or Matlab. 

It is possible to have more than one camera connected at the same time. Each local frame of coordinates (offset of the lower left corner and rotation w.r.t. World coordinates) are set in the file camerasInfo.xml. The library interfaces the connected camera(s) to your PC (employing openCV) and as a result it gives you the position and orientation of the markers in XY World coordinates. The algorithm runs in a parallel thread to your main application, so it will not block your main program. That makes attractive the usage of the library in real-time scenarios. Check the example_libvisionloc.cc as an example of usage.

## 0. COMPILING REQUIREMENTS
It is necessary to have installed three libraries:
1. libdmtx 0.7.4 http://libdmtx.sourceforge.net/
2. libopencv 3.1.0 http://opencv.org/
3. libtinyxml2 5.0.1 http://www.grinninglizard.com/tinyxml2/


## 1. IMAGES
There are some test images in the images folder and some markers 
in the markers folder. The markers are 2D data matrices generated
at http://datamatrix.kaywa.com/ .

## 2. EXAMPLES
1. imagedmtx: Loads a static image and localizes N markers. You do not need the library libvisionloc for this example. In fact, you do not need of OpenCV for
this example.

usage: imagedmtx \<path to your image\> \<number of robots\> (you can use the images in the test folder)

2. example_libvisionloc: Reads camerasInfo.xml and extracts the makers from the
camera 0 (possibily multi-camera). You need to compile the library libvisionloc first.
usage: example_libvisionloc

### 2.5. USAGE camerasInfo.xml
ID: identification number of the camera.  
Width, Height: Resolution of the camera.  
WcHeight: It sets the ratio between camera's pixel and the World coordinates. For a direct correspondence, set it to the same number as Height.  
WCOffX, WCOffY: Offset of the origin of the camera w.r.t. World Coordinates.  
WCOffAngle: Rotation of the camera w.r.t. World Coordinates.

## 3. LIBRARY FUNCTIONS
start_visionloc: starts a thread which takes the default camera and 
updates the status of the robots as soon as it processes one frame.

stop_visionloc: kills the thread, do not forget to call it when you
do not need to localize the robots anymore.

set_expected_num_of_markers(camera, num): set the maximum n number of expected markers for the chosen camera.

read_camera(camera): reads the last update about the status of the robots from 
the chosen camera. It is an array of 256 doubles. The first value is the number of localized robots (n) in the frame (always less or equal than the expected number of robots). The rest of the array corresponds to 6*n doubles with the information of the localized robots. The packet of 6 doubles is always as follows

1. ID (ASCII CODE of the Marker)
2. PosX of the Corner [pixels]
3. PosY of the Corner [pixels]
4. PosX of the Center [pixels]
5. PosY of the Center [pixels]
6. Heading [degrees], w.r.t the X axis counter clockwise [-pi, pi)

read_all_cameras: it reads the last updated about the status of the robots at the camera "number". It is an array of 256 doubles. The first value is the id of the camera. The second number is the number of localized robots (n) by the camera (always less or equal than the expected number of robots). The rest of the array corresponds to 6*n doubles with the information of the localized robots as in read_camera.

get_tag(camera): tag of the latest time update from the chosen camera. Useful to know if the chosen camera has new information.

get_cam_width(camera): gets X resolution from the chosen camera
get_cam_height(camera): gets Y resolution from the chosen camera

get_markers_from_camera(camera): gets the struct markers (check visionloc.hh) from the chosen camera.

get_frame_from_camera(camera): gets the latest processed frame (opencv Mat) of the chosen camera. Usefull for paiting over it with the marker's positions.

Do not forget to copy the library to your /usr/lib or wherever your
system can find it.


## 4. USAGE OF THE LIBRARY WITH MATLAB
In the ./matlab directory you can find a practical example.

## 5. Calibration for two overlapped cameras
Place a marker(s) in the overlapped area and run calibration_2_cameras . The output will suggest the parameters for the World Coordinates in camerasInfo.xml

## 6. TO DO
--------
Python support

Support of more markers such as Aruco or Apriltags 

Calibration of cameras 
