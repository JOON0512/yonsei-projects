#include <vector>
#include <complex>
#include <cmath>
#include <cassert>
#include <iostream>
#include <mpi.h>
#include "dft.h"
int main(int argc,char **argv) {
    MPI_Init(&argc,&argv);
    int rank,ranks;MPI_Comm_rank(MPI_COMM_WORLD,&rank);MPI_Comm_size(MPI_COMM_WORLD,&ranks);
    const unsigned n=4;
    std::vector<std::complex<double>> input(n*n),actual;
    for(unsigned i=0;i<n*n;i++) input[i]={double(int(i%5)-2),double(i%3)};
    actual=input;dft2d(actual.data(),n,n,ranks,rank);
    for(unsigned v=0;v<n;v++) for(unsigned u=0;u<n;u++) {
        std::complex<double> expected=0;
        for(unsigned y=0;y<n;y++) for(unsigned x=0;x<n;x++) {
            double angle=-2*std::acos(-1)*double(u*x+v*y)/n;
            expected+=input[y*n+x]*std::polar(1.0,angle);
        }
        if(std::abs(actual[v*n+u]-expected)>1e-8) MPI_Abort(MPI_COMM_WORLD,2);
    }
    if(!rank) std::cout << "PASS: independent complex DFT reference, ranks=" << ranks << '\n';
    MPI_Finalize();
}
