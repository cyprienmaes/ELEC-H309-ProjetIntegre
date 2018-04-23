clear all; close all; clc;

fe = 16000;
te = 1/fe;
fcentre = 1100;
echant = 20;
gain = 8;
change = 2**10;
n = 1;
j = 0;
sum = 0;

t = [0 : te : 2000*te];
coef1 = [1024, 2048, 1024, 1024, -1843, 1009];
coef2 = [1024, 2048, 1024, 1024, -1848, 1009];
coef3 = [1024, -2048, 1024, 1024, -1847, 1018];
coef4 = [1024, -2048, 1024, 1024, -1859, 1018];
%coef1 = [1024, 2048, 1024, 1024, -1907, 1012];
%coef2 = [1024, 2048, 1024, 1024, -1911, 1012];
%coef3 = [1024, -2048, 1024, 1024, -1911, 1019];
%coef4 = [1024, -2048, 1024, 1024, -1920, 1019];

compa = [];
tcomp = [];

w1 = [0 0];
w2 = [0 0];
w3 = [0 0];
w4 = [0 0];

m = 1:length(t);
sortie = 1:length(t);

for i = 1:1:length(m)
  m(1,i) = round(512 + 512*sin(2*pi*fcentre*i*te));
  [interval1 newval1] = sos2(coef1, m(1,i), w1);
  w1(1,1) = w1(1,2);
  w1(1,2) = interval1/change;
  [interval2 newval2] = sos2(coef2, newval1*gain/(change^2), w2);
  w2(1,1) = w2(1,2);
  w2(1,2) = interval2/change;
  [interval3 newval3] = sos2(coef3, newval2*gain/(change^2) , w3);
  w3(1,1) = w3(1,2);
  w3(1,2) = interval3/change;
  [interval4 newval4] = sos2(coef4, newval3*gain/(change^2) , w4);
  w4(1,1) = w4(1,2);
  w4(1,2) = interval4/change;
  sortie(1,i) = round(512 + (newval4*gain/(change^2)));
  if sortie(1,i) >= 1024 
    sum +=1;
  elseif sortie(1,i) <= 10
    sum +=1;
  else
    sum +=0;
  end
  j += 1;
  if j == n*echant
    if sum >= 2*n
      compa = [compa 1];
    else 
      compa = [compa 0];
    end
    tcomp = [tcomp i*te];
    sum = 0;
    j = 0;
  end
end

subplot(2,1,1);  
plot(t, m, 'r-');
hold on;
plot(t, sortie, 'g-');
title("digital band-pass filter for 900 Hz")
xlabel("time [s]");
ylabel("Amplitude");
grid on;
subplot(2,1,2);
plot(tcomp, compa, 'b-o')
title("Output correlation with input");
xlabel("time [s]");
ylabel("Correlation");
grid on;
ylim([-0.1 1.2]);
