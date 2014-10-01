ALGORITHM FOR THE LOCALIZATION OF ROBOTS EMPLOYING ONE CAMERA 
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

Here there are two examples/tests. First example searches for three robots in a
single image (ideal for prototyping your setup). The second example involves a
camera using OpenCV, searches for one robot in every frame, a timeout watches whether a robot is not found.

The code is quite simple and readable, modify it in order to fit it with
your requirements such as number of robots, image size, etc.

It is necessary to have installed two libraries:
1. libdmtx http://www.libdmtx.org/
2. opencv  http://opencv.org/

Compile it!

1. imagedmtx: Loads a static image (1600x1200) and localizes three robots
usage: imagedmtx <path to your image> (you can use the images in the test folder)

2. camdmtx: Streams from the default camera, localizes one robot
usage: camdmtx


2. LIBRARY
----------

The library has three functions.

start_visionloc: starts a thread which takes the default camera and 
updates the status of the robots as soon as it processes one frame.

stop_visionloc: kills the thread, do not forget to call it when you
do not need to localize the robots anymore.

read_visionloc: reads the last update about the status of the robots. It
is an array of 256 doubles. The first value is the number of localized
robots (n) in the frame (always less or equal than the expected number of
robots). The rest of the array corresponds to 4*n doubles with the information
of the localized robots. The packet of 4 doubles is always as follows

1. ID (the message is a char, so the vale here is the ASCII code)
2. PosX [pixels]
3. PosY [pixels]
4. Heading [degrees], w.r.t the X axis counter clockwise [-pi, pi)



3. USE THE LIBRARY WITH MATLAB
------------------------------

In the Matlab folder, there is an example and some wrappers. In order to make
it work, you have to put libvisionloc.so abd visionloc.h in your work folder.

As before, start with startLocalizingRobots, call to the RobotsLoc
(which returns a struct), and finally stopLocalizingRobots
