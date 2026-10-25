# Network labs

이호준·전상욱이 함께 진행한 통신네트워크 실험. LabVIEW/NI 예제를 기반으로 합니다.

- `spectrogram/Conv2Img.py` — 측정 CSV에서 스펙트로그램 이미지 생성
- `radar/Tx Continuous Async (CDB Cluster).gvi` — LabVIEW NXG 송신 실험

Python 의존성은 `spectrogram/requirements.txt`에 있습니다. 측정 CSV를 준비하고 스크립트의 입력·출력 경로를 맞춘 뒤 실행합니다.

송신 VI에는 LabVIEW NXG, USRP 지원 패키지와 장비가 필요합니다.
