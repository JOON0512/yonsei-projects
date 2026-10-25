# Restored from the author's HW5 report, section 5.
# Python version of the MATLAB coding experiment
from PIL import Image, ImageOps
import numpy as np
import matplotlib.pyplot as plt

img = Image.open('uploaded_classroom_screenshot.png').convert('RGB')
gray = ImageOps.grayscale(img).resize((512,384))
m2 = np.array(gray, dtype=float)

# 1-D signal from one image row
row = int(0.63*m2.shape[0])
sig1 = m2[row,:]
sig1fft = np.fft.fft(sig1)
plt.plot(sig1)
plt.figure(); plt.plot(np.fft.fftshift(np.abs(sig1fft)))

# 2-D Fourier transform
m2fft = np.fft.fft2(m2)
plt.figure(); plt.imshow(np.fft.fftshift(np.log1p(np.abs(m2fft))), cmap='gray')
plt.figure(); plt.imshow(np.fft.fftshift(np.angle(m2fft)), cmap='twilight')

# Reconstruction experiments
mag = np.abs(m2fft)
ph = np.angle(m2fft)
full = np.real(np.fft.ifft2(m2fft))
mag_only = np.real(np.fft.ifft2(mag))
phase_only = np.real(np.fft.ifft2(np.exp(1j*ph)))
random_phase = np.random.default_rng(0).uniform(-np.pi, np.pi, m2fft.shape)
random_phase_recon = np.real(np.fft.ifft2(mag*np.exp(1j*random_phase)))

# Linear and nonlinear phase on 1-D row signal
N = len(sig1)
omega = 2*np.pi*np.fft.fftfreq(N)
delay = 40
Hlin = np.exp(-1j*omega*delay)
y_shift = np.real(np.fft.ifft(sig1fft*Hlin))
phase_nl = -omega*delay - 8*np.sign(omega)*(omega**2)
Hnl = np.exp(1j*phase_nl)
y_nl = np.real(np.fft.ifft(sig1fft*Hnl))
