function startLocalizingRobots(n, h, w)

% n: number of expected robots
% h & w: webcam resolution

if libisloaded('libvisionloc') == 0
    loadlibrary('libvisionloc.so','visionloc.h')
end

calllib('libvisionloc','start_visionloc', n, 1600, 1200);

pause(2);

end

