function [allrobots, Robots] = RobotsLoc(camera, n)

%camera: camera ID
%n: number of expected robots
% allrobots: 1 if all the expected robots were localized, 0 otherwise.
% Robots: struct with the information about the robots

allrobots = 0;

inPtr = calllib('libvisionloc','read_camera', camera);
setdatatype(inPtr, 'doublePtr', 1, 256);
numrobots = int8(inPtr.value(1));

if numrobots == n
    allrobots = 1;
end

j = 1;

if numrobots == 0
    Robots = 0;
else
    
    for i = 1:numrobots
        
        Robots(i).Id = char(inPtr.value(j+1));
        Robots(i).CorX = inPtr.value(j+2);
        Robots(i).CorY = inPtr.value(j+3);
		Robots(i).MidX = inPtr.value(j+4);
		Robots(i).MidY = inPtr.value(j+5);
        Robots(i).Heading = inPtr.value(j+6);
        
        j = j + 6;
    end
    
end
