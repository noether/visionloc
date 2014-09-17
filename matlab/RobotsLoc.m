function [allrobots, Robots] = RobotsLoc(n)

%n: number of expected robots
% allrobots: 1 if all the expected robots were localized, 0 otherwise.
% Robots: struct with the information about the robots

allrobots = 0;

inPtr = calllib('libvisionloc','read_visionloc');
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
        Robots(i).PosX = inPtr.value(j+2);
        Robots(i).PosY = inPtr.value(j+3);
        Robots(i).Heading = inPtr.value(j+4);
        
        j = j + 4;
    end
    
end