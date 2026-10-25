# 통신네트워크 실험 자료

이호준·전상욱 공동 제출물의 소스 보관본입니다. LabVIEW/NI 예제를 바탕으로 실험한 자료이며, 단독 개발 프로젝트로 표시하지 않습니다.

- `spectrogram/Conv2Img.py`: 측정 CSV에서 스펙트로그램 이미지를 만드는 Python 코드. 필요한 패키지는 같은 폴더의 `requirements.txt`에 있습니다.
- `radar/Tx Continuous Async (CDB Cluster).gvi`: LabVIEW NXG의 송신 실험 VI. 해당 NI 도구와 USRP 지원 패키지·장비가 필요합니다.

3·4주차 소스가 같아 한 벌만 보존했습니다. 5주차 압축파일에는 측정 CSV만 있어 별도 구현으로 추가하지 않았습니다. 측정 원본, 생성 이미지, 장비 IP가 담긴 로컬 프로젝트 설정은 포함하지 않았습니다.

복원 시 Python 구문과 VI XML 구조를 검사했습니다. 실제 측정 파일을 사용한 스펙트로그램 생성이나 LabVIEW 컴파일·USRP 송수신은 재검증하지 않았습니다. Python 파일의 입력·출력 폴더는 실행 환경에 맞게 확인하세요.
