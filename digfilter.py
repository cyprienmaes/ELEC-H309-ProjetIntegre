from scipy import signal
import math
from numpy import *
import matplotlib.pyplot as plt
#gain900 = 1.60414857371e-09 pour 16000
#gain1100 = 3.566569109e-09
#gain900 = 1.82039715031e-09  pour 15500
#gain1100 = 4.04688652613e-09
#gain900 = 2.07434467798e-09 pour 15000
#gain1100 = 4.61085254787e-09
#gain900 = 2.37415995725e-09
#gain1100 = 5.27657333547e-09 pour 14500
gain900 = 2.58024724617e-09
gain1100 = 5.73411475348e-09
n = 2**9
# important datas for the digital filter
fsample = 14200/2 # Hz
fcenter = 900 # Hz
f_cut_min = fcenter - (1.5/100)*fcenter # Hz
f_cut_max = fcenter + (1.5/100)*fcenter # Hz
f_stop_min = fcenter - (3.5/100)*fcenter # Hz
f_stop_max = fcenter + (3.5/100)*fcenter # Hz

# maximum gain loss in the passband
gpass = 20*log10(0.9)
# minimum attenuation in the stopband
gstop = 20*log10(0.1)

# We need de f_cut and f_stop in half-cycles/sample
w_cut_min = f_cut_min/fsample   # half-cycles/sample
w_cut_max = f_cut_max/fsample   # half-cycles/sample
w_stop_min = f_stop_min/fsample # half-cycles/sample
w_stop_max = f_stop_max/fsample # half-cycles/sample

# filter's passband
wp = [w_cut_min, w_cut_max]
# filter's stopband
ws = [w_stop_min, w_stop_max]

N, Wn = signal.buttord(wp, ws, gpass, gstop)
b, a = signal.butter(N, Wn, 'band', analog=False, output='ba')
w, h = signal.freqz(b, a, worN=10000)
plt.plot((fsample / pi) * w, abs(h))
sos = signal.tf2sos(b, a, pairing='nearest')
sos[0,1] = (sos[0,1]/sos[0,0])*n
sos[0,2] = (sos[0,2]/sos[0,0])*n
print(sos[0,0])
sos[0,0] = 1*n
sos[0,4] = sos[0,4]*n
sos[0,5] = sos[0,5]*n
sos[1] = sos[1]*n
sos[2] = sos[2]*n
sos[3] = sos[3]*n
plt.plot([800, 1000], [0.9, 0.9], "r--")
plt.plot([800, 1000], [0.1, 0.1], "r--")
plt.plot([f_cut_min, f_cut_min], [0.01, 1.2], "g--")
plt.plot([f_cut_max, f_cut_max], [0.01, 1.2], "g--")
plt.plot([f_stop_min, f_stop_min], [0.01, 1.2], "b--")
plt.plot([f_stop_max, f_stop_max], [0.01, 1.2], "b--")
plt.xscale('log')
plt.title('Butterworth bandpass filter fit to constraints')
plt.xlabel('Frequency [Hz]')
plt.ylabel('Amplitude [dB]')
plt.grid(which='both', axis='both')
plt.axis([800, 1000, 0.01, 1.2])
plt.show()
print(a)
print(b)
print(sos)
print((n)**2)
print((gain900**(1/4))*(n))
