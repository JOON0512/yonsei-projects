# C++ and parallel programming

고급프로그래밍 수업 실습. 각 폴더는 수업 제공 코드와 구현을 포함한 독립 프로젝트입니다.

| 폴더 | 내용 | 빌드 환경 |
| --- | --- | --- |
| [class](class) | 클래스·상속과 도형 계산 | C++, make |
| [template](template) | 템플릿 기반 vector | C++, make |
| [thread](thread) | 병렬 merge sort | C++, pthread |
| [mpi](mpi) | 분산 1D/2D DFT | MPI, mpirun |
| [gpu](gpu) | Mandelbrot 시각화 | CUDA, OpenGL/GLUT |

각 폴더에서 `make`로 빌드합니다. CPU·MPI 테스트는 이 폴더에서 실행합니다.

```sh
python3 tests/run.py
```

테스트는 vector의 복사·삽입·수명, 병렬 정렬과 `std::sort`의 일치, MPI DFT와 기준 계산의 일치를 확인합니다. C++ 검사에는 ASan/UBSan을 사용합니다. CUDA 실행에는 별도 NVIDIA GPU 환경이 필요합니다.
