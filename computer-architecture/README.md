# 컴퓨터구조 실습

본인의 LearnUs 제출본을 연도·과제별로 복원했습니다. 각 폴더는 독립적인 당시 상태이며, 번호가 큰 과제에 앞선 모든 구현이 합쳐져 있다는 의미는 아닙니다.

| 경로 | 내용 |
|---|---|
| 2025/project-1-1, project-1-2 | Quartus 회로 설계 제출본 |
| 2025/project-2, project-3 | Verilog·블록 회로 설계 제출본 |
| 2026/assignment-1/kite | RISC-V 어셈블리 프로그램 |
| 2026/assignment-2/kite | 함수 호출을 사용하는 RISC-V 프로그램 |
| 2026/assignment-3/float | 소프트웨어 반정밀도 부동소수점 연산 |
| 2026/assignment-4/kite | 분기 예측기 실습 |
| 2026/assignment-5/kite | 캐시·victim cache 실습 |

Kite는 William J. Song / 연세대학교 Computer Architecture and Systems Lab의 시뮬레이터입니다. 기반 소스의 README·BSD LICENSE를 각 제출본에 보존했습니다. 기반 시뮬레이터 전체를 개인 창작으로 표시하지 않습니다.

Kite 폴더에서 `make && ./kite program_code`, float 폴더에서 `make && ./float`로 실행합니다. 2026-10-25 Ubuntu 24.04/g++ 13에서 네 Kite 버전과 float를 빌드하고 포함된 예제를 끝까지 실행했습니다. float의 기본 실행은 계산 결과를 출력하는 예제이며 독립 정답을 판정하는 전체 IEEE 754 적합성 테스트는 아닙니다. 분기 예측·캐시의 성능이나 모든 경계 조건이 검증됐다는 의미도 아닙니다.

2025 자료는 Quartus 프로젝트 설정·회로·HDL 소스를 보존하고 생성된 데이터베이스·출력물을 제외했습니다. Quartus 합성, 타이밍 분석, FPGA 보드 실험은 다시 실행하지 않았습니다. 원본 프로젝트가 참조하는 외부 IP나 추가 라이브러리는 해당 환경에서 별도로 필요할 수 있습니다.
