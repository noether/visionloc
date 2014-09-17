% test of Liblocation matlab
clc; clear all; close all;

% Start the parallel thread of localization
startLocalizingRobots(1, 1600, 1200);

% Do your stuff here

[allrobots, Robots] = RobotsLoc(1);

% Do your stuff here

% Do not forget to stop the parallel thread once you do not need
% the localization
stopLocalizingRobots();
