DRM RECEIVER DESIGN
===================

Robustness mode and bandwidth detection, synchronization
===========================================================
Single block utilizing kernels to avoid state handling across blocks and maintain readability.
a) Mode detection, symbol timing recovery, and fractional frequency offset correction based on cyclic prefix correlation.
    - FFT length @ 48 kHz for RM A/B/C/D: 1152/1024/704/448
    - CP length @ 48 kHz for RM A/B/C/D: 128/256/256/448/352
b) Bandwidth detection and coarse frequency offset correction based on subcarrier energy in frequency domain.
    - Number of occupied subcarriers (Kmax - Kmin) for...
        - RM A and SO 0/1/2/3/4/5: 101/113/205/229/413/451
        - RM B and SO 0/1/2/3/4/5: 91/103/183/207/367/411
        - RM C and SO 3/5: 139/281
        - RM D and SO 3/5: 89/189
        
SO detection and coarse frequency correction
============================================
Based on a known RM, possible SOs can be evaluated based on the estimated bandwidth of the signal.
The determined number of occupied subcarriers can then be used to determine the coarse frequency offset.
This could also be done by using the time pilots.

Frame start detection
=====================
Known time pilots can be used to determine the frame start. Also tracks symbol timing in order to avoid clock drift problems.
Consideration: Frame duration is 400 ms, clock drift is assumed to be < 100 ppm. This results in 0.1 * 400 ms = 0.4 ms offset per frame. The minimum cyclic prefix length is 26.66/9 ms >> 0.4 ms.
Therefore, this kind of symbol timing tracking should be enough. Problems with frequency dependent phase offsets due to slowly accumulating time offset will be compensated by the channel equalization.

Channel estimation and equalization
===================================
Channel estimation and equalization based on known gain references. Possibly use gr-digitals equalizer.
