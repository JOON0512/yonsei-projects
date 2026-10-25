# 학습과 프로젝트 모음

수업에서 작업한 소스와 개인 프로젝트를 정리한 인덱스입니다. 각 저장소의 README에 기반 코드, 실행 방법, 검증 범위를 기록했습니다.

| 저장소 | 내용 | 검증·복원 상태 |
|---|---|---|
| [LLVM101](https://github.com/JOON0512/LLVM101) | LLVM 패스, 함수·메모리 계측 | LLVM 14 통합 테스트 통과 |
| [yonsei-OS](https://github.com/JOON0512/yonsei-OS) | xv6 RISC-V 스케줄러 실습 | 로컬 stride와 제출본 6개 복원, QEMU 테스트 |
| [yonsei-ai](https://github.com/JOON0512/yonsei-ai) | 회귀·앙상블·신경망 실습 | 소스와 노트북 복원, 학습 전체 재현 전 |
| [yonsei-embedded-systems](https://github.com/JOON0512/yonsei-embedded-systems) | Zynq, Verilog, 주변장치·인터럽트 | 4개 주차 소스, 보드 재검증 전 |
| [yonsei-Digital-Labs](https://github.com/JOON0512/yonsei-Digital-Labs) | FPGA Julia set 시각화 | Verilog 12개·BRAM 설정 복원, 산술 시뮬레이션 통과 |
| [yonsei-sp](https://github.com/JOON0512/yonsei-sp) | 시스템프로그래밍 학습 기록 | 원본 복원·오류 재현·로컬 수정 검증, 공개 분석 기록 |
| [proc-lens](https://github.com/JOON0512/proc-lens) | 새로 구현한 Linux 프로세스 스냅샷 모니터 | Linux 6.8 VM 반복 조회·제거 검증 |
| [branch-prediction-lab](https://github.com/JOON0512/branch-prediction-lab) | gem5 분기 예측기 구현 초안 | API 호환성 문제를 명시한 실험 보관본 |
| [History_At_Site](https://github.com/JOON0512/History_At_Site) | 영화·역사·장소 탐색 지구본 | 테스트 43개와 프로덕션 빌드 통과 |
| [harulog](https://github.com/JOON0512/harulog) | 할 일·하루 기록 웹앱 | TypeScript와 프로덕션 빌드 통과 |
| [stock-bridge](https://github.com/JOON0512/stock-bridge) | 증권사 API 연결과 로컬 모의 거래 | 오프라인 테스트 50개, 실거래 미검증 |
| [study-tools](https://github.com/JOON0512/study-tools) | 회로 계산·음원·이미지 제작 도구 | 수치 검산 72개, 도구별 실행 조건 명시 |

복원은 2026년 10월의 로컬 보관 자료를 기준으로 진행했습니다. 수업·오픈소스·보드 제공 코드는 출처와 기존 라이선스를 유지합니다. 공개용으로 학번, 개인 환경 설정, 개인 대화와 실제 계좌·서비스 데이터는 제외했습니다. 저장소에 코드가 있다는 사실과 완전한 재현·성능 검증을 마쳤다는 사실은 각각 README에서 구분합니다.

## 이 저장소에 추가한 제출 소스

| 폴더 | 내용 | 확인 범위 |
|---|---|---|
| [advanced-programming](advanced-programming) | C++·템플릿·스레드·MPI·CUDA 5개 실습 | CPU·MPI 검증, CUDA 실행 전 |
| [computer-architecture](computer-architecture) | 2025 Quartus 4개, 2026 RISC-V·부동소수점·예측기·캐시 5개 | 2026 소스 빌드·예제 실행 |
| [network-labs](network-labs) | 공동 실험의 스펙트로그램·LabVIEW VI | 소스 복원, 장비 재검증 전 |
| [signals-and-systems](signals-and-systems) | Fourier 합·위상·샘플링 실험 | 본인 보고서 코드 복원, 합성 데이터 검증 |

2026-10-25에 LearnUs의 기술 과목 18개와 과제 페이지 86개를 조사하고, 소스가 포함된 제출 압축파일 28개를 내려받아 로컬 자료와 대조했습니다. 제출 첨부파일이 없는 항목이나 보고서에만 남아 있는 항목도 있어 학기 전체의 모든 원본 프로젝트가 복원됐다고 주장하지 않습니다. 수업의 공개 제한이 있는 시스템프로그래밍 원본·수정본은 로컬에 보관하고 공개 저장소에는 분석과 별도 구현을 제공합니다.
