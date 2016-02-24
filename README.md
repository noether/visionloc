LIGHT VISION LOCALIZATION ALGORITHM
--------------------------------------------------------------

Author: H. Garcia de Marina  
Contact mail: noeth3r@gmail.com  
Webpage: http://masteringrobotics.com/  
Licence: GNU GPL V2, http://www.gnu.org/licenses/gpl-2.0.en.html

0. COMPILING REQUIREMENTS
-------------------------
It is necessary to have installed two libraries:  
1. libdmtx http://www.libdmtx.org/  
2. opencv  http://opencv.org/


1. IMAGES
---------

There are some test images in the images folder and some markers 
in the markers folder. The markers are 2D data matrices generated
at http://datamatrix.kaywa.com/ .

2. DIRECT EXAMPLES
------------------

1. imagedmtx: Loads a static image and localizes N markers  
usage: imagedmtx \<path to your image\> \<number of robots\> (you can use the images in the test folder)

2. example_libvisionloc: Reads camerasInfo.xml and extracts the makers from the
camera 0 (possibily multi-camera). Before using this example you need to compile  library (following section)  
usage: example_libvisionloc

3. USAGE camerasInfo.xml
------------------------

ID: identification number of the camera.
Width, Height: Resolution of the camera.
WcHeight: It sets the ratio between camera's pixel and the World coordinates. For a direct correspondence, set it to the same number as Height.
WCOffX, WCOffY: Offset of the origin of the camera w.r.t. World Coordinates.
WCOffAngle: Rotation of the camera w.r.t. World Coordinates.

4. LIBRARY FUNCTIONS
-------------------------

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


5. USAGE OF THE LIBRARY WITH MATLAB
------------------------------
In the ./matlab directory you can find a practical example.


6. USAGE OF THE LIBRARY WITH PYTHON
-----------------------------------
TBA
