function [interval, newval] = sos(coef, in, w) 
    interval = in - (coef(1,5)*w(1,2)) - (coef(1,6)*w(1,1));
    newval = (coef(1,1)*interval) + (coef(1,2)*w(1,2)) + (coef(1,4)*w(1,1));
