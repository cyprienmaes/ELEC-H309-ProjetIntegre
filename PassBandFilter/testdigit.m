clear all; close all; clc;
te = 1/15938;
t = [0 : te : 2000*te];
coef1 = [1 1.99999999 1 1 -1.79934545 0.98560384];
coef2 = [1 2.00000001 1 1 -1.80449128 0.98578688];
coef3 = [1 -2.00000002 1.00000002 1 -1.8033025 0.99395788];
coef4 = [1 -1.99999998 0.99999998 1 -1.81554213 0.99414247];

error = 1:length(t);
w1 = [0 0];
w2 = [0 0];
w3 = [0 0];
w4 = [0 0];

m = 1:length(t);
sortie = 1:length(t);


for i = 1:1:length(m)
  m(1,i) = round(512 + 512*sin(2*pi*1100*i*te));
  [interval1 newval1] = sos(coef1, m(1,i), w1);
  w1(1,1) = w1(1,2);
  w1(1,2) = interval1;
  [interval2 newval2] = sos(coef2, newval1*0.007757830666124229, w2);
  w2(1,1) = w2(1,2);
  w2(1,2) = interval2;
  [interval3 newval3] = sos(coef3, newval2*0.007757830666124229 , w3);
  w3(1,1) = w3(1,2);
  w3(1,2) = interval3;
  [interval4 newval4] = sos(coef4, newval3*0.007757830666124229 , w4);
  w4(1,1) = w4(1,2);
  w4(1,2) = interval4;
  sortie(1,i) = round(512 + newval4*0.007757830666124229);
  error(1,i) = (sortie(1,i)-m(1,i))/512;
end

subplot(2,1,1);
plot(t, m, 'r-');
hold on;
plot(t, sortie, 'g-');
title("digital band-pass filter for 1100 Hz")
xlabel("time [s]");
ylabel("Amplitude");
subplot(2,1,2);
plot(t,error, 'r-');
xlabel("time [s]");
ylabel("normalized error");
0.5380554199218750*2^15