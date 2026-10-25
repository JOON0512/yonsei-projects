# Signals and systems

신호 및 시스템 수업의 Fourier·샘플링 실험.

| 스크립트 | 내용 | 입력 |
| --- | --- | --- |
| `impulse_sum.py` | cosine 합의 항 수에 따른 주기적 peak | 없음 |
| `magnitude_phase.py` | 영상 FFT의 크기·위상 분리, 선형·비선형 위상 | `uploaded_classroom_screenshot.png` |
| `undersampling.py` | k-space undersampling과 zero fill | `testMRimage.mat`의 `img1` |

```sh
pip install -r requirements.txt
python3 impulse_sum.py
```

이미지와 `.mat` 입력은 별도로 준비해야 합니다. `undersampling.py`는 256×256 실수·비음수 영상을 전제로 하며, 재구성에서 절댓값을 취합니다.

합성 데이터로 세 스크립트를 확인하려면:

```sh
MPLBACKEND=Agg python3 verify.py
```
