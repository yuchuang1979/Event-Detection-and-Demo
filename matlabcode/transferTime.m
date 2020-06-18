function [hours, minutes, seconds] = transferTime(total_seconds)

hours = floor(total_seconds/3600);
minutes = floor(mod(total_seconds,3600)/60);
seconds = total_seconds - hours*3600 - minutes*60;