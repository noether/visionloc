function startLocalizingRobots(n, h, w)

% n: number of expected robots
% h & w: webcam resolution
% do not forget about camerasInfo.xml

if libisloaded('libvisionloc') == 0
    loadlibrary('libvisionloc.so','visionloc.h')
end

calllib('libvisionloc','start_visionloc'); 

pause(2);

end

