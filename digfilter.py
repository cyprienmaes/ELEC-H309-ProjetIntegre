from scipy import signal
import math
from numpy import *
import matplotlib.pyplot as plt
gain900 = 1.60414857371e-09
gain1100 = 3.566569109e-09
# important datas for the digital filter
fsample = 16000/2 # Hz
fcenter = 1100 # Hz
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
sos[0,1] = (sos[0,1]/sos[0,0])*2**10
sos[0,2] = (sos[0,2]/sos[0,0])*2**10
print(sos[0,0])
sos[0,0] = 1*2**10
sos[0,4] = sos[0,4]*2**10
sos[0,5] = sos[0,5]*2**10
sos[1] = sos[1]*2**10
sos[2] = sos[2]*2**10
sos[3] = sos[3]*2**10
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
print((2**10)**2)
print((gain1100**(1/4))*(2**10))
