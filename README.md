ALGORITHM FOR THE LOCALIZATION OF ROBOTS EMPLOYING CAMERAS
--------------------------------------------------------------

Developed by H. Garcia de Marina
Contact mail: noeth3r@gmail.com
Webpage: http://mathtronics.wordpress.com/

0. IMAGES
---------

There are some test images in the images folder and some markers 
in the markers folder. The markers are 2D data matrices generated
at http://datamatrix.kaywa.com/ .

1. EXAMPLES
-----------

Here there are three examples/tests. First example searches for three robots in a
single image (ideal for prototyping your setup). The second example involves a
camera using OpenCV, searches for one robot in every frame, a timeout watches whether a robot is not found. The third one shows how to use the library for
multicamera.

The code is quite simple and readable, modify it in order to fit it with
your requirements such as number of robots, image size, etc.

It is necessary to have installed two libraries:
1. libdmtx http://www.libdmtx.org/
2. opencv  http://opencv.org/

Compile it!

1. imagedmtx: Loads a static image and localizes N markers
usage: imagedmtx <path to your image> <number of robots> (you can use the images in the test folder)

2. camdmtx: Streams from the default camera or video file (there is one demo vido at ./videos), localizes one maker:
usage for video file: camdmtx <path to your video file>  
usage for webcam: camdmtx <width> <height>

3. example_libvisionloc: Reads camerasInfo.xml and extracts the makers from the
camera 0. Do not forget to compile before the library.
usage: example_libvisionloc


2. LIBRARY for one camera
-------------------------

The library has three functions.

start_visionloc: starts a thread which takes the default camera and 
updates the status of the robots as soon as it processes one frame.

stop_visionloc: kills the thread, do not forget to call it when you
do not need to localize the robots anymore.

read_visionloc: reads the last update about the status of the robots. It
is an array of 256 doubles. The first value is the number of localized
robots (n) in the frame (always less or equal than the expected number of
robots). The rest of the array corresponds to 6*n doubles with the information
of the localized robots. The packet of 6 doubles is always as follows

1. ID (the message is a char, so the vale here is the ASCII code)
2. PosX of the Corner [pixels]
3. PosY of the Corner [pixels]
4. PosX of the Center [pixels]
5. PosY of the Center [pixels]
6. Heading [degrees], w.r.t the X axis counter clockwise [-pi, pi)

Do not forget to copy the library to your /usr/lib or wherever your
system can find it.


3. LIBRARY for multi camera
---------------------------

First you need to define your cameras in the file camerasInfo.xml.

The library has four functions:

start_visionloc: starts a thread which takes the default camera and 
updates the status of the robots as soon as it processes one frame.

stop_visionloc: kills the thread, do not forget to call it when you
do not need to localize the robots anymore.

read_camera(number): reads the last updated about the status of the robots at
the camera "number". It is an array of 256 doubles. The first value is the id of the camera. The second number is the number of localized robots (n) by the camera (always less or equal than the expected number of robots). The rest of the array corresponds to 6*n doubles with the information of the localized robots (look at read_visionloc)

read_all_cameras: reads the last updated about the status of the robots from
all the cameras. It is an array of 256 doubles. The array is populated as in read_camera.

Do not forget to copy the library to your /usr/lib or wherever your
system can find it.


4. USE THE LIBRARY WITH MATLAB
------------------------------

In the Matlab folder, there is an example and some wrappers. In order to make
it work, you have to put libvisionloc.so abd visionloc.h in your work folder.

As before, start with startLocalizingRobots, call to the RobotsLoc
(which returns a struct), and finally stopLocalizingRobots
