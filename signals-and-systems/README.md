# 신호 및 시스템 코딩 실험

본인의 HW3-1·HW5·HW6 보고서 코드 부분을 PDF와 로컬 문서에서 대조하여 Python 파일로 복원했습니다. 수업 실험을 구현한 학습 기록이며, 보고서 본문·교재 요약·강의 화면·MR 원본 데이터는 포함하지 않았습니다.

- `impulse_sum.py`: cosine 합의 항 수에 따른 주기적 peak 비교. 실행하면 그림 2개를 저장합니다.
- `magnitude_phase.py`: 영상 FFT의 크기·위상 분리 및 1D 선형/비선형 위상 실험. 입력 파일명은 `uploaded_classroom_screenshot.png`이며 직접 준비한 이미지로 대체할 수 있습니다. 원래 코드처럼 계산 변수와 figure를 생성합니다.
- `undersampling.py`: `testMRimage.mat`의 2D `img1`에서 k-space 행·열 제거와 zero fill을 비교하고 그림을 저장합니다. 원 실험은 256×256의 실수·비음수 영상이며 plot 제목도 이 크기를 전제로 합니다. 재구성에서 절댓값을 취하므로 일반 복소 영상의 위상 보존 실험으로 해석하면 안 됩니다.

`pip install -r requirements.txt` 후 각 파일을 실행하세요. 입력 데이터 없이 확인하려면 `MPLBACKEND=Agg python3 verify.py`를 실행할 수 있습니다. 이 검사는 임시 폴더에 합성 이미지와 .mat를 만들어 세 스크립트를 실행하고, FFT/IFFT 일치, 정수 지연, alias 합과 출력 크기를 확인합니다. 실제 강의 이미지·MRI 데이터에서 과거 결과를 재현했다는 의미는 아닙니다.
