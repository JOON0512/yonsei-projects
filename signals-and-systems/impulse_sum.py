# Restored from the author's HW3-1 report appendix.
import numpy as np
import matplotlib.pyplot as plt
T = 1.0
w0 = 2 * np.pi / T
Ns = [5, 20, 80]
plt.rcParams["axes.unicode_minus"] = False
def xN(t, N):
    y = np.ones_like(t)
    for k in range(1, N + 1):
        y += 2 * np.cos(k * w0 * t)
    return y
# 1) periodic impulse-like signal
t1 = np.linspace(-1.5, 1.5, 8000)
plt.figure(figsize=(7, 3.6))
plt.plot(t1, xN(t1, 80))
plt.xlim(-1.5, 1.5)
plt.xlabel("t")
plt.ylabel("x_N(t)")
plt.title("N = 80")
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("student_impulse_periodic.png", dpi=220)
plt.close()
# 2) comparison near t = 0
t2 = np.linspace(-0.12, 0.12, 5000)
plt.figure(figsize=(7, 3.8))
for N in Ns:
    plt.plot(t2, xN(t2, N), label=f"N={N}")
plt.xlabel("t")
plt.ylabel("x_N(t)")
plt.title("Zoom near t = 0")
plt.grid(True, alpha=0.3)
plt.legend()
plt.tight_layout()
plt.savefig("student_impulse_compare.png", dpi=220)
plt.close()
# simple comparison values
for N in Ns:
    peak = 2 * N + 1
    first_zero = T / (2 * N + 1)
    print(f"N={N:>3d}, peak at t=0 = {peak:>3d}, first zero ~ {first_zero:.5f}")
