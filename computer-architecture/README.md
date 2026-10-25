# Computer architecture

컴퓨터구조 수업의 회로 설계와 RISC-V 실습. 각 폴더는 별도 프로젝트입니다.

| 경로 | 내용 |
| --- | --- |
| `2025/project-1-1`, `project-1-2` | Quartus 회로 설계 |
| `2025/project-2`, `project-3` | Verilog·블록 회로 설계 |
| `2026/assignment-1/kite` | RISC-V 어셈블리 |
| `2026/assignment-2/kite` | RISC-V 함수 호출 |
| `2026/assignment-3/float` | 반정밀도 부동소수점 연산 |
| `2026/assignment-4/kite` | 분기 예측기 |
| `2026/assignment-5/kite` | 캐시와 victim cache |

Kite 폴더에서:

```sh
make
./kite program_code
```

float 폴더에서는 `make && ./float`로 예제를 실행합니다. 2025 프로젝트는 Quartus와 해당 보드 환경이 필요합니다.

Kite는 William J. Song / 연세대학교 Computer Architecture and Systems Lab의 시뮬레이터입니다. 원본 README와 BSD 라이선스는 각 Kite 폴더에 있습니다.
