"""Run the recovered experiments with generated, non-personal input data."""
import os
from pathlib import Path
import runpy
import tempfile
import numpy as np
from PIL import Image
from scipy.io import savemat
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

root = Path(__file__).resolve().parent
previous = Path.cwd()
with tempfile.TemporaryDirectory() as temp:
    try:
        os.chdir(temp)
        y, x = np.mgrid[:256, :256]
        img = np.exp(-((x-81)**2 + (y-109)**2)/1800) + .4*((x>130)&(x<174)&(y>40)&(y<180))
        savemat('testMRimage.mat', {'img1': img})
        Image.fromarray(np.uint8(img / img.max() * 255)).save('uploaded_classroom_screenshot.png')
        impulse = runpy.run_path(str(root/'impulse_sum.py'))
        for n in [5, 20, 80]:
            np.testing.assert_allclose(impulse['xN'](np.array([0., 1.]), n), 2*n+1)
            np.testing.assert_allclose(impulse['xN'](np.array([1/(2*n+1)]), n), 0, atol=1e-10)
        phase = runpy.run_path(str(root/'magnitude_phase.py'))
        np.testing.assert_allclose(phase['full'], phase['m2'], atol=1e-10)
        np.testing.assert_allclose(phase['y_shift'], np.roll(phase['sig1'], 40), atol=1e-10)
        plt.close('all')
        sampling = runpy.run_path(str(root/'undersampling.py'))
        np.testing.assert_allclose(sampling['img_full'], img, atol=1e-12)
        np.testing.assert_allclose(sampling['img_zf_ky2'], (img+np.roll(img,128,axis=0))/2, atol=1e-12)
        np.testing.assert_allclose(sampling['img_zf_kx2'], (img+np.roll(img,128,axis=1))/2, atol=1e-12)
        for name, shape in [('img_ky2',(128,256)),('img_kx2',(256,128)),('img_both2',(128,128)),('img_ky4',(64,256))]:
            assert sampling[name].shape == shape, name
        assert len(list(Path('.').rglob('*.png'))) == 7
    finally:
        os.chdir(previous)
print('SIGNAL_EXPERIMENTS_TEST_SUCCESS')
