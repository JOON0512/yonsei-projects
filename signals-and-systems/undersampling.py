# Restored from the author's HW6 report, Source Code section.
from pathlib import Path
import numpy as np
import matplotlib.pyplot as plt
from scipy.io import loadmat

# testMRimage.mat가 같은 폴더에 있으면 그대로 사용하고,
# ChatGPT 실행 환경처럼 /mnt/data에 있으면 그 경로를 사용한다.
LOCAL_FILE = Path("testMRimage.mat")
DATA_FILE = LOCAL_FILE if LOCAL_FILE.exists() else Path("/mnt/data/testMRimage.mat")
OUT_DIR = Path("hw6_sampling_outputs")
OUT_DIR.mkdir(parents=True, exist_ok=True)


def imshow_clean(ax, arr, title, aspect="equal"):
    ax.imshow(np.abs(arr), cmap="gray", aspect=aspect)
    ax.set_title(title, fontsize=10)
    ax.set_xticks([])
    ax.set_yticks([])


def log_kspace(Fs):
    # Fourier magnitude는 dynamic range가 크므로 log scale로 표시한다.
    return np.log1p(np.abs(Fs))


def reconstruct_from_shifted(Fs_sub):
    # centered k-space를 원래 FFT 배열 순서로 되돌린 뒤 inverse FFT를 수행한다.
    return np.abs(np.fft.ifft2(np.fft.ifftshift(Fs_sub)))


def zero_fill_mask(Fs, row_step=1, col_step=1):
    # full k-space grid를 유지하되, 획득하지 않은 line은 0으로 둔다.
    mask = np.zeros_like(Fs, dtype=float)
    mask[::row_step, ::col_step] = 1.0
    return Fs * mask, mask


mat = loadmat(DATA_FILE)
img = mat["img1"].astype(float)
Ny, Nx = img.shape

# 1) full k-space와 full reconstruction
F = np.fft.fft2(img)
Fs = np.fft.fftshift(F)
img_full = np.abs(np.fft.ifft2(F))
max_recon_error = np.max(np.abs(img - img_full))

# 2) Lecture-style deleted-line undersampling
Fs_ky2 = Fs[::2, :]       # keep every 2nd k_y line
Fs_kx2 = Fs[:, ::2]       # keep every 2nd k_x line
Fs_both2 = Fs[::2, ::2]   # keep every 2nd line in both axes
Fs_ky4 = Fs[::4, :]       # keep every 4th k_y line

img_ky2 = reconstruct_from_shifted(Fs_ky2)
img_kx2 = reconstruct_from_shifted(Fs_kx2)
img_both2 = reconstruct_from_shifted(Fs_both2)
img_ky4 = reconstruct_from_shifted(Fs_ky4)

# 3) zero-filled full-grid reconstruction
Fs_zf_ky2, mask_ky2 = zero_fill_mask(Fs, row_step=2, col_step=1)
Fs_zf_kx2, mask_kx2 = zero_fill_mask(Fs, row_step=1, col_step=2)
Fs_zf_both2, mask_both2 = zero_fill_mask(Fs, row_step=2, col_step=2)

img_zf_ky2 = reconstruct_from_shifted(Fs_zf_ky2)
img_zf_kx2 = reconstruct_from_shifted(Fs_zf_kx2)
img_zf_both2 = reconstruct_from_shifted(Fs_zf_both2)

# Figure 1: original image and centered k-space
fig, axes = plt.subplots(1, 2, figsize=(8.5, 4.0), constrained_layout=True)
imshow_clean(axes[0], img, f"Original MR image\nshape={img.shape}")
axes[1].imshow(log_kspace(Fs), cmap="gray")
axes[1].set_title("Centered Fourier space |FFT2| (log scale)", fontsize=10)
axes[1].set_xticks([])
axes[1].set_yticks([])
fig.savefig(OUT_DIR / "fig1_original_and_kspace.png", dpi=200)
plt.close(fig)

# Figure 2: k-space sampling patterns
fig, axes = plt.subplots(2, 2, figsize=(8.5, 7.2), constrained_layout=True)
axes = axes.ravel()
axes[0].imshow(log_kspace(Fs), cmap="gray")
axes[0].set_title("Full k-space: 256 x 256", fontsize=10)
axes[1].imshow(log_kspace(Fs_ky2), cmap="gray", aspect="auto")
axes[1].set_title("Keep every 2nd k_y line: 128 x 256", fontsize=10)
axes[2].imshow(log_kspace(Fs_kx2), cmap="gray", aspect="auto")
axes[2].set_title("Keep every 2nd k_x line: 256 x 128", fontsize=10)
axes[3].imshow(log_kspace(Fs_both2), cmap="gray", aspect="auto")
axes[3].set_title("Keep every 2nd line in both axes: 128 x 128", fontsize=10)
for ax in axes:
    ax.set_xticks([])
    ax.set_yticks([])
fig.savefig(OUT_DIR / "fig2_kspace_undersampling_patterns.png", dpi=200)
plt.close(fig)

# Figure 3: image-space results using deleted-line undersampling
fig, axes = plt.subplots(2, 3, figsize=(10, 6.8), constrained_layout=True)
axes = axes.ravel()
imshow_clean(axes[0], img, f"Original\n{img.shape}")
imshow_clean(axes[1], img_full, f"Full FFT -> IFFT\nmax error={max_recon_error:.2e}")
imshow_clean(axes[2], img_ky2, f"k_y 2x undersampling\n{img_ky2.shape}", aspect="auto")
imshow_clean(axes[3], img_kx2, f"k_x 2x undersampling\n{img_kx2.shape}", aspect="auto")
imshow_clean(axes[4], img_both2, f"k_x,k_y 2x undersampling\n{img_both2.shape}", aspect="auto")
imshow_clean(axes[5], img_ky4, f"k_y 4x undersampling\n{img_ky4.shape}", aspect="auto")
fig.savefig(OUT_DIR / "fig3_image_space_results_deleted_lines.png", dpi=200)
plt.close(fig)

# Figure 4: zero-filled full-grid reconstructions
fig, axes = plt.subplots(1, 4, figsize=(11, 3.2), constrained_layout=True)
imshow_clean(axes[0], img, "Original")
imshow_clean(axes[1], img_zf_ky2, "Zero-filled: keep every 2nd k_y")
imshow_clean(axes[2], img_zf_kx2, "Zero-filled: keep every 2nd k_x")
imshow_clean(axes[3], img_zf_both2, "Zero-filled: both axes")
fig.savefig(OUT_DIR / "fig4_zero_filled_full_grid_results.png", dpi=200)
plt.close(fig)

print("original_shape:", img.shape)
print("full_reconstruction_shape:", img_full.shape)
print("max_full_reconstruction_error:", max_recon_error)
print("ky2_shape:", img_ky2.shape)
print("kx2_shape:", img_kx2.shape)
print("both2_shape:", img_both2.shape)
print("ky4_shape:", img_ky4.shape)
print("Figures saved to", OUT_DIR)
